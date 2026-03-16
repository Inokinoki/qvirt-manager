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

#ifndef QVIRT_CONSOLE_VNCWIDGET_H
#define QVIRT_CONSOLE_VNCWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPointer>

#ifdef LIBVNCCLIENT_FOUND
#include <rfb/rfbclient.h>
#endif

namespace QVirt {

/**
 * @brief Integrated VNC viewer widget
 *
 * Provides VNC console functionality without external dependencies
 */
class VNCWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VNCWidget(QWidget *parent = nullptr);
    ~VNCWidget() override;

    QString host() const { return m_host; }
    int port() const { return m_port; }
    bool isConnected() const { return m_connected; }

    void connectToServer(const QString &host, int port, const QString &password = QString());
    void disconnect();

    void sendKeyEvent(int key, bool pressed);
    void sendPointerEvent(int x, int y, int buttonMask);
    void sendClipboardText(const QString &text);

    QImage framebuffer() const { return m_framebuffer; }
    void setScalingEnabled(bool enabled);
    bool isScalingEnabled() const { return m_scalingEnabled; }

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &error);
    void bell();
    void clipboardReceived(const QString &text);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private slots:
    void updateFramebuffer();
    void onConnectionFailed();

private:
    void setupUI();
    void updateScaling();
    int keyToRFBCode(int qtKey);

    QLabel *m_displayLabel;
    QString m_host;
    int m_port;
    QString m_password;
    bool m_connected;
    bool m_scalingEnabled;
    QImage m_framebuffer;
    QImage m_scaledFramebuffer;

#ifdef LIBVNCCLIENT_FOUND
    rfbClient *m_client;
#endif
};

} // namespace QVirt

#endif // QVIRT_CONSOLE_VNCWIDGET_H
