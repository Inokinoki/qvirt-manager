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

#ifndef QVIRT_LIBVIRT_STORAGEPOOL_H
#define QVIRT_LIBVIRT_STORAGEPOOL_H

#include "../core/BaseObject.h"
#include <QString>
#include <QList>

#ifdef LIBVIRT_FOUND
#include <libvirt/libvirt.h>

// Windows.h defines 'state' as a macro which breaks our code
#ifdef _WIN32
#undef state
#endif
#else
// Forward declarations for when libvirt is not available
typedef void *virStoragePoolPtr;
#endif // LIBVIRT_FOUND

namespace QVirt {

class Connection;
class StorageVolume;

/**
 * @brief libvirt storage pool wrapper
 */
class StoragePool : public BaseObject
{
    Q_OBJECT

public:
    enum PoolState {
        StateInactive = 0,
        StateRunning = 1,
        StateDegraded = 2
    };

    enum PoolType {
        TypeDir = 0,
        TypeFS = 1,
        TypeNetFS = 2,
        TypeLogical = 3,
        TypeDisk = 4,
        TypeISCSI = 5,
        TypeSCSI = 6,
        TypeMPATH = 7,
        TypeRBD = 8,
        TypeSheepdog = 9,
        TypeGlusterFS = 10,
        TypeZFS = 11,
        TypeVStorage = 12
    };

    ~StoragePool() override;

    QString name() const { return m_name; }
    QString uuid() const { return m_uuid; }
    bool isActive() const { return m_active; }
    PoolState state() const { return m_state; }
    PoolType type() const { return m_type; }

    quint64 capacity() const { return m_capacity; }
    quint64 allocation() const { return m_allocation; }
    quint64 available() const { return m_available; }

    // Access to virStoragePoolPtr for volume operations
    virStoragePoolPtr virPool() const { return m_pool; }

    // Volume operations
    QList<StorageVolume*> volumes();

    bool start();
    bool stop();
    bool refresh();
    bool undefine();

signals:
    void stateChanged();

private:
    StoragePool(Connection *conn, virStoragePoolPtr pool);

    Connection *m_connection;
    virStoragePoolPtr m_pool;

    QString m_name;
    QString m_uuid;
    bool m_active;
    PoolState m_state;
    PoolType m_type;

    quint64 m_capacity;
    quint64 m_allocation;
    quint64 m_available;

    void parseXML(const QString &xml);

    friend class Connection;
};

} // namespace QVirt

#endif // QVIRT_LIBVIRT_STORAGEPOOL_H
