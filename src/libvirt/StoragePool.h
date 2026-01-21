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

#ifndef QVIRT_LIBVIRT_STORAGEPOOL_H
#define QVIRT_LIBVIRT_STORAGEPOOL_H

#include "../core/BaseObject.h"
#include <QString>

#include <libvirt/libvirt.h>

namespace QVirt {

class Connection;

/**
 * @brief libvirt storage pool wrapper
 */
class StoragePool : public BaseObject
{
    Q_OBJECT

public:
    ~StoragePool() override;

    QString name() const { return m_name; }
    QString uuid() const { return m_uuid; }
    bool isActive() const { return m_active; }

    quint64 capacity() const { return m_capacity; }
    quint64 allocation() const { return m_allocation; }
    quint64 available() const { return m_available; }

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

    quint64 m_capacity;
    quint64 m_allocation;
    quint64 m_available;

    friend class Connection;
};

} // namespace QVirt

#endif // QVIRT_LIBVIRT_STORAGEPOOL_H
