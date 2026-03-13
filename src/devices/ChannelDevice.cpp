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

#include "ChannelDevice.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomNode>

namespace QVirt {

ChannelDevice::ChannelDevice(QObject *parent)
    : Device(parent)
    , m_channelType(ChannelType::VirtioSerial)
    , m_targetType("virtio-serial")
    , m_targetName("org.qemu.guest_agent.0")
    , m_sourceMode("bind")
{
}

ChannelDevice::~ChannelDevice()
{
}

void ChannelDevice::setChannelType(ChannelType type)
{
    if (m_channelType != type) {
        m_channelType = type;
        emit configurationChanged();
    }
}

void ChannelDevice::setTargetType(const QString &type)
{
    if (m_targetType != type) {
        m_targetType = type;
        emit configurationChanged();
    }
}

void ChannelDevice::setTargetName(const QString &name)
{
    if (m_targetName != name) {
        m_targetName = name;
        emit configurationChanged();
    }
}

void ChannelDevice::setSourcePath(const QString &path)
{
    if (m_sourcePath != path) {
        m_sourcePath = path;
        emit configurationChanged();
    }
}

void ChannelDevice::setSourceMode(const QString &mode)
{
    if (m_sourceMode != mode) {
        m_sourceMode = mode;
        emit configurationChanged();
    }
}

void ChannelDevice::setAddress(const QString &addr)
{
    if (m_address != addr) {
        m_address = addr;
        emit configurationChanged();
    }
}

ChannelDevice *ChannelDevice::clone() const
{
    auto *clone = new ChannelDevice(parent());
    clone->setChannelType(m_channelType);
    clone->setTargetType(m_targetType);
    clone->setTargetName(m_targetName);
    clone->setSourcePath(m_sourcePath);
    clone->setSourceMode(m_sourceMode);
    clone->setAddress(m_address);
    return clone;
}

bool ChannelDevice::isValid() const
{
    // Channel is valid if it has a target type and name
    return !m_targetType.isEmpty() && !m_targetName.isEmpty();
}

QString ChannelDevice::toXML() const
{
    QDomDocument doc;
    QDomElement channelElem = doc.createElement("channel");

    // Set type
    QString typeStr = (m_channelType == ChannelType::Unix) ? "unix" : "unix";
    channelElem.setAttribute("type", typeStr);

    // Source element
    if (!m_sourcePath.isEmpty()) {
        QDomElement sourceElem = doc.createElement("source");
        sourceElem.setAttribute("mode", m_sourceMode);
        sourceElem.setAttribute("path", m_sourcePath);
        channelElem.appendChild(sourceElem);
    }

    // Target element
    QDomElement targetElem = doc.createElement("target");
    targetElem.setAttribute("type", m_targetType);
    targetElem.setAttribute("name", m_targetName);
    channelElem.appendChild(targetElem);

    // Address element (if set)
    if (!m_address.isEmpty()) {
        QDomElement addrElem = doc.createElement("address");
        // Parse address string and add attributes
        // Format: "type='virtio-serial' controller='0' bus='0' port='1'"
        QStringList parts = m_address.split(" ");
        for (const QString &part : parts) {
            QStringList kv = part.split("=");
            if (kv.size() == 2) {
                addrElem.setAttribute(kv[0].trimmed(), kv[1].trimmed().replace("'", ""));
            }
        }
        channelElem.appendChild(addrElem);
    }

    doc.appendChild(channelElem);
    return doc.toString();
}

bool ChannelDevice::fromXML(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) {
        return false;
    }

    QDomElement elem = doc.documentElement();
    if (elem.tagName() != "channel") {
        return false;
    }

    // Get type attribute
    QString type = elem.attribute("type");
    if (type == "unix") {
        m_channelType = ChannelType::Unix;
    }

    // Get source element
    QDomElement sourceElem = elem.firstChildElement("source");
    if (!sourceElem.isNull()) {
        m_sourceMode = sourceElem.attribute("mode", "bind");
        m_sourcePath = sourceElem.attribute("path");
    }

    // Get target element
    QDomElement targetElem = elem.firstChildElement("target");
    if (!targetElem.isNull()) {
        m_targetType = targetElem.attribute("type", "virtio-serial");
        m_targetName = targetElem.attribute("name");
    }

    // Get address element
    QDomElement addrElem = elem.firstChildElement("address");
    if (!addrElem.isNull()) {
        // Convert address element to string representation
        QStringList addrParts;
        QDomNamedNodeMap attrs = addrElem.attributes();
        for (int i = 0; i < attrs.count(); ++i) {
            QDomAttr attr = attrs.item(i).toAttr();
            if (!attr.isNull()) {
                addrParts.append(QString("%1='%2'").arg(attr.name()).arg(attr.value()));
            }
        }
        m_address = addrParts.join(" ");
    }

    emit configurationChanged();
    return true;
}

} // namespace QVirt
