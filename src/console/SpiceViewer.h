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

#ifndef QVIRT_CONSOLE_SPICEVIEWER_H
#define QVIRT_CONSOLE_SPICEVIEWER_H

#include "Viewer.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>

#ifdef HAVE_SPICE
#include <spice-client-gtk.h>
#include <gtk/gtk.h>
#include <QPointer>
#endif

namespace QVirt {

/**
 * @brief SPICE Console Viewer with full SPICE protocol support
 *
 * Provides native SPICE console viewing using Spice GTK library when available,
 * with fallback to external viewer launching.
 *
 * Features:
 * - SPICE protocol support via Spice GTK
 * - Keyboard and mouse input
 * - USB device redirection
 * - File transfer support
 * - Smartcard passthrough
 * - Scaling and fullscreen modes
 * - Screenshot capture
 */
class SpiceViewer : public Viewer
{
    Q_OBJECT

public:
    explicit SpiceViewer(QWidget *parent = nullptr);
    ~SpiceViewer() override;

    bool connectToHost(const QString &host, int port) override;
    void disconnect() override;
    void sendKeys(const QString &keys) override;
    QPixmap takeScreenshot() override;
    void toggleFullscreen() override;

    /**
     * @brief Set SPICE TLS port
     */
    void setTLSPort(int port);

    /**
     * @brief Set SPICE password
     */
    void setPassword(const QString &password);

    /**
     * @brief Enable/disable scaling
     */
    void setScalingEnabled(bool enabled);

    /**
     * @brief Enable/disable USB redirection
     */
    void setUSBRedirectionEnabled(bool enabled);

    /**
     * @brief Check if SPICE is connected
     */
    bool isConnected() const { return m_connected; }

signals:
    void connectionLost();
    void bellSignal();
    void usbDeviceConnected(const QString &deviceName);
    void usbDeviceDisconnected(const QString &deviceName);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void onSpiceConnected();
    void onSpiceDisconnected();
    void onSpiceError(const QString &error);
    void onUSBDeviceAdded(const QString &device);
    void onUSBDeviceRemoved(const QString &device);

private:
    void setupUI();
    void setupExternalViewerUI();
    void embedSpiceWidget();
    void cleanupSpice();
    void sendKeyEvent(QKeyEvent *event, bool isPress);

#ifdef HAVE_SPICE
    SpiceDisplay *m_spiceDisplay = nullptr;
    SpiceSession *m_spiceSession = nullptr;
    GtkWidget *m_spiceWidget = nullptr;
    QWidget *m_gtkContainer = nullptr;
#endif

    QLabel *m_displayLabel;
    QPushButton *m_connectBtn;
    QScrollArea *m_scrollArea;
    QVBoxLayout *m_mainLayout = nullptr;
    QString m_host;
    QString m_password;
    int m_port = 0;
    int m_tlsPort = 0;
    bool m_scalingEnabled = true;
    bool m_usbRedirectionEnabled = true;
};

} // namespace QVirt

#endif // QVIRT_CONSOLE_SPICEVIEWER_H
