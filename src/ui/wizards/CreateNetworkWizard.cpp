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

#include "CreateNetworkWizard.h"
#include "../../core/Error.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWizardPage>

namespace QVirt {

//=============================================================================
// Network Name Page
//=============================================================================

NetworkNamePage::NetworkNamePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Network Name");
    setSubTitle("Enter a name for the virtual network");

    auto *layout = new QFormLayout(this);

    m_nameEdit = new QLineEdit();
    m_nameEdit->setPlaceholderText("mynetwork");
    layout->addRow("Network Name:", m_nameEdit);

    registerField("networkName*", m_nameEdit);
}

bool NetworkNamePage::validatePage()
{
    return !m_nameEdit->text().isEmpty();
}

//=============================================================================
// Network Mode Page
//=============================================================================

NetworkModePage::NetworkModePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Network Mode");
    setSubTitle("Choose the network forwarding mode");

    auto *layout = new QVBoxLayout(this);

    // Mode selection
    auto *modeGroup = new QGroupBox("Forwarding Mode", this);
    auto *modeLayout = new QVBoxLayout(modeGroup);

    m_modeCombo = new QComboBox();
    m_modeCombo->addItem("NAT (Network Address Translation)", "nat");
    m_modeCombo->addItem("Routed (Forwarding)", "route");
    m_modeCombo->addItem("Isolated (No forwarding)", "isolated");
    m_modeCombo->addItem("Bridge (Physical network bridge)", "bridge");

    modeLayout->addWidget(m_modeCombo);
    layout->addWidget(modeGroup);

    // Description
    auto *descLabel = new QLabel();
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet("QLabel { padding: 10px; }");
    layout->addWidget(descLabel);

    connect(m_modeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NetworkModePage::updateDescription);

    layout->addStretch();

    updateDescription();
}

void NetworkModePage::updateDescription()
{
    QString mode = m_modeCombo->currentData().toString();
    QString desc;

    if (mode == "nat") {
        desc = "NAT Mode:\n\n"
               "Virtual machines will have private IP addresses. "
               "The network will provide NAT and DHCP services.\n\n"
               "Recommended for: Most users. Provides Internet access to VMs.";
    } else if (mode == "route") {
        desc = "Routed Mode:\n\n"
               "Virtual machines will have private IP addresses. "
               "Routing must be configured on the host for network access.\n\n"
               "Recommended for: Advanced users with existing routing infrastructure.";
    } else if (mode == "isolated") {
        desc = "Isolated Mode:\n\n"
               "Virtual machines will have private IP addresses. "
               "No routing or NAT provided.\n\n"
               "Recommended for: Testing and development.";
    } else if (mode == "bridge") {
        desc = "Bridge Mode:\n\n"
               "Connects the virtual network to a physical network bridge.\n\n"
               "Recommended for: Advanced users wanting direct network access.";
    }

    auto *descLabel = findChild<QLabel*>();
    if (descLabel) {
        descLabel->setText(desc);
    }
}

//=============================================================================
// IPv4 Configuration Page
//=============================================================================

NetworkIPv4Page::NetworkIPv4Page(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("IPv4 Addressing");
    setSubTitle("Configure IPv4 network settings");

    auto *layout = new QVBoxLayout(this);

    m_ipv4EnabledCheck = new QCheckBox("Enable IPv4");
    m_ipv4EnabledCheck->setChecked(true);
    layout->addWidget(m_ipv4EnabledCheck);

    auto *ipv4Group = new QGroupBox("IPv4 Settings", this);
    ipv4Group->setEnabled(true);
    auto *ipv4Layout = new QFormLayout(ipv4Group);

    m_networkEdit = new QLineEdit();
    m_networkEdit->setPlaceholderText("192.168.122.0");
    ipv4Layout->addRow("Network Address:", m_networkEdit);

    m_dhcp4Check = new QCheckBox("Enable DHCPv4");
    m_dhcp4Check->setChecked(true);
    ipv4Layout->addRow(m_dhcp4Check);

    layout->addWidget(ipv4Group);

    layout->addStretch();

    connect(m_ipv4EnabledCheck, &QCheckBox::toggled,
            ipv4Group, &QGroupBox::setEnabled);
}

//=============================================================================
// IPv6 Configuration Page
//=============================================================================

NetworkIPv6Page::NetworkIPv6Page(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("IPv6 Addressing");
    setSubTitle("Configure IPv6 network settings (optional)");

    auto *layout = new QVBoxLayout(this);

    m_ipv6EnabledCheck = new QCheckBox("Enable IPv6");
    m_ipv6EnabledCheck->setChecked(false);
    layout->addWidget(m_ipv6EnabledCheck);

    auto *ipv6Group = new QGroupBox("IPv6 Settings", this);
    ipv6Group->setEnabled(false);
    auto *ipv6Layout = new QFormLayout(ipv6Group);

    m_prefixEdit = new QLineEdit();
    m_prefixEdit->setPlaceholderText("fd00:0::0");
    ipv6Layout->addRow("Prefix:", m_prefixEdit);

    m_dhcp6Check = new QCheckBox("Enable DHCPv6");
    ipv6Layout->addRow(m_dhcp6Check);

    layout->addWidget(ipv6Group);

    layout->addStretch();

    connect(m_ipv6EnabledCheck, &QCheckBox::toggled,
            ipv6Group, &QGroupBox::setEnabled);
}

//=============================================================================
// Forwarding Configuration Page
//=============================================================================

NetworkForwardPage::NetworkForwardPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Forwarding");
    setSubTitle("Physical network connection (optional)");

    auto *layout = new QVBoxLayout(this);

    m_forwardModeCombo = new QComboBox();
    m_forwardModeCombo->addItem("No forwarding", "none");
    m_forwardModeCombo->addItem("Open", "open");
    m_forwardModeCombo->addItem("NAT to physical device", "nat");
    m_forwardModeCombo->addItem("Routed to physical device", "route");
    layout->addWidget(m_forwardModeCombo);

    auto *deviceGroup = new QGroupBox("Physical Device");
    deviceGroup->setEnabled(false);
    m_devEdit = new QLineEdit();
    m_devEdit->setPlaceholderText("virbr0");
    deviceGroup->layout()->addWidget(m_devEdit);

    layout->addWidget(deviceGroup);

    layout->addStretch();

    connect(m_forwardModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this, deviceGroup](int index) {
                deviceGroup->setEnabled(index != 0);
            });
}

//=============================================================================
// Finish Page
//=============================================================================

NetworkFinishPage::NetworkFinishPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Ready to Create");
    setSubTitle("Review and create the virtual network");

    auto *layout = new QVBoxLayout(this);

    auto *summaryLabel = new QLabel(
        "Click 'Finish' to create the virtual network.\n\n"
        "The network will be added to libvirt's configuration."
    );
    summaryLabel->setWordWrap(true);
    layout->addWidget(summaryLabel);

    layout->addStretch();
}

//=============================================================================
// CreateNetworkWizard Implementation
//=============================================================================

CreateNetworkWizard::CreateNetworkWizard(Connection *conn, QWidget *parent)
    : QWizard(parent)
    , m_connection(conn)
{
    setWindowTitle("Create Virtual Network");
    setMinimumSize(600, 500);
    setWizardStyle(QWizard::ModernStyle);

    setupPages();
}

void CreateNetworkWizard::setupPages()
{
    setPage(0, new NetworkNamePage(this));
    setPage(1, new NetworkModePage(this));
    setPage(2, new NetworkIPv4Page(this));
    setPage(3, new NetworkIPv6Page(this));
    setPage(4, new NetworkForwardPage(this));
    setPage(5, new NetworkFinishPage(this));

    setStartId(0);
}

QString CreateNetworkWizard::getXML() const
{
    return m_networkXML;
}

} // namespace QVirt
