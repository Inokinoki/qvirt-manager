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

#include "Connection.h"
#include "Domain.h"
#include "Network.h"
#include "StoragePool.h"
#include "NodeDevice.h"
#include "../core/Error.h"
#include <QDebug>

#ifdef LIBVIRT_FOUND
#include <libvirt/libvirt.h>

// Windows.h defines 'state' as a macro which breaks our code
#ifdef _WIN32
#undef state
#endif

// Auth callback data structure
struct ConnectionAuthData {
    QString password;
    QString sshKeyPath;
};

// Libvirt auth callback function
static int virConnectAuthCallback(virConnectCredentialPtr cred, unsigned int ncred, void *cbdata)
{
    auto *authData = static_cast<ConnectionAuthData *>(cbdata);

    for (unsigned int i = 0; i < ncred; ++i) {
        if (cred[i].type == VIR_CRED_PASSPHRASE || cred[i].type == VIR_CRED_NOECHOPROMPT) {
            // Use provided password if available
            if (!authData->password.isEmpty()) {
                cred[i].result = strdup(authData->password.toUtf8().constData());
                if (!cred[i].result) {
                    return -1;
                }
                cred[i].resultlen = strlen(cred[i].result);
            } else {
                // No password provided, request it
                return -1;
            }
        } else if (cred[i].type == VIR_CRED_EXTERNAL) {
            // External auth method (e.g., SSH agent)
            if (cred[i].prompt && strcmp(cred[i].prompt, "Auth Name") == 0) {
                // For SSH key auth, we can set the SSH_AUTH_SOCK environment variable
                // or the user can configure it in ~/.ssh/config
                // The sshKeyPath would typically be used via SSH config
                continue;
            }
        } else {
            // Unsupported credential type
            return -1;
        }
    }
    return 0;
}

// Auth callback struct for libvirt
static virConnectAuth authHelper = {
    .cb = virConnectAuthCallback,
    .cbdata = nullptr,
    .credtype = nullptr,
    .ncredtype = 0
};
#endif

namespace QVirt {

Connection::Connection(const QString &uri)
    : BaseObject()
    , m_uri(uri)
    , m_state(Disconnected)
    , m_conn(nullptr)
    , m_tickCounter(0)
    , m_initialPoll(true)
{
    // Attempt to open the connection (no auth)
    m_conn = virConnectOpen(uri.toUtf8().constData());

    if (m_conn) {
        m_state = Active;
        emit stateChanged(m_state);
        qDebug() << "Connected to" << m_uri;
    } else {
        m_state = Disconnected;
        emit stateChanged(m_state);
        qWarning() << "Failed to connect to" << m_uri;
    }
}

Connection::Connection(const QString &uri, const QString &sshKeyPath, const QString &password)
    : BaseObject()
    , m_uri(uri)
    , m_state(Disconnected)
    , m_conn(nullptr)
    , m_tickCounter(0)
    , m_initialPoll(true)
    , m_sshKeyPath(sshKeyPath)
{
#ifdef LIBVIRT_FOUND
    // Set up authentication data
    ConnectionAuthData authData;
    authData.password = password;
    authData.sshKeyPath = sshKeyPath;

    // For SSH key authentication, we can set the environment variable
    // or rely on SSH config. The user should configure SSH config properly.
    if (!sshKeyPath.isEmpty()) {
        qputenv("LIBVIRT_SSH_KEY_PATH", sshKeyPath.toUtf8());
    }

    // Extract username from URI for persistence
    // URI format: qemu+ssh://[username@]hostname[:port]/system
    QString uriCopy = uri;
    if (uriCopy.contains("://")) {
        QString authPart = uriCopy.split("://")[1];
        authPart = authPart.split("/")[0]; // Remove /system part
        if (authPart.contains("@")) {
            m_sshUsername = authPart.split("@")[0];
        }
    }

    // Attempt to open the connection with auth
    authHelper.cbdata = &authData;
    m_conn = virConnectOpenAuth(uri.toUtf8().constData(), &authHelper, 0);

    if (m_conn) {
        m_state = Active;
        emit stateChanged(m_state);
        qDebug() << "Connected to" << m_uri << "with authentication";
    } else {
        m_state = Disconnected;
        emit stateChanged(m_state);
        qWarning() << "Failed to connect to" << m_uri << "with authentication";
    }

    // Clear environment variable
    if (!sshKeyPath.isEmpty()) {
        qunsetenv("LIBVIRT_SSH_KEY_PATH");
    }
#else
    // No libvirt support
    m_state = Disconnected;
    emit stateChanged(m_state);
#endif
}

Connection::~Connection()
{
    close();
}

Connection *Connection::open(const QString &uri, const QString &sshKeyPath, const QString &password)
{
    Connection *conn = nullptr;

    // If no credentials provided, use simple connection (for local or SSH agent auth)
    if (sshKeyPath.isEmpty() && password.isEmpty()) {
        conn = new Connection(uri);
    } else {
        // Use authenticated connection
        conn = new Connection(uri, sshKeyPath, password);
    }

    if (!conn->isOpen()) {
        delete conn;
        return nullptr;
    }
    return conn;
}

bool Connection::isOpen() const
{
    return m_conn != nullptr && m_state == Active;
}

void Connection::close()
{
    // Clean up cached objects
    for (auto *domain : m_domains) {
        delete domain;
    }
    m_domains.clear();

    for (auto *network : m_networks) {
        delete network;
    }
    m_networks.clear();

    for (auto *pool : m_storagePools) {
        delete pool;
    }
    m_storagePools.clear();

    for (auto *device : m_nodeDevices) {
        delete device;
    }
    m_nodeDevices.clear();

    if (m_conn) {
        virConnectClose(m_conn);
        m_conn = nullptr;
    }

    m_state = Disconnected;
    emit stateChanged(m_state);
}

QList<Domain *> Connection::domains() const
{
    return m_domains.values();
}

Domain *Connection::getDomain(const QString &name)
{
    return m_domains.value(name, nullptr);
}

Domain *Connection::getDomainByUUID(const QString &uuid)
{
    for (auto *domain : m_domains) {
        if (domain && domain->uuid() == uuid) {
            return domain;
        }
    }
    return nullptr;
}

QList<Network *> Connection::networks() const
{
    return m_networks.values();
}

Network *Connection::getNetwork(const QString &name)
{
    return m_networks.value(name, nullptr);
}

QList<StoragePool *> Connection::storagePools() const
{
    return m_storagePools.values();
}

StoragePool *Connection::getStoragePool(const QString &name)
{
    return m_storagePools.value(name, nullptr);
}

QList<NodeDevice *> Connection::nodeDevices() const
{
    return m_nodeDevices.values();
}

NodeDevice *Connection::getNodeDevice(const QString &name)
{
    return m_nodeDevices.value(name, nullptr);
}

QString Connection::hostname() const
{
    if (!m_conn) {
        return QString();
    }

    char *hostname = virConnectGetHostname(m_conn);
    if (!hostname) {
        return QString();
    }

    QString result = QString::fromUtf8(hostname);
    free(hostname);
    return result;
}

QString Connection::capabilities() const
{
    if (!m_conn) {
        return QString();
    }

    char *caps = virConnectGetCapabilities(m_conn);
    if (!caps) {
        return QString();
    }

    QString result = QString::fromUtf8(caps);
    free(caps);
    return result;
}

QString Connection::libvirtVersion() const
{
    if (!m_conn) {
        return "Libvirt (unknown version)";
    }

    unsigned long libvirtVersion = 0;
    if (virConnectGetLibVersion(m_conn, &libvirtVersion) == 0) {
        unsigned long major = libvirtVersion / 1000000;
        unsigned long minor = (libvirtVersion % 1000000) / 1000;
        unsigned long micro = libvirtVersion % 1000;
        return QString("Libvirt %1.%2.%3").arg(major).arg(minor).arg(micro);
    }

    return "Libvirt (unknown version)";
}

void Connection::tick()
{
    if (!isOpen()) {
        return;
    }

    m_tickCounter++;

    // Validate connection
    if (virConnectIsAlive(m_conn) != 1) {
        qDebug() << "Connection to" << m_uri << "is no longer alive";
        close();
        return;
    }

    // Initial resource loading
    if (m_initialPoll && m_tickCounter > 2) {
        m_initialPoll = false;
        initAllResources();
    }

    // Poll for changes
    if (m_tickCounter % 2 == 0) {
        pollDomains();
    }

    if (m_tickCounter % 5 == 0) {
        pollNetworks();
        pollStoragePools();
    }

    // Update all domain stats
    for (auto *domain : m_domains) {
        if (domain) {
            domain->updateInfo();
        }
    }
}

void Connection::initAllResources()
{
    qDebug() << "Initializing resources for" << m_uri;

    // List all domains (active and inactive)
    int numDomains = virConnectNumOfDomains(m_conn);
    int numInactiveDomains = virConnectNumOfDefinedDomains(m_conn);

    qDebug() << "Found" << numDomains << "active domains and"
             << numInactiveDomains << "inactive domains";

    // Get active domains
    if (numDomains > 0) {
        int *domainIds = new int[numDomains];
        int actualDomains = virConnectListDomains(m_conn, domainIds, numDomains);
        if (actualDomains >= 0) {
            for (int i = 0; i < actualDomains; i++) {
                virDomainPtr domainPtr = virDomainLookupByID(m_conn, domainIds[i]);
                if (domainPtr) {
                    auto *domain = new Domain(this, domainPtr);
                    m_domains[domain->name()] = domain;
                    emit domainAdded(domain);
                }
            }
        }
        delete[] domainIds;
    }

    // Get inactive domains
    if (numInactiveDomains > 0) {
        char **domainNames = new char *[numInactiveDomains];
        int actualDomains = virConnectListDefinedDomains(m_conn, domainNames, numInactiveDomains);
        if (actualDomains >= 0) {
            for (int i = 0; i < actualDomains; i++) {
                virDomainPtr domainPtr = virDomainLookupByName(m_conn, domainNames[i]);
                if (domainPtr) {
                    auto *domain = new Domain(this, domainPtr);
                    m_domains[domain->name()] = domain;
                    emit domainAdded(domain);
                }
                free(domainNames[i]);
            }
        }
        delete[] domainNames;
    }

    // List networks
    int numNetworks = virConnectNumOfNetworks(m_conn);
    int numInactiveNetworks = virConnectNumOfDefinedNetworks(m_conn);

    qDebug() << "Found" << numNetworks << "active networks and"
             << numInactiveNetworks << "inactive networks";

    // Get active networks
    if (numNetworks > 0) {
        char **networkNames = new char *[numNetworks];
        int actualNetworks = virConnectListNetworks(m_conn, networkNames, numNetworks);
        if (actualNetworks >= 0) {
            for (int i = 0; i < actualNetworks; i++) {
                virNetworkPtr networkPtr = virNetworkLookupByName(m_conn, networkNames[i]);
                if (networkPtr) {
                    auto *network = new Network(this, networkPtr);
                    m_networks[network->name()] = network;
                    emit networkAdded(network);
                }
                free(networkNames[i]);
            }
        }
        delete[] networkNames;
    }

    // Get inactive networks
    if (numInactiveNetworks > 0) {
        char **networkNames = new char *[numInactiveNetworks];
        int actualNetworks = virConnectListDefinedNetworks(m_conn, networkNames, numInactiveNetworks);
        if (actualNetworks >= 0) {
            for (int i = 0; i < actualNetworks; i++) {
                virNetworkPtr networkPtr = virNetworkLookupByName(m_conn, networkNames[i]);
                if (networkPtr) {
                    auto *network = new Network(this, networkPtr);
                    m_networks[network->name()] = network;
                    emit networkAdded(network);
                }
                free(networkNames[i]);
            }
        }
        delete[] networkNames;
    }

    // List storage pools
    int numPools = virConnectNumOfStoragePools(m_conn);
    int numInactivePools = virConnectNumOfDefinedStoragePools(m_conn);

    qDebug() << "Found" << numPools << "active storage pools and"
             << numInactivePools << "inactive storage pools";

    // Get active pools
    if (numPools > 0) {
        char **poolNames = new char *[numPools];
        int actualPools = virConnectListStoragePools(m_conn, poolNames, numPools);
        if (actualPools >= 0) {
            for (int i = 0; i < actualPools; i++) {
                virStoragePoolPtr poolPtr = virStoragePoolLookupByName(m_conn, poolNames[i]);
                if (poolPtr) {
                    auto *pool = new StoragePool(this, poolPtr);
                    m_storagePools[pool->name()] = pool;
                    emit storagePoolAdded(pool);
                }
                free(poolNames[i]);
            }
        }
        delete[] poolNames;
    }

    // Get inactive pools
    if (numInactivePools > 0) {
        char **poolNames = new char *[numInactivePools];
        int actualPools = virConnectListDefinedStoragePools(m_conn, poolNames, numInactivePools);
        if (actualPools >= 0) {
            for (int i = 0; i < actualPools; i++) {
                virStoragePoolPtr poolPtr = virStoragePoolLookupByName(m_conn, poolNames[i]);
                if (poolPtr) {
                    auto *pool = new StoragePool(this, poolPtr);
                    m_storagePools[pool->name()] = pool;
                    emit storagePoolAdded(pool);
                }
                free(poolNames[i]);
            }
        }
        delete[] poolNames;
    }
}

void Connection::pollDomains()
{
    // Check for new or removed domains
    int numDomains = virConnectNumOfDomains(m_conn);
    int numInactiveDomains = virConnectNumOfDefinedDomains(m_conn);

    QSet<QString> currentDomainNames;

    // Check active domains
    if (numDomains > 0) {
        int *domainIds = new int[numDomains];
        int actualDomains = virConnectListDomains(m_conn, domainIds, numDomains);
        if (actualDomains >= 0) {
            for (int i = 0; i < actualDomains; i++) {
                virDomainPtr domainPtr = virDomainLookupByID(m_conn, domainIds[i]);
                if (domainPtr) {
                    const char *name = virDomainGetName(domainPtr);
                    if (name) {
                        QString domainName = QString::fromUtf8(name);
                        currentDomainNames.insert(domainName);

                        if (!m_domains.contains(domainName)) {
                            auto *domain = new Domain(this, domainPtr);
                            m_domains[domainName] = domain;
                            emit domainAdded(domain);
                        } else {
                            virDomainFree(domainPtr);
                        }
                    }
                }
            }
        }
        delete[] domainIds;
    }

    // Check inactive domains
    if (numInactiveDomains > 0) {
        char **domainNames = new char *[numInactiveDomains];
        int actualDomains = virConnectListDefinedDomains(m_conn, domainNames, numInactiveDomains);
        if (actualDomains >= 0) {
            for (int i = 0; i < actualDomains; i++) {
                QString domainName = QString::fromUtf8(domainNames[i]);
                currentDomainNames.insert(domainName);

                if (!m_domains.contains(domainName)) {
                    virDomainPtr domainPtr = virDomainLookupByName(m_conn, domainNames[i]);
                    if (domainPtr) {
                        auto *domain = new Domain(this, domainPtr);
                        m_domains[domainName] = domain;
                        emit domainAdded(domain);
                    }
                }
                free(domainNames[i]);
            }
        }
        delete[] domainNames;
    }

    // Check for removed domains
    QStringList removedDomains;
    for (auto it = m_domains.begin(); it != m_domains.end(); ++it) {
        if (!currentDomainNames.contains(it.key())) {
            removedDomains.append(it.key());
        }
    }

    for (const QString &name : removedDomains) {
        Domain *domain = m_domains.take(name);
        if (domain) {
            emit domainRemoved(domain);
            delete domain;
        }
    }
}

void Connection::pollNetworks()
{
    // Similar to pollDomains but for networks
    // For now, just log
    qDebug() << "Polling networks for" << m_uri;
}

void Connection::pollStoragePools()
{
    // Similar to pollDomains but for storage pools
    // For now, just log
    qDebug() << "Polling storage pools for" << m_uri;
}

} // namespace QVirt
