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

#include "Guest.h"
#include "Connection.h"
#include "../devices/Device.h"

#include <QUuid>
#include <QXmlStreamWriter>
#include <QRegularExpression>

namespace QVirt {

Guest::Guest(Connection *conn, QObject *parent)
    : BaseObject(parent)
    , m_connection(conn)
{
    // Set defaults
    m_osArch = "x86_64";
    m_osMachine = "q35";
}

Guest::~Guest()
{
    // Clean up devices
    for (auto *disk : m_disks) {
        disk->setParent(nullptr);
        delete disk;
    }
    for (auto *nic : m_networkInterfaces) {
        nic->setParent(nullptr);
        delete nic;
    }
}

void Guest::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit configChanged();
    }
}

void Guest::setUUID(const QString &uuid)
{
    if (m_uuid != uuid) {
        m_uuid = uuid;
        emit configChanged();
    }
}

void Guest::generateUUID()
{
    m_uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    emit configChanged();
}

void Guest::setTitle(const QString &title)
{
    if (m_title != title) {
        m_title = title;
        emit configChanged();
    }
}

void Guest::setDescription(const QString &desc)
{
    if (m_description != desc) {
        m_description = desc;
        emit configChanged();
    }
}

void Guest::setOSType(const QString &type)
{
    if (m_osType != type) {
        m_osType = type;
        emit configChanged();
    }
}

void Guest::setOSArch(const QString &arch)
{
    if (m_osArch != arch) {
        m_osArch = arch;
        emit configChanged();
    }
}

void Guest::setOSMachine(const QString &machine)
{
    if (m_osMachine != machine) {
        m_osMachine = machine;
        emit configChanged();
    }
}

void Guest::setMemory(quint64 memKB)
{
    if (m_memory != memKB) {
        m_memory = memKB;
        emit configChanged();
    }
}

void Guest::setCurrentMemory(quint64 memKB)
{
    if (m_currentMemory != memKB) {
        m_currentMemory = memKB;
        emit configChanged();
    }
}

void Guest::setVCPUs(int count)
{
    if (m_vcpus != count) {
        m_vcpus = count;
        emit configChanged();
    }
}

void Guest::setMaxVCPUs(int count)
{
    if (m_maxVcpus != count) {
        m_maxVcpus = count;
        emit configChanged();
    }
}

void Guest::setGraphicsType(const QString &type)
{
    if (m_graphicsType != type) {
        m_graphicsType = type;
        emit configChanged();
    }
}

void Guest::addDisk(DiskDevice *disk)
{
    if (disk && !m_disks.contains(disk)) {
        disk->setParent(this);
        m_disks.append(disk);
        emit configChanged();
    }
}

void Guest::removeDisk(DiskDevice *disk)
{
    if (m_disks.removeOne(disk)) {
        disk->setParent(nullptr);
        emit configChanged();
    }
}

void Guest::clearDisks()
{
    while (!m_disks.isEmpty()) {
        auto *disk = m_disks.takeFirst();
        disk->setParent(nullptr);
        delete disk;
    }
    emit configChanged();
}

void Guest::addNetworkInterface(NetworkDevice *nic)
{
    if (nic && !m_networkInterfaces.contains(nic)) {
        nic->setParent(this);
        m_networkInterfaces.append(nic);
        emit configChanged();
    }
}

void Guest::removeNetworkInterface(NetworkDevice *nic)
{
    if (m_networkInterfaces.removeOne(nic)) {
        nic->setParent(nullptr);
        emit configChanged();
    }
}

void Guest::clearNetworkInterfaces()
{
    while (!m_networkInterfaces.isEmpty()) {
        auto *nic = m_networkInterfaces.takeFirst();
        nic->setParent(nullptr);
        delete nic;
    }
    emit configChanged();
}

bool Guest::validate() const
{
    m_validationError.clear();

    // Validate name
    if (m_name.isEmpty()) {
        m_validationError = "VM name cannot be empty";
        return false;
    }

    if (m_name.contains(QRegularExpression("[^a-zA-Z0-9_-]"))) {
        m_validationError = "VM name can only contain letters, numbers, hyphens, and underscores";
        return false;
    }

    // Validate UUID
    if (!m_uuid.isEmpty()) {
        QUuid uuid(m_uuid);
        if (uuid.isNull()) {
            m_validationError = "Invalid UUID format";
            return false;
        }
    }

    // Validate memory
    if (m_currentMemory == 0 || m_memory == 0) {
        m_validationError = "Memory cannot be zero";
        return false;
    }

    if (m_currentMemory > m_memory) {
        m_validationError = "Current memory cannot exceed maximum memory";
        return false;
    }

    // Validate CPUs
    if (m_vcpus == 0 || m_maxVcpus == 0) {
        m_validationError = "CPU count cannot be zero";
        return false;
    }

    if (m_vcpus > m_maxVcpus) {
        m_validationError = "vCPUs cannot exceed maximum vCPUs";
        return false;
    }

    // Validate disks
    for (const auto *disk : m_disks) {
        if (disk->diskType() == DiskDevice::DiskType::File) {
            QString path = disk->source();
            if (!path.isEmpty() && !disk->source().startsWith("/")) {
                m_validationError = QString("Disk path must be absolute: %1").arg(path);
                return false;
            }
        }
    }

    return true;
}

QString Guest::buildOSSection() const
{
    QString xml;
    xml += "  <os>\n";

    // OS type
    xml += QString("    <type arch='%1' machine='%2'>hvm</type>\n")
        .arg(m_osArch)
        .arg(m_osMachine);

    // UEFI loader
    if (m_uefiRequested) {
        xml += "    <loader readonly='yes' type='pflash'>/usr/share/OVMF/OVMF_CODE.fd</loader>\n";
        xml += "    <nvram>/usr/share/OVMF/OVMF_VARS.fd</nvram>\n";
    }

    xml += "  </os>\n";

    return xml;
}

QString Guest::buildFeaturesSection() const
{
    QString xml;
    xml += "  <features>\n";

    if (m_acpiEnabled) {
        xml += "    <acpi/>\n";
    }

    if (m_apicEnabled) {
        xml += "    <apic/>\n";
    }

    xml += "  </features>\n";

    return xml;
}

QString Guest::buildCPUSection() const
{
    QString xml;
    xml += QString("  <vcpu placement='static'>%1</vcpu>\n").arg(m_vcpus);
    return xml;
}

QString Guest::buildDevicesSection() const
{
    QString xml;
    xml += "  <devices>\n";

    // Disk devices
    for (const auto *disk : m_disks) {
        QString diskXML = disk->toXML();
        // Indent the disk XML
        QStringList lines = diskXML.split('\n');
        for (auto &line : lines) {
            if (!line.trimmed().isEmpty()) {
                xml += "  " + line + "\n";
            }
        }
    }

    // Network interfaces
    for (const auto *nic : m_networkInterfaces) {
        QString nicXML = nic->toXML();
        QStringList lines = nicXML.split('\n');
        for (auto &line : lines) {
            if (!line.trimmed().isEmpty()) {
                xml += "  " + line + "\n";
            }
        }
    }

    // Input devices (default tablet and keyboard)
    xml += "    <input type='tablet' bus='usb'/>\n";
    xml += "    <input type='keyboard' bus='usb'/>\n";

    // Graphics
    if (m_hasGraphics) {
        xml += QString("    <graphics type='%1' autoport='yes' listen='0.0.0.0'>\n")
            .arg(m_graphicsType);
        if (m_graphicsListen) {
            xml += "      <listen type='address' address='0.0.0.0'/>\n";
        }
        xml += "    </graphics>\n";
    }

    // Console
    xml += "    <console type='pty'>\n";
    xml += "      <target type='serial' port='0'/>\n";
    xml += "    </console>\n";

    // Video
    xml += "    <video>\n";
    xml += "      <model type='qxl'/>\n";
    xml += "    </video>\n";

    // Sound
    xml += "    <sound model='ich6'/>\n";

    // USB controller
    xml += "    <controller type='usb' index='0' model='qemu-xhci'/>\n";

    xml += "  </devices>\n";

    return xml;
}

QString Guest::toXML() const
{
    QString xml;

    xml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml += "<domain type='kvm'>\n";

    // Name
    xml += QString("  <name>%1</name>\n").arg(m_name);

    // UUID
    if (!m_uuid.isEmpty()) {
        xml += QString("  <uuid>%1</uuid>\n").arg(m_uuid);
    }

    // Title and description
    if (!m_title.isEmpty()) {
        xml += QString("  <title>%1</title>\n").arg(m_title);
    }

    if (!m_description.isEmpty()) {
        xml += QString("  <description>%1</description>\n").arg(m_description);
    }

    // Memory
    xml += QString("  <memory unit='KiB'>%1</memory>\n").arg(m_memory);
    xml += QString("  <currentMemory unit='KiB'>%1</currentMemory>\n").arg(m_currentMemory);

    // CPU
    xml += buildCPUSection();

    // OS
    xml += buildOSSection();

    // Features
    xml += buildFeaturesSection();

    // Devices
    xml += buildDevicesSection();

    xml += "</domain>\n";

    return xml;
}

QString Guest::toXMLPretty() const
{
    // For now, return the same XML
    // Could use QDomDocument for pretty printing
    return toXML();
}

} // namespace QVirt
