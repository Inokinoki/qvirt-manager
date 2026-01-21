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

#ifndef QVIRT_UI_MODELS_CONNECTIONLISTMODEL_H
#define QVIRT_UI_MODELS_CONNECTIONLISTMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "../../libvirt/Connection.h"

namespace QVirt {

/**
 * @brief Qt model for displaying list of libvirt connections
 *
 * Provides a Qt Model/View interface for connections
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
        StoragePoolCountRole
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
    Connection* connectionAt(int index) const;
    Connection* connectionByURI(const QString &uri) const;
    QList<Connection*> connections() const;

    // Update signals from connections
    void onConnectionStateChanged(Connection::State state);
    void onDomainAdded(QVirt::Domain *domain);
    void onDomainRemoved(QVirt::Domain *domain);

private:
    QList<Connection*> m_connections;
};

} // namespace QVirt

#endif // QVIRT_UI_MODELS_CONNECTIONLISTMODEL_H
