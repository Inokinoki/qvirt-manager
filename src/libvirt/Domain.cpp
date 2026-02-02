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
    , m_vcpuCount(0)
    , m_cpuTime(0)
{
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
    if (virDomainGetInfo(m_domain, &info) < 0) {
        qWarning() << "Failed to get domain info for" << m_name;
        return;
    }

    // Update state
    State newState = static_cast<State>(info.state);
    if (newState != m_state) {
        setState(newState);
    }

    // Update cached values
    m_maxMemory = info.maxMem;
    m_vcpuCount = info.nrVirtCpu;
    m_cpuTime = info.cpuTime;

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
    if (!m_domain) {
        return QString();
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
        return 0;
    }

    return virDomainGetVcpusFlags(m_domain, VIR_DOMAIN_AFFECT_CURRENT);
}

quint64 Domain::cpuTime() const
{
    return m_cpuTime;
}

float Domain::cpuUsage() const
{
    // Calculate CPU usage percentage
    // This is a simplified calculation - real implementation needs
    // to compare delta between polls
    if (!m_domain || m_state != StateRunning) {
        return 0.0f;
    }

    virDomainInfo info;
    if (virDomainGetInfo(m_domain, &info) < 0) {
        return 0.0f;
    }

    // Very rough approximation - will be improved with proper polling
    return static_cast<float>(info.cpuTime) / 1000000000.0f;  // ns to seconds
}

quint64 Domain::currentMemory() const
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

} // namespace QVirt
