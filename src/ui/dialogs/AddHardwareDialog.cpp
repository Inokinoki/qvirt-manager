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

#include "AddHardwareDialog.h"
#include "../../devices/DiskDevice.h"
#include "../../devices/NetworkDevice.h"
#include "../../devices/ControllerDevice.h"
#include "../../devices/InputDevice.h"
#include "../../devices/GraphicsDevice.h"
#include "../../devices/VideoDevice.h"
#include "../../devices/SoundDevice.h"
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

    m_pageStack->addWidget(storagePage);
    m_pageStack->addWidget(networkPage);
    m_pageStack->addWidget(controllerPage);
    m_pageStack->addWidget(inputPage);
    m_pageStack->addWidget(graphicsPage);
    m_pageStack->addWidget(videoPage);
    m_pageStack->addWidget(soundPage);
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

} // namespace QVirt
