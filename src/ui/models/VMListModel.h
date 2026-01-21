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

#ifndef QVIRT_UI_MODELS_VMLISTMODEL_H
#define QVIRT_UI_MODELS_VMLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QPixmap>

#include "../../libvirt/Domain.h"
#include "ConnectionListModel.h"

namespace QVirt {

/**
 * @brief Qt model for displaying list of VMs (domains)
 *
 * Provides a Qt Model/View interface for VMs with filtering support
 */
class VMListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        UUIDRole,
        IDRole,
        StateRole,
        StateStringRole,
        ConnectionURIRole,
        CPURole,
        MemoryRole,
        MaxMemoryRole,
        VCPUCountRole
    };

    explicit VMListModel(QObject *parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // VM management
    void setConnection(Connection *conn);
    void setConnections(QList<Connection*> connections);
    void addConnection(Connection *conn);
    void removeConnection(Connection *conn);

    Domain* domainAt(int index) const;
    QList<Domain*> domains() const { return m_domains; }

    // Filtering
    void setShowActiveVMs(bool show);
    void setShowInactiveVMs(bool show);
    void refresh();

public slots:
    void onDomainAdded(Domain *domain);
    void onDomainRemoved(Domain *domain);
    void onDomainStateChanged(Domain::State state);
    void onDomainStatsUpdated();

private:
    void rebuildDomainList();

    QList<Connection*> m_connections;
    QList<Domain*> m_domains;

    bool m_showActive;
    bool m_showInactive;
};

} // namespace QVirt

#endif // QVIRT_UI_MODELS_VMLISTMODEL_H
