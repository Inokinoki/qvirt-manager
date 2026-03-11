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

#ifndef QVIRT_UI_MODELS_CONNECTIONLISTMODEL_H
#define QVIRT_UI_MODELS_CONNECTIONLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QSet>

#include "../../libvirt/Connection.h"
#include "../../core/Config.h"

namespace QVirt {

/**
 * @brief Qt model for displaying list of libvirt connections
 *
 * Provides a Qt Model/View interface for connections
 * Supports both active Connection objects and disconnected ConnectionInfo
 */
class ConnectionListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        URIRole = Qt::UserRole + 1,
        StateRole,
        HostnameRole,
        DomainCountRole,
        NetworkCountRole,
        StoragePoolCountRole,
        IsConnectedRole,
        AutoconnectRole
    };

    explicit ConnectionListModel(QObject *parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Connection management
    void addConnection(Connection *conn);
    void removeConnection(Connection *conn);
    void addDisconnectedConnection(const QString &uri, bool autoconnect = false);
    void removeDisconnectedConnection(const QString &uri);

    // Get connection at index (may return nullptr for disconnected)
    Connection* connectionAt(int index) const;
    Connection* connectionByURI(const QString &uri) const;
    ConnectionInfo* connectionInfoAt(int index) const;
    QList<Connection*> connections() const;

    // Check if connection exists (connected or disconnected)
    bool hasConnection(const QString &uri) const;

    // Refresh the model (emit layoutChanged signal)
    void refresh();

    // Update signals from connections
    void onConnectionStateChanged(Connection::State state);
    void onDomainAdded(QVirt::Domain *domain);
    void onDomainRemoved(QVirt::Domain *domain);

private:
    QList<Connection*> m_connections;
    QList<ConnectionInfo*> m_disconnectedConnections; // For disconnected connections
    QSet<QString> m_allURIs; // Track all URIs to prevent duplicates
};

} // namespace QVirt

#endif // QVIRT_UI_MODELS_CONNECTIONLISTMODEL_H
