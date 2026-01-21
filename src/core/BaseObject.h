/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 The QVirt-Manager Developers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef QVIRT_CORE_BASEOBJECT_H
#define QVIRT_CORE_BASEOBJECT_H

#include <QObject>
#include <QList>
#include <QTimer>
#include <QMetaObject>
#include <functional>

namespace QVirt {

/**
 * @brief Base class for all objects in qvirt-manager
 *
 * This class provides:
 * - Automatic signal/slot lifecycle management
 * - Timer tracking and cleanup
 * - Idle callback support
 * - Thread checking helpers
 *
 * Mirrors the Python vmmGObject class from virt-manager
 */
class BaseObject : public QObject
{
    Q_OBJECT

public:
    explicit BaseObject(QObject *parent = nullptr);
    virtual ~BaseObject();

    /**
     * @brief Cleanup method to be called before object destruction
     *
     * Subclasses should override this to perform cleanup.
     * Base implementation disconnects all tracked signals and stops timers.
     */
    virtual void cleanup();

    /**
     * @brief Check if we're running in the main thread
     * @return true if in main thread, false otherwise
     */
    static bool isMainThread();

    /**
     * @brief Add a function to be called in the next event loop iteration
     * @param func Function to call
     */
    static void idleAdd(std::function<void()> func);

protected:
    /**
     * @brief Track a signal/slot connection for automatic cleanup
     * @param conn Connection to track
     */
    void trackConnection(const QMetaObject::Connection &conn);

    /**
     * @brief Track a timer for automatic cleanup
     * @param timer Timer to track
     */
    void trackTimer(QTimer *timer);

    /**
     * @brief Untrack a connection (e.g., after manual disconnect)
     * @param conn Connection to untrack
     */
    void untrackConnection(const QMetaObject::Connection &conn);

    /**
     * @brief Start a tracked timer
     * @param timer Timer to start and track
     * @param interval Interval in milliseconds
     */
    void startTrackedTimer(QTimer *timer, int interval);

private:
    QList<QMetaObject::Connection> m_connections;
    QList<QTimer *> m_timers;
};

} // namespace QVirt

#endif // QVIRT_CORE_BASEOBJECT_H
