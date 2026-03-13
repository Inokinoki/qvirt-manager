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

#include "VSockDevice.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

VSockDevice::VSockDevice(QObject *parent)
    : Device(parent)
{
}

void VSockDevice::setCidAssignment(CIDAssignment assignment)
{
    if (m_cidAssignment != assignment) {
        m_cidAssignment = assignment;
        emit configChanged();
    }
}

void VSockDevice::setCid(unsigned int cid)
{
    if (m_cid != cid) {
        m_cid = cid;
        emit configChanged();
    }
}

void VSockDevice::setAddress(const DeviceAddress &addr)
{
    if (m_addr.type != addr.type || m_addr.toString() != addr.toString()) {
        m_addr = addr;
        emit configChanged();
    }
}

QString VSockDevice::toXML() const
{
    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("vsock");
    writer.writeAttribute("id", "1");

    if (!m_alias.isEmpty()) {
        writer.writeStartElement("alias");
        writer.writeAttribute("name", m_alias);
        writer.writeEndElement();
    }

    writer.writeStartElement("cid");
    if (m_cidAssignment == CIDAssignment::Auto) {
        writer.writeAttribute("auto", "yes");
    } else {
        writer.writeAttribute("auto", "no");
        writer.writeAttribute("value", QString::number(m_cid));
    }
    writer.writeEndElement();

    if (!m_addr.isEmpty()) {
        writer.writeStartElement("address");
        switch (m_addr.type) {
        case DeviceAddress::AddressType::PCI:
            writer.writeAttribute("type", "pci");
            writer.writeAttribute("domain", QString("0x%1").arg(m_addr.domain, 4, 16, QChar('0')));
            writer.writeAttribute("bus", QString("0x%1").arg(m_addr.pciBus, 2, 16, QChar('0')));
            writer.writeAttribute("slot", QString("0x%1").arg(m_addr.slot, 2, 16, QChar('0')));
            writer.writeAttribute("function", QString("0x%1").arg(m_addr.function, 1, 16));
            break;
        case DeviceAddress::AddressType::CCW:
            writer.writeAttribute("type", "ccw");
            writer.writeAttribute("cssid", QString::number(m_addr.cssid));
            writer.writeAttribute("ssid", QString::number(m_addr.ssid));
            writer.writeAttribute("devno", m_addr.devno);
            break;
        default:
            break;
        }
        writer.writeEndElement();
    }

    writer.writeEndElement();
    return xml;
}

bool VSockDevice::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("vsock")) {
                QString alias = reader.attributes().value("alias").toString();
                if (!alias.isEmpty()) {
                    m_alias = alias;
                }
            } else if (reader.name() == QLatin1String("alias")) {
                m_alias = reader.attributes().value("name").toString();
            } else if (reader.name() == QLatin1String("cid")) {
                QString autoAttr = reader.attributes().value("auto").toString();
                if (autoAttr == QLatin1String("yes")) {
                    m_cidAssignment = CIDAssignment::Auto;
                } else {
                    m_cidAssignment = CIDAssignment::Manual;
                    m_cid = reader.attributes().value("value").toString().toUInt();
                }
            } else if (reader.name() == QLatin1String("address")) {
                QString typeStr = reader.attributes().value("type").toString();
                if (typeStr == QLatin1String("pci")) {
                    m_addr.type = DeviceAddress::AddressType::PCI;
                    m_addr.domain = reader.attributes().value("domain").toString().toUInt(nullptr, 16);
                    m_addr.pciBus = reader.attributes().value("bus").toString().toUInt(nullptr, 16);
                    m_addr.slot = reader.attributes().value("slot").toString().toUInt(nullptr, 16);
                    m_addr.function = reader.attributes().value("function").toString().toUInt(nullptr, 16);
                } else if (typeStr == QLatin1String("ccw")) {
                    m_addr.type = DeviceAddress::AddressType::CCW;
                    m_addr.cssid = reader.attributes().value("cssid").toString().toInt();
                    m_addr.ssid = reader.attributes().value("ssid").toString().toInt();
                    m_addr.devno = reader.attributes().value("devno").toString();
                }
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("vsock")) {
            break;
        }
    }
    
    return !reader.hasError();
}

QString VSockDevice::description() const
{
    if (m_cidAssignment == CIDAssignment::Auto) {
        return QStringLiteral("VSOCK (auto CID)");
    }
    return QString::fromLatin1("VSOCK (CID: %1)").arg(m_cid);
}

QString VSockDevice::cidAssignmentToString(CIDAssignment assignment)
{
    switch (assignment) {
        case CIDAssignment::Auto: return QStringLiteral("auto");
        case CIDAssignment::Manual: return QStringLiteral("manual");
        default: return QStringLiteral("auto");
    }
}

VSockDevice::CIDAssignment VSockDevice::stringToCidAssignment(const QString &str)
{
    if (str == QLatin1String("manual")) return CIDAssignment::Manual;
    return CIDAssignment::Auto;
}

} // namespace QVirt
