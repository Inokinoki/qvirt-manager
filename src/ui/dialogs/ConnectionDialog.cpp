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

#include "ConnectionDialog.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QCoreApplication>

#ifdef LIBVIRT_FOUND
#include <libvirt/libvirt.h>
#endif

namespace QVirt {

ConnectionDialog::ConnectionDialog(QWidget *parent)
    : QDialog(parent)
    , m_autoconnect(false)
    , m_savePassword(false)
{
    setupUI();
    setWindowTitle(tr("Add Connection"));
    setModal(true);
    updateURI();
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
    m_typeCombo->addItem(tr("Bhyve"), "bhyve");
    m_typeCombo->addItem(tr("Custom"), "custom");
    formLayout->addRow(tr("Hypervisor:"), m_typeCombo);

    // Remote connection checkbox
    m_remoteCheck = new QCheckBox(tr("Remote Connection"));
    m_remoteCheck->setChecked(false);
    formLayout->addRow("", m_remoteCheck);

    // Remote connection group
    m_remoteGroup = new QGroupBox(tr("Remote Settings"));
    QFormLayout *remoteLayout = new QFormLayout(m_remoteGroup);

    // Transport
    m_transportCombo = new QComboBox();
    m_transportCombo->addItem(tr("SSH"), "ssh");
    m_transportCombo->addItem(tr("TLS"), "tls");
    m_transportCombo->addItem(tr("TCP"), "tcp");
    m_transportCombo->addItem(tr("Libvirt SSH"), "libvirt+ssh");
    remoteLayout->addRow(tr("Transport:"), m_transportCombo);

    // Hostname
    m_hostnameEdit = new QLineEdit();
    m_hostnameEdit->setPlaceholderText(tr("e.g., 192.168.1.100 or server.example.com"));
    remoteLayout->addRow(tr("Hostname:"), m_hostnameEdit);

    // Username (optional)
    m_usernameEdit = new QLineEdit();
    m_usernameEdit->setPlaceholderText(tr("Optional, defaults to current user"));
    remoteLayout->addRow(tr("Username:"), m_usernameEdit);

    // Port (optional)
    m_portEdit = new QLineEdit();
    m_portEdit->setPlaceholderText(tr("Optional, default port"));
    m_portEdit->setValidator(new QIntValidator(1, 65535, this));
    remoteLayout->addRow(tr("Port:"), m_portEdit);

    // SSH Key Path (for SSH transport)
    m_sshKeyPathEdit = new QLineEdit();
    m_sshKeyPathEdit->setPlaceholderText(tr("Optional, path to SSH private key"));
    m_browseKeyPathBtn = new QPushButton(tr("Browse..."));
    auto *keyPathLayout = new QHBoxLayout();
    keyPathLayout->addWidget(m_sshKeyPathEdit);
    keyPathLayout->addWidget(m_browseKeyPathBtn);
    remoteLayout->addRow(tr("SSH Key:"), keyPathLayout);

    // Save password option
    m_savePasswordCheck = new QCheckBox(tr("Save Password (insecure)"));
    m_savePasswordCheck->setToolTip(
        tr("Warning: Passwords will be stored in plain text in configuration. "
           "For better security, use SSH keys instead."));
    remoteLayout->addRow("", m_savePasswordCheck);

    formLayout->addRow(m_remoteGroup);

    // Connection URI (read-only, auto-generated)
    m_uriEdit = new QLineEdit();
    m_uriEdit->setReadOnly(true);
    m_uriEdit->setPlaceholderText(tr("Auto-generated URI"));
    formLayout->addRow(tr("URI:"), m_uriEdit);

    // Test Connection button
    m_testConnectionBtn = new QPushButton(tr("Test Connection"));
    m_testConnectionBtn->setEnabled(false);
    formLayout->addRow("", m_testConnectionBtn);

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

    // Initially disable remote group
    m_remoteGroup->setEnabled(false);

    // Connect signals
    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
        updateURI();
        Q_UNUSED(index);
    });

    connect(m_remoteCheck, &QCheckBox::toggled,
            this, &ConnectionDialog::onRemoteToggled);

    connect(m_transportCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ConnectionDialog::onTransportChanged);

    connect(m_hostnameEdit, &QLineEdit::textChanged,
            this, &ConnectionDialog::updateURI);

    connect(m_usernameEdit, &QLineEdit::textChanged,
            this, &ConnectionDialog::updateURI);

    connect(m_portEdit, &QLineEdit::textChanged,
            this, &ConnectionDialog::updateURI);

    connect(m_sshKeyPathEdit, &QLineEdit::textChanged,
            this, &ConnectionDialog::updateURI);

    connect(m_browseKeyPathBtn, &QPushButton::clicked,
            this, [this]() {
        QString path = QFileDialog::getOpenFileName(
            this,
            tr("Select SSH Private Key"),
            QDir::homePath(),
            tr("SSH Keys (*.pem *.key id_rsa id_ed25519 id_ecdsa);;All Files (*)")
        );
        if (!path.isEmpty()) {
            m_sshKeyPathEdit->setText(path);
        }
    });

    connect(m_testConnectionBtn, &QPushButton::clicked,
            this, [this]() {
        testConnection();
    });

    connect(m_btnOK, &QPushButton::clicked, this, &ConnectionDialog::validateAndAccept);
    connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    // Enable OK button only if URI is valid
    connect(m_uriEdit, &QLineEdit::textChanged, [this](const QString &text) {
        m_btnOK->setEnabled(!text.isEmpty());
        m_testConnectionBtn->setEnabled(!text.isEmpty());
    });

    m_btnOK->setEnabled(true);
}

void ConnectionDialog::onRemoteToggled(bool enabled)
{
    // Enable/disable remote settings group
    if (m_remoteGroup) {
        m_remoteGroup->setEnabled(enabled);
    }
    updateURI();
}

void ConnectionDialog::onTransportChanged(int index)
{
    // SSH key path and save password only relevant for SSH-based transports
    bool isSsh = index == 0 || index == 3; // SSH or libvirt+ssh
    m_sshKeyPathEdit->setEnabled(isSsh);
    m_browseKeyPathBtn->setEnabled(isSsh);
    m_savePasswordCheck->setEnabled(isSsh);
    updateURI();
}

void ConnectionDialog::updateURI()
{
    m_uri = buildURI();
    m_uriEdit->setText(m_uri);
}

QString ConnectionDialog::buildURI() const
{
    QString hypervisor = m_typeCombo->currentData().toString();

    // For custom URI preset (e.g., qemu:///session)
    if (hypervisor.contains(":///")) {
        return hypervisor;
    }

    // Local connection (no remote)
    if (!m_remoteCheck->isChecked()) {
        return hypervisor + ":///system";
    }

    // Remote connection
    QString transport = m_transportCombo->currentData().toString();
    QString hostname = m_hostnameEdit->text().trimmed();
    QString username = m_usernameEdit->text().trimmed();
    QString port = m_portEdit->text().trimmed();

    if (hostname.isEmpty()) {
        return QString();
    }

    // Build URI: hypervisor+transport://[username@]hostname[:port]/system
    QString uri = hypervisor + "+" + transport + "://";

    if (!username.isEmpty()) {
        uri += username + "@";
    }

    uri += hostname;

    if (!port.isEmpty()) {
        uri += ":" + port;
    }

    uri += "/system";

    // Note: SSH key path is stored separately and not part of the URI
    // Libvirt will use SSH agent or default key locations

    return uri;
}

void ConnectionDialog::testConnection()
{
    QString testUri = buildURI();
    if (testUri.isEmpty()) {
        QMessageBox::warning(this, tr("Test Connection"),
            tr("Please enter a valid hostname for remote connections."));
        return;
    }

    // Show testing dialog
    QMessageBox testMsg(this);
    testMsg.setWindowTitle(tr("Test Connection"));
    testMsg.setText(tr("Testing connection to %1...").arg(testUri));
    testMsg.setStandardButtons(QMessageBox::NoButton);
    testMsg.show();

    // Process events to show the dialog
    QCoreApplication::processEvents();

    bool success = testConnection(testUri);

    testMsg.hide();

    if (success) {
        QMessageBox::information(this, tr("Test Connection"),
            tr("Successfully connected to %1").arg(testUri));
    } else {
        QMessageBox::critical(this, tr("Test Connection"),
            tr("Failed to connect to %1\n\n"
               "Possible reasons:\n"
               "- Host is unreachable\n"
               "- libvirt is not running on remote host\n"
               "- SSH authentication failed\n"
               "- Firewall is blocking the connection\n"
               "- Username or password is incorrect").arg(testUri));
    }
}

bool ConnectionDialog::testConnection(const QString &testUri)
{
#ifdef LIBVIRT_FOUND
    // Try to open the connection
    virConnectPtr conn = virConnectOpenAuth(
        testUri.toUtf8().constData(),
        nullptr,  // Use default auth callback
        0         // Flags
    );

    if (conn) {
        // Connection successful, close it
        virConnectClose(conn);
        return true;
    }
    return false;
#else
    // No libvirt, just return true for UI testing
    Q_UNUSED(testUri);
    return true;
#endif
}

void ConnectionDialog::validateAndAccept()
{
    m_uri = buildURI();
    m_autoconnect = m_autoconnectCheck->isChecked();
    m_sshKeyPath = m_sshKeyPathEdit->text();
    m_sshUsername = m_usernameEdit->text();

    // For password-based auth, prompt if needed
    if (m_remoteCheck->isChecked() && m_savePasswordCheck->isChecked()) {
        // In a real implementation, you would use QInputDialog with Password mode
        // and store the password securely using QtKeychain or similar
        bool ok;
        QString password = QInputDialog::getText(
            this,
            tr("SSH Password"),
            tr("Enter SSH password for %1@%2:")
                .arg(m_usernameEdit->text().isEmpty() ? QString("<current_user>") : m_usernameEdit->text())
                .arg(m_hostnameEdit->text()),
            QLineEdit::Password,
            QString(),
            &ok
        );

        if (ok) {
            m_sshPassword = password;
        }
    }

    if (!m_uri.isEmpty()) {
        accept();
    }
}

} // namespace QVirt
