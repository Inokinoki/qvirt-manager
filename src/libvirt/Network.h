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

#ifndef QVIRT_LIBVIRT_NETWORK_H
#define QVIRT_LIBVIRT_NETWORK_H

#include "../core/BaseObject.h"
#include <QString>

#include <libvirt/libvirt.h>

namespace QVirt {

class Connection;

/**
 * @brief libvirt network wrapper
 */
class Network : public BaseObject
{
    Q_OBJECT

public:
    ~Network() override;

    QString name() const { return m_name; }
    QString uuid() const { return m_uuid; }
    bool isActive() const { return m_active; }

    bool start();
    bool stop();
    bool undefine();

signals:
    void stateChanged();

private:
    Network(Connection *conn, virNetworkPtr network);

    Connection *m_connection;
    virNetworkPtr m_network;

    QString m_name;
    QString m_uuid;
    bool m_active;

    friend class Connection;
};

} // namespace QVirt

#endif // QVIRT_LIBVIRT_NETWORK_H
