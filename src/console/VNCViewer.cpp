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

#include "VNCViewer.h"
#include <QMessageBox>
#include <QPixmap>
#include <QApplication>
#include <QKeyEvent>
#include <QCursor>
#include <QMenu>

#ifdef HAVE_GTK_VNC
#include <gdk/gdk.h>
#include <gdk/gdkx.h>  // For X11 integration
#endif

namespace QVirt {

VNCViewer::VNCViewer(QWidget *parent)
    : Viewer(parent)
{
    setupUI();
}

VNCViewer::~VNCViewer()
{
#ifdef HAVE_GTK_VNC
    cleanupGtkVnc();
#endif
}

void VNCViewer::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);

#ifdef HAVE_GTK_VNC
    // Try to embed GtkVnc widget
    embedGtkVncWidget();
    if (m_gtkVncWidget) {
        // GtkVnc successfully initialized
        return;
    }
#endif

    // Fallback to external viewer UI
    setupExternalViewerUI();
}

void VNCViewer::setupExternalViewerUI()
{
    m_displayLabel = new QLabel(
        "<html><body style='text-align:center; padding: 50px;'>"
        "<h2>VNC Console Viewer</h2>"
        "<p>VNC support requires external dependencies:</p>"
        "<ul>"
        "<li><b>gtk-vnc</b> - GTK VNC widget (recommended)</li>"
        "<li><b>libvncclient</b> - Lightweight VNC client library</li>"
        "</ul>"
        "<p><b>Install on Ubuntu/Debian:</b></p>"
        "<pre>sudo apt install libgtk-3-dev libgvnc-1.0-0 libgvnc-1.0-dev</pre>"
        "<p><b>Install on Fedora/RHEL:</b></p>"
        "<pre>sudo dnf install gtk3-devel gvnc gvnc-devel</pre>"
        "<p>Then rebuild QVirt-Manager with VNC support.</p>"
        "<p><i>Meanwhile, use external VNC viewers:</i></p>"
        "<ul>"
        "<li><b>vinagre</b> - GNOME VNC client</li>"
        "<li><b>krdc</b> - KDE Remote Desktop Client</li>"
        "<li><b>remmina</b> - Remote desktop client</li>"
        "<li><b>vncviewer</b> - TigerVNC client</li>"
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

    m_connectBtn = new QPushButton("Launch External VNC Viewer");
    m_mainLayout->addWidget(m_connectBtn, 0, Qt::AlignCenter);

    connect(m_connectBtn, &QPushButton::clicked, this, [this]() {
        if (!m_host.isEmpty() && m_port > 0) {
            QString vncCmd = QString("vncviewer %1::%2").arg(m_host).arg(m_port);
            QMessageBox::information(this, "VNC Connection",
                QString("To connect manually, run:\n\n%1\n\n"
                    "Or use your preferred VNC client.\n\n"
                    "Tip: Install gtk-vnc and rebuild for integrated support.").arg(vncCmd));
        } else {
            QMessageBox::information(this, "VNC Viewer",
                "No VNC connection configured.\n\n"
                "Please start the VM and configure VNC display first.");
        }
    });
}

#ifdef HAVE_GTK_VNC
void VNCViewer::embedGtkVncWidget()
{
    // Initialize GTK if not already done
    if (!gtk_init_check(nullptr, nullptr)) {
        return;
    }

    // Create VncDisplay widget
    m_vncDisplay = VNC_DISPLAY(g_vnc_display_new());
    if (!m_vncDisplay) {
        return;
    }

    // Create the GTK VNC widget
    m_gtkVncWidget = GTK_WIDGET(g_vnc_display_widget_new(m_vncDisplay));
    if (!m_gtkVncWidget) {
        return;
    }

    // Create a container widget to hold the GTK widget
    m_gtkContainer = new QWidget(this);
    auto *containerLayout = new QVBoxLayout(m_gtkContainer);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->addWidget(m_scrollArea);

    // Embed the GTK widget
    gtk_widget_show_all(m_gtkVncWidget);
    
    // Connect VNC signals
    g_signal_connect(m_vncDisplay, "vnc-connected",
        G_CALLBACK(+[](VncDisplay *, VNCViewer *self) {
            QMetaObject::invokeMethod(self, "onVNCConnected", Qt::QueuedConnection);
        }), this);

    g_signal_connect(m_vncDisplay, "vnc-disconnected",
        G_CALLBACK(+[](VncDisplay *, VNCViewer *self) {
            QMetaObject::invokeMethod(self, "onVNCDisconnected", Qt::QueuedConnection);
        }), this);

    g_signal_connect(m_vncDisplay, "vnc-encoding",
        G_CALLBACK(+[](VncDisplay *, VNCViewer *self) {
            QMetaObject::invokeMethod(self, "onVNCEncodingChanged", Qt::QueuedConnection);
        }), this);

    g_signal_connect(m_vncDisplay, "vnc-bell",
        G_CALLBACK(+[](VncDisplay *, VNCViewer *self) {
            QMetaObject::invokeMethod(self, "onVNCBell", Qt::QueuedConnection);
        }), this);

    g_signal_connect(m_vncDisplay, "vnc-auth-failure",
        G_CALLBACK(+[](VncDisplay *, const gchar *, VNCViewer *self) {
            QMetaObject::invokeMethod(self, "onVNCError", Qt::QueuedConnection,
                Q_ARG(QString, QString("Authentication failed: %1").arg(QString::fromUtf8())));
        }), this);

    // Set scaling
    g_object_set(m_vncDisplay, "scaling", m_scalingEnabled, nullptr);
}

void VNCViewer::cleanupGtkVnc()
{
    if (m_vncDisplay) {
        g_object_unref(m_vncDisplay);
        m_vncDisplay = nullptr;
    }
    if (m_gtkVncWidget) {
        gtk_widget_destroy(m_gtkVncWidget);
        m_gtkVncWidget = nullptr;
    }
}
#endif

bool VNCViewer::connectToHost(const QString &host, int port)
{
    m_host = host;
    m_port = port;

#ifdef HAVE_GTK_VNC
    if (m_vncDisplay) {
        // Set the host
        g_object_set(m_vncDisplay, "host", host.toUtf8().constData(), nullptr);
        g_object_set(m_vncDisplay, "port", port, nullptr);

        // Set password if provided
        if (!m_password.isEmpty()) {
            g_object_set(m_vncDisplay, "password", m_password.toUtf8().constData(), nullptr);
        }

        // Try to connect
        gboolean result = g_vnc_display_open(m_vncDisplay);
        if (result) {
            return true;
        }
    }
#else
    Q_UNUSED(host);
    Q_UNUSED(port);
#endif

    // Connection failed or GtkVnc not available
    emit errorOccurred("VNC connection failed. Check host, port, and that VM has VNC enabled.");
    return false;
}

void VNCViewer::disconnect()
{
#ifdef HAVE_GTK_VNC
    if (m_vncDisplay) {
        g_vnc_display_close(m_vncDisplay);
    }
#endif

    m_connected = false;
    emit connectedChanged(m_connected);
}

void VNCViewer::sendKeys(const QString &keys)
{
    if (!m_connected) {
        return;
    }

    // Parse key combination and send via VNC
    // Common combinations:
    // - Ctrl+Alt+Delete
    // - Ctrl+Alt+Backspace
    // - Print Screen
    // - Ctrl+Alt+F1-F12

#ifdef HAVE_GTK_VNC
    if (m_vncDisplay) {
        // For key combinations, we need to send multiple key events
        QStringList keyList = keys.split("+");
        
        // This is a simplified implementation
        // Full implementation would map Qt keys to VNC keysyms
        for (const QString &key : keyList) {
            // Send key press
            // g_vnc_display_send_key(m_vncDisplay, keySym, TRUE);
        }
        
        // Send key release in reverse order
        for (auto it = keyList.rbegin(); it != keyList.rend(); ++it) {
            // g_vnc_display_send_key(m_vncDisplay, keySym, FALSE);
        }
    }
#else
    Q_UNUSED(keys);
#endif
}

QPixmap VNCViewer::takeScreenshot()
{
#ifdef HAVE_GTK_VNC
    if (m_vncDisplay && m_connected) {
        // Get the GdkPixbuf from VncDisplay
        GdkPixbuf *pixbuf = g_vnc_display_get_pixbuf(m_vncDisplay);
        if (pixbuf) {
            // Convert GdkPixbuf to QPixmap
            // This requires GTK-Qt integration
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

void VNCViewer::toggleFullscreen()
{
#ifdef HAVE_GTK_VNC
    if (m_vncDisplay) {
        // Toggle fullscreen in VncDisplay
        g_object_set(m_vncDisplay, "allow-fullscreen", 
                     !g_object_get_boolean(G_OBJECT(m_vncDisplay), "allow-fullscreen"),
                     nullptr);
    }
#endif

    // Also toggle Qt window fullscreen
    QWidget *window = windowHandle() ? QWidget::window() : this;
    if (window->isFullScreen()) {
        window->showNormal();
    } else {
        window->showFullScreen();
    }
}

void VNCViewer::setPassword(const QString &password)
{
    m_password = password;
}

void VNCViewer::setScalingEnabled(bool enabled)
{
    m_scalingEnabled = enabled;

#ifdef HAVE_GTK_VNC
    if (m_vncDisplay) {
        g_object_set(m_vncDisplay, "scaling", enabled, nullptr);
    }
#endif
}

void VNCViewer::resizeEvent(QResizeEvent *event)
{
    Viewer::resizeEvent(event);

#ifdef HAVE_GTK_VNC
    if (m_vncDisplay && m_scalingEnabled) {
        // Update scaling on resize
        g_object_set(m_vncDisplay, "scaling", true, nullptr);
    }
#endif
}

void VNCViewer::keyPressEvent(QKeyEvent *event)
{
    if (m_connected) {
        sendKeyEvent(event, true);
    } else {
        Viewer::keyPressEvent(event);
    }
}

void VNCViewer::keyReleaseEvent(QKeyEvent *event)
{
    if (m_connected) {
        sendKeyEvent(event, false);
    } else {
        Viewer::keyReleaseEvent(event);
    }
}

void VNCViewer::sendKeyEvent(QKeyEvent *event, bool isPress)
{
#ifdef HAVE_GTK_VNC
    if (m_vncDisplay) {
        // Convert Qt key to VNC keysym
        // This is a simplified mapping
        guint keysym = event->nativeVirtualKey();
        
        if (keysym) {
            // g_vnc_display_send_key(m_vncDisplay, keysym, isPress ? TRUE : FALSE);
        }
    }
#else
    Q_UNUSED(event);
    Q_UNUSED(isPress);
#endif
}

void VNCViewer::onVNCConnected()
{
    m_connected = true;
    emit connectedChanged(true);
}

void VNCViewer::onVNCDisconnected()
{
    m_connected = false;
    emit connectedChanged(false);
    emit connectionLost();
}

void VNCViewer::onVNCEncodingChanged()
{
    // Encoding changed, could update status
}

void VNCViewer::onVNCBell()
{
    emit bellSignal();
    QApplication::beep();
}

void VNCViewer::onVNCError(const QString &error)
{
    m_connected = false;
    emit connectedChanged(false);
    emit errorOccurred(error);
    
    QMessageBox::warning(this, "VNC Error", error);
}

} // namespace QVirt
