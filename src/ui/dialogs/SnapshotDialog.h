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

#ifndef QVIRT_UI_DIALOGS_SNAPSHOTDIALOG_H
#define QVIRT_UI_DIALOGS_SNAPSHOTDIALOG_H

#include <QDialog>
#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QMenu>
#include <QAction>

#include "../../libvirt/Domain.h"

namespace QVirt {

/**
 * @brief Snapshot Management Dialog
 *
 * Dialog for managing VM snapshots
 */
class SnapshotDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SnapshotDialog(Domain *domain, QWidget *parent = nullptr);
    ~SnapshotDialog() override = default;

private slots:
    void onSnapshotSelected();
    void onTakeSnapshot();
    void onRevertSnapshot();
    void onDeleteSnapshot();
    void onViewSnapshotXML();
    void onRefresh();
    void onContextMenuRequested(const QPoint &pos);

private:
    void setupUI();
    void updateSnapshotList();
    void updateSnapshotInfo();
    void takeSnapshot();
    void revertSnapshot();
    void deleteSnapshot();
    void viewSnapshotXML();

    Domain *m_domain;
    DomainSnapshot *m_currentSnapshot;

    // UI components
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
 * @brief Take Snapshot Dialog
 *
 * Dialog for creating a new VM snapshot
 */
class TakeSnapshotDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TakeSnapshotDialog(Domain *domain, QWidget *parent = nullptr);
    ~TakeSnapshotDialog() override = default;

    QString snapshotName() const { return m_nameEdit->text(); }
    QString snapshotDescription() const { return m_descriptionEdit->toPlainText(); }
    bool includeMemory() const { return m_memoryCheck->isChecked(); }

private slots:
    void validate();

private:
    void setupUI();

    Domain *m_domain;

    QLineEdit *m_nameEdit;
    QTextEdit *m_descriptionEdit;
    QCheckBox *m_memoryCheck;
    QCheckBox *m_disksCheck;
    QPushButton *m_btnOK;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_SNAPSHOTDIALOG_H
