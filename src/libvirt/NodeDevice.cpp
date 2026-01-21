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

#include "NodeDevice.h"
#include "Connection.h"
#include <QDebug>

namespace QVirt {

NodeDevice::NodeDevice(Connection *conn, virNodeDevicePtr device)
    : BaseObject(conn)
    , m_connection(conn)
    , m_device(device)
{
    const char *name = virNodeDeviceGetName(m_device);
    if (name) {
        m_name = QString::fromUtf8(name);
    }

    const char *parent = virNodeDeviceGetParent(m_device);
    if (parent) {
        m_parent = QString::fromUtf8(parent);
    }

    qDebug() << "Created NodeDevice wrapper for" << m_name;
}

NodeDevice::~NodeDevice()
{
    if (m_device) {
        virNodeDeviceFree(m_device);
        m_device = nullptr;
    }
}

} // namespace QVirt
