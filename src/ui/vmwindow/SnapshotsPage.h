/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 Inoki <veyx.shaw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version).
 */

#ifndef QVIRT_UI_VMWINDOW_SNAPSHOTSPAGE_H
#define QVIRT_UI_VMWINDOW_SNAPSHOTSPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QMenu>
#include <QHeaderView>
#include <QSplitter>

#include "../../libvirt/Domain.h"
#include "../../libvirt/DomainSnapshot.h"

namespace QVirt {

/**
 * @brief Snapshots Page for VMWindow
 *
 * Provides snapshot management functionality including:
 * - Listing all snapshots
 * - Taking new snapshots
 * - Reverting to snapshots
 * - Deleting snapshots
 * - Viewing snapshot details
 */
class SnapshotsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SnapshotsPage(Domain *domain, QWidget *parent = nullptr);
    ~SnapshotsPage() override = default;

    void refresh();
    void updateSnapshotList();

private slots:
    void onSnapshotSelected(const QModelIndex &index);
    void onTakeSnapshot();
    void onRevertSnapshot();
    void onDeleteSnapshot();
    void onViewSnapshotXML();
    void onRefresh();
    void onContextMenuRequested(const QPoint &pos);

private:
    void setupUI();
    void setupSnapshotList();
    void setupSnapshotInfo();
    void updateSnapshotInfo();
    void updateButtonStates();
    void takeSnapshot();
    void revertSnapshot();
    void deleteSnapshot();
    void viewSnapshotXML();

    Domain *m_domain;
    DomainSnapshot *m_currentSnapshot;
    QList<DomainSnapshot*> m_snapshots;

    // UI components
    QSplitter *m_splitter;
    QTableView *m_snapshotList;
    QLabel *m_snapshotInfoLabel;
    QPushButton *m_btnTakeSnapshot;
    QPushButton *m_btnRevertSnapshot;
    QPushButton *m_btnDeleteSnapshot;
    QPushButton *m_btnViewXML;
    QPushButton *m_btnRefresh;

    // Context menu
    QMenu *m_contextMenu;
    QAction *m_actionRevert;
    QAction *m_actionDelete;
    QAction *m_actionViewXML;
};

/**
 * @brief Simple model for snapshot list display
 */
class SnapshotListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SnapshotListModel(QObject *parent = nullptr);

    void setSnapshots(const QList<DomainSnapshot*> &snapshots);
    DomainSnapshot* snapshotAt(int row) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    QList<DomainSnapshot*> m_snapshots;
};

} // namespace QVirt

#endif // QVIRT_UI_VMWINDOW_SNAPSHOTSPAGE_H
