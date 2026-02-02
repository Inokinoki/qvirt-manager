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
#include <QFont>
#include <QColor>

namespace QVirt {

ConnectionListModel::ConnectionListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ConnectionListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_connections.count() + m_disconnectedConnections.count();
}

QVariant ConnectionListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount()) {
        return QVariant();
    }

    bool isConnected = index.row() < m_connections.count();
    Connection *conn = nullptr;
    ConnectionInfo *info = nullptr;

    if (isConnected) {
        conn = m_connections.at(index.row());
        if (!conn) {
            return QVariant();
        }
    } else {
        info = m_disconnectedConnections.at(index.row() - m_connections.count());
        if (!info) {
            return QVariant();
        }
    }

    switch (role) {
    case Qt::DisplayRole:
        if (conn) {
            QString display = conn->uri();
            if (conn->state() != Connection::Active) {
                display += tr(" (Disconnected)");
            }
            return display;
        } else {
            return info->uri + tr(" (Disconnected)");
        }
    case Qt::FontRole:
        // Show disconnected connections in italic
        if (!isConnected || (conn && conn->state() != Connection::Active)) {
            QFont font;
            font.setItalic(true);
            return font;
        }
        break;
    case Qt::ForegroundRole:
        // Gray out disconnected connections
        if (!isConnected || (conn && conn->state() != Connection::Active)) {
            return QVariant::fromValue(QColor(Qt::gray));
        }
        break;
    case URIRole:
        return conn ? conn->uri() : info->uri;
    case StateRole:
        return conn ? static_cast<int>(conn->state()) : static_cast<int>(Connection::Disconnected);
    case HostnameRole:
        return conn ? conn->hostname() : QString();
    case DomainCountRole:
        return conn ? conn->domains().count() : 0;
    case NetworkCountRole:
        return conn ? conn->networks().count() : 0;
    case StoragePoolCountRole:
        return conn ? conn->storagePools().count() : 0;
    case IsConnectedRole:
        return isConnected && conn && conn->state() == Connection::Active;
    case AutoconnectRole:
        if (conn) {
            // For active connections, we need to check config
            return false; // Would need Config access here
        } else {
            return info->autoconnect;
        }
    default:
        break;
    }
    return QVariant();
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

    QString uri = conn->uri();

    // Remove from disconnected list if it exists there
    if (m_allURIs.contains(uri)) {
        // Remove from disconnected list
        for (int i = 0; i < m_disconnectedConnections.count(); ++i) {
            if (m_disconnectedConnections[i]->uri == uri) {
                beginRemoveRows(QModelIndex(), m_connections.count() + i, m_connections.count() + i);
                delete m_disconnectedConnections.takeAt(i);
                endRemoveRows();
                break;
            }
        }
    }

    beginInsertRows(QModelIndex(), m_connections.count(), m_connections.count());
    m_connections.append(conn);
    m_allURIs.insert(uri);

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

    QString uri = conn->uri();

    int index = m_connections.indexOf(conn);
    if (index >= 0) {
        beginRemoveRows(QModelIndex(), index, index);
        m_connections.removeAt(index);
        m_allURIs.remove(uri);
        disconnect(conn, nullptr, this, nullptr);
        endRemoveRows();
    }
}

void ConnectionListModel::addDisconnectedConnection(const QString &uri, bool autoconnect)
{
    if (uri.isEmpty() || m_allURIs.contains(uri)) {
        return;
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    auto *info = new ConnectionInfo(uri);
    info->autoconnect = autoconnect;
    m_disconnectedConnections.append(info);
    m_allURIs.insert(uri);
    endInsertRows();
}

void ConnectionListModel::removeDisconnectedConnection(const QString &uri)
{
    for (int i = 0; i < m_disconnectedConnections.count(); ++i) {
        if (m_disconnectedConnections[i]->uri == uri) {
            int row = m_connections.count() + i;
            beginRemoveRows(QModelIndex(), row, row);
            delete m_disconnectedConnections.takeAt(i);
            m_allURIs.remove(uri);
            endRemoveRows();
            return;
        }
    }
}

Connection* ConnectionListModel::connectionAt(int index) const
{
    if (index >= 0 && index < m_connections.count()) {
        return m_connections.at(index);
    }
    return nullptr;
}

ConnectionInfo* ConnectionListModel::connectionInfoAt(int index) const
{
    if (index >= m_connections.count() && index < rowCount()) {
        return m_disconnectedConnections.at(index - m_connections.count());
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

bool ConnectionListModel::hasConnection(const QString &uri) const
{
    return m_allURIs.contains(uri);
}

void ConnectionListModel::refresh()
{
    // Emit layoutChanged to notify the view to update
    emit layoutChanged();
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
