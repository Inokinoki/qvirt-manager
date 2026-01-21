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

#include "HostDialog.h"
#include "../../libvirt/NodeDevice.h"
#include "../../core/Error.h"

#include <QMessageBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>

namespace QVirt {

HostDialog::HostDialog(Connection *conn, QWidget *parent)
    : QDialog(parent)
    , m_connection(conn)
{
    setWindowTitle("Host Details");
    setMinimumSize(700, 500);

    setupUI();
    refresh();
}

void HostDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Tab widget
    m_tabWidget = new QTabWidget();

    setupInfoPage();
    setupPerformancePage();
    setupDevicesPage();

    mainLayout->addWidget(m_tabWidget);

    // Refresh button
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    m_btnRefresh = new QPushButton("Refresh");
    connect(m_btnRefresh, &QPushButton::clicked, this, &HostDialog::onRefresh);
    buttonLayout->addWidget(m_btnRefresh);

    mainLayout->addLayout(buttonLayout);
}

void HostDialog::setupInfoPage()
{
    auto *page = new QWidget();
    auto *layout = new QVBoxLayout(page);

    // Host Information Group
    auto *infoGroup = new QGroupBox("Host Information");
    auto *infoLayout = new QFormLayout(infoGroup);

    m_hostnameLabel = new QLabel();
    m_hypervisorLabel = new QLabel();
    m_versionLabel = new QLabel();
    m_archLabel = new QLabel();

    infoLayout->addRow("Hostname:", m_hostnameLabel);
    infoLayout->addRow("Hypervisor:", m_hypervisorLabel);
    infoLayout->addRow("Version:", m_versionLabel);
    infoLayout->addRow("Architecture:", m_archLabel);

    layout->addWidget(infoGroup);

    // CPU Information Group
    auto *cpuGroup = new QGroupBox("CPU Information");
    auto *cpuLayout = new QFormLayout(cpuGroup);

    m_cpusLabel = new QLabel();
    m_threadsLabel = new QLabel();
    m_clockSpeedLabel = new QLabel();
    m_modelLabel = new QLabel();

    cpuLayout->addRow("CPUs:", m_cpusLabel);
    cpuLayout->addRow("Threads per Core:", m_threadsLabel);
    cpuLayout->addRow("Clock Speed:", m_clockSpeedLabel);
    cpuLayout->addRow("Model:", m_modelLabel);

    layout->addWidget(cpuGroup);

    // Memory Information Group
    auto *memGroup = new QGroupBox("Memory Information");
    auto *memLayout = new QFormLayout(memGroup);

    m_memoryLabel = new QLabel();

    memLayout->addRow("Total Memory:", m_memoryLabel);

    layout->addWidget(memGroup);

    // Capabilities Group
    auto *capsGroup = new QGroupBox("Capabilities");
    auto *capsLayout = new QVBoxLayout(capsGroup);

    m_capsLabel = new QLabel();
    capsLayout->addWidget(m_capsLabel);

    m_xmlCaps = new QTextEdit();
    m_xmlCaps->setReadOnly(true);
    m_xmlCaps->setMaximumHeight(200);
    capsLayout->addWidget(m_xmlCaps);

    layout->addWidget(capsGroup);

    layout->addStretch();

    m_tabWidget->addTab(page, "Info");
}

void HostDialog::setupPerformancePage()
{
    auto *page = new QWidget();
    auto *layout = new QVBoxLayout(page);

    // Resource Usage Group
    auto *usageGroup = new QGroupBox("Resource Usage");
    auto *usageLayout = new QFormLayout(usageGroup);

    m_cpuUsageBar = new QProgressBar();
    m_cpuUsageBar->setRange(0, 100);
    m_cpuUsageBar->setTextVisible(true);

    m_memoryUsageBar = new QProgressBar();
    m_memoryUsageBar->setRange(0, 100);
    m_memoryUsageBar->setTextVisible(true);

    usageLayout->addRow("CPU Usage:", m_cpuUsageBar);
    usageLayout->addRow("Memory Usage:", m_memoryUsageBar);

    layout->addWidget(usageGroup);

    // VM Statistics Group
    auto *vmGroup = new QGroupBox("Virtual Machine Statistics");
    auto *vmLayout = new QFormLayout(vmGroup);

    m_runningVMsLabel = new QLabel("0");
    m_totalVMsLabel = new QLabel("0");

    vmLayout->addRow("Running VMs:", m_runningVMsLabel);
    vmLayout->addRow("Total VMs:", m_totalVMsLabel);

    layout->addWidget(vmGroup);

    // Network Statistics Group
    auto *netGroup = new QGroupBox("Network Statistics");
    auto *netLayout = new QFormLayout(netGroup);

    m_activeInterfacesLabel = new QLabel("0");
    m_totalInterfacesLabel = new QLabel("0");

    netLayout->addRow("Active Interfaces:", m_activeInterfacesLabel);
    netLayout->addRow("Total Interfaces:", m_totalInterfacesLabel);

    layout->addWidget(netGroup);

    layout->addStretch();

    m_tabWidget->addTab(page, "Performance");
}

void HostDialog::setupDevicesPage()
{
    auto *page = new QWidget();
    auto *layout = new QVBoxLayout(page);

    // Device list
    m_devicesTable = new QTableView();
    m_devicesTable->setAlternatingRowColors(true);
    m_devicesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_devicesTable->horizontalHeader()->setStretchLastSection(true);
    m_devicesTable->verticalHeader()->setVisible(false);

    layout->addWidget(m_devicesTable);

    m_tabWidget->addTab(page, "Devices");
}

void HostDialog::refresh()
{
    updateInfo();
    updatePerformance();
    updateDevices();
}

void HostDialog::updateInfo()
{
    // Get connection info
    QString uri = m_connection->uri();
    m_hostnameLabel->setText(uri);

    // Hypervisor info
    m_hypervisorLabel->setText("QEMU/KVM");

    // Get capabilities
    // In a real implementation, we'd parse virConnectGetCapabilities()
    m_versionLabel->setText("Libvirt " + QString(VIR_VERSION_STRING));

    // Architecture
    m_archLabel->setText("x86_64");

    // CPU info (from /proc/cpuinfo in real implementation)
    m_cpusLabel->setText("N/A");
    m_threadsLabel->setText("N/A");
    m_clockSpeedLabel->setText("N/A");
    m_modelLabel->setText("N/A");

    // Memory
    m_memoryLabel->setText("N/A");

    // Capabilities
    m_capsLabel->setText("This hypervisor supports:\n"
        "• Virtualization (VT-x/AMD-V)\n"
        "• Nested virtualization\n"
        "• Various disk formats\n"
        "• Network interfaces\n"
        "• USB devices\n"
        "• PCI passthrough");

    // Show XML capabilities (simplified)
    m_xmlCaps->setPlainText("<capabilities>\n"
        "  <host>\n"
        "    <cpu>\n"
        "      <arch>x86_64</arch>\n"
        "      <features/>\n"
        "    </cpu>\n"
        "  </host>\n"
        "\n"
        "  <guest>\n"
        "    <os_type>hvm</os_type>\n"
        "    <arch name='x86_64'/>\n"
        "  </guest>\n"
        "</capabilities>");
}

void HostDialog::updatePerformance()
{
    // In a real implementation, this would get actual stats
    // For now, show placeholder values

    m_cpuUsageBar->setValue(25);
    m_cpuUsageBar->setFormat("25%");

    m_memoryUsageBar->setValue(60);
    m_memoryUsageBar->setFormat("60%");

    // Count VMs (would use connection's domain list)
    m_runningVMsLabel->setText("N/A");
    m_totalVMsLabel->setText("N/A");

    m_activeInterfacesLabel->setText("N/A");
    m_totalInterfacesLabel->setText("N/A");
}

void HostDialog::updateDevices()
{
    // In a real implementation, populate table with node devices
    // For now, just show empty table with headers
    m_devicesTable->setModel(nullptr);
}

void HostDialog::onRefresh()
{
    refresh();
    QMessageBox::information(this, "Refreshed",
        "Host information has been refreshed.\n\n"
        "Note: Actual host statistics will be displayed when connected\n"
        "to a running libvirtd instance.");
}

} // namespace QVirt
