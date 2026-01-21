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

#include "VMListModel.h"
#include "../../libvirt/EnumMapper.h"
#include <QDebug>

namespace QVirt {

VMListModel::VMListModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_showActive(true)
    , m_showInactive(true)
{
}

int VMListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_domains.count();
}

QVariant VMListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_domains.count()) {
        return QVariant();
    }

    Domain *domain = m_domains.at(index.row());
    if (!domain) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
        return QString("%1 (%2)").arg(domain->name(),
                                     EnumMapper::domainStatusToString(domain->state()));
    case NameRole:
        return domain->name();
    case UUIDRole:
        return domain->uuid();
    case IDRole:
        return domain->id();
    case StateRole:
        return static_cast<int>(domain->state());
    case StateStringRole:
        return EnumMapper::domainStatusToString(domain->state());
    case ConnectionURIRole:
        if (domain->connection()) {
            return domain->connection()->uri();
        }
        return QString();
    case CPURole:
        return domain->cpuUsage();
    case MemoryRole:
        return static_cast<quint64>(domain->currentMemory() / 1024); // KB to MB
    case MaxMemoryRole:
        return static_cast<quint64>(domain->maxMemory() / 1024); // KB to MB
    case VCPUCountRole:
        return domain->vcpuCount();
    default:
        return QVariant();
    }
}

QVariant VMListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("VM Name");
        case 1:
            return tr("State");
        case 2:
            return tr("CPU");
        case 3:
            return tr("Memory (MB)");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

void VMListModel::setConnection(Connection *conn)
{
    beginResetModel();

    // Disconnect from existing connections
    for (Connection *c : m_connections) {
        disconnect(c, nullptr, this, nullptr);
    }

    m_connections.clear();
    m_domains.clear();

    if (conn) {
        m_connections.append(conn);
        rebuildDomainList();

        // Connect signals
        connect(conn, &Connection::domainAdded,
                this, &VMListModel::onDomainAdded);
        connect(conn, &Connection::domainRemoved,
                this, &VMListModel::onDomainRemoved);
    }

    endResetModel();
}

void VMListModel::setConnections(QList<Connection*> connections)
{
    beginResetModel();

    // Disconnect from existing connections
    for (Connection *c : m_connections) {
        disconnect(c, nullptr, this, nullptr);
    }

    m_connections = connections;
    m_domains.clear();

    rebuildDomainList();

    // Connect signals
    for (Connection *conn : m_connections) {
        connect(conn, &Connection::domainAdded,
                this, &VMListModel::onDomainAdded);
        connect(conn, &Connection::domainRemoved,
                this, &VMListModel::onDomainRemoved);
    }

    endResetModel();
}

void VMListModel::addConnection(Connection *conn)
{
    if (!conn || m_connections.contains(conn)) {
        return;
    }

    m_connections.append(conn);

    connect(conn, &Connection::domainAdded,
            this, &VMListModel::onDomainAdded);
    connect(conn, &Connection::domainRemoved,
            this, &VMListModel::onDomainRemoved);

    rebuildDomainList();
}

void VMListModel::removeConnection(Connection *conn)
{
    if (!conn) {
        return;
    }

    disconnect(conn, nullptr, this, nullptr);
    m_connections.removeAll(conn);

    rebuildDomainList();
}

Domain* VMListModel::domainAt(int index) const
{
    if (index >= 0 && index < m_domains.count()) {
        return m_domains.at(index);
    }
    return nullptr;
}

void VMListModel::setShowActiveVMs(bool show)
{
    if (m_showActive != show) {
        m_showActive = show;
        rebuildDomainList();
    }
}

void VMListModel::setShowInactiveVMs(bool show)
{
    if (m_showInactive != show) {
        m_showInactive = show;
        rebuildDomainList();
    }
}

void VMListModel::refresh()
{
    beginResetModel();
    rebuildDomainList();
    endResetModel();
}

void VMListModel::onDomainAdded(Domain *domain)
{
    if (!domain) {
        return;
    }

    // Check if domain should be shown based on filter
    bool isActive = (domain->state() == Domain::StateRunning);
    if ((isActive && m_showActive) || (!isActive && m_showInactive)) {
        beginInsertRows(QModelIndex(), m_domains.count(), m_domains.count());
        m_domains.append(domain);

        // Connect to domain signals
        connect(domain, &Domain::stateChanged,
                this, [this](Domain::State) { onDomainStateChanged(Domain::StateRunning); });
        connect(domain, &Domain::statsUpdated,
                this, &VMListModel::onDomainStatsUpdated);

        endInsertRows();
    }
}

void VMListModel::onDomainRemoved(Domain *domain)
{
    if (!domain) {
        return;
    }

    int index = m_domains.indexOf(domain);
    if (index >= 0) {
        beginRemoveRows(QModelIndex(), index, index);
        m_domains.removeAt(index);
        disconnect(domain, nullptr, this, nullptr);
        endRemoveRows();
    }
}

void VMListModel::onDomainStateChanged(Domain::State state)
{
    Q_UNUSED(state);

    Domain *domain = qobject_cast<Domain*>(sender());
    if (!domain) {
        return;
    }

    // Check if domain should still be visible
    bool isActive = (domain->state() == Domain::StateRunning);
    bool shouldBeVisible = (isActive && m_showActive) || (!isActive && m_showInactive);
    int index = m_domains.indexOf(domain);

    if (index >= 0 && !shouldBeVisible) {
        // Remove from list
        beginRemoveRows(QModelIndex(), index, index);
        m_domains.removeAt(index);
        endRemoveRows();
    } else if (index < 0 && shouldBeVisible) {
        // Add to list
        onDomainAdded(domain);
    } else if (index >= 0) {
        // Just update
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

void VMListModel::onDomainStatsUpdated()
{
    Domain *domain = qobject_cast<Domain*>(sender());
    if (!domain) {
        return;
    }

    int index = m_domains.indexOf(domain);
    if (index >= 0) {
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

void VMListModel::rebuildDomainList()
{
    beginResetModel();
    m_domains.clear();

    for (Connection *conn : m_connections) {
        if (!conn) {
            continue;
        }

        for (Domain *domain : conn->domains()) {
            if (!domain) {
                continue;
            }

            bool isActive = (domain->state() == Domain::StateRunning);
            if ((isActive && m_showActive) || (!isActive && m_showInactive)) {
                m_domains.append(domain);

                // Connect to domain signals (disconnect first to avoid duplicates)
                disconnect(domain, &Domain::stateChanged, this, nullptr);
                disconnect(domain, &Domain::statsUpdated, this, nullptr);

                connect(domain, &Domain::stateChanged,
                        this, [this](Domain::State) { onDomainStateChanged(Domain::StateRunning); });
                connect(domain, &Domain::statsUpdated,
                        this, &VMListModel::onDomainStatsUpdated);
            }
        }
    }

    endResetModel();
}

} // namespace QVirt
