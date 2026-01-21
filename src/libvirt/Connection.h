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

#include <libvirt/libvirt.h>

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
        Active
    };
    Q_ENUM(State)

    /**
     * @brief Open a new libvirt connection
     * @param uri Connection URI (e.g., "qemu:///system")
     * @return Connection object, or nullptr on failure
     */
    static Connection *open(const QString &uri);

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

private:
    Connection(const QString &uri);
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

    // Object caches
    QMap<QString, Domain *> m_domains;
    QMap<QString, Network *> m_networks;
    QMap<QString, StoragePool *> m_storagePools;
    QMap<QString, NodeDevice *> m_nodeDevices;
};

} // namespace QVirt

#endif // QVIRT_LIBVIRT_CONNECTION_H
