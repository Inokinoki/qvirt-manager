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

#include "DetailsPage.h"
#include "../dialogs/AddHardwareDialog.h"
#include "../../core/Error.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QIcon>
#include <QDomDocument>
#include <QTextStream>

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

    // Set icon if available
    if (!icon.isEmpty()) {
        QString iconPath;

        // Map icon names to resource paths
        if (icon == "cpu") {
            iconPath = ":/icons/devices/cpu.svg";
        } else if (icon == "memory") {
            iconPath = ":/icons/devices/memory.svg";
        } else if (icon == "drive-harddisk" || icon == "disk") {
            iconPath = ":/icons/devices/disk.svg";
        } else if (icon == "network-wired" || icon == "network") {
            iconPath = ":/icons/devices/network.svg";
        } else if (icon == "video-display" || icon == "display") {
            iconPath = ":/icons/devices/display.svg";
        } else if (icon == "audio-card" || icon == "sound") {
            // No sound icon defined yet, use a fallback
            iconPath = QString();
        } else if (icon == "input-keyboard" || icon == "input") {
            // No input icon defined yet, use a fallback
            iconPath = QString();
        } else if (icon == "usb") {
            // No USB icon defined yet, use a fallback
            iconPath = QString();
        } else if (icon == "boot") {
            // No boot icon defined yet, use a fallback
            iconPath = QString();
        }

        if (!iconPath.isEmpty()) {
            item->setIcon(0, QIcon(iconPath));
        }
    }

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

    QString formattedXML;
    if (deviceName == "Overview") {
        // Format the domain XML using QDomDocument for proper indentation
        QString xml = m_domain->getXMLDesc();
        QDomDocument doc;
        QString errorMsg;
        int errorLine = 0, errorColumn = 0;

        if (doc.setContent(xml, &errorMsg, &errorLine, &errorColumn)) {
            // Convert to formatted string
            QString buffer;
            QTextStream stream(&buffer);
            doc.save(stream, 2);  // Indent with 2 spaces
            stream.flush();
            formattedXML = buffer;

            // Truncate if too long (more than 100 lines)
            QStringList lines = formattedXML.split('\n');
            if (lines.size() > 100) {
                formattedXML = lines.mid(0, 100).join('\n') + "\n... (truncated)";
            }
        } else {
            formattedXML = QString("<!-- Error parsing XML: %1 -->").arg(errorMsg);
        }
    } else {
        // For device categories, extract relevant XML section
        formattedXML = getDeviceXML(deviceName);
        if (formattedXML.isEmpty()) {
            formattedXML = "<!-- No XML available for this device (category: " + deviceName + ") -->\n";
        }
    }

    // HTML escape the formatted XML for display
    formattedXML = formattedXML.replace("&", "&amp;");
    formattedXML = formattedXML.replace("<", "&lt;");
    formattedXML = formattedXML.replace(">", "&gt;");
    details += formattedXML;

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
        // Get device info for display
        QString deviceName = item->text(0);
        QString deviceType = parentName;

        QMessageBox::information(this, "Device Removed",
            QString("Device '%1' (%2) has been marked for removal.\n\n")
                .arg(deviceName)
                .arg(deviceType) +
            "In production, this would:\n"
            "1. Remove the device from VM configuration\n"
            "2. Update the domain XML using virDomainUndefine() / virDomainDefineXML()\n"
            "3. Handle hot-unplug for running VMs if supported\n\n"
            "The device tree will refresh to show the updated configuration.");

        // Remove from tree for now
        delete item;

        // Refresh to show updated state
        populateDeviceTree();
    }
}

void DetailsPage::refresh()
{
    m_domain->updateInfo();
    populateDeviceTree();
}

QString DetailsPage::getDeviceXML(const QString &categoryName)
{
    QString domainXML = m_domain->getXMLDesc();
    if (domainXML.isEmpty()) {
        return "<!-- Error: Could not retrieve domain XML -->";
    }

    QDomDocument doc;
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;

    if (!doc.setContent(domainXML, &errorMsg, &errorLine, &errorColumn)) {
        return QString("<!-- Error parsing XML at line %1, column %2: %3 -->")
            .arg(errorLine).arg(errorColumn).arg(errorMsg);
    }

    QDomElement root = doc.documentElement();

    // Helper function to convert node to XML string with proper formatting
    auto nodeToString = [](const QDomNode& node) -> QString {
        QString result;
        QTextStream stream(&result);
        node.save(stream, 2);  // Indent with 2 spaces
        stream.flush();  // Ensure all content is written
        return result;
    };

    QStringList elements;

    if (categoryName == "CPU") {
        QDomNodeList vcpus = root.elementsByTagName("vcpu");
        for (int i = 0; i < vcpus.size(); ++i) {
            elements << nodeToString(vcpus.at(i));
        }
        QDomNodeList features = root.elementsByTagName("features");
        for (int i = 0; i < features.size(); ++i) {
            elements << nodeToString(features.at(i));
        }
    } else if (categoryName == "Memory") {
        QDomNodeList memory = root.elementsByTagName("memory");
        for (int i = 0; i < memory.size(); ++i) {
            elements << nodeToString(memory.at(i));
        }
        QDomNodeList currentMemory = root.elementsByTagName("currentMemory");
        for (int i = 0; i < currentMemory.size(); ++i) {
            elements << nodeToString(currentMemory.at(i));
        }
    } else if (categoryName == "Boot") {
        QDomNodeList osElements = root.elementsByTagName("os");
        for (int i = 0; i < osElements.size(); ++i) {
            elements << nodeToString(osElements.at(i));
        }
    } else if (categoryName == "Disk Devices") {
        QDomNodeList disks = root.elementsByTagName("disk");
        for (int i = 0; i < disks.size(); ++i) {
            elements << nodeToString(disks.at(i));
        }
    } else if (categoryName == "Network Interfaces") {
        QDomNodeList interfaces = root.elementsByTagName("interface");
        for (int i = 0; i < interfaces.size(); ++i) {
            elements << nodeToString(interfaces.at(i));
        }
    } else if (categoryName == "Input") {
        QDomNodeList inputs = root.elementsByTagName("input");
        for (int i = 0; i < inputs.size(); ++i) {
            elements << nodeToString(inputs.at(i));
        }
    } else if (categoryName == "Display") {
        QDomNodeList graphics = root.elementsByTagName("graphics");
        for (int i = 0; i < graphics.size(); ++i) {
            elements << nodeToString(graphics.at(i));
        }
        QDomNodeList videos = root.elementsByTagName("video");
        for (int i = 0; i < videos.size(); ++i) {
            elements << nodeToString(videos.at(i));
        }
    } else if (categoryName == "Sound") {
        QDomNodeList sounds = root.elementsByTagName("sound");
        for (int i = 0; i < sounds.size(); ++i) {
            elements << nodeToString(sounds.at(i));
        }
    } else if (categoryName == "USB") {
        QDomNodeList controllers = root.elementsByTagName("controller");
        for (int i = 0; i < controllers.size(); ++i) {
            QDomElement controller = controllers.at(i).toElement();
            if (!controller.isNull() && controller.attribute("type") == "usb") {
                elements << nodeToString(controller);
            }
        }
    }

    if (elements.isEmpty()) {
        return QString();
    }

    // Join elements with newlines for proper formatting
    return elements.join("\n");
}

} // namespace QVirt
