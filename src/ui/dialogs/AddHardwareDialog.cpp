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

#include "AddHardwareDialog.h"
#include "../../devices/DiskDevice.h"
#include "../../devices/NetworkDevice.h"
#include "../../devices/ControllerDevice.h"
#include "../../devices/InputDevice.h"
#include "../../devices/GraphicsDevice.h"
#include "../../devices/VideoDevice.h"
#include "../../devices/SoundDevice.h"
#include "../../devices/TPMDevice.h"
#include "../../devices/HostDevice.h"
#include "../../devices/FileSystemDevice.h"
#include "../../devices/WatchdogDevice.h"
#include "../../devices/RNGDevice.h"
#include "../../devices/SmartcardDevice.h"
#include "../../devices/MemballoonDevice.h"
#include "../../core/Error.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFormLayout>

namespace QVirt {

//=============================================================================
// AddHardwareDialog
//=============================================================================

AddHardwareDialog::AddHardwareDialog(Domain *domain, QWidget *parent)
    : QDialog(parent)
    , m_domain(domain)
{
    setWindowTitle("Add Hardware");
    setMinimumSize(600, 400);

    setupUI();
    setupDeviceList();
    setupDevicePages();
}

void AddHardwareDialog::setupUI()
{
    auto *layout = new QHBoxLayout(this);

    // Left side - device list
    m_deviceList = new QListWidget();
    m_deviceList->setMaximumWidth(200);
    layout->addWidget(m_deviceList);

    // Right side - configuration pages
    m_pageStack = new QStackedWidget();
    layout->addWidget(m_pageStack, 1);

    // Connect selection change
    connect(m_deviceList, &QListWidget::currentItemChanged,
            this, &AddHardwareDialog::onDeviceTypeChanged);

    // Button box
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &AddHardwareDialog::onAccepted);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layout);
    mainLayout->addWidget(m_buttonBox);
}

void AddHardwareDialog::setupDeviceList()
{
    // Add device types to list
    QListWidgetItem *item;

    item = new QListWidgetItem("Storage Devices");
    item->setData(Qt::UserRole, static_cast<int>(DeviceType::Storage));
    m_deviceList->addItem(item);

    item = new QListWidgetItem("Network Devices");
    item->setData(Qt::UserRole, static_cast<int>(DeviceType::Network));
    m_deviceList->addItem(item);

    item = new QListWidgetItem("Controllers");
    item->setData(Qt::UserRole, static_cast<int>(DeviceType::Controller));
    m_deviceList->addItem(item);

    item = new QListWidgetItem("Input Devices");
    item->setData(Qt::UserRole, static_cast<int>(DeviceType::Input));
    m_deviceList->addItem(item);

    item = new QListWidgetItem("Graphics");
    item->setData(Qt::UserRole, static_cast<int>(DeviceType::Graphics));
    m_deviceList->addItem(item);

    item = new QListWidgetItem("Video");
    item->setData(Qt::UserRole, static_cast<int>(DeviceType::Video));
    m_deviceList->addItem(item);

    item = new QListWidgetItem("Sound");
    item->setData(Qt::UserRole, static_cast<int>(DeviceType::Sound));
    m_deviceList->addItem(item);

    item = new QListWidgetItem("Host Devices");
    item->setData(Qt::UserRole, static_cast<int>(DeviceType::HostDevice));
    m_deviceList->addItem(item);

    item = new QListWidgetItem("Filesystem");
    item->setData(Qt::UserRole, static_cast<int>(DeviceType::Filesystem));
    m_deviceList->addItem(item);

    item = new QListWidgetItem("TPM");
    item->setData(Qt::UserRole, static_cast<int>(DeviceType::TPM));
    m_deviceList->addItem(item);

    item = new QListWidgetItem("RNG");
    item->setData(Qt::UserRole, static_cast<int>(DeviceType::RNG));
    m_deviceList->addItem(item);

    item = new QListWidgetItem("Watchdog");
    item->setData(Qt::UserRole, static_cast<int>(DeviceType::Watchdog));
    m_deviceList->addItem(item);

    item = new QListWidgetItem("Smartcard");
    item->setData(Qt::UserRole, static_cast<int>(DeviceType::Smartcard));
    m_deviceList->addItem(item);

    item = new QListWidgetItem("Memory Balloon");
    item->setData(Qt::UserRole, static_cast<int>(DeviceType::Memballoon));
    m_deviceList->addItem(item);

    m_deviceList->setCurrentRow(0);
}

void AddHardwareDialog::setupDevicePages()
{
    // Create configuration pages for each device type
    auto *storagePage = new StorageDevicePage(this);
    auto *networkPage = new NetworkDevicePage(this);
    auto *controllerPage = new ControllerDevicePage(this);
    auto *inputPage = new InputDevicePage(this);
    auto *graphicsPage = new GraphicsDevicePage(this);
    auto *videoPage = new VideoDevicePage(this);
    auto *soundPage = new SoundDevicePage(this);
    auto *tpmPage = new TPMDevicePage(this);
    auto *hostDevicePage = new HostDevicePage(this);
    auto *filesystemPage = new FileSystemDevicePage(this);
    auto *watchdogPage = new WatchdogDevicePage(this);
    auto *rngPage = new RNGDevicePage(this);
    auto *smartcardPage = new SmartcardDevicePage(this);
    auto *memballoonPage = new MemballoonDevicePage(this);

    m_pageStack->addWidget(storagePage);
    m_pageStack->addWidget(networkPage);
    m_pageStack->addWidget(controllerPage);
    m_pageStack->addWidget(inputPage);
    m_pageStack->addWidget(graphicsPage);
    m_pageStack->addWidget(videoPage);
    m_pageStack->addWidget(soundPage);
    m_pageStack->addWidget(tpmPage);
    m_pageStack->addWidget(hostDevicePage);
    m_pageStack->addWidget(filesystemPage);
    m_pageStack->addWidget(rngPage);
    m_pageStack->addWidget(watchdogPage);
    m_pageStack->addWidget(smartcardPage);
    m_pageStack->addWidget(memballoonPage);
}

void AddHardwareDialog::onDeviceTypeChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);

    if (!current) {
        return;
    }

    int deviceTypeInt = current->data(Qt::UserRole).toInt();
    DeviceType deviceType = static_cast<DeviceType>(deviceTypeInt);

    // Show appropriate page
    switch (deviceType) {
    case DeviceType::Storage:
        m_pageStack->setCurrentIndex(0);
        break;
    case DeviceType::Network:
        m_pageStack->setCurrentIndex(1);
        break;
    case DeviceType::Controller:
        m_pageStack->setCurrentIndex(2);
        break;
    case DeviceType::Input:
        m_pageStack->setCurrentIndex(3);
        break;
    case DeviceType::Graphics:
        m_pageStack->setCurrentIndex(4);
        break;
    case DeviceType::Video:
        m_pageStack->setCurrentIndex(5);
        break;
    case DeviceType::Sound:
        m_pageStack->setCurrentIndex(6);
        break;
    case DeviceType::TPM:
        m_pageStack->setCurrentIndex(7);
        break;
    case DeviceType::HostDevice:
        m_pageStack->setCurrentIndex(8);
        break;
    case DeviceType::Filesystem:
        m_pageStack->setCurrentIndex(9);
        break;
    case DeviceType::RNG:
        m_pageStack->setCurrentIndex(10);
        break;
    case DeviceType::Watchdog:
        m_pageStack->setCurrentIndex(11);
        break;
    case DeviceType::Smartcard:
        m_pageStack->setCurrentIndex(12);
        break;
    case DeviceType::Memballoon:
        m_pageStack->setCurrentIndex(13);
        break;
    default:
        break;
    }
}

void AddHardwareDialog::onAccepted()
{
    // Get current page and create device
    DeviceConfigPage *page = static_cast<DeviceConfigPage*>(m_pageStack->currentWidget());

    if (!page->validate()) {
        return;
    }

    m_createdDevice = page->createDevice();

    if (!m_createdDevice) {
        QMessageBox::warning(this, "Error", "Failed to create device");
        return;
    }

    accept();
}

//=============================================================================
// StorageDevicePage
//=============================================================================

StorageDevicePage::StorageDevicePage(QWidget *parent)
    : DeviceConfigPage(parent)
{
    setupUI();
}

void StorageDevicePage::setupUI()
{
    auto *layout = new QFormLayout(this);

    // Device type
    m_deviceTypeCombo = new QComboBox();
    m_deviceTypeCombo->addItem("Disk", static_cast<int>(DiskDevice::DeviceType::Disk));
    m_deviceTypeCombo->addItem("CDROM", static_cast<int>(DiskDevice::DeviceType::CDROM));
    m_deviceTypeCombo->addItem("Floppy", static_cast<int>(DiskDevice::DeviceType::Floppy));
    layout->addRow("Device Type:", m_deviceTypeCombo);

    // Bus type
    m_busTypeCombo = new QComboBox();
    m_busTypeCombo->addItem("Virtio", static_cast<int>(DiskDevice::BusType::Virtio));
    m_busTypeCombo->addItem("IDE", static_cast<int>(DiskDevice::BusType::IDE));
    m_busTypeCombo->addItem("SCSI", static_cast<int>(DiskDevice::BusType::SCSI));
    m_busTypeCombo->addItem("SATA", static_cast<int>(DiskDevice::BusType::SATA));
    m_busTypeCombo->addItem("USB", static_cast<int>(DiskDevice::BusType::USB));
    layout->addRow("Bus Type:", m_busTypeCombo);

    // Path
    m_pathEdit = new QLineEdit();
    m_pathEdit->setPlaceholderText("/var/lib/libvirt/images/disk.qcow2");
    layout->addRow("Path:", m_pathEdit);

    // Size
    m_sizeSpin = new QSpinBox();
    m_sizeSpin->setRange(1, 1000000);
    m_sizeSpin->setValue(20);
    m_sizeSpin->setSuffix(" GB");
    layout->addRow("Size:", m_sizeSpin);

    // Format
    m_formatCombo = new QComboBox();
    m_formatCombo->addItem("qcow2");
    m_formatCombo->addItem("raw");
    m_formatCombo->addItem("vmdk");
    m_formatCombo->addItem("vdi");
    layout->addRow("Format:", m_formatCombo);

    // Readonly
    m_readonlyCheck = new QCheckBox("Read-only");
    layout->addRow("", m_readonlyCheck);
}

Device* StorageDevicePage::createDevice()
{
    auto *disk = new DiskDevice();

    disk->setDevice(static_cast<DiskDevice::DeviceType>(m_deviceTypeCombo->currentData().toInt()));
    disk->setBus(static_cast<DiskDevice::BusType>(m_busTypeCombo->currentData().toInt()));
    disk->setSource(m_pathEdit->text());
    disk->setDriverType(m_formatCombo->currentText());
    disk->setReadonly(m_readonlyCheck->isChecked());
    disk->setTarget("vda"); // Default target, will be assigned properly later

    return disk;
}

bool StorageDevicePage::validate()
{
    if (m_pathEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a disk path");
        return false;
    }

    return true;
}

//=============================================================================
// NetworkDevicePage
//=============================================================================

NetworkDevicePage::NetworkDevicePage(QWidget *parent)
    : DeviceConfigPage(parent)
{
    setupUI();
}

void NetworkDevicePage::setupUI()
{
    auto *layout = new QFormLayout(this);

    // Network type
    m_networkTypeCombo = new QComboBox();
    m_networkTypeCombo->addItem("Virtual Network (NAT)", "nat");
    m_networkTypeCombo->addItem("Bridged", "bridge");
    m_networkTypeCombo->addItem("Direct", "direct");
    m_networkTypeCombo->addItem("Isolated", "isolated");
    layout->addRow("Network Type:", m_networkTypeCombo);

    // Source
    m_sourceCombo = new QComboBox();
    m_sourceCombo->addItem("default");
    m_sourceCombo->addItem("virbr0");
    layout->addRow("Source:", m_sourceCombo);

    // Model
    m_modelCombo = new QComboBox();
    m_modelCombo->addItem("virtio", static_cast<int>(NetworkDevice::ModelType::Virtio));
    m_modelCombo->addItem("e1000", static_cast<int>(NetworkDevice::ModelType::E1000));
    m_modelCombo->addItem("rtl8139", static_cast<int>(NetworkDevice::ModelType::RTL8139));
    m_modelCombo->addItem("vmxnet3", static_cast<int>(NetworkDevice::ModelType::VMXNET3));
    layout->addRow("Model:", m_modelCombo);

    // MAC address
    m_macEdit = new QLineEdit();
    m_macEdit->setPlaceholderText("Auto-generated");
    layout->addRow("MAC Address:", m_macEdit);
}

Device* NetworkDevicePage::createDevice()
{
    auto *nic = new NetworkDevice();

    QString type = m_networkTypeCombo->currentData().toString();
    if (type == "nat" || type == "isolated") {
        nic->setNetworkType(NetworkDevice::NetworkType::Network);
    } else if (type == "bridge") {
        nic->setNetworkType(NetworkDevice::NetworkType::Bridge);
    } else {
        nic->setNetworkType(NetworkDevice::NetworkType::Direct);
    }

    nic->setSource(m_sourceCombo->currentText());
    nic->setModel(static_cast<NetworkDevice::ModelType>(m_modelCombo->currentData().toInt()));

    QString mac = m_macEdit->text().trimmed();
    if (!mac.isEmpty()) {
        nic->setMacAddress(mac);
    }

    return nic;
}

bool NetworkDevicePage::validate()
{
    return true;
}

//=============================================================================
// ControllerDevicePage
//=============================================================================

ControllerDevicePage::ControllerDevicePage(QWidget *parent)
    : DeviceConfigPage(parent)
{
    setupUI();
}

void ControllerDevicePage::setupUI()
{
    auto *layout = new QFormLayout(this);

    // Controller type
    m_controllerTypeCombo = new QComboBox();
    m_controllerTypeCombo->addItem("USB", static_cast<int>(ControllerDevice::ControllerType::USB));
    m_controllerTypeCombo->addItem("PCI", static_cast<int>(ControllerDevice::ControllerType::PCI));
    m_controllerTypeCombo->addItem("SCSI", static_cast<int>(ControllerDevice::ControllerType::SCSI));
    m_controllerTypeCombo->addItem("SATA", static_cast<int>(ControllerDevice::ControllerType::SATA));
    m_controllerTypeCombo->addItem("IDE", static_cast<int>(ControllerDevice::ControllerType::IDE));
    layout->addRow("Controller Type:", m_controllerTypeCombo);

    // USB model (only for USB controllers)
    m_modelCombo = new QComboBox();
    m_modelCombo->addItem("qemu-xhci", static_cast<int>(ControllerDevice::USBModel::QemuXhci));
    m_modelCombo->addItem("ich9-ehci1", static_cast<int>(ControllerDevice::USBModel::Ich9Ehci1));
    m_modelCombo->addItem("ehci", static_cast<int>(ControllerDevice::USBModel::Ehci));
    layout->addRow("USB Model:", m_modelCombo);

    // Index
    m_indexSpin = new QSpinBox();
    m_indexSpin->setRange(0, 31);
    m_indexSpin->setValue(0);
    layout->addRow("Index:", m_indexSpin);
}

Device* ControllerDevicePage::createDevice()
{
    auto *controller = new ControllerDevice();

    controller->setControllerType(static_cast<ControllerDevice::ControllerType>(
        m_controllerTypeCombo->currentData().toInt()));
    controller->setUSBModel(static_cast<ControllerDevice::USBModel>(m_modelCombo->currentData().toInt()));
    controller->setIndex(m_indexSpin->value());

    return controller;
}

bool ControllerDevicePage::validate()
{
    return true;
}

//=============================================================================
// InputDevicePage
//=============================================================================

InputDevicePage::InputDevicePage(QWidget *parent)
    : DeviceConfigPage(parent)
{
    setupUI();
}

void InputDevicePage::setupUI()
{
    auto *layout = new QFormLayout(this);

    // Input type
    m_inputTypeCombo = new QComboBox();
    m_inputTypeCombo->addItem("Tablet", static_cast<int>(InputDevice::InputType::Tablet));
    m_inputTypeCombo->addItem("Keyboard", static_cast<int>(InputDevice::InputType::Keyboard));
    m_inputTypeCombo->addItem("Mouse", static_cast<int>(InputDevice::InputType::Mouse));
    layout->addRow("Input Type:", m_inputTypeCombo);

    // Bus
    m_busCombo = new QComboBox();
    m_busCombo->addItem("USB", static_cast<int>(InputDevice::BusType::USB));
    m_busCombo->addItem("Virtio", static_cast<int>(InputDevice::BusType::Virtio));
    m_busCombo->addItem("PS2", static_cast<int>(InputDevice::BusType::PS2));
    layout->addRow("Bus:", m_busCombo);
}

Device* InputDevicePage::createDevice()
{
    auto *input = new InputDevice();

    input->setInputType(static_cast<InputDevice::InputType>(m_inputTypeCombo->currentData().toInt()));
    input->setBus(static_cast<InputDevice::BusType>(m_busCombo->currentData().toInt()));

    return input;
}

bool InputDevicePage::validate()
{
    return true;
}

//=============================================================================
// GraphicsDevicePage
//=============================================================================

GraphicsDevicePage::GraphicsDevicePage(QWidget *parent)
    : DeviceConfigPage(parent)
{
    setupUI();
}

void GraphicsDevicePage::setupUI()
{
    auto *layout = new QFormLayout(this);

    // Graphics type
    m_graphicsTypeCombo = new QComboBox();
    m_graphicsTypeCombo->addItem("SPICE", static_cast<int>(GraphicsDevice::GraphicsType::SPICE));
    m_graphicsTypeCombo->addItem("VNC", static_cast<int>(GraphicsDevice::GraphicsType::VNC));
    layout->addRow("Type:", m_graphicsTypeCombo);

    // Auto port
    m_autoPortCheck = new QCheckBox("Auto-allocate port");
    m_autoPortCheck->setChecked(true);
    layout->addRow("", m_autoPortCheck);

    // Port
    m_portSpin = new QSpinBox();
    m_portSpin->setRange(-1, 65535);
    m_portSpin->setValue(-1);
    m_portSpin->setEnabled(false);
    layout->addRow("Port:", m_portSpin);

    // Listen address
    m_listenAddressEdit = new QLineEdit("0.0.0.0");
    layout->addRow("Listen Address:", m_listenAddressEdit);

    // Keymap
    m_keymapCombo = new QComboBox();
    m_keymapCombo->addItem("en-us");
    m_keymapCombo->addItem("en-gb");
    m_keymapCombo->addItem("de");
    m_keymapCombo->addItem("fr");
    layout->addRow("Keymap:", m_keymapCombo);

    connect(m_autoPortCheck, &QCheckBox::toggled, [this](bool checked) {
        m_portSpin->setEnabled(!checked);
    });
}

Device* GraphicsDevicePage::createDevice()
{
    auto *graphics = new GraphicsDevice();

    graphics->setGraphicsType(static_cast<GraphicsDevice::GraphicsType>(
        m_graphicsTypeCombo->currentData().toInt()));
    graphics->setAutoPort(m_autoPortCheck->isChecked());
    graphics->setPort(m_portSpin->value());
    graphics->setListenAddress(m_listenAddressEdit->text());
    graphics->setKeyMap(m_keymapCombo->currentText());

    return graphics;
}

bool GraphicsDevicePage::validate()
{
    return true;
}

//=============================================================================
// VideoDevicePage
//=============================================================================

VideoDevicePage::VideoDevicePage(QWidget *parent)
    : DeviceConfigPage(parent)
{
    setupUI();
}

void VideoDevicePage::setupUI()
{
    auto *layout = new QFormLayout(this);

    // Model
    m_modelCombo = new QComboBox();
    m_modelCombo->addItem("QXL", static_cast<int>(VideoDevice::VideoModel::QXL));
    m_modelCombo->addItem("Virtio", static_cast<int>(VideoDevice::VideoModel::Virtio));
    m_modelCombo->addItem("Cirrus", static_cast<int>(VideoDevice::VideoModel::Cirrus));
    m_modelCombo->addItem("VGA", static_cast<int>(VideoDevice::VideoModel::VGA));
    layout->addRow("Model:", m_modelCombo);

    // VRAM
    m_vramSpin = new QSpinBox();
    m_vramSpin->setRange(1024, 262144); // 1 MB to 256 MB
    m_vramSpin->setValue(16384); // 16 MB
    m_vramSpin->setSuffix(" KB");
    layout->addRow("VRAM:", m_vramSpin);

    // Heads
    m_headsSpin = new QSpinBox();
    m_headsSpin->setRange(1, 4);
    m_headsSpin->setValue(1);
    layout->addRow("Heads:", m_headsSpin);

    // 3D Acceleration
    m_accel3DCheck = new QCheckBox("Enable 3D acceleration");
    layout->addRow("", m_accel3DCheck);
}

Device* VideoDevicePage::createDevice()
{
    auto *video = new VideoDevice();

    video->setModel(static_cast<VideoDevice::VideoModel>(m_modelCombo->currentData().toInt()));
    video->setVram(m_vramSpin->value());
    video->setHeads(m_headsSpin->value());
    video->setAccel3D(m_accel3DCheck->isChecked());

    return video;
}

bool VideoDevicePage::validate()
{
    return true;
}

//=============================================================================
// SoundDevicePage
//=============================================================================

SoundDevicePage::SoundDevicePage(QWidget *parent)
    : DeviceConfigPage(parent)
{
    setupUI();
}

void SoundDevicePage::setupUI()
{
    auto *layout = new QFormLayout(this);

    // Model
    m_modelCombo = new QComboBox();
    m_modelCombo->addItem("ICH6", static_cast<int>(SoundDevice::SoundModel::ICH6));
    m_modelCombo->addItem("ICH9", static_cast<int>(SoundDevice::SoundModel::ICH9));
    m_modelCombo->addItem("AC97", static_cast<int>(SoundDevice::SoundModel::AC97));
    m_modelCombo->addItem("ES1370", static_cast<int>(SoundDevice::SoundModel::ES1370));
    layout->addRow("Model:", m_modelCombo);
}

Device* SoundDevicePage::createDevice()
{
    auto *sound = new SoundDevice();

    sound->setModel(static_cast<SoundDevice::SoundModel>(m_modelCombo->currentData().toInt()));

    return sound;
}

bool SoundDevicePage::validate()
{
    return true;
}

//=============================================================================
// TPMDevicePage
//=============================================================================

TPMDevicePage::TPMDevicePage(QWidget *parent)
    : DeviceConfigPage(parent)
{
    setupUI();
}

void TPMDevicePage::setupUI()
{
    auto *layout = new QFormLayout(this);

    // Model
    m_modelCombo = new QComboBox();
    m_modelCombo->addItem("TIS", static_cast<int>(TPMDevice::TPMModel::TIS));
    m_modelCombo->addItem("CRB", static_cast<int>(TPMDevice::TPMModel::CRB));
    m_modelCombo->addItem("SPAPR", static_cast<int>(TPMDevice::TPMModel::SPAPR));
    layout->addRow("Model:", m_modelCombo);

    // Backend
    m_backendCombo = new QComboBox();
    m_backendCombo->addItem("Emulator", static_cast<int>(TPMDevice::TPMBackend::Emulator));
    m_backendCombo->addItem("Passthrough", static_cast<int>(TPMDevice::TPMBackend::Passthrough));
    layout->addRow("Backend:", m_backendCombo);

    // Version
    m_versionCombo = new QComboBox();
    m_versionCombo->addItem("1.2", static_cast<int>(TPMDevice::TPMVersion::V1_2));
    m_versionCombo->addItem("2.0", static_cast<int>(TPMDevice::TPMVersion::V2_0));
    layout->addRow("Version:", m_versionCombo);

    // Device path (for passthrough)
    m_devicePathEdit = new QLineEdit();
    layout->addRow("Device Path:", m_devicePathEdit);
}

Device* TPMDevicePage::createDevice()
{
    auto *tpm = new TPMDevice();

    tpm->setModel(static_cast<TPMDevice::TPMModel>(m_modelCombo->currentData().toInt()));
    tpm->setBackend(static_cast<TPMDevice::TPMBackend>(m_backendCombo->currentData().toInt()));
    tpm->setVersion(static_cast<TPMDevice::TPMVersion>(m_versionCombo->currentData().toInt()));
    tpm->setDevicePath(m_devicePathEdit->text());

    return tpm;
}

bool TPMDevicePage::validate()
{
    auto backend = static_cast<TPMDevice::TPMBackend>(m_backendCombo->currentData().toInt());
    if (backend == TPMDevice::TPMBackend::Passthrough && m_devicePathEdit->text().isEmpty()) {
        return false;
    }
    return true;
}

//=============================================================================
// HostDevicePage
//=============================================================================

HostDevicePage::HostDevicePage(QWidget *parent)
    : DeviceConfigPage(parent)
{
    setupUI();
}

void HostDevicePage::setupUI()
{
    auto *layout = new QFormLayout(this);

    // Device type
    m_deviceTypeCombo = new QComboBox();
    m_deviceTypeCombo->addItem("USB", static_cast<int>(HostDevice::HostDeviceType::USB));
    m_deviceTypeCombo->addItem("PCI", static_cast<int>(HostDevice::HostDeviceType::PCI));
    layout->addRow("Device Type:", m_deviceTypeCombo);

    // USB mode
    m_usbModeCombo = new QComboBox();
    m_usbModeCombo->addItem("Bus", static_cast<int>(HostDevice::USBMode::Bus));
    m_usbModeCombo->addItem("Vendor/Product", static_cast<int>(HostDevice::USBMode::Device));
    layout->addRow("USB Mode:", m_usbModeCombo);

    // USB Bus
    m_usbBusSpin = new QSpinBox();
    m_usbBusSpin->setRange(0, 255);
    layout->addRow("USB Bus:", m_usbBusSpin);

    // USB Device
    m_usbDeviceSpin = new QSpinBox();
    m_usbDeviceSpin->setRange(0, 255);
    layout->addRow("USB Device:", m_usbDeviceSpin);

    // Vendor ID
    m_vendorIdEdit = new QLineEdit();
    m_vendorIdEdit->setPlaceholderText("0x1234");
    layout->addRow("Vendor ID:", m_vendorIdEdit);

    // Product ID
    m_productIdEdit = new QLineEdit();
    m_productIdEdit->setPlaceholderText("0x5678");
    layout->addRow("Product ID:", m_productIdEdit);
}

Device* HostDevicePage::createDevice()
{
    auto *host = new HostDevice();

    auto deviceType = static_cast<HostDevice::HostDeviceType>(m_deviceTypeCombo->currentData().toInt());
    host->setHostDeviceType(deviceType);

    if (deviceType == HostDevice::HostDeviceType::USB) {
        auto usbMode = static_cast<HostDevice::USBMode>(m_usbModeCombo->currentData().toInt());
        host->setUsbMode(usbMode);
        host->setUsbBus(m_usbBusSpin->value());
        host->setUsbDevice(m_usbDeviceSpin->value());
        host->setUsbVendorId(m_vendorIdEdit->text());
        host->setUsbProductId(m_productIdEdit->text());
    }

    return host;
}

bool HostDevicePage::validate()
{
    auto deviceType = static_cast<HostDevice::HostDeviceType>(m_deviceTypeCombo->currentData().toInt());
    if (deviceType == HostDevice::HostDeviceType::USB) {
        auto usbMode = static_cast<HostDevice::USBMode>(m_usbModeCombo->currentData().toInt());
        if (usbMode == HostDevice::USBMode::Device) {
            if (m_vendorIdEdit->text().isEmpty() || m_productIdEdit->text().isEmpty()) {
                return false;
            }
        }
    }
    return true;
}

//=============================================================================
// FileSystemDevicePage
//=============================================================================

FileSystemDevicePage::FileSystemDevicePage(QWidget *parent)
    : DeviceConfigPage(parent)
{
    setupUI();
}

void FileSystemDevicePage::setupUI()
{
    auto *layout = new QFormLayout(this);

    // Driver
    m_driverCombo = new QComboBox();
    m_driverCombo->addItem("virtio-fs", static_cast<int>(FileSystemDevice::DriverType::VirtioFS));
    m_driverCombo->addItem("9p", static_cast<int>(FileSystemDevice::DriverType::Virtio9P));
    layout->addRow("Driver:", m_driverCombo);

    // Source path
    m_sourcePathEdit = new QLineEdit();
    m_sourcePathEdit->setPlaceholderText("/host/path");
    layout->addRow("Source Path:", m_sourcePathEdit);

    // Target directory
    m_targetDirEdit = new QLineEdit();
    m_targetDirEdit->setPlaceholderText("/guest/path");
    layout->addRow("Target Directory:", m_targetDirEdit);

    // Mount tag
    m_mountTagEdit = new QLineEdit();
    m_mountTagEdit->setPlaceholderText("mytag");
    layout->addRow("Mount Tag:", m_mountTagEdit);

    // Access mode
    m_accessModeCombo = new QComboBox();
    m_accessModeCombo->addItem("Passthrough", static_cast<int>(FileSystemDevice::AccessMode::Passthrough));
    m_accessModeCombo->addItem("Mapped", static_cast<int>(FileSystemDevice::AccessMode::Mapped));
    m_accessModeCombo->addItem("Squash", static_cast<int>(FileSystemDevice::AccessMode::Squash));
    layout->addRow("Access Mode:", m_accessModeCombo);

    // Read-only
    m_readonlyCheck = new QCheckBox();
    layout->addRow("Read-only:", m_readonlyCheck);
}

Device* FileSystemDevicePage::createDevice()
{
    auto *fs = new FileSystemDevice();

    fs->setDriverType(static_cast<FileSystemDevice::DriverType>(m_driverCombo->currentData().toInt()));
    fs->setSourcePath(m_sourcePathEdit->text());
    fs->setTargetDir(m_targetDirEdit->text());
    fs->setMountTag(m_mountTagEdit->text());
    fs->setAccessMode(static_cast<FileSystemDevice::AccessMode>(m_accessModeCombo->currentData().toInt()));
    fs->setReadonly(m_readonlyCheck->isChecked());

    return fs;
}

bool FileSystemDevicePage::validate()
{
    if (m_sourcePathEdit->text().isEmpty()) {
        return false;
    }
    if (m_targetDirEdit->text().isEmpty()) {
        return false;
    }
    return true;
}

//=============================================================================
// WatchdogDevicePage
//=============================================================================

WatchdogDevicePage::WatchdogDevicePage(QWidget *parent)
    : DeviceConfigPage(parent)
{
    setupUI();
}

void WatchdogDevicePage::setupUI()
{
    auto *layout = new QFormLayout(this);

    // Model
    m_modelCombo = new QComboBox();
    m_modelCombo->addItem("i6300esb", static_cast<int>(WatchdogDevice::WatchdogModel::I6300ESB));
    m_modelCombo->addItem("ib700", static_cast<int>(WatchdogDevice::WatchdogModel::IB700));
    m_modelCombo->addItem("diag288", static_cast<int>(WatchdogDevice::WatchdogModel::Diag288));
    m_modelCombo->addItem("itco", static_cast<int>(WatchdogDevice::WatchdogModel::ITCo));
    m_modelCombo->addItem("aspeed", static_cast<int>(WatchdogDevice::WatchdogModel::ASpeed));
    layout->addRow("Model:", m_modelCombo);

    // Action
    m_actionCombo = new QComboBox();
    m_actionCombo->addItem("Reset", static_cast<int>(WatchdogDevice::WatchdogAction::Reset));
    m_actionCombo->addItem("Shutdown", static_cast<int>(WatchdogDevice::WatchdogAction::Shutdown));
    m_actionCombo->addItem("Poweroff", static_cast<int>(WatchdogDevice::WatchdogAction::Poweroff));
    m_actionCombo->addItem("Pause", static_cast<int>(WatchdogDevice::WatchdogAction::Pause));
    m_actionCombo->addItem("None", static_cast<int>(WatchdogDevice::WatchdogAction::None));
    m_actionCombo->addItem("Dump", static_cast<int>(WatchdogDevice::WatchdogAction::Dump));
    m_actionCombo->addItem("Inject NMI", static_cast<int>(WatchdogDevice::WatchdogAction::InjectNMI));
    layout->addRow("Action:", m_actionCombo);
}

Device* WatchdogDevicePage::createDevice()
{
    auto *watchdog = new WatchdogDevice();

    watchdog->setModel(static_cast<WatchdogDevice::WatchdogModel>(m_modelCombo->currentData().toInt()));
    watchdog->setAction(static_cast<WatchdogDevice::WatchdogAction>(m_actionCombo->currentData().toInt()));

    return watchdog;
}

bool WatchdogDevicePage::validate()
{
    return true;
}

//=============================================================================
// RNGDevicePage
//=============================================================================

RNGDevicePage::RNGDevicePage(QWidget *parent)
    : DeviceConfigPage(parent)
{
    setupUI();
}

void RNGDevicePage::setupUI()
{
    auto *layout = new QFormLayout(this);

    // Model
    m_modelCombo = new QComboBox();
    m_modelCombo->addItem("Virtio", static_cast<int>(RNGDevice::RNGModel::Virtio));
    layout->addRow("Model:", m_modelCombo);

    // Backend
    m_backendCombo = new QComboBox();
    m_backendCombo->addItem("Random", static_cast<int>(RNGDevice::RNGBackend::Random));
    m_backendCombo->addItem("EGD", static_cast<int>(RNGDevice::RNGBackend::EGD));
    layout->addRow("Backend:", m_backendCombo);

    // Source path
    m_sourcePathEdit = new QLineEdit();
    m_sourcePathEdit->setText("/dev/urandom");
    layout->addRow("Source Path:", m_sourcePathEdit);
}

Device* RNGDevicePage::createDevice()
{
    auto *rng = new RNGDevice();

    rng->setModel(static_cast<RNGDevice::RNGModel>(m_modelCombo->currentData().toInt()));
    rng->setBackend(static_cast<RNGDevice::RNGBackend>(m_backendCombo->currentData().toInt()));
    rng->setSourcePath(m_sourcePathEdit->text());

    return rng;
}

bool RNGDevicePage::validate()
{
    if (m_sourcePathEdit->text().isEmpty()) {
        return false;
    }
    return true;
}

//=============================================================================
// SmartcardDevicePage
//=============================================================================

SmartcardDevicePage::SmartcardDevicePage(QWidget *parent)
    : DeviceConfigPage(parent)
{
    setupUI();
}

void SmartcardDevicePage::setupUI()
{
    auto *layout = new QFormLayout(this);

    // Mode
    m_modeCombo = new QComboBox();
    m_modeCombo->addItem("Host", static_cast<int>(SmartcardDevice::SmartcardMode::Host));
    m_modeCombo->addItem("Host Certificates", static_cast<int>(SmartcardDevice::SmartcardMode::HostCertificates));
    m_modeCombo->addItem("Passthrough", static_cast<int>(SmartcardDevice::SmartcardMode::Passthrough));
    layout->addRow("Mode:", m_modeCombo);

    // Type
    m_typeCombo = new QComboBox();
    m_typeCombo->addItem("SpiceVMC", static_cast<int>(SmartcardDevice::SmartcardType::SpiceVMC));
    m_typeCombo->addItem("TCP", static_cast<int>(SmartcardDevice::SmartcardType::TCP));
    layout->addRow("Type:", m_typeCombo);

    // Database
    m_databaseEdit = new QLineEdit();
    layout->addRow("Database:", m_databaseEdit);

    // Source path
    m_sourcePathEdit = new QLineEdit();
    layout->addRow("Source Path:", m_sourcePathEdit);
}

Device* SmartcardDevicePage::createDevice()
{
    auto *smartcard = new SmartcardDevice();

    smartcard->setMode(static_cast<SmartcardDevice::SmartcardMode>(m_modeCombo->currentData().toInt()));
    smartcard->setType(static_cast<SmartcardDevice::SmartcardType>(m_typeCombo->currentData().toInt()));
    smartcard->setDatabase(m_databaseEdit->text());
    smartcard->setSourcePath(m_sourcePathEdit->text());

    return smartcard;
}

bool SmartcardDevicePage::validate()
{
    auto mode = static_cast<SmartcardDevice::SmartcardMode>(m_modeCombo->currentData().toInt());
    if (mode == SmartcardDevice::SmartcardMode::Passthrough && m_sourcePathEdit->text().isEmpty()) {
        return false;
    }
    return true;
}

//=============================================================================
// MemballoonDevicePage
//=============================================================================

MemballoonDevicePage::MemballoonDevicePage(QWidget *parent)
    : DeviceConfigPage(parent)
{
    setupUI();
}

void MemballoonDevicePage::setupUI()
{
    auto *layout = new QFormLayout(this);

    // Model
    m_modelCombo = new QComboBox();
    m_modelCombo->addItem("Virtio", static_cast<int>(MemballoonDevice::MemballoonModel::Virtio));
    m_modelCombo->addItem("Virtio Transitional", static_cast<int>(MemballoonDevice::MemballoonModel::VirtioTraditional));
    m_modelCombo->addItem("Virtio Non-Traditional", static_cast<int>(MemballoonDevice::MemballoonModel::VirtioNonTraditional));
    m_modelCombo->addItem("Xen", static_cast<int>(MemballoonDevice::MemballoonModel::Xen));
    m_modelCombo->addItem("None", static_cast<int>(MemballoonDevice::MemballoonModel::None));
    layout->addRow("Model:", m_modelCombo);

    // Autodeflate
    m_autodeflateCheck = new QCheckBox();
    layout->addRow("Auto Deflate:", m_autodeflateCheck);

    // Deflate on OOM
    m_deflateOnOOMCheck = new QCheckBox();
    layout->addRow("Deflate on OOM:", m_deflateOnOOMCheck);

    // Period
    m_periodSpin = new QSpinBox();
    m_periodSpin->setRange(0, 60);
    m_periodSpin->setValue(5);
    m_periodSpin->setSuffix(" seconds");
    layout->addRow("Period:", m_periodSpin);
}

Device* MemballoonDevicePage::createDevice()
{
    auto *balloon = new MemballoonDevice();

    balloon->setModel(static_cast<MemballoonDevice::MemballoonModel>(m_modelCombo->currentData().toInt()));
    balloon->setAutodeflate(m_autodeflateCheck->isChecked());
    balloon->setDeflateOnOOM(m_deflateOnOOMCheck->isChecked());
    balloon->setPeriod(m_periodSpin->value());

    return balloon;
}

bool MemballoonDevicePage::validate()
{
    return true;
}

} // namespace QVirt
