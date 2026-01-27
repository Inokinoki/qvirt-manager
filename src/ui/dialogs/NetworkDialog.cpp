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

#include "NetworkDialog.h"
#include "../../core/Error.h"
#include "../../libvirt/EnumMapper.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QGridLayout>
#include <QScrollArea>

namespace QVirt {

NetworkDialog::NetworkDialog(Connection *conn, QWidget *parent)
    : QDialog(parent)
    , m_connection(conn)
    , m_currentNetwork(nullptr)
{
    setWindowTitle("Virtual Networks");
    resize(900, 600);

    setupUI();
    updateNetworkList();
}

void NetworkDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Create tab widget
    m_tabWidget = new QTabWidget(this);

    setupNetworksTab();

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
    connect(m_actionStart, &QAction::triggered, this, &NetworkDialog::onNetworkStart);

    m_actionStop = m_contextMenu->addAction("Stop");
    connect(m_actionStop, &QAction::triggered, this, &NetworkDialog::onNetworkStop);

    m_actionDelete = m_contextMenu->addAction("Delete");
    connect(m_actionDelete, &QAction::triggered, this, &NetworkDialog::onNetworkDelete);

    m_actionRefresh = m_contextMenu->addAction("Refresh");
    connect(m_actionRefresh, &QAction::triggered, this, &NetworkDialog::onNetworkRefresh);
}

void NetworkDialog::setupNetworksTab()
{
    auto *networksTab = new QWidget(m_tabWidget);
    auto *networksLayout = new QVBoxLayout(networksTab);

    // Network list
    auto *networkGroup = new QGroupBox("Virtual Networks", networksTab);
    auto *networkLayout = new QVBoxLayout(networkGroup);

    m_networkList = new QTableView(networkGroup);
    m_networkList->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_networkList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_networkList->setAlternatingRowColors(true);
    m_networkList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_networkList, &QTableView::customContextMenuRequested,
            this, &NetworkDialog::onContextMenuRequested);
    connect(m_networkList, &QTableView::clicked, this, &NetworkDialog::onNetworkSelected);

    networkLayout->addWidget(m_networkList);

    // Network info
    m_networkInfoLabel = new QLabel("Select a network to view details", networkGroup);
    m_networkInfoLabel->setWordWrap(true);
    networkLayout->addWidget(m_networkInfoLabel);

    // Network control buttons
    auto *buttonLayout = new QHBoxLayout();

    m_btnNetworkStart = new QPushButton("Start", networkGroup);
    m_btnNetworkStop = new QPushButton("Stop", networkGroup);
    m_btnNetworkRefresh = new QPushButton("Refresh", networkGroup);
    m_btnNetworkDelete = new QPushButton("Delete", networkGroup);
    m_btnCreateNetwork = new QPushButton("Create Network", networkGroup);
    m_btnRefreshNetworks = new QPushButton("Refresh List", networkGroup);

    connect(m_btnNetworkStart, &QPushButton::clicked, this, &NetworkDialog::onNetworkStart);
    connect(m_btnNetworkStop, &QPushButton::clicked, this, &NetworkDialog::onNetworkStop);
    connect(m_btnNetworkRefresh, &QPushButton::clicked, this, &NetworkDialog::onNetworkRefresh);
    connect(m_btnNetworkDelete, &QPushButton::clicked, this, &NetworkDialog::onNetworkDelete);
    connect(m_btnCreateNetwork, &QPushButton::clicked, this, &NetworkDialog::onCreateNetwork);
    connect(m_btnRefreshNetworks, &QPushButton::clicked, this, &NetworkDialog::onRefresh);

    buttonLayout->addWidget(m_btnNetworkStart);
    buttonLayout->addWidget(m_btnNetworkStop);
    buttonLayout->addWidget(m_btnNetworkRefresh);
    buttonLayout->addWidget(m_btnNetworkDelete);
    buttonLayout->addWidget(m_btnCreateNetwork);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_btnRefreshNetworks);

    networkLayout->addLayout(buttonLayout);

    networksLayout->addWidget(networkGroup);

    m_tabWidget->addTab(networksTab, "Networks");

    // Initial state
    m_btnNetworkStart->setEnabled(false);
    m_btnNetworkStop->setEnabled(false);
    m_btnNetworkRefresh->setEnabled(false);
    m_btnNetworkDelete->setEnabled(false);
}

void NetworkDialog::updateNetworkList()
{
    if (!m_connection) {
        return;
    }

    // Get list of networks from connection
    QList<Network*> networks = m_connection->networks();

    // Update network info with count
    m_networkInfoLabel->setText(QString("Found %1 network(s)").arg(networks.size()));
}

void NetworkDialog::updateNetworkInfo()
{
    if (!m_currentNetwork) {
        m_networkInfoLabel->setText("Select a network to view details");
        return;
    }

    QString info = QString("<b>Network:</b> %1<br>").arg(m_currentNetwork->name());
    info += QString("<b>UUID:</b> %1<br>").arg(m_currentNetwork->uuid());
    info += QString("<b>State:</b> %1<br>").arg(EnumMapper::networkStateToString(m_currentNetwork->state()));
    info += QString("<b>Forward Mode:</b> %1<br>").arg(EnumMapper::forwardModeToString(m_currentNetwork->forwardMode()));

    if (m_currentNetwork->forwardMode() == Network::ForwardNAT ||
        m_currentNetwork->forwardMode() == Network::ForwardRoute) {
        info += QString("<b>IP Address:</b> %1<br>").arg(m_currentNetwork->ipAddress());
        info += QString("<b>Netmask:</b> %1<br>").arg(m_currentNetwork->netmask());
        info += QString("<b>DHCP Range:</b> %1 - %2<br>")
            .arg(m_currentNetwork->dhcpStart())
            .arg(m_currentNetwork->dhcpEnd());
        info += QString("<b>DHCP Active:</b> %1").arg(m_currentNetwork->dhcpEnabled() ? "Yes" : "No");
    } else if (m_currentNetwork->forwardMode() == Network::ForwardBridge) {
        info += QString("<b>Bridge:</b> %1").arg(m_currentNetwork->bridgeName());
    }

    m_networkInfoLabel->setText(info);

    // Update button states
    bool isActive = m_currentNetwork->state() == Network::StateRunning;
    m_btnNetworkStart->setEnabled(!isActive);
    m_btnNetworkStop->setEnabled(isActive);
    m_btnNetworkRefresh->setEnabled(isActive);
    m_btnNetworkDelete->setEnabled(!isActive);
}

void NetworkDialog::onNetworkSelected()
{
    // Get selected network from table
    // Current implementation: use first network from connection
    // Future enhancement: implement proper table selection model
    QList<Network*> networks = m_connection->networks();
    if (!networks.isEmpty()) {
        m_currentNetwork = networks.first();
        updateNetworkInfo();
    }
}

void NetworkDialog::onNetworkStart()
{
    if (!m_currentNetwork) {
        return;
    }

    if (m_currentNetwork->start()) {
        m_networkInfoLabel->setText(QString("Network '%1' started").arg(m_currentNetwork->name()));
        updateNetworkInfo();
    } else {
        QMessageBox::warning(this, "Failed to Start Network",
            QString("Failed to start network '%1'").arg(m_currentNetwork->name()));
    }
}

void NetworkDialog::onNetworkStop()
{
    if (!m_currentNetwork) {
        return;
    }

    auto reply = QMessageBox::question(this, "Stop Network",
        QString("Are you sure you want to stop network '%1'?").arg(m_currentNetwork->name()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (m_currentNetwork->stop()) {
            m_networkInfoLabel->setText(QString("Network '%1' stopped").arg(m_currentNetwork->name()));
            updateNetworkInfo();
        } else {
            QMessageBox::warning(this, "Failed to Stop Network",
                QString("Failed to stop network '%1'").arg(m_currentNetwork->name()));
        }
    }
}

void NetworkDialog::onNetworkDelete()
{
    if (!m_currentNetwork) {
        return;
    }

    auto reply = QMessageBox::question(this, "Delete Network",
        QString("Are you sure you want to delete network '%1'?").arg(m_currentNetwork->name()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (m_currentNetwork->undefine()) {
            m_networkInfoLabel->setText(QString("Network '%1' deleted").arg(m_currentNetwork->name()));
            m_currentNetwork = nullptr;
            updateNetworkList();
        } else {
            QMessageBox::warning(this, "Failed to Delete Network",
                QString("Failed to delete network '%1'").arg(m_currentNetwork->name()));
        }
    }
}

void NetworkDialog::onNetworkRefresh()
{
    if (!m_currentNetwork) {
        return;
    }

    // Refresh network info
    m_currentNetwork->updateInfo();
    updateNetworkInfo();
}

void NetworkDialog::onCreateNetwork()
{
    CreateNetworkDialog dialog(m_connection, this);
    if (dialog.exec() == QDialog::Accepted) {
        createNetwork();
    }
}

void NetworkDialog::createNetwork()
{
    QMessageBox::information(this, "Create Network",
        "Network creation will be implemented with Network class.\n\n"
        "This requires:\n"
        "1. Network XML generation\n"
        "2. virNetworkDefineXML() call\n"
        "3. virNetworkCreate() to start\n"
        "4. Refresh network list");

    updateNetworkList();
}

void NetworkDialog::onRefresh()
{
    updateNetworkList();
    updateNetworkInfo();
}

void NetworkDialog::onContextMenuRequested(const QPoint &pos)
{
    if (m_networkList->indexAt(pos).isValid()) {
        m_contextMenu->exec(m_networkList->viewport()->mapToGlobal(pos));
    }
}

// ============================================================================
// CreateNetworkDialog
// ============================================================================

CreateNetworkDialog::CreateNetworkDialog(Connection *conn, QWidget *parent)
    : QDialog(parent)
    , m_connection(conn)
{
    setWindowTitle("Create Virtual Network");
    setMinimumSize(500, 400);

    setupUI();
    validate();
}

void CreateNetworkDialog::setupUI()
{
    auto *layout = new QVBoxLayout(this);

    // Scroll area for form
    auto *scrollArea = new QWidget(this);
    auto *formLayout = new QFormLayout(scrollArea);

    // Network name
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("Enter network name");
    connect(m_nameEdit, &QLineEdit::textChanged, this, &CreateNetworkDialog::validate);
    formLayout->addRow("Name:", m_nameEdit);

    // Forward mode
    m_modeCombo = new QComboBox(this);
    m_modeCombo->addItem("NAT", "nat");
    m_modeCombo->addItem("Routed", "route");
    m_modeCombo->addItem("Isolated", "none");
    m_modeCombo->addItem("Bridge", "bridge");
    m_modeCombo->addItem("macvtap", "bridge"); // Uses bridge device
    m_modeCombo->addItem("Direct", "passthrough");
    connect(m_modeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CreateNetworkDialog::onModeChanged);
    formLayout->addRow("Forward Mode:", m_modeCombo);

    // Bridge device (for bridge/macvtap modes)
    m_deviceLabel = new QLabel("Bridge Device:", this);
    m_deviceEdit = new QLineEdit(this);
    m_deviceEdit->setPlaceholderText("e.g., virbr0");
    formLayout->addRow(m_deviceLabel, m_deviceEdit);

    // IPv4 settings group
    auto *ipv4Group = new QGroupBox("IPv4 Settings", this);
    auto *ipv4Layout = new QFormLayout(ipv4Group);

    m_ipEdit = new QLineEdit(this);
    m_ipEdit->setPlaceholderText("e.g., 192.168.122.1");
    ipv4Layout->addRow("Network Address:", m_ipEdit);

    m_netmaskEdit = new QLineEdit(this);
    m_netmaskEdit->setPlaceholderText("e.g., 255.255.255.0");
    ipv4Layout->addRow("Netmask:", m_netmaskEdit);

    formLayout->addRow(ipv4Group);

    // DHCP settings
    m_dhcpCheck = new QCheckBox("Enable DHCP", this);
    m_dhcpCheck->setChecked(true);
    connect(m_dhcpCheck, &QCheckBox::stateChanged,
            this, &CreateNetworkDialog::onDHCPEnabledChanged);
    formLayout->addRow(m_dhcpCheck);

    auto *dhcpGroup = new QGroupBox("DHCP Range", this);
    auto *dhcpLayout = new QGridLayout(dhcpGroup);

    m_dhcpStartEdit = new QLineEdit(this);
    m_dhcpStartEdit->setPlaceholderText("e.g., 192.168.122.100");
    dhcpLayout->addWidget(new QLabel("Start:", this), 0, 0);
    dhcpLayout->addWidget(m_dhcpStartEdit, 0, 1);

    m_dhcpEndEdit = new QLineEdit(this);
    m_dhcpEndEdit->setPlaceholderText("e.g., 192.168.122.254");
    dhcpLayout->addWidget(new QLabel("End:", this), 1, 0);
    dhcpLayout->addWidget(m_dhcpEndEdit, 1, 1);

    formLayout->addRow(dhcpGroup);

    // IPv6 settings
    m_ipv6Check = new QCheckBox("Enable IPv6", this);
    formLayout->addRow(m_ipv6Check);

    layout->addWidget(scrollArea);
    layout->addStretch();

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

    // Initial mode check
    onModeChanged(m_modeCombo->currentIndex());
}

QString CreateNetworkDialog::networkMode() const
{
    return m_modeCombo->currentData().toString();
}

void CreateNetworkDialog::validate()
{
    bool valid = true;

    // Check name
    if (m_nameEdit->text().trimmed().isEmpty()) {
        valid = false;
    }

    // Check IP address if NAT or routed mode
    QString mode = networkMode();
    if ((mode == "nat" || mode == "route") && m_ipEdit->text().isEmpty()) {
        valid = false;
    }

    // Check DHCP range if DHCP enabled
    if (m_dhcpCheck->isChecked() &&
        (m_dhcpStartEdit->text().isEmpty() || m_dhcpEndEdit->text().isEmpty())) {
        valid = false;
    }

    m_btnOK->setEnabled(valid);
}

void CreateNetworkDialog::onModeChanged(int index)
{
    QString mode = m_modeCombo->itemData(index).toString();

    // Show/hide device field based on mode
    if (mode == "bridge" || mode == "macvtap" || mode == "passthrough") {
        m_deviceLabel->setVisible(true);
        m_deviceEdit->setVisible(true);
    } else {
        m_deviceLabel->setVisible(false);
        m_deviceEdit->setVisible(false);
    }

    // Enable/disable IP settings based on mode
    bool needsIP = (mode == "nat" || mode == "route");
    m_ipEdit->setEnabled(needsIP);
    m_netmaskEdit->setEnabled(needsIP);
    m_dhcpCheck->setEnabled(needsIP);

    validate();
}

void CreateNetworkDialog::onDHCPEnabledChanged(int state)
{
    bool enabled = (state == Qt::Checked);
    m_dhcpStartEdit->setEnabled(enabled);
    m_dhcpEndEdit->setEnabled(enabled);
    validate();
}

} // namespace QVirt
