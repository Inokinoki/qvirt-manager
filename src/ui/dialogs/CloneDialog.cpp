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

#include "CloneDialog.h"
#include "../../core/Error.h"
#include "../../libvirt/EnumMapper.h"

#include <QMessageBox>
#include <QFileDialog>

namespace QVirt {

CloneDialog::CloneDialog(Domain *sourceDomain, QWidget *parent)
    : QDialog(parent)
    , m_sourceDomain(sourceDomain)
{
    setWindowTitle("Clone Virtual Machine");
    setMinimumSize(600, 400);

    setupUI();
    validate();
}

void CloneDialog::setupUI()
{
    auto *layout = new QVBoxLayout(this);

    // Source VM information
    auto *sourceGroup = new QGroupBox("Source Virtual Machine", this);
    auto *sourceLayout = new QFormLayout(sourceGroup);

    m_sourceNameLabel = new QLabel(m_sourceDomain->name(), sourceGroup);
    m_sourceUUIDLabel = new QLabel(m_sourceDomain->uuid(), sourceGroup);
    m_sourceStateLabel = new QLabel(EnumMapper::stateToString(m_sourceDomain->state()), sourceGroup);

    sourceLayout->addRow("Name:", m_sourceNameLabel);
    sourceLayout->addRow("UUID:", m_sourceUUIDLabel);
    sourceLayout->addRow("State:", m_sourceStateLabel);

    layout->addWidget(sourceGroup);

    // Clone configuration
    auto *configGroup = new QGroupBox("Clone Configuration", this);
    auto *configLayout = new QFormLayout(configGroup);

    // Clone name
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("Enter name for the cloned VM");
    m_nameEdit->setText(m_sourceDomain->name() + "-clone");
    connect(m_nameEdit, &QLineEdit::textChanged, this, &CloneDialog::validate);
    configLayout->addRow("Clone Name:", m_nameEdit);

    // Clone storage
    m_cloneStorageCheck = new QCheckBox("Clone storage volumes", this);
    m_cloneStorageCheck->setChecked(true);
    m_cloneStorageCheck->setToolTip("Create copies of all disk images for the cloned VM");
    connect(m_cloneStorageCheck, &QCheckBox::stateChanged,
            this, &CloneDialog::onCloneStorageChanged);
    configLayout->addRow("", m_cloneStorageCheck);

    // Storage path
    auto *pathLayout = new QHBoxLayout();

    m_storagePathEdit = new QLineEdit(this);
    m_storagePathEdit->setPlaceholderText("Path for cloned storage");
    m_storagePathEdit->setEnabled(true);
    pathLayout->addWidget(m_storagePathEdit);

    m_btnBrowseStorage = new QPushButton("Browse...", this);
    m_btnBrowseStorage->setEnabled(true);
    connect(m_btnBrowseStorage, &QPushButton::clicked, this, &CloneDialog::onStorageBrowse);
    pathLayout->addWidget(m_btnBrowseStorage);

    configLayout->addRow("Storage Path:", pathLayout);

    // Generate new MAC addresses
    m_generateMACsCheck = new QCheckBox("Generate new MAC addresses", this);
    m_generateMACsCheck->setChecked(true);
    m_generateMACsCheck->setToolTip(
        "Generate new MAC addresses for network interfaces to avoid conflicts");
    configLayout->addRow("", m_generateMACsCheck);

    layout->addWidget(configGroup);

    layout->addStretch();

    // Information about cloning
    auto *infoLabel = new QLabel(
        "<b>Note:</b> The cloned VM will be created with the same configuration "
        "as the source VM. Storage volumes will be copied if selected.",
        this);
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    auto *btnCancel = new QPushButton("Cancel", this);
    m_btnOK = new QPushButton("Clone", this);

    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_btnOK, &QPushButton::clicked, this, &QDialog::accept);

    buttonLayout->addWidget(btnCancel);
    buttonLayout->addWidget(m_btnOK);

    layout->addLayout(buttonLayout);
}

void CloneDialog::validate()
{
    bool valid = true;

    // Check clone name
    if (m_nameEdit->text().trimmed().isEmpty()) {
        valid = false;
    }

    // Check clone name is different from source
    if (m_nameEdit->text() == m_sourceDomain->name()) {
        valid = false;
    }

    m_btnOK->setEnabled(valid);
}

void CloneDialog::onStorageBrowse()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select Storage Directory");

    if (!path.isEmpty()) {
        m_storagePathEdit->setText(path);
    }
}

void CloneDialog::onCloneStorageChanged(int state)
{
    bool checked = (state == Qt::Checked);

    m_storagePathEdit->setEnabled(checked);
    m_btnBrowseStorage->setEnabled(checked);
}

} // namespace QVirt
