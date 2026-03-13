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

#ifndef QVIRT_DEVICES_USBREDIRDEVICE_H
#define QVIRT_DEVICES_USBREDIRDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief USB Redirection device for SPICE
 *
 * Represents a USB redirection device that allows redirecting USB devices
 * from host to guest over SPICE protocol
 */
class USBRedirDevice : public Device
{
    Q_OBJECT

public:
    explicit USBRedirDevice(QObject *parent = nullptr);
    ~USBRedirDevice() override = default;

    DeviceType deviceType() const override { return DeviceType::RedirDevice; }
    QString deviceTypeName() const override { return QStringLiteral("USB Redirection"); }

    int bus() const { return m_bus; }
    void setBus(int bus);

    int port() const { return m_port; }
    void setPort(int port);

    QString vendorId() const { return m_vendorId; }
    void setVendorId(const QString &id);

    QString productId() const { return m_productId; }
    void setProductId(const QString &id);

    DeviceAddress address() const { return m_addr; }
    void setAddress(const DeviceAddress &addr);

    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    QString description() const override;

private:
    int m_bus = -1;
    int m_port = -1;
    QString m_vendorId;
    QString m_productId;
    DeviceAddress m_addr;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_USBREDIRDEVICE_H
