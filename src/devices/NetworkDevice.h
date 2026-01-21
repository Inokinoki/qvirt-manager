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

#ifndef QVIRT_DEVICES_NETWORKDEVICE_H
#define QVIRT_DEVICES_NETWORKDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Network interface device
 *
 * Represents <interface> element in domain XML
 */
class NetworkDevice : public Device
{
    Q_OBJECT

public:
    enum class NetworkType {
        None,
        Bridge,
        Network,
        Direct,
        User,
        Vhostuser,
        Server,
        Client,
        MCast,
        UDP,
        Hostdev
    };
    Q_ENUM(NetworkType)

    enum class ModelType {
        None,
        Virtio,
        E1000,
    RTL8139,
        Ne2kPci,
        Pcnet,
        i82551,
        i82557b,
        i82559er,
        i82801,
        VMXNET3,
        Lance
    };
    Q_ENUM(ModelType)

    enum class LinkState {
        Default,
        Up,
        Down
    };
    Q_ENUM(LinkState)

    explicit NetworkDevice(QObject *parent = nullptr);
    ~NetworkDevice() override = default;

    // Device interface
    DeviceType deviceType() const override { return DeviceType::Network; }
    QString deviceTypeName() const override { return QStringLiteral("interface"); }
    QString description() const override;
    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    // Network properties
    NetworkType networkType() const { return m_networkType; }
    void setNetworkType(NetworkType type) { m_networkType = type; }

    QString source() const { return m_source; }
    void setSource(const QString &source);

    QString sourceMode() const { return m_sourceMode; }
    void setSourceMode(const QString &mode) { m_sourceMode = mode; }

    QString macAddress() const { return m_macAddress; }
    void setMacAddress(const QString &mac);

    ModelType model() const { return m_model; }
    void setModel(ModelType model) { m_model = model; }

    LinkState linkState() const { return m_linkState; }
    void setLinkState(LinkState state) { m_linkState = state; }

    DeviceAddress address() const { return m_address; }
    void setAddress(const DeviceAddress &addr) { m_address = addr; }

    int bootOrder() const { return m_bootOrder; }
    void setBootOrder(int order) { m_bootOrder = order; }

    QString targetDev() const { return m_targetDev; }
    void setTargetDev(const QString &dev) { m_targetDev = dev; }

    // Format helpers
    static QString networkTypeToString(NetworkType type);
    static NetworkType stringToNetworkType(const QString &str);

    static QString modelTypeToString(ModelType model);
    static ModelType stringToModelType(const QString &str);

    static QString linkStateToString(LinkState state);
    static LinkState stringToLinkState(const QString &str);

    // Generate random MAC address
    static QString generateMACAddress();

private:
    NetworkType m_networkType = NetworkType::Network;
    QString m_source;
    QString m_sourceMode;
    QString m_macAddress;
    ModelType m_model = ModelType::Virtio;
    LinkState m_linkState = LinkState::Default;
    DeviceAddress m_address;
    int m_bootOrder = 0;
    QString m_targetDev;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_NETWORKDEVICE_H
