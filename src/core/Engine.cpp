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

#include "Engine.h"
#include "../libvirt/Connection.h"
#include "../ui/manager/ManagerWindow.h"
#include <QApplication>
#include <QCoreApplication>

namespace QVirt {

Engine *Engine::s_instance = nullptr;

Engine::Engine()
    : BaseObject(nullptr)
    , m_tickTimer(new QTimer(this))
    , m_managerWindow(nullptr)
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
    // Start the tick timer (poll every 1 second)
    m_tickTimer->start(1000);
}

void Engine::showManager()
{
    // Create manager window if it doesn't exist
    if (!m_managerWindow) {
        m_managerWindow = new ManagerWindow();
    }

    // Show the manager window
    m_managerWindow->show();
    m_managerWindow->raise();
    m_managerWindow->activateWindow();
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

void Engine::onTick()
{
    // Update all connections
    // This will poll for changes in domains, networks, storage pools, etc.
    for (auto *conn : m_connections) {
        if (conn) {
            // Trigger connection tick
            conn->tick();
        }
    }
}

void Engine::onAboutToQuit()
{
    emit appClosing();
}

} // namespace QVirt
