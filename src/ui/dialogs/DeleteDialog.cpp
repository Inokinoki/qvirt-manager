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

#include "DeleteDialog.h"
#include "../../libvirt/EnumMapper.h"
#include "../../core/Error.h"

#include <QMessageBox>
#include <QFormLayout>

namespace QVirt {

DeleteDialog::DeleteDialog(Domain *domain, QWidget *parent)
    : QDialog(parent)
    , m_domain(domain)
{
    setWindowTitle("Delete Virtual Machine");
    setMinimumSize(500, 300);

    setupUI();
}

void DeleteDialog::setupUI()
{
    auto *layout = new QVBoxLayout(this);

    // Warning message
    m_messageLabel = new QLabel(
        "Are you sure you want to delete this virtual machine? "
        "This operation cannot be undone."
    );
    m_messageLabel->setWordWrap(true);
    layout->addWidget(m_messageLabel);

    // VM Information group
    auto *infoGroup = new QGroupBox("Virtual Machine Information");
    auto *infoLayout = new QFormLayout(infoGroup);

    m_nameLabel = new QLabel(m_domain->name());
    m_stateLabel = new QLabel(EnumMapper::stateToString(m_domain->state()));
    m_pathLabel = new QLabel("N/A");

    infoLayout->addRow("Name:", m_nameLabel);
    infoLayout->addRow("State:", m_stateLabel);
    infoLayout->addRow("Config Path:", m_pathLabel);

    layout->addWidget(infoGroup);

    // Options group
    auto *optionsGroup = new QGroupBox("Options");
    auto *optionsLayout = new QVBoxLayout(optionsGroup);

    m_deleteStorageCheck = new QCheckBox("Delete associated storage files");
    m_deleteStorageCheck->setChecked(false);
    optionsLayout->addWidget(m_deleteStorageCheck);

    m_forceDeleteCheck = new QCheckBox("Force delete (bypass safety checks)");
    m_forceDeleteCheck->setChecked(false);
    m_forceDeleteCheck->setEnabled(false);
    optionsLayout->addWidget(m_forceDeleteCheck);

    // Warning about storage deletion
    auto *storageWarning = new QLabel(
        "<b>Warning:</b> Deleting storage files will permanently remove all data "
        "on the disk. This cannot be undone."
    );
    storageWarning->setWordWrap(true);
    optionsLayout->addWidget(storageWarning);

    layout->addWidget(optionsGroup);

    layout->addStretch();

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_btnCancel = new QPushButton("Cancel");
    m_btnDelete = new QPushButton("Delete");
    m_btnDelete->setProperty("class", "danger"); // For styling if needed

    buttonLayout->addWidget(m_btnCancel);
    buttonLayout->addWidget(m_btnDelete);

    layout->addLayout(buttonLayout);

    // Connect signals
    connect(m_btnDelete, &QPushButton::clicked, this, &DeleteDialog::onDeleteClicked);
    connect(m_btnCancel, &QPushButton::clicked, this, &DeleteDialog::onCancelClicked);

    // Enable force delete if VM is running
    if (m_domain->state() == Domain::StateRunning ||
        m_domain->state() == Domain::StatePaused) {
        m_forceDeleteCheck->setEnabled(true);
        m_forceDeleteCheck->setChecked(true);
    }
}

void DeleteDialog::onDeleteClicked()
{
    m_deleteStorage = m_deleteStorageCheck->isChecked();
    m_forceDelete = m_forceDeleteCheck->isChecked();

    // Validate
    if (!m_forceDelete) {
        // Check if VM is running
        if (m_domain->state() == Domain::StateRunning) {
            QMessageBox::warning(this, "Cannot Delete",
                "The VM is currently running. Please shut it down first "
                "or use Force delete to stop and delete it.");
            return;
        }

        if (m_domain->state() == Domain::StatePaused) {
            auto reply = QMessageBox::question(this, "Paused VM",
                "The VM is currently paused. Resume and shutdown before deleting?",
                QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                // Would resume and shutdown
                // For now, just warn
                QMessageBox::information(this, "Not Implemented",
                    "The VM will be resumed and shut down before deletion.");
            }
            return;
        }
    }

    if (m_deleteStorage) {
        auto reply = QMessageBox::question(this, "Confirm Storage Deletion",
            "Are you sure you want to delete the storage files? "
            "This will permanently delete all data on the disk.",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply != QMessageBox::Yes) {
            return;
        }
    }

    accept();
}

void DeleteDialog::onCancelClicked()
{
    reject();
}

} // namespace QVirt
