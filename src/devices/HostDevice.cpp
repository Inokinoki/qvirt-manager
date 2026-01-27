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

#include "HostDevice.h"

namespace QVirt {

HostDevice::HostDevice(QObject *parent)
    : Device(parent)
{
}

void HostDevice::setHostDeviceType(HostDeviceType type)
{
    if (m_deviceType != type) {
        m_deviceType = type;
        emit configChanged();
    }
}

void HostDevice::setUsbMode(USBMode mode)
{
    if (m_usbMode != mode) {
        m_usbMode = mode;
        emit configChanged();
    }
}

void HostDevice::setUsbBus(int bus)
{
    if (m_usbBus != bus) {
        m_usbBus = bus;
        emit configChanged();
    }
}

void HostDevice::setUsbDevice(int device)
{
    if (m_usbDevice != device) {
        m_usbDevice = device;
        emit configChanged();
    }
}

void HostDevice::setUsbVendorId(const QString &id)
{
    if (m_usbVendorId != id) {
        m_usbVendorId = id;
        emit configChanged();
    }
}

void HostDevice::setUsbProductId(const QString &id)
{
    if (m_usbProductId != id) {
        m_usbProductId = id;
        emit configChanged();
    }
}

void HostDevice::setPciDomain(const QString &domain)
{
    if (m_pciDomain != domain) {
        m_pciDomain = domain;
        emit configChanged();
    }
}

void HostDevice::setPciBus(const QString &bus)
{
    if (m_pciBus != bus) {
        m_pciBus = bus;
        emit configChanged();
    }
}

void HostDevice::setPciSlot(const QString &slot)
{
    if (m_pciSlot != slot) {
        m_pciSlot = slot;
        emit configChanged();
    }
}

void HostDevice::setPciFunction(const QString &function)
{
    if (m_pciFunction != function) {
        m_pciFunction = function;
        emit configChanged();
    }
}

void HostDevice::setScsiAdapter(const QString &adapter)
{
    if (m_scsiAdapter != adapter) {
        m_scsiAdapter = adapter;
        emit configChanged();
    }
}

void HostDevice::setScsiBus(const QString &bus)
{
    if (m_scsiBus != bus) {
        m_scsiBus = bus;
        emit configChanged();
    }
}

void HostDevice::setScsiTarget(const QString &target)
{
    if (m_scsiTarget != target) {
        m_scsiTarget = target;
        emit configChanged();
    }
}

void HostDevice::setScsiLun(const QString &lun)
{
    if (m_scsiLun != lun) {
        m_scsiLun = lun;
        emit configChanged();
    }
}

void HostDevice::setScsiWWID(const QString &wwid)
{
    if (m_scsiWWID != wwid) {
        m_scsiWWID = wwid;
        emit configChanged();
    }
}

void HostDevice::setMdevUUID(const QString &uuid)
{
    if (m_mdevUUID != uuid) {
        m_mdevUUID = uuid;
        emit configChanged();
    }
}

void HostDevice::setMdevAddress(const QString &address)
{
    if (m_mdevAddress != address) {
        m_mdevAddress = address;
        emit configChanged();
    }
}

QString HostDevice::toXML() const
{
    QString xml = QString::fromLatin1("  <hostdev mode='subsystem' type='%1'>\n")
        .arg(typeToString(m_deviceType));

    xml += QString::fromLatin1("    <source>\n");

    switch (m_deviceType) {
        case HostDeviceType::USB:
            if (m_usbMode == USBMode::Bus) {
                xml += QString::fromLatin1("      <address bus='%1' device='%2'/>\n")
                    .arg(m_usbBus)
                    .arg(m_usbDevice);
            } else {
                xml += QString::fromLatin1("      <vendor id='%1'/>\n")
                    .arg(m_usbVendorId);
                xml += QString::fromLatin1("      <product id='%1'/>\n")
                    .arg(m_usbProductId);
            }
            break;

        case HostDeviceType::PCI:
            xml += QString::fromLatin1("      <address domain='%1' bus='%2' slot='%3' function='%4'/>\n")
                .arg(m_pciDomain)
                .arg(m_pciBus)
                .arg(m_pciSlot)
                .arg(m_pciFunction);
            break;

        case HostDeviceType::SCSI:
            xml += QString::fromLatin1("      <adapter name='%1'/>\n")
                .arg(m_scsiAdapter);
            xml += QString::fromLatin1("      <address bus='%1' target='%2' unit='%3'/>\n")
                .arg(m_scsiBus)
                .arg(m_scsiTarget)
                .arg(m_scsiLun);
            break;

        case HostDeviceType::MDev:
            xml += QString::fromLatin1("      <address uuid='%1'/>\n")
                .arg(m_mdevUUID);
            xml += QString::fromLatin1("      <address type='pci' domain='%1' bus='%2' slot='%3' function='%4'/>\n")
                .arg(m_mdevAddress);
            break;

        default:
            break;
    }

    xml += QString::fromLatin1("    </source>\n");
    xml += QString::fromLatin1("  </hostdev>");

    return xml;
}

bool HostDevice::fromXML(const QString &xml)
{
    Q_UNUSED(xml);
    return true;
}

QString HostDevice::description() const
{
    QString desc = typeToString(m_deviceType);

    switch (m_deviceType) {
        case HostDeviceType::USB:
            if (m_usbMode == USBMode::Bus) {
                desc += QString::fromLatin1(" (Bus %1, Device %2)")
                    .arg(m_usbBus)
                    .arg(m_usbDevice);
            } else {
                desc += QString::fromLatin1(" (Vendor %1, Product %2)")
                    .arg(m_usbVendorId)
                    .arg(m_usbProductId);
            }
            break;

        case HostDeviceType::PCI:
            desc += QString::fromLatin1(" (%1:%2.%3)")
                .arg(m_pciBus)
                .arg(m_pciSlot)
                .arg(m_pciFunction);
            break;

        case HostDeviceType::SCSI:
            desc += QString::fromLatin1(" (%1:%2:%3)")
                .arg(m_scsiBus)
                .arg(m_scsiTarget)
                .arg(m_scsiLun);
            break;

        case HostDeviceType::MDev:
            desc += QString::fromLatin1(" (%1)").arg(m_mdevUUID);
            break;

        default:
            break;
    }

    return desc;
}

QString HostDevice::typeToString(HostDeviceType type)
{
    switch (type) {
        case HostDeviceType::USB: return QStringLiteral("usb");
        case HostDeviceType::PCI: return QStringLiteral("pci");
        case HostDeviceType::SCSI: return QStringLiteral("scsi");
        case HostDeviceType::SCSIHost: return QStringLiteral("scsi_host");
        case HostDeviceType::SCSIWWID: return QStringLiteral("scsi_wwn");
        case HostDeviceType::MDev: return QStringLiteral("mdev");
        default: return QStringLiteral("usb");
    }
}

HostDevice::HostDeviceType HostDevice::stringToType(const QString &str)
{
    if (str == QLatin1String("pci")) return HostDeviceType::PCI;
    if (str == QLatin1String("scsi")) return HostDeviceType::SCSI;
    if (str == QLatin1String("scsi_host")) return HostDeviceType::SCSIHost;
    if (str == QLatin1String("scsi_wwn")) return HostDeviceType::SCSIWWID;
    if (str == QLatin1String("mdev")) return HostDeviceType::MDev;
    return HostDeviceType::USB;
}

} // namespace QVirt
