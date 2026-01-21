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

#ifndef QVIRT_DEVICES_CONTROLLERDEVICE_H
#define QVIRT_DEVICES_CONTROLLERDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Controller device (USB, SCSI, SATA, PCI, IDE, etc.)
 *
 * Represents <controller> element in domain XML
 */
class ControllerDevice : public Device
{
    Q_OBJECT

public:
    enum class ControllerType {
        None,
        USB,
        PCI,
        SCSI,
        SATA,
        IDE,
        VirtioSerial,
        CCID,
        IEEE1394
    };
    Q_ENUM(ControllerType)

    enum class USBModel {
        None,
        Piix3Uhci,
        Piix4Uhci,
        Ehci,
        Ich9Ehci1,
        Ich9Uhci1,
        Ich9Uhci2,
        Ich9Uhci3,
        VT8231Uhci,
        VT82C686BUhci,
        PCIOhci,
        NecXhci,
        TegraUhci,
        QemuXhci
    };
    Q_ENUM(USBModel)

    explicit ControllerDevice(QObject *parent = nullptr);
    ~ControllerDevice() override = default;

    // Device interface
    DeviceType deviceType() const override { return DeviceType::Controller; }
    QString deviceTypeName() const override { return QStringLiteral("controller"); }
    QString description() const override;
    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    // Controller properties
    ControllerType controllerType() const { return m_controllerType; }
    void setControllerType(ControllerType type) { m_controllerType = type; }

    int index() const { return m_index; }
    void setIndex(int index) { m_index = index; }

    USBModel usbModel() const { return m_usbModel; }
    void setUSBModel(USBModel model) { m_usbModel = model; }

    int ports() const { return m_ports; }
    void setPorts(int ports) { m_ports = ports; }

    DeviceAddress address() const { return m_address; }
    void setAddress(const DeviceAddress &addr) { m_address = addr; }

    // Format helpers
    static QString controllerTypeToString(ControllerType type);
    static ControllerType stringToControllerType(const QString &str);

    static QString usbModelToString(USBModel model);
    static USBModel stringToUSBModel(const QString &str);

private:
    ControllerType m_controllerType = ControllerType::None;
    int m_index = 0;
    USBModel m_usbModel = USBModel::None;
    int m_ports = 0;
    DeviceAddress m_address;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_CONTROLLERDEVICE_H
