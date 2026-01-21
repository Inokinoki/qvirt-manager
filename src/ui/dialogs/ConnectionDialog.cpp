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

#include "ConnectionDialog.h"
#include <QDebug>

namespace QVirt {

ConnectionDialog::ConnectionDialog(QWidget *parent)
    : QDialog(parent)
    , m_autoconnect(false)
{
    setupUI();
    setWindowTitle(tr("Add Connection"));
    setModal(true);
}

ConnectionDialog::~ConnectionDialog() = default;

void ConnectionDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();

    // Connection type
    m_typeCombo = new QComboBox();
    m_typeCombo->addItem(tr("QEMU/KVM"), "qemu");
    m_typeCombo->addItem(tr("QEMU Session"), "qemu:///session");
    m_typeCombo->addItem(tr("Xen"), "xen");
    m_typeCombo->addItem(tr("LXC"), "lxc");
    m_typeCombo->addItem(tr("OpenVZ"), "openvz");
    m_typeCombo->addItem(tr("VMware"), "vmware");
    m_typeCombo->addItem(tr("Custom"), "custom");
    formLayout->addRow(tr("Hypervisor:"), m_typeCombo);

    // Connection URI
    m_uriEdit = new QLineEdit("qemu:///system");
    m_uriEdit->setPlaceholderText(tr("e.g., qemu:///system"));
    formLayout->addRow(tr("URI:"), m_uriEdit);

    // Auto-connect
    m_autoconnectCheck = new QCheckBox(tr("Auto-connect on startup"));
    formLayout->addRow("", m_autoconnectCheck);

    mainLayout->addLayout(formLayout);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_btnOK = new QPushButton(tr("Connect"));
    m_btnCancel = new QPushButton(tr("Cancel"));

    buttonLayout->addWidget(m_btnOK);
    buttonLayout->addWidget(m_btnCancel);

    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
        switch (index) {
        case 0: // QEMU/KVM
            m_uriEdit->setText("qemu:///system");
            break;
        case 1: // QEMU Session
            m_uriEdit->setText("qemu:///session");
            break;
        case 2: // Xen
            m_uriEdit->setText("xen:///");
            break;
        case 3: // LXC
            m_uriEdit->setText("lxc:///");
            break;
        case 4: // OpenVZ
            m_uriEdit->setText("openvz:///system");
            break;
        case 5: // VMware
            m_uriEdit->setText("vmware:///session");
            break;
        case 6: // Custom
            m_uriEdit->clear();
            m_uriEdit->setFocus();
            break;
        }
    });

    connect(m_btnOK, &QPushButton::clicked, this, &ConnectionDialog::validateAndAccept);
    connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    connect(m_uriEdit, &QLineEdit::textChanged, [this](const QString &text) {
        m_btnOK->setEnabled(!text.isEmpty());
    });

    m_btnOK->setEnabled(true);
}

void ConnectionDialog::validateAndAccept()
{
    m_uri = m_uriEdit->text();
    m_autoconnect = m_autoconnectCheck->isChecked();

    if (!m_uri.isEmpty()) {
        accept();
    }
}

} // namespace QVirt
