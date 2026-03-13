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

#ifndef QVIRT_CONSOLE_VNCVIEWER_H
#define QVIRT_CONSOLE_VNCVIEWER_H

#include "Viewer.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMenu>

#ifdef HAVE_GTK_VNC
#include <gtk-vnc/vncwidget.h>
#include <gtk/gtk.h>
#include <QPointer>
#endif

namespace QVirt {

/**
 * @brief VNC Console Viewer with full VNC protocol support
 *
 * Provides native VNC console viewing using GtkVnc library when available,
 * with fallback to external viewer launching.
 *
 * Features:
 * - VNC protocol support via GtkVnc
 * - Keyboard and mouse input
 * - Scaling and fullscreen modes
 * - Screenshot capture
 * - Send key combinations
 */
class VNCViewer : public Viewer
{
    Q_OBJECT

public:
    explicit VNCViewer(QWidget *parent = nullptr);
    ~VNCViewer() override;

    bool connectToHost(const QString &host, int port) override;
    void disconnect() override;
    void sendKeys(const QString &keys) override;
    QPixmap takeScreenshot() override;
    void toggleFullscreen() override;

    /**
     * @brief Set VNC password for authentication
     */
    void setPassword(const QString &password);

    /**
     * @brief Enable/disable scaling
     */
    void setScalingEnabled(bool enabled);

    /**
     * @brief Check if VNC is connected
     */
    bool isConnected() const { return m_connected; }

signals:
    void connectionLost();
    void bellSignal();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void onVNCConnected();
    void onVNCDisconnected();
    void onVNCEncodingChanged();
    void onVNCBell();
    void onVNCError(const QString &error);

private:
    void setupUI();
    void setupExternalViewerUI();
    void embedGtkVncWidget();
    void cleanupGtkVnc();
    void sendKeyEvent(QKeyEvent *event, bool isPress);
    void sendPointerEvent(int x, int y, int buttons);
    QString mapQtKeyToVNC(QKeyEvent *event);

#ifdef HAVE_GTK_VNC
    GtkWidget *m_gtkVncWidget = nullptr;
    QWidget *m_gtkContainer = nullptr;
    QPointer<VncDisplay> m_vncDisplay;
#endif

    QLabel *m_displayLabel;
    QPushButton *m_connectBtn;
    QScrollArea *m_scrollArea;
    QWidget *m_viewport;
    QVBoxLayout *m_mainLayout = nullptr;
    QString m_host;
    QString m_password;
    int m_port = 0;
    bool m_scalingEnabled = true;
    int m_mouseButtons = 0;
};

} // namespace QVirt

#endif // QVIRT_CONSOLE_VNCVIEWER_H
