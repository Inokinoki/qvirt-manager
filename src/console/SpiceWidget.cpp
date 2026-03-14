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

#include "SpiceWidget.h"
#include <QVBoxLayout>
#include <QLabel>

#ifdef SPICE_CLIENT_GTK_FOUND
#include <spice-client-gtk.h>
#endif

namespace QVirt {

SpiceWidget::SpiceWidget(QWidget *parent)
    : QWidget(parent)
    , m_port(5900)
    , m_connected(false)
    , m_usbRedirectionEnabled(true)
    , m_keyboardGrabbed(false)
    , m_scalingEnabled(true)
#ifdef SPICE_CLIENT_GTK_FOUND
    , m_session(nullptr)
    , m_display(nullptr)
    , m_container(nullptr)
#endif
{
    setupUI();
}

SpiceWidget::~SpiceWidget()
{
    disconnect();
}

void SpiceWidget::setupUI()
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

#ifdef SPICE_CLIENT_GTK_FOUND
    m_container = new QWidget(this);
    m_container->setMinimumSize(640, 480);
    m_container->setStyleSheet("QWidget { background-color: #1a1a1a; }");
    layout->addWidget(m_container);
#else
    auto label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumSize(640, 480);
    label->setStyleSheet("QLabel { background-color: #1a1a1a; color: #888; }");
    label->setText(tr("SPICE Console\n\nSPICE support not available\n\nInstall spice-client-gtk to enable SPICE console"));
    layout->addWidget(label);
#endif

    setFocusPolicy(Qt::StrongFocus);
}

void SpiceWidget::connectToServer(const QString &host, int port, const QString &password)
{
    m_host = host;
    m_port = port;
    m_password = password;

#ifdef SPICE_CLIENT_GTK_FOUND
    m_session = spice_session_new();
    g_object_set(m_session,
                 "host", m_host.toUtf8().data(),
                 "port", QString::number(m_port).toUtf8().data(),
                 "password", m_password.toUtf8().data(),
                 "read-only", FALSE,
                 nullptr);

    if (m_usbRedirectionEnabled) {
        g_object_set(m_session, "auto-usbredir", TRUE, nullptr);
    }

    m_display = spice_display_new(m_session, 0);
    g_object_set(m_display,
                 "scaling", m_scalingEnabled,
                 "resize-guest", TRUE,
                 nullptr);

    // Connect signals
    g_signal_connect(m_session, "channel-event",
                     G_CALLBACK([](SpiceSession *, gint channel, gint event, gpointer userData) {
                         SpiceWidget *self = static_cast<SpiceWidget*>(userData);
                         QMetaObject::invokeMethod(self, "onChannelEvent",
                                                   Qt::QueuedConnection,
                                                   Q_ARG(int, channel),
                                                   Q_ARG(int, event));
                     }), this);

    g_signal_connect(m_display, "usb-redir",
                     G_CALLBACK([](SpiceDisplay *, gpointer, gboolean added, gpointer userData) {
                         SpiceWidget *self = static_cast<SpiceWidget*>(userData);
                         if (added) {
                             QMetaObject::invokeMethod(self, "onUsbDeviceAdded",
                                                       Qt::QueuedConnection,
                                                       Q_ARG(QString, "USB Device"));
                         } else {
                             QMetaObject::invokeMethod(self, "onUsbDeviceRemoved",
                                                       Qt::QueuedConnection,
                                                       Q_ARG(QString, "USB Device"));
                         }
                     }), this);

    // Embed display in Qt widget
    GtkWidget *gtkWidget = GTK_WIDGET(m_display);
    gtk_widget_show(gtkWidget);

    m_connected = true;
    emit connected();
#else
    Q_UNUSED(password);
    emit connectionError(tr("SPICE support not available - spice-client-gtk not found"));
#endif
}

void SpiceWidget::connectToServerTLS(const QString &host, int tlsPort, const QString &password)
{
    m_host = host;
    m_tlsPort = QString::number(tlsPort);
    m_password = password;

#ifdef SPICE_CLIENT_GTK_FOUND
    m_session = spice_session_new();
    g_object_set(m_session,
                 "host", m_host.toUtf8().data(),
                 "tls-port", m_tlsPort.toUtf8().data(),
                 "password", m_password.toUtf8().data(),
                 "tls-ciphers", "default",
                 nullptr);

    m_display = spice_display_new(m_session, 0);
    m_connected = true;
    emit connected();
#else
    Q_UNUSED(password);
    emit connectionError(tr("SPICE support not available"));
#endif
}

void SpiceWidget::disconnect()
{
#ifdef SPICE_CLIENT_GTK_FOUND
    if (m_display) {
        gtk_widget_destroy(GTK_WIDGET(m_display));
        m_display = nullptr;
    }
    if (m_session) {
        g_object_unref(m_session);
        m_session = nullptr;
    }
#endif
    m_connected = false;
    m_keyboardGrabbed = false;
    emit disconnected();
}

void SpiceWidget::sendKeyEvent(int key, bool pressed)
{
#ifdef SPICE_CLIENT_GTK_FOUND
    if (m_display && m_connected) {
        // Send key event to SPICE display
        Q_UNUSED(key);
        Q_UNUSED(pressed);
    }
#else
    Q_UNUSED(key);
    Q_UNUSED(pressed);
#endif
}

void SpiceWidget::sendClipboardText(const QString &text)
{
#ifdef SPICE_CLIENT_GTK_FOUND
    if (m_session && m_connected) {
        SpiceClipboard *clipboard = spice_session_get_clipboard(m_session);
        if (clipboard) {
            spice_clipboard_set_text(clipboard, text.toUtf8().data(), text.length());
        }
    }
#else
    Q_UNUSED(text);
#endif
}

void SpiceWidget::setUsbRedirectionEnabled(bool enabled)
{
    m_usbRedirectionEnabled = enabled;
#ifdef SPICE_CLIENT_GTK_FOUND
    if (m_session) {
        g_object_set(m_session, "auto-usbredir", enabled ? TRUE : FALSE, nullptr);
    }
#endif
}

void SpiceWidget::grabKeyboard()
{
#ifdef SPICE_CLIENT_GTK_FOUND
    if (m_display) {
        spice_display_keyboard_grab(m_display);
        m_keyboardGrabbed = true;
        updateGrabState();
    }
#endif
}

void SpiceWidget::ungrabKeyboard()
{
#ifdef SPICE_CLIENT_GTK_FOUND
    if (m_display) {
        spice_display_keyboard_ungrab(m_display);
        m_keyboardGrabbed = false;
        updateGrabState();
    }
#endif
}

void SpiceWidget::setScalingEnabled(bool enabled)
{
    m_scalingEnabled = enabled;
#ifdef SPICE_CLIENT_GTK_FOUND
    if (m_display) {
        g_object_set(m_display, "scaling", enabled, nullptr);
    }
#endif
}

void SpiceWidget::focusInEvent(QFocusEvent *event)
{
    QWidget::focusInEvent(event);
    if (m_connected) {
        grabKeyboard();
    }
}

void SpiceWidget::focusOutEvent(QFocusEvent *event)
{
    QWidget::focusOutEvent(event);
    ungrabKeyboard();
}

void SpiceWidget::onChannelEvent(int channel, int event)
{
    Q_UNUSED(channel);
    Q_UNUSED(event);
    // Handle channel events
}

void SpiceWidget::onUsbDeviceAdded(const QString &device)
{
    emit usbDeviceConnected(device);
}

void SpiceWidget::onUsbDeviceRemoved(const QString &device)
{
    emit usbDeviceDisconnected(device);
}

void SpiceWidget::updateGrabState()
{
    // Update UI to show keyboard grab state
}

} // namespace QVirt
