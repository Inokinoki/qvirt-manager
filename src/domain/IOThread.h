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

#ifndef QVIRT_DOMAIN_IOTHREAD_H
#define QVIRT_DOMAIN_IOTHREAD_H

#include "../core/BaseObject.h"
#include <QString>
#include <QList>

namespace QVirt {

/**
 * @brief I/O thread configuration
 */
class IOThread
{
public:
    IOThread();
    IOThread(const IOThread &other);
    IOThread &operator=(const IOThread &other);

    int id() const { return m_id; }
    void setId(int id);

    QString cpuset() const { return m_cpuset; }
    void setCpuset(const QString &cpuset);

    QString toXML() const;
    bool fromXML(const QString &xml);

    bool isEmpty() const;

private:
    int m_id = 0;
    QString m_cpuset;
};

/**
 * @brief I/O thread configuration list
 */
class IOThreadConfig : public BaseObject
{
    Q_OBJECT

public:
    explicit IOThreadConfig(QObject *parent = nullptr);
    ~IOThreadConfig() override = default;

    QList<IOThread> threads() const { return m_threads; }
    void setThreads(const QList<IOThread> &threads);
    void addThread(const IOThread &thread);
    void removeThread(int id);

    QString toXML() const;
    bool fromXML(const QString &xml);

    bool isEmpty() const;

signals:
    void configChanged();

private:
    QList<IOThread> m_threads;
};

} // namespace QVirt

#endif // QVIRT_DOMAIN_IOTHREAD_H
