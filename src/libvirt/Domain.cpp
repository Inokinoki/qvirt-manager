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

#include "Domain.h"
#include "Connection.h"
#include "EnumMapper.h"
#include "../core/Error.h"
#include <QDebug>

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
        // TODO: Parse XML for description, title, etc.
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
    // TODO: Implement disk usage calculation
    // Requires querying block stats
    return 0.0f;
}

float Domain::networkUsage() const
{
    // TODO: Implement network usage calculation
    // Requires querying interface stats
    return 0.0f;
}

} // namespace QVirt
