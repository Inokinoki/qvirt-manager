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

#ifndef QVIRT_CORE_GUESTAGENT_H
#define QVIRT_CORE_GUESTAGENT_H

#include "BaseObject.h"
#include <QString>
#include <QList>

namespace QVirt {

/**
 * @brief Guest OS information
 */
struct GuestOSInfo
{
    QString name;
    QString version;
    QString id;
    QString prettyName;
    QString variant;
    QString variantId;
};

/**
 * @brief Guest filesystem information
 */
struct GuestFilesystem
{
    QString name;
    QString mountpoint;
    QString type;
    unsigned long long totalBytes = 0;
    unsigned long long usedBytes = 0;
};

/**
 * @brief Guest user information
 */
struct GuestUser
{
    QString name;
    bool loggedIn = false;
};

/**
 * @brief QEMU Guest Agent integration
 *
 * Provides guest agent functionality for IP detection, time sync, etc.
 */
class GuestAgent : public BaseObject
{
    Q_OBJECT

public:
    explicit GuestAgent(QObject *parent = nullptr);
    ~GuestAgent() override = default;

    bool connected() const { return m_connected; }
    void setConnected(bool connected);

    QString version() const { return m_version; }
    void setVersion(const QString &version);

    GuestOSInfo osInfo() const { return m_osInfo; }
    void setOsInfo(const GuestOSInfo &info);

    QList<GuestFilesystem> filesystems() const { return m_filesystems; }
    void setFilesystems(const QList<GuestFilesystem> &fs);

    QList<GuestUser> users() const { return m_users; }
    void setUsers(const QList<GuestUser> &users);

    QString hostname() const { return m_hostname; }
    void setHostname(const QString &hostname);

    QList<QString> ipAddresses() const { return m_ipAddresses; }
    void setIpAddresses(const QList<QString> &ips);

    bool timeSyncEnabled() const { return m_timeSyncEnabled; }
    void setTimeSyncEnabled(bool enabled);

    QString toXML() const;
    bool fromXML(const QString &xml);

    bool isEmpty() const;

signals:
    void infoUpdated();
    void configChanged();

private:
    bool m_connected = false;
    QString m_version;
    GuestOSInfo m_osInfo;
    QList<GuestFilesystem> m_filesystems;
    QList<GuestUser> m_users;
    QString m_hostname;
    QList<QString> m_ipAddresses;
    bool m_timeSyncEnabled = false;
};

} // namespace QVirt

#endif // QVIRT_CORE_GUESTAGENT_H
