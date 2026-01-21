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

#include "SnapshotsPage.h"
#include "../../libvirt/EnumMapper.h"
#include "../dialogs/SnapshotDialog.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QDateTime>
#include <QFontDatabase>

namespace QVirt {

//=============================================================================
// SnapshotListModel
//=============================================================================

SnapshotListModel::SnapshotListModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void SnapshotListModel::setSnapshots(const QList<DomainSnapshot*> &snapshots)
{
    beginResetModel();
    m_snapshots = snapshots;
    endResetModel();
}

DomainSnapshot* SnapshotListModel::snapshotAt(int row) const
{
    if (row >= 0 && row < m_snapshots.size()) {
        return m_snapshots.at(row);
    }
    return nullptr;
}

int SnapshotListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_snapshots.size();
}

int SnapshotListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3; // Name, Time, State
}

QVariant SnapshotListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_snapshots.size()) {
        return QVariant();
    }

    DomainSnapshot *snapshot = m_snapshots.at(index.row());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return snapshot->name();
        case 1:
            return snapshot->creationTime().toString("yyyy-MM-dd hh:mm:ss");
        case 2:
            return EnumMapper::stateToString(snapshot->state());
        default:
            break;
        }
    }

    return QVariant();
}

QVariant SnapshotListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return "Name";
        case 1:
            return "Created";
        case 2:
            return "State";
        }
    }

    return QVariant();
}

//=============================================================================
// SnapshotsPage
//=============================================================================

SnapshotsPage::SnapshotsPage(Domain *domain, QWidget *parent)
    : QWidget(parent)
    , m_domain(domain)
    , m_currentSnapshot(nullptr)
{
    setupUI();
    setupSnapshotList();
    setupSnapshotInfo();
    updateSnapshotList();
}

void SnapshotsPage::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // Create splitter for resizable panes
    m_splitter = new QSplitter(Qt::Vertical, this);

    // Top section: Snapshot list
    auto *topWidget = new QWidget(m_splitter);
    auto *topLayout = new QVBoxLayout(topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);

    auto *listGroup = new QGroupBox("Snapshots", topWidget);
    auto *listLayout = new QVBoxLayout(listGroup);

    m_snapshotList = new QTableView(listGroup);
    m_snapshotList->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_snapshotList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_snapshotList->setAlternatingRowColors(true);
    m_snapshotList->setContextMenuPolicy(Qt::CustomContextMenu);
    m_snapshotList->horizontalHeader()->setStretchLastSection(true);
    m_snapshotList->verticalHeader()->setVisible(false);

    connect(m_snapshotList, &QTableView::clicked,
            this, &SnapshotsPage::onSnapshotSelected);
    connect(m_snapshotList, &QTableView::customContextMenuRequested,
            this, &SnapshotsPage::onContextMenuRequested);

    listLayout->addWidget(m_snapshotList);

    // Snapshot control buttons
    auto *buttonLayout = new QHBoxLayout();

    m_btnTakeSnapshot = new QPushButton("Take Snapshot", listGroup);
    m_btnRevertSnapshot = new QPushButton("Revert", listGroup);
    m_btnDeleteSnapshot = new QPushButton("Delete", listGroup);
    m_btnViewXML = new QPushButton("View XML", listGroup);
    m_btnRefresh = new QPushButton("Refresh", listGroup);

    connect(m_btnTakeSnapshot, &QPushButton::clicked, this, &SnapshotsPage::onTakeSnapshot);
    connect(m_btnRevertSnapshot, &QPushButton::clicked, this, &SnapshotsPage::onRevertSnapshot);
    connect(m_btnDeleteSnapshot, &QPushButton::clicked, this, &SnapshotsPage::onDeleteSnapshot);
    connect(m_btnViewXML, &QPushButton::clicked, this, &SnapshotsPage::onViewSnapshotXML);
    connect(m_btnRefresh, &QPushButton::clicked, this, &SnapshotsPage::onRefresh);

    buttonLayout->addWidget(m_btnTakeSnapshot);
    buttonLayout->addWidget(m_btnRevertSnapshot);
    buttonLayout->addWidget(m_btnDeleteSnapshot);
    buttonLayout->addWidget(m_btnViewXML);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_btnRefresh);

    listLayout->addLayout(buttonLayout);

    topLayout->addWidget(listGroup);
    m_splitter->addWidget(topWidget);

    // Bottom section: Snapshot info
    auto *bottomWidget = new QWidget(m_splitter);
    auto *bottomLayout = new QVBoxLayout(bottomWidget);
    bottomLayout->setContentsMargins(0, 0, 0, 0);

    auto *infoGroup = new QGroupBox("Snapshot Details", bottomWidget);
    auto *infoLayout = new QVBoxLayout(infoGroup);

    m_snapshotInfoLabel = new QLabel("Select a snapshot to view details", infoGroup);
    m_snapshotInfoLabel->setWordWrap(true);
    m_snapshotInfoLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    infoLayout->addWidget(m_snapshotInfoLabel);
    bottomLayout->addWidget(infoGroup);

    m_splitter->addWidget(bottomWidget);
    m_splitter->setStretchFactor(0, 2);
    m_splitter->setStretchFactor(1, 1);

    mainLayout->addWidget(m_splitter);

    // Setup context menu
    m_contextMenu = new QMenu(this);

    m_actionRevert = m_contextMenu->addAction("Revert to Snapshot");
    connect(m_actionRevert, &QAction::triggered, this, &SnapshotsPage::onRevertSnapshot);

    m_actionDelete = m_contextMenu->addAction("Delete Snapshot");
    connect(m_actionDelete, &QAction::triggered, this, &SnapshotsPage::onDeleteSnapshot);

    m_contextMenu->addSeparator();

    m_actionViewXML = m_contextMenu->addAction("View XML");
    connect(m_actionViewXML, &QAction::triggered, this, &SnapshotsPage::onViewSnapshotXML);
}

void SnapshotsPage::setupSnapshotList()
{
    auto *model = new SnapshotListModel(m_snapshotList);
    m_snapshotList->setModel(model);

    // Adjust column widths
    m_snapshotList->setColumnWidth(0, 200);
    m_snapshotList->setColumnWidth(1, 180);
    m_snapshotList->setColumnWidth(2, 100);

    // Update button states
    updateButtonStates();
}

void SnapshotsPage::setupSnapshotInfo()
{
    m_snapshotInfoLabel->setText(
        "<p>Select a snapshot from the list above to view its details.</p>"
        "<p><b>Snapshot Operations:</b></p>"
        "<ul>"
        "<li><b>Take Snapshot:</b> Create a new snapshot of the current VM state</li>"
        "<li><b>Revert:</b> Restore VM to the state saved in a snapshot</li>"
        "<li><b>Delete:</b> Remove a snapshot (this cannot be undone)</li>"
        "<li><b>View XML:</b> Show the libvirt XML configuration</li>"
        "</ul>"
    );
}

void SnapshotsPage::updateSnapshotList()
{
    // Clear existing snapshots
    qDeleteAll(m_snapshots);
    m_snapshots.clear();
    m_currentSnapshot = nullptr;

    // Fetch snapshots from libvirt
    int numSnapshots = 0;
    virDomainSnapshotPtr *snapshots = nullptr;

    if (m_domain->rawDomain()) {
        numSnapshots = virDomainListAllSnapshots(m_domain->rawDomain(), &snapshots, 0);
    }

    if (numSnapshots >= 0) {
        for (int i = 0; i < numSnapshots; ++i) {
            auto *snapshot = new DomainSnapshot(snapshots[i], m_domain, this);
            m_snapshots.append(snapshot);
        }

        if (snapshots) {
            free(snapshots);
        }
    }

    // Update model
    auto *model = static_cast<SnapshotListModel*>(m_snapshotList->model());
    model->setSnapshots(m_snapshots);

    // Update button states
    updateButtonStates();
}

void SnapshotsPage::updateSnapshotInfo()
{
    if (!m_currentSnapshot) {
        m_snapshotInfoLabel->setText("Select a snapshot to view details");
        return;
    }

    QString info = QString(
        "<h3>%1</h3>"
        "<table>"
        "<tr><td><b>Description:</b></td><td>%2</td></tr>"
        "<tr><td><b>Created:</b></td><td>%3</td></tr>"
        "<tr><td><b>State:</b></td><td>%4</td></tr>"
        "<tr><td><b>Is Current:</b></td><td>%5</td></tr>"
        "</table>"
    ).arg(
        m_currentSnapshot->name(),
        m_currentSnapshot->description().isEmpty() ? "N/A" : m_currentSnapshot->description(),
        m_currentSnapshot->creationTime().toString("yyyy-MM-dd hh:mm:ss"),
        EnumMapper::stateToString(m_currentSnapshot->state()),
        m_currentSnapshot->isCurrent() ? "Yes" : "No"
    );

    m_snapshotInfoLabel->setText(info);
}

void SnapshotsPage::updateButtonStates()
{
    bool hasSnapshot = m_currentSnapshot != nullptr;
    bool vmRunning = m_domain->state() == Domain::StateRunning;

    m_btnRevertSnapshot->setEnabled(hasSnapshot);
    m_btnDeleteSnapshot->setEnabled(hasSnapshot);
    m_btnViewXML->setEnabled(hasSnapshot);
    m_btnTakeSnapshot->setEnabled(vmRunning || m_domain->state() == Domain::StatePaused);
}

void SnapshotsPage::refresh()
{
    updateSnapshotList();
}

void SnapshotsPage::onSnapshotSelected(const QModelIndex &index)
{
    auto *model = static_cast<SnapshotListModel*>(m_snapshotList->model());
    m_currentSnapshot = model->snapshotAt(index.row());
    updateSnapshotInfo();
    updateButtonStates();
}

void SnapshotsPage::onTakeSnapshot()
{
    // Simple dialog for taking a snapshot
    QDialog dialog(this);
    dialog.setWindowTitle("Take Snapshot");

    auto *layout = new QFormLayout(&dialog);

    auto *nameEdit = new QLineEdit(&dialog);
    nameEdit->setPlaceholderText("Snapshot name");
    nameEdit->setText(QString("snap-%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss")));

    auto *descEdit = new QTextEdit(&dialog);
    descEdit->setPlaceholderText("Snapshot description (optional)");
    descEdit->setMaximumHeight(100);

    auto *memoryCheck = new QCheckBox("Include VM memory state", &dialog);
    memoryCheck->setToolTip("Save the running VM's memory to the snapshot");

    layout->addRow("Name:", nameEdit);
    layout->addRow("Description:", descEdit);
    layout->addRow("", memoryCheck);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString name = nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Invalid Name", "Please enter a snapshot name");
        return;
    }

    // Create snapshot XML
    QString xml = QString(
        "<domainsnapshot>"
        "<name>%1</name>"
        "<description>%2</description>"
        "%3"
        "</domainsnapshot>"
    ).arg(
        name,
        descEdit->toPlainText().toHtmlEscaped(),
        memoryCheck->isChecked() ? "<memory snapshot='internal'/>" : ""
    );

    // Take snapshot via libvirt
    if (m_domain->rawDomain()) {
        virDomainSnapshotPtr snap = virDomainSnapshotCreateXML(
            m_domain->rawDomain(),
            xml.toUtf8().constData(),
            0
        );

        if (snap) {
            virDomainSnapshotFree(snap);
            QMessageBox::information(this, "Success",
                QString("Snapshot '%1' created successfully").arg(name));
            updateSnapshotList();
        } else {
            QMessageBox::critical(this, "Error",
                QString("Failed to create snapshot"));
        }
    }
}

void SnapshotsPage::onRevertSnapshot()
{
    if (!m_currentSnapshot) {
        return;
    }

    int result = QMessageBox::question(
        this,
        "Revert to Snapshot",
        QString("Are you sure you want to revert to snapshot '%1'?\n\n"
                "This will discard the current VM state and cannot be undone.")
            .arg(m_currentSnapshot->name()),
        QMessageBox::Yes | QMessageBox::No
    );

    if (result != QMessageBox::Yes) {
        return;
    }

    if (m_currentSnapshot->revert(0)) {
        QMessageBox::information(this, "Success",
            QString("Reverted to snapshot '%1'").arg(m_currentSnapshot->name()));
        updateSnapshotList();
    } else {
        QMessageBox::critical(this, "Error",
            QString("Failed to revert to snapshot"));
    }
}

void SnapshotsPage::onDeleteSnapshot()
{
    if (!m_currentSnapshot) {
        return;
    }

    int result = QMessageBox::question(
        this,
        "Delete Snapshot",
        QString("Are you sure you want to delete snapshot '%1'?\n\n"
                "This cannot be undone.")
            .arg(m_currentSnapshot->name()),
        QMessageBox::Yes | QMessageBox::No
    );

    if (result != QMessageBox::Yes) {
        return;
    }

    if (m_currentSnapshot->delete_(0)) {
        QMessageBox::information(this, "Success",
            QString("Snapshot '%1' deleted").arg(m_currentSnapshot->name()));
        m_currentSnapshot = nullptr;
        updateSnapshotList();
    } else {
        QMessageBox::critical(this, "Error",
            QString("Failed to delete snapshot"));
    }
}

void SnapshotsPage::onViewSnapshotXML()
{
    if (!m_currentSnapshot) {
        return;
    }

    QString xml = m_currentSnapshot->getXMLDesc(0);

    QDialog dialog(this);
    dialog.setWindowTitle("Snapshot XML - " + m_currentSnapshot->name());
    dialog.resize(600, 400);

    auto *layout = new QVBoxLayout(&dialog);

    auto *textEdit = new QTextEdit(&dialog);
    textEdit->setPlainText(xml);
    textEdit->setReadOnly(true);
    textEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    layout->addWidget(textEdit);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);

    dialog.exec();
}

void SnapshotsPage::onRefresh()
{
    updateSnapshotList();
}

void SnapshotsPage::onContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = m_snapshotList->indexAt(pos);
    if (!index.isValid()) {
        return;
    }

    onSnapshotSelected(index);
    m_contextMenu->popup(m_snapshotList->viewport()->mapToGlobal(pos));
}

} // namespace QVirt
