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

#include "GuestAgent.h"

namespace QVirt {

GuestAgent::GuestAgent(QObject *parent)
    : BaseObject(parent)
{
}

void GuestAgent::setConnected(bool connected)
{
    if (m_connected != connected) {
        m_connected = connected;
        emit infoUpdated();
        emit configChanged();
    }
}

void GuestAgent::setVersion(const QString &version)
{
    if (m_version != version) {
        m_version = version;
        emit infoUpdated();
        emit configChanged();
    }
}

void GuestAgent::setOsInfo(const GuestOSInfo &info)
{
    m_osInfo = info;
    emit infoUpdated();
    emit configChanged();
}

void GuestAgent::setFilesystems(const QList<GuestFilesystem> &fs)
{
    m_filesystems = fs;
    emit infoUpdated();
    emit configChanged();
}

void GuestAgent::setUsers(const QList<GuestUser> &users)
{
    m_users = users;
    emit infoUpdated();
    emit configChanged();
}

void GuestAgent::setHostname(const QString &hostname)
{
    if (m_hostname != hostname) {
        m_hostname = hostname;
        emit infoUpdated();
        emit configChanged();
    }
}

void GuestAgent::setIpAddresses(const QList<QString> &ips)
{
    m_ipAddresses = ips;
    emit infoUpdated();
    emit configChanged();
}

void GuestAgent::setTimeSyncEnabled(bool enabled)
{
    if (m_timeSyncEnabled != enabled) {
        m_timeSyncEnabled = enabled;
        emit configChanged();
    }
}

QString GuestAgent::toXML() const
{
    if (isEmpty()) {
        return QString();
    }

    QString xml;
    xml += QLatin1String("<guestAgent>\n");
    xml += QLatin1String("  <connected>") + (m_connected ? QLatin1String("yes") : QLatin1String("no")) + QLatin1String("</connected>\n");
    if (!m_version.isEmpty()) {
        xml += QLatin1String("  <version>") + m_version + QLatin1String("</version>\n");
    }
    if (!m_hostname.isEmpty()) {
        xml += QLatin1String("  <hostname>") + m_hostname + QLatin1String("</hostname>\n");
    }
    xml += QLatin1String("</guestAgent>");
    return xml;
}

bool GuestAgent::fromXML(const QString &xml)
{
    Q_UNUSED(xml);
    return true;
}

bool GuestAgent::isEmpty() const
{
    return !m_connected && m_version.isEmpty();
}

} // namespace QVirt
