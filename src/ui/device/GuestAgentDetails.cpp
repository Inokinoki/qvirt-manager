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

#include "GuestAgentDetails.h"
#include <QMessageBox>
#include <QTextEdit>
#include <QDialog>
#include <QDialogButtonBox>

namespace QVirt {

GuestAgentDetails::GuestAgentDetails(Domain *domain, QWidget *parent)
    : QWidget(parent)
    , m_domain(domain)
{
    setupUI();
    refresh();
}

void GuestAgentDetails::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Status group
    m_statusGroup = new QGroupBox(tr("Guest Agent Status"), this);
    auto *statusLayout = new QVBoxLayout(m_statusGroup);

    m_statusLabel = new QLabel(tr("Unknown"), this);
    m_statusLabel->setStyleSheet("font-weight: bold; color: #666;");
    statusLayout->addWidget(m_statusLabel);

    m_versionLabel = new QLabel(tr("Version: N/A"), this);
    statusLayout->addWidget(m_versionLabel);

    mainLayout->addWidget(m_statusGroup);

    // Info group
    m_infoGroup = new QGroupBox(tr("Guest Information"), this);
    auto *infoLayout = new QFormLayout(m_infoGroup);

    m_hostnameLabel = new QLabel(tr("N/A"), this);
    infoLayout->addRow(tr("Hostname:"), m_hostnameLabel);

    m_osLabel = new QLabel(tr("N/A"), this);
    infoLayout->addRow(tr("Operating System:"), m_osLabel);

    m_ipAddressesLabel = new QLabel(tr("N/A"), this);
    m_ipAddressesLabel->setWordWrap(true);
    infoLayout->addRow(tr("IP Addresses:"), m_ipAddressesLabel);

    mainLayout->addWidget(m_infoGroup);

    // Commands group
    m_commandsGroup = new QGroupBox(tr("Guest Agent Commands"), this);
    auto *commandsLayout = new QVBoxLayout(m_commandsGroup);

    auto *btnLayout1 = new QHBoxLayout();
    m_pingBtn = new QPushButton(tr("Ping"), this);
    m_pingBtn->setToolTip(tr("Check if guest agent is responding"));
    connect(m_pingBtn, &QPushButton::clicked, this, &GuestAgentDetails::onPingClicked);
    btnLayout1->addWidget(m_pingBtn);

    m_userInfoBtn = new QPushButton(tr("Get Users"), this);
    m_userInfoBtn->setToolTip(tr("Get logged-in users"));
    connect(m_userInfoBtn, &QPushButton::clicked, this, &GuestAgentDetails::onGetUserInfoClicked);
    btnLayout1->addWidget(m_userInfoBtn);

    commandsLayout->addLayout(btnLayout1);

    auto *btnLayout2 = new QHBoxLayout();
    m_networkBtn = new QPushButton(tr("Network Info"), this);
    m_networkBtn->setToolTip(tr("Get network interfaces"));
    connect(m_networkBtn, &QPushButton::clicked, this, &GuestAgentDetails::onGetNetworkInterfacesClicked);
    btnLayout2->addWidget(m_networkBtn);

    m_fsInfoBtn = new QPushButton(tr("Filesystem Info"), this);
    m_fsInfoBtn->setToolTip(tr("Get filesystem information"));
    connect(m_fsInfoBtn, &QPushButton::clicked, this, &GuestAgentDetails::onGetFSInfoClicked);
    btnLayout2->addWidget(m_fsInfoBtn);

    commandsLayout->addLayout(btnLayout2);

    m_shutdownBtn = new QPushButton(tr("Shutdown Guest"), this);
    m_shutdownBtn->setToolTip(tr("Gracefully shutdown guest via agent"));
    m_shutdownBtn->setStyleSheet("QPushButton { background-color: #ff6b6b; color: white; }");
    connect(m_shutdownBtn, &QPushButton::clicked, this, &GuestAgentDetails::onShutdownClicked);
    commandsLayout->addWidget(m_shutdownBtn);

    mainLayout->addWidget(m_commandsGroup);

    mainLayout->addStretch();
}

void GuestAgentDetails::refresh()
{
    updateStatus();
}

void GuestAgentDetails::updateStatus()
{
    if (!m_domain) {
        m_statusLabel->setText(tr("No domain"));
        m_statusLabel->setStyleSheet("font-weight: bold; color: #999;");
        return;
    }

    // Check if guest agent is available
    bool agentConnected = m_domain->guestAgentConnected();
    
    if (agentConnected) {
        m_statusLabel->setText(tr("Connected"));
        m_statusLabel->setStyleSheet("font-weight: bold; color: #27ae60;");
        
        // Get guest info
        QString hostname = m_domain->guestHostname();
        QString os = m_domain->guestOS();
        QStringList ips = m_domain->guestIPAddresses();

        m_hostnameLabel->setText(hostname.isEmpty() ? tr("N/A") : hostname);
        m_osLabel->setText(os.isEmpty() ? tr("N/A") : os);
        m_ipAddressesLabel->setText(ips.isEmpty() ? tr("N/A") : ips.join(", "));

        m_versionLabel->setText(tr("Version: ") + m_domain->guestAgentVersion());
    } else {
        m_statusLabel->setText(tr("Not Connected"));
        m_statusLabel->setStyleSheet("font-weight: bold; color: #e74c3c;");
        m_versionLabel->setText(tr("Version: N/A"));
        m_hostnameLabel->setText(tr("N/A"));
        m_osLabel->setText(tr("N/A"));
        m_ipAddressesLabel->setText(tr("N/A"));
    }

    // Enable/disable buttons based on connection state
    m_pingBtn->setEnabled(agentConnected);
    m_userInfoBtn->setEnabled(agentConnected);
    m_networkBtn->setEnabled(agentConnected);
    m_fsInfoBtn->setEnabled(agentConnected);
    m_shutdownBtn->setEnabled(agentConnected && m_domain->state() == Domain::StateRunning);
}

void GuestAgentDetails::showResult(const QString &title, const QString &result)
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(title);
    dialog->setMinimumSize(500, 400);

    auto *layout = new QVBoxLayout(dialog);

    auto *textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(result);
    layout->addWidget(textEdit);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, dialog);
    connect(buttons, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
    layout->addWidget(buttons);

    dialog->exec();
    dialog->deleteLater();
}

void GuestAgentDetails::onPingClicked()
{
    if (!m_domain) return;

    bool success = m_domain->guestAgentPing();
    if (success) {
        QMessageBox::information(this, tr("Guest Agent Ping"),
            tr("Guest agent is responding."));
    } else {
        QMessageBox::warning(this, tr("Guest Agent Ping"),
            tr("Failed to ping guest agent."));
    }
}

void GuestAgentDetails::onGetUserInfoClicked()
{
    if (!m_domain) return;

    QStringList users = m_domain->guestGetUsers();
    if (users.isEmpty()) {
        QMessageBox::information(this, tr("Guest Users"),
            tr("No users logged in."));
    } else {
        QString result = tr("Logged-in Users:\n\n");
        for (const QString &user : users) {
            result += "  - " + user + "\n";
        }
        showResult(tr("Guest Users"), result);
    }
}

void GuestAgentDetails::onGetNetworkInterfacesClicked()
{
    if (!m_domain) return;

    auto interfaces = m_domain->guestGetNetworkInterfaces();
    if (interfaces.isEmpty()) {
        QMessageBox::information(this, tr("Network Interfaces"),
            tr("No network interfaces found."));
    } else {
        QString result;
        for (auto it = interfaces.begin(); it != interfaces.end(); ++it) {
            result += tr("Interface: %1\n").arg(it.key());
            result += tr("  Hardware Address: %1\n").arg(it.value().macAddress);
            result += tr("  IP Addresses:\n");
            for (const QString &ip : it.value().ipAddresses) {
                result += "    - " + ip + "\n";
            }
            result += "\n";
        }
        showResult(tr("Network Interfaces"), result);
    }
}

void GuestAgentDetails::onGetFSInfoClicked()
{
    if (!m_domain) return;

    auto filesystems = m_domain->guestGetFilesystems();
    if (filesystems.isEmpty()) {
        QMessageBox::information(this, tr("Filesystems"),
            tr("No filesystem information available."));
    } else {
        QString result;
        for (const auto &fs : filesystems) {
            result += tr("Mount Point: %1\n").arg(fs.mountPoint);
            result += tr("  Device: %1\n").arg(fs.device);
            result += tr("  Type: %1\n").arg(fs.type);
            result += tr("  Total: %1 GB\n").arg(fs.totalSize / 1024.0 / 1024.0 / 1024.0, 0, 'f', 2);
            result += tr("  Used: %1 GB\n").arg(fs.usedSize / 1024.0 / 1024.0 / 1024.0, 0, 'f', 2);
            result += tr("  Free: %1 GB\n\n").arg(fs.freeSize / 1024.0 / 1024.0 / 1024.0, 0, 'f', 2);
        }
        showResult(tr("Filesystems"), result);
    }
}

void GuestAgentDetails::onShutdownClicked()
{
    if (!m_domain) return;

    int ret = QMessageBox::question(this, tr("Shutdown Guest"),
        tr("Are you sure you want to shutdown the guest via agent?\n\n"
           "This will gracefully shutdown the operating system."),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        bool success = m_domain->guestAgentShutdown();
        if (success) {
            QMessageBox::information(this, tr("Shutdown Requested"),
                tr("Shutdown request sent to guest."));
        } else {
            QMessageBox::warning(this, tr("Shutdown Failed"),
                tr("Failed to shutdown guest via agent."));
        }
    }
}

} // namespace QVirt
