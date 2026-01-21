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

#include "Device.h"

namespace QVirt {

//=============================================================================
// Device
//=============================================================================

Device::Device(QObject *parent)
    : BaseObject(parent)
    , m_alias(QString())
{
}

void Device::setAlias(const QString &alias)
{
    if (m_alias != alias) {
        m_alias = alias;
        emit configChanged();
    }
}

//=============================================================================
// DeviceAddress
//=============================================================================

DeviceAddress::DeviceAddress() = default;

DeviceAddress::DeviceAddress(const DeviceAddress &other)
    : type(other.type)
    , domain(other.domain)
    , pciBus(other.pciBus)
    , slot(other.slot)
    , function(other.function)
    , controller(other.controller)
    , driveBus(other.driveBus)
    , target(other.target)
    , unit(other.unit)
    , virtioBus(other.virtioBus)
    , port(other.port)
    , iobase(other.iobase)
    , irq(other.irq)
    , reg(other.reg)
    , cssid(other.cssid)
    , ssid(other.ssid)
    , devno(other.devno)
    , base(other.base)
{
}

bool DeviceAddress::isEmpty() const
{
    return type == AddressType::None;
}

QString DeviceAddress::toString() const
{
    if (isEmpty()) {
        return QString();
    }

    switch (type) {
    case AddressType::PCI:
        return QString("pci:%1:%2:%3.%4")
            .arg(domain)
            .arg(pciBus)
            .arg(slot)
            .arg(function);

    case AddressType::Drive:
        return QString("drive:%1:%2:%3:%4")
            .arg(controller)
            .arg(driveBus)
            .arg(target)
            .arg(unit);

    case AddressType::VirtioSerial:
        return QString("virtio-serial:%1:%2:%3")
            .arg(controller)
            .arg(virtioBus)
            .arg(port);

    case AddressType::CCID:
        return QString("ccid:%1:%2")
            .arg(controller)
            .arg(slot);

    case AddressType::SpaprVio:
        return QString("spapr-vio:%1").arg(reg);

    case AddressType::CCW:
        return QString("ccw:%1.%2.%3")
            .arg(cssid)
            .arg(ssid)
            .arg(devno);

    case AddressType::ISA:
        return QString("isa:%1:%2").arg(iobase).arg(irq);

    case AddressType::DIMM:
        return QString("dimm:%1").arg(base);

    default:
        return QString();
    }
}

bool DeviceAddress::fromString(const QString &str)
{
    if (str.isEmpty()) {
        return false;
    }

    // Parse address type
    QStringList parts = str.split(':');
    if (parts.isEmpty()) {
        return false;
    }

    QString typeStr = parts[0];
    if (typeStr == "pci") {
        type = AddressType::PCI;
        // Parse pci:domain:bus:slot.function
        if (parts.size() >= 4) {
            domain = parts[1].toUInt();
            pciBus = parts[2].toUInt();
            QStringList slotFunc = parts[3].split('.');
            if (slotFunc.size() >= 2) {
                slot = slotFunc[0].toUInt();
                function = slotFunc[1].toUInt();
            }
        }
    } else if (typeStr == "drive") {
        type = AddressType::Drive;
        // Parse drive:controller:bus:target:unit
        if (parts.size() >= 5) {
            controller = parts[1].toInt();
            driveBus = parts[2].toInt();
            target = parts[3].toInt();
            unit = parts[4].toInt();
        }
    }
    // Add other types as needed...

    return true;
}

//=============================================================================
// DeviceBoot
//=============================================================================

DeviceBoot::DeviceBoot() = default;

DeviceBoot::DeviceBoot(const DeviceBoot &other)
    : order(other.order)
    , loadParm(other.loadParm)
{
}

bool DeviceBoot::isEmpty() const
{
    return order == 0 && loadParm.isEmpty();
}

} // namespace QVirt
