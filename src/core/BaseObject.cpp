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

#include "BaseObject.h"
#include <QThread>
#include <QTimer>
#include <QCoreApplication>

namespace QVirt {

BaseObject::BaseObject(QObject *parent)
    : QObject(parent)
{
}

BaseObject::~BaseObject()
{
    cleanup();
}

void BaseObject::cleanup()
{
    // Disconnect all tracked signals
    for (const auto &conn : m_connections) {
        QObject::disconnect(conn);
    }
    m_connections.clear();

    // Stop and delete all tracked timers
    for (auto *timer : m_timers) {
        if (timer) {
            timer->stop();
            timer->deleteLater();
        }
    }
    m_timers.clear();
}

bool BaseObject::isMainThread()
{
    return QThread::currentThread() == QCoreApplication::instance()->thread();
}

void BaseObject::idleAdd(std::function<void()> func)
{
    QTimer::singleShot(0, QCoreApplication::instance(), [func]() {
        func();
    });
}

void BaseObject::trackConnection(const QMetaObject::Connection &conn)
{
    if (conn) {
        m_connections.append(conn);
    }
}

void BaseObject::trackTimer(QTimer *timer)
{
    if (timer) {
        m_timers.append(timer);
    }
}

void BaseObject::untrackConnection(const QMetaObject::Connection &conn)
{
    m_connections.removeAll(conn);
}

void BaseObject::startTrackedTimer(QTimer *timer, int interval)
{
    if (timer) {
        trackTimer(timer);
        timer->start(interval);
    }
}

} // namespace QVirt
