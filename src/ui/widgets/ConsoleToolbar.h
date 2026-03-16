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

#ifndef QVIRT_UI_WIDGETS_CONSOLETOOLBAR_H
#define QVIRT_UI_WIDGETS_CONSOLETOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QComboBox>
#include <QMenu>
#include <QPushButton>

namespace QVirt {

/**
 * @brief Unified toolbar for console viewers
 *
 * Provides common actions for VNC/SPICE console viewers:
 * - Fullscreen toggle
 * - Scaling options
 * - Send key combinations
 * - Screenshot
 * - USB redirection (SPICE)
 * - File transfer (SPICE)
 * - Disconnect
 */
class ConsoleToolbar : public QToolBar
{
    Q_OBJECT

public:
    explicit ConsoleToolbar(QWidget *parent = nullptr);
    ~ConsoleToolbar() override = default;

    void setViewerType(const QString &type);
    void setConnected(bool connected);
    void setScalingEnabled(bool enabled);
    bool isScalingEnabled() const;
    void setFullscreen(bool fullscreen);
    bool isFullscreen() const { return m_fullscreen; }

    void setUSBRedirectionEnabled(bool enabled);
    bool isUSBRedirectionEnabled() const;

signals:
    void fullscreenToggled(bool enabled);
    void scalingChanged(const QString &scaleMode);
    void sendKeyRequested(const QString &keyCombo);
    void screenshotRequested();
    void resizeGuestChanged(bool enabled);
    void usbRedirectionRequested();
    void fileTransferRequested();
    void disconnectRequested();
    void scale100Requested();
    void scaleFitRequested();
    void scaleFillRequested();

private slots:
    void onScaleModeChanged(const QString &mode);
    void onSendKeyTriggered();
    void showSendKeyMenu();

private:
    void setupUI();
    void setupScaleMenu();
    void setupSendKeyMenu();
    void setupUSBMenu();

    QAction *m_actionFullscreen;
    QAction *m_actionScale;
    QComboBox *m_scaleCombo;
    QAction *m_actionSendKey;
    QAction *m_actionScreenshot;
    QAction *m_actionResizeGuest;
    QAction *m_actionUSBRedir;
    QAction *m_actionFileTransfer;
    QAction *m_actionDisconnect;

    QMenu *m_sendKeyMenu;
    QMenu *m_usbMenu;

    bool m_fullscreen = false;
    bool m_scalingEnabled = true;
    bool m_usbRedirectionEnabled = false;
    QString m_viewerType;
};

} // namespace QVirt

#endif // QVIRT_UI_WIDGETS_CONSOLETOOLBAR_H
