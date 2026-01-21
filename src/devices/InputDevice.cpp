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

#include "InputDevice.h"

namespace QVirt {

InputDevice::InputDevice(QObject *parent)
    : Device(parent)
{
}

QString InputDevice::description() const
{
    QString typeStr = inputTypeToString(m_inputType);
    QString busStr = busTypeToString(m_bus);

    if (!busStr.isEmpty()) {
        return QString("%1 (%2)").arg(typeStr).arg(busStr);
    }

    return typeStr;
}

QString InputDevice::toXML() const
{
    QString xml;

    xml += QString("<input type='%1'").arg(inputTypeToString(m_inputType));

    if (m_bus != BusType::None) {
        xml += QString(" bus='%1'").arg(busTypeToString(m_bus));
    }

    xml += "/>\n";

    return xml;
}

bool InputDevice::fromXML(const QString &xml)
{
    // Parse XML and populate properties
    // TODO: Implement proper XML parsing
    Q_UNUSED(xml);
    return false;
}

QString InputDevice::inputTypeToString(InputType type)
{
    switch (type) {
    case InputType::Tablet: return "tablet";
    case InputType::Mouse: return "mouse";
    case InputType::Keyboard: return "keyboard";
    default: return "tablet";
    }
}

InputDevice::InputType InputDevice::stringToInputType(const QString &str)
{
    QString lower = str.toLower();
    if (lower == "tablet") return InputType::Tablet;
    if (lower == "mouse") return InputType::Mouse;
    if (lower == "keyboard") return InputType::Keyboard;
    return InputType::None;
}

QString InputDevice::busTypeToString(BusType bus)
{
    switch (bus) {
    case BusType::USB: return "usb";
    case BusType::Virtio: return "virtio";
    case BusType::PS2: return "ps2";
    case BusType::Xen: return "xen";
    case BusType::i8042: return "i8042";
    default: return QString();
    }
}

InputDevice::BusType InputDevice::stringToBusType(const QString &str)
{
    QString lower = str.toLower();
    if (lower == "usb") return BusType::USB;
    if (lower == "virtio") return BusType::Virtio;
    if (lower == "ps2") return BusType::PS2;
    if (lower == "xen") return BusType::Xen;
    if (lower == "i8042") return BusType::i8042;
    return BusType::None;
}

} // namespace QVirt
