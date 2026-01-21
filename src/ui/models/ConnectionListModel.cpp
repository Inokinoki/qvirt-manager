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

#include "ConnectionListModel.h"
#include "../../libvirt/Domain.h"
#include <QDebug>

namespace QVirt {

ConnectionListModel::ConnectionListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ConnectionListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_connections.count();
}

QVariant ConnectionListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_connections.count()) {
        return QVariant();
    }

    Connection *conn = m_connections.at(index.row());
    if (!conn) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
        return conn->uri();
    case URIRole:
        return conn->uri();
    case StateRole:
        return static_cast<int>(conn->state());
    case HostnameRole:
        return conn->hostname();
    case DomainCountRole:
        return conn->domains().count();
    case NetworkCountRole:
        return conn->networks().count();
    case StoragePoolCountRole:
        return conn->storagePools().count();
    default:
        return QVariant();
    }
}

QVariant ConnectionListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return tr("Connection");
    }

    return QVariant();
}

void ConnectionListModel::addConnection(Connection *conn)
{
    if (!conn) {
        return;
    }

    beginInsertRows(QModelIndex(), m_connections.count(), m_connections.count());
    m_connections.append(conn);

    // Connect signals
    connect(conn, &Connection::stateChanged,
            this, &ConnectionListModel::onConnectionStateChanged);
    connect(conn, &Connection::domainAdded,
            this, &ConnectionListModel::onDomainAdded);
    connect(conn, &Connection::domainRemoved,
            this, &ConnectionListModel::onDomainRemoved);

    endInsertRows();
}

void ConnectionListModel::removeConnection(Connection *conn)
{
    if (!conn) {
        return;
    }

    int index = m_connections.indexOf(conn);
    if (index >= 0) {
        beginRemoveRows(QModelIndex(), index, index);
        m_connections.removeAt(index);
        disconnect(conn, nullptr, this, nullptr);
        endRemoveRows();
    }
}

Connection* ConnectionListModel::connectionAt(int index) const
{
    if (index >= 0 && index < m_connections.count()) {
        return m_connections.at(index);
    }
    return nullptr;
}

Connection* ConnectionListModel::connectionByURI(const QString &uri) const
{
    for (Connection *conn : m_connections) {
        if (conn && conn->uri() == uri) {
            return conn;
        }
    }
    return nullptr;
}

QList<Connection*> ConnectionListModel::connections() const
{
    return m_connections;
}

void ConnectionListModel::onConnectionStateChanged(Connection::State state)
{
    Q_UNUSED(state);

    Connection *conn = qobject_cast<Connection*>(sender());
    if (!conn) {
        return;
    }

    int index = m_connections.indexOf(conn);
    if (index >= 0) {
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

void ConnectionListModel::onDomainAdded(QVirt::Domain *domain)
{
    Q_UNUSED(domain);

    Connection *conn = qobject_cast<Connection*>(sender());
    if (!conn) {
        return;
    }

    int index = m_connections.indexOf(conn);
    if (index >= 0) {
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

void ConnectionListModel::onDomainRemoved(QVirt::Domain *domain)
{
    Q_UNUSED(domain);

    Connection *conn = qobject_cast<Connection*>(sender());
    if (!conn) {
        return;
    }

    int index = m_connections.indexOf(conn);
    if (index >= 0) {
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

} // namespace QVirt
