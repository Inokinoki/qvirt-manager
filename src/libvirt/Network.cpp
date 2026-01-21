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

#include "Network.h"
#include "Connection.h"
#include <QDebug>

namespace QVirt {

Network::Network(Connection *conn, virNetworkPtr network)
    : BaseObject(conn)
    , m_connection(conn)
    , m_network(network)
    , m_active(false)
{
    const char *name = virNetworkGetName(m_network);
    if (name) {
        m_name = QString::fromUtf8(name);
    }

    char uuid[VIR_UUID_STRING_BUFLEN];
    if (virNetworkGetUUIDString(m_network, uuid) == 0) {
        m_uuid = QString::fromUtf8(uuid);
    }

    // Check if network is active using isActive function
    m_active = (virNetworkIsActive(m_network) == 1);

    qDebug().noquote() << "Created Network wrapper for" << m_name;
}

Network::~Network()
{
    if (m_network) {
        virNetworkFree(m_network);
        m_network = nullptr;
    }
}

bool Network::start()
{
    if (!m_network) {
        return false;
    }

    if (virNetworkCreate(m_network) < 0) {
        return false;
    }

    m_active = true;
    emit stateChanged();
    return true;
}

bool Network::stop()
{
    if (!m_network) {
        return false;
    }

    if (virNetworkDestroy(m_network) < 0) {
        return false;
    }

    m_active = false;
    emit stateChanged();
    return true;
}

bool Network::undefine()
{
    if (!m_network) {
        return false;
    }

    if (virNetworkUndefine(m_network) < 0) {
        return false;
    }

    return true;
}

} // namespace QVirt
