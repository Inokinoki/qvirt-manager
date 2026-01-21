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

#include "StoragePool.h"
#include "Connection.h"
#include <QDebug>

namespace QVirt {

StoragePool::StoragePool(Connection *conn, virStoragePoolPtr pool)
    : BaseObject(conn)
    , m_connection(conn)
    , m_pool(pool)
    , m_active(false)
    , m_capacity(0)
    , m_allocation(0)
    , m_available(0)
{
    const char *name = virStoragePoolGetName(m_pool);
    if (name) {
        m_name = QString::fromUtf8(name);
    }

    char uuid[VIR_UUID_STRING_BUFLEN];
    if (virStoragePoolGetUUIDString(m_pool, uuid) == 0) {
        m_uuid = QString::fromUtf8(uuid);
    }

    // Check if pool is active and get info
    m_active = (virStoragePoolIsActive(m_pool) == 1);

    // Get capacity/allocation info
    virStoragePoolInfo poolInfo;
    if (virStoragePoolGetInfo(m_pool, &poolInfo) == 0) {
        m_capacity = poolInfo.capacity;
        m_allocation = poolInfo.allocation;
        m_available = poolInfo.available;
    }

    qDebug() << "Created StoragePool wrapper for" << m_name;
}

StoragePool::~StoragePool()
{
    if (m_pool) {
        virStoragePoolFree(m_pool);
        m_pool = nullptr;
    }
}

bool StoragePool::start()
{
    if (!m_pool) {
        return false;
    }

    if (virStoragePoolCreate(m_pool, 0) < 0) {
        return false;
    }

    m_active = true;
    emit stateChanged();
    return true;
}

bool StoragePool::stop()
{
    if (!m_pool) {
        return false;
    }

    if (virStoragePoolDestroy(m_pool) < 0) {
        return false;
    }

    m_active = false;
    emit stateChanged();
    return true;
}

bool StoragePool::refresh()
{
    if (!m_pool) {
        return false;
    }

    return (virStoragePoolRefresh(m_pool, 0) == 0);
}

bool StoragePool::undefine()
{
    if (!m_pool) {
        return false;
    }

    if (virStoragePoolUndefine(m_pool) < 0) {
        return false;
    }

    return true;
}

} // namespace QVirt
