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

#include "ControllerDevice.h"

namespace QVirt {

ControllerDevice::ControllerDevice(QObject *parent)
    : Device(parent)
{
}

QString ControllerDevice::description() const
{
    QString typeStr = controllerTypeToString(m_controllerType);

    if (m_controllerType == ControllerType::USB && m_usbModel != USBModel::None) {
        return QString("%1 Controller (%2)").arg(typeStr).arg(usbModelToString(m_usbModel));
    }

    return QString("%1 Controller").arg(typeStr);
}

QString ControllerDevice::toXML() const
{
    QString xml;

    xml += QString("<controller type='%1'").arg(controllerTypeToString(m_controllerType));

    if (m_index > 0) {
        xml += QString(" index='%1'").arg(m_index);
    }

    if (m_controllerType == ControllerType::USB && m_usbModel != USBModel::None) {
        xml += QString(" model='%1'").arg(usbModelToString(m_usbModel));
    }

    if (m_ports > 0) {
        xml += QString(" ports='%1'").arg(m_ports);
    }

    xml += ">\n";

    // Address
    if (!m_address.isEmpty()) {
        xml += QString("  <address type='%1'/>\n").arg(m_address.toString());
    }

    // Alias
    if (!m_alias.isEmpty()) {
        xml += QString("  <alias name='%1'/>\n").arg(m_alias);
    }

    xml += "</controller>\n";

    return xml;
}

bool ControllerDevice::fromXML(const QString &xml)
{
    // Parse XML and populate properties
    // TODO: Implement proper XML parsing
    Q_UNUSED(xml);
    return false;
}

QString ControllerDevice::controllerTypeToString(ControllerType type)
{
    switch (type) {
    case ControllerType::USB: return "usb";
    case ControllerType::PCI: return "pci";
    case ControllerType::SCSI: return "scsi";
    case ControllerType::SATA: return "sata";
    case ControllerType::IDE: return "ide";
    case ControllerType::VirtioSerial: return "virtio-serial";
    case ControllerType::CCID: return "ccid";
    case ControllerType::IEEE1394: return "ieee1394";
    default: return "usb";
    }
}

ControllerDevice::ControllerType ControllerDevice::stringToControllerType(const QString &str)
{
    QString lower = str.toLower();
    if (lower == "usb") return ControllerType::USB;
    if (lower == "pci") return ControllerType::PCI;
    if (lower == "scsi") return ControllerType::SCSI;
    if (lower == "sata") return ControllerType::SATA;
    if (lower == "ide") return ControllerType::IDE;
    if (lower == "virtio-serial") return ControllerType::VirtioSerial;
    if (lower == "ccid") return ControllerType::CCID;
    if (lower == "ieee1394") return ControllerType::IEEE1394;
    return ControllerType::None;
}

QString ControllerDevice::usbModelToString(USBModel model)
{
    switch (model) {
    case USBModel::Piix3Uhci: return "piix3-uhci";
    case USBModel::Piix4Uhci: return "piix4-uhci";
    case USBModel::Ehci: return "ehci";
    case USBModel::Ich9Ehci1: return "ich9-ehci1";
    case USBModel::Ich9Uhci1: return "ich9-uhci1";
    case USBModel::Ich9Uhci2: return "ich9-uhci2";
    case USBModel::Ich9Uhci3: return "ich9-uhci3";
    case USBModel::VT8231Uhci: return "vt8231-uhci";
    case USBModel::VT82C686BUhci: return "vt82c686b-uhci";
    case USBModel::PCIOhci: return "pci-ohci";
    case USBModel::NecXhci: return "nec-xhci";
    case USBModel::TegraUhci: return "tegra-uhci";
    case USBModel::QemuXhci: return "qemu-xhci";
    default: return "usb";
    }
}

ControllerDevice::USBModel ControllerDevice::stringToUSBModel(const QString &str)
{
    QString lower = str.toLower();
    if (lower == "piix3-uhci") return USBModel::Piix3Uhci;
    if (lower == "piix4-uhci") return USBModel::Piix4Uhci;
    if (lower == "ehci") return USBModel::Ehci;
    if (lower == "ich9-ehci1") return USBModel::Ich9Ehci1;
    if (lower == "ich9-uhci1") return USBModel::Ich9Uhci1;
    if (lower == "ich9-uhci2") return USBModel::Ich9Uhci2;
    if (lower == "ich9-uhci3") return USBModel::Ich9Uhci3;
    if (lower == "vt8231-uhci") return USBModel::VT8231Uhci;
    if (lower == "vt82c686b-uhci") return USBModel::VT82C686BUhci;
    if (lower == "pci-ohci") return USBModel::PCIOhci;
    if (lower == "nec-xhci") return USBModel::NecXhci;
    if (lower == "tegra-uhci") return USBModel::TegraUhci;
    if (lower == "qemu-xhci") return USBModel::QemuXhci;
    return USBModel::None;
}

} // namespace QVirt
