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

#include "ConnectionTreeModel.h"
#include "../../libvirt/Domain.h"
#include "../../libvirt/EnumMapper.h"
#include <QDebug>
#include <QFont>
#include <QColor>
#include <QApplication>
#include <QStyle>

namespace QVirt {

// TreeItem implementation
TreeItem::TreeItem(Type type, const QString &displayName, TreeItem *parent)
    : m_type(type)
    , m_displayName(displayName)
    , m_parent(parent)
    , m_connection(nullptr)
    , m_domain(nullptr)
    , m_connectionInfo(nullptr)
{
}

TreeItem::~TreeItem()
{
    // Children are owned by the model, so we don't delete them here
    clearChildren();
}

int TreeItem::row() const
{
    if (!m_parent) {
        return 0;
    }
    return m_parent->m_children.indexOf(const_cast<TreeItem*>(this));
}

void TreeItem::setConnection(Connection *conn)
{
    m_connection = conn;
    if (conn) {
        m_displayName = conn->uri();
    }
}

void TreeItem::setDomain(Domain *domain)
{
    m_domain = domain;
    if (domain) {
        m_displayName = domain->name();
    }
}

void TreeItem::setConnectionInfo(ConnectionInfo *info)
{
    m_connectionInfo = info;
    if (info) {
        m_displayName = info->uri;
    }
}

void TreeItem::addChild(TreeItem *child)
{
    if (child && !m_children.contains(child)) {
        m_children.append(child);
        child->m_parent = this;
    }
}

void TreeItem::removeChild(TreeItem *child)
{
    if (child) {
        m_children.removeAll(child);
        child->m_parent = nullptr;
    }
}

void TreeItem::clearChildren()
{
    // Delete all children
    for (TreeItem *child : m_children) {
        delete child;
    }
    m_children.clear();
}

bool TreeItem::isConnected() const
{
    if (m_connection) {
        return m_connection->state() == Connection::Active;
    }
    return false;
}

bool TreeItem::isDisconnected() const
{
    return m_type == ConnectionItem && !isConnected();
}

QString TreeItem::uri() const
{
    if (m_connection) {
        return m_connection->uri();
    } else if (m_connectionInfo) {
        return m_connectionInfo->uri;
    }
    return QString();
}

// ConnectionTreeModel implementation
ConnectionTreeModel::ConnectionTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new TreeItem(TreeItem::ConnectionItem, "Root", nullptr);
}

ConnectionTreeModel::~ConnectionTreeModel()
{
    delete m_rootItem;
}

QModelIndex ConnectionTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TreeItem *parentItem = parent.isValid() ? static_cast<TreeItem*>(parent.internalPointer()) : m_rootItem;
    TreeItem *childItem = parentItem->children().value(row);

    if (childItem) {
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

QModelIndex ConnectionTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == m_rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int ConnectionTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) {
        return 0;
    }

    TreeItem *parentItem = parent.isValid() ? static_cast<TreeItem*>(parent.internalPointer()) : m_rootItem;
    return parentItem->children().count();
}

int ConnectionTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant ConnectionTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (!item) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: {
        QString name = item->displayName();
        // For running VMs, show [Running] prefix
        if (item->type() == TreeItem::VMItem && item->domain()) {
            if (item->domain()->state() == Domain::StateRunning) {
                return "[Running] " + name;
            }
        }
        return name;
    }

    case Qt::FontRole: {
        QFont font;
        if (item->isDisconnected()) {
            font.setItalic(true);
        }
        return font;
    }

    case Qt::ForegroundRole: {
        if (item->isDisconnected()) {
            return QVariant::fromValue(QColor(Qt::gray));
        }
        return QVariant();
    }

    case Qt::DecorationRole: {
        // Icons for items using theme icons with fallbacks
        if (item->type() == TreeItem::ConnectionItem) {
            // Connection icon - network/server icon
            return QIcon::fromTheme("network-server", QApplication::style()->standardIcon(QStyle::SP_DriveNetIcon));
        } else if (item->type() == TreeItem::VMItem && item->domain()) {
            Domain::State state = item->domain()->state();
            switch (state) {
            case Domain::StateRunning:
                return QIcon::fromTheme("media-playback-start", QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
            case Domain::StatePaused:
                return QIcon::fromTheme("media-playback-pause", QApplication::style()->standardIcon(QStyle::SP_MediaPause));
            case Domain::StateShutOff:
                return QIcon::fromTheme("computer", QApplication::style()->standardIcon(QStyle::SP_DesktopIcon));
            default:
                return QIcon::fromTheme("computer", QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));
            }
        }
        return QVariant();
    }

    case URIRole:
        return item->uri();

    case StateRole: {
        if (item->domain()) {
            return static_cast<int>(item->domain()->state());
        } else if (item->connection()) {
            return static_cast<int>(item->connection()->state());
        }
        return static_cast<int>(Connection::Disconnected);
    }

    case IsConnectedRole:
        return item->isConnected();

    case IsVMRole:
        return item->type() == TreeItem::VMItem;

    case DomainRole:
        return QVariant::fromValue(static_cast<void*>(item->domain()));

    case ConnectionRole:
        return QVariant::fromValue(static_cast<void*>(item->connection()));

    default:
        return QVariant();
    }
}

QVariant ConnectionTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return tr("Connections");
    }
    return QVariant();
}

Qt::ItemFlags ConnectionTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (item && item->type() == TreeItem::ConnectionItem && item->isDisconnected()) {
        // Disconnected connections are still selectable
        flags |= Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }

    return flags;
}

void ConnectionTreeModel::addConnection(Connection *conn)
{
    if (!conn) {
        return;
    }

    QString uri = conn->uri();

    // Check if we already have this connection
    TreeItem *existingItem = findConnectionItem(uri);
    if (existingItem) {
        // Update existing item
        existingItem->setConnection(conn);
        m_connectionToItem[conn] = existingItem;
        updateConnectionDisplay(existingItem);

        QModelIndex idx = index(existingItem->row(), 0, QModelIndex());
        emit dataChanged(idx, idx);

        // Reconnect signals
        connect(conn, &Connection::stateChanged,
                this, &ConnectionTreeModel::onConnectionStateChanged);
        connect(conn, &Connection::domainAdded,
                this, &ConnectionTreeModel::onDomainAdded);
        connect(conn, &Connection::domainRemoved,
                this, &ConnectionTreeModel::onDomainRemoved);

        // Add existing domains
        QList<Domain *> existingDomains = conn->domains();
        if (!existingDomains.isEmpty()) {
            int childRow = existingItem->children().count();
            beginInsertRows(idx, childRow, childRow + existingDomains.count() - 1);
            
            for (Domain *domain : existingDomains) {
                TreeItem *vmItem = new TreeItem(TreeItem::VMItem, domain->name(), existingItem);
                vmItem->setDomain(domain);
                existingItem->addChild(vmItem);
                
                // Connect to domain signals
                connect(domain, &Domain::stateChanged,
                        this, &ConnectionTreeModel::onDomainStateChanged);
            }
            
            endInsertRows();
        }

        return;
    }

    // Create new connection item
    int row = m_rootItem->children().count();
    beginInsertRows(QModelIndex(), row, row);

    TreeItem *connItem = new TreeItem(TreeItem::ConnectionItem, uri, m_rootItem);
    connItem->setConnection(conn);
    m_rootItem->addChild(connItem);
    m_connectionItems[uri] = connItem;
    m_connectionToItem[conn] = connItem;

    // Connect signals BEFORE adding domains
    connect(conn, &Connection::stateChanged,
            this, &ConnectionTreeModel::onConnectionStateChanged);
    connect(conn, &Connection::domainAdded,
            this, &ConnectionTreeModel::onDomainAdded);
    connect(conn, &Connection::domainRemoved,
            this, &ConnectionTreeModel::onDomainRemoved);

    endInsertRows();

    // Add existing domains AFTER the connection item is in the model
    // This ensures the parent index is valid for child items
    QList<Domain *> existingDomains = conn->domains();
    if (!existingDomains.isEmpty()) {
        QModelIndex connIndex = index(row, 0, QModelIndex());
        beginInsertRows(connIndex, 0, existingDomains.count() - 1);
        
        for (Domain *domain : existingDomains) {
            TreeItem *vmItem = new TreeItem(TreeItem::VMItem, domain->name(), connItem);
            vmItem->setDomain(domain);
            connItem->addChild(vmItem);
            
            // Connect to domain signals
            connect(domain, &Domain::stateChanged,
                    this, &ConnectionTreeModel::onDomainStateChanged);
        }
        
        endInsertRows();
    }
}

void ConnectionTreeModel::removeConnection(Connection *conn)
{
    if (!conn) {
        return;
    }

    QString uri = conn->uri();
    TreeItem *connItem = findConnectionItem(conn);
    if (!connItem) {
        return;
    }

    int row = connItem->row();

    // Disconnect signals
    disconnect(conn, nullptr, this, nullptr);

    // Convert to disconnected state instead of removing
    connItem->setConnection(nullptr);
    ConnectionInfo *info = new ConnectionInfo(uri);
    connItem->setConnectionInfo(info);

    // Clear all VM children
    QModelIndex connIndex = index(row, 0, QModelIndex());
    int childCount = connItem->children().count();
    if (childCount > 0) {
        beginRemoveRows(connIndex, 0, childCount - 1);
        connItem->clearChildren();
        endRemoveRows();
    }

    updateConnectionDisplay(connItem);
    emit dataChanged(connIndex, connIndex);

    m_connectionToItem.remove(conn);
}

void ConnectionTreeModel::addDisconnectedConnection(const QString &uri, bool autoconnect)
{
    if (uri.isEmpty() || m_connectionItems.contains(uri)) {
        return;
    }

    int row = m_rootItem->children().count();
    beginInsertRows(QModelIndex(), row, row);

    TreeItem *connItem = new TreeItem(TreeItem::ConnectionItem, uri, m_rootItem);
    ConnectionInfo *info = new ConnectionInfo(uri);
    info->autoconnect = autoconnect;
    connItem->setConnectionInfo(info);
    m_rootItem->addChild(connItem);
    m_connectionItems[uri] = connItem;

    endInsertRows();
}

void ConnectionTreeModel::removeDisconnectedConnection(const QString &uri)
{
    TreeItem *connItem = findConnectionItem(uri);
    if (!connItem) {
        return;
    }

    int row = connItem->row();
    beginRemoveRows(QModelIndex(), row, row);

    m_connectionItems.remove(uri);
    m_rootItem->removeChild(connItem);
    delete connItem;

    endRemoveRows();
}

Connection* ConnectionTreeModel::connectionAt(const QModelIndex &index) const
{
    TreeItem *item = itemAt(index);
    return item ? item->connection() : nullptr;
}

Connection* ConnectionTreeModel::connectionAt(int row) const
{
    TreeItem *item = m_rootItem->children().value(row);
    return item ? item->connection() : nullptr;
}

Domain* ConnectionTreeModel::domainAt(const QModelIndex &index) const
{
    TreeItem *item = itemAt(index);
    return item ? item->domain() : nullptr;
}

ConnectionInfo* ConnectionTreeModel::connectionInfoAt(const QModelIndex &index) const
{
    TreeItem *item = itemAt(index);
    return item ? item->connectionInfo() : nullptr;
}

ConnectionInfo* ConnectionTreeModel::connectionInfoAt(int row) const
{
    TreeItem *item = m_rootItem->children().value(row);
    return item ? item->connectionInfo() : nullptr;
}

Connection* ConnectionTreeModel::connectionByURI(const QString &uri) const
{
    TreeItem *item = findConnectionItem(uri);
    return item ? item->connection() : nullptr;
}

TreeItem* ConnectionTreeModel::itemAt(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return nullptr;
    }
    return static_cast<TreeItem*>(index.internalPointer());
}

void ConnectionTreeModel::refresh()
{
    emit layoutChanged();
}

QModelIndex ConnectionTreeModel::connectionIndex(Connection *conn) const
{
    if (!conn) {
        return QModelIndex();
    }

    TreeItem *item = findConnectionItem(conn);
    if (item) {
        return createIndex(item->row(), 0, item);
    }
    return QModelIndex();
}

void ConnectionTreeModel::setupConnectionItem(TreeItem *item, Connection *conn)
{
    Q_UNUSED(item);
    Q_UNUSED(conn);
    // Signals are connected in addConnection
}

void ConnectionTreeModel::updateConnectionDisplay(TreeItem *item)
{
    if (!item) {
        return;
    }

    QString displayName;
    if (item->connection()) {
        displayName = item->connection()->uri();
    } else if (item->connectionInfo()) {
        displayName = item->connectionInfo()->uri;
    }

    item->setDisplayName(displayName);
}

TreeItem* ConnectionTreeModel::findConnectionItem(const QString &uri) const
{
    return m_connectionItems.value(uri);
}

TreeItem* ConnectionTreeModel::findConnectionItem(Connection *conn) const
{
    return m_connectionToItem.value(conn);
}

TreeItem* ConnectionTreeModel::findVMItem(TreeItem *connItem, Domain *domain) const
{
    if (!connItem || !domain) {
        return nullptr;
    }

    for (TreeItem *child : connItem->children()) {
        if (child->domain() && child->domain()->uuid() == domain->uuid()) {
            return child;
        }
    }
    return nullptr;
}

void ConnectionTreeModel::onConnectionStateChanged(Connection::State state)
{
    Connection *conn = qobject_cast<Connection*>(sender());
    if (!conn) {
        return;
    }

    TreeItem *connItem = findConnectionItem(conn);
    if (!connItem) {
        return;
    }

    QModelIndex idx = index(connItem->row(), 0, QModelIndex());
    emit dataChanged(idx, idx);
}

void ConnectionTreeModel::onDomainAdded(Domain *domain)
{
    qDebug() << "ConnectionTreeModel::onDomainAdded called for domain:" << (domain ? domain->name() : "null");

    if (!domain || !domain->connection()) {
        qWarning() << "onDomainAdded: domain or connection is null, ignoring";
        return;
    }

    Connection *conn = domain->connection();
    TreeItem *connItem = findConnectionItem(conn);
    if (!connItem) {
        qWarning() << "onDomainAdded: connection item not found for connection:" << conn->uri();
        return;
    }

    // Check for duplicates
    if (findVMItem(connItem, domain)) {
        qDebug() << "onDomainAdded: domain already exists in tree, ignoring:" << domain->name();
        return;
    }

    QModelIndex connIndex = index(connItem->row(), 0, QModelIndex());
    int row = connItem->children().count();

    qDebug() << "onDomainAdded: Adding domain" << domain->name() << "to tree at row" << row;
    beginInsertRows(connIndex, row, row);

    TreeItem *vmItem = new TreeItem(TreeItem::VMItem, domain->name(), connItem);
    vmItem->setDomain(domain);
    connItem->addChild(vmItem);

    // Connect to domain signals
    connect(domain, &Domain::stateChanged,
            this, &ConnectionTreeModel::onDomainStateChanged);

    endInsertRows();
    qDebug() << "onDomainAdded: Domain" << domain->name() << "added to tree successfully. Children count:" << connItem->children().count();
}

void ConnectionTreeModel::onDomainRemoved(Domain *domain)
{
    if (!domain || !domain->connection()) {
        return;
    }

    Connection *conn = domain->connection();
    TreeItem *connItem = findConnectionItem(conn);
    if (!connItem) {
        return;
    }

    TreeItem *vmItem = findVMItem(connItem, domain);
    if (!vmItem) {
        return;
    }

    QModelIndex connIndex = index(connItem->row(), 0, QModelIndex());
    int row = vmItem->row();

    beginRemoveRows(connIndex, row, row);

    connItem->removeChild(vmItem);
    delete vmItem;

    endRemoveRows();
}

void ConnectionTreeModel::onDomainStateChanged()
{
    Domain *domain = qobject_cast<Domain*>(sender());
    if (!domain || !domain->connection()) {
        return;
    }

    Connection *conn = domain->connection();
    TreeItem *connItem = findConnectionItem(conn);
    if (!connItem) {
        return;
    }

    TreeItem *vmItem = findVMItem(connItem, domain);
    if (!vmItem) {
        return;
    }

    QModelIndex idx = index(vmItem->row(), 0, index(connItem->row(), 0, QModelIndex()));
    emit dataChanged(idx, idx);
}

} // namespace QVirt
