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

#ifndef QVIRT_DEVICES_DEVICE_H
#define QVIRT_DEVICES_DEVICE_H

#include "../core/BaseObject.h"
#include <QString>

namespace QVirt {

/**
 * @brief Base class for all VM devices
 *
 * Represents a device in a VM's XML configuration.
 * This is the base class for all device types (disk, network, etc.)
 */
class Device : public BaseObject
{
    Q_OBJECT

public:
    enum class DeviceType {
        None,
        Disk,
        Network,
        Controller,
        Input,
        Graphics,
        Sound,
        Video,
        HostDevice,
        FileSystem,
        Smartcard,
        RedirDevice,
        TPM,
        RNG,
        Panic,
        VSock,
        Watchdog,
        Serial,
        Parallel,
        Console,
        Channel,
        Memory,
        ShMem,
        IOMMU,
        PStore,
        Memballoon
    };
    Q_ENUM(DeviceType)

    virtual ~Device() = default;

    // Device identification
    virtual DeviceType deviceType() const = 0;
    virtual QString deviceTypeName() const = 0;

    // Common device properties
    QString alias() const { return m_alias; }
    void setAlias(const QString &alias);

    // XML representation
    virtual QString toXML() const = 0;
    virtual bool fromXML(const QString &xml) = 0;

    // Device description for UI
    virtual QString description() const = 0;

signals:
    void configChanged();

protected:
    explicit Device(QObject *parent = nullptr);

    QString m_alias;
};

/**
 * @brief Device address information
 *
 * Represents the <address> element in device XML
 */
class DeviceAddress
{
public:
    enum class AddressType {
        None,
        PCI,
        Drive,
        VirtioSerial,
        CCID,
        SpaprVio,
        CCW,
        ISA,
        DIMM
    };

    DeviceAddress();
    DeviceAddress(const DeviceAddress &other);

    AddressType type = AddressType::None;

    // PCI address
    unsigned int domain = 0;
    unsigned int pciBus = 0;
    unsigned int slot = 0;
    unsigned int function = 0;

    // Drive address
    int controller = 0;
    int driveBus = 0;
    int target = 0;
    int unit = 0;

    // VirtioSerial address
    int virtioBus = 0;
    int port = 0;

    // Other types
    QString iobase;
    QString irq;
    QString reg;
    int cssid = 0;
    int ssid = 0;
    QString devno;
    QString base;

    bool isEmpty() const;
    QString toString() const;
    bool fromString(const QString &str);
};

/**
 * @brief Device boot configuration
 */
class DeviceBoot
{
public:
    DeviceBoot();
    DeviceBoot(const DeviceBoot &other);

    int order = 0;        // Boot order
    QString loadParm;     // Boot load parameter

    bool isEmpty() const;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_DEVICE_H
