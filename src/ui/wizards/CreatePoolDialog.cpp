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

#include "CreatePoolDialog.h"
#include "../../core/Error.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWizardPage>

namespace QVirt {

//=============================================================================
// Pool Name Page
//=============================================================================

PoolNamePage::PoolNamePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Storage Pool Name");
    setSubTitle("Enter a name for the storage pool");

    auto *layout = new QFormLayout(this);

    m_nameEdit = new QLineEdit();
    m_nameEdit->setPlaceholderText("mypool");
    layout->addRow("Pool Name:", m_nameEdit);

    registerField("poolName*", m_nameEdit);
}

bool PoolNamePage::validatePage()
{
    return !m_nameEdit->text().isEmpty();
}

//=============================================================================
// Pool Type Page
//=============================================================================

PoolTypePage::PoolTypePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Storage Pool Type");
    setSubTitle("Choose the type of storage pool");

    auto *layout = new QVBoxLayout(this);

    // Pool type
    auto *typeGroup = new QGroupBox("Pool Type", this);
    auto *typeLayout = new QFormLayout(typeGroup);

    m_typeCombo = new QComboBox();
    m_typeCombo->addItem("Directory Pool", "dir");
    m_typeCombo->addItem("Filesystem Pool", "fs");
    m_typeCombo->addItem("Logical Volume Manager", "logical");
    m_typeCombo->addItem("Disk Pool", "disk");
    m_typeCombo->addItem("iSCSI Target", "iscsi");
    m_typeCombo->addItem("SCSI Host Adapter", "scsi");
    m_typeCombo->addItem("Multipath Device", "mpath");
    m_typeCombo->addItem("RBD (Ceph)", "rbd");
    m_typeCombo->addItem("Sheepdog", "sheepdog");
    m_typeCombo->addItem("GlusterFS", "glusterfs");
    m_typeCombo->addItem("ZFS", "zfs");

    typeLayout->addRow("Pool Type:", m_typeCombo);
    layout->addWidget(typeGroup);

    // Description
    auto *descLabel = new QLabel();
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet("QLabel { padding: 10px; }");
    layout->addWidget(descLabel);

    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PoolTypePage::updateDescription);

    layout->addStretch();

    updateDescription();
}

void PoolTypePage::updateDescription()
{
    QString type = m_typeCombo->currentData().toString();
    QString desc;

    if (type == "dir") {
        desc = "Directory Pool:\n"
               "A file system directory. Uses a pre-existing directory to store disk images.\n\n"
               "Best for: Development and testing environments.";
    } else if (type == "fs") {
        desc = "Filesystem Pool:\n"
               "Formatted filesystem that can be partitioned.\n\n"
               "Best for: Local storage with custom partitioning.";
    } else if (type == "logical") {
        desc = "Logical Volume Manager:\n"
               "LVM volume group for flexible storage management.\n\n"
               "Best for: Production environments requiring snapshots.";
    } else if (type == "disk") {
        desc = "Disk Pool:\n"
               "Entire physical disk or partition.\n\n"
               "Best for: Dedicated storage hardware.";
    } else if (type == "iscsi") {
        desc = "iSCSI Target:\n"
               "Network storage via iSCSI protocol.\n\n"
               "Best for: SAN environments.";
    } else if (type == "rbd") {
        desc = "RBD (Ceph):\n"
               "Distributed block device using Ceph.\n\n"
               "Best for: Clusters and cloud storage.";
    } else if (type == "zfs") {
        desc = "ZFS:\n"
               "Advanced filesystem with built-in volume management.\n\n"
               "Best for: Advanced users requiring snapshots and compression.";
    } else {
        desc = QString("%1 Pool:\nAdvanced storage type.").arg(m_typeCombo->currentText());
    }

    // Find and update the description label
    auto *descLabel = findChild<QLabel*>();
    if (descLabel) {
        descLabel->setText(desc);
    }
}

//=============================================================================
// Pool Target Page
//=============================================================================

PoolTargetPage::PoolTargetPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Target Path");
    setSubTitle("Specify the location for the storage pool");

    auto *layout = new QFormLayout(this);

    m_pathEdit = new QLineEdit();
    m_pathEdit->setPlaceholderText("/path/to/pool");
    layout->addRow("Pool Path:", m_pathEdit);

    m_sourceEdit = new QLineEdit();
    m_sourceEdit->setPlaceholderText("/dev/sdb1");
    m_sourceEdit->setVisible(false);
    layout->addRow("Source Device:", m_sourceEdit);

    m_hostEdit = new QLineEdit();
    m_hostEdit->setPlaceholderText("iscsi.example.com");
    m_hostEdit->setVisible(false);
    layout->addRow("iSCSI Host:", m_hostEdit);

    m_deviceEdit = new QLineEdit();
    m_deviceEdit->setPlaceholderText("/dev/sda");
    m_deviceEdit->setVisible(false);
    layout->addRow("Device Path:", m_deviceEdit);

    registerField("targetPath*", m_pathEdit);
}

void PoolTargetPage::initializePage()
{
    auto *wizard = static_cast<CreatePoolDialog*>(this->wizard());
    if (wizard) {
        // Find the type combo from the type page
        auto *typePage = dynamic_cast<PoolTypePage*>(wizard->page(1));
        if (typePage) {
            QString type = typePage->findChild<QComboBox*>()->currentData().toString();

            // Show/hide fields based on pool type
            m_sourceEdit->setVisible(type == "fs");
            m_hostEdit->setVisible(type == "iscsi");
            m_deviceEdit->setVisible(type == "disk");
        }
    }
}

bool PoolTargetPage::validatePage()
{
    return !m_pathEdit->text().isEmpty();
}

//=============================================================================
// Pool Build Page
//=============================================================================

PoolBuildPage::PoolBuildPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Build Pool");
    setSubTitle("Storage pool options and creation");

    auto *layout = new QVBoxLayout(this);

    auto *optionsGroup = new QGroupBox("Pool Options", this);
    auto *optionsLayout = new QVBoxLayout(optionsGroup);

    m_buildPoolCheck = new QCheckBox("Build the pool after defining");
    m_buildPoolCheck->setChecked(true);
    optionsLayout->addWidget(m_buildPoolCheck);

    m_startPoolCheck = new QCheckBox("Start the pool after building");
    m_startPoolCheck->setChecked(false);
    optionsLayout->addWidget(m_startPoolCheck);

    layout->addWidget(optionsGroup);

    auto *infoLabel = new QLabel(
        "Click 'Finish' to define the storage pool.\n\n"
        "The pool will be added to libvirt's configuration."
    );
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);

    layout->addStretch();
}

//=============================================================================
// CreatePoolDialog Implementation
//=============================================================================

CreatePoolDialog::CreatePoolDialog(Connection *conn, QWidget *parent)
    : QWizard(parent)
    , m_connection(conn)
{
    setWindowTitle("Create Storage Pool");
    setMinimumSize(600, 500);
    setWizardStyle(QWizard::ModernStyle);

    setupPages();
}

void CreatePoolDialog::setupPages()
{
    setPage(0, new PoolNamePage(this));
    setPage(1, new PoolTypePage(this));
    setPage(2, new PoolTargetPage(this));
    setPage(3, new PoolBuildPage(this));

    setStartId(0);
}

QString CreatePoolDialog::getXML() const
{
    return m_poolXML;
}

} // namespace QVirt
