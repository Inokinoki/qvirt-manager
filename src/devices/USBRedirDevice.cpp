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

#include "USBRedirDevice.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

USBRedirDevice::USBRedirDevice(QObject *parent)
    : Device(parent)
{
}

void USBRedirDevice::setBus(int bus)
{
    if (m_bus != bus) {
        m_bus = bus;
        emit configChanged();
    }
}

void USBRedirDevice::setPort(int port)
{
    if (m_port != port) {
        m_port = port;
        emit configChanged();
    }
}

void USBRedirDevice::setVendorId(const QString &id)
{
    if (m_vendorId != id) {
        m_vendorId = id;
        emit configChanged();
    }
}

void USBRedirDevice::setProductId(const QString &id)
{
    if (m_productId != id) {
        m_productId = id;
        emit configChanged();
    }
}

void USBRedirDevice::setAddress(const DeviceAddress &addr)
{
    if (m_addr.type != addr.type || m_addr.toString() != addr.toString()) {
        m_addr = addr;
        emit configChanged();
    }
}

QString USBRedirDevice::toXML() const
{
    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("redirdev");
    writer.writeAttribute("bus", "usb");

    if (!m_alias.isEmpty()) {
        writer.writeStartElement("alias");
        writer.writeAttribute("name", m_alias);
        writer.writeEndElement();
    }

    if (!m_vendorId.isEmpty() || !m_productId.isEmpty()) {
        writer.writeStartElement("source");
        if (!m_vendorId.isEmpty()) {
            writer.writeAttribute("vendorId", m_vendorId);
        }
        if (!m_productId.isEmpty()) {
            writer.writeAttribute("productId", m_productId);
        }
        writer.writeEndElement();
    }

    if (m_bus >= 0) {
        writer.writeStartElement("address");
        writer.writeAttribute("type", "usb");
        writer.writeAttribute("bus", QString::number(m_bus));
        if (m_port >= 0) {
            writer.writeAttribute("port", QString::number(m_port));
        }
        writer.writeEndElement();
    }

    writer.writeEndElement();
    return xml;
}

bool USBRedirDevice::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("redirdev")) {
                QString alias = reader.attributes().value("alias").toString();
                if (!alias.isEmpty()) {
                    m_alias = alias;
                }
            } else if (reader.name() == QLatin1String("alias")) {
                m_alias = reader.attributes().value("name").toString();
            } else if (reader.name() == QLatin1String("source")) {
                m_vendorId = reader.attributes().value("vendorId").toString();
                m_productId = reader.attributes().value("productId").toString();
            } else if (reader.name() == QLatin1String("address")) {
                QString typeStr = reader.attributes().value("type").toString();
                if (typeStr == QLatin1String("usb")) {
                    m_addr.type = DeviceAddress::AddressType::PCI;
                    m_bus = reader.attributes().value("bus").toString().toInt();
                    m_port = reader.attributes().value("port").toString().toInt();
                }
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("redirdev")) {
            break;
        }
    }
    
    return !reader.hasError();
}

QString USBRedirDevice::description() const
{
    if (!m_vendorId.isEmpty() || !m_productId.isEmpty()) {
        return QString::fromLatin1("USB Redirection (%1:%2)")
            .arg(m_vendorId.isEmpty() ? QStringLiteral("*") : m_vendorId)
            .arg(m_productId.isEmpty() ? QStringLiteral("*") : m_productId);
    }
    return QStringLiteral("USB Redirection (any)");
}

} // namespace QVirt
