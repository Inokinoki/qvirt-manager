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

#include "SnapshotDialog.h"
#include "../../core/Error.h"
#include "../../libvirt/EnumMapper.h"
#include "../../libvirt/DomainSnapshot.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QFormLayout>

namespace QVirt {

SnapshotDialog::SnapshotDialog(Domain *domain, QWidget *parent)
    : QDialog(parent)
    , m_domain(domain)
    , m_currentSnapshot(nullptr)
{
    setWindowTitle("VM Snapshots");
    resize(800, 600);

    setupUI();
    updateSnapshotList();
}

void SnapshotDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Snapshot list
    auto *listGroup = new QGroupBox("Snapshots", this);
    auto *listLayout = new QVBoxLayout(listGroup);

    m_snapshotList = new QTableView(listGroup);
    m_snapshotList->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_snapshotList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_snapshotList->setAlternatingRowColors(true);
    m_snapshotList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_snapshotList, &QTableView::customContextMenuRequested,
            this, &SnapshotDialog::onContextMenuRequested);
    connect(m_snapshotList, &QTableView::clicked, this, &SnapshotDialog::onSnapshotSelected);

    listLayout->addWidget(m_snapshotList);

    // Snapshot info
    m_snapshotInfoLabel = new QLabel("Select a snapshot to view details", listGroup);
    m_snapshotInfoLabel->setWordWrap(true);
    listLayout->addWidget(m_snapshotInfoLabel);

    // Snapshot control buttons
    auto *buttonLayout = new QHBoxLayout();

    m_btnTakeSnapshot = new QPushButton("Take Snapshot", listGroup);
    m_btnRevertSnapshot = new QPushButton("Revert", listGroup);
    m_btnDeleteSnapshot = new QPushButton("Delete", listGroup);
    m_btnViewXML = new QPushButton("View XML", listGroup);
    m_btnRefresh = new QPushButton("Refresh", listGroup);

    connect(m_btnTakeSnapshot, &QPushButton::clicked, this, &SnapshotDialog::onTakeSnapshot);
    connect(m_btnRevertSnapshot, &QPushButton::clicked, this, &SnapshotDialog::onRevertSnapshot);
    connect(m_btnDeleteSnapshot, &QPushButton::clicked, this, &SnapshotDialog::onDeleteSnapshot);
    connect(m_btnViewXML, &QPushButton::clicked, this, &SnapshotDialog::onViewSnapshotXML);
    connect(m_btnRefresh, &QPushButton::clicked, this, &SnapshotDialog::onRefresh);

    buttonLayout->addWidget(m_btnTakeSnapshot);
    buttonLayout->addWidget(m_btnRevertSnapshot);
    buttonLayout->addWidget(m_btnDeleteSnapshot);
    buttonLayout->addWidget(m_btnViewXML);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_btnRefresh);

    listLayout->addLayout(buttonLayout);

    mainLayout->addWidget(listGroup);

    // Close button
    auto *closeLayout = new QHBoxLayout();
    closeLayout->addStretch();

    auto *btnClose = new QPushButton("Close", this);
    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);
    closeLayout->addWidget(btnClose);

    mainLayout->addLayout(closeLayout);

    // Setup context menu
    m_contextMenu = new QMenu(this);

    m_actionRevert = m_contextMenu->addAction("Revert to Snapshot");
    connect(m_actionRevert, &QAction::triggered, this, &SnapshotDialog::onRevertSnapshot);

    m_actionDelete = m_contextMenu->addAction("Delete Snapshot");
    connect(m_actionDelete, &QAction::triggered, this, &SnapshotDialog::onDeleteSnapshot);

    m_actionViewXML = m_contextMenu->addAction("View XML");
    connect(m_actionViewXML, &QAction::triggered, this, &SnapshotDialog::onViewSnapshotXML);

    // Initial state
    m_btnRevertSnapshot->setEnabled(false);
    m_btnDeleteSnapshot->setEnabled(false);
    m_btnViewXML->setEnabled(false);
}

void SnapshotDialog::updateSnapshotList()
{
    if (!m_domain) {
        return;
    }

    // Get list of snapshots from domain
    QList<DomainSnapshot*> snapshots = m_domain->snapshots();

    // Update snapshot info with count
    m_snapshotInfoLabel->setText(QString("Found %1 snapshot(s)").arg(snapshots.size()));
}

void SnapshotDialog::updateSnapshotInfo()
{
    if (!m_currentSnapshot) {
        m_snapshotInfoLabel->setText("Select a snapshot to view details");
        return;
    }

    QString info = QString("<b>Name:</b> %1<br>").arg(m_currentSnapshot->name());
    info += QString("<b>Description:</b> %1<br>").arg(m_currentSnapshot->description());
    info += QString("<b>State:</b> %1<br>").arg(EnumMapper::stateToString(m_currentSnapshot->state()));
    info += QString("<b>Creation Time:</b> %1").arg(m_currentSnapshot->creationTime().toString());

    m_snapshotInfoLabel->setText(info);

    // Update button states
    m_btnRevertSnapshot->setEnabled(true);
    m_btnDeleteSnapshot->setEnabled(true);
    m_btnViewXML->setEnabled(true);
}

void SnapshotDialog::onSnapshotSelected()
{
    // Get selected snapshot from table
    // Current implementation: use first snapshot from domain
    // Future enhancement: implement proper table selection model
    QList<DomainSnapshot*> snapshots = m_domain->snapshots();
    if (!snapshots.isEmpty()) {
        m_currentSnapshot = snapshots.first();
        updateSnapshotInfo();
    }
}

void SnapshotDialog::onTakeSnapshot()
{
    TakeSnapshotDialog dialog(m_domain, this);
    if (dialog.exec() == QDialog::Accepted) {
        takeSnapshot();
    }
}

void SnapshotDialog::takeSnapshot()
{
    QMessageBox::information(this, "Take Snapshot",
        "Snapshot creation will be implemented with DomainSnapshot class.\n\n"
        "This requires:\n"
        "1. Snapshot XML generation\n"
        "2. virDomainSnapshotCreateXML() call\n"
        "3. Refresh snapshot list");

    updateSnapshotList();
}

void SnapshotDialog::onRevertSnapshot()
{
    if (!m_currentSnapshot) {
        return;
    }

    auto reply = QMessageBox::question(this, "Revert to Snapshot",
        QString("Are you sure you want to revert to snapshot '%1'?\n\n"
                "The VM will be reverted to the state it was in when this snapshot was taken.").arg(m_currentSnapshot->name()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        revertSnapshot();
    }
}

void SnapshotDialog::revertSnapshot()
{
    if (!m_currentSnapshot) {
        return;
    }

    // Confirm revert operation
    auto reply = QMessageBox::question(this, "Revert Snapshot",
        QString("Are you sure you want to revert to snapshot '%1'?\n\n"
                "The VM will be restored to the state it was in when this snapshot was taken.").arg(m_currentSnapshot->name()),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    // Revert to snapshot
    if (m_currentSnapshot->revert(0)) {
        QMessageBox::information(this, "Success",
            QString("Successfully reverted to snapshot '%1'.").arg(m_currentSnapshot->name()));
    } else {
        QMessageBox::critical(this, "Error",
            QString("Failed to revert to snapshot '%1'.").arg(m_currentSnapshot->name()));
    }

    updateSnapshotList();
}

void SnapshotDialog::onDeleteSnapshot()
{
    if (!m_currentSnapshot) {
        return;
    }

    auto reply = QMessageBox::question(this, "Delete Snapshot",
        QString("Are you sure you want to delete snapshot '%1'?").arg(m_currentSnapshot->name()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        deleteSnapshot();
    }
}

void SnapshotDialog::deleteSnapshot()
{
    if (!m_currentSnapshot) {
        return;
    }

    // Delete snapshot (metadata and children)
    if (m_currentSnapshot->delete_(0)) {
        QMessageBox::information(this, "Success",
            QString("Successfully deleted snapshot '%1'.").arg(m_currentSnapshot->name()));
        m_currentSnapshot = nullptr;
    } else {
        QMessageBox::critical(this, "Error",
            QString("Failed to delete snapshot '%1'.").arg(m_currentSnapshot->name()));
    }

    updateSnapshotList();
}

void SnapshotDialog::onViewSnapshotXML()
{
    if (!m_currentSnapshot) {
        return;
    }

    viewSnapshotXML();
}

void SnapshotDialog::viewSnapshotXML()
{
    if (!m_currentSnapshot) {
        return;
    }

    QString xml = m_currentSnapshot->getXMLDesc();

    // Show XML in a message box (simplified - should use a proper dialog)
    QMessageBox::information(this, "Snapshot XML",
        "Snapshot XML viewer will be implemented.\n\n"
        "This requires:\n"
        "1. Get XML from snapshot\n"
        "2. Display in syntax-highlighted text view\n"
        "3. Allow copying to clipboard");
}

void SnapshotDialog::onRefresh()
{
    updateSnapshotList();
    updateSnapshotInfo();
}

void SnapshotDialog::onContextMenuRequested(const QPoint &pos)
{
    if (m_snapshotList->indexAt(pos).isValid()) {
        m_contextMenu->exec(m_snapshotList->viewport()->mapToGlobal(pos));
    }
}

// ============================================================================
// TakeSnapshotDialog
// ============================================================================

TakeSnapshotDialog::TakeSnapshotDialog(Domain *domain, QWidget *parent)
    : QDialog(parent)
    , m_domain(domain)
{
    setWindowTitle("Take VM Snapshot");
    setMinimumSize(500, 300);

    setupUI();
    validate();
}

void TakeSnapshotDialog::setupUI()
{
    auto *layout = new QVBoxLayout(this);

    // Form layout for inputs
    auto *formLayout = new QFormLayout();

    // Snapshot name
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("Enter snapshot name");
    connect(m_nameEdit, &QLineEdit::textChanged, this, &TakeSnapshotDialog::validate);
    formLayout->addRow("Name:", m_nameEdit);

    // Snapshot description
    m_descriptionEdit = new QTextEdit(this);
    m_descriptionEdit->setPlaceholderText("Enter snapshot description (optional)");
    m_descriptionEdit->setMaximumHeight(100);
    formLayout->addRow("Description:", m_descriptionEdit);

    layout->addLayout(formLayout);

    // Snapshot options
    auto *optionsGroup = new QGroupBox("Snapshot Options", this);
    auto *optionsLayout = new QVBoxLayout(optionsGroup);

    m_memoryCheck = new QCheckBox("Include VM memory state", optionsGroup);
    m_memoryCheck->setToolTip("Save the current memory state. This requires more disk space.");
    m_memoryCheck->setChecked(false);
    optionsLayout->addWidget(m_memoryCheck);

    m_disksCheck = new QCheckBox("Snapshot all disks", optionsGroup);
    m_disksCheck->setToolTip("Create snapshots of all disk devices.");
    m_disksCheck->setChecked(true);
    m_disksCheck->setEnabled(false); // Always true for now
    optionsLayout->addWidget(m_disksCheck);

    layout->addWidget(optionsGroup);

    layout->addStretch();

    // Information about memory state
    auto *infoLabel = new QLabel(
        "<b>Note:</b> Including memory state allows you to resume exactly "
        "where you left off, but requires significantly more disk space.",
        this);
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    auto *btnCancel = new QPushButton("Cancel", this);
    m_btnOK = new QPushButton("Take Snapshot", this);

    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_btnOK, &QPushButton::clicked, this, &QDialog::accept);

    buttonLayout->addWidget(btnCancel);
    buttonLayout->addWidget(m_btnOK);

    layout->addLayout(buttonLayout);
}

void TakeSnapshotDialog::validate()
{
    bool valid = true;

    // Check name
    if (m_nameEdit->text().trimmed().isEmpty()) {
        valid = false;
    }

    m_btnOK->setEnabled(valid);
}

} // namespace QVirt
