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

#ifndef QVIRT_DEVICES_INPUTDEVICE_H
#define QVIRT_DEVICES_INPUTDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Input device (keyboard, mouse, tablet)
 *
 * Represents <input> element in domain XML
 */
class InputDevice : public Device
{
    Q_OBJECT

public:
    enum class InputType {
        None,
        Tablet,
        Mouse,
        Keyboard
    };
    Q_ENUM(InputType)

    enum class BusType {
        None,
        USB,
        Virtio,
        PS2,
        Xen,
        i8042
    };
    Q_ENUM(BusType)

    explicit InputDevice(QObject *parent = nullptr);
    ~InputDevice() override = default;

    // Device interface
    DeviceType deviceType() const override { return DeviceType::Input; }
    QString deviceTypeName() const override { return QStringLiteral("input"); }
    QString description() const override;
    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    // Input properties
    InputType inputType() const { return m_inputType; }
    void setInputType(InputType type) { m_inputType = type; }

    BusType bus() const { return m_bus; }
    void setBus(BusType bus) { m_bus = bus; }

    // Format helpers
    static QString inputTypeToString(InputType type);
    static InputType stringToInputType(const QString &str);

    static QString busTypeToString(BusType bus);
    static BusType stringToBusType(const QString &str);

private:
    InputType m_inputType = InputType::None;
    BusType m_bus = BusType::None;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_INPUTDEVICE_H
