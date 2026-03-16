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

#include "CustomizeGuestDialog.h"
#include "../../libvirt/Guest.h"
#include "../../libvirt/Connection.h"
#include "../../devices/DiskDevice.h"
#include "../../devices/NetworkDevice.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QRegularExpression>

namespace QVirt {

CustomizeGuestDialog::CustomizeGuestDialog(Guest *guest, QWidget *parent)
    : QDialog(parent)
    , m_guest(guest)
{
    setWindowTitle("Customize VM before installation");
    setModal(false);
    resize(800, 600);

    setupUI();
    populateDiskList();
    populateNetworkList();
}

CustomizeGuestDialog::~CustomizeGuestDialog() = default;

void CustomizeGuestDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Tab widget
    m_tabWidget = new QTabWidget(this);
    mainLayout->addWidget(m_tabWidget);

    // Setup tabs
    setupOverviewTab();
    setupCpuMemTab();
    setupDisksTab();
    setupNetworkTab();
    setupBootTab();

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_cancelBtn = new QPushButton("Cancel");
    connect(m_cancelBtn, &QPushButton::clicked, this, &CustomizeGuestDialog::onCancelClicked);
    buttonLayout->addWidget(m_cancelBtn);

    m_okayBtn = new QPushButton("Apply");
    connect(m_okayBtn, &QPushButton::clicked, this, &CustomizeGuestDialog::onOkayClicked);
    buttonLayout->addWidget(m_okayBtn);

    mainLayout->addLayout(buttonLayout);
}

void CustomizeGuestDialog::setupOverviewTab()
{
    auto *overviewTab = new QWidget();
    auto *layout = new QGridLayout(overviewTab);

    int row = 0;

    // VM Name
    layout->addWidget(new QLabel("VM Name:"), row, 0);
    m_nameEdit = new QLineEdit();
    m_nameEdit->setText(m_guest->name());
    layout->addWidget(m_nameEdit, row, 1);
    row++;

    // OS Type
    layout->addWidget(new QLabel("OS Type:"), row, 0);
    m_osTypeCombo = new QComboBox();
    m_osTypeCombo->addItems({"Linux", "Windows", "UNIX", "Other"});
    layout->addWidget(m_osTypeCombo, row, 1);
    row++;

    // OS Version
    layout->addWidget(new QLabel("OS Version:"), row, 0);
    m_osVersionCombo = new QComboBox();
    layout->addWidget(m_osVersionCombo, row, 1);
    row++;

    // UEFI
    m_uefiCheck = new QCheckBox("Use UEFI instead of BIOS");
    m_uefiCheck->setChecked(m_guest->uefiRequested());
    layout->addWidget(m_uefiCheck, row, 0, 1, 2);
    row++;

    layout->setRowStretch(row, 1);

    // Populate OS versions based on type
    auto updateVersions = [this]() {
        m_osVersionCombo->clear();
        QString osType = m_osTypeCombo->currentText();

        if (osType == "Linux") {
            m_osVersionCombo->addItems({"Generic Linux", "Ubuntu 22.04", "Ubuntu 20.04",
                "Debian 11", "Debian 12", "Fedora 38", "Fedora 39", "RHEL 9",
                "RHEL 8", "Arch Linux", "CentOS Stream 9"});
        } else if (osType == "Windows") {
            m_osVersionCombo->addItems({"Windows 11", "Windows 10", "Windows Server 2022",
                "Windows Server 2019", "Windows 8.1", "Windows 7"});
        } else if (osType == "UNIX") {
            m_osVersionCombo->addItems({"FreeBSD 13", "OpenBSD 7", "NetBSD 9"});
        } else {
            m_osVersionCombo->addItem("Generic");
        }
    };

    connect(m_osTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, updateVersions);

    // Set current values
    updateVersions();
    QString osType = m_guest->osType();
    if (osType == "Linux") {
        m_osTypeCombo->setCurrentText("Linux");
    } else if (osType == "Windows") {
        m_osTypeCombo->setCurrentText("Windows");
    } else if (osType == "UNIX") {
        m_osTypeCombo->setCurrentText("UNIX");
    } else {
        m_osTypeCombo->setCurrentText("Other");
    }
}

void CustomizeGuestDialog::setupCpuMemTab()
{
    auto *cpuMemTab = new QWidget();
    auto *layout = new QGridLayout(cpuMemTab);

    int row = 0;

    // Memory
    auto *memGroup = new QGroupBox("Memory");
    auto *memLayout = new QGridLayout(memGroup);

    memLayout->addWidget(new QLabel("Memory (MB):"), 0, 0);
    m_memorySpin = new QSpinBox();
    m_memorySpin->setRange(128, 65536);
    m_memorySpin->setSingleStep(256);
    m_memorySpin->setValue(m_guest->currentMemory() / 1024);
    memLayout->addWidget(m_memorySpin, 0, 1);

    memLayout->addWidget(new QLabel("Max Memory (MB):"), 1, 0);
    m_maxMemorySpin = new QSpinBox();
    m_maxMemorySpin->setRange(128, 65536);
    m_maxMemorySpin->setSingleStep(256);
    m_maxMemorySpin->setValue(m_guest->memory() / 1024);
    memLayout->addWidget(m_maxMemorySpin, 1, 1);

    layout->addWidget(memGroup, row, 0, 1, 2);
    row++;

    // CPUs
    auto *cpuGroup = new QGroupBox("CPUs");
    auto *cpuLayout = new QGridLayout(cpuGroup);

    cpuLayout->addWidget(new QLabel("vCPUs:"), 0, 0);
    m_vcpuSpin = new QSpinBox();
    m_vcpuSpin->setRange(1, 256);
    m_vcpuSpin->setValue(m_guest->vcpus());
    cpuLayout->addWidget(m_vcpuSpin, 0, 1);

    cpuLayout->addWidget(new QLabel("Max vCPUs:"), 1, 0);
    m_maxVcpuSpin = new QSpinBox();
    m_maxVcpuSpin->setRange(1, 256);
    m_maxVcpuSpin->setValue(m_guest->maxVcpus());
    cpuLayout->addWidget(m_maxVcpuSpin, 1, 1);

    layout->addWidget(cpuGroup, row, 0, 1, 2);
    row++;

    layout->setRowStretch(row, 1);
}

void CustomizeGuestDialog::setupDisksTab()
{
    auto *disksTab = new QWidget();
    auto *mainLayout = new QHBoxLayout(disksTab);

    // Left side - disk list
    auto *leftPanel = new QWidget();
    auto *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    m_diskList = new QListWidget();
    connect(m_diskList, &QListWidget::currentRowChanged,
            this, &CustomizeGuestDialog::onCurrentDiskChanged);
    leftLayout->addWidget(m_diskList);

    auto *btnLayout = new QHBoxLayout();
    m_addDiskBtn = new QPushButton("Add");
    connect(m_addDiskBtn, &QPushButton::clicked, this, &CustomizeGuestDialog::onDiskAdded);
    btnLayout->addWidget(m_addDiskBtn);

    m_removeDiskBtn = new QPushButton("Remove");
    connect(m_removeDiskBtn, &QPushButton::clicked, this, &CustomizeGuestDialog::onDiskRemoved);
    btnLayout->addWidget(m_removeDiskBtn);

    leftLayout->addLayout(btnLayout);
    mainLayout->addWidget(leftPanel);

    // Right side - disk details
    auto *detailsPanel = new QWidget();
    auto *detailsLayout = new QGridLayout(detailsPanel);
    detailsLayout->setContentsMargins(0, 0, 0, 0);

    int row = 0;

    detailsLayout->addWidget(new QLabel("Type:"), row, 0);
    m_diskTypeCombo = new QComboBox();
    m_diskTypeCombo->addItems({"File", "Block"});
    detailsLayout->addWidget(m_diskTypeCombo, row, 1);
    row++;

    detailsLayout->addWidget(new QLabel("Path:"), row, 0);
    m_diskPathEdit = new QLineEdit();
    detailsLayout->addWidget(m_diskPathEdit, row, 1);
    row++;

    m_diskBrowseBtn = new QPushButton("Browse...");
    connect(m_diskBrowseBtn, &QPushButton::clicked, this, [this]() {
        QString path = QFileDialog::getSaveFileName(this, "Select Disk Location",
            QDir::homePath(), "QCOW2 Images (*.qcow2);;All Files (*)");
        if (!path.isEmpty()) {
            m_diskPathEdit->setText(path);
        }
    });
    detailsLayout->addWidget(m_diskBrowseBtn, row, 2);
    row++;

    detailsLayout->addWidget(new QLabel("Size (GB):"), row, 0);
    m_diskSizeSpin = new QSpinBox();
    m_diskSizeSpin->setRange(1, 1000000);
    m_diskSizeSpin->setValue(20);
    m_diskSizeSpin->setSuffix(" GB");
    detailsLayout->addWidget(m_diskSizeSpin, row, 1);
    row++;

    detailsLayout->addWidget(new QLabel("Format:"), row, 0);
    m_diskFormatCombo = new QComboBox();
    m_diskFormatCombo->addItems({"qcow2", "raw", "vmdk", "vdi"});
    detailsLayout->addWidget(m_diskFormatCombo, row, 1);
    row++;

    detailsLayout->addWidget(new QLabel("Bus:"), row, 0);
    m_diskBusCombo = new QComboBox();
    m_diskBusCombo->addItems({"Virtio", "SATA", "SCSI", "IDE"});
    detailsLayout->addWidget(m_diskBusCombo, row, 1);
    row++;

    detailsLayout->addWidget(new QLabel("Device:"), row, 0);
    m_diskDeviceCombo = new QComboBox();
    m_diskDeviceCombo->addItems({"Disk", "CDROM", "Floppy"});
    detailsLayout->addWidget(m_diskDeviceCombo, row, 1);
    row++;

    detailsLayout->setRowStretch(row, 1);

    mainLayout->addWidget(detailsPanel);

    // Set initial panel widths
    leftPanel->setMinimumWidth(200);
    leftPanel->setMaximumWidth(250);
    detailsPanel->setMinimumWidth(300);
}

void CustomizeGuestDialog::setupNetworkTab()
{
    auto *networkTab = new QWidget();
    auto *mainLayout = new QHBoxLayout(networkTab);

    // Left side - network list
    auto *leftPanel = new QWidget();
    auto *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    m_networkList = new QListWidget();
    connect(m_networkList, &QListWidget::currentRowChanged,
            this, &CustomizeGuestDialog::onCurrentNetworkChanged);
    leftLayout->addWidget(m_networkList);

    auto *btnLayout = new QHBoxLayout();
    m_addNetworkBtn = new QPushButton("Add");
    connect(m_addNetworkBtn, &QPushButton::clicked, this, &CustomizeGuestDialog::onNetworkAdded);
    btnLayout->addWidget(m_addNetworkBtn);

    m_removeNetworkBtn = new QPushButton("Remove");
    connect(m_removeNetworkBtn, &QPushButton::clicked, this, &CustomizeGuestDialog::onNetworkRemoved);
    btnLayout->addWidget(m_removeNetworkBtn);

    leftLayout->addLayout(btnLayout);
    mainLayout->addWidget(leftPanel);

    // Right side - network details
    auto *detailsPanel = new QWidget();
    auto *detailsLayout = new QGridLayout(detailsPanel);
    detailsLayout->setContentsMargins(0, 0, 0, 0);

    int row = 0;

    detailsLayout->addWidget(new QLabel("Network Type:"), row, 0);
    m_networkTypeCombo = new QComboBox();
    m_networkTypeCombo->addItems({"Virtual Network (NAT)", "Bridged", "Direct", "Isolated", "User", "None"});
    detailsLayout->addWidget(m_networkTypeCombo, row, 1);
    row++;

    detailsLayout->addWidget(new QLabel("Network Source:"), row, 0);
    m_networkSourceCombo = new QComboBox();
    m_networkSourceCombo->addItem("default");
    detailsLayout->addWidget(m_networkSourceCombo, row, 1);
    row++;

    detailsLayout->addWidget(new QLabel("Device Model:"), row, 0);
    m_networkModelCombo = new QComboBox();
    m_networkModelCombo->addItems({"virtio", "e1000", "rtl8139", "e1000e"});
    detailsLayout->addWidget(m_networkModelCombo, row, 1);
    row++;

    detailsLayout->setRowStretch(row, 1);

    mainLayout->addWidget(detailsPanel);

    // Set initial panel widths
    leftPanel->setMinimumWidth(200);
    leftPanel->setMaximumWidth(250);
    detailsPanel->setMinimumWidth(300);
}

void CustomizeGuestDialog::setupBootTab()
{
    auto *bootTab = new QWidget();
    auto *layout = new QVBoxLayout(bootTab);

    // Boot order
    auto *orderGroup = new QGroupBox("Boot Order");
    auto *orderLayout = new QVBoxLayout(orderGroup);

    m_bootOrderList = new QListWidget();
    m_bootOrderList->setSelectionMode(QListWidget::SingleSelection);
    orderLayout->addWidget(m_bootOrderList);

    layout->addWidget(orderGroup);

    // Features
    auto *featuresGroup = new QGroupBox("Features");
    auto *featuresLayout = new QVBoxLayout(featuresGroup);

    m_acpiCheck = new QCheckBox("ACPI");
    m_acpiCheck->setChecked(m_guest->acpiEnabled());
    featuresLayout->addWidget(m_acpiCheck);

    m_apicCheck = new QCheckBox("APIC");
    m_apicCheck->setChecked(m_guest->apicEnabled());
    featuresLayout->addWidget(m_apicCheck);

    layout->addWidget(featuresGroup);

    // Populate boot order list
    m_bootOrderList->addItem("Hard Disk");
    m_bootOrderList->addItem("CDROM");
    m_bootOrderList->addItem("Network (PXE)");
    m_bootOrderList->addItem("Floppy");
}

void CustomizeGuestDialog::populateDiskList()
{
    m_diskList->clear();
    const auto &disks = m_guest->disks();

    for (int i = 0; i < disks.size(); ++i) {
        auto *disk = disks[i];
        QString label = QString("Disk %1: %2 (%3)")
            .arg(i + 1)
            .arg(disk->target().isEmpty() ? "vd?" : disk->target())
            .arg(disk->deviceTypeName());

        if (disk->device() == DiskDevice::DeviceType::CDROM) {
            label = QString("CDROM %1").arg(i + 1);
        }

        m_diskList->addItem(label);
    }

    if (m_diskList->count() > 0) {
        m_diskList->setCurrentRow(0);
    }
}

void CustomizeGuestDialog::populateNetworkList()
{
    m_networkList->clear();
    const auto &networks = m_guest->networkInterfaces();

    for (int i = 0; i < networks.size(); ++i) {
        auto *nic = networks[i];
        QString label = QString("NIC %1: %2")
            .arg(i + 1)
            .arg(nic->networkType() == NetworkDevice::NetworkType::Network ? "Virtual Network" :
                 nic->networkType() == NetworkDevice::NetworkType::Bridge ? "Bridged" :
                 nic->networkType() == NetworkDevice::NetworkType::Direct ? "Direct" : "Other");

        m_networkList->addItem(label);
    }

    if (m_networkList->count() > 0) {
        m_networkList->setCurrentRow(0);
    }
}

void CustomizeGuestDialog::updateDiskDetails()
{
    int row = m_diskList->currentRow();
    if (row < 0 || row >= m_guest->disks().size()) {
        return;
    }

    auto *disk = m_guest->disks()[row];

    // Update UI with disk settings
    m_diskTypeCombo->setCurrentIndex(disk->diskType() == DiskDevice::DiskType::File ? 0 : 1);
    m_diskPathEdit->setText(disk->source());
    m_diskFormatCombo->setCurrentText(disk->driverType().isEmpty() ? "qcow2" : disk->driverType());

    // Bus
    QString busStr = DiskDevice::busTypeToString(disk->bus());
    int busIdx = m_diskBusCombo->findText(busStr);
    if (busIdx >= 0) {
        m_diskBusCombo->setCurrentIndex(busIdx);
    }

    // Device
    QString devStr = DiskDevice::deviceTypeToString(disk->device());
    int devIdx = m_diskDeviceCombo->findText(devStr);
    if (devIdx >= 0) {
        m_diskDeviceCombo->setCurrentIndex(devIdx);
    }
}

void CustomizeGuestDialog::updateNetworkDetails()
{
    int row = m_networkList->currentRow();
    if (row < 0 || row >= m_guest->networkInterfaces().size()) {
        return;
    }

    auto *nic = m_guest->networkInterfaces()[row];

    // Update UI with network settings
    NetworkDevice::NetworkType type = nic->networkType();
    if (type == NetworkDevice::NetworkType::Network) {
        m_networkTypeCombo->setCurrentText("Virtual Network (NAT)");
    } else if (type == NetworkDevice::NetworkType::Bridge) {
        m_networkTypeCombo->setCurrentText("Bridged");
    } else if (type == NetworkDevice::NetworkType::Direct) {
        m_networkTypeCombo->setCurrentText("Direct");
    }

    m_networkSourceCombo->setCurrentText(nic->source());

    QString modelStr = NetworkDevice::modelTypeToString(nic->model());
    int modelIdx = m_networkModelCombo->findText(modelStr);
    if (modelIdx >= 0) {
        m_networkModelCombo->setCurrentIndex(modelIdx);
    }
}

void CustomizeGuestDialog::onCurrentDiskChanged(int row)
{
    if (row >= 0 && row < m_guest->disks().size()) {
        updateDiskDetails();
    }
}

void CustomizeGuestDialog::onCurrentNetworkChanged(int row)
{
    if (row >= 0 && row < m_guest->networkInterfaces().size()) {
        updateNetworkDetails();
    }
}

void CustomizeGuestDialog::onDiskAdded()
{
    auto *disk = new DiskDevice(m_guest);
    disk->setDiskType(DiskDevice::DiskType::File);
    disk->setDevice(DiskDevice::DeviceType::Disk);
    disk->setBus(DiskDevice::BusType::Virtio);
    disk->setTarget(QString::asprintf("vd%c", static_cast<char>('a' + m_guest->disks().size())));
    disk->setDriverType("qcow2");
    disk->setSource("/var/lib/libvirt/images/new-disk.qcow2");

    m_guest->addDisk(disk);
    populateDiskList();
    m_diskList->setCurrentRow(m_diskList->count() - 1);
}

void CustomizeGuestDialog::onDiskRemoved()
{
    int row = m_diskList->currentRow();
    if (row < 0 || row >= m_guest->disks().size()) {
        return;
    }

    auto *disk = m_guest->disks()[row];
    m_guest->removeDisk(disk);
    delete disk;
    populateDiskList();
}

void CustomizeGuestDialog::onNetworkAdded()
{
    auto *nic = new NetworkDevice(m_guest);
    nic->setNetworkType(NetworkDevice::NetworkType::Network);
    nic->setSource("default");
    nic->setModel(NetworkDevice::ModelType::Virtio);
    nic->setMacAddress(NetworkDevice::generateMACAddress());

    m_guest->addNetworkInterface(nic);
    populateNetworkList();
    m_networkList->setCurrentRow(m_networkList->count() - 1);
}

void CustomizeGuestDialog::onNetworkRemoved()
{
    int row = m_networkList->currentRow();
    if (row < 0 || row >= m_guest->networkInterfaces().size()) {
        return;
    }

    auto *nic = m_guest->networkInterfaces()[row];
    m_guest->removeNetworkInterface(nic);
    delete nic;
    populateNetworkList();
}

void CustomizeGuestDialog::onOkayClicked()
{
    // Validate name
    QString name = m_nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Invalid Name", "VM name cannot be empty.");
        return;
    }
    if (name.contains(QRegularExpression("[^a-zA-Z0-9_-]"))) {
        QMessageBox::warning(this, "Invalid Name",
            "VM name can only contain letters, numbers, hyphens, and underscores.");
        return;
    }

    // Update guest with all settings
    m_guest->setName(name);
    m_guest->setUefiRequested(m_uefiCheck->isChecked());

    // Memory (convert MB to KB)
    quint64 currentMem = m_memorySpin->value() * 1024;
    quint64 maxMem = m_maxMemorySpin->value() * 1024;
    m_guest->setCurrentMemory(currentMem);
    m_guest->setMemory(maxMem);

    // CPUs
    m_guest->setVCPUs(m_vcpuSpin->value());
    m_guest->setMaxVCPUs(m_maxVcpuSpin->value());

    // Features
    m_guest->setACPIEnabled(m_acpiCheck->isChecked());
    m_guest->setAPICEnabled(m_apicCheck->isChecked());

    // Update disk settings from UI
    for (int i = 0; i < m_guest->disks().size(); ++i) {
        auto *disk = m_guest->disks()[i];
        // Update from UI - in a full implementation, each disk would have its own settings
        disk->setBus(m_diskBusCombo->currentText().toLower() == "virtio" ?
                     DiskDevice::BusType::Virtio :
                     m_diskBusCombo->currentText().toLower() == "sata" ?
                     DiskDevice::BusType::SATA :
                     m_diskBusCombo->currentText().toLower() == "scsi" ?
                     DiskDevice::BusType::SCSI : DiskDevice::BusType::IDE);
    }

    // Validate guest
    if (!m_guest->validate()) {
        QMessageBox::critical(this, "Invalid Configuration",
            "The VM configuration is invalid:\n" + m_guest->validationError());
        return;
    }

    emit customizationAccepted(m_guest);
    accept();
}

void CustomizeGuestDialog::onCancelClicked()
{
    emit customizationRejected();
    reject();
}

} // namespace QVirt
