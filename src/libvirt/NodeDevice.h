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

#ifndef QVIRT_LIBVIRT_NODEDEVICE_H
#define QVIRT_LIBVIRT_NODEDEVICE_H

#include "../core/BaseObject.h"
#include <QString>

#include <libvirt/libvirt.h>

namespace QVirt {

class Connection;

/**
 * @brief libvirt node device wrapper
 */
class NodeDevice : public BaseObject
{
    Q_OBJECT

public:
    ~NodeDevice() override;

    QString name() const { return m_name; }
    QString parent() const { return m_parent; }

signals:
    void stateChanged();

private:
    NodeDevice(Connection *conn, virNodeDevicePtr device);

    Connection *m_connection;
    virNodeDevicePtr m_device;

    QString m_name;
    QString m_parent;

    friend class Connection;
};

} // namespace QVirt

#endif // QVIRT_LIBVIRT_NODEDEVICE_H
