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

#include "Engine.h"
#include "../libvirt/Connection.h"
#include "SystemTray.h"
#include <QCoreApplication>

namespace QVirt {

Engine *Engine::s_instance = nullptr;

Engine::Engine()
    : BaseObject(nullptr)
    , m_tickTimer(new QTimer(this))
    , m_systemTray(nullptr)
{
    // Setup tick timer for polling
    connect(m_tickTimer, &QTimer::timeout, this, &Engine::onTick);

    // Connect to application quit signal
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit,
            this, &Engine::onAboutToQuit);
}

Engine::~Engine()
{
    cleanup();
}

Engine *Engine::instance()
{
    if (!s_instance) {
        s_instance = new Engine();
    }
    return s_instance;
}

void Engine::init()
{
    // Initialize system tray
    m_systemTray = new SystemTray(this);
    m_systemTray->init();
    connect(m_systemTray, &SystemTray::showManagerRequested,
            this, &Engine::onShowManagerFromTray);
    connect(m_systemTray, &SystemTray::quitRequested,
            this, &Engine::onQuitFromTray);

    // Show system tray icon
    m_systemTray->show();

    // Start the tick timer (poll every 1 second)
    m_tickTimer->start(1000);
}

void Engine::showManager()
{
    // Emit signal to show manager window
    // The ManagerWindow should connect to this signal
    emit showManagerRequested();
}

void Engine::registerConnection(Connection *conn)
{
    if (!conn) {
        return;
    }
    m_connections[conn->uri()] = conn;
}

void Engine::unregisterConnection(Connection *conn)
{
    if (!conn) {
        return;
    }
    m_connections.remove(conn->uri());
}

void Engine::onAboutToQuit()
{
    emit appClosing();
}

void Engine::onTick()
{
    // Poll for changes - connections handle their own updates
}

void Engine::onShowManagerFromTray()
{
    showManager();
}

void Engine::onQuitFromTray()
{
    QCoreApplication::quit();
}

} // namespace QVirt
