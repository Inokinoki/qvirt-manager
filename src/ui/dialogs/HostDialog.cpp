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

#include "HostDialog.h"
#include "../../libvirt/NodeDevice.h"
#include "../../libvirt/Domain.h"
#include "../../libvirt/Network.h"

#include <QMessageBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QXmlStreamReader>
#include <QDebug>
#include <QStandardItemModel>

#ifdef LIBVIRT_FOUND
#include <libvirt/libvirt.h>
#endif

namespace QVirt {

HostDialog::HostDialog(Connection *conn, QWidget *parent)
    : QDialog(parent)
    , m_connection(conn)
    , m_refreshTimer(nullptr)
{
    setWindowTitle("Host Details");
    setMinimumSize(700, 500);

    setupUI();
    refresh();

    // Setup auto-refresh timer (every 2 seconds)
    m_refreshTimer = new QTimer(this);
    connect(m_refreshTimer, &QTimer::timeout, this, &HostDialog::refresh);
    m_refreshTimer->start(2000);

    // Connect async signals
    connect(m_connection, &Connection::connectionInfoFetched,
            this, &HostDialog::onConnectionInfoFetched);
    connect(m_connection, &Connection::hostStatsFetched,
            this, &HostDialog::onHostStatsFetched);
    connect(m_connection, &Connection::fetchFailed,
            this, &HostDialog::onFetchFailed);
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

    // CPU Usage row with progress bar and percentage label
    auto *cpuLayout = new QHBoxLayout();
    m_cpuUsageBar = new QProgressBar();
    m_cpuUsageBar->setRange(0, 100);
    m_cpuUsageBar->setTextVisible(false);
    m_cpuUsageLabel = new QLabel("0%");
    m_cpuUsageLabel->setMinimumWidth(50);
    cpuLayout->addWidget(m_cpuUsageBar, 1);
    cpuLayout->addWidget(m_cpuUsageLabel);

    // Memory Usage row with progress bar and percentage label
    auto *memLayout = new QHBoxLayout();
    m_memoryUsageBar = new QProgressBar();
    m_memoryUsageBar->setRange(0, 100);
    m_memoryUsageBar->setTextVisible(false);
    m_memoryUsageLabel = new QLabel("0%");
    m_memoryUsageLabel->setMinimumWidth(50);
    memLayout->addWidget(m_memoryUsageBar, 1);
    memLayout->addWidget(m_memoryUsageLabel);

    usageLayout->addRow("CPU Usage:", cpuLayout);
    usageLayout->addRow("Memory Usage:", memLayout);

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
    if (!m_connection || !m_connection->isOpen()) {
        m_hostnameLabel->setText("Not connected");
        m_hypervisorLabel->setText("N/A");
        m_versionLabel->setText("N/A");
        m_archLabel->setText("N/A");
        m_cpusLabel->setText("N/A");
        m_threadsLabel->setText("N/A");
        m_clockSpeedLabel->setText("N/A");
        m_modelLabel->setText("N/A");
        m_memoryLabel->setText("N/A");
        m_capsLabel->setText("Unable to retrieve host information");
        m_xmlCaps->setPlainText("");
        return;
    }

    // Use cached values for immediate display (may be stale on first open)
    QString hostname = m_connection->cachedHostname();
    m_hostnameLabel->setText(hostname.isEmpty() ? "Fetching..." : hostname);

    // Get hypervisor type from URI
    QString uri = m_connection->uri();
    QString hypervisor = "Unknown";
    if (uri.startsWith("qemu")) {
        hypervisor = "QEMU/KVM";
    } else if (uri.startsWith("xen")) {
        hypervisor = "Xen";
    } else if (uri.startsWith("vmware")) {
        hypervisor = "VMware";
    } else if (uri.startsWith("virtualbox")) {
        hypervisor = "VirtualBox";
    }
    m_hypervisorLabel->setText(hypervisor);

    // Use cached version
    QString version = m_connection->cachedLibvirtVersion();
    m_versionLabel->setText(version.isEmpty() ? "Fetching..." : version);

    // Use cached capabilities
    QString capsXml = m_connection->cachedCapabilities();
    if (capsXml.isEmpty()) {
        m_archLabel->setText("N/A");
        m_cpusLabel->setText("N/A");
        m_memoryLabel->setText("N/A");
        m_capsLabel->setText("Fetching capabilities...");
        m_xmlCaps->setPlainText("");
        // Trigger async fetch
        m_connection->fetchConnectionInfoAsync();
        return;
    }

    // Parse capabilities XML with proper structure handling
    QString arch;
    QString cpuModel;
    int cpuCount = 0;
    int threads = 0;
    ulong memoryKB = 0;
    QStringList features;
    double cpuMhz = 0.0;

    QXmlStreamReader xml(capsXml);
    bool inHost = false;
    bool inHostCPU = false;
    bool inCell = false;

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            QString name = xml.name().toString();

            // Track if we're in the <host> section
            if (name == QLatin1String("host")) {
                inHost = true;
            }
            // Track if we're in the host <cpu> section
            else if (name == QLatin1String("cpu") && inHost) {
                inHostCPU = true;
            }
            // Track if we're in a <cell> element
            else if (name == QLatin1String("cell") && inHost) {
                inCell = true;
            }
            // Parse architecture from <host><cpu><arch>
            else if (name == QLatin1String("arch") && inHostCPU) {
                arch = xml.readElementText();
            }
            // Parse CPU model from <host><cpu><model>
            else if (name == QLatin1String("model") && inHostCPU) {
                cpuModel = xml.readElementText();
            }
            // Parse CPU topology (sockets, cores, threads)
            else if (name == QLatin1String("topology") && inHostCPU) {
                QString sockets = xml.attributes().value("sockets").toString();
                QString cores = xml.attributes().value("cores").toString();
                QString threadsAttr = xml.attributes().value("threads").toString();
                if (!sockets.isEmpty() && !cores.isEmpty() && !threadsAttr.isEmpty()) {
                    cpuCount = sockets.toInt() * cores.toInt() * threadsAttr.toInt();
                    threads = threadsAttr.toInt();
                }
            }
            // Parse clock speed from TSC counter
            else if (name == QLatin1String("counter") && inHostCPU) {
                QString counterName = xml.attributes().value("name").toString();
                if (counterName == QLatin1String("tsc")) {
                    QString freqAttr = xml.attributes().value("frequency").toString();
                    if (!freqAttr.isEmpty()) {
                        cpuMhz = freqAttr.toDouble() / 1000000.0; // Convert Hz to MHz
                    }
                }
            }
            // Parse memory from <cell><memory>
            else if (name == QLatin1String("memory") && inCell) {
                QString memText = xml.readElementText();
                bool ok;
                ulong mem = memText.toULong(&ok);
                if (ok && mem > 0) {
                    memoryKB = mem;
                }
            }
            // Parse CPU features
            else if (name == QLatin1String("feature") && inHostCPU) {
                QString featureName = xml.attributes().value("name").toString();
                if (!featureName.isEmpty()) {
                    features.append(featureName);
                }
            }
        } else if (token == QXmlStreamReader::EndElement) {
            QString name = xml.name().toString();
            if (name == QLatin1String("host")) {
                inHost = false;
            } else if (name == QLatin1String("cpu")) {
                inHostCPU = false;
            } else if (name == QLatin1String("cell")) {
                inCell = false;
            }
        }
    }

    m_archLabel->setText(arch.isEmpty() ? "x86_64" : arch);
    m_modelLabel->setText(cpuModel.isEmpty() ? "Unknown" : cpuModel);
    m_cpusLabel->setText(cpuCount > 0 ? QString::number(cpuCount) : "N/A");
    m_threadsLabel->setText(threads > 0 ? QString::number(threads) : "N/A");

    // Format memory
    if (memoryKB > 0) {
        double memoryGB = memoryKB / 1024.0 / 1024.0;
        m_memoryLabel->setText(QString("%1 GB").arg(memoryGB, 0, 'f', 2));
    } else {
        m_memoryLabel->setText("N/A");
    }

    // Format clock speed
    if (cpuMhz > 0) {
        if (cpuMhz >= 1000) {
            m_clockSpeedLabel->setText(QString("%1 GHz").arg(cpuMhz / 1000.0, 0, 'f', 2));
        } else {
            m_clockSpeedLabel->setText(QString("%1 MHz").arg(cpuMhz, 0, 'f', 0));
        }
    } else {
        m_clockSpeedLabel->setText("N/A");
    }

    // Build capabilities text
    QString capsText;
    if (!features.isEmpty()) {
        capsText = "CPU Features:\n";
        for (const QString &feature : features) {
            capsText += "• " + feature + "\n";
        }
    }

    if (!capsText.isEmpty()) {
        m_capsLabel->setText(capsText);
    } else {
        m_capsLabel->setText("No specific capabilities listed");
    }

    // Show full XML (truncated for display)
    m_xmlCaps->setPlainText(capsXml.left(5000) + (capsXml.length() > 5000 ? "\n... (truncated)" : ""));
}

void HostDialog::updatePerformance()
{
    if (!m_connection || !m_connection->isOpen()) {
        m_cpuUsageBar->setValue(0);
        m_memoryUsageBar->setValue(0);
        m_runningVMsLabel->setText("N/A");
        m_totalVMsLabel->setText("N/A");
        m_activeInterfacesLabel->setText("N/A");
        m_totalInterfacesLabel->setText("N/A");
        return;
    }

    // Trigger async host stats fetch
    m_connection->fetchHostStatsAsync();
    // Note: The actual values will be updated via the hostStatsFetched signal

    // Get VM counts (use cached data, don't refresh to avoid race conditions)
    QList<Domain*> domains = m_connection->domains();
    int runningVMs = 0;
    for (Domain *domain : domains) {
        if (domain && domain->state() == Domain::StateRunning) {
            runningVMs++;
        }
    }
    m_runningVMsLabel->setText(QString::number(runningVMs));
    m_totalVMsLabel->setText(QString::number(domains.count()));

    // Get network counts
    QList<Network*> networks = m_connection->networks();
    int activeNetworks = 0;
    for (Network *network : networks) {
        if (network && network->isActive()) {
            activeNetworks++;
        }
    }
    m_activeInterfacesLabel->setText(QString::number(activeNetworks));
    m_totalInterfacesLabel->setText(QString::number(networks.count()));
}

void HostDialog::updateDevices()
{
    m_devicesTable->setModel(nullptr);

    if (!m_connection || !m_connection->isOpen()) {
        return;
    }

    // Get node devices
    QList<NodeDevice*> devices = m_connection->nodeDevices();

    if (devices.isEmpty()) {
        return;
    }

    // Create a simple table model
    auto *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"Name"});

    for (NodeDevice *device : devices) {
        if (!device) {
            continue;
        }

        QList<QStandardItem*> row;
        row.append(new QStandardItem(device->name()));

        model->appendRow(row);
    }

    m_devicesTable->setModel(model);
    m_devicesTable->resizeColumnsToContents();
}

void HostDialog::onRefresh()
{
    refresh();
}

void HostDialog::onConnectionInfoFetched(const QString &hostname, const QString &capabilities,
                                          const QString &version)
{
    // Update hostname
    if (!hostname.isEmpty()) {
        m_hostnameLabel->setText(hostname.isEmpty() ? "Unknown" : hostname);
    }

    // Update version
    if (!version.isEmpty()) {
        m_versionLabel->setText(version);
    }

    // Update capabilities
    if (!capabilities.isEmpty()) {
        // Re-run updateInfo to parse the capabilities XML
        updateInfo();
    }
}

void HostDialog::onHostStatsFetched(int cpuUsage, unsigned long long memoryTotal,
                                     unsigned long long memoryUsed)
{
    // Update CPU usage
    m_cpuUsageBar->setValue(cpuUsage);
    m_cpuUsageLabel->setText(QString("%1%").arg(cpuUsage));

    // Update memory usage
    if (memoryTotal > 0) {
        int memPercentage = static_cast<int>((memoryUsed * 100) / memoryTotal);
        m_memoryUsageBar->setValue(memPercentage);
        m_memoryUsageLabel->setText(QString("%1%").arg(memPercentage));
    }
}

void HostDialog::onFetchFailed(const QString &error)
{
    qWarning() << "Failed to fetch host info:" << error;
    // Show error in relevant labels
    if (m_hostnameLabel->text() == "Fetching...") {
        m_hostnameLabel->setText("Failed");
    }
    if (m_versionLabel->text() == "Fetching...") {
        m_versionLabel->setText("Failed");
    }
    if (m_capsLabel->text() == "Fetching capabilities...") {
        m_capsLabel->setText("Failed to load capabilities");
    }
}

} // namespace QVirt
