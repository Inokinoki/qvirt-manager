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

#ifndef QVIRT_DEVICES_GRAPHICSDEVICE_H
#define QVIRT_DEVICES_GRAPHICSDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Graphics display device
 *
 * Represents <graphics> element in domain XML
 */
class GraphicsDevice : public Device
{
    Q_OBJECT

public:
    enum class GraphicsType {
        None,
        VNC,
        SPICE,
        SDL,
        RTP
    };
    Q_ENUM(GraphicsType)

    enum class ListenType {
        None,
        Address,
        Network,
        Socket
    };
    Q_ENUM(ListenType)

    explicit GraphicsDevice(QObject *parent = nullptr);
    ~GraphicsDevice() override = default;

    // Device interface
    DeviceType deviceType() const override { return DeviceType::Graphics; }
    QString deviceTypeName() const override { return QStringLiteral("graphics"); }
    QString description() const override;
    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    // Graphics properties
    GraphicsType graphicsType() const { return m_graphicsType; }
    void setGraphicsType(GraphicsType type) { m_graphicsType = type; }

    int port() const { return m_port; }
    void setPort(int port) { m_port = port; }

    bool autoPort() const { return m_autoPort; }
    void setAutoPort(bool autoPort) { m_autoPort = autoPort; }

    QString listenAddress() const { return m_listenAddress; }
    void setListenAddress(const QString &addr) { m_listenAddress = addr; }

    ListenType listenType() const { return m_listenType; }
    void setListenType(ListenType type) { m_listenType = type; }

    QString keyMap() const { return m_keyMap; }
    void setKeyMap(const QString &keymap) { m_keyMap = keymap; }

    bool passwordEnabled() const { return m_passwordEnabled; }
    void setPasswordEnabled(bool enabled) { m_passwordEnabled = enabled; }

    QString password() const { return m_password; }
    void setPassword(const QString &password) { m_password = password; }

    bool tlsPortEnabled() const { return m_tlsPortEnabled; }
    void setTLSPortEnabled(bool enabled) { m_tlsPortEnabled = enabled; }

    int tlsPort() const { return m_tlsPort; }
    void setTLSPort(int port) { m_tlsPort = port; }

    // Format helpers
    static QString graphicsTypeToString(GraphicsType type);
    static GraphicsType stringToGraphicsType(const QString &str);

private:
    GraphicsType m_graphicsType = GraphicsType::SPICE;
    int m_port = -1;
    bool m_autoPort = true;
    QString m_listenAddress = "0.0.0.0";
    ListenType m_listenType = ListenType::Address;
    QString m_keyMap = "en-us";
    bool m_passwordEnabled = false;
    QString m_password;
    bool m_tlsPortEnabled = false;
    int m_tlsPort = -1;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_GRAPHICSDEVICE_H
