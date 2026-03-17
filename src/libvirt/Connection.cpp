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
#include "../core/Config.h"
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>

#ifdef LIBVIRT_FOUND
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>

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
            // For SSH key auth, SSH will use the key from ssh-agent or SSH config
            // The sshKeyPath should be configured via SSH config or the key should be loaded into ssh-agent
            continue;
        } else {
            // Unsupported credential type
            return -1;
        }
    }
    return 0;
}

// Auth callback struct for libvirt
static virConnectAuth authHelper = {
    .credtype = nullptr,
    .ncredtype = 0,
    .cb = virConnectAuthCallback,
    .cbdata = nullptr
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
    , m_pollingEnabled(true)
    , m_lastCPUTime(0)
    , m_lastIdleTime(0)
    , m_lastCPUUsage(0)
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
    , m_pollingEnabled(true)
    , m_sshKeyPath(sshKeyPath)
    , m_lastCPUTime(0)
    , m_lastIdleTime(0)
    , m_lastCPUUsage(0)
{
#ifdef LIBVIRT_FOUND
    // Set up authentication data
    ConnectionAuthData authData;
    authData.password = password;
    authData.sshKeyPath = sshKeyPath;

    // For SSH key authentication with libvirt's SSH transport:
    // libvirt uses the standard SSH client internally.
    // We can configure SSH to use a specific key by setting the SSH command
    // with the -i option via environment variables.
    //
    // libvirt checks these in order:
    // 1. LIBVIRT_SSH - custom SSH command for libvirt
    // 2. SSH - generic SSH command
    // 3. Default: ssh
    //
    // We'll use LIBVIRT_SSH to specify the key file directly.
    QByteArray originalLibvirtSsh;
    if (!sshKeyPath.isEmpty()) {
        originalLibvirtSsh = qgetenv("LIBVIRT_SSH");
        // Use ssh with the specified key, disabling other auth methods for efficiency
        // Quote the key path to handle spaces
        QString sshCmd = QString("ssh -i \"%1\" -o IdentitiesOnly=yes -o BatchMode=no")
            .arg(sshKeyPath);
        qputenv("LIBVIRT_SSH", sshCmd.toUtf8());
        qDebug() << "Set LIBVIRT_SSH to:" << sshCmd;
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
        if (!sshKeyPath.isEmpty()) {
            qWarning() << "SSH key was specified:" << sshKeyPath;
            qWarning() << "Ensure the key is valid and has correct permissions";
        }
    }

    // Restore original LIBVIRT_SSH environment variable
    if (!sshKeyPath.isEmpty()) {
        if (originalLibvirtSsh.isEmpty()) {
            qunsetenv("LIBVIRT_SSH");
        } else {
            qputenv("LIBVIRT_SSH", originalLibvirtSsh);
        }
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

Connection *Connection::create(const QString &uri)
{
    return new Connection(uri);
}

Connection *Connection::createDisconnected(const QString &uri)
{
    return new Connection(uri, true);  // Internal constructor, no connection attempt
}

// Internal constructor - creates Connection object without attempting to connect
Connection::Connection(const QString &uri, bool /* internal */)
    : BaseObject()
    , m_uri(uri)
    , m_state(Disconnected)
    , m_conn(nullptr)
    , m_tickCounter(0)
    , m_initialPoll(true)
    , m_pollingEnabled(true)
    , m_lastCPUTime(0)
    , m_lastIdleTime(0)
    , m_lastCPUUsage(0)
{
    // Do not attempt to open connection - used for cached VM display only
}

void Connection::openAsync(const QString &sshKeyPath, const QString &password)
{
    m_state = Connecting;
    emit stateChanged(m_state);

    auto *watcher = new QFutureWatcher<bool>(this);

    connect(watcher, &QFutureWatcher<bool>::finished, this, [this, watcher]() {
        bool success = watcher->result();

        if (success) {
            m_state = Active;
            m_connectionError.clear();
            emit connectionProgress(tr("Connection established"));
            qDebug() << "Async connection to" << m_uri << "succeeded";
        } else {
            m_state = ConnectionFailed;
            if (m_connectionError.isEmpty()) {
                m_connectionError = tr("Connection timed out or failed");
            }
            qWarning() << "Async connection to" << m_uri << "failed:" << m_connectionError;
        }

        emit stateChanged(m_state);
        watcher->deleteLater();
    });

    QFuture<bool> future = QtConcurrent::run([this, sshKeyPath, password]() -> bool {
#ifdef LIBVIRT_FOUND
        virConnectPtr conn = nullptr;

        // Extract hostname for status messages
        QString hostname = m_uri;
        if (hostname.contains("://")) {
            hostname = hostname.split("://")[1];
            if (hostname.contains("/")) {
                hostname = hostname.split("/")[0];
            }
            if (hostname.contains("@")) {
                hostname = hostname.split("@")[1];
            }
            if (hostname.contains(":")) {
                hostname = hostname.split(":")[0];
            }
        }

        if (sshKeyPath.isEmpty() && password.isEmpty()) {
            emit connectionProgress(tr("Connecting to %1...").arg(m_uri));
            conn = virConnectOpen(m_uri.toUtf8().constData());
        } else {
            ConnectionAuthData authData;
            authData.password = password;
            authData.sshKeyPath = sshKeyPath;

            // Show SSH connection status
            emit connectionProgress(tr("Connecting via SSH to %1...").arg(hostname));

            QByteArray originalLibvirtSsh;
            if (!sshKeyPath.isEmpty()) {
                originalLibvirtSsh = qgetenv("LIBVIRT_SSH");
                QString sshCmd = QString("ssh -i \"%1\" -o IdentitiesOnly=yes -o BatchMode=no -o ConnectTimeout=10")
                    .arg(sshKeyPath);
                qputenv("LIBVIRT_SSH", sshCmd.toUtf8());
                emit connectionProgress(tr("Authenticating with SSH key..."));
            }

            authHelper.cbdata = &authData;
            emit connectionProgress(tr("Connecting to libvirt..."));
            conn = virConnectOpenAuth(m_uri.toUtf8().constData(), &authHelper, 0);

            if (!sshKeyPath.isEmpty()) {
                if (originalLibvirtSsh.isEmpty()) {
                    qunsetenv("LIBVIRT_SSH");
                } else {
                    qputenv("LIBVIRT_SSH", originalLibvirtSsh);
                }
            }
        }

        if (conn) {
            virConnectClose(conn);
            return true;
        } else {
            virErrorPtr err = virGetLastError();
            if (err) {
                m_connectionError = QString::fromUtf8(err->message);
            }
            return false;
        }
#else
        m_connectionError = tr("libvirt not available");
        return false;
#endif
    });

    watcher->setFuture(future);
}

bool Connection::isOpen() const
{
    return m_conn != nullptr && m_state == Active;
}

void Connection::refresh()
{
    if (!isOpen()) {
        return;
    }
    initAllResources();
}


void Connection::close()
{
    if (!m_conn) {
        return;  // Already closed
    }

    // Save VM cache before closing
    saveVMCache();

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

Domain *Connection::defineDomain(const QString &xml, QString *errorOutput)
{
#ifdef LIBVIRT_FOUND
    if (!m_conn) {
        if (errorOutput) {
            *errorOutput = tr("Connection is not open");
        }
        return nullptr;
    }

    qDebug() << "defineDomain: Connection URI:" << m_uri;
    qDebug() << "defineDomain: Connection state:" << state();
    qDebug() << "defineDomain: XML length:" << xml.length();

    // Define the domain from XML
    virDomainPtr domainPtr = virDomainDefineXML(m_conn, xml.toUtf8().constData());
    if (!domainPtr) {
        virErrorPtr err = virGetLastError();
        if (errorOutput && err) {
            *errorOutput = QString::fromUtf8(err->message);
        } else if (errorOutput) {
            *errorOutput = tr("Failed to define domain");
        }
        qWarning() << "defineDomain: virDomainDefineXML failed:" << (err ? QString::fromUtf8(err->message) : "unknown error");
        qWarning() << "defineDomain: XML was:\n" << xml;
        return nullptr;
    }

    // Get the domain name from the pointer
    const char *name = virDomainGetName(domainPtr);
    if (!name || QString::fromUtf8(name).isEmpty()) {
        if (errorOutput) {
            *errorOutput = tr("Defined domain has no name");
        }
        qWarning() << "defineDomain: Defined domain has no name, freeing";
        virDomainFree(domainPtr);
        return nullptr;
    }

    // Check if domain already exists in cache
    if (m_domains.contains(QString::fromUtf8(name))) {
        if (errorOutput) {
            *errorOutput = tr("Domain '%1' already exists").arg(QString::fromUtf8(name));
        }
        qWarning() << "defineDomain: Domain" << name << "already exists in cache";
        virDomainFree(domainPtr);
        return nullptr;
    }

    // Create Domain object and add to cache
    auto *domain = new Domain(this, domainPtr);
    m_domains[domain->name()] = domain;

    qDebug() << "defineDomain: Defined new domain:" << domain->name() << "with state:" << domain->state();
    qDebug() << "defineDomain: Emitting domainAdded signal for:" << domain->name();
    emit domainAdded(domain);
    qDebug() << "defineDomain: domainAdded signal emitted, domain cache now has" << m_domains.count() << "domains";

    return domain;
#else
    Q_UNUSED(xml);
    if (errorOutput) {
        *errorOutput = tr("libvirt not available");
    }
    return nullptr;
#endif
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

int Connection::getHostCPUUsage()
{
#ifdef LIBVIRT_FOUND
    if (!m_conn) {
        return 0;
    }

    // Get CPU stats for all CPUs (-1 means all CPUs)
    int nparams = 0;
    if (virNodeGetCPUStats(m_conn, -1, nullptr, &nparams, 0) < 0 || nparams <= 0) {
        return 0;
    }

    virNodeCPUStats *params = new virNodeCPUStats[nparams];
    memset(params, 0, sizeof(virNodeCPUStats) * nparams);
    
    if (virNodeGetCPUStats(m_conn, -1, params, &nparams, 0) < 0) {
        delete[] params;
        return 0;
    }

    // Calculate total CPU time and idle time
    // Fields from libvirt: kernel, user, idle, iowait
    unsigned long long totalTime = 0;
    unsigned long long idleTime = 0;
    unsigned long long userTime = 0;
    unsigned long long kernelTime = 0;
    unsigned long long iowaitTime = 0;

    for (int i = 0; i < nparams; i++) {
        if (strcmp(params[i].field, "idle") == 0) {
            idleTime = params[i].value;
        } else if (strcmp(params[i].field, "user") == 0) {
            userTime = params[i].value;
        } else if (strcmp(params[i].field, "kernel") == 0) {
            kernelTime = params[i].value;
        } else if (strcmp(params[i].field, "iowait") == 0) {
            iowaitTime = params[i].value;
        }
    }

    delete[] params;

    // Calculate total time from all fields
    totalTime = userTime + kernelTime + idleTime + iowaitTime;

    // If we have cached values, calculate percentage
    if (m_lastCPUTime > 0 && totalTime > m_lastCPUTime) {
        unsigned long long totalDiff = totalTime - m_lastCPUTime;
        unsigned long long idleDiff = idleTime - m_lastIdleTime;

        if (totalDiff > 0) {
            int usage = 100 - ((idleDiff * 100) / totalDiff);
            if (usage < 0) usage = 0;
            if (usage > 100) usage = 100;
            m_lastCPUUsage = usage;
        }
    }

    // Cache values for next calculation
    m_lastCPUTime = totalTime;
    m_lastIdleTime = idleTime;

    return m_lastCPUUsage;
#else
    return 0;
#endif
}

unsigned long long Connection::getHostMemoryTotal()
{
#ifdef LIBVIRT_FOUND
    if (!m_conn) {
        return 0;
    }

    virNodeMemoryStats *params = nullptr;
    int nparams = 0;

    // First call to get number of parameters
    if (virNodeGetMemoryStats(m_conn, VIR_NODE_MEMORY_STATS_ALL_CELLS, nullptr, &nparams, 0) < 0) {
        return 0;
    }

    if (nparams <= 0) {
        return 0;
    }

    params = new virNodeMemoryStats[nparams];
    if (virNodeGetMemoryStats(m_conn, VIR_NODE_MEMORY_STATS_ALL_CELLS, params, &nparams, 0) < 0) {
        delete[] params;
        return 0;
    }

    unsigned long long total = 0;
    for (int i = 0; i < nparams; i++) {
        if (strcmp(params[i].field, "total") == 0) {
            total = params[i].value;
            break;
        }
    }

    delete[] params;
    return total;
#else
    return 0;
#endif
}

unsigned long long Connection::getHostMemoryUsed()
{
#ifdef LIBVIRT_FOUND
    if (!m_conn) {
        return 0;
    }

    virNodeMemoryStats *params = nullptr;
    int nparams = 0;

    // First call to get number of parameters
    if (virNodeGetMemoryStats(m_conn, VIR_NODE_MEMORY_STATS_ALL_CELLS, nullptr, &nparams, 0) < 0) {
        return 0;
    }

    if (nparams <= 0) {
        return 0;
    }

    params = new virNodeMemoryStats[nparams];
    if (virNodeGetMemoryStats(m_conn, VIR_NODE_MEMORY_STATS_ALL_CELLS, params, &nparams, 0) < 0) {
        delete[] params;
        return 0;
    }

    unsigned long long total = 0;
    unsigned long long free = 0;

    for (int i = 0; i < nparams; i++) {
        if (strcmp(params[i].field, "total") == 0) {
            total = params[i].value;
        } else if (strcmp(params[i].field, "free") == 0) {
            free = params[i].value;
        }
    }

    delete[] params;
    return (total > free) ? (total - free) : 0;
#else
    return 0;
#endif
}

void Connection::tick()
{
    if (!isOpen()) {
        return;
    }

    // Skip polling if disabled (e.g., when modal dialog is open)
    if (!m_pollingEnabled) {
        return;
    }

    m_tickCounter++;

    // Validate connection
    if (virConnectIsAlive(m_conn) != 1) {
        qDebug() << "Connection to" << m_uri << "is no longer alive";
        close();
        return;
    }

    // Initial resource loading - run only once, before any polling
    if (m_initialPoll && m_tickCounter > 1) {
        m_initialPoll = false;
        qDebug() << "Starting initial resource loading...";
        m_pollingEnabled = false;  // Disable polling during init
        initAllResources();
        m_pollingEnabled = true;   // Re-enable after init
        qDebug() << "Initial resource loading completed";
        return;  // Skip polling on this tick
    }

    // Skip polling if disabled
    if (!m_pollingEnabled) {
        return;
    }

    // Poll for changes
    if (m_tickCounter % 2 == 0) {
        pollDomains();
    }

    if (m_tickCounter % 5 == 0) {
        pollNetworks();
        pollStoragePools();
    }

    // Update all domain stats - only if we have domains
    if (!m_domains.isEmpty()) {
        for (auto *domain : m_domains) {
            if (domain) {
                domain->updateInfo();
            }
        }
    }
}

void Connection::initAllResources()
{
    qDebug() << "Initializing resources for" << m_uri;

    // Clear existing cache before repopulating
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

    // Only load from cache if connection is not open
    // When connected, we load live data directly to avoid duplicate emissions
    if (!isOpen()) {
        loadVMCache();
        qDebug() << "Connection not open, only loaded cached VMs for" << m_uri;
        return;
    }

    // List all domains (active and inactive)
    int numDomains = virConnectNumOfDomains(m_conn);
    int numInactiveDomains = virConnectNumOfDefinedDomains(m_conn);

    qDebug() << "Found" << numDomains << "active domains and"
             << numInactiveDomains << "inactive domains";

    // Check for errors (negative return values indicate failure)
    if (numDomains < 0) {
        qWarning() << "Failed to get active domains:" << numDomains;
        numDomains = 0;
    }
    if (numInactiveDomains < 0) {
        qWarning() << "Failed to get inactive domains:" << numInactiveDomains;
        numInactiveDomains = 0;
    }

    // Track which cached domains we've found to remove stale entries later
    QSet<QString> foundDomainNames;

    // Get active domains
    if (numDomains > 0) {
        int *domainIds = new int[numDomains];
        int actualDomains = virConnectListDomains(m_conn, domainIds, numDomains);
        if (actualDomains >= 0) {
            qDebug() << "Listing" << actualDomains << "active domains";
            for (int i = 0; i < actualDomains; i++) {
                virDomainPtr domainPtr = virDomainLookupByID(m_conn, domainIds[i]);
                if (domainPtr) {
                    const char *name = virDomainGetName(domainPtr);
                    if (!name || QString::fromUtf8(name).isEmpty()) {
                        qWarning() << "Domain ID" << domainIds[i] << "has no name, skipping";
                        virDomainFree(domainPtr);
                        continue;
                    }
                    QString domainName = QString::fromUtf8(name);
                    foundDomainNames.insert(domainName);

                    // Check if domain exists in cache
                    auto *existingDomain = m_domains.value(domainName, nullptr);
                    if (existingDomain) {
                        // Update existing cached domain with fresh data
                        qDebug() << "Updating cached domain:" << domainName;
                        delete existingDomain;
                        m_domains.remove(domainName);
                    }

                    auto *domain = new Domain(this, domainPtr);
                    if (domain->name().isEmpty()) {
                        qWarning() << "Domain wrapper created with empty name, skipping";
                        delete domain;
                        continue;
                    }
                    // Add domain regardless of state - newly created VMs may have StateNoState temporarily
                    m_domains[domain->name()] = domain;
                    qDebug() << "Added active domain:" << domain->name();
                    emit domainAdded(domain);
                } else {
                    qWarning() << "Failed to look up domain by ID:" << domainIds[i];
                }
            }
        } else {
            qWarning() << "Failed to list active domains:" << actualDomains;
        }
        delete[] domainIds;
    }

    // Get inactive domains
    if (numInactiveDomains > 0) {
        char **domainNames = new char *[numInactiveDomains];
        int actualDomains = virConnectListDefinedDomains(m_conn, domainNames, numInactiveDomains);
        if (actualDomains >= 0) {
            qDebug() << "Listing" << actualDomains << "inactive domains";
            for (int i = 0; i < actualDomains; i++) {
                virDomainPtr domainPtr = virDomainLookupByName(m_conn, domainNames[i]);
                if (domainPtr) {
                    const char *name = virDomainGetName(domainPtr);
                    if (!name || QString::fromUtf8(name).isEmpty()) {
                        qWarning() << "Domain name" << domainNames[i] << "is empty, skipping";
                        virDomainFree(domainPtr);
                        continue;
                    }
                    QString domainName = QString::fromUtf8(name);
                    foundDomainNames.insert(domainName);

                    // Check if domain exists in cache
                    auto *existingDomain = m_domains.value(domainName, nullptr);
                    if (existingDomain) {
                        // Update existing cached domain with fresh data
                        qDebug() << "Updating cached domain:" << domainName;
                        delete existingDomain;
                        m_domains.remove(domainName);
                    }

                    auto *domain = new Domain(this, domainPtr);
                    if (domain->name().isEmpty()) {
                        qWarning() << "Domain wrapper created with empty name, skipping";
                        delete domain;
                        continue;
                    }
                    // Add domain regardless of state - newly created VMs may have StateNoState temporarily
                    m_domains[domain->name()] = domain;
                    qDebug() << "Added inactive domain:" << domain->name();
                    emit domainAdded(domain);
                } else {
                    qWarning() << "Failed to look up domain by name:" << domainNames[i];
                }
                free(domainNames[i]);
            }
        } else {
            qWarning() << "Failed to list inactive domains:" << actualDomains;
        }
        delete[] domainNames;
    }

    // Remove cached domains that no longer exist on the host
    QStringList staleDomains;
    for (auto it = m_domains.begin(); it != m_domains.end(); ++it) {
        if (!foundDomainNames.contains(it.key())) {
            staleDomains.append(it.key());
        }
    }

    for (const QString &name : staleDomains) {
        Domain *domain = m_domains.take(name);
        if (domain) {
            qDebug() << "Removing stale cached domain:" << name;
            emit domainRemoved(domain);
            delete domain;
        }
    }

    if (m_domains.isEmpty()) {
        qDebug() << "No domains found on remote host";
    } else {
        qDebug() << "Total domains loaded:" << m_domains.count();
    }

    // Save updated cache after refreshing from libvirt
    saveVMCache();

    // List networks
    int numNetworks = virConnectNumOfNetworks(m_conn);
    int numInactiveNetworks = virConnectNumOfDefinedNetworks(m_conn);

    qDebug() << "Found" << numNetworks << "active networks and"
             << numInactiveNetworks << "inactive networks";

    // Check for errors
    if (numNetworks < 0) {
        qWarning() << "Failed to get active networks:" << numNetworks;
        numNetworks = 0;
    }
    if (numInactiveNetworks < 0) {
        qWarning() << "Failed to get inactive networks:" << numInactiveNetworks;
        numInactiveNetworks = 0;
    }

    // Get active networks
    if (numNetworks > 0) {
        char **networkNames = new char *[numNetworks];
        int actualNetworks = virConnectListNetworks(m_conn, networkNames, numNetworks);
        if (actualNetworks >= 0) {
            qDebug() << "Listing" << actualNetworks << "active networks";
            for (int i = 0; i < actualNetworks; i++) {
                virNetworkPtr networkPtr = virNetworkLookupByName(m_conn, networkNames[i]);
                if (networkPtr) {
                    auto *network = new Network(this, networkPtr);
                    m_networks[network->name()] = network;
                    qDebug() << "Added network:" << network->name();
                    emit networkAdded(network);
                }
                free(networkNames[i]);
            }
        } else {
            qWarning() << "Failed to list active networks:" << actualNetworks;
        }
        delete[] networkNames;
    }

    // Get inactive networks
    if (numInactiveNetworks > 0) {
        char **networkNames = new char *[numInactiveNetworks];
        int actualNetworks = virConnectListDefinedNetworks(m_conn, networkNames, numInactiveNetworks);
        if (actualNetworks >= 0) {
            qDebug() << "Listing" << actualNetworks << "inactive networks";
            for (int i = 0; i < actualNetworks; i++) {
                virNetworkPtr networkPtr = virNetworkLookupByName(m_conn, networkNames[i]);
                if (networkPtr) {
                    auto *network = new Network(this, networkPtr);
                    m_networks[network->name()] = network;
                    qDebug() << "Added inactive network:" << network->name();
                    emit networkAdded(network);
                }
                free(networkNames[i]);
            }
        } else {
            qWarning() << "Failed to list inactive networks:" << actualNetworks;
        }
        delete[] networkNames;
    }

    // List storage pools
    int numPools = virConnectNumOfStoragePools(m_conn);
    int numInactivePools = virConnectNumOfDefinedStoragePools(m_conn);

    qDebug() << "Found" << numPools << "active storage pools and"
             << numInactivePools << "inactive storage pools";

    // Check for errors
    if (numPools < 0) {
        qWarning() << "Failed to get active storage pools:" << numPools;
        numPools = 0;
    }
    if (numInactivePools < 0) {
        qWarning() << "Failed to get inactive storage pools:" << numInactivePools;
        numInactivePools = 0;
    }

    // Get active pools
    if (numPools > 0) {
        char **poolNames = new char *[numPools];
        int actualPools = virConnectListStoragePools(m_conn, poolNames, numPools);
        if (actualPools >= 0) {
            qDebug() << "Listing" << actualPools << "active storage pools";
            for (int i = 0; i < actualPools; i++) {
                QString poolName = QString::fromUtf8(poolNames[i]);
                if (poolName.isEmpty()) {
                    qWarning() << "Active storage pool at index" << i << "has empty name, skipping";
                    free(poolNames[i]);
                    continue;
                }
                virStoragePoolPtr poolPtr = virStoragePoolLookupByName(m_conn, poolNames[i]);
                if (poolPtr) {
                    auto *pool = new StoragePool(this, poolPtr);
                    if (pool->name().isEmpty()) {
                        qWarning() << "StoragePool created with empty name, skipping";
                        delete pool;
                        free(poolNames[i]);
                        continue;
                    }
                    m_storagePools[pool->name()] = pool;
                    qDebug() << "Added storage pool:" << pool->name();
                    emit storagePoolAdded(pool);
                }
                free(poolNames[i]);
            }
        } else {
            qWarning() << "Failed to list active storage pools:" << actualPools;
        }
        delete[] poolNames;
    }

    // Get inactive pools
    if (numInactivePools > 0) {
        char **poolNames = new char *[numInactivePools];
        int actualPools = virConnectListDefinedStoragePools(m_conn, poolNames, numInactivePools);
        if (actualPools >= 0) {
            qDebug() << "Listing" << actualPools << "inactive storage pools";
            for (int i = 0; i < actualPools; i++) {
                QString poolName = QString::fromUtf8(poolNames[i]);
                if (poolName.isEmpty()) {
                    qWarning() << "Inactive storage pool at index" << i << "has empty name, skipping";
                    free(poolNames[i]);
                    continue;
                }
                virStoragePoolPtr poolPtr = virStoragePoolLookupByName(m_conn, poolNames[i]);
                if (poolPtr) {
                    auto *pool = new StoragePool(this, poolPtr);
                    if (pool->name().isEmpty()) {
                        qWarning() << "StoragePool created with empty name, skipping";
                        delete pool;
                        free(poolNames[i]);
                        continue;
                    }
                    m_storagePools[pool->name()] = pool;
                    qDebug() << "Added inactive storage pool:" << pool->name();
                    emit storagePoolAdded(pool);
                }
                free(poolNames[i]);
            }
        } else {
            qWarning() << "Failed to list inactive storage pools:" << actualPools;
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
                    if (name && QString::fromUtf8(name).isEmpty()) {
                        qWarning() << "Domain ID" << domainIds[i] << "has empty name, skipping";
                        virDomainFree(domainPtr);
                        continue;
                    }
                    if (name) {
                        QString domainName = QString::fromUtf8(name);
                        currentDomainNames.insert(domainName);

                        if (!m_domains.contains(domainName)) {
                            auto *domain = new Domain(this, domainPtr);
                            // Check if domain has valid name - skip if empty
                            // Don't skip based on state alone as newly created VMs may have StateNoState temporarily
                            if (domain->name().isEmpty()) {
                                qWarning() << "Poll: Domain" << domainName << "has no valid name, skipping";
                                delete domain;
                                virDomainFree(domainPtr);
                                continue;
                            }
                            m_domains[domainName] = domain;
                            qDebug() << "Poll: Added new active domain:" << domainName;
                            emit domainAdded(domain);
                        } else {
                            virDomainFree(domainPtr);
                        }
                    } else {
                        virDomainFree(domainPtr);
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
                if (domainName.isEmpty()) {
                    qWarning() << "Inactive domain name at index" << i << "is empty, skipping";
                    free(domainNames[i]);
                    continue;
                }
                currentDomainNames.insert(domainName);

                if (!m_domains.contains(domainName)) {
                    virDomainPtr domainPtr = virDomainLookupByName(m_conn, domainNames[i]);
                    if (domainPtr) {
                        auto *domain = new Domain(this, domainPtr);
                        // Check if domain has valid name - skip if empty
                        // Don't skip based on state alone as newly created VMs may have StateNoState temporarily
                        if (domain->name().isEmpty()) {
                            qWarning() << "Poll: Domain" << domainName << "has no valid name, skipping";
                            delete domain;
                            virDomainFree(domainPtr);
                            continue;
                        }
                        m_domains[domainName] = domain;
                        qDebug() << "Poll: Added new inactive domain:" << domainName;
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
    // Check for new or removed networks
    int numNetworks = virConnectNumOfNetworks(m_conn);
    int numInactiveNetworks = virConnectNumOfDefinedNetworks(m_conn);

    QSet<QString> currentNetworkNames;

    // Check active networks
    if (numNetworks > 0) {
        char **networkNames = new char *[numNetworks];
        int actualNetworks = virConnectListNetworks(m_conn, networkNames, numNetworks);
        if (actualNetworks >= 0) {
            for (int i = 0; i < actualNetworks; i++) {
                QString networkName = QString::fromUtf8(networkNames[i]);
                currentNetworkNames.insert(networkName);

                if (!m_networks.contains(networkName)) {
                    virNetworkPtr networkPtr = virNetworkLookupByName(m_conn, networkNames[i]);
                    if (networkPtr) {
                        auto *network = new Network(this, networkPtr);
                        m_networks[networkName] = network;
                        emit networkAdded(network);
                    }
                }
                free(networkNames[i]);
            }
        }
        delete[] networkNames;
    }

    // Check inactive networks
    if (numInactiveNetworks > 0) {
        char **networkNames = new char *[numInactiveNetworks];
        int actualNetworks = virConnectListDefinedNetworks(m_conn, networkNames, numInactiveNetworks);
        if (actualNetworks >= 0) {
            for (int i = 0; i < actualNetworks; i++) {
                QString networkName = QString::fromUtf8(networkNames[i]);
                currentNetworkNames.insert(networkName);

                if (!m_networks.contains(networkName)) {
                    virNetworkPtr networkPtr = virNetworkLookupByName(m_conn, networkNames[i]);
                    if (networkPtr) {
                        auto *network = new Network(this, networkPtr);
                        m_networks[networkName] = network;
                        emit networkAdded(network);
                    }
                }
                free(networkNames[i]);
            }
        }
        delete[] networkNames;
    }

    // Check for removed networks
    QStringList removedNetworks;
    for (auto it = m_networks.begin(); it != m_networks.end(); ++it) {
        if (!currentNetworkNames.contains(it.key())) {
            removedNetworks.append(it.key());
        }
    }

    for (const QString &name : removedNetworks) {
        Network *network = m_networks.take(name);
        if (network) {
            emit networkRemoved(network);
            delete network;
        }
    }
}

void Connection::pollStoragePools()
{
    // Check for new or removed storage pools
    int numPools = virConnectNumOfStoragePools(m_conn);
    int numInactivePools = virConnectNumOfDefinedStoragePools(m_conn);

    QSet<QString> currentPoolNames;

    // Check for errors
    if (numPools < 0) {
        qWarning() << "Failed to get active storage pools:" << numPools;
        numPools = 0;
    }
    if (numInactivePools < 0) {
        qWarning() << "Failed to get inactive storage pools:" << numInactivePools;
        numInactivePools = 0;
    }

    // Check active pools
    if (numPools > 0) {
        char **poolNames = new char *[numPools];
        int actualPools = virConnectListStoragePools(m_conn, poolNames, numPools);
        if (actualPools >= 0) {
            for (int i = 0; i < actualPools; i++) {
                QString poolName = QString::fromUtf8(poolNames[i]);
                if (poolName.isEmpty()) {
                    qWarning() << "Active storage pool name at index" << i << "is empty, skipping";
                    free(poolNames[i]);
                    continue;
                }
                currentPoolNames.insert(poolName);

                if (!m_storagePools.contains(poolName)) {
                    virStoragePoolPtr poolPtr = virStoragePoolLookupByName(m_conn, poolNames[i]);
                    if (poolPtr) {
                        auto *pool = new StoragePool(this, poolPtr);
                        m_storagePools[poolName] = pool;
                        qDebug() << "Poll: Added storage pool:" << poolName;
                        emit storagePoolAdded(pool);
                    }
                }
                free(poolNames[i]);
            }
        } else {
            qWarning() << "Failed to list active storage pools:" << actualPools;
        }
        delete[] poolNames;
    }

    // Check inactive pools
    if (numInactivePools > 0) {
        char **poolNames = new char *[numInactivePools];
        int actualPools = virConnectListDefinedStoragePools(m_conn, poolNames, numInactivePools);
        if (actualPools >= 0) {
            for (int i = 0; i < actualPools; i++) {
                QString poolName = QString::fromUtf8(poolNames[i]);
                if (poolName.isEmpty()) {
                    qWarning() << "Inactive storage pool name at index" << i << "is empty, skipping";
                    free(poolNames[i]);
                    continue;
                }
                currentPoolNames.insert(poolName);

                if (!m_storagePools.contains(poolName)) {
                    virStoragePoolPtr poolPtr = virStoragePoolLookupByName(m_conn, poolNames[i]);
                    if (poolPtr) {
                        auto *pool = new StoragePool(this, poolPtr);
                        m_storagePools[poolName] = pool;
                        qDebug() << "Poll: Added inactive storage pool:" << poolName;
                        emit storagePoolAdded(pool);
                    }
                }
                free(poolNames[i]);
            }
        } else {
            qWarning() << "Failed to list inactive storage pools:" << actualPools;
        }
        delete[] poolNames;
    }

    // Check for removed pools
    QStringList removedPools;
    for (auto it = m_storagePools.begin(); it != m_storagePools.end(); ++it) {
        if (!currentPoolNames.contains(it.key())) {
            removedPools.append(it.key());
        }
    }

    for (const QString &name : removedPools) {
        StoragePool *pool = m_storagePools.take(name);
        if (pool) {
            emit storagePoolRemoved(pool);
            delete pool;
        }
    }
}

// VM Cache management
void Connection::saveVMCache() const
{
    auto *config = Config::instance();
    for (auto *domain : m_domains) {
        if (domain && !domain->uuid().isEmpty()) {
            Domain::CacheInfo domainInfo = domain->toCacheInfo();
            // Convert Domain::CacheInfo to VMCacheInfo
            VMCacheInfo info;
            info.name = domainInfo.name;
            info.uuid = domainInfo.uuid;
            info.state = domainInfo.state;
            info.description = domainInfo.description;
            info.title = domainInfo.title;
            info.memory = domainInfo.memory;
            info.currentMemory = domainInfo.currentMemory;
            info.vcpuCount = domainInfo.vcpuCount;
            info.maxVcpuCount = domainInfo.maxVcpuCount;
            info.xmlDesc = domainInfo.xmlDesc;
            info.lastUpdated = domainInfo.lastUpdated;
            config->saveVMCache(m_uri, domain->uuid(), info);
        }
    }
    qDebug() << "Saved VM cache for connection:" << m_uri;
}

void Connection::loadVMCache()
{
    qWarning() << "=== loadVMCache START ===";
    qWarning() << "Connection URI:" << m_uri;

    auto *config = Config::instance();
    QList<VMCacheInfo> cachedVMs = config->loadAllVMCache(m_uri);

    qWarning() << "Loading" << cachedVMs.size() << "VMs from cache for connection:" << m_uri;
    qWarning() << "Connection m_domains count before load:" << m_domains.count();

    if (cachedVMs.isEmpty()) {
        qWarning() << "WARNING: No cached VMs found for URI:" << m_uri;
    }

    for (const VMCacheInfo &cacheInfo : cachedVMs) {
        qWarning() << "Processing cached VM:" << cacheInfo.name << "(" << cacheInfo.uuid << ")";

        // Check if domain already exists in cache (by UUID)
        bool found = false;
        for (auto *domain : m_domains) {
            if (domain && domain->uuid() == cacheInfo.uuid) {
                found = true;
                break;
            }
        }

        if (found) {
            qWarning() << "Skipping cached VM" << cacheInfo.name << "- already have live data";
            continue;
        }

        // Check if domain exists by name
        if (m_domains.contains(cacheInfo.name)) {
            qWarning() << "Skipping cached VM" << cacheInfo.name << "- already in cache by name";
            continue;
        }

        // Convert VMCacheInfo to Domain::CacheInfo
        Domain::CacheInfo domainCacheInfo;
        domainCacheInfo.name = cacheInfo.name;
        domainCacheInfo.uuid = cacheInfo.uuid;
        domainCacheInfo.state = cacheInfo.state;
        domainCacheInfo.description = cacheInfo.description;
        domainCacheInfo.title = cacheInfo.title;
        domainCacheInfo.memory = cacheInfo.memory;
        domainCacheInfo.vcpuCount = cacheInfo.vcpuCount;
        domainCacheInfo.xmlDesc = cacheInfo.xmlDesc;
        domainCacheInfo.lastUpdated = cacheInfo.lastUpdated;

        // Create domain from cache info
        Domain *domain = Domain::fromCacheInfo(this, domainCacheInfo);
        if (domain) {
            m_domains[cacheInfo.name] = domain;
            qWarning() << "Loaded VM from cache:" << cacheInfo.name << "(" << cacheInfo.uuid << ")";
            emit domainAdded(domain);
        } else {
            qWarning() << "FAILED to create domain from cache:" << cacheInfo.name;
        }
    }
    qWarning() << "Connection m_domains count after load:" << m_domains.count();
    qWarning() << "=== loadVMCache END ===";
}

void Connection::clearVMCache() const
{
    auto *config = Config::instance();
    config->clearVMCache(m_uri);
    qDebug() << "Cleared VM cache for connection:" << m_uri;
}

} // namespace QVirt
