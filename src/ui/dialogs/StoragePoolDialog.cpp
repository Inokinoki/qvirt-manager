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

#include "StoragePoolDialog.h"
#include "../../core/Error.h"
#include "../../libvirt/EnumMapper.h"
#include "../wizards/CreateVolumeWizard.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QFileInfo>

namespace QVirt {

StoragePoolDialog::StoragePoolDialog(Connection *conn, QWidget *parent)
    : QDialog(parent)
    , m_connection(conn)
    , m_currentPool(nullptr)
    , m_currentVolume(nullptr)
{
    setWindowTitle("Storage Pools");
    resize(900, 600);

    setupUI();
    updatePoolList();
}

void StoragePoolDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Create tab widget
    m_tabWidget = new QTabWidget(this);

    setupPoolsTab();
    setupVolumesTab();

    mainLayout->addWidget(m_tabWidget);

    // Close button
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    auto *btnClose = new QPushButton("Close", this);
    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(btnClose);

    mainLayout->addLayout(buttonLayout);

    // Setup context menu
    m_contextMenu = new QMenu(this);

    m_actionStart = m_contextMenu->addAction("Start");
    connect(m_actionStart, &QAction::triggered, this, &StoragePoolDialog::onPoolStart);

    m_actionStop = m_contextMenu->addAction("Stop");
    connect(m_actionStop, &QAction::triggered, this, &StoragePoolDialog::onPoolStop);

    m_actionDelete = m_contextMenu->addAction("Delete");
    connect(m_actionDelete, &QAction::triggered, this, &StoragePoolDialog::onPoolDelete);

    m_actionRefresh = m_contextMenu->addAction("Refresh");
    connect(m_actionRefresh, &QAction::triggered, this, &StoragePoolDialog::onPoolRefresh);
}

void StoragePoolDialog::setupPoolsTab()
{
    auto *poolsTab = new QWidget(m_tabWidget);
    auto *poolsLayout = new QVBoxLayout(poolsTab);

    // Pool list
    auto *poolGroup = new QGroupBox("Storage Pools", poolsTab);
    auto *poolLayout = new QVBoxLayout(poolGroup);

    m_poolList = new QTableView(poolGroup);
    m_poolList->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_poolList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_poolList->setAlternatingRowColors(true);
    m_poolList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_poolList, &QTableView::customContextMenuRequested,
            this, &StoragePoolDialog::onContextMenuRequested);
    connect(m_poolList, &QTableView::clicked, this, &StoragePoolDialog::onPoolSelected);

    poolLayout->addWidget(m_poolList);

    // Pool info
    m_poolInfoLabel = new QLabel("Select a pool to view details", poolGroup);
    m_poolInfoLabel->setWordWrap(true);
    poolLayout->addWidget(m_poolInfoLabel);

    // Pool control buttons
    auto *buttonLayout = new QHBoxLayout();

    m_btnPoolStart = new QPushButton("Start", poolGroup);
    m_btnPoolStop = new QPushButton("Stop", poolGroup);
    m_btnPoolRefresh = new QPushButton("Refresh", poolGroup);
    m_btnPoolDelete = new QPushButton("Delete", poolGroup);
    m_btnRefreshPools = new QPushButton("Refresh List", poolGroup);

    connect(m_btnPoolStart, &QPushButton::clicked, this, &StoragePoolDialog::onPoolStart);
    connect(m_btnPoolStop, &QPushButton::clicked, this, &StoragePoolDialog::onPoolStop);
    connect(m_btnPoolRefresh, &QPushButton::clicked, this, &StoragePoolDialog::onPoolRefresh);
    connect(m_btnPoolDelete, &QPushButton::clicked, this, &StoragePoolDialog::onPoolDelete);
    connect(m_btnRefreshPools, &QPushButton::clicked, this, &StoragePoolDialog::onRefresh);

    buttonLayout->addWidget(m_btnPoolStart);
    buttonLayout->addWidget(m_btnPoolStop);
    buttonLayout->addWidget(m_btnPoolRefresh);
    buttonLayout->addWidget(m_btnPoolDelete);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_btnRefreshPools);

    poolLayout->addLayout(buttonLayout);

    poolsLayout->addWidget(poolGroup);

    m_tabWidget->addTab(poolsTab, "Pools");

    // Initial state
    m_btnPoolStart->setEnabled(false);
    m_btnPoolStop->setEnabled(false);
    m_btnPoolRefresh->setEnabled(false);
    m_btnPoolDelete->setEnabled(false);
}

void StoragePoolDialog::setupVolumesTab()
{
    auto *volumesTab = new QWidget(m_tabWidget);
    auto *volumesLayout = new QVBoxLayout(volumesTab);

    // Volume list
    auto *volumeGroup = new QGroupBox("Storage Volumes", volumesTab);
    auto *volumeLayout = new QVBoxLayout(volumeGroup);

    m_volumeList = new QTableView(volumeGroup);
    m_volumeList->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_volumeList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_volumeList->setAlternatingRowColors(true);
    connect(m_volumeList, &QTableView::clicked, this, &StoragePoolDialog::onVolumeSelected);

    volumeLayout->addWidget(m_volumeList);

    // Volume info
    m_volumeInfoLabel = new QLabel("Select a pool and volume to view details", volumeGroup);
    m_volumeInfoLabel->setWordWrap(true);
    volumeLayout->addWidget(m_volumeInfoLabel);

    // Volume control buttons
    auto *buttonLayout = new QHBoxLayout();

    m_btnVolumeCreate = new QPushButton("Create Volume", volumeGroup);
    m_btnVolumeDelete = new QPushButton("Delete", volumeGroup);
    m_btnVolumeDownload = new QPushButton("Download", volumeGroup);
    m_btnVolumeUpload = new QPushButton("Upload", volumeGroup);
    m_btnVolumeClone = new QPushButton("Clone", volumeGroup);
    m_btnVolumeWipe = new QPushButton("Wipe", volumeGroup);
    m_btnRefreshVolumes = new QPushButton("Refresh", volumeGroup);

    connect(m_btnVolumeCreate, &QPushButton::clicked, this, &StoragePoolDialog::onVolumeCreate);
    connect(m_btnVolumeDelete, &QPushButton::clicked, this, &StoragePoolDialog::onVolumeDelete);
    connect(m_btnVolumeDownload, &QPushButton::clicked, this, &StoragePoolDialog::onVolumeDownload);
    connect(m_btnVolumeUpload, &QPushButton::clicked, this, &StoragePoolDialog::onVolumeUpload);
    connect(m_btnVolumeClone, &QPushButton::clicked, this, &StoragePoolDialog::onVolumeClone);
    connect(m_btnVolumeWipe, &QPushButton::clicked, this, &StoragePoolDialog::onVolumeWipe);
    connect(m_btnRefreshVolumes, &QPushButton::clicked, this, &StoragePoolDialog::onRefresh);

    buttonLayout->addWidget(m_btnVolumeCreate);
    buttonLayout->addWidget(m_btnVolumeDelete);
    buttonLayout->addWidget(m_btnVolumeDownload);
    buttonLayout->addWidget(m_btnVolumeUpload);
    buttonLayout->addWidget(m_btnVolumeClone);
    buttonLayout->addWidget(m_btnVolumeWipe);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_btnRefreshVolumes);

    volumeLayout->addLayout(buttonLayout);

    volumesLayout->addWidget(volumeGroup);

    m_tabWidget->addTab(volumesTab, "Volumes");

    // Initial state
    m_btnVolumeCreate->setEnabled(false);
    m_btnVolumeDelete->setEnabled(false);
    m_btnVolumeDownload->setEnabled(false);
    m_btnVolumeUpload->setEnabled(false);
    m_btnVolumeClone->setEnabled(false);
    m_btnVolumeWipe->setEnabled(false);
}

void StoragePoolDialog::updatePoolList()
{
    if (!m_connection) {
        return;
    }

    // Get list of storage pools from connection
    QList<StoragePool*> pools = m_connection->storagePools();

    // Update pool info with count
    m_poolInfoLabel->setText(QString("Found %1 storage pool(s)").arg(pools.size()));
}

void StoragePoolDialog::updateVolumeList()
{
    if (!m_currentPool) {
        return;
    }

    // Get list of volumes from pool
    QList<StorageVolume*> volumes = m_currentPool->volumes();

    // Update volume info with count
    m_volumeInfoLabel->setText(QString("Found %1 volume(s)").arg(volumes.size()));
}

void StoragePoolDialog::updatePoolInfo()
{
    if (!m_currentPool) {
        m_poolInfoLabel->setText("Select a pool to view details");
        return;
    }

    QString info = QString("<b>Pool:</b> %1<br>").arg(m_currentPool->name());
    info += QString("<b>UUID:</b> %1<br>").arg(m_currentPool->uuid());
    info += QString("<b>State:</b> %1<br>").arg(EnumMapper::poolStateToString(m_currentPool->state()));
    info += QString("<b>Type:</b> %1<br>").arg(EnumMapper::poolTypeToString(m_currentPool->type()));
    info += QString("<b>Capacity:</b> %1 GB<br>").arg(m_currentPool->capacity() / (1024.0 * 1024 * 1024), 0, 'f', 2);
    info += QString("<b>Allocation:</b> %1 GB<br>").arg(m_currentPool->allocation() / (1024.0 * 1024 * 1024), 0, 'f', 2);
    info += QString("<b>Available:</b> %1 GB").arg(m_currentPool->available() / (1024.0 * 1024 * 1024), 0, 'f', 2);

    m_poolInfoLabel->setText(info);

    // Update button states
    bool isActive = m_currentPool->state() == StoragePool::StateRunning;
    m_btnPoolStart->setEnabled(!isActive);
    m_btnPoolStop->setEnabled(isActive);
    m_btnPoolRefresh->setEnabled(isActive);
    m_btnPoolDelete->setEnabled(!isActive);
}

void StoragePoolDialog::updateVolumeInfo()
{
    if (!m_currentVolume) {
        m_volumeInfoLabel->setText("Select a volume to view details");
        return;
    }

    QString info = QString("<b>Volume:</b> %1<br>").arg(m_currentVolume->name());
    info += QString("<b>Type:</b> %1<br>").arg(EnumMapper::volumeTypeToString(m_currentVolume->type()));
    info += QString("<b>Capacity:</b> %1 GB<br>").arg(m_currentVolume->capacity() / (1024.0 * 1024 * 1024), 0, 'f', 2);
    info += QString("<b>Allocation:</b> %1 GB<br>").arg(m_currentVolume->allocation() / (1024.0 * 1024 * 1024), 0, 'f', 2);
    info += QString("<b>Format:</b> %1").arg(m_currentVolume->format());

    m_volumeInfoLabel->setText(info);

    // Update button states
    m_btnVolumeDelete->setEnabled(true);
    m_btnVolumeDownload->setEnabled(true);
    m_btnVolumeUpload->setEnabled(true);
    m_btnVolumeClone->setEnabled(true);
    m_btnVolumeWipe->setEnabled(true);
}

void StoragePoolDialog::onPoolSelected()
{
    // Get selected pool from table
    // Current implementation: use first pool from connection
    // Future enhancement: implement proper table selection model
    QList<StoragePool*> pools = m_connection->storagePools();
    if (!pools.isEmpty()) {
        m_currentPool = pools.first();
        updatePoolInfo();
        m_btnVolumeCreate->setEnabled(m_currentPool->state() == StoragePool::StateRunning);
        updateVolumeList();
    }
}

void StoragePoolDialog::onVolumeSelected()
{
    // Get selected volume from table
    // Current implementation: use first volume from pool
    // Future enhancement: implement proper table selection model
    if (m_currentPool) {
        QList<StorageVolume*> volumes = m_currentPool->volumes();
        if (!volumes.isEmpty()) {
            m_currentVolume = volumes.first();
            updateVolumeInfo();
        }
    }
}

void StoragePoolDialog::onPoolStart()
{
    if (!m_currentPool) {
        return;
    }

    if (m_currentPool->start()) {
        m_poolInfoLabel->setText(QString("Pool '%1' started").arg(m_currentPool->name()));
        updatePoolInfo();
        updateVolumeList();
    } else {
        QMessageBox::warning(this, "Failed to Start Pool",
            QString("Failed to start pool '%1'").arg(m_currentPool->name()));
    }
}

void StoragePoolDialog::onPoolStop()
{
    if (!m_currentPool) {
        return;
    }

    auto reply = QMessageBox::question(this, "Stop Pool",
        QString("Are you sure you want to stop pool '%1'?").arg(m_currentPool->name()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (m_currentPool->stop()) {
            m_poolInfoLabel->setText(QString("Pool '%1' stopped").arg(m_currentPool->name()));
            updatePoolInfo();
        } else {
            QMessageBox::warning(this, "Failed to Stop Pool",
                QString("Failed to stop pool '%1'").arg(m_currentPool->name()));
        }
    }
}

void StoragePoolDialog::onPoolDelete()
{
    if (!m_currentPool) {
        return;
    }

    auto reply = QMessageBox::question(this, "Delete Pool",
        QString("Are you sure you want to delete pool '%1'?").arg(m_currentPool->name()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (m_currentPool->undefine()) {
            m_poolInfoLabel->setText(QString("Pool '%1' deleted").arg(m_currentPool->name()));
            m_currentPool = nullptr;
            updatePoolList();
            updateVolumeList();
        } else {
            QMessageBox::warning(this, "Failed to Delete Pool",
                QString("Failed to delete pool '%1'").arg(m_currentPool->name()));
        }
    }
}

void StoragePoolDialog::onPoolRefresh()
{
    if (!m_currentPool) {
        return;
    }

    if (m_currentPool->refresh()) {
        m_poolInfoLabel->setText(QString("Pool '%1' refreshed").arg(m_currentPool->name()));
        updatePoolInfo();
        updateVolumeList();
    } else {
        QMessageBox::warning(this, "Failed to Refresh Pool",
            QString("Failed to refresh pool '%1'").arg(m_currentPool->name()));
    }
}

void StoragePoolDialog::onVolumeCreate()
{
    if (!m_currentPool) {
        return;
    }

    CreateVolumeDialog dialog(m_currentPool, this);
    if (dialog.exec() == QDialog::Accepted) {
        createVolume();
    }
}

void StoragePoolDialog::createVolume()
{
    if (!m_currentPool) {
        return;
    }

    // Launch the volume creation wizard
    CreateVolumeWizard wizard(m_connection, m_currentPool, this);
    if (wizard.exec() == QDialog::Accepted) {
        // Get the XML from the wizard and create the volume
        QString xml = wizard.getXML();
        if (!xml.isEmpty()) {
            virStorageVolPtr vol = virStorageVolCreateXML(m_currentPool->virPool(), xml.toUtf8().constData(), 0);
            if (vol) {
                QMessageBox::information(this, "Success",
                    "Volume created successfully.");
                updateVolumeList();
            } else {
                QMessageBox::critical(this, "Error",
                    "Failed to create volume.");
            }
        }
    }
}

void StoragePoolDialog::onVolumeDelete()
{
    if (!m_currentVolume) {
        return;
    }

    auto reply = QMessageBox::question(this, "Delete Volume",
        QString("Are you sure you want to delete volume '%1'?").arg(m_currentVolume->name()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (m_currentVolume->delete_()) {
            m_volumeInfoLabel->setText(QString("Volume '%1' deleted").arg(m_currentVolume->name()));
            m_currentVolume = nullptr;
            updateVolumeList();
        } else {
            QMessageBox::warning(this, "Failed to Delete Volume",
                QString("Failed to delete volume '%1'").arg(m_currentVolume->name()));
        }
    }
}

void StoragePoolDialog::onVolumeDownload()
{
    if (!m_currentVolume) {
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, "Save Volume",
        m_currentVolume->name() + ".img");

    if (!filename.isEmpty()) {
        // Download volume to file
        if (m_currentVolume->download(filename, 0)) {
            QMessageBox::information(this, "Success",
                QString("Successfully downloaded volume '%1' to '%2'.").arg(m_currentVolume->name()).arg(filename));
        } else {
            QMessageBox::critical(this, "Error",
                QString("Failed to download volume '%1'.").arg(m_currentVolume->name()));
        }
    }
}

void StoragePoolDialog::onVolumeUpload()
{
    if (!m_currentPool) {
        return;
    }

    QString filename = QFileDialog::getOpenFileName(this, "Upload Volume Image");

    if (!filename.isEmpty()) {
        // Create volume from file (upload)
        QFileInfo fileInfo(filename);
        QString volumeName = fileInfo.baseName();

        // Generate XML for new volume
        qint64 fileSize = fileInfo.size();
        QString xml = QString("<volume>\n"
                              "  <name>%1</name>\n"
                              "  <capacity>%2</capacity>\n"
                              "  <target>\n"
                              "    <path>%3</path>\n"
                              "  </target>\n"
                              "</volume>\n")
                       .arg(volumeName)
                       .arg(fileSize)
                       .arg(volumeName);

        // Create volume and upload data
        virStorageVolPtr vol = virStorageVolCreateXML(m_currentPool->virPool(), xml.toUtf8().constData(), 0);
        if (vol) {
            StorageVolume *newVolume = new StorageVolume(vol, m_currentPool, this);
            if (newVolume->upload(filename, 0)) {
                QMessageBox::information(this, "Success",
                    QString("Successfully uploaded '%1' to pool '%2'.").arg(filename).arg(m_currentPool->name()));
                updateVolumeList();
            } else {
                QMessageBox::critical(this, "Error",
                    QString("Failed to upload data to volume.").arg(filename));
            }
        } else {
            QMessageBox::critical(this, "Error",
                QString("Failed to create volume for upload."));
        }
    }
}

void StoragePoolDialog::onVolumeClone()
{
    if (!m_currentVolume) {
        return;
    }

    bool ok;
    QString newName = QInputDialog::getText(this, "Clone Volume",
        "Enter name for the cloned volume:", QLineEdit::Normal,
        m_currentVolume->name() + "-clone", &ok);

    if (ok && !newName.isEmpty()) {
        // Clone volume
        StorageVolume *clonedVolume = m_currentVolume->clone(newName, 0);
        if (clonedVolume) {
            QMessageBox::information(this, "Success",
                QString("Successfully cloned volume '%1' to '%2'.").arg(m_currentVolume->name()).arg(newName));
            updateVolumeList();
        } else {
            QMessageBox::critical(this, "Error",
                QString("Failed to clone volume '%1'.").arg(m_currentVolume->name()));
        }
    }
}

void StoragePoolDialog::onVolumeWipe()
{
    if (!m_currentVolume) {
        return;
    }

    auto reply = QMessageBox::question(this, "Wipe Volume",
        QString("Are you sure you want to securely wipe volume '%1'?\n\n"
                "This will overwrite all data on the volume.").arg(m_currentVolume->name()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Wipe volume (securely erase data)
        if (m_currentVolume->wipe(0)) {
            QMessageBox::information(this, "Success",
                QString("Successfully wiped volume '%1'.").arg(m_currentVolume->name()));
            updateVolumeList();
        } else {
            QMessageBox::critical(this, "Error",
                QString("Failed to wipe volume '%1'.").arg(m_currentVolume->name()));
        }
    }
}

void StoragePoolDialog::onRefresh()
{
    updatePoolList();
    updateVolumeList();
    updatePoolInfo();
    updateVolumeInfo();
}

void StoragePoolDialog::onContextMenuRequested(const QPoint &pos)
{
    if (m_poolList->indexAt(pos).isValid()) {
        m_contextMenu->exec(m_poolList->viewport()->mapToGlobal(pos));
    }
}

// ============================================================================
// CreateVolumeDialog
// ============================================================================

CreateVolumeDialog::CreateVolumeDialog(StoragePool *pool, QWidget *parent)
    : QDialog(parent)
    , m_pool(pool)
{
    setWindowTitle("Create Storage Volume");
    setMinimumSize(500, 300);

    setupUI();
    validate();
}

void CreateVolumeDialog::setupUI()
{
    auto *layout = new QVBoxLayout(this);

    // Form layout for inputs
    auto *formLayout = new QFormLayout();

    // Volume name
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("Enter volume name");
    connect(m_nameEdit, &QLineEdit::textChanged, this, &CreateVolumeDialog::validate);
    formLayout->addRow("Name:", m_nameEdit);

    // Volume size (in MB)
    m_sizeSpin = new QSpinBox(this);
    m_sizeSpin->setRange(1, 1024 * 1024); // 1 MB to 1 TB
    m_sizeSpin->setValue(10240); // 10 GB default
    m_sizeSpin->setSuffix(" MB");
    m_sizeSpin->setAlignment(Qt::AlignRight);
    connect(m_sizeSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &CreateVolumeDialog::validate);
    formLayout->addRow("Size:", m_sizeSpin);

    // Allocation (in MB)
    m_allocationSpin = new QSpinBox(this);
    m_allocationSpin->setRange(1, 1024 * 1024);
    m_allocationSpin->setValue(10240); // 10 GB default
    m_allocationSpin->setSuffix(" MB");
    m_allocationSpin->setAlignment(Qt::AlignRight);
    formLayout->addRow("Allocation:", m_allocationSpin);

    // Format
    m_formatCombo = new QComboBox(this);
    m_formatCombo->addItem("qcow2", "qcow2");
    m_formatCombo->addItem("raw", "raw");
    m_formatCombo->addItem("vmdk", "vmdk");
    m_formatCombo->addItem("vdi", "vdi");
    m_formatCombo->addItem("vpc", "vpc");
    connect(m_formatCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CreateVolumeDialog::onFormatChanged);
    formLayout->addRow("Format:", m_formatCombo);

    // Max size label
    m_maxSizeLabel = new QLabel(this);
    formLayout->addRow("", m_maxSizeLabel);

    layout->addLayout(formLayout);

    layout->addStretch();

    // Information about available space
    if (m_pool) {
        qint64 availableGB = m_pool->available() / (1024 * 1024 * 1024);
        auto *infoLabel = new QLabel(
            QString("Available space in pool: %1 GB").arg(static_cast<double>(availableGB), 0, 'f', 2),
            this);
        infoLabel->setWordWrap(true);
        layout->addWidget(infoLabel);
    }

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    auto *btnCancel = new QPushButton("Cancel", this);
    m_btnOK = new QPushButton("Create", this);

    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_btnOK, &QPushButton::clicked, this, &QDialog::accept);

    buttonLayout->addWidget(btnCancel);
    buttonLayout->addWidget(m_btnOK);

    layout->addLayout(buttonLayout);

    // Initial format check
    onFormatChanged(m_formatCombo->currentIndex());
}

void CreateVolumeDialog::validate()
{
    bool valid = true;

    // Check name
    if (m_nameEdit->text().trimmed().isEmpty()) {
        valid = false;
    }

    // Check size
    if (m_sizeSpin->value() <= 0) {
        valid = false;
    }

    m_btnOK->setEnabled(valid);
}

void CreateVolumeDialog::onFormatChanged(int index)
{
    QString format = m_formatCombo->itemData(index).toString();

    if (format == "qcow2") {
        m_maxSizeLabel->setText("QCOW2 format: Sparse allocation supported");
        m_allocationSpin->setEnabled(false);
    } else if (format == "raw") {
        m_maxSizeLabel->setText("RAW format: Full allocation required");
        m_allocationSpin->setEnabled(true);
        m_allocationSpin->setValue(m_sizeSpin->value());
    } else {
        m_maxSizeLabel->setText("");
        m_allocationSpin->setEnabled(true);
    }
}

} // namespace QVirt
