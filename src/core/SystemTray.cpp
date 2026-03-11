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

#include "SystemTray.h"
#include <QApplication>
#include <QDebug>

namespace QVirt {

SystemTray::SystemTray(QObject *parent)
    : BaseObject(parent)
    , m_trayIcon(nullptr)
    , m_menu(nullptr)
    , m_showAction(nullptr)
    , m_quitAction(nullptr)
{
}

void SystemTray::init()
{
    // Check if system tray is available
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qWarning() << "System tray is not available on this platform";
        return;
    }

    // Create system tray icon
    m_trayIcon = new QSystemTrayIcon(this);

    // Set icon
    updateIcon();

    // Create context menu
    createMenu();

    // Connect signals
    connect(m_trayIcon, &QSystemTrayIcon::activated,
            this, &SystemTray::onActivated);

    qDebug() << "System tray initialized";
}

void SystemTray::createMenu()
{
    m_menu = new QMenu();

    m_showAction = m_menu->addAction(tr("Show Manager"));
    m_showAction->setIcon(QIcon::fromTheme("window-show"));
    connect(m_showAction, &QAction::triggered, this, &SystemTray::onShowManager);

    m_menu->addSeparator();

    m_quitAction = m_menu->addAction(tr("Quit"));
    m_quitAction->setIcon(QIcon::fromTheme("application-exit"));
    connect(m_quitAction, &QAction::triggered, this, &SystemTray::onQuit);

    m_trayIcon->setContextMenu(m_menu);
}

void SystemTray::updateIcon()
{
    // Try to load icon from theme or resources
    QIcon icon;

    // Try standard icon names
    if (icon.isNull()) {
        icon = QIcon::fromTheme("qvirt-manager");
    }
    if (icon.isNull()) {
        icon = QIcon::fromTheme("virt-manager");
    }
    if (icon.isNull()) {
        icon = QIcon::fromTheme("computer");
    }

    // Fallback to resource
    if (icon.isNull()) {
        icon = QIcon(":/icons/app/qvirt-manager.svg");
    }

    m_trayIcon->setIcon(icon);
    m_trayIcon->setToolTip(tr("QVirt Manager"));
}

void SystemTray::show()
{
    if (m_trayIcon) {
        m_trayIcon->show();
        qDebug() << "System tray icon shown";
    }
}

void SystemTray::hide()
{
    if (m_trayIcon) {
        m_trayIcon->hide();
        qDebug() << "System tray icon hidden";
    }
}

void SystemTray::onActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            // Single or double click - show manager
            emit showManagerRequested();
            break;

        case QSystemTrayIcon::MiddleClick:
            // Middle click - do nothing or could show quick menu
            break;

        case QSystemTrayIcon::Context:
            // Context menu - handled by context menu
            break;

        case QSystemTrayIcon::Unknown:
        default:
            break;
    }
}

void SystemTray::onShowManager()
{
    emit showManagerRequested();
}

void SystemTray::onQuit()
{
    emit quitRequested();
}

} // namespace QVirt
