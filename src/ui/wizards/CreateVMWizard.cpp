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

#include "CreateVMWizard.h"
#include "../../core/Error.h"
#include "../../libvirt/Guest.h"
#include "../../devices/DiskDevice.h"
#include "../../devices/NetworkDevice.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QDebug>

namespace QVirt {

//=============================================================================
// CreateVMWizard
//=============================================================================

CreateVMWizard::CreateVMWizard(Connection *conn, QWidget *parent)
    : QWizard(parent)
    , m_connection(conn)
{
    setWindowTitle("Create a New Virtual Machine");
    setWizardStyle(QWizard::ModernStyle);
    setOption(QWizard::HaveHelpButton, false);
    setOption(QWizard::HaveCustomButton1, true);

    // Set button text
    setButtonText(QWizard::CustomButton1, "Customize");
    setButtonText(QWizard::FinishButton, "_Finish");
    setButtonText(QWizard::CancelButton, "_Cancel");

    setupPages();
    setupConnections();

    resize(800, 600);
}

CreateVMWizard::~CreateVMWizard() = default;

void CreateVMWizard::setupPages()
{
    setPage(Page_NameAndOS, new NameAndOSPage(this, m_connection));
    setPage(Page_InstallMedia, new InstallMediaPage(this, m_connection));
    setPage(Page_CPU_Memory, new CPUMemoryPage(this, m_connection));
    setPage(Page_Storage, new StoragePage(this, m_connection));
    setPage(Page_Network, new NetworkPage(this, m_connection));
    setPage(Page_Summary, new SummaryPage(this, m_connection));

    setStartId(Page_NameAndOS);
}

void CreateVMWizard::setupConnections()
{
    connect(this, &QWizard::currentIdChanged, this, &CreateVMWizard::onCurrentIdChanged);
}

void CreateVMWizard::onCurrentIdChanged(int id)
{
    Q_UNUSED(id);
    // Can perform page-specific actions here
}

bool CreateVMWizard::validateCurrentPage()
{
    // Basic validation - can be extended per page
    return true;
}

QString CreateVMWizard::vmName() const
{
    return field("vmName").toString();
}

QString CreateVMWizard::osType() const
{
    return field("osType").toString();
}

int CreateVMWizard::memoryMB() const
{
    return field("memory").toInt();
}

int CreateVMWizard::vcpus() const
{
    return field("vcpus").toInt();
}

QString CreateVMWizard::diskPath() const
{
    return field("diskPath").toString();
}

qint64 CreateVMWizard::diskSizeGB() const
{
    return field("diskSizeGB").toLongLong();
}

QString CreateVMWizard::networkType() const
{
    return field("networkType").toString();
}

QString CreateVMWizard::networkSource() const
{
    return field("networkSource").toString();
}

bool CreateVMWizard::startAfterInstall() const
{
    return field("startAfterInstall").toBool();
}

//=============================================================================
// NameAndOSPage
//=============================================================================

NameAndOSPage::NameAndOSPage(CreateVMWizard *wizard, Connection *conn)
    : QWizardPage(wizard)
    , m_wizard(wizard)
    , m_connection(conn)
{
    setTitle("Step 1: Name and Operating System");
    setSubTitle("Choose a name and operating system type for your virtual machine.");

    setupUI();
}

void NameAndOSPage::setupUI()
{
    auto *layout = new QGridLayout(this);

    int row = 0;

    // VM Name
    QLabel *nameLabel = new QLabel("Name:");
    m_nameEdit = new QLineEdit();
    m_nameEdit->setPlaceholderText("New VM");
    registerField("vmName*", m_nameEdit);

    layout->addWidget(nameLabel, row, 0);
    layout->addWidget(m_nameEdit, row, 1);
    row++;

    // OS Type
    QLabel *osLabel = new QLabel("OS Type:");
    m_osTypeCombo = new QComboBox();
    registerField("osType", m_osTypeCombo, "currentText");

    layout->addWidget(osLabel, row, 0);
    layout->addWidget(m_osTypeCombo, row, 1);
    row++;

    // OS Version
    QLabel *versionLabel = new QLabel("Version:");
    m_osVersionCombo = new QComboBox();
    m_osIcon = new QLabel();
    m_osIcon->setMinimumSize(64, 64);
    m_osIcon->setAlignment(Qt::AlignCenter);

    layout->addWidget(versionLabel, row, 0);
    layout->addWidget(m_osVersionCombo, row, 1);
    row++;

    // Icon
    layout->addWidget(m_osIcon, row, 0, 1, 2, Qt::AlignCenter);

    layout->setRowStretch(row, 1);

    // Load OS types
    loadOSTypes();

    // Connect OS type change to update versions
    connect(m_osTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this]() {
        // Update versions based on selected OS type
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
    });

    // Set defaults
    m_osTypeCombo->setCurrentIndex(0);
    m_nameEdit->setFocus();
}

void NameAndOSPage::loadOSTypes()
{
    m_osTypeCombo->addItems({"Linux", "Windows", "UNIX", "Other"});
}

void NameAndOSPage::initializePage()
{
    // Suggest a VM name based on OS selection
    if (m_nameEdit->text().isEmpty()) {
        QString osType = m_osTypeCombo->currentText();
        QString suggestion = QString("%1-vm").arg(osType.toLower());
        m_nameEdit->setText(suggestion);
    }
}

bool NameAndOSPage::validatePage()
{
    QString name = m_nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Invalid Name",
            "Please enter a name for the virtual machine.");
        return false;
    }

    // Check for invalid characters
    if (name.contains(QRegularExpression("[^a-zA-Z0-9_-]"))) {
        QMessageBox::warning(this, "Invalid Name",
            "VM name can only contain letters, numbers, hyphens, and underscores.");
        return false;
    }

    return true;
}

//=============================================================================
// InstallMediaPage
//=============================================================================

InstallMediaPage::InstallMediaPage(CreateVMWizard *wizard, Connection *conn)
    : QWizardPage(wizard)
    , m_wizard(wizard)
    , m_connection(conn)
{
    setTitle("Step 2: Installation Media");
    setSubTitle("Choose how you would like to install the operating system.");

    setupUI();
}

void InstallMediaPage::setupUI()
{
    auto *layout = new QVBoxLayout(this);

    // Install type group
    auto *typeGroup = new QGroupBox("Installation Method");
    auto *typeLayout = new QVBoxLayout(typeGroup);

    m_radioISO = new QRadioButton("Install from local ISO image");
    m_radioURL = new QRadioButton("Install from network (URL)");
    m_radioImport = new QRadioButton("Import existing disk image");
    m_radioPXE = new QRadioButton("Network Boot (PXE)");
    m_radioContainer = new QRadioButton("Container Boot");

    m_radioISO->setChecked(true); // Default

    typeLayout->addWidget(m_radioISO);
    typeLayout->addWidget(m_radioURL);
    typeLayout->addWidget(m_radioImport);
    typeLayout->addWidget(m_radioPXE);
    typeLayout->addWidget(m_radioContainer);

    layout->addWidget(typeGroup);

    // Media settings
    auto *settingsGroup = new QGroupBox("Media Settings");
    auto *settingsLayout = new QGridLayout(settingsGroup);

    int row = 0;

    // ISO path
    m_isoPathEdit = new QLineEdit();
    m_isoBrowseBtn = new QPushButton("Browse...");

    settingsLayout->addWidget(new QLabel("ISO Path:"), row, 0);
    settingsLayout->addWidget(m_isoPathEdit, row, 1);
    settingsLayout->addWidget(m_isoBrowseBtn, row, 2);
    row++;

    // URL
    m_urlEdit = new QLineEdit();
    m_urlEdit->setPlaceholderText("https://example.com/installer");

    settingsLayout->addWidget(new QLabel("URL:"), row, 0);
    settingsLayout->addWidget(m_urlEdit, row, 1, 1, 2);
    row++;

    // Import path
    m_importPathEdit = new QLineEdit();
    m_importBrowseBtn = new QPushButton("Browse...");

    settingsLayout->addWidget(new QLabel("Disk Image:"), row, 0);
    settingsLayout->addWidget(m_importPathEdit, row, 1);
    settingsLayout->addWidget(m_importBrowseBtn, row, 2);

    settingsLayout->setColumnStretch(1, 1);

    layout->addWidget(settingsGroup);
    layout->addStretch();

    // Connect signals
    connect(m_radioISO, &QRadioButton::toggled, this, &InstallMediaPage::onInstallTypeChanged);
    connect(m_radioURL, &QRadioButton::toggled, this, &InstallMediaPage::onInstallTypeChanged);
    connect(m_radioImport, &QRadioButton::toggled, this, &InstallMediaPage::onInstallTypeChanged);
    connect(m_radioPXE, &QRadioButton::toggled, this, &InstallMediaPage::onInstallTypeChanged);
    connect(m_radioContainer, &QRadioButton::toggled, this, &InstallMediaPage::onInstallTypeChanged);

    connect(m_isoBrowseBtn, &QPushButton::clicked, this, &InstallMediaPage::onBrowseISO);
    connect(m_importBrowseBtn, &QPushButton::clicked, this, &InstallMediaPage::onBrowseImport);

    // Register fields
    registerField("isoPath", m_isoPathEdit);
    registerField("installURL", m_urlEdit);
    registerField("importPath", m_importPathEdit);

    // Initial UI update
    onInstallTypeChanged();
}

void InstallMediaPage::onInstallTypeChanged()
{
    if (m_radioISO->isChecked()) {
        m_installType = InstallType::LocalISO;
    } else if (m_radioURL->isChecked()) {
        m_installType = InstallType::URL;
    } else if (m_radioImport->isChecked()) {
        m_installType = InstallType::Import;
    } else if (m_radioPXE->isChecked()) {
        m_installType = InstallType::PXE;
    } else {
        m_installType = InstallType::Container;
    }

    updateUIForType();
}

void InstallMediaPage::updateUIForType()
{
    // Hide all widgets first
    m_isoPathEdit->setEnabled(false);
    m_isoBrowseBtn->setEnabled(false);
    m_urlEdit->setEnabled(false);
    m_importPathEdit->setEnabled(false);
    m_importBrowseBtn->setEnabled(false);
    m_isoPathEdit->hide();
    m_isoBrowseBtn->hide();
    m_urlEdit->hide();
    m_importPathEdit->hide();
    m_importBrowseBtn->hide();

    // Show based on type
    switch (m_installType) {
    case InstallType::LocalISO:
        m_isoPathEdit->show();
        m_isoBrowseBtn->show();
        m_isoPathEdit->setEnabled(true);
        m_isoBrowseBtn->setEnabled(true);
        break;
    case InstallType::URL:
        m_urlEdit->show();
        m_urlEdit->setEnabled(true);
        break;
    case InstallType::Import:
        m_importPathEdit->show();
        m_importBrowseBtn->show();
        m_importPathEdit->setEnabled(true);
        m_importBrowseBtn->setEnabled(true);
        break;
    case InstallType::PXE:
    case InstallType::Container:
        // No additional settings needed
        break;
    }
}

void InstallMediaPage::onBrowseISO()
{
    QString path = QFileDialog::getOpenFileName(this, "Select ISO Image",
        QDir::homePath(), "ISO Images (*.iso);;All Files (*)");
    if (!path.isEmpty()) {
        m_isoPathEdit->setText(path);
    }
}

void InstallMediaPage::onBrowseImport()
{
    QString path = QFileDialog::getOpenFileName(this, "Select Disk Image",
        QDir::homePath(), "Disk Images (*.qcow2 *.img *.vmdk);;All Files (*)");
    if (!path.isEmpty()) {
        m_importPathEdit->setText(path);
    }
}

QString InstallMediaPage::getISOPath() const
{
    return m_isoPathEdit->text();
}

QString InstallMediaPage::getURL() const
{
    return m_urlEdit->text();
}

QString InstallMediaPage::getImportPath() const
{
    return m_importPathEdit->text();
}

void InstallMediaPage::initializePage()
{
    // Can suggest ISO paths from recent installs here
}

bool InstallMediaPage::validatePage()
{
    if (m_installType == InstallType::LocalISO) {
        if (m_isoPathEdit->text().isEmpty()) {
            QMessageBox::warning(this, "No ISO Selected",
                "Please select an ISO image to install from.");
            return false;
        }
        if (!QFile::exists(m_isoPathEdit->text())) {
            QMessageBox::warning(this, "ISO Not Found",
                "The specified ISO file does not exist.");
            return false;
        }
    } else if (m_installType == InstallType::URL) {
        if (m_urlEdit->text().isEmpty()) {
            QMessageBox::warning(this, "No URL",
                "Please enter a URL for the installation media.");
            return false;
        }
        QUrl url(m_urlEdit->text());
        if (!url.isValid()) {
            QMessageBox::warning(this, "Invalid URL",
                "The entered URL is not valid.");
            return false;
        }
    } else if (m_installType == InstallType::Import) {
        if (m_importPathEdit->text().isEmpty()) {
            QMessageBox::warning(this, "No Disk Image",
                "Please select a disk image to import.");
            return false;
        }
        if (!QFile::exists(m_importPathEdit->text())) {
            QMessageBox::warning(this, "Image Not Found",
                "The specified disk image does not exist.");
            return false;
        }
    }

    return true;
}

//=============================================================================
// CPUMemoryPage
//=============================================================================

CPUMemoryPage::CPUMemoryPage(CreateVMWizard *wizard, Connection *conn)
    : QWizardPage(wizard)
    , m_wizard(wizard)
    , m_connection(conn)
{
    setTitle("Step 3: CPUs and Memory");
    setSubTitle("Choose the number of CPUs and amount of memory for the virtual machine.");

    setupUI();
}

void CPUMemoryPage::setupUI()
{
    auto *layout = new QGridLayout(this);

    int row = 0;

    // Memory
    auto *memGroup = new QGroupBox("Memory");
    auto *memLayout = new QGridLayout(memGroup);

    memLayout->addWidget(new QLabel("Memory (MB):"), 0, 0);

    m_memorySpin = new QSpinBox();
    m_memorySpin->setRange(128, 68719476736);
    m_memorySpin->setValue(1024);
    m_memorySpin->setSingleStep(256);
    memLayout->addWidget(m_memorySpin, 0, 1);

    m_maxMemorySpin = new QSpinBox();
    m_maxMemorySpin->setRange(128, 68719476736);
    m_maxMemorySpin->setValue(4096);
    m_maxMemorySpin->setSingleStep(256);
    memLayout->addWidget(new QLabel("Max Memory (MB):"), 1, 0);
    memLayout->addWidget(m_maxMemorySpin, 1, 1);

    layout->addWidget(memGroup, row, 0, 1, 2);
    row++;

    // CPUs
    auto *cpuGroup = new QGroupBox("CPUs");
    auto *cpuLayout = new QGridLayout(cpuGroup);

    cpuLayout->addWidget(new QLabel("vCPUs:"), 0, 0);

    m_vcpuSpin = new QSpinBox();
    m_vcpuSpin->setRange(1, 1024);
    m_vcpuSpin->setValue(1);
    cpuLayout->addWidget(m_vcpuSpin, 0, 1);

    m_maxVcpuSpin = new QSpinBox();
    m_maxVcpuSpin->setRange(1, 1024);
    m_maxVcpuSpin->setValue(16);
    cpuLayout->addWidget(new QLabel("Max vCPUs:"), 1, 0);
    cpuLayout->addWidget(m_maxVcpuSpin, 1, 1);

    layout->addWidget(cpuGroup, row, 0, 1, 2);
    row++;

    layout->setRowStretch(row, 1);

    // Register fields
    registerField("memory", m_memorySpin);
    registerField("vcpus", m_vcpuSpin);
}

void CPUMemoryPage::initializePage()
{
    // Set defaults based on selected OS
    QString osType = field("osType").toString();

    if (osType == "Windows") {
        m_memorySpin->setValue(2048);
        m_vcpuSpin->setValue(2);
    } else if (osType == "Linux") {
        m_memorySpin->setValue(1024);
        m_vcpuSpin->setValue(1);
    }
}

bool CPUMemoryPage::validatePage()
{
    if (m_memorySpin->value() > m_maxMemorySpin->value()) {
        QMessageBox::warning(this, "Invalid Memory",
            "Memory cannot exceed maximum memory.");
        return false;
    }

    if (m_vcpuSpin->value() > m_maxVcpuSpin->value()) {
        QMessageBox::warning(this, "Invalid CPUs",
            "vCPUs cannot exceed maximum vCPUs.");
        return false;
    }

    return true;
}

//=============================================================================
// StoragePage
//=============================================================================

StoragePage::StoragePage(CreateVMWizard *wizard, Connection *conn)
    : QWizardPage(wizard)
    , m_wizard(wizard)
    , m_connection(conn)
{
    setTitle("Step 4: Enable Storage");
    setSubTitle("Select storage for your virtual machine.");

    setupUI();
}

void StoragePage::setupUI()
{
    auto *layout = new QVBoxLayout(this);

    // Storage type group
    auto *typeGroup = new QGroupBox("Storage Type");
    auto *typeLayout = new QVBoxLayout(typeGroup);

    m_radioNewDisk = new QRadioButton("Create new disk image");
    m_radioExisting = new QRadioButton("Use existing disk image");
    m_radioNoStorage = new QRadioButton("No storage (live only)");

    m_radioNewDisk->setChecked(true); // Default

    typeLayout->addWidget(m_radioNewDisk);
    typeLayout->addWidget(m_radioExisting);
    typeLayout->addWidget(m_radioNoStorage);

    layout->addWidget(typeGroup);

    // Storage settings
    auto *settingsGroup = new QGroupBox("Storage Settings");
    auto *settingsLayout = new QGridLayout(settingsGroup);

    int row = 0;

    // Disk path
    m_diskPathEdit = new QLineEdit();
    m_diskPathEdit->setPlaceholderText("/var/lib/libvirt/images/vm-name.qcow2");
    m_browseBtn = new QPushButton("Browse...");

    settingsLayout->addWidget(new QLabel("Path:"), row, 0);
    settingsLayout->addWidget(m_diskPathEdit, row, 1);
    settingsLayout->addWidget(m_browseBtn, row, 2);
    row++;

    // Disk size
    m_diskSizeSpin = new QSpinBox();
    m_diskSizeSpin->setRange(1, 1000000);
    m_diskSizeSpin->setValue(20);
    m_diskSizeSpin->setSuffix(" GB");

    settingsLayout->addWidget(new QLabel("Size:"), row, 0);
    settingsLayout->addWidget(m_diskSizeSpin, row, 1, 1, 2);
    row++;

    // Disk format
    m_diskFormatCombo = new QComboBox();
    m_diskFormatCombo->addItems({"qcow2", "raw", "vmdk", "vdi"});

    settingsLayout->addWidget(new QLabel("Format:"), row, 0);
    settingsLayout->addWidget(m_diskFormatCombo, row, 1, 1, 2);
    row++;

    // Disk bus
    m_diskBusCombo = new QComboBox();
    m_diskBusCombo->addItems({"Virtio", "SATA", "SCSI", "IDE"});

    settingsLayout->addWidget(new QLabel("Bus:"), row, 0);
    settingsLayout->addWidget(m_diskBusCombo, row, 1, 1, 2);

    settingsLayout->setColumnStretch(1, 1);

    layout->addWidget(settingsGroup);
    layout->addStretch();

    // Connect signals
    connect(m_radioNewDisk, &QRadioButton::toggled, this, &StoragePage::onStorageTypeChanged);
    connect(m_radioExisting, &QRadioButton::toggled, this, &StoragePage::onStorageTypeChanged);
    connect(m_radioNoStorage, &QRadioButton::toggled, this, &StoragePage::onStorageTypeChanged);

    connect(m_browseBtn, &QPushButton::clicked, this, &StoragePage::onBrowseDisk);

    // Register fields
    registerField("diskPath", m_diskPathEdit);
    registerField("diskSizeGB", m_diskSizeSpin);

    // Initial UI update
    onStorageTypeChanged();
}

void StoragePage::onStorageTypeChanged()
{
    if (m_radioNewDisk->isChecked()) {
        m_storageType = StorageType::NewDisk;
    } else if (m_radioExisting->isChecked()) {
        m_storageType = StorageType::ExistingDisk;
    } else {
        m_storageType = StorageType::NoStorage;
    }

    updateUIForType();
}

void StoragePage::updateUIForType()
{
    m_diskPathEdit->setEnabled(m_storageType != StorageType::NoStorage);
    m_browseBtn->setEnabled(m_storageType != StorageType::NoStorage);
    m_diskSizeSpin->setEnabled(m_storageType == StorageType::NewDisk);
    m_diskFormatCombo->setEnabled(m_storageType == StorageType::NewDisk);
}

void StoragePage::onBrowseDisk()
{
    QString path;
    if (m_storageType == StorageType::NewDisk) {
        path = QFileDialog::getSaveFileName(this, "Select Disk Location",
            "/var/lib/libvirt/images", "QCOW2 Images (*.qcow2);;All Files (*)");
    } else {
        path = QFileDialog::getOpenFileName(this, "Select Existing Disk",
            QDir::homePath(), "Disk Images (*.qcow2 *.img *.vmdk);;All Files (*)");
    }

    if (!path.isEmpty()) {
        m_diskPathEdit->setText(path);
    }
}

void StoragePage::onCreateVolume()
{
    // This would open a dialog to create a new volume in a storage pool
    QMessageBox::information(this, "Not Implemented",
        "Storage pool browser will be implemented in Phase 7.");
}

QString StoragePage::diskPath() const
{
    return m_diskPathEdit->text();
}

qint64 StoragePage::diskSizeGB() const
{
    return m_diskSizeSpin->value();
}

void StoragePage::initializePage()
{
    // Generate default disk path based on VM name
    if (m_diskPathEdit->text().isEmpty()) {
        QString name = field("vmName").toString();
        QString defaultPath = QString("/var/lib/libvirt/images/%1.qcow2").arg(name);
        m_diskPathEdit->setText(defaultPath);
    }
}

bool StoragePage::validatePage()
{
    if (m_storageType != StorageType::NoStorage) {
        if (m_diskPathEdit->text().isEmpty()) {
            QMessageBox::warning(this, "No Storage Path",
                "Please enter a path for the disk image.");
            return false;
        }

        // For new disks, ensure parent directory exists
        if (m_storageType == StorageType::NewDisk) {
            QFileInfo fileInfo(m_diskPathEdit->text());
            QDir dir = fileInfo.dir();
            if (!dir.exists()) {
                auto reply = QMessageBox::question(this, "Directory Not Found",
                    "The parent directory does not exist. Create it?",
                    QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    if (!dir.mkpath(dir.absolutePath())) {
                        QMessageBox::critical(this, "Cannot Create Directory",
                            "Failed to create the directory.");
                        return false;
                    }
                } else {
                    return false;
                }
            }
        }
    }

    return true;
}

//=============================================================================
// NetworkPage
//=============================================================================

NetworkPage::NetworkPage(CreateVMWizard *wizard, Connection *conn)
    : QWizardPage(wizard)
    , m_wizard(wizard)
    , m_connection(conn)
{
    setTitle("Step 5: Network Configuration");
    setSubTitle("Configure network connectivity for the virtual machine.");

    setupUI();
}

void NetworkPage::setupUI()
{
    auto *layout = new QGridLayout(this);

    int row = 0;

    // Network type
    layout->addWidget(new QLabel("Network Type:"), row, 0);
    m_networkTypeCombo = new QComboBox();
    m_networkTypeCombo->addItems({"Virtual Network (NAT)", "Bridged", "Direct", "Isolated", "None"});
    layout->addWidget(m_networkTypeCombo, row, 1);
    row++;

    // Network source
    layout->addWidget(new QLabel("Network Source:"), row, 0);
    m_networkSourceCombo = new QComboBox();
    m_networkSourceCombo->addItem("default");
    layout->addWidget(m_networkSourceCombo, row, 1);
    row++;

    // Device model
    layout->addWidget(new QLabel("Device Model:"), row, 0);
    m_deviceModelCombo = new QComboBox();
    m_deviceModelCombo->addItems({"virtio", "e1000", "rtl8139", "e1000e"});
    layout->addWidget(m_deviceModelCombo, row, 1);
    row++;

    layout->setRowStretch(row, 1);

    // Connect signals
    connect(m_networkTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NetworkPage::onNetworkTypeChanged);

    // Register fields
    registerField("networkType", m_networkTypeCombo, "currentText");
    registerField("networkSource", m_networkSourceCombo, "currentText");
}

void NetworkPage::onNetworkTypeChanged(int index)
{
    Q_UNUSED(index);
    updateUIForType();
}

void NetworkPage::updateUIForType()
{
    QString type = m_networkTypeCombo->currentText();
    m_networkSourceCombo->clear();

    if (type == "Virtual Network (NAT)" || type == "Isolated") {
        // Would load virtual networks from libvirt
        m_networkSourceCombo->addItem("default");
    } else if (type == "Bridged") {
        // Would load bridge devices from host
        m_networkSourceCombo->addItem("virbr0");
        m_networkSourceCombo->addItem("br0");
    } else if (type == "Direct") {
        // Would load physical interfaces
        m_networkSourceCombo->addItem("eth0");
    }

    m_networkSourceCombo->setEnabled(type != "None");
}

QString NetworkPage::networkType() const
{
    return m_networkTypeCombo->currentText();
}

QString NetworkPage::networkSource() const
{
    return m_networkSourceCombo->currentText();
}

void NetworkPage::initializePage()
{
    // Load actual networks from connection
    // For now, just use defaults
}

bool NetworkPage::validatePage()
{
    return true;
}

//=============================================================================
// SummaryPage
//=============================================================================

SummaryPage::SummaryPage(CreateVMWizard *wizard, Connection *conn)
    : QWizardPage(wizard)
    , m_wizard(wizard)
    , m_connection(conn)
{
    setTitle("Step 6: Ready to Install");
    setSubTitle("Review the configuration and click Finish to create the virtual machine.");

    setupUI();
}

void SummaryPage::setupUI()
{
    auto *layout = new QVBoxLayout(this);

    m_summaryLabel = new QLabel();
    m_summaryLabel->setWordWrap(true);
    m_summaryLabel->setTextFormat(Qt::PlainText);

    layout->addWidget(m_summaryLabel);

    m_startCheck = new QCheckBox("Start VM after installation");
    m_startCheck->setChecked(true);
    layout->addWidget(m_startCheck);

    layout->addStretch();

    // Register field
    registerField("startAfterInstall", m_startCheck);
}

void SummaryPage::initializePage()
{
    updateSummary();
}

void SummaryPage::updateSummary()
{
    QString summary;

    summary += "=== Virtual Machine Configuration ===\n\n";

    summary += "Name: " + m_wizard->vmName() + "\n";
    summary += "OS Type: " + m_wizard->osType() + "\n\n";

    summary += "=== Resources ===\n";
    summary += "Memory: " + QString::number(m_wizard->memoryMB()) + " MB\n";
    summary += "CPUs: " + QString::number(m_wizard->vcpus()) + "\n\n";

    summary += "=== Storage ===\n";
    // Get storage info from StoragePage
    auto *storagePage = static_cast<StoragePage*>(m_wizard->page(CreateVMWizard::Page_Storage));
    if (storagePage->storageType() != StoragePage::StorageType::NoStorage) {
        summary += "Path: " + storagePage->diskPath() + "\n";
        if (storagePage->storageType() == StoragePage::StorageType::NewDisk) {
            summary += "Size: " + QString::number(storagePage->diskSizeGB()) + " GB\n";
        }
    } else {
        summary += "No storage configured\n";
    }
    summary += "\n";

    summary += "=== Network ===\n";
    auto *networkPage = static_cast<NetworkPage*>(m_wizard->page(CreateVMWizard::Page_Network));
    summary += "Type: " + networkPage->networkType() + "\n";
    summary += "Source: " + networkPage->networkSource() + "\n\n";

    // Get install media info
    auto *installPage = static_cast<InstallMediaPage*>(m_wizard->page(CreateVMWizard::Page_InstallMedia));
    summary += "=== Installation Method ===\n";
    switch (installPage->getInstallType()) {
    case InstallMediaPage::InstallType::LocalISO:
        summary += "Method: Local ISO\n";
        summary += "ISO: " + installPage->getISOPath() + "\n";
        break;
    case InstallMediaPage::InstallType::URL:
        summary += "Method: Network Install\n";
        summary += "URL: " + installPage->getURL() + "\n";
        break;
    case InstallMediaPage::InstallType::Import:
        summary += "Method: Import Existing\n";
        summary += "Disk: " + installPage->getImportPath() + "\n";
        break;
    case InstallMediaPage::InstallType::PXE:
        summary += "Method: PXE Boot\n";
        break;
    case InstallMediaPage::InstallType::Container:
        summary += "Method: Container Boot\n";
        break;
    }

    m_summaryLabel->setText(summary);
}

bool SummaryPage::validatePage()
{
    // This is where we actually create the VM
    QString vmName = m_wizard->vmName();

    auto reply = QMessageBox::question(this, "Create Virtual Machine",
        "Are you sure you want to create the virtual machine '" + vmName + "'?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Create Guest object from wizard data
        auto *guest = new Guest(m_connection, this);

        // Set basic properties
        guest->setName(m_wizard->vmName());
        guest->setOSType(m_wizard->osType());
        guest->generateUUID();

        // Set memory (convert MB to KB)
        guest->setMemory(m_wizard->memoryMB() * 1024);
        guest->setCurrentMemory(m_wizard->memoryMB() * 1024);

        // Set CPUs
        guest->setVCPUs(m_wizard->vcpus());
        guest->setMaxVCPUs(m_wizard->vcpus() * 2);

        // Get storage configuration
        auto *storagePage = static_cast<StoragePage*>(m_wizard->page(CreateVMWizard::Page_Storage));
        if (storagePage->storageType() != StoragePage::StorageType::NoStorage) {
            auto *disk = new DiskDevice(guest);

            // Set disk type based on storage type
            if (storagePage->storageType() == StoragePage::StorageType::NewDisk) {
                disk->setDiskType(DiskDevice::DiskType::File);
                disk->setDevice(DiskDevice::DeviceType::Disk);
            } else {
                disk->setDiskType(DiskDevice::DiskType::File);
                disk->setDevice(DiskDevice::DeviceType::Disk);
            }

            disk->setSource(storagePage->diskPath());
            disk->setTarget("vda");
            disk->setBus(DiskDevice::BusType::Virtio);
            disk->setDriverType("qcow2");

            guest->addDisk(disk);
        }

        // Get install media configuration
        auto *installPage = static_cast<InstallMediaPage*>(m_wizard->page(CreateVMWizard::Page_InstallMedia));

        // Add installation media as CDROM if using ISO
        if (installPage->getInstallType() == InstallMediaPage::InstallType::LocalISO) {
            auto *cdrom = new DiskDevice(guest);
            cdrom->setDiskType(DiskDevice::DiskType::File);
            cdrom->setDevice(DiskDevice::DeviceType::CDROM);
            cdrom->setSource(installPage->getISOPath());
            cdrom->setTarget("hda");
            cdrom->setBus(DiskDevice::BusType::SATA);
            cdrom->setReadonly(true);

            guest->addDisk(cdrom);
        }

        // Get network configuration
        auto *networkPage = static_cast<NetworkPage*>(m_wizard->page(CreateVMWizard::Page_Network));
        if (networkPage->networkType() != "None") {
            auto *nic = new NetworkDevice(guest);

            QString netType = networkPage->networkType();
            if (netType == "Virtual Network (NAT)") {
                nic->setNetworkType(NetworkDevice::NetworkType::Network);
                nic->setSource(networkPage->networkSource());
            } else if (netType == "Bridged") {
                nic->setNetworkType(NetworkDevice::NetworkType::Bridge);
                nic->setSource(networkPage->networkSource());
            } else if (netType == "Direct") {
                nic->setNetworkType(NetworkDevice::NetworkType::Direct);
            } else if (netType == "Isolated") {
                nic->setNetworkType(NetworkDevice::NetworkType::Network);
            }

            nic->setModel(NetworkDevice::ModelType::Virtio);
            guest->addNetworkInterface(nic);
        }

        // Validate the guest configuration
        if (!guest->validate()) {
            QMessageBox::critical(this, "Invalid Configuration",
                "The VM configuration is invalid:\n" + guest->validationError());
            guest->deleteLater();
            return false;
        }

        // Show the generated XML
        QString xml = guest->toXML();

        // For debugging, show the XML
        qDebug() << "Generated XML:\n" << xml;

        // In a real implementation, we would:
        // 1. Create storage if needed
        // 2. Define the VM using virDomainDefineXML()
        // 3. Start the VM
        // 4. Open console window

        QMessageBox::information(this, "VM Configuration",
            "VM configuration generated successfully!\n\n"
            "The following XML will be defined:\n\n" +
            QString(xml).left(1000) + "...\n\n"
            "Note: Actual VM creation requires connection to a running libvirtd.\n"
            "The wizard will complete the VM definition when connected to libvirt.");

        guest->deleteLater();
        return true; // Allow wizard to finish
    }

    return false;
}

} // namespace QVirt
