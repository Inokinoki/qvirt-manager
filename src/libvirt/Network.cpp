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

#include "Network.h"
#include "Connection.h"
#include <QDebug>
#include <QDomDocument>

namespace QVirt {

Network::Network(Connection *conn, virNetworkPtr network)
    : BaseObject(conn)
    , m_connection(conn)
    , m_network(network)
    , m_active(false)
    , m_state(StateInactive)
    , m_forwardMode(ForwardNAT)
    , m_dhcpEnabled(false)
{
    const char *name = virNetworkGetName(m_network);
    if (name) {
        m_name = QString::fromUtf8(name);
    }

    char uuid[VIR_UUID_STRING_BUFLEN];
    if (virNetworkGetUUIDString(m_network, uuid) == 0) {
        m_uuid = QString::fromUtf8(uuid);
    }

    // Check if network is active using isActive function
    m_active = (virNetworkIsActive(m_network) == 1);
    if (m_active) {
        m_state = StateRunning;
    }

    // Get and parse XML to determine forward mode and configuration
    char *xml = virNetworkGetXMLDesc(m_network, 0);
    if (xml) {
        parseXML(QString::fromUtf8(xml));
        free(xml);
    }

    qDebug().noquote() << "Created Network wrapper for" << m_name;
}

Network::~Network()
{
    if (m_network) {
        virNetworkFree(m_network);
        m_network = nullptr;
    }
}

bool Network::start()
{
    if (!m_network) {
        return false;
    }

    if (virNetworkCreate(m_network) < 0) {
        return false;
    }

    m_active = true;
    emit stateChanged();
    return true;
}

bool Network::stop()
{
    if (!m_network) {
        return false;
    }

    if (virNetworkDestroy(m_network) < 0) {
        return false;
    }

    m_active = false;
    emit stateChanged();
    return true;
}

bool Network::undefine()
{
    if (!m_network) {
        return false;
    }

    if (virNetworkUndefine(m_network) < 0) {
        return false;
    }

    return true;
}

void Network::updateInfo()
{
    if (!m_network) {
        return;
    }

    // Refresh network info by re-parsing XML
    m_active = (virNetworkIsActive(m_network) == 1);
    if (m_active) {
        m_state = StateRunning;
    } else {
        m_state = StateInactive;
    }

    char *xml = virNetworkGetXMLDesc(m_network, 0);
    if (xml) {
        parseXML(QString::fromUtf8(xml));
        free(xml);
    }
}

void Network::parseXML(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) {
        qWarning() << "Failed to parse network XML";
        return;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "network") {
        qWarning() << "Invalid network XML";
        return;
    }

    // Parse forward mode
    QDomElement forwardElement = root.firstChildElement("forward");
    if (!forwardElement.isNull()) {
        QString mode = forwardElement.attribute("mode", "nat");
        if (mode == "nat") {
            m_forwardMode = ForwardNAT;
        } else if (mode == "route") {
            m_forwardMode = ForwardRoute;
        } else if (mode == "bridge") {
            m_forwardMode = ForwardBridge;
        } else if (mode == "private") {
            m_forwardMode = ForwardPrivate;
        } else if (mode == "vepa") {
            m_forwardMode = ForwardVEPA;
        } else if (mode == "passthrough") {
            m_forwardMode = ForwardPassthrough;
        } else if (mode == "hostdev") {
            m_forwardMode = ForwardHostdev;
        }

        // Get bridge device if specified
        m_bridgeName = forwardElement.attribute("dev");
    }

    // Parse bridge name
    QDomElement bridgeElement = root.firstChildElement("bridge");
    if (!bridgeElement.isNull()) {
        if (m_bridgeName.isEmpty()) {
            m_bridgeName = bridgeElement.attribute("name");
        }
        QString bridgeName = bridgeElement.attribute("name");
        if (!bridgeName.isEmpty()) {
            m_bridgeName = bridgeName;
        }
    }

    // Parse IP configuration
    QDomElement ipElement = root.firstChildElement("ip");
    if (!ipElement.isNull()) {
        m_ipAddress = ipElement.attribute("address");
        m_netmask = ipElement.attribute("netmask");

        // Parse DHCP configuration
        QDomElement dhcpElement = ipElement.firstChildElement("dhcp");
        if (!dhcpElement.isNull()) {
            m_dhcpEnabled = true;

            QDomElement rangeElement = dhcpElement.firstChildElement("range");
            if (!rangeElement.isNull()) {
                m_dhcpStart = rangeElement.attribute("start");
                m_dhcpEnd = rangeElement.attribute("end");
            }
        } else {
            m_dhcpEnabled = false;
        }
    }
}

} // namespace QVirt
