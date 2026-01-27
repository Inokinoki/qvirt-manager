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

#include "MigrateDialog.h"
#include "../../core/Error.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

namespace QVirt {

MigrateDialog::MigrateDialog(Domain *domain, QWidget *parent)
    : QDialog(parent)
    , m_domain(domain)
{
    setWindowTitle("Migrate Virtual Machine");
    setMinimumSize(600, 400);

    setupUI();
}

void MigrateDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Title
    auto *titleLabel = new QLabel(QString("<h2>Migrate '%1'</h2>").arg(m_domain->name()));
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Basic options group
    setupBasicOptions();
    auto *basicGroup = new QGroupBox("Basic Options", this);
    auto *basicLayout = new QVBoxLayout(basicGroup);

    // Connection type
    auto *connTypeLayout = new QFormLayout();
    m_connectionTypeCombo = new QComboBox();
    m_connectionTypeCombo->addItem("Remote SSH", 0);
    m_connectionTypeCombo->addItem("Remote TLS", 1);
    m_connectionTypeCombo->addItem("Peer-to-Peer", 2);
    m_connectionTypeCombo->setCurrentIndex(0);
    connect(m_connectionTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MigrateDialog::onConnectionTypeChanged);
    connTypeLayout->addRow("Connection Type:", m_connectionTypeCombo);

    // Remote host
    m_remoteHostEdit = new QLineEdit();
    m_remoteHostEdit->setPlaceholderText("example.com or 192.168.1.100");
    connTypeLayout->addRow("Remote Host:", m_remoteHostEdit);

    // Remote port
    m_remotePortSpin = new QSpinBox();
    m_remotePortSpin->setRange(1, 65535);
    m_remotePortSpin->setValue(22);  // Default SSH port
    connTypeLayout->addRow("Remote Port:", m_remotePortSpin);

    // Username (for SSH)
    m_usernameEdit = new QLineEdit();
    m_usernameEdit->setPlaceholderText("root");
    connTypeLayout->addRow("Username:", m_usernameEdit);

    basicLayout->addLayout(connTypeLayout);

    mainLayout->addWidget(basicGroup);

    // Migration mode
    auto *modeGroup = new QGroupBox("Migration Mode", this);
    auto *modeLayout = new QVBoxLayout(modeGroup);

    m_migrationModeGroup = new QButtonGroup(this);

    m_liveRadio = new QRadioButton("Live Migration (VM continues running)");
    m_liveRadio->setChecked(true);
    m_liveRadio->setToolTip("Migrate the VM while it is running with minimal downtime");
    connect(m_liveRadio, &QRadioButton::toggled, this, &MigrateDialog::onLiveMigrationToggled);
    m_migrationModeGroup->addButton(m_liveRadio, 0);

    m_offlineRadio = new QRadioButton("Offline Migration (VM must be shut off)");
    m_offlineRadio->setToolTip("Migrate the VM while it is shut off");
    m_migrationModeGroup->addButton(m_offlineRadio, 1);

    modeLayout->addWidget(m_liveRadio);
    modeLayout->addWidget(m_offlineRadio);

    mainLayout->addWidget(modeGroup);

    // Advanced options
    setupAdvancedOptions();
    auto *advancedGroup = new QGroupBox("Advanced Options", this);
    auto *advancedLayout = new QFormLayout(advancedGroup);

    m_persistentCheck = new QCheckBox("Make VM persistent on destination");
    m_persistentCheck->setChecked(true);
    m_persistentCheck->setToolTip("VM configuration will be saved on the destination host");
    advancedLayout->addRow(m_persistentCheck);

    m_undefineSourceCheck = new QCheckBox("Undefine VM from source host");
    m_undefineSourceCheck->setToolTip("Remove VM from source host after successful migration");
    advancedLayout->addRow(m_undefineSourceCheck);

    m_compressedCheck = new QCheckBox("Compress migration data");
    m_compressedCheck->setToolTip("Reduce network bandwidth usage during migration");
    advancedLayout->addRow(m_compressedCheck);

    m_allowUnsafeCheck = new QCheckBox("Allow unsafe migration (no storage verification)");
    m_allowUnsafeCheck->setToolTip("Force migration even if storage is not properly shared");
    advancedLayout->addRow(m_allowUnsafeCheck);

    m_bandwidthSpin = new QSpinBox();
    m_bandwidthSpin->setRange(0, 1000000);
    m_bandwidthSpin->setValue(0);
    m_bandwidthSpin->setSuffix(" MiB/s");
    m_bandwidthSpin->setSpecialValueText("Unlimited");
    m_bandwidthSpin->setToolTip("Limit migration bandwidth (0 = unlimited)");
    advancedLayout->addRow("Bandwidth Limit:", m_bandwidthSpin);

    m_maxDowntimeSpin = new QSpinBox();
    m_maxDowntimeSpin->setRange(1, 10000);
    m_maxDowntimeSpin->setValue(30);
    m_maxDowntimeSpin->setSuffix(" ms");
    m_maxDowntimeSpin->setToolTip("Maximum downtime allowed during live migration");
    advancedLayout->addRow("Max Downtime:", m_maxDowntimeSpin);

    mainLayout->addWidget(advancedGroup);

    // Status label
    m_statusLabel = new QLabel("Ready to migrate");
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setStyleSheet("QLabel { padding: 10px; }");
    mainLayout->addWidget(m_statusLabel);

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_btnCancel = new QPushButton("Cancel", this);
    connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(m_btnCancel);

    m_btnMigrate = new QPushButton("Migrate", this);
    m_btnMigrate->setDefault(true);
    m_btnMigrate->setEnabled(false);
    connect(m_btnMigrate, &QPushButton::clicked, this, &MigrateDialog::validateAndAccept);
    buttonLayout->addWidget(m_btnMigrate);

    mainLayout->addLayout(buttonLayout);

    // Connect input changes to validation
    connect(m_remoteHostEdit, &QLineEdit::textChanged, [this]() {
        m_btnMigrate->setEnabled(!m_remoteHostEdit->text().isEmpty());
    });
}

void MigrateDialog::setupBasicOptions()
{
    // Setup is done in setupUI()
}

void MigrateDialog::setupAdvancedOptions()
{
    // Setup is done in setupUI()
}

void MigrateDialog::onAdvancedToggled(bool checked)
{
    Q_UNUSED(checked);
    // Advanced options are always visible in this implementation
}

void MigrateDialog::onLiveMigrationToggled(bool checked)
{
    m_config.live = checked;

    // Update max downtime availability
    m_maxDowntimeSpin->setEnabled(checked);

    // Update status
    if (checked) {
        m_statusLabel->setText("Live migration: VM will continue running with minimal downtime");
    } else {
        m_statusLabel->setText("Offline migration: VM will be shut off before migration");
    }
}

void MigrateDialog::onConnectionTypeChanged()
{
    int type = m_connectionTypeCombo->currentData().toInt();

    switch (type) {
        case 0:  // SSH
            m_remotePortSpin->setValue(22);
            m_usernameEdit->setEnabled(true);
            break;
        case 1:  // TLS
            m_remotePortSpin->setValue(16514);
            m_usernameEdit->setEnabled(false);
            break;
        case 2:  // Peer-to-Peer
            m_remotePortSpin->setValue(16509);
            m_usernameEdit->setEnabled(false);
            break;
    }
}

void MigrateDialog::validateAndAccept()
{
    // Validate inputs
    if (m_remoteHostEdit->text().isEmpty()) {
        m_statusLabel->setText("<span style='color: red;'>Error: Remote host is required</span>");
        return;
    }

    // Check domain state for live migration
    if (m_liveRadio->isChecked()) {
        if (m_domain->state() != Domain::StateRunning) {
            m_statusLabel->setText("<span style='color: red;'>Error: VM must be running for live migration</span>");
            return;
        }
    } else {
        if (m_domain->state() != Domain::StateShutOff) {
            auto result = QMessageBox::warning(
                this,
                "VM Running",
                "The VM is currently running. It will be shut off before offline migration.\n\nContinue?",
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Cancel
            );
            if (result != QMessageBox::Ok) {
                return;
            }
        }
    }

    // Build connection URI
    QString uri;
    int type = m_connectionTypeCombo->currentData().toInt();

    switch (type) {
        case 0:  // SSH
            {
                QString username = m_usernameEdit->text().isEmpty() ?
                    QString() : m_usernameEdit->text() + "@";
                uri = QString("qemu+ssh://%1%2/system").arg(username, m_remoteHostEdit->text());
            }
            break;
        case 1:  // TLS
            uri = QString("qemu+%1/system").arg(m_remoteHostEdit->text());
            break;
        case 2:  // Peer-to-Peer
            uri = QString("qemu+tcp://%1/system").arg(m_remoteHostEdit->text());
            break;
    }

    m_config.destinationUri = uri;
    m_config.live = m_liveRadio->isChecked();
    m_config.persistent = m_persistentCheck->isChecked();
    m_config.undefineSource = m_undefineSourceCheck->isChecked();
    m_config.compressed = m_compressedCheck->isChecked();
    m_config.bandwidth = m_bandwidthSpin->value();
    m_config.maxDowntime = m_maxDowntimeSpin->value();

    // Show summary
    QString summary = QString(
        "Migration Summary:\n"
        "• Destination: %1\n"
        "• Mode: %2\n"
        "• Persistent: %3\n"
    ).arg(m_config.destinationUri)
     .arg(m_config.live ? "Live" : "Offline")
     .arg(m_config.persistent ? "Yes" : "No");

    if (m_config.bandwidth > 0) {
        summary += QString("• Bandwidth Limit: %1 MiB/s\n").arg(m_config.bandwidth);
    }

    if (m_config.live) {
        summary += QString("• Max Downtime: %1 ms\n").arg(m_config.maxDowntime);
    }

    auto result = QMessageBox::information(
        this,
        "Confirm Migration",
        summary + "\nProceed with migration?",
        QMessageBox::Ok | QMessageBox::Cancel,
        QMessageBox::Cancel
    );

    if (result == QMessageBox::Ok) {
        accept();
    }
}

} // namespace QVirt
