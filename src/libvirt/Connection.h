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

#ifndef QVIRT_LIBVIRT_CONNECTION_H
#define QVIRT_LIBVIRT_CONNECTION_H

#include "../core/BaseObject.h"
#include <QString>
#include <QList>
#include <QMap>

#ifdef LIBVIRT_FOUND
#include <libvirt/libvirt.h>

// Windows.h defines 'state' as a macro which breaks our code
#ifdef _WIN32
#undef state
#endif
#else
// Forward declarations for when libvirt is not available
typedef void *virConnectPtr;
#endif // LIBVIRT_FOUND

namespace QVirt {

class Domain;
class Network;
class StoragePool;
class NodeDevice;

/**
 * @brief libvirt connection wrapper
 *
 * Wraps virConnectPtr with RAII and provides:
 * - Connection state management
 * - Object caching (domains, networks, storage pools)
 * - Polling-based change detection
 *
 * Mirrors the Python vmmConnection class from virt-manager
 */
class Connection : public BaseObject
{
    Q_OBJECT

public:
    enum State {
        Disconnected,
        Connecting,
        Active,
        ConnectionFailed
    };
    Q_ENUM(State)

    /**
     * @brief Open a new libvirt connection (blocking)
     * @param uri Connection URI (e.g., "qemu:///system" or "qemu+ssh://user@host/system")
     * @param sshKeyPath Optional path to SSH private key
     * @param password Optional password for authentication
     * @return Connection object, or nullptr on failure
     */
    static Connection *open(const QString &uri, const QString &sshKeyPath = QString(),
                           const QString &password = QString());

    /**
     * @brief Create a connection object without opening (for async connection)
     * @param uri Connection URI
     * @return Connection object
     */
    static Connection *create(const QString &uri);

    /**
     * @brief Open a new libvirt connection asynchronously (non-blocking)
     * @param sshKeyPath Optional path to SSH private key
     * @param password Optional password for authentication
     */
    void openAsync(const QString &sshKeyPath = QString(), const QString &password = QString());

    /**
     * @brief Get connection error message after failed connection
     */
    QString connectionError() const { return m_connectionError; }

    ~Connection() override;

    // Connection state
    State state() const { return m_state; }
    QString uri() const { return m_uri; }
    bool isOpen() const;

    // Close the connection
    void close();

    // Object accessors
    QList<Domain *> domains() const;
    Domain *getDomain(const QString &name);
    Domain *getDomainByUUID(const QString &uuid);

    QList<Network *> networks() const;
    Network *getNetwork(const QString &name);

    QList<StoragePool *> storagePools() const;
    StoragePool *getStoragePool(const QString &name);

    QList<NodeDevice *> nodeDevices() const;
    NodeDevice *getNodeDevice(const QString &name);

    // Connection info
    QString hostname() const;
    QString capabilities() const;
    QString libvirtVersion() const;

    // Host statistics
    int getHostCPUUsage();  // Returns percentage 0-100
    unsigned long long getHostMemoryTotal();  // In KB
    unsigned long long getHostMemoryUsed();   // In KB

    // Polling control
    bool isPollingEnabled() const { return m_pollingEnabled; }
    void setPollingEnabled(bool enabled) { m_pollingEnabled = enabled; }

    // SSH credentials (stored for persistence)
    QString sshKeyPath() const { return m_sshKeyPath; }
    QString sshUsername() const { return m_sshUsername; }

signals:
    void stateChanged(State newState);
    void domainAdded(Domain *domain);
    void domainRemoved(Domain *domain);
    void networkAdded(Network *network);
    void networkRemoved(Network *network);
    void storagePoolAdded(StoragePool *pool);
    void storagePoolRemoved(StoragePool *pool);

 public slots:
     void tick();

     // Refresh cached objects (domains, networks, storage pools)
     void refresh();

 private:
    Connection(const QString &uri);
    Connection(const QString &uri, const QString &sshKeyPath, const QString &password);
    void initAllResources();
    void pollDomains();
    void pollNetworks();
    void pollStoragePools();

    // Make m_conn accessible to Domain for XML operations
    friend class Domain;

    QString m_uri;
    State m_state;

    virConnectPtr m_conn;  // libvirt connection pointer

    int m_tickCounter;
    bool m_initialPoll;
    bool m_pollingEnabled;

    // SSH credentials (for persistence)
    QString m_sshKeyPath;
    QString m_sshUsername;

    // Connection error message
    QString m_connectionError;

    // Object caches
    QMap<QString, Domain *> m_domains;
    QMap<QString, Network *> m_networks;
    QMap<QString, StoragePool *> m_storagePools;
    QMap<QString, NodeDevice *> m_nodeDevices;

    // Cached host stats for CPU usage calculation
    unsigned long long m_lastCPUTime;
    unsigned long long m_lastIdleTime;
    int m_lastCPUUsage;
};

} // namespace QVirt

#endif // QVIRT_LIBVIRT_CONNECTION_H
