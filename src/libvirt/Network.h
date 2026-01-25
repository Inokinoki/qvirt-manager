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

#ifndef QVIRT_LIBVIRT_NETWORK_H
#define QVIRT_LIBVIRT_NETWORK_H

#include "../core/BaseObject.h"
#include <QString>

#ifdef LIBVIRT_FOUND
#include <libvirt/libvirt.h>

// Windows.h defines 'state' as a macro which breaks our code
#ifdef _WIN32
#undef state
#endif
#else
// Forward declarations for when libvirt is not available
typedef void *virNetworkPtr;
#endif // LIBVIRT_FOUND

namespace QVirt {

class Connection;

/**
 * @brief libvirt network wrapper
 */
class Network : public BaseObject
{
    Q_OBJECT

public:
    enum NetworkState {
        StateInactive = 0,
        StateRunning = 1
    };

    enum ForwardMode {
        ForwardNAT = 0,
        ForwardRoute = 1,
        ForwardBridge = 2,
        ForwardPrivate = 3,
        ForwardVEPA = 4,
        ForwardPassthrough = 5,
        ForwardHostdev = 6
    };

    ~Network() override;

    QString name() const { return m_name; }
    QString uuid() const { return m_uuid; }
    bool isActive() const { return m_active; }
    NetworkState state() const { return m_state; }
    ForwardMode forwardMode() const { return m_forwardMode; }

    // Network configuration
    QString bridgeName() const { return m_bridgeName; }
    QString ipAddress() const { return m_ipAddress; }
    QString netmask() const { return m_netmask; }
    QString dhcpStart() const { return m_dhcpStart; }
    QString dhcpEnd() const { return m_dhcpEnd; }
    bool dhcpEnabled() const { return m_dhcpEnabled; }

    bool start();
    bool stop();
    bool undefine();
    void updateInfo();

signals:
    void stateChanged();

private:
    Network(Connection *conn, virNetworkPtr network);
    void parseXML(const QString &xml);

    Connection *m_connection;
    virNetworkPtr m_network;

    QString m_name;
    QString m_uuid;
    bool m_active;
    NetworkState m_state;
    ForwardMode m_forwardMode;

    QString m_bridgeName;
    QString m_ipAddress;
    QString m_netmask;
    QString m_dhcpStart;
    QString m_dhcpEnd;
    bool m_dhcpEnabled;

    friend class Connection;
};

} // namespace QVirt

#endif // QVIRT_LIBVIRT_NETWORK_H
