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

#include "DetailsPage.h"
#include "../dialogs/AddHardwareDialog.h"
#include "../../core/Error.h"

#include <QHeaderView>
#include <QMessageBox>

namespace QVirt {

DetailsPage::DetailsPage(Domain *domain, QWidget *parent)
    : QWidget(parent)
    , m_domain(domain)
{
    setupUI();
    populateDeviceTree();
}

void DetailsPage::setupUI()
{
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create splitter for resizable panels
    m_splitter = new QSplitter(Qt::Horizontal, this);

    // Left panel - Device tree
    auto *leftPanel = new QWidget(m_splitter);
    auto *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(5, 5, 5, 5);
    leftLayout->setSpacing(5);

    // Device tree
    m_deviceTree = new QTreeWidget(leftPanel);
    m_deviceTree->setHeaderLabels(QStringList() << "Device" << "Details");
    m_deviceTree->setColumnWidth(0, 200);
    m_deviceTree->setRootIsDecorated(true);
    m_deviceTree->setAlternatingRowColors(true);
    connect(m_deviceTree, &QTreeWidget::itemClicked, this, &DetailsPage::onDeviceSelected);

    leftLayout->addWidget(m_deviceTree);

    // Button layout
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(5);

    m_btnAddHardware = new QPushButton("Add Hardware", leftPanel);
    m_btnRemoveHardware = new QPushButton("Remove", leftPanel);
    m_btnRefresh = new QPushButton("Refresh", leftPanel);

    connect(m_btnAddHardware, &QPushButton::clicked, this, &DetailsPage::onAddHardware);
    connect(m_btnRemoveHardware, &QPushButton::clicked, this, &DetailsPage::onRemoveHardware);
    connect(m_btnRefresh, &QPushButton::clicked, this, &DetailsPage::refresh);

    buttonLayout->addWidget(m_btnAddHardware);
    buttonLayout->addWidget(m_btnRemoveHardware);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_btnRefresh);

    leftLayout->addLayout(buttonLayout);

    // Right panel - Device details
    auto *rightPanel = new QWidget(m_splitter);
    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(5, 5, 5, 5);
    rightLayout->setSpacing(5);

    auto *detailsGroup = new QGroupBox("Device Details", rightPanel);
    auto *detailsLayout = new QVBoxLayout(detailsGroup);

    m_deviceDetails = new QTextEdit(detailsGroup);
    m_deviceDetails->setReadOnly(true);
    m_deviceDetails->setPlainText("Select a device to view its details");

    detailsLayout->addWidget(m_deviceDetails);
    rightLayout->addWidget(detailsGroup);

    // Add panels to splitter
    m_splitter->addWidget(leftPanel);
    m_splitter->addWidget(rightPanel);
    m_splitter->setStretchFactor(0, 1);
    m_splitter->setStretchFactor(1, 2);

    mainLayout->addWidget(m_splitter);
}

void DetailsPage::populateDeviceTree()
{
    m_deviceTree->clear();

    // Overview item (basic VM info)
    auto *overviewItem = addDeviceCategory("Overview", QString());
    addDevice(overviewItem, "Name", m_domain->name());
    addDevice(overviewItem, "UUID", m_domain->uuid());
    addDevice(overviewItem, "State", QString::number(static_cast<int>(m_domain->state())));
    addDevice(overviewItem, "CPUs", QString::number(m_domain->vcpuCount()));
    addDevice(overviewItem, "Memory", QString("%1 MB").arg(m_domain->currentMemory() / 1024));
    overviewItem->setExpanded(true);

    // CPU section
    auto *cpuItem = addDeviceCategory("CPU", "cpu");
    addDevice(cpuItem, "vCPUs", QString::number(m_domain->vcpuCount()));
    addDevice(cpuItem, "Max vCPUs", QString::number(m_domain->maxVcpuCount()));

    // Memory section
    auto *memItem = addDeviceCategory("Memory", "memory");
    addDevice(memItem, "Current Memory", QString("%1 MB").arg(m_domain->currentMemory() / 1024));
    addDevice(memItem, "Max Memory", QString("%1 MB").arg(m_domain->maxMemory() / 1024));

    // Boot section
    auto *bootItem = addDeviceCategory("Boot", "boot");
    addDevice(bootItem, "Boot Device 1", "hd");
    addDevice(bootItem, "Boot Device 2", "cdrom");

    // Disks section
    auto *disksItem = addDeviceCategory("Disk Devices", "drive-harddisk");
    // Parse domain XML to find disks
    QString xml = m_domain->getXMLDesc();
    if (xml.contains("<disk")) {
        // This is a simplified check - in production you'd properly parse the XML
        int count = xml.count("<disk ");
        for (int i = 0; i < count; ++i) {
            addDevice(disksItem, QString("Disk %1").arg(i + 1), "virtio disk");
        }
    } else {
        addDevice(disksItem, "No disks", "-");
    }

    // Network section
    auto *netItem = addDeviceCategory("Network Interfaces", "network-wired");
    if (xml.contains("<interface")) {
        int count = xml.count("<interface ");
        for (int i = 0; i < count; ++i) {
            addDevice(netItem, QString("NIC %1").arg(i + 1), "virtio");
        }
    } else {
        addDevice(netItem, "No network interfaces", "-");
    }

    // Input section
    auto *inputItem = addDeviceCategory("Input", "input-keyboard");
    addDevice(inputItem, "Keyboard", "usb");
    addDevice(inputItem, "Tablet", "usb");

    // Graphics section
    auto *gfxItem = addDeviceCategory("Display", "video-display");
    addDevice(gfxItem, "Video", "qxl");
    addDevice(gfxItem, "Spice Server", "spice");

    // Sound section
    auto *soundItem = addDeviceCategory("Sound", "audio-card");
    addDevice(soundItem, "Audio", "ich6");

    // USB section
    auto *usbItem = addDeviceCategory("USB", "usb");
    addDevice(usbItem, "USB Controller", "ehci");

    // Expand all items by default
    for (int i = 0; i < m_deviceTree->topLevelItemCount(); ++i) {
        m_deviceTree->topLevelItem(i)->setExpanded(true);
    }

    // Select first item
    m_deviceTree->setCurrentItem(m_deviceTree->topLevelItem(0));
}

QTreeWidgetItem* DetailsPage::addDeviceCategory(const QString &name, const QString &icon)
{
    auto *item = new QTreeWidgetItem(m_deviceTree);
    item->setText(0, name);
    item->setText(1, QString());
    item->setFirstColumnSpanned(false);
    // TODO: Set icon when icons are available
    return item;
}

QTreeWidgetItem* DetailsPage::addDevice(QTreeWidgetItem *parent, const QString &name, const QString &details)
{
    auto *item = new QTreeWidgetItem(parent);
    item->setText(0, name);
    item->setText(1, details);
    return item;
}

void DetailsPage::onDeviceSelected(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    if (!item) {
        m_deviceDetails->setPlainText("Select a device to view its details");
        return;
    }

    // Build details text
    QString details;

    QString deviceName = item->text(0);
    QString deviceInfo = item->text(1);

    details += QString("<h3>%1</h3>").arg(deviceName);
    details += "<table border='0' cellpadding='5'>";

    // Add basic info
    if (!deviceInfo.isEmpty()) {
        details += QString("<tr><td><b>Details:</b></td><td>%1</td></tr>").arg(deviceInfo);
    }

    // For top-level categories, add summary
    if (item->parent() == nullptr) {
        int childCount = item->childCount();
        details += QString("<tr><td><b>Items:</b></td><td>%1</td></tr>").arg(childCount);
    }

    details += "</table>";

    // Add raw XML section for advanced users
    details += "<hr>";
    details += "<h4>Raw XML</h4>";
    details += "<pre>";

    // If this is the Overview, show domain XML
    if (item->text(0) == "Overview") {
        QString xml = m_domain->getXMLDesc();
        // Show first 50 lines to avoid overwhelming display
        QStringList lines = xml.split('\n');
        if (lines.size() > 50) {
            details += lines.mid(0, 50).join('\n');
            details += "\n... (truncated)";
        } else {
            details += xml;
        }
    } else {
        details += "&lt;!-- Device XML would be shown here --&gt;\n";
        details += "&lt;!-- Full XML parsing will be implemented in Phase 6 --&gt;";
    }

    details += "</pre>";

    m_deviceDetails->setHtml(details);
}

void DetailsPage::onAddHardware()
{
    auto *dialog = new AddHardwareDialog(m_domain, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    if (dialog->exec() == QDialog::Accepted) {
        Device *device = dialog->getCreatedDevice();
        if (device) {
            // Add device to domain
            // For now, just show a message with the device XML
            QString xml = device->toXML();
            QMessageBox::information(this, "Device Added",
                "Device created successfully!\n\n" +
                QString("Type: %1\n").arg(device->deviceTypeName()) +
                QString("Description: %1\n\n").arg(device->description()) +
                "XML:\n" + xml.left(500) + "...\n\n"
                "Note: Device will be added to VM configuration in the next iteration.\n"
                "This requires virDomainAttachDevice() or domain XML update.");

            // Refresh the device tree
            populateDeviceTree();
        }
    }
}

void DetailsPage::onRemoveHardware()
{
    auto *item = m_deviceTree->currentItem();
    if (!item) {
        return;
    }

    // Don't allow removing categories
    if (item->parent() == nullptr) {
        QMessageBox::warning(this, "Cannot Remove",
            "Cannot remove device categories.\n"
            "Please select a specific device to remove.");
        return;
    }

    // Don't allow removing core items
    QString parentName = item->parent()->text(0);
    if (parentName == "Overview" || parentName == "CPU" || parentName == "Memory" || parentName == "Boot") {
        QMessageBox::warning(this, "Cannot Remove",
            "Cannot remove core VM components.");
        return;
    }

    auto reply = QMessageBox::question(this, "Remove Hardware",
        QString("Are you sure you want to remove '%1'?").arg(item->text(0)),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QMessageBox::information(this, "Not Implemented",
            "Device removal will be implemented in Phase 6.\n\n"
            "This will:\n"
            "1. Remove the device from VM configuration\n"
            "2. Update the domain XML\n"
            "3. Handle hot-plug for running VMs");
    }
}

void DetailsPage::refresh()
{
    m_domain->updateInfo();
    populateDeviceTree();
}

} // namespace QVirt
