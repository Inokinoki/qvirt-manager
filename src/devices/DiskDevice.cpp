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

#include "DiskDevice.h"
#include <QDomDocument>

namespace QVirt {

DiskDevice::DiskDevice(QObject *parent)
    : Device(parent)
{
}

QString DiskDevice::description() const
{
    QString busStr = busTypeToString(m_bus);
    QString devStr = deviceTypeToString(m_device);

    if (m_device == DeviceType::Floppy) {
        return QString("Floppy");
    } else if (m_device == DeviceType::CDROM) {
        return QString("%1 CDROM").arg(busStr);
    } else if (m_device == DeviceType::Disk) {
        return QString("%1 Disk %2").arg(busStr).arg(m_target);
    } else if (m_device == DeviceType::LUN) {
        return QString("%1 LUN %2").arg(busStr).arg(m_target);
    }

    return QString("Disk");
}

QString DiskDevice::toXML() const
{
    QString xml;

    xml += QString("<disk type='%1' device='%2'>\n")
        .arg(
            m_diskType == DiskType::File ? "file" :
            m_diskType == DiskType::Block ? "block" :
            m_diskType == DiskType::Dir ? "dir" :
            m_diskType == DiskType::Network ? "network" :
            m_diskType == DiskType::Volume ? "volume" : "file"
        )
        .arg(
            m_device == DeviceType::Disk ? "disk" :
            m_device == DeviceType::CDROM ? "cdrom" :
            m_device == DeviceType::Floppy ? "floppy" :
            m_device == DeviceType::LUN ? "lun" : "disk"
        );

    // Source
    QString sourceAttr;
    switch (m_diskType) {
    case DiskType::File:
        sourceAttr = "file";
        break;
    case DiskType::Block:
        sourceAttr = "dev";
        break;
    case DiskType::Dir:
        sourceAttr = "dir";
        break;
    case DiskType::Volume:
        sourceAttr = "volume";
        break;
    default:
        sourceAttr = "file";
    }

    if (!m_source.isEmpty()) {
        xml += QString("  <source %1='%2'/>\n").arg(sourceAttr).arg(m_source);
    }

    // Target
    if (!m_target.isEmpty()) {
        xml += QString("  <target dev='%1' bus='%2'/>\n")
            .arg(m_target)
            .arg(busTypeToString(m_bus).toLower());
    }

    // Driver
    if (!m_driverName.isEmpty() || !m_driverType.isEmpty()) {
        xml += "  <driver";
        if (!m_driverName.isEmpty()) {
            xml += QString(" name='%1'").arg(m_driverName);
        }
        if (!m_driverType.isEmpty()) {
            xml += QString(" type='%1'").arg(m_driverType);
        }
        if (m_cacheMode != CacheMode::Default) {
            xml += QString(" cache='%1'").arg(cacheModeToString(m_cacheMode));
        }
        xml += "/>\n";
    }

    // Readonly
    if (m_readonly) {
        xml += "  <readonly/>\n";
    }

    // Shareable
    if (m_shareable) {
        xml += "  <shareable/>\n";
    }

    // Boot order
    if (m_bootOrder > 0) {
        xml += QString("  <boot order='%1'/>\n").arg(m_bootOrder);
    }

    // Address
    if (!m_address.isEmpty()) {
        xml += QString("  <address type='%1'/>\n").arg(m_address.toString());
    }

    // Alias
    if (!m_alias.isEmpty()) {
        xml += QString("  <alias name='%1'/>\n").arg(m_alias);
    }

    xml += "</disk>\n";

    return xml;
}

bool DiskDevice::fromXML(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) {
        return false;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "disk") {
        return false;
    }

    // Parse type attribute
    QString typeStr = root.attribute("type");
    if (!typeStr.isEmpty()) {
        if (typeStr == "file") m_diskType = DiskType::File;
        else if (typeStr == "block") m_diskType = DiskType::Block;
        else if (typeStr == "dir") m_diskType = DiskType::Dir;
        else if (typeStr == "network") m_diskType = DiskType::Network;
        else if (typeStr == "volume") m_diskType = DiskType::Volume;
    }

    // Parse device attribute
    QString deviceStr = root.attribute("device");
    if (!deviceStr.isEmpty()) {
        m_device = stringToDeviceType(deviceStr);
    }

    // Parse target element
    QDomNodeList targetNodes = root.elementsByTagName("target");
    if (!targetNodes.isEmpty()) {
        QDomElement targetElem = targetNodes.at(0).toElement();
        m_target = targetElem.attribute("dev");

        QString busStr = targetElem.attribute("bus");
        if (!busStr.isEmpty()) {
            m_bus = stringToBusType(busStr);
        }
    }

    // Parse source element (file, dev, dir, or volume)
    QDomNodeList sourceNodes = root.elementsByTagName("source");
    if (!sourceNodes.isEmpty()) {
        QDomElement sourceElem = sourceNodes.at(0).toElement();

        // Try different source attributes based on disk type
        QString fileStr = sourceElem.attribute("file");
        if (!fileStr.isEmpty()) {
            m_source = fileStr;
        } else {
            QString devStr = sourceElem.attribute("dev");
            if (!devStr.isEmpty()) {
                m_source = devStr;
            } else {
                QString dirStr = sourceElem.attribute("dir");
                if (!dirStr.isEmpty()) {
                    m_source = dirStr;
                } else {
                    QString volumeStr = sourceElem.attribute("volume");
                    if (!volumeStr.isEmpty()) {
                        m_source = volumeStr;
                    } else {
                        QString nameStr = sourceElem.attribute("name");
                        if (!nameStr.isEmpty()) {
                            m_source = nameStr;
                        }
                    }
                }
            }
        }
    }

    // Parse driver element
    QDomNodeList driverNodes = root.elementsByTagName("driver");
    if (!driverNodes.isEmpty()) {
        QDomElement driverElem = driverNodes.at(0).toElement();
        m_driverName = driverElem.attribute("name");
        m_driverType = driverElem.attribute("type");

        // Parse driver cache mode
        QString cacheStr = driverElem.attribute("cache");
        if (!cacheStr.isEmpty()) {
            m_cacheMode = stringToCacheMode(cacheStr);
        }
    }

    // Parse readonly element
    QDomNodeList readonlyNodes = root.elementsByTagName("readonly");
    m_readonly = !readonlyNodes.isEmpty();

    // Parse shareable element
    QDomNodeList shareableNodes = root.elementsByTagName("shareable");
    m_shareable = !shareableNodes.isEmpty();

    // Parse address element
    QDomNodeList addressNodes = root.elementsByTagName("address");
    if (!addressNodes.isEmpty()) {
        QDomElement addressElem = addressNodes.at(0).toElement();
        QString addressStr = addressElem.attribute("type");
        if (!addressStr.isEmpty()) {
            m_address.fromString(addressStr);
        }
    }

    // Parse alias element
    QDomNodeList aliasNodes = root.elementsByTagName("alias");
    if (!aliasNodes.isEmpty()) {
        QDomElement aliasElem = aliasNodes.at(0).toElement();
        m_alias = aliasElem.attribute("name");
    }

    return true;
}

void DiskDevice::setTarget(const QString &target)
{
    if (m_target != target) {
        m_target = target;
        emit configChanged();
    }
}

void DiskDevice::setSource(const QString &source)
{
    if (m_source != source) {
        m_source = source;
        emit configChanged();
    }
}

QString DiskDevice::busTypeToString(BusType bus)
{
    switch (bus) {
    case BusType::IDE: return "IDE";
    case BusType::SCSI: return "SCSI";
    case BusType::Virtio: return "Virtio";
    case BusType::SATA: return "SATA";
    case BusType::USB: return "USB";
    case BusType::SD: return "SD";
    case BusType::FDC: return "FDC";
    default: return "None";
    }
}

DiskDevice::BusType DiskDevice::stringToBusType(const QString &str)
{
    QString lower = str.toLower();
    if (lower == "ide") return BusType::IDE;
    if (lower == "scsi") return BusType::SCSI;
    if (lower == "virtio" || lower == "virtio-scsi" || lower == "virtio-blk") return BusType::Virtio;
    if (lower == "sata") return BusType::SATA;
    if (lower == "usb") return BusType::USB;
    if (lower == "sd") return BusType::SD;
    if (lower == "fdc" || lower == "floppy") return BusType::FDC;
    return BusType::None;
}

QString DiskDevice::deviceTypeToString(DeviceType dev)
{
    switch (dev) {
    case DeviceType::Disk: return "disk";
    case DeviceType::CDROM: return "cdrom";
    case DeviceType::Floppy: return "floppy";
    case DeviceType::LUN: return "lun";
    default: return "disk";
    }
}

DiskDevice::DeviceType DiskDevice::stringToDeviceType(const QString &str)
{
    QString lower = str.toLower();
    if (lower == "cdrom") return DeviceType::CDROM;
    if (lower == "floppy") return DeviceType::Floppy;
    if (lower == "lun") return DeviceType::LUN;
    return DeviceType::Disk;
}

QString DiskDevice::cacheModeToString(CacheMode mode)
{
    switch (mode) {
    case CacheMode::None: return "none";
    case CacheMode::Writethrough: return "writethrough";
    case CacheMode::Writeback: return "writeback";
    case CacheMode::Directsync: return "directsync";
    case CacheMode::Unsafe: return "unsafe";
    default: return "default";
    }
}

DiskDevice::CacheMode DiskDevice::stringToCacheMode(const QString &str)
{
    QString lower = str.toLower();
    if (lower == "none") return CacheMode::None;
    if (lower == "writethrough") return CacheMode::Writethrough;
    if (lower == "writeback") return CacheMode::Writeback;
    if (lower == "directsync") return CacheMode::Directsync;
    if (lower == "unsafe") return CacheMode::Unsafe;
    return CacheMode::Default;
}

} // namespace QVirt
