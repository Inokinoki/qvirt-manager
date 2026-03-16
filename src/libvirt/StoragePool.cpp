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

#include "StoragePool.h"
#include "StorageVolume.h"
#include "Connection.h"
#include <QDebug>
#include <QDomDocument>

namespace QVirt {

StoragePool::StoragePool(Connection *conn, virStoragePoolPtr pool)
    : BaseObject(conn)
    , m_connection(conn)
    , m_pool(pool)
    , m_active(false)
    , m_state(StateInactive)
    , m_type(TypeDir)
    , m_capacity(0)
    , m_allocation(0)
    , m_available(0)
{
    if (!m_pool) {
        qWarning() << "StoragePool created with null virStoragePoolPtr";
        return;
    }

    const char *name = virStoragePoolGetName(m_pool);
    if (!name || QString::fromUtf8(name).isEmpty()) {
        qWarning() << "StoragePool has no name, using placeholder";
        m_name = "unknown-pool";
    } else {
        m_name = QString::fromUtf8(name);
    }

    char uuid[VIR_UUID_STRING_BUFLEN];
    int uuidRet = virStoragePoolGetUUIDString(m_pool, uuid);
    if (uuidRet == 0) {
        m_uuid = QString::fromUtf8(uuid);
    } else {
        m_uuid = QString();
    }

    // Check if pool is active - handle potential remote access errors
    int isActive = virStoragePoolIsActive(m_pool);
    if (isActive < 0) {
        qWarning() << "Failed to check if storage pool" << m_name << "is active, assuming inactive";
        m_active = false;
    } else {
        m_active = (isActive == 1);
    }

    if (m_active) {
        m_state = StateRunning;
    }

    // Get capacity/allocation info - handle potential remote access errors
    virStoragePoolInfo poolInfo;
    int infoRet = virStoragePoolGetInfo(m_pool, &poolInfo);
    if (infoRet == 0) {
        m_capacity = poolInfo.capacity;
        m_allocation = poolInfo.allocation;
        m_available = poolInfo.available;
        m_state = static_cast<PoolState>(poolInfo.state);
    } else {
        qWarning() << "Failed to get storage pool info for" << m_name << ", using defaults";
    }

    // Get and parse XML to determine pool type
    char *xml = virStoragePoolGetXMLDesc(m_pool, 0);
    if (xml) {
        parseXML(QString::fromUtf8(xml));
        free(xml);
    } else {
        qWarning() << "Failed to get XML for storage pool" << m_name;
    }

    qDebug() << "Created StoragePool wrapper for" << m_name << "(type:" << m_type << ", active:" << m_active << ")";
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

QList<StorageVolume*> StoragePool::volumes()
{
    QList<StorageVolume*> volumeList;

    if (!m_pool || !m_active) {
        return volumeList;
    }

    // List all volumes in the pool
    int numVolumes = virStoragePoolNumOfVolumes(m_pool);
    if (numVolumes <= 0) {
        return volumeList;
    }

    // Get volume names
    char **names = new char*[numVolumes];
    int actualVolumes = virStoragePoolListVolumes(m_pool, names, numVolumes);
    if (actualVolumes < 0) {
        delete[] names;
        return volumeList;
    }

    for (int i = 0; i < actualVolumes; ++i) {
        virStorageVolPtr vol = virStorageVolLookupByName(m_pool, names[i]);
        if (vol) {
            auto *volume = new StorageVolume(vol, this, this);
            volumeList.append(volume);
        }
        free(names[i]);
    }

    delete[] names;

    return volumeList;
}

void StoragePool::parseXML(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) {
        qWarning() << "Failed to parse storage pool XML";
        return;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "pool") {
        qWarning() << "Invalid storage pool XML";
        return;
    }

    // Parse pool type
    QString typeStr = root.attribute("type");
    if (typeStr == "dir") {
        m_type = TypeDir;
    } else if (typeStr == "fs") {
        m_type = TypeFS;
    } else if (typeStr == "netfs") {
        m_type = TypeNetFS;
    } else if (typeStr == "logical") {
        m_type = TypeLogical;
    } else if (typeStr == "disk") {
        m_type = TypeDisk;
    } else if (typeStr == "iscsi") {
        m_type = TypeISCSI;
    } else if (typeStr == "scsi") {
        m_type = TypeSCSI;
    } else if (typeStr == "mpath") {
        m_type = TypeMPATH;
    } else if (typeStr == "rbd") {
        m_type = TypeRBD;
    } else if (typeStr == "sheepdog") {
        m_type = TypeSheepdog;
    } else if (typeStr == "glusterfs") {
        m_type = TypeGlusterFS;
    } else if (typeStr == "zfs") {
        m_type = TypeZFS;
    } else if (typeStr == "vstorage") {
        m_type = TypeVStorage;
    }
}

} // namespace QVirt
