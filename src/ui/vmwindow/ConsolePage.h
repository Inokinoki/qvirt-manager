/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 Inoki <veyx.shaw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version).
 */

#ifndef QVIRT_UI_VMWINDOW_CONSOLEPAGE_H
#define QVIRT_UI_VMWINDOW_CONSOLEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include <QTextEdit>
#include <QToolBar>
#include <QAction>

#include "../../libvirt/Domain.h"

namespace QVirt {

/**
 * @brief Console Page for VMWindow
 *
 * Provides VNC/SPICE console viewing functionality.
 * This is a placeholder implementation that can be extended
 * with actual VNC/SPICE viewers in the future.
 */
class ConsolePage : public QWidget
{
    Q_OBJECT

public:
    explicit ConsolePage(Domain *domain, QWidget *parent = nullptr);
    ~ConsolePage() override = default;

    void refresh();
    void connectConsole();
    void disconnectConsole();

signals:
    void consoleConnected();
    void consoleDisconnected();
    void consoleError(const QString &message);

private slots:
    void onConnectClicked();
    void onDisconnectClicked();
    void onFullscreenToggled(bool checked);
    void onSendKey();
    void onScreenshot();
    void onResizeGuestChanged(bool checked);

private:
    void setupUI();
    void setupToolbar();
    void setupInfoView();
    void setupPlaceholderView();
    void updateConsoleInfo();
    QString getGraphicsType() const;

    Domain *m_domain;

    // UI components
    QVBoxLayout *m_mainLayout;
    QStackedWidget *m_viewStack;
    QWidget *m_infoWidget;
    QWidget *m_placeholderWidget;
    QTextEdit *m_infoText;
    QLabel *m_statusLabel;
    QLabel *m_graphicsTypeLabel;
    QLabel *m_portLabel;
    QLabel *m_addressLabel;
    QComboBox *m_keyComboCombo;

    // Toolbar
    QToolBar *m_toolbar;
    QAction *m_actionConnect;
    QAction *m_actionDisconnect;
    QAction *m_actionFullscreen;
    QAction *m_actionSendKey;
    QAction *m_actionScreenshot;
    QAction *m_actionGrabKeys;
    QAction *m_actionResizeGuest;

    // Console state
    bool m_connected;
    bool m_fullscreen;
};

} // namespace QVirt

#endif // QVIRT_UI_VMWINDOW_CONSOLEPAGE_H
