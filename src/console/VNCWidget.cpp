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

#include "VNCWidget.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QKeyEvent>

#ifdef LIBVNCCLIENT_FOUND
#include <rfb/rfbclient.h>
#endif

namespace QVirt {

VNCWidget::VNCWidget(QWidget *parent)
    : QWidget(parent)
    , m_displayLabel(nullptr)
    , m_port(5900)
    , m_connected(false)
    , m_scalingEnabled(true)
#ifdef LIBVNCCLIENT_FOUND
    , m_client(nullptr)
#endif
{
    setupUI();
}

VNCWidget::~VNCWidget()
{
    disconnect();
}

void VNCWidget::setupUI()
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_displayLabel = new QLabel(this);
    m_displayLabel->setAlignment(Qt::AlignCenter);
    m_displayLabel->setMinimumSize(640, 480);
    m_displayLabel->setStyleSheet("QLabel { background-color: #1a1a1a; color: #888; }");
    m_displayLabel->setText(tr("VNC Console\n\nClick 'Connect' to start the VNC session"));
    layout->addWidget(m_displayLabel);

    setFocusPolicy(Qt::StrongFocus);
}

void VNCWidget::connectToServer(const QString &host, int port, const QString &password)
{
    m_host = host;
    m_port = port;
    m_password = password;

#ifdef LIBVNCCLIENT_FOUND
    m_client = rfbGetClient(8, 3, 4);
    if (!m_client) {
        emit connectionError(tr("Failed to initialize VNC client"));
        return;
    }

    m_client->frameBuffer = m_host.toUtf8().data();
    m_client->serverPort = m_port;
    m_client->clientData = this;

    if (!rfbInitClient(m_client, nullptr, nullptr)) {
        emit connectionError(tr("Failed to connect to VNC server"));
        m_client = nullptr;
        return;
    }

    m_connected = true;
    emit connected();
#else
    Q_UNUSED(password);
    emit connectionError(tr("VNC support not available - libvncclient not found"));
#endif
}

void VNCWidget::disconnect()
{
#ifdef LIBVNCCLIENT_FOUND
    if (m_client) {
        rfbClientCleanup(m_client);
        m_client = nullptr;
    }
#endif
    m_connected = false;
    m_framebuffer = QImage();
    m_scaledFramebuffer = QImage();
    m_displayLabel->clear();
    m_displayLabel->setText(tr("VNC Console\n\nDisconnected"));
    emit disconnected();
}

void VNCWidget::sendKeyEvent(int key, bool pressed)
{
#ifdef LIBVNCCLIENT_FOUND
    if (m_client && m_connected) {
        int rfbKey = keyToRFBCode(key);
        SendKeyEvent(m_client, rfbKey, pressed ? 1 : 0);
    }
#else
    Q_UNUSED(key);
    Q_UNUSED(pressed);
#endif
}

void VNCWidget::sendPointerEvent(int x, int y, int buttonMask)
{
#ifdef LIBVNCCLIENT_FOUND
    if (m_client && m_connected) {
        SendPointerEvent(m_client, x, y, buttonMask);
    }
#else
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(buttonMask);
#endif
}

void VNCWidget::sendClipboardText(const QString &text)
{
#ifdef LIBVNCCLIENT_FOUND
    if (m_client && m_connected) {
        SendClientCutText(m_client, text.toUtf8().data(), text.length());
    }
#else
    Q_UNUSED(text);
#endif
}

void VNCWidget::setScalingEnabled(bool enabled)
{
    if (m_scalingEnabled != enabled) {
        m_scalingEnabled = enabled;
        updateScaling();
    }
}

void VNCWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void VNCWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateScaling();
}

void VNCWidget::keyPressEvent(QKeyEvent *event)
{
    if (m_connected) {
        sendKeyEvent(event->key(), true);
    }
    QWidget::keyPressEvent(event);
}

void VNCWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (m_connected) {
        sendKeyEvent(event->key(), false);
    }
    QWidget::keyReleaseEvent(event);
}

void VNCWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_connected) {
        int buttonMask = 0;
        if (event->buttons() & Qt::LeftButton) buttonMask |= 0x01;
        if (event->buttons() & Qt::MiddleButton) buttonMask |= 0x02;
        if (event->buttons() & Qt::RightButton) buttonMask |= 0x04;
        sendPointerEvent(event->pos().x(), event->pos().y(), buttonMask);
    }
    QWidget::mousePressEvent(event);
}

void VNCWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_connected) {
        sendPointerEvent(event->pos().x(), event->pos().y(), 0);
    }
    QWidget::mouseReleaseEvent(event);
}

void VNCWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_connected) {
        int buttonMask = 0;
        if (event->buttons() & Qt::LeftButton) buttonMask |= 0x01;
        if (event->buttons() & Qt::MiddleButton) buttonMask |= 0x02;
        if (event->buttons() & Qt::RightButton) buttonMask |= 0x04;
        sendPointerEvent(event->pos().x(), event->pos().y(), buttonMask);
    }
    QWidget::mouseMoveEvent(event);
}

void VNCWidget::wheelEvent(QWheelEvent *event)
{
    if (m_connected) {
#ifdef LIBVNCCLIENT_FOUND
        if (event->angleDelta().y() > 0) {
            SendPointerEvent(m_client, event->pos().x(), event->pos().y(), 0x08);
            SendPointerEvent(m_client, event->pos().x(), event->pos().y(), 0);
        } else {
            SendPointerEvent(m_client, event->pos().x(), event->pos().y(), 0x10);
            SendPointerEvent(m_client, event->pos().x(), event->pos().y(), 0);
        }
#endif
    }
    QWidget::wheelEvent(event);
}

void VNCWidget::focusInEvent(QFocusEvent *event)
{
    QWidget::focusInEvent(event);
}

void VNCWidget::focusOutEvent(QFocusEvent *event)
{
    QWidget::focusOutEvent(event);
}

void VNCWidget::updateFramebuffer()
{
#ifdef LIBVNCCLIENT_FOUND
    if (m_client && m_connected) {
        // Update framebuffer from VNC client
        // This would be called by the VNC client callback
    }
#endif
}

void VNCWidget::onConnectionFailed()
{
    disconnect();
    emit connectionError(tr("Connection failed"));
}

void VNCWidget::updateScaling()
{
    if (m_scalingEnabled && !m_framebuffer.isNull()) {
        m_scaledFramebuffer = m_framebuffer.scaled(
            m_displayLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );
        m_displayLabel->setPixmap(QPixmap::fromImage(m_scaledFramebuffer));
    } else if (!m_framebuffer.isNull()) {
        m_displayLabel->setPixmap(QPixmap::fromImage(m_framebuffer));
    }
}

int VNCWidget::keyToRFBCode(int qtKey)
{
#ifdef LIBVNCCLIENT_FOUND
    // Map Qt key codes to RFB key codes
    switch (qtKey) {
        case Qt::Key_Backspace: return XK_BackSpace;
        case Qt::Key_Tab: return XK_Tab;
        case Qt::Key_Return: return XK_Return;
        case Qt::Key_Enter: return XK_KP_Enter;
        case Qt::Key_Escape: return XK_Escape;
        case Qt::Key_Space: return XK_space;
        case Qt::Key_Delete: return XK_Delete;
        case Qt::Key_Up: return XK_Up;
        case Qt::Key_Down: return XK_Down;
        case Qt::Key_Left: return XK_Left;
        case Qt::Key_Right: return XK_Right;
        case Qt::Key_F1: return XK_F1;
        case Qt::Key_F2: return XK_F2;
        case Qt::Key_F3: return XK_F3;
        case Qt::Key_F4: return XK_F4;
        case Qt::Key_F5: return XK_F5;
        case Qt::Key_F6: return XK_F6;
        case Qt::Key_F7: return XK_F7;
        case Qt::Key_F8: return XK_F8;
        case Qt::Key_F9: return XK_F9;
        case Qt::Key_F10: return XK_F10;
        case Qt::Key_F11: return XK_F11;
        case Qt::Key_F12: return XK_F12;
        default:
            if (qtKey >= Qt::Key_A && qtKey <= Qt::Key_Z) {
                return qtKey;
            }
            if (qtKey >= Qt::Key_0 && qtKey <= Qt::Key_9) {
                return qtKey;
            }
            return qtKey;
    }
#else
    Q_UNUSED(qtKey);
    return 0;
#endif
}

} // namespace QVirt
