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

#ifndef QVIRT_LIBVIRT_DOMAIN_H
#define QVIRT_LIBVIRT_DOMAIN_H

#include "../core/BaseObject.h"
#include <QString>
#include <QPixmap>
#include <QList>

#include <libvirt/libvirt.h>

// Windows.h defines 'state' as a macro which breaks our code
#ifdef _WIN32
#undef state
#endif

namespace QVirt {

class Connection;
class DomainSnapshot;

/**
 * @brief libvirt domain (VM) wrapper
 *
 * Wraps virDomainPtr and provides:
 * - VM lifecycle operations (start/stop/reboot)
 * - Statistics collection (CPU, memory, disk)
 * - Configuration access
 * - Snapshot management
 */
class Domain : public BaseObject
{
    Q_OBJECT

public:
    enum State {
        StateNoState = VIR_DOMAIN_NOSTATE,
        StateRunning = VIR_DOMAIN_RUNNING,
        StateBlocked = VIR_DOMAIN_BLOCKED,
        StatePaused = VIR_DOMAIN_PAUSED,
        StateShutdown = VIR_DOMAIN_SHUTDOWN,
        StateShutOff = VIR_DOMAIN_SHUTOFF,
        StateCrashed = VIR_DOMAIN_CRASHED,
        StatePMSuspended = VIR_DOMAIN_PMSUSPENDED
    };
    Q_ENUM(State)

    ~Domain() override;

    // Basic info
    QString name() const { return m_name; }
    QString uuid() const { return m_uuid; }
    QString id() const { return m_id; }
    State state() const { return m_state; }
    QString description() const { return m_description; }
    QString title() const { return m_title; }

    // Lifecycle operations
    bool start();
    bool stop();
    bool reboot();
    bool reset();
    bool destroy();
    bool shutdown();
    bool save(const QString &path);

    // Configuration
    QString getXMLDesc(unsigned int flags = 0) const;
    bool setXML(const QString &xml, unsigned int flags = 0);

    // Resources
    quint64 maxMemory() const;  // in KB
    quint64 memory() const;
    int vcpuCount() const;
    int maxVcpuCount() const;

    // Stats
    quint64 cpuTime() const;
    float cpuUsage() const;  // percentage
    quint64 currentMemory() const;
    float diskUsage() const;  // percentage
    float networkUsage() const;  // percentage

    // Update cached info
    void updateInfo();

    // Snapshot operations
    QList<DomainSnapshot*> snapshots() const;
    DomainSnapshot *createSnapshot(const QString &xml, unsigned int flags = 0);
    DomainSnapshot *currentSnapshot() const;
    bool hasCurrentSnapshot() const;

    // Connection
    Connection *connection() const { return m_connection; }

    // Raw libvirt pointer (for advanced operations)
    virDomainPtr rawDomain() const { return m_domain; }

signals:
    void stateChanged(State newState);
    void configChanged();
    void statsUpdated();

private:
    Domain(Connection *conn, virDomainPtr domain);
    void setState(State state);

    Connection *m_connection;
    virDomainPtr m_domain;

    QString m_name;
    QString m_uuid;
    QString m_id;
    State m_state;

    QString m_description;
    QString m_title;

    // Cached values
    quint64 m_maxMemory;
    int m_vcpuCount;
    quint64 m_cpuTime;

    friend class Connection;
};

} // namespace QVirt

#endif // QVIRT_LIBVIRT_DOMAIN_H
