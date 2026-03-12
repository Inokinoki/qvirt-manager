/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 Inoki <veyx.shaw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "SpiceViewer.h"
#include <QMessageBox>
#include <QPixmap>
#include <QApplication>
#include <QKeyEvent>

#ifdef HAVE_SPICE
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#endif

namespace QVirt {

SpiceViewer::SpiceViewer(QWidget *parent)
    : Viewer(parent)
{
    setupUI();
}

SpiceViewer::~SpiceViewer()
{
#ifdef HAVE_SPICE
    cleanupSpice();
#endif
}

void SpiceViewer::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);

#ifdef HAVE_SPICE
    // Try to embed Spice widget
    embedSpiceWidget();
    if (m_spiceWidget) {
        // Spice successfully initialized
        return;
    }
#endif

    // Fallback to external viewer UI
    setupExternalViewerUI();
}

void SpiceViewer::setupExternalViewerUI()
{
    m_displayLabel = new QLabel(
        "<html><body style='text-align:center; padding: 50px;'>"
        "<h2>SPICE Console Viewer</h2>"
        "<p>SPICE support requires external dependencies:</p>"
        "<ul>"
        "<li><b>spice-gtk</b> - SPICE client library (recommended)</li>"
        "<li><b>spice-client-gtk</b> - GTK SPICE widget</li>"
        "</ul>"
        "<p><b>Install on Ubuntu/Debian:</b></p>"
        "<pre>sudo apt install libspice-client-gtk-3.0-dev libspice-client-glib-2.0-dev</pre>"
        "<p><b>Install on Fedora/RHEL:</b></p>"
        "<pre>sudo dnf install spice-gtk3-devel</pre>"
        "<p>Then rebuild QVirt-Manager with SPICE support.</p>"
        "<p><i>Meanwhile, use external SPICE viewers:</i></p>"
        "<ul>"
        "<li><b>remote-viewer</b> - Virt-viewer SPICE client</li>"
        "<li><b>spicy</b> - SPICE GTK test client</li>"
        "</ul>"
        "</body></html>"
    );
    m_displayLabel->setWordWrap(true);
    m_displayLabel->setAlignment(Qt::AlignCenter);
    m_displayLabel->setOpenExternalLinks(true);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidget(m_displayLabel);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setAlignment(Qt::AlignCenter);

    m_mainLayout->addWidget(m_scrollArea);

    m_connectBtn = new QPushButton("Launch External SPICE Viewer");
    m_mainLayout->addWidget(m_connectBtn, 0, Qt::AlignCenter);

    connect(m_connectBtn, &QPushButton::clicked, this, [this]() {
        if (!m_host.isEmpty() && m_port > 0) {
            QString spiceCmd;
            if (m_tlsPort > 0) {
                spiceCmd = QString("remote-viewer spice://%1:%2?tls-port=%3")
                    .arg(m_host).arg(m_port).arg(m_tlsPort);
            } else {
                spiceCmd = QString("remote-viewer spice://%1:%2").arg(m_host).arg(m_port);
            }
            QMessageBox::information(this, "SPICE Connection",
                QString("To connect manually, run:\n\n%1\n\n"
                    "Or use your preferred SPICE client.\n\n"
                    "Tip: Install spice-gtk and rebuild for integrated support.").arg(spiceCmd));
        } else {
            QMessageBox::information(this, "SPICE Viewer",
                "No SPICE connection configured.\n\n"
                "Please start the VM and configure SPICE display first.");
        }
    });
}

#ifdef HAVE_SPICE
void SpiceViewer::embedSpiceWidget()
{
    // Initialize GTK if not already done
    if (!gtk_init_check(nullptr, nullptr)) {
        return;
    }

    // Create Spice session
    m_spiceSession = spice_session_new();
    if (!m_spiceSession) {
        return;
    }

    // Configure session
    g_object_set(m_spiceSession,
                 "scaling", m_scalingEnabled,
                 "resize-guest", TRUE,
                 "read-only", FALSE,
                 nullptr);

    // Create Spice display
    m_spiceDisplay = spice_display_new(m_spiceSession, 0);
    if (!m_spiceDisplay) {
        return;
    }

    // Configure display
    g_object_set(m_spiceDisplay,
                 "scaling", m_scalingEnabled,
                 "grab-keyboard", TRUE,
                 "grab-mouse", TRUE,
                 nullptr);

    // Create the GTK Spice widget
    m_spiceWidget = GTK_WIDGET(m_spiceDisplay);
    gtk_widget_show_all(m_spiceWidget);

    // Create container for the widget
    m_gtkContainer = new QWidget(this);
    auto *containerLayout = new QVBoxLayout(m_gtkContainer);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->addWidget(m_scrollArea);

    // Connect Spice signals
    g_signal_connect(m_spiceSession, "channel-event",
        G_CALLBACK(+[](SpiceSession *, SpiceChannel *, SpiceChannelEvent event, 
                      SpiceViewer *self) {
            if (event == SPICE_CHANNEL_OPENED) {
                QMetaObject::invokeMethod(self, "onSpiceConnected", Qt::QueuedConnection);
            } else if (event == SPICE_CHANNEL_CLOSED) {
                QMetaObject::invokeMethod(self, "onSpiceDisconnected", Qt::QueuedConnection);
            }
        }), this);

    g_signal_connect(m_spiceSession, "session-error",
        G_CALLBACK(+[](SpiceSession *, GError *error, SpiceViewer *self) {
            QMetaObject::invokeMethod(self, "onSpiceError", Qt::QueuedConnection,
                Q_ARG(QString, QString::fromUtf8(error->message)));
        }), this);

    // USB redirection signals (if enabled)
    if (m_usbRedirectionEnabled) {
        g_signal_connect(m_spiceSession, "usb-device-added",
            G_CALLBACK(+[](SpiceSession *, SpiceUsbDevice *device, SpiceViewer *self) {
                QString deviceName = QString::fromUtf8(spice_usb_device_get_device_name(device));
                QMetaObject::invokeMethod(self, "onUSBDeviceAdded", Qt::QueuedConnection,
                    Q_ARG(QString, deviceName));
            }), this);

        g_signal_connect(m_spiceSession, "usb-device-removed",
            G_CALLBACK(+[](SpiceSession *, SpiceUsbDevice *device, SpiceViewer *self) {
                QString deviceName = QString::fromUtf8(spice_usb_device_get_device_name(device));
                QMetaObject::invokeMethod(self, "onUSBDeviceRemoved", Qt::QueuedConnection,
                    Q_ARG(QString, deviceName));
            }), this);
    }
}

void SpiceViewer::cleanupSpice()
{
    if (m_spiceDisplay) {
        g_object_unref(m_spiceDisplay);
        m_spiceDisplay = nullptr;
    }
    if (m_spiceSession) {
        g_object_unref(m_spiceSession);
        m_spiceSession = nullptr;
    }
    if (m_spiceWidget) {
        gtk_widget_destroy(m_spiceWidget);
        m_spiceWidget = nullptr;
    }
}
#endif

bool SpiceViewer::connectToHost(const QString &host, int port)
{
    m_host = host;
    m_port = port;

#ifdef HAVE_SPICE
    if (m_spiceSession) {
        // Set the host and port
        g_object_set(m_spiceSession, "host", host.toUtf8().constData(), nullptr);
        g_object_set(m_spiceSession, "port", QString::number(port).toUtf8().constData(), nullptr);

        // Set TLS port if available
        if (m_tlsPort > 0) {
            g_object_set(m_spiceSession, "tls-port", QString::number(m_tlsPort).toUtf8().constData(), nullptr);
        }

        // Set password if provided
        if (!m_password.isEmpty()) {
            g_object_set(m_spiceSession, "password", m_password.toUtf8().constData(), nullptr);
        }

        // Connect to the session
        spice_session_connect(m_spiceSession);
        return true;
    }
#else
    Q_UNUSED(host);
    Q_UNUSED(port);
#endif

    // Connection failed or Spice not available
    emit errorOccurred("SPICE connection failed. Check host, port, and that VM has SPICE enabled.");
    return false;
}

void SpiceViewer::disconnect()
{
#ifdef HAVE_SPICE
    if (m_spiceSession) {
        spice_session_disconnect(m_spiceSession);
    }
#endif

    m_connected = false;
    emit connectedChanged(m_connected);
}

void SpiceViewer::sendKeys(const QString &keys)
{
    if (!m_connected) {
        return;
    }

#ifdef HAVE_SPICE
    if (m_spiceDisplay) {
        QStringList keyList = keys.split("+");
        for (const QString &key : keyList) {
            // Send key press
        }
        for (auto it = keyList.rbegin(); it != keyList.rend(); ++it) {
            // Send key release
        }
    }
#else
    Q_UNUSED(keys);
#endif
}

QPixmap SpiceViewer::takeScreenshot()
{
#ifdef HAVE_SPICE
    if (m_spiceDisplay && m_connected) {
        GdkPixbuf *pixbuf = spice_display_get_pixbuf(m_spiceDisplay);
        if (pixbuf) {
            QPixmap pixmap = QPixmap::fromImage(
                QImage(gdk_pixbuf_read_pixels(pixbuf),
                       gdk_pixbuf_get_width(pixbuf),
                       gdk_pixbuf_get_height(pixbuf),
                       gdk_pixbuf_get_rowstride(pixbuf),
                       QImage::Format_RGB32)
            );
            g_object_unref(pixbuf);
            return pixmap;
        }
    }
#endif

    return QPixmap();
}

void SpiceViewer::toggleFullscreen()
{
#ifdef HAVE_SPICE
    if (m_spiceDisplay) {
        gboolean current = g_object_get_boolean(G_OBJECT(m_spiceDisplay), "allow-fullscreen");
        g_object_set(m_spiceDisplay, "allow-fullscreen", !current, nullptr);
    }
#endif

    QWidget *window = windowHandle() ? QWidget::window() : this;
    if (window->isFullScreen()) {
        window->showNormal();
    } else {
        window->showFullScreen();
    }
}

void SpiceViewer::setTLSPort(int port)
{
    m_tlsPort = port;
}

void SpiceViewer::setPassword(const QString &password)
{
    m_password = password;
}

void SpiceViewer::setScalingEnabled(bool enabled)
{
    m_scalingEnabled = enabled;

#ifdef HAVE_SPICE
    if (m_spiceSession) {
        g_object_set(m_spiceSession, "scaling", enabled, nullptr);
    }
    if (m_spiceDisplay) {
        g_object_set(m_spiceDisplay, "scaling", enabled, nullptr);
    }
#endif
}

void SpiceViewer::setUSBRedirectionEnabled(bool enabled)
{
    m_usbRedirectionEnabled = enabled;

#ifdef HAVE_SPICE
    if (m_spiceSession) {
        g_object_set(m_spiceSession, "enable-usbredir", enabled, nullptr);
    }
#endif
}

void SpiceViewer::resizeEvent(QResizeEvent *event)
{
    Viewer::resizeEvent(event);

#ifdef HAVE_SPICE
    if (m_spiceDisplay && m_scalingEnabled) {
        g_object_set(m_spiceDisplay, "scaling", true, nullptr);
    }
#endif
}

void SpiceViewer::keyPressEvent(QKeyEvent *event)
{
    if (m_connected) {
        sendKeyEvent(event, true);
    } else {
        Viewer::keyPressEvent(event);
    }
}

void SpiceViewer::keyReleaseEvent(QKeyEvent *event)
{
    if (m_connected) {
        sendKeyEvent(event, false);
    } else {
        Viewer::keyReleaseEvent(event);
    }
}

void SpiceViewer::sendKeyEvent(QKeyEvent *event, bool isPress)
{
#ifdef HAVE_SPICE
    if (m_spiceDisplay) {
        guint keysym = event->nativeVirtualKey();
        if (keysym) {
            // spice_inputs_send_key(m_spiceDisplay, keysym, isPress ? TRUE : FALSE);
        }
    }
#else
    Q_UNUSED(event);
    Q_UNUSED(isPress);
#endif
}

void SpiceViewer::onSpiceConnected()
{
    m_connected = true;
    emit connectedChanged(true);
}

void SpiceViewer::onSpiceDisconnected()
{
    m_connected = false;
    emit connectedChanged(false);
    emit connectionLost();
}

void SpiceViewer::onSpiceError(const QString &error)
{
    m_connected = false;
    emit connectedChanged(false);
    emit errorOccurred(error);
    
    QMessageBox::warning(this, "SPICE Error", error);
}

void SpiceViewer::onUSBDeviceAdded(const QString &device)
{
    emit usbDeviceConnected(device);
}

void SpiceViewer::onUSBDeviceRemoved(const QString &device)
{
    emit usbDeviceDisconnected(device);
}

} // namespace QVirt
