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

#ifndef QVIRT_CORE_SYSTEMTRAY_H
#define QVIRT_CORE_SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

#include "BaseObject.h"

namespace QVirt {

/**
 * @brief System Tray Icon Manager
 *
 * Provides system tray integration for QVirt-Manager
 * Allows minimizing to tray and quick access to VMs
 */
class SystemTray : public BaseObject
{
    Q_OBJECT

public:
    explicit SystemTray(QObject *parent = nullptr);
    ~SystemTray() override = default;

    void init();
    void show();
    void hide();
    bool isVisible() const { return m_trayIcon ? m_trayIcon->isVisible() : false; }

signals:
    void showManagerRequested();
    void quitRequested();

private slots:
    void onActivated(QSystemTrayIcon::ActivationReason reason);
    void onShowManager();
    void onQuit();

private:
    void createMenu();
    void updateIcon();

    QSystemTrayIcon *m_trayIcon;
    QMenu *m_menu;
    QAction *m_showAction;
    QAction *m_quitAction;
};

} // namespace QVirt

#endif // QVIRT_CORE_SYSTEMTRAY_H
