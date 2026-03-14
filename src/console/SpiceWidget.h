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

#ifndef QVIRT_CONSOLE_SPICEWIDGET_H
#define QVIRT_CONSOLE_SPICEWIDGET_H

#include <QWidget>
#include <QString>
#include <QPointer>

#ifdef SPICE_CLIENT_GTK_FOUND
#include <spice-client-gtk.h>
#endif

namespace QVirt {

/**
 * @brief Integrated SPICE viewer widget
 *
 * Provides SPICE console functionality with USB redirection support
 */
class SpiceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpiceWidget(QWidget *parent = nullptr);
    ~SpiceWidget() override;

    QString host() const { return m_host; }
    int port() const { return m_port; }
    QString tlsPort() const { return m_tlsPort; }
    QString password() const { return m_password; }
    bool isConnected() const { return m_connected; }

    void connectToServer(const QString &host, int port, const QString &password = QString());
    void connectToServerTLS(const QString &host, int tlsPort, const QString &password = QString());
    void disconnect();

    void sendKeyEvent(int key, bool pressed);
    void sendClipboardText(const QString &text);

    bool usbRedirectionEnabled() const { return m_usbRedirectionEnabled; }
    void setUsbRedirectionEnabled(bool enabled);

    void grabKeyboard();
    void ungrabKeyboard();
    bool isKeyboardGrabbed() const { return m_keyboardGrabbed; }

    void setScalingEnabled(bool enabled);
    bool isScalingEnabled() const { return m_scalingEnabled; }

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &error);
    void usbDeviceConnected(const QString &device);
    void usbDeviceDisconnected(const QString &device);
    void clipboardReceived(const QString &text);

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private slots:
    void onChannelEvent(int channel, int event);
    void onUsbDeviceAdded(const QString &device);
    void onUsbDeviceRemoved(const QString &device);

private:
    void setupUI();
    void updateGrabState();

    QString m_host;
    int m_port;
    QString m_tlsPort;
    QString m_password;
    bool m_connected;
    bool m_usbRedirectionEnabled;
    bool m_keyboardGrabbed;
    bool m_scalingEnabled;

#ifdef SPICE_CLIENT_GTK_FOUND
    SpiceSession *m_session;
    SpiceDisplay *m_display;
    QWidget *m_container;
#endif
};

} // namespace QVirt

#endif // QVIRT_CONSOLE_SPICEWIDGET_H
