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
#include <QMutex>

#ifdef LIBVIRT_FOUND
#include <libvirt/libvirt.h>

// Windows.h defines 'state' as a macro which breaks our code
#ifdef _WIN32
#undef state
#endif
#else
// Forward declarations for when libvirt is not available
typedef void *virDomainPtr;

// Define libvirt constants when libvirt is not available
#define VIR_DOMAIN_NOSTATE 0
#define VIR_DOMAIN_RUNNING 1
#define VIR_DOMAIN_BLOCKED 2
#define VIR_DOMAIN_PAUSED 3
#define VIR_DOMAIN_SHUTDOWN 4
#define VIR_DOMAIN_SHUTOFF 5
#define VIR_DOMAIN_CRASHED 6
#define VIR_DOMAIN_PMSUSPENDED 7
#endif // LIBVIRT_FOUND

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

    // Async lifecycle operations (non-blocking)
    void startAsync();
    void shutdownAsync();
    void destroyAsync();
    void saveAsync(const QString &path);

    // Configuration
    QString getXMLDesc(unsigned int flags = 0) const;
    bool setXML(const QString &xml, unsigned int flags = 0);

    // Device manipulation
    bool attachDevice(const QString &xml);
    bool detachDevice(const QString &xml);
    bool updateDevice(const QString &xml);

    // Resources (cached - never call libvirt)
    quint64 maxMemory() const { return m_maxMemory; }
    quint64 memory() const { return m_currentMemory; }
    int vcpuCount() const { return m_vcpuCount; }
    int maxVcpuCount() const { return m_maxVcpuCount; }

    // Stats (cached - never call libvirt)
    quint64 cpuTime() const { return m_cpuTime; }
    float cpuUsage() const { return m_cachedCpuUsage; }
    quint64 currentMemory() const { return m_currentMemory; }
    float diskUsage() const { return m_cachedDiskUsage; }
    float networkUsage() const { return m_cachedNetworkUsage; }

    // Update cached info (synchronous - may block)
    void updateInfo();

    // Update cached info with minimal data (no XML, faster)
    void updateInfoMinimal();

    // Apply pre-collected stats (no libvirt calls, main thread only)
    void applyStats(int state, quint64 maxMemory, quint64 currentMemory,
                    int vcpuCount, quint64 cpuTime);

    // Update cached info (asynchronous - non-blocking)
    void updateInfoAsync();
    void updateInfoAsync(bool fetchXml);  // Option to skip XML fetching

    // Snapshot operations
    QList<DomainSnapshot*> snapshots() const;
    DomainSnapshot *createSnapshot(const QString &xml, unsigned int flags = 0);
    DomainSnapshot *currentSnapshot() const;
    bool hasCurrentSnapshot() const;

    // Guest Agent
    bool guestAgentConnected() const;
    QString guestAgentVersion() const;
    bool guestAgentPing() const;
    QString guestHostname() const;
    QString guestOS() const;
    QStringList guestIPAddresses() const;
    QStringList guestGetUsers() const;
    struct NetworkInterface {
        QString macAddress;
        QStringList ipAddresses;
    };
    QMap<QString, NetworkInterface> guestGetNetworkInterfaces() const;
    struct Filesystem {
        QString device;
        QString mountPoint;
        QString type;
        quint64 totalSize;
        quint64 usedSize;
        quint64 freeSize;
    };
    QList<Filesystem> guestGetFilesystems() const;
    bool guestAgentShutdown(int timeout = 60);

    // Connection
    Connection *connection() const { return m_connection; }

    // Raw libvirt pointer (for advanced operations)
    virDomainPtr rawDomain() const { return m_domain; }

    /**
     * @brief Check if this domain is from cache (offline mode)
     * @return true if domain was loaded from cache without live libvirt connection
     */
    bool isCached() const { return m_domain == nullptr; }

    /**
     * @brief Cached VM information for local storage
     */
    struct CacheInfo
    {
        QString name;
        QString uuid;
        int state = 0;  // Domain::State enum value
        QString description;
        QString title;
        quint64 memory = 0;  // in KB (max memory)
        quint64 currentMemory = 0;  // in KB (current memory)
        int vcpuCount = 0;
        int maxVcpuCount = 0;
        QString xmlDesc;  // Full XML configuration
        qint64 lastUpdated = 0;  // Unix timestamp

        CacheInfo() = default;

        explicit CacheInfo(const QString &name, const QString &uuid)
            : name(name), uuid(uuid) {}
    };

    // Serialization for caching
    CacheInfo toCacheInfo() const;
    static Domain *fromCacheInfo(Connection *conn, const CacheInfo &info);

signals:
    void stateChanged(State newState);
    void configChanged();
    void statsUpdated();

    // Async operation completion signals
    void infoUpdated();
    void infoUpdateFailed();
    void lifecycleOperationFinished(const QString &operation, bool success);

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

    // Cached XML for offline mode
    QString m_cachedXmlDesc;

    // Cached values
    quint64 m_maxMemory;
    quint64 m_currentMemory;
    int m_vcpuCount;
    quint64 m_cpuTime;

    // CPU usage calculation
    quint64 m_prevCpuTime;
    qint64 m_prevCpuTimestamp;
    float m_cachedCpuUsage;

    // Cached stats
    float m_cachedDiskUsage;
    float m_cachedNetworkUsage;
    int m_maxVcpuCount;

    // Track if XML has been fetched
    bool m_xmlFetched;

    friend class Connection;
};

} // namespace QVirt

#endif // QVIRT_LIBVIRT_DOMAIN_H
