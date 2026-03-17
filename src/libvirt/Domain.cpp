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

#include "Domain.h"
#include "DomainSnapshot.h"
#include "Connection.h"
#include "EnumMapper.h"
#include "../core/Error.h"
#include <QDebug>
#include <QDomDocument>
#include <cstring>

namespace QVirt {

Domain::Domain(Connection *conn, virDomainPtr domain)
    : BaseObject(conn)
    , m_connection(conn)
    , m_domain(domain)
    , m_state(StateNoState)
    , m_maxMemory(0)
    , m_currentMemory(0)
    , m_vcpuCount(0)
    , m_maxVcpuCount(0)
    , m_cpuTime(0)
    , m_prevCpuTime(0)
    , m_prevCpuTimestamp(0)
    , m_cachedCpuUsage(0.0f)
{
    // Only call libvirt functions if we have a valid virDomainPtr
    // For cached domains (m_domain == nullptr), values will be set by fromCacheInfo()
    if (m_domain) {
        // Get domain name
        const char *name = virDomainGetName(m_domain);
        if (name) {
            m_name = QString::fromUtf8(name);
        }

        // Get domain UUID
        char uuid[VIR_UUID_STRING_BUFLEN];
        if (virDomainGetUUIDString(m_domain, uuid) == 0) {
            m_uuid = QString::fromUtf8(uuid);
        }

        // Get domain ID
        int id = virDomainGetID(m_domain);
        if (id >= 0) {
            m_id = QString::number(id);
        } else {
            m_id = "-";
        }

        // Initialize with current info
        updateInfo();

        qDebug() << "Created Domain wrapper for" << m_name << "(" << m_uuid << ")";
    }
    // For cached domains (m_domain == nullptr), no libvirt calls are made
    // The caller (fromCacheInfo) will set the member variables directly
}

Domain::~Domain()
{
    if (m_domain) {
        virDomainFree(m_domain);
        m_domain = nullptr;
    }
}

void Domain::updateInfo()
{
    if (!m_domain) {
        return;
    }

    // Get domain info
    virDomainInfo info;
    int ret = virDomainGetInfo(m_domain, &info);
    if (ret < 0) {
        qWarning() << "Failed to get domain info for" << m_name << "- domain may be inaccessible";
        // Don't return early - keep the domain but mark as inaccessible
        // However, we should still try to get the state from the domain directly
        // For a newly defined (shut off) VM, virDomainGetInfo may fail but the domain is still valid
        return;
    }

    // Update state
    State newState = static_cast<State>(info.state);
    if (newState != m_state) {
        setState(newState);
    }

    // Update cached values
    m_maxMemory = info.maxMem;
    m_currentMemory = info.memory;
    m_vcpuCount = info.nrVirtCpu;
    m_cpuTime = info.cpuTime;

    // Get max vcpu count
    int maxVcpu = virDomainGetVcpusFlags(m_domain, VIR_DOMAIN_AFFECT_CURRENT);
    if (maxVcpu > 0) {
        m_maxVcpuCount = maxVcpu;
    } else {
        m_maxVcpuCount = m_vcpuCount;  // Fallback to current count
    }

    // Get XML description for additional info
    char *xml = virDomainGetXMLDesc(m_domain, 0);
    if (xml) {
        QString xmlStr = QString::fromUtf8(xml);
        free(xml);

        // Parse description and title from XML
        QDomDocument doc;
        if (doc.setContent(xmlStr)) {
            QDomElement root = doc.documentElement();
            if (root.tagName() == "domain") {
                // Parse description
                QDomNodeList descNodes = root.elementsByTagName("description");
                if (!descNodes.isEmpty()) {
                    m_description = descNodes.at(0).toElement().text();
                }

                // Parse title
                QDomNodeList titleNodes = root.elementsByTagName("title");
                if (!titleNodes.isEmpty()) {
                    m_title = titleNodes.at(0).toElement().text();
                }
            }
        }
    }

    emit statsUpdated();
}

void Domain::setState(State state)
{
    if (m_state != state) {
        m_state = state;
        emit stateChanged(m_state);
    }
}

bool Domain::start()
{
    if (!m_domain) {
        return false;
    }

    if (virDomainCreate(m_domain) < 0) {
        Error::showError(tr("Start Failed"),
                        tr("Failed to start VM '%1'").arg(m_name));
        return false;
    }

    updateInfo();
    return true;
}

bool Domain::stop()
{
    return shutdown();
}

bool Domain::shutdown()
{
    if (!m_domain) {
        return false;
    }

    if (virDomainShutdown(m_domain) < 0) {
        Error::showError(tr("Shutdown Failed"),
                        tr("Failed to shutdown VM '%1'").arg(m_name));
        return false;
    }

    updateInfo();
    return true;
}

bool Domain::reboot()
{
    if (!m_domain) {
        return false;
    }

    if (virDomainReboot(m_domain, 0) < 0) {
        Error::showError(tr("Reboot Failed"),
                        tr("Failed to reboot VM '%1'").arg(m_name));
        return false;
    }

    return true;
}

bool Domain::reset()
{
    if (!m_domain) {
        return false;
    }

    if (virDomainReset(m_domain, 0) < 0) {
        Error::showError(tr("Reset Failed"),
                        tr("Failed to reset VM '%1'").arg(m_name));
        return false;
    }

    return true;
}

bool Domain::destroy()
{
    if (!m_domain) {
        return false;
    }

    if (virDomainDestroy(m_domain) < 0) {
        Error::showError(tr("Destroy Failed"),
                        tr("Failed to forcefully stop VM '%1'").arg(m_name));
        return false;
    }

    updateInfo();
    return true;
}

bool Domain::save(const QString &path)
{
    if (!m_domain) {
        return false;
    }

    if (virDomainSave(m_domain, path.toUtf8().constData()) < 0) {
        Error::showError(tr("Save Failed"),
                        tr("Failed to save VM '%1' to %2").arg(m_name, path));
        return false;
    }

    updateInfo();
    return true;
}

QString Domain::getXMLDesc(unsigned int flags) const
{
    // For cached domains (offline mode), return the cached XML
    if (!m_domain) {
        return m_cachedXmlDesc;
    }

    char *xml = virDomainGetXMLDesc(m_domain, flags);
    if (!xml) {
        return QString();
    }

    QString xmlStr = QString::fromUtf8(xml);
    free(xml);
    return xmlStr;
}

bool Domain::setXML(const QString &xml, unsigned int flags)
{
    if (!m_domain) {
        return false;
    }

    // For persistent XML changes
    if (flags == VIR_DOMAIN_XML_INACTIVE) {
        // This requires defining a new XML
        virDomainPtr newDomain = virDomainDefineXML(
            m_connection->m_conn,
            xml.toUtf8().constData());

        if (!newDomain) {
            Error::showError(tr("XML Update Failed"),
                            tr("Failed to update VM '%1' configuration").arg(m_name));
            return false;
        }

        virDomainFree(newDomain);
        emit configChanged();
        return true;
    }

    return false;
}

bool Domain::attachDevice(const QString &xml)
{
    if (!m_domain) {
        return false;
    }

    if (virDomainAttachDevice(m_domain, xml.toUtf8().constData()) < 0) {
        Error::showError(tr("Device Attach Failed"),
                        tr("Failed to attach device to VM '%1'").arg(m_name));
        return false;
    }

    emit configChanged();
    return true;
}

bool Domain::detachDevice(const QString &xml)
{
    if (!m_domain) {
        return false;
    }

    if (virDomainDetachDevice(m_domain, xml.toUtf8().constData()) < 0) {
        Error::showError(tr("Device Detach Failed"),
                        tr("Failed to detach device from VM '%1'").arg(m_name));
        return false;
    }

    emit configChanged();
    return true;
}

bool Domain::updateDevice(const QString &xml)
{
    if (!m_domain) {
        return false;
    }

    if (virDomainUpdateDeviceFlags(m_domain, xml.toUtf8().constData(),
                                    VIR_DOMAIN_DEVICE_MODIFY_LIVE) < 0) {
        Error::showError(tr("Device Update Failed"),
                        tr("Failed to update device on VM '%1'").arg(m_name));
        return false;
    }

    emit configChanged();
    return true;
}

quint64 Domain::maxMemory() const
{
    return m_maxMemory;
}

quint64 Domain::memory() const
{
    if (!m_domain) {
        return 0;
    }

    virDomainInfo info;
    if (virDomainGetInfo(m_domain, &info) < 0) {
        return 0;
    }

    return info.memory;
}

int Domain::vcpuCount() const
{
    return m_vcpuCount;
}

int Domain::maxVcpuCount() const
{
    if (!m_domain) {
        return m_maxVcpuCount;
    }

    int maxVcpu = virDomainGetVcpusFlags(m_domain, VIR_DOMAIN_AFFECT_CURRENT);
    return maxVcpu > 0 ? maxVcpu : m_vcpuCount;
}

quint64 Domain::cpuTime() const
{
    return m_cpuTime;
}

float Domain::cpuUsage()
{
    // Calculate CPU usage percentage based on delta over time
    if (!m_domain || m_state != StateRunning) {
        m_cachedCpuUsage = 0.0f;
        return m_cachedCpuUsage;
    }

    virDomainInfo info;
    if (virDomainGetInfo(m_domain, &info) < 0) {
        return m_cachedCpuUsage;
    }

    // Get current CPU time (in nanoseconds)
    quint64 currentCpuTime = info.cpuTime;
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();

    // If this is the first call, just store the values
    if (m_prevCpuTimestamp == 0) {
        m_prevCpuTime = currentCpuTime;
        m_prevCpuTimestamp = currentTime;
        m_cachedCpuUsage = 0.0f;
        return m_cachedCpuUsage;
    }

    // Calculate delta
    quint64 cpuTimeDelta = currentCpuTime - m_prevCpuTime;
    qint64 timeDelta = currentTime - m_prevCpuTimestamp;

    // Only update if enough time has passed (at least 100ms)
    if (timeDelta > 100) {
        // Calculate percentage
        // cpuTimeDelta is in nanoseconds, timeDelta is in milliseconds
        // Convert to percentage: (cpuTimeDelta / (timeDelta * 1000000)) / vcpuCount * 100
        float usage = 0.0f;
        if (m_vcpuCount > 0) {
            usage = (static_cast<float>(cpuTimeDelta) / 1000000.0f) / static_cast<float>(timeDelta) / m_vcpuCount * 100.0f;
        }

        // Clamp to reasonable range (0-100% per vCPU, so up to vcpuCount * 100)
        float maxUsage = m_vcpuCount * 100.0f;
        if (usage < 0.0f) usage = 0.0f;
        if (usage > maxUsage) usage = maxUsage;

        m_cachedCpuUsage = usage;

        // Store current values for next calculation
        m_prevCpuTime = currentCpuTime;
        m_prevCpuTimestamp = currentTime;
    }

    return m_cachedCpuUsage;
}

quint64 Domain::currentMemory() const
{
    if (!m_domain) {
        return m_currentMemory;
    }

    virDomainInfo info;
    if (virDomainGetInfo(m_domain, &info) < 0) {
        return m_currentMemory;
    }

    return info.memory;
}

float Domain::diskUsage() const
{
    if (!m_domain) {
        return 0.0f;
    }

    // Get domain XML to find disk devices
    char *xml = virDomainGetXMLDesc(m_domain, 0);
    if (!xml) {
        return 0.0f;
    }

    QString xmlStr = QString::fromUtf8(xml);
    free(xml);

    QDomDocument doc;
    if (!doc.setContent(xmlStr)) {
        return 0.0f;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "domain") {
        return 0.0f;
    }

    // Find all disk devices
    long long totalBytes = 0;
    QDomNodeList diskNodes = root.elementsByTagName("disk");

    for (int i = 0; i < diskNodes.count(); i++) {
        QDomElement diskElem = diskNodes.at(i).toElement();
        QString diskType = diskElem.attribute("type");

        // Only get stats for file and block devices
        if (diskType != "file" && diskType != "block") {
            continue;
        }

        // Get the target device name (e.g., "vda", "sda")
        QDomNodeList targetNodes = diskElem.elementsByTagName("target");
        if (targetNodes.isEmpty()) {
            continue;
        }

        QDomElement targetElem = targetNodes.at(0).toElement();
        QString targetDev = targetElem.attribute("dev");
        if (targetDev.isEmpty()) {
            continue;
        }

        // Query block stats for this disk
        virDomainBlockStatsStruct stats;
        memset(&stats, 0, sizeof(stats));

        if (virDomainBlockStats(m_domain, targetDev.toUtf8().constData(),
                                &stats, sizeof(stats)) < 0) {
            continue;
        }

        // Accumulate read and write bytes
        if (stats.rd_bytes > 0) {
            totalBytes += stats.rd_bytes;
        }
        if (stats.wr_bytes > 0) {
            totalBytes += stats.wr_bytes;
        }
    }

    // Return usage in MB
    return static_cast<float>(totalBytes) / (1024.0f * 1024.0f);
}

float Domain::networkUsage() const
{
    if (!m_domain) {
        return 0.0f;
    }

    // Get domain XML to find network interfaces
    char *xml = virDomainGetXMLDesc(m_domain, 0);
    if (!xml) {
        return 0.0f;
    }

    QString xmlStr = QString::fromUtf8(xml);
    free(xml);

    QDomDocument doc;
    if (!doc.setContent(xmlStr)) {
        return 0.0f;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "domain") {
        return 0.0f;
    }

    // Find all network interface devices
    long long totalBytes = 0;
    QDomNodeList interfaceNodes = root.elementsByTagName("interface");

    for (int i = 0; i < interfaceNodes.count(); i++) {
        QDomElement interfaceElem = interfaceNodes.at(i).toElement();

        // Get the target device name (e.g., "vnet0")
        QDomNodeList targetNodes = interfaceElem.elementsByTagName("target");
        if (targetNodes.isEmpty()) {
            continue;
        }

        QDomElement targetElem = targetNodes.at(0).toElement();
        QString targetDev = targetElem.attribute("dev");
        if (targetDev.isEmpty()) {
            continue;
        }

        // Query interface stats for this network device
        virDomainInterfaceStatsStruct stats;
        memset(&stats, 0, sizeof(stats));

        if (virDomainInterfaceStats(m_domain, targetDev.toUtf8().constData(),
                                    &stats, sizeof(stats)) < 0) {
            continue;
        }

        // Accumulate RX and TX bytes
        if (stats.rx_bytes > 0) {
            totalBytes += stats.rx_bytes;
        }
        if (stats.tx_bytes > 0) {
            totalBytes += stats.tx_bytes;
        }
    }

    // Return usage in MB
    return static_cast<float>(totalBytes) / (1024.0f * 1024.0f);
}

QList<DomainSnapshot*> Domain::snapshots() const
{
    QList<DomainSnapshot*> snapshotList;

    if (!m_domain) {
        return snapshotList;
    }

    // List all snapshots
    int numSnapshots = virDomainSnapshotNum(m_domain, 0);
    if (numSnapshots < 0) {
        return snapshotList;
    }

    // Get snapshot names
    char **names = new char*[numSnapshots];
    numSnapshots = virDomainSnapshotListNames(m_domain, names, numSnapshots, 0);

    for (int i = 0; i < numSnapshots; ++i) {
        virDomainSnapshotPtr snap = virDomainSnapshotLookupByName(m_domain, names[i], 0);
        if (snap) {
            auto *snapshot = new DomainSnapshot(snap, const_cast<Domain*>(this),
                                                const_cast<Domain*>(this));
            snapshotList.append(snapshot);
        }
        free(names[i]);
    }

    delete[] names;

    return snapshotList;
}

DomainSnapshot *Domain::createSnapshot(const QString &xml, unsigned int flags)
{
    if (!m_domain) {
        return nullptr;
    }

    QByteArray xmlBytes = xml.toUtf8();
    virDomainSnapshotPtr snap = virDomainSnapshotCreateXML(m_domain, xmlBytes.constData(), flags);

    if (snap) {
        return new DomainSnapshot(snap, this, this);
    }

    return nullptr;
}

DomainSnapshot *Domain::currentSnapshot() const
{
    if (!m_domain) {
        return nullptr;
    }

    virDomainSnapshotPtr snap = virDomainSnapshotCurrent(m_domain, 0);
    if (snap) {
        return new DomainSnapshot(snap, const_cast<Domain*>(this),
                                  const_cast<Domain*>(this));
    }

    return nullptr;
}

bool Domain::hasCurrentSnapshot() const
{
    if (!m_domain) {
        return false;
    }

    return virDomainHasCurrentSnapshot(m_domain, 0) == 1;
}

// Guest Agent methods - simplified implementation
bool Domain::guestAgentConnected() const
{
#ifdef LIBVIRT_FOUND
    if (!m_domain) return false;
    // Check if guest agent is responding by trying to get guest info
    // This is a simplified check - full implementation requires libvirt >= 6.4
    virDomainInfo info;
    if (virDomainGetInfo(m_domain, &info) == 0) {
        // If domain is running, assume agent might be available
        // Full agent check would require virDomainGuestPing
        return info.state == VIR_DOMAIN_RUNNING;
    }
#endif
    return false;
}

QString Domain::guestAgentVersion() const
{
    // Full implementation requires libvirt >= 6.4
    return QString();
}

bool Domain::guestAgentPing() const
{
#ifdef LIBVIRT_FOUND
    if (!m_domain) return false;
    // virDomainGuestPing requires libvirt >= 6.4
    // For now, just check if domain is running
    virDomainInfo info;
    if (virDomainGetInfo(m_domain, &info) == 0) {
        return info.state == VIR_DOMAIN_RUNNING;
    }
#endif
    return false;
}

QString Domain::guestHostname() const
{
    // Full implementation requires libvirt >= 6.4
    return QString();
}

QString Domain::guestOS() const
{
    // Full implementation requires libvirt >= 6.4
    return QString();
}

QStringList Domain::guestIPAddresses() const
{
    QStringList ips;
    // Full implementation requires libvirt >= 6.4
    return ips;
}

QStringList Domain::guestGetUsers() const
{
    QStringList users;
    // Full implementation requires libvirt >= 6.4
    return users;
}

QMap<QString, Domain::NetworkInterface> Domain::guestGetNetworkInterfaces() const
{
    QMap<QString, NetworkInterface> interfaces;
    // Full implementation requires libvirt >= 6.4
    return interfaces;
}

QList<Domain::Filesystem> Domain::guestGetFilesystems() const
{
    QList<Filesystem> filesystems;
    // Full implementation requires libvirt >= 6.4
    return filesystems;
}

bool Domain::guestAgentShutdown(int timeout)
{
#ifdef LIBVIRT_FOUND
    if (!m_domain) return false;
    // Try graceful shutdown with agent flag if available
    // Fallback to regular shutdown
    Q_UNUSED(timeout);
    return virDomainShutdown(m_domain) == 0;
#else
    Q_UNUSED(timeout);
    return false;
#endif
}

// Serialization for caching
Domain::CacheInfo Domain::toCacheInfo() const
{
    CacheInfo info;
    info.name = m_name;
    info.uuid = m_uuid;
    info.state = static_cast<int>(m_state);
    info.description = m_description;
    info.title = m_title;
    info.memory = m_maxMemory;
    info.currentMemory = m_currentMemory;
    info.vcpuCount = m_vcpuCount;
    info.maxVcpuCount = m_maxVcpuCount;
    // Use cached XML if available (for cached domains), otherwise fetch from libvirt
    info.xmlDesc = m_cachedXmlDesc.isEmpty() ? getXMLDesc() : m_cachedXmlDesc;
    info.lastUpdated = QDateTime::currentMSecsSinceEpoch();
    return info;
}

Domain *Domain::fromCacheInfo(Connection *conn, const CacheInfo &info)
{
    // Create a minimal Domain object from cached info
    // Note: This domain won't have a valid virDomainPtr, so operations requiring
    // libvirt will fail. It's meant for display purposes until the connection
    // is refreshed and the domain is reloaded from libvirt.
    // State is set to StateNoState to indicate the VM is not actively running
    // (the actual state is unknown without a live connection)
    auto *domain = new Domain(conn, nullptr);
    domain->m_name = info.name;
    domain->m_uuid = info.uuid;
    domain->m_state = State::StateNoState;  // Always use StateNoState for cached VMs
    domain->m_description = info.description;
    domain->m_title = info.title;
    domain->m_maxMemory = info.memory;
    domain->m_currentMemory = info.currentMemory;
    domain->m_vcpuCount = info.vcpuCount;
    domain->m_maxVcpuCount = info.maxVcpuCount;
    domain->m_cachedXmlDesc = info.xmlDesc;  // Store cached XML for device display

    // Parse values from XML to fill in missing cache data
    if (!info.xmlDesc.isEmpty()) {
        QDomDocument doc;
        if (doc.setContent(info.xmlDesc)) {
            QDomElement root = doc.documentElement();
            if (root.tagName() == "domain") {
                // Parse memory from <memory> element (in KB)
                QDomNodeList memoryNodes = root.elementsByTagName("memory");
                if (!memoryNodes.isEmpty() && domain->m_maxMemory == 0) {
                    QDomElement memElem = memoryNodes.at(0).toElement();
                    QString unit = memElem.attribute("unit", "KiB");
                    quint64 memValue = memElem.text().toULongLong();
                    // Convert to KB if needed
                    if (unit == "KiB" || unit == "K") {
                        domain->m_maxMemory = memValue;
                    } else if (unit == "MiB" || unit == "M") {
                        domain->m_maxMemory = memValue * 1024;
                    } else if (unit == "GiB" || unit == "G") {
                        domain->m_maxMemory = memValue * 1024 * 1024;
                    } else if (unit == "bytes" || unit == "B") {
                        domain->m_maxMemory = memValue / 1024;
                    }
                }

                // Parse currentMemory from <currentMemory> element (in KB)
                QDomNodeList currentMemoryNodes = root.elementsByTagName("currentMemory");
                if (!currentMemoryNodes.isEmpty() && domain->m_currentMemory == 0) {
                    QDomElement curMemElem = currentMemoryNodes.at(0).toElement();
                    QString unit = curMemElem.attribute("unit", "KiB");
                    quint64 memValue = curMemElem.text().toULongLong();
                    // Convert to KB if needed
                    if (unit == "KiB" || unit == "K") {
                        domain->m_currentMemory = memValue;
                    } else if (unit == "MiB" || unit == "M") {
                        domain->m_currentMemory = memValue * 1024;
                    } else if (unit == "GiB" || unit == "G") {
                        domain->m_currentMemory = memValue * 1024 * 1024;
                    } else if (unit == "bytes" || unit == "B") {
                        domain->m_currentMemory = memValue / 1024;
                    }
                }

                // Parse vcpu from <vcpu> element
                QDomNodeList vcpuNodes = root.elementsByTagName("vcpu");
                if (!vcpuNodes.isEmpty()) {
                    QDomElement vcpuElem = vcpuNodes.at(0).toElement();
                    // Parse current vcpu count
                    if (domain->m_vcpuCount == 0 && !vcpuElem.text().isEmpty()) {
                        domain->m_vcpuCount = vcpuElem.text().toInt();
                    }
                    // Parse max vcpu from 'max' attribute
                    if (domain->m_maxVcpuCount == 0) {
                        QString maxAttr = vcpuElem.attribute("max");
                        if (!maxAttr.isEmpty()) {
                            domain->m_maxVcpuCount = maxAttr.toInt();
                        } else {
                            // Use current value as fallback
                            domain->m_maxVcpuCount = domain->m_vcpuCount;
                        }
                    }
                }
            }
        }
    }

    return domain;
}

} // namespace QVirt
