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

#include "BootConfig.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

//=============================================================================
// BootDevice
//=============================================================================

BootDevice::BootDevice() = default;

BootDevice::BootDevice(const BootDevice &other)
    : type(other.type)
    , order(other.order)
    , dev(other.dev)
{
}

BootDevice &BootDevice::operator=(const BootDevice &other)
{
    if (this != &other) {
        type = other.type;
        order = other.order;
        dev = other.dev;
    }
    return *this;
}

bool BootDevice::isEmpty() const
{
    return dev.isEmpty() && order == 0;
}

//=============================================================================
// BootConfig
//=============================================================================

BootConfig::BootConfig(QObject *parent)
    : BaseObject(parent)
{
}

void BootConfig::setDevices(const QList<BootDevice> &devices)
{
    m_devices = devices;
    emit configChanged();
}

void BootConfig::addDevice(const BootDevice &device)
{
    m_devices.append(device);
    emit configChanged();
}

void BootConfig::setBootMenu(bool enabled)
{
    if (m_bootMenu != enabled) {
        m_bootMenu = enabled;
        emit configChanged();
    }
}

void BootConfig::setBootMenuTimeout(int timeout)
{
    if (m_bootMenuTimeout != timeout) {
        m_bootMenuTimeout = timeout;
        emit configChanged();
    }
}

QString BootConfig::toXML() const
{
    if (isEmpty()) {
        return QString();
    }

    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("os");

    for (const BootDevice &device : m_devices) {
        if (!device.isEmpty()) {
            writer.writeStartElement("boot");
            if (!device.dev.isEmpty()) {
                writer.writeAttribute("dev", device.dev);
            }
            if (device.order > 0) {
                writer.writeAttribute("order", QString::number(device.order));
            }
            writer.writeEndElement();
        }
    }

    if (m_bootMenu) {
        writer.writeStartElement("bootmenu");
        writer.writeAttribute("enable", "yes");
        if (m_bootMenuTimeout > 0) {
            writer.writeAttribute("timeout", QString::number(m_bootMenuTimeout));
        }
        writer.writeEndElement();
    }

    writer.writeEndElement();
    return xml;
}

bool BootConfig::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("boot")) {
                BootDevice device;
                device.dev = reader.attributes().value("dev").toString();
                device.order = reader.attributes().value("order").toString().toInt();
                m_devices.append(device);
            } else if (reader.name() == QLatin1String("bootmenu")) {
                QString enabled = reader.attributes().value("enable").toString();
                m_bootMenu = (enabled == QLatin1String("yes"));
                m_bootMenuTimeout = reader.attributes().value("timeout").toString().toInt();
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("os")) {
            break;
        }
    }
    
    return !reader.hasError();
}

bool BootConfig::isEmpty() const
{
    if (m_bootMenu) {
        return false;
    }
    for (const BootDevice &device : m_devices) {
        if (!device.isEmpty()) {
            return false;
        }
    }
    return true;
}

} // namespace QVirt
