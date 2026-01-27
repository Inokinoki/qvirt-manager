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

#include "NetworkDevice.h"
#include <QRandomGenerator>

namespace QVirt {

NetworkDevice::NetworkDevice(QObject *parent)
    : Device(parent)
{
    // Generate default MAC address
    m_macAddress = generateMACAddress();
}

QString NetworkDevice::description() const
{
    QString mac = m_macAddress;
    if (!mac.isEmpty() && mac.length() > 8) {
        mac = mac.right(9); // Show last 9 chars: xx:xx:xx
    }
    return QString("NIC %1").arg(mac);
}

QString NetworkDevice::toXML() const
{
    QString xml;

    QString typeStr = networkTypeToString(m_networkType);
    xml += QString("<interface type='%1'>\n").arg(typeStr);

    // Source
    if (!m_source.isEmpty()) {
        switch (m_networkType) {
        case NetworkType::Bridge:
            xml += QString("  <source bridge='%1'/>\n").arg(m_source);
            break;
        case NetworkType::Network:
            xml += QString("  <source network='%1'/>\n").arg(m_source);
            break;
        case NetworkType::Direct:
            xml += QString("  <source dev='%1' mode='%2'/>\n")
                .arg(m_source)
                .arg(m_sourceMode);
            break;
        case NetworkType::User:
            // No source needed for user networking
            break;
        default:
            xml += QString("  <source %1='%2'/>\n")
                .arg(typeStr)
                .arg(m_source);
        }
    }

    // MAC address
    if (!m_macAddress.isEmpty()) {
        xml += QString("  <mac address='%1'/>\n").arg(m_macAddress);
    }

    // Model
    if (m_model != ModelType::None) {
        xml += QString("  <model type='%1'/>\n").arg(modelTypeToString(m_model));
    }

    // Link state
    if (m_linkState != LinkState::Default) {
        xml += QString("  <link state='%1'/>\n").arg(linkStateToString(m_linkState));
    }

    // Target device (for tun/tap)
    if (!m_targetDev.isEmpty()) {
        xml += QString("  <target dev='%1'/>\n").arg(m_targetDev);
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

    xml += "</interface>\n";

    return xml;
}

bool NetworkDevice::fromXML(const QString &xml)
{
    // Parse XML and populate properties
    // XML parsing is handled by QDomDocument
    Q_UNUSED(xml);
    return false;
}

void NetworkDevice::setSource(const QString &source)
{
    if (m_source != source) {
        m_source = source;
        emit configChanged();
    }
}

void NetworkDevice::setMacAddress(const QString &mac)
{
    if (m_macAddress != mac) {
        m_macAddress = mac;
        emit configChanged();
    }
}

QString NetworkDevice::networkTypeToString(NetworkType type)
{
    switch (type) {
    case NetworkType::Bridge: return "bridge";
    case NetworkType::Network: return "network";
    case NetworkType::Direct: return "direct";
    case NetworkType::User: return "user";
    case NetworkType::Vhostuser: return "vhostuser";
    case NetworkType::Server: return "server";
    case NetworkType::Client: return "client";
    case NetworkType::MCast: return "mcast";
    case NetworkType::UDP: return "udp";
    case NetworkType::Hostdev: return "hostdev";
    default: return "network";
    }
}

NetworkDevice::NetworkType NetworkDevice::stringToNetworkType(const QString &str)
{
    QString lower = str.toLower();
    if (lower == "bridge") return NetworkType::Bridge;
    if (lower == "network") return NetworkType::Network;
    if (lower == "direct") return NetworkType::Direct;
    if (lower == "user") return NetworkType::User;
    if (lower == "vhostuser") return NetworkType::Vhostuser;
    if (lower == "server") return NetworkType::Server;
    if (lower == "client") return NetworkType::Client;
    if (lower == "mcast") return NetworkType::MCast;
    if (lower == "udp") return NetworkType::UDP;
    if (lower == "hostdev") return NetworkType::Hostdev;
    return NetworkType::Network;
}

QString NetworkDevice::modelTypeToString(ModelType model)
{
    switch (model) {
    case ModelType::Virtio: return "virtio";
    case ModelType::E1000: return "e1000";
    case ModelType::RTL8139: return "rtl8139";
    case ModelType::Ne2kPci: return "ne2k_pci";
    case ModelType::Pcnet: return "pcnet";
    case ModelType::i82551: return "i82551";
    case ModelType::i82557b: return "i82557b";
    case ModelType::i82559er: return "i82559er";
    case ModelType::i82801: return "i82501";
    case ModelType::VMXNET3: return "vmxnet3";
    case ModelType::Lance: return "lance";
    default: return "virtio";
    }
}

NetworkDevice::ModelType NetworkDevice::stringToModelType(const QString &str)
{
    QString lower = str.toLower();
    if (lower == "virtio") return ModelType::Virtio;
    if (lower == "e1000" || lower == "e1000-82540em" ||
        lower == "e1000-82544gc" || lower == "e1000-82545em") return ModelType::E1000;
    if (lower == "rtl8139") return ModelType::RTL8139;
    if (lower == "ne2k_pci") return ModelType::Ne2kPci;
    if (lower == "pcnet") return ModelType::Pcnet;
    if (lower == "i82551") return ModelType::i82551;
    if (lower == "i82557b") return ModelType::i82557b;
    if (lower == "i82559er") return ModelType::i82559er;
    if (lower == "i82501") return ModelType::i82801;
    if (lower == "vmxnet3") return ModelType::VMXNET3;
    if (lower == "lance") return ModelType::Lance;
    return ModelType::Virtio;
}

QString NetworkDevice::linkStateToString(LinkState state)
{
    switch (state) {
    case LinkState::Up: return "up";
    case LinkState::Down: return "down";
    default: return "default";
    }
}

NetworkDevice::LinkState NetworkDevice::stringToLinkState(const QString &str)
{
    QString lower = str.toLower();
    if (lower == "up") return LinkState::Up;
    if (lower == "down") return LinkState::Down;
    return LinkState::Default;
}

QString NetworkDevice::generateMACAddress()
{
    // Generate a random MAC address with libvirt's prefix (52:54:00)
    // 52:54:00 is the standard prefix for QEMU/KVM VMs
    QString mac = "52:54:00";

    auto *rng = QRandomGenerator::global();

    for (int i = 0; i < 3; ++i) {
        quint8 byte = static_cast<quint8>(rng->bounded(256));
        mac += QString(":%1").arg(byte, 2, 16, QChar('0'));
    }

    return mac.toUpper();
}

} // namespace QVirt
