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
#include "../../libvirt/EnumMapper.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QIcon>
#include <QDomDocument>
#include <QTextStream>

namespace QVirt {

DetailsPage::DetailsPage(Domain *domain, QWidget *parent)
    : QWidget(parent)
    , m_readOnly(false)
    , m_domain(domain)
{
    setupUI();
    updateReadOnlyMode();
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

    // Read-only warning label (hidden by default)
    m_readOnlyLabel = new QLabel(leftPanel);
    m_readOnlyLabel->setText("<b>Note:</b> This VM is loaded from cache (offline mode). Hardware configuration cannot be modified.");
    m_readOnlyLabel->setWordWrap(true);
    m_readOnlyLabel->setVisible(false);

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

    leftLayout->addWidget(m_readOnlyLabel);
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
    m_splitter->setStretchFactor(0, 3);
    m_splitter->setStretchFactor(1, 1);

    // Set a maximum width for the right panel to make it smaller
    rightPanel->setMaximumWidth(400);
    rightPanel->setMinimumWidth(200);

    mainLayout->addWidget(m_splitter);
}

void DetailsPage::updateReadOnlyMode()
{
    m_readOnly = m_domain->isCached();

    // Show/hide read-only label
    m_readOnlyLabel->setVisible(m_readOnly);

    // Disable hardware modification buttons when in read-only mode
    m_btnAddHardware->setEnabled(!m_readOnly);
    m_btnRemoveHardware->setEnabled(!m_readOnly);

    // Update tooltip to explain why buttons are disabled
    if (m_readOnly) {
        m_btnAddHardware->setToolTip("Cannot add hardware to cached VMs (offline mode)");
        m_btnRemoveHardware->setToolTip("Cannot remove hardware from cached VMs (offline mode)");
    } else {
        m_btnAddHardware->setToolTip("");
        m_btnRemoveHardware->setToolTip("");
    }
}

void DetailsPage::populateDeviceTree()
{
    m_deviceTree->clear();

    // Parse domain XML once and reuse for all sections
    QString xml = m_domain->getXMLDesc();
    QDomDocument doc;
    QDomDocument::ParseResult result = doc.setContent(xml);
    bool xmlValid = result.errorMessage.isEmpty();

    // Overview item (basic VM info)
    auto *overviewItem = addDeviceCategory("Overview", QString());
    addDevice(overviewItem, "Name", m_domain->name());
    addDevice(overviewItem, "UUID", m_domain->uuid());

    // For cached VMs, show "Cached (Offline)" state
    QString stateText;
    if (m_domain->isCached()) {
        stateText = "Cached (Offline)";
    } else {
        stateText = EnumMapper::stateToString(m_domain->state());
    }
    addDevice(overviewItem, "State", stateText);
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

    // Boot section - parse from XML
    auto *bootItem = addDeviceCategory("Boot", "boot");
    bool hasBoot = false;
    if (xmlValid) {
        QDomElement root = doc.documentElement();
        QDomNodeList osElements = root.elementsByTagName("os");
        if (!osElements.isEmpty()) {
            QDomElement osElement = osElements.at(0).toElement();
            QDomNodeList bootNodes = osElement.elementsByTagName("boot");
            if (bootNodes.isEmpty()) {
                addDevice(bootItem, "Boot Device", "hd (default)");
            } else {
                for (int i = 0; i < bootNodes.size(); ++i) {
                    QDomElement bootElement = bootNodes.at(i).toElement();
                    QString dev = bootElement.attribute("dev", "hd");
                    addDevice(bootItem, QString("Boot Device %1").arg(i + 1), dev);
                }
            }
            hasBoot = true;
        }
    }
    if (!hasBoot) {
        addDevice(bootItem, "Boot Device", "hd (default)");
    }

    // Disks section - properly parse from XML
    auto *disksItem = addDeviceCategory("Disk Devices", "drive-harddisk");
    bool hasDisks = false;
    if (xmlValid) {
        QDomElement root = doc.documentElement();
        QDomNodeList diskNodes = root.elementsByTagName("disk");
        for (int i = 0; i < diskNodes.size(); ++i) {
            QDomElement diskElement = diskNodes.at(i).toElement();
            QString device = diskElement.attribute("device", "disk");
            QDomElement targetElem = diskElement.firstChildElement("target");
            QString targetDev = targetElem.attribute("dev", "");
            QDomElement sourceElem = diskElement.firstChildElement("source");
            QString sourceFile = sourceElem.attribute("file", "");
            if (sourceFile.isEmpty()) {
                sourceFile = sourceElem.attribute("dev", "");
            }

            QString diskInfo = QString("%1 - %2").arg(device, targetDev);
            if (!sourceFile.isEmpty()) {
                diskInfo += QString(" (%1)").arg(sourceFile);
            }
            addDevice(disksItem, QString("Disk %1").arg(i + 1), diskInfo);
            hasDisks = true;
        }
    }
    if (!hasDisks) {
        addDevice(disksItem, "No disk devices", "-");
    }

    // Network section - properly parse from XML
    auto *netItem = addDeviceCategory("Network Interfaces", "network-wired");
    bool hasNetwork = false;
    if (xmlValid) {
        QDomElement root = doc.documentElement();
        QDomNodeList interfaceNodes = root.elementsByTagName("interface");
        for (int i = 0; i < interfaceNodes.size(); ++i) {
            QDomElement ifaceElement = interfaceNodes.at(i).toElement();
            QString type = ifaceElement.attribute("type", "unknown");
            QString typeDetail;

            // Get type-specific details
            QDomElement macElem = ifaceElement.firstChildElement("mac");
            QString macAddr = macElem.attribute("address", "");

            QDomElement sourceElem = ifaceElement.firstChildElement("source");
            QString source = sourceElem.attribute("bridge", sourceElem.attribute("network", ""));

            QDomElement targetElem = ifaceElement.firstChildElement("target");
            QString target = targetElem.attribute("dev", "");

            QDomElement modelElem = ifaceElement.firstChildElement("model");
            QString model = modelElem.attribute("type", "");

            if (type == "network") {
                typeDetail = QString("network=%1").arg(source);
            } else if (type == "bridge") {
                typeDetail = QString("bridge=%1").arg(source);
            } else if (type == "direct") {
                typeDetail = QString("dev=%1").arg(source);
            } else {
                typeDetail = type;
            }

            if (!macAddr.isEmpty()) {
                typeDetail += QString(", MAC=%1").arg(macAddr);
            }
            if (!model.isEmpty()) {
                typeDetail += QString(", model=%1").arg(model);
            }

            addDevice(netItem, QString("Network %1").arg(i + 1), typeDetail);
            hasNetwork = true;
        }
    }
    if (!hasNetwork) {
        addDevice(netItem, "No network interfaces", "-");
    }

    // Input section - parse from XML
    auto *inputItem = addDeviceCategory("Input", "input-keyboard");
    bool hasInputDevices = false;
    if (xmlValid) {
        QDomElement root = doc.documentElement();
        QDomNodeList inputNodes = root.elementsByTagName("input");
        for (int i = 0; i < inputNodes.size(); ++i) {
            QDomElement inputElement = inputNodes.at(i).toElement();
            QString type = inputElement.attribute("type", "mouse");
            QString bus = inputElement.attribute("bus", "ps2");
            addDevice(inputItem, QString("%1 (%2)").arg(type, bus), bus);
            hasInputDevices = true;
        }
    }
    if (!hasInputDevices) {
        addDevice(inputItem, "No input devices", "-");
    }

    // Graphics section - parse from XML
    auto *gfxItem = addDeviceCategory("Display", "video-display");
    bool hasGraphics = false;
    if (xmlValid) {
        QDomElement root = doc.documentElement();
        // Parse graphics elements
        QDomNodeList graphicsNodes = root.elementsByTagName("graphics");
        for (int i = 0; i < graphicsNodes.size(); ++i) {
            QDomElement gfxElement = graphicsNodes.at(i).toElement();
            QString type = gfxElement.attribute("type", "vnc");
            addDevice(gfxItem, QString("Graphics (%1)").arg(type), type);
            hasGraphics = true;
        }
        // Parse video elements
        QDomNodeList videoNodes = root.elementsByTagName("video");
        for (int i = 0; i < videoNodes.size(); ++i) {
            QDomElement videoElement = videoNodes.at(i).toElement();
            QDomNodeList modelNodes = videoElement.elementsByTagName("model");
            if (!modelNodes.isEmpty()) {
                QDomElement modelElement = modelNodes.at(0).toElement();
                QString modelType = modelElement.attribute("type", "cirrus");
                addDevice(gfxItem, QString("Video (%1)").arg(modelType), modelType);
                hasGraphics = true;
            }
        }
    }
    if (!hasGraphics) {
        addDevice(gfxItem, "No graphics devices", "-");
    }

    // Sound section - parse from XML
    auto *soundItem = addDeviceCategory("Sound", "audio-card");
    bool hasSound = false;
    if (xmlValid) {
        QDomElement root = doc.documentElement();
        QDomNodeList soundNodes = root.elementsByTagName("sound");
        for (int i = 0; i < soundNodes.size(); ++i) {
            QDomElement soundElement = soundNodes.at(i).toElement();
            QString model = soundElement.attribute("model", "ich6");
            addDevice(soundItem, QString("Audio (%1)").arg(model), model);
            hasSound = true;
        }
    }
    if (!hasSound) {
        addDevice(soundItem, "No sound devices", "-");
    }

    // USB section - parse from XML
    auto *usbItem = addDeviceCategory("USB", "usb");
    bool hasUSB = false;
    if (xmlValid) {
        QDomElement root = doc.documentElement();
        QDomNodeList controllerNodes = root.elementsByTagName("controller");
        for (int i = 0; i < controllerNodes.size(); ++i) {
            QDomElement controllerElement = controllerNodes.at(i).toElement();
            QString type = controllerElement.attribute("type", "");
            if (type == "usb") {
                QString model = controllerElement.attribute("model", "usb3.0");
                addDevice(usbItem, QString("USB Controller (%1)").arg(model), model);
                hasUSB = true;
            }
        }
    }
    if (!hasUSB) {
        addDevice(usbItem, "No USB controllers", "-");
    }

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
            iconPath = ":/icons/devices/sound.svg";
        } else if (icon == "input-keyboard" || icon == "input") {
            iconPath = ":/icons/devices/input.svg";
        } else if (icon == "usb") {
            iconPath = ":/icons/devices/usb.svg";
        } else if (icon == "boot") {
            iconPath = ":/icons/devices/boot.svg";
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
    // Prevent adding hardware in read-only mode
    if (m_readOnly) {
        QMessageBox::warning(this, "Cannot Add Hardware",
            "Cannot add hardware to a VM loaded from cache (offline mode).\n\n"
            "Connect to the hypervisor to modify VM configuration.");
        return;
    }

    auto *dialog = new AddHardwareDialog(m_domain, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    if (dialog->exec() == QDialog::Accepted) {
        Device *device = dialog->getCreatedDevice();
        if (device) {
            // Add device to domain
            QString xml = device->toXML();

            // Check if VM is running to determine if we need live attach
            bool isRunning = (m_domain->state() == Domain::StateRunning);

            if (isRunning) {
                // Try to attach device to running VM
                if (m_domain->attachDevice(xml)) {
                    QMessageBox::information(this, "Device Added",
                        QString("Device '%1' (%2) has been attached to the running VM.")
                            .arg(device->deviceTypeName())
                            .arg(device->description()));
                } else {
                    QMessageBox::warning(this, "Device Attach Failed",
                        "Failed to attach device to the running VM.\n"
                        "The device will be added to the persistent configuration instead.");
                    // Fall through to persistent configuration update
                }
            } else {
                // VM is not running, just update the persistent XML
                QString domainXML = m_domain->getXMLDesc(VIR_DOMAIN_XML_INACTIVE);
                QDomDocument doc;
                if (doc.setContent(domainXML)) {
                    // Parse the device XML and insert it into the domain XML
                    QDomDocument deviceDoc;
                    if (deviceDoc.setContent(xml)) {
                        QDomElement devicesElement = doc.documentElement().firstChildElement("devices");
                        if (!devicesElement.isNull()) {
                            QDomNode importedNode = doc.importNode(deviceDoc.documentElement(), true);
                            devicesElement.appendChild(importedNode);

                            // Update the domain XML
                            if (m_domain->setXML(doc.toString(), VIR_DOMAIN_XML_INACTIVE)) {
                                QMessageBox::information(this, "Device Added",
                                    QString("Device '%1' (%2) has been added to the VM configuration.\n\n"
                                           "The device will be available when the VM is started.")
                                        .arg(device->deviceTypeName())
                                        .arg(device->description()));
                            } else {
                                QMessageBox::critical(this, "Device Add Failed",
                                    "Failed to add device to VM configuration.");
                            }
                        }
                    }
                }
            }

            // Refresh the device tree
            populateDeviceTree();
        }
    }
}

void DetailsPage::onRemoveHardware()
{
    // Prevent removing hardware in read-only mode
    if (m_readOnly) {
        QMessageBox::warning(this, "Cannot Remove Hardware",
            "Cannot remove hardware from a VM loaded from cache (offline mode).\n\n"
            "Connect to the hypervisor to modify VM configuration.");
        return;
    }

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

        // Get the device XML from the getDeviceXML function
        QString deviceXML = getDeviceXML(parentName);

        if (deviceXML.isEmpty()) {
            QMessageBox::warning(this, "Cannot Remove",
                "Could not retrieve device XML for removal.\n"
                "This device might not be removable.");
            return;
        }

        // Check if VM is running
        bool isRunning = (m_domain->state() == Domain::StateRunning);

        bool success = false;
        if (isRunning) {
            // Try to detach device from running VM
            // Note: We need the specific device XML for the selected device
            // For now, we'll attempt to remove it from the category XML
            success = m_domain->detachDevice(deviceXML);

            if (!success) {
                QMessageBox::warning(this, "Device Detach Failed",
                    "Failed to detach device from the running VM.\n"
                    "The device might not support hot-unplug.");
                return;
            }
        } else {
            // VM is not running, update the persistent XML
            QString domainXML = m_domain->getXMLDesc(VIR_DOMAIN_XML_INACTIVE);
            QDomDocument doc;
            if (!doc.setContent(domainXML)) {
                QMessageBox::critical(this, "Cannot Remove",
                    "Failed to parse domain XML.");
                return;
            }

            // Remove the device element from the domain XML
            QDomElement root = doc.documentElement();
            QDomElement devicesElement = root.firstChildElement("devices");
            if (devicesElement.isNull()) {
                QMessageBox::critical(this, "Cannot Remove",
                    "No devices element in domain XML.");
                return;
            }

            // Find and remove the specific device
            bool deviceRemoved = false;
            QDomNode deviceNode = findDeviceNode(devicesElement, parentName, deviceName);
            if (!deviceNode.isNull()) {
                devicesElement.removeChild(deviceNode);
                deviceRemoved = true;
            }

            if (!deviceRemoved) {
                QMessageBox::warning(this, "Cannot Remove",
                    QString("Could not find device '%1' in the configuration.").arg(deviceName));
                return;
            }

            // Update the domain XML
            if (m_domain->setXML(doc.toString(), VIR_DOMAIN_XML_INACTIVE)) {
                success = true;
            }
        }

        if (success) {
            QMessageBox::information(this, "Device Removed",
                QString("Device '%1' (%2) has been removed.")
                    .arg(deviceName)
                    .arg(deviceType));
        } else {
            QMessageBox::critical(this, "Device Remove Failed",
                "Failed to remove device from VM configuration.");
            return;
        }

        // Refresh the device tree to show updated state
        populateDeviceTree();
    }
}

QDomNode DetailsPage::findDeviceNode(QDomElement &devicesElement, const QString &category, const QString &deviceName)
{
    // Map category names to XML element types
    QString elementType;
    if (category == "Disk Devices") {
        elementType = "disk";
    } else if (category == "Network Interfaces") {
        elementType = "interface";
    } else if (category == "Input") {
        elementType = "input";
    } else if (category == "Display") {
        elementType = "graphics";  // or "video"
    } else if (category == "Sound") {
        elementType = "sound";
    } else if (category == "USB") {
        elementType = "controller";
    } else {
        return QDomNode();
    }

    QDomNodeList nodes = devicesElement.elementsByTagName(elementType);
    for (int i = 0; i < nodes.size(); ++i) {
        QDomNode node = nodes.at(i);
        // Try to match the device name with some property of the node
        // This is a simplified approach - in production you'd match more precisely
        QDomElement element = node.toElement();
        QString target = element.attribute("dev");
        if (target.isEmpty()) {
            QDomNodeList targetNodes = element.elementsByTagName("target");
            if (!targetNodes.isEmpty()) {
                target = targetNodes.at(0).toElement().attribute("dev");
            }
        }

        // Check if this matches the device we want to remove
        if (deviceName.contains(target) || deviceName.contains(element.attribute("type"))) {
            return node;
        }
    }

    return QDomNode();
}

void DetailsPage::refresh()
{
    m_domain->updateInfo();
    updateReadOnlyMode();
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
