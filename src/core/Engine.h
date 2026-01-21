/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 The QVirt-Manager Developers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef QVIRT_CORE_ENGINE_H
#define QVIRT_CORE_ENGINE_H

#include "BaseObject.h"
#include <QTimer>
#include <QMap>
#include <QString>

namespace QVirt {

class Connection;

/**
 * @brief Main application engine (singleton)
 *
 * This class coordinates:
 * - Window management
 * - Connection management
 * - Global tick timer for polling updates
 * - Application lifecycle
 *
 * Mirrors the Python vmmEngine class from virt-manager
 */
class Engine : public BaseObject
{
    Q_OBJECT

public:
    /**
     * @brief Get the singleton instance
     * @return Engine instance
     */
    static Engine *instance();

    /**
     * @brief Initialize the engine
     */
    void init();

    /**
     * @brief Show the main manager window
     */
    void showManager();

signals:
    /**
     * @brief Emitted when the application is closing
     */
    void appClosing();

private slots:
    void onTick();
    void onAboutToQuit();

private:
    Engine();
    ~Engine() override;
    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    static Engine *s_instance;

    QTimer *m_tickTimer;
    QMap<QString, Connection *> m_connections;
};

} // namespace QVirt

#endif // QVIRT_CORE_ENGINE_H
