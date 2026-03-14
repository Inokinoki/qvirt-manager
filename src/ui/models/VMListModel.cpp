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

#include "VMListModel.h"
#include "../../libvirt/EnumMapper.h"
#include <QDebug>

namespace QVirt {

VMListModel::VMListModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_showActive(true)
    , m_showInactive(true)
    , m_resetting(false)
{
}

int VMListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_domains.count();
}

QVariant VMListModel::data(const QModelIndex &index, int role) const
{
    // Prevent access during model reset
    if (m_resetting || !index.isValid() || index.row() >= m_domains.count()) {
        return QVariant();
    }

    Domain *domain = m_domains.at(index.row());
    if (!domain) {
        return QVariant();
    }

    // Safety check - verify domain has valid data
    QString domainName = domain->name();
    if (domainName.isEmpty()) {
        return QVariant();
    }

    // Handle table view display by column
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: // Name
            return domainName;
        case 1: // State
            return EnumMapper::domainStatusToString(domain->state());
        case 2: // Memory
            return data(index, MemoryFormattedRole).toString();
        default:
            return QVariant();
        }
    }

    switch (role) {
    case NameRole:
        return domainName;
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
    case DescriptionRole:
        return domain->description();
    case MemoryFormattedRole: {
        quint64 current = domain->currentMemory() / 1024; // MB
        quint64 max = domain->maxMemory() / 1024; // MB
        if (current == max) {
            return QString("%1 MB").arg(max);
        }
        return QString("%1 / %2 MB").arg(current).arg(max);
    }
    default:
        return QVariant();
    }
}

QVariant VMListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("State");
        case 2:
            return tr("Memory");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

void VMListModel::setConnection(Connection *conn)
{
    m_resetting = true;
    beginResetModel();

    // Disconnect from existing connections
    for (Connection *c : m_connections) {
        disconnect(c, nullptr, this, nullptr);
    }

    m_connections.clear();
    m_domains.clear();

    if (conn) {
        m_connections.append(conn);
        rebuildDomainListInternal();

        // Connect signals
        connect(conn, &Connection::domainAdded,
                this, &VMListModel::onDomainAdded);
        connect(conn, &Connection::domainRemoved,
                this, &VMListModel::onDomainRemoved);
    }

    endResetModel();
    m_resetting = false;
}

void VMListModel::setConnections(QList<Connection*> connections)
{
    m_resetting = true;
    beginResetModel();

    // Disconnect from existing connections
    for (Connection *c : m_connections) {
        disconnect(c, nullptr, this, nullptr);
    }

    m_connections = connections;
    m_domains.clear();

    rebuildDomainListInternal();

    // Connect signals
    for (Connection *conn : m_connections) {
        connect(conn, &Connection::domainAdded,
                this, &VMListModel::onDomainAdded);
        connect(conn, &Connection::domainRemoved,
                this, &VMListModel::onDomainRemoved);
    }

    endResetModel();
    m_resetting = false;
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
    rebuildDomainList();
}

void VMListModel::onDomainAdded(Domain *domain)
{
    if (!domain) {
        return;
    }

    // Check if domain is already in the list by UUID to avoid duplicates
    for (Domain *existing : m_domains) {
        if (existing && existing->uuid() == domain->uuid()) {
            qWarning() << "Duplicate domain detected:" << domain->name() << "UUID:" << domain->uuid();
            return;
        }
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
    rebuildDomainListInternal();
    endResetModel();
}

void VMListModel::rebuildDomainListInternal()
{
    m_domains.clear();

    for (Connection *conn : m_connections) {
        if (!conn) {
            continue;
        }

        for (Domain *domain : conn->domains()) {
            if (!domain) {
                continue;
            }

            // Check for duplicates by UUID
            bool isDuplicate = false;
            for (Domain *existing : m_domains) {
                if (existing && existing->uuid() == domain->uuid()) {
                    isDuplicate = true;
                    qWarning() << "Skipping duplicate domain in rebuild:" << domain->name();
                    break;
                }
            }
            if (isDuplicate) {
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
}

} // namespace QVirt
