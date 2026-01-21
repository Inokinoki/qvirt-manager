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

#include "GraphicsDevice.h"

namespace QVirt {

GraphicsDevice::GraphicsDevice(QObject *parent)
    : Device(parent)
{
}

QString GraphicsDevice::description() const
{
    QString typeStr = graphicsTypeToString(m_graphicsType);
    return QString("%1 Display").arg(typeStr);
}

QString GraphicsDevice::toXML() const
{
    QString xml;

    QString typeStr = graphicsTypeToString(m_graphicsType);
    xml += QString("<graphics type='%1'").arg(typeStr);

    if (m_autoPort) {
        xml += " autoport='yes'";
    } else {
        if (m_port > 0) {
            xml += QString(" port='%1'").arg(m_port);
        }
    }

    xml += ">\n";

    // Listen configuration
    if (m_listenType == ListenType::Address) {
        xml += QString("  <listen type='address' address='%1'/>\n").arg(m_listenAddress);
    } else if (m_listenType == ListenType::Network) {
        xml += QString("  <listen type='network' network='%1'/>\n").arg(m_listenAddress);
    } else if (m_listenType == ListenType::Socket) {
        xml += QString("  <listen type='socket' socket='%1'/>\n").arg(m_listenAddress);
    }

    // Keymap
    if (!m_keyMap.isEmpty()) {
        xml += QString("  <keymap>%1</keymap>\n").arg(m_keyMap);
    }

    // Password
    if (m_passwordEnabled && !m_password.isEmpty()) {
        xml += QString("  <password>%1</password>\n").arg(m_password);
    }

    // TLS port for SPICE
    if (m_graphicsType == GraphicsType::SPICE && m_tlsPortEnabled) {
        if (m_tlsPort > 0) {
            xml += QString("  <channel name='main' mode='secure' tls='%1'/>\n").arg(m_tlsPort);
        }
    }

    xml += "</graphics>\n";

    return xml;
}

bool GraphicsDevice::fromXML(const QString &xml)
{
    // Parse XML and populate properties
    // TODO: Implement proper XML parsing
    Q_UNUSED(xml);
    return false;
}

QString GraphicsDevice::graphicsTypeToString(GraphicsType type)
{
    switch (type) {
    case GraphicsType::VNC: return "vnc";
    case GraphicsType::SPICE: return "spice";
    case GraphicsType::SDL: return "sdl";
    case GraphicsType::RTP: return "rtp";
    default: return "vnc";
    }
}

GraphicsDevice::GraphicsType GraphicsDevice::stringToGraphicsType(const QString &str)
{
    QString lower = str.toLower();
    if (lower == "vnc") return GraphicsType::VNC;
    if (lower == "spice") return GraphicsType::SPICE;
    if (lower == "sdl") return GraphicsType::SDL;
    if (lower == "rtp") return GraphicsType::RTP;
    return GraphicsType::VNC;
}

} // namespace QVirt
