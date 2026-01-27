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

#include "CreateVolumeWizard.h"
#include "../../core/Error.h"
#include "../../libvirt/StoragePool.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWizardPage>

namespace QVirt {

//=============================================================================
// Volume Name Page
//=============================================================================

VolumeNamePage::VolumeNamePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Volume Name");
    setSubTitle("Enter a name for the storage volume");

    auto *layout = new QFormLayout(this);

    m_nameEdit = new QLineEdit();
    m_nameEdit->setPlaceholderText("myvolume.qcow2");
    layout->addRow("Volume Name:", m_nameEdit);

    registerField("volumeName*", m_nameEdit);

    auto *infoLabel = new QLabel(
        "The volume name should include the file extension (e.g., .qcow2, .raw, .img)."
    );
    infoLabel->setWordWrap(true);
    layout->addRow(infoLabel);
}

bool VolumeNamePage::validatePage()
{
    return !m_nameEdit->text().isEmpty();
}

//=============================================================================
// Volume Configuration Page
//=============================================================================

VolumeConfigPage::VolumeConfigPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Volume Configuration");
    setSubTitle("Configure the volume size and format");

    auto *layout = new QFormLayout(this);

    // Volume size
    m_sizeLabel = new QLabel("Size (GB):");
    m_sizeSpin = new QSpinBox();
    m_sizeSpin->setRange(1, 10000);
    m_sizeSpin->setValue(20);
    m_sizeSpin->setSuffix(" GB");
    layout->addRow(m_sizeLabel, m_sizeSpin);

    // Volume format
    m_formatCombo = new QComboBox();
    m_formatCombo->addItem("QEMU Copy-On-Write (qcow2)", "qcow2");
    m_formatCombo->addItem("Raw (raw)", "raw");
    m_formatCombo->addItem("QEMU old format (qcow)", "qcow");
    m_formatCombo->addItem("VMware (vmdk)", "vmdk");
    m_formatCombo->addItem("VirtualBox (vdi)", "vdi");
    m_formatCombo->addItem("Parallels (vpc)", "vpc");
    layout->addRow("Format:", m_formatCombo);

    // Allocation
    m_allocationSpin = new QSpinBox();
    m_allocationSpin->setRange(0, 100);
    m_allocationSpin->setValue(0);
    m_allocationSpin->setSuffix(" %");
    m_allocationSpin->setSpecialValueText("Thin provision");
    layout->addRow("Allocation:", m_allocationSpin);

    auto *descLabel = new QLabel(
        "Thin provisioning means the file will grow as data is added. "
        "Full allocation reserves all space immediately."
    );
    descLabel->setWordWrap(true);
    layout->addRow(descLabel);

    connect(m_formatCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VolumeConfigPage::onFormatChanged);
}

void VolumeConfigPage::onFormatChanged(int index)
{
    // Update size label based on format
    Q_UNUSED(index);
    QString format = m_formatCombo->currentData().toString();

    if (format == "raw") {
        m_sizeLabel->setText("Size (GB):");
    } else {
        m_sizeLabel->setText("Max Size (GB):");
    }
}

//=============================================================================
// Volume Options Page
//=============================================================================

VolumeOptionsPage::VolumeOptionsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Volume Options");
    setSubTitle("Configure additional volume options");

    auto *layout = new QVBoxLayout(this);

    auto *optionsGroup = new QGroupBox("Options", this);
    auto *optionsLayout = new QVBoxLayout(optionsGroup);

    m_backingCheck = new QCheckBox("Use backing store (base image)");
    m_backingCheck->setToolTip("Create a volume based on an existing image");
    optionsLayout->addWidget(m_backingCheck);

    m_readOnlyCheck = new QCheckBox("Read-only");
    m_readOnlyCheck->setToolTip("Mark the volume as read-only");
    optionsLayout->addWidget(m_readOnlyCheck);

    m_sparseCheck = new QCheckBox("Sparse volume");
    m_sparseCheck->setChecked(true);
    m_sparseCheck->setToolTip("Create a sparse volume (no space allocated for empty blocks)");
    optionsLayout->addWidget(m_sparseCheck);

    layout->addWidget(optionsGroup);

    auto *infoLabel = new QLabel(
        "These options are advanced and typically not needed for standard VM disks."
    );
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("QLabel { padding: 10px; }");
    layout->addWidget(infoLabel);

    layout->addStretch();
}

//=============================================================================
// Volume Finish Page
//=============================================================================

VolumeFinishPage::VolumeFinishPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Ready to Create");
    setSubTitle("Review and create the storage volume");

    auto *layout = new QVBoxLayout(this);

    auto *summaryLabel = new QLabel(
        "Click 'Finish' to create the storage volume.\n\n"
        "The volume will be added to the storage pool."
    );
    summaryLabel->setWordWrap(true);
    layout->addWidget(summaryLabel);

    layout->addStretch();
}

void VolumeFinishPage::initializePage()
{
    auto *wizard = static_cast<CreateVolumeWizard*>(this->wizard());
    QString name = wizard->field("volumeName").toString();

    auto *configPage = dynamic_cast<VolumeConfigPage*>(wizard->page(1));
    if (configPage) {
        qint64 sizeGB = configPage->getSizeBytes() / (1024 * 1024 * 1024);
        QString format = configPage->getFormat();

        QString summary = QString(
            "Volume Name: %1\n"
            "Size: %2 GB\n"
            "Format: %3\n\n"
            "Click 'Finish' to create the storage volume."
        ).arg(name).arg(sizeGB).arg(format.toUpper());

        auto *label = findChild<QLabel*>();
        if (label) {
            label->setText(summary);
        }
    }
}

//=============================================================================
// CreateVolumeWizard Implementation
//=============================================================================

CreateVolumeWizard::CreateVolumeWizard(Connection *conn, StoragePool *pool, QWidget *parent)
    : QWizard(parent)
    , m_connection(conn)
    , m_pool(pool)
{
    setWindowTitle("Create Storage Volume");
    setMinimumSize(600, 500);
    setWizardStyle(QWizard::ModernStyle);

    setupPages();
}

void CreateVolumeWizard::setupPages()
{
    setPage(0, new VolumeNamePage(this));
    setPage(1, new VolumeConfigPage(this));
    setPage(2, new VolumeOptionsPage(this));
    setPage(3, new VolumeFinishPage(this));

    setStartId(0);
}

QString CreateVolumeWizard::getXML() const
{
    return m_volumeXML;
}

} // namespace QVirt
