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

#ifndef QVIRT_UI_MODELS_CONNECTIONTREEMODEL_H
#define QVIRT_UI_MODELS_CONNECTIONTREEMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QSet>
#include <QIcon>

#include "../../libvirt/Connection.h"
#include "../../core/Config.h"

namespace QVirt {

class Domain;

/**
 * @brief Tree item for connection tree model
 */
class TreeItem {
public:
    enum Type {
        ConnectionItem,
        VMItem
    };

    TreeItem(Type type, const QString &displayName, TreeItem *parent = nullptr);
    ~TreeItem();

    Type type() const { return m_type; }
    QString displayName() const { return m_displayName; }
    TreeItem *parent() const { return m_parent; }
    const QList<TreeItem*> &children() const { return m_children; }
    int row() const;

    Connection *connection() const { return m_connection; }
    Domain *domain() const { return m_domain; }
    ConnectionInfo *connectionInfo() const { return m_connectionInfo; }

    void setConnection(Connection *conn);
    void setDomain(Domain *domain);
    void setConnectionInfo(ConnectionInfo *info);
    void setDisplayName(const QString &name) { m_displayName = name; }

    void addChild(TreeItem *child);
    void removeChild(TreeItem *child);
    void clearChildren();

    // Helper methods
    bool isConnected() const;
    bool isDisconnected() const;
    QString uri() const;

private:
    Type m_type;
    QString m_displayName;
    TreeItem *m_parent;
    QList<TreeItem*> m_children;

    Connection *m_connection;
    Domain *m_domain;
    ConnectionInfo *m_connectionInfo;
};

/**
 * @brief Tree model for connections with expandable VM lists
 *
 * Provides a tree view where:
 * - Top level: Connections (connected or disconnected)
 * - Second level: VMs under each connection
 */
class ConnectionTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Roles {
        URIRole = Qt::UserRole + 1,
        StateRole,
        IsConnectedRole,
        IsVMRole,
        DomainRole,
        ConnectionRole,
        IconRole,
        IsCachedRole
    };

    explicit ConnectionTreeModel(QObject *parent = nullptr);
    ~ConnectionTreeModel() override;

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Connection management
    void addConnection(Connection *conn);
    void removeConnection(Connection *conn);
    void addDisconnectedConnection(const QString &uri, bool autoconnect = false);
    void removeDisconnectedConnection(const QString &uri);

    // Getters
    Connection* connectionAt(const QModelIndex &index) const;
    Connection* connectionAt(int row) const;
    Domain* domainAt(const QModelIndex &index) const;
    ConnectionInfo* connectionInfoAt(const QModelIndex &index) const;
    ConnectionInfo* connectionInfoAt(int row) const;
    Connection* connectionByURI(const QString &uri) const;
    TreeItem* itemAt(const QModelIndex &index) const;

    // Model refresh
    void refresh();

    // Get connection row index
    QModelIndex connectionIndex(Connection *conn) const;

signals:
    void connectionActivated(Connection *conn);
    void vmActivated(Domain *domain);

private slots:
    void onConnectionStateChanged(Connection::State state);
    void onDomainAdded(Domain *domain);
    void onDomainRemoved(Domain *domain);
    void onDomainStateChanged();

private:
    TreeItem *m_rootItem;
    QMap<QString, TreeItem*> m_connectionItems;
    QMap<Connection*, TreeItem*> m_connectionToItem;

    void setupConnectionItem(TreeItem *item, Connection *conn);
    void updateConnectionDisplay(TreeItem *item);
    TreeItem* findConnectionItem(const QString &uri) const;
    TreeItem* findConnectionItem(Connection *conn) const;
    TreeItem* findVMItem(TreeItem *connItem, Domain *domain) const;
};

} // namespace QVirt

#endif // QVIRT_UI_MODELS_CONNECTIONTREEMODEL_H
