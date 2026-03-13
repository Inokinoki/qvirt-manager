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

#include "PanicDevice.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

PanicDevice::PanicDevice(QObject *parent)
    : Device(parent)
{
}

void PanicDevice::setModel(PanicModel model)
{
    if (m_model != model) {
        m_model = model;
        emit configChanged();
    }
}

void PanicDevice::setIoPort(const QString &port)
{
    if (m_ioPort != port) {
        m_ioPort = port;
        emit configChanged();
    }
}

void PanicDevice::setMmioAddress(const QString &addr)
{
    if (m_mmioAddress != addr) {
        m_mmioAddress = addr;
        emit configChanged();
    }
}

void PanicDevice::setMmioSize(const QString &size)
{
    if (m_mmioSize != size) {
        m_mmioSize = size;
        emit configChanged();
    }
}

void PanicDevice::setAddress(const DeviceAddress &addr)
{
    if (m_addr.type != addr.type || m_addr.toString() != addr.toString()) {
        m_addr = addr;
        emit configChanged();
    }
}

QString PanicDevice::toXML() const
{
    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("panic");
    writer.writeAttribute("model", modelToString(m_model));

    if (!m_alias.isEmpty()) {
        writer.writeStartElement("alias");
        writer.writeAttribute("name", m_alias);
        writer.writeEndElement();
    }

    if (!m_ioPort.isEmpty()) {
        writer.writeStartElement("ioport");
        writer.writeAttribute("port", m_ioPort);
        writer.writeEndElement();
    }

    if (!m_mmioAddress.isEmpty()) {
        writer.writeStartElement("mmio");
        writer.writeAttribute("address", m_mmioAddress);
        if (!m_mmioSize.isEmpty()) {
            writer.writeAttribute("size", m_mmioSize);
        }
        writer.writeEndElement();
    }

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

bool PanicDevice::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("panic")) {
                QString modelStr = reader.attributes().value("model").toString();
                m_model = stringToModel(modelStr);
                
                QString alias = reader.attributes().value("alias").toString();
                if (!alias.isEmpty()) {
                    m_alias = alias;
                }
            } else if (reader.name() == QLatin1String("alias")) {
                m_alias = reader.attributes().value("name").toString();
            } else if (reader.name() == QLatin1String("ioport")) {
                m_ioPort = reader.attributes().value("port").toString();
            } else if (reader.name() == QLatin1String("mmio")) {
                m_mmioAddress = reader.attributes().value("address").toString();
                m_mmioSize = reader.attributes().value("size").toString();
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
        
        if (reader.isEndElement() && reader.name() == QLatin1String("panic")) {
            break;
        }
    }
    
    return !reader.hasError();
}

QString PanicDevice::description() const
{
    return QString::fromLatin1("Panic (%1)").arg(modelToString(m_model));
}

QString PanicDevice::modelToString(PanicModel model)
{
    switch (model) {
        case PanicModel::ISA: return QStringLiteral("isa");
        case PanicModel::PSpics: return QStringLiteral("pspics");
        case PanicModel::PVPanel: return QStringLiteral("pvpanic");
        case PanicModel::S390: return QStringLiteral("s390");
        default: return QStringLiteral("isa");
    }
}

PanicDevice::PanicModel PanicDevice::stringToModel(const QString &str)
{
    if (str == QLatin1String("pspics")) return PanicModel::PSpics;
    if (str == QLatin1String("pvpanic")) return PanicModel::PVPanel;
    if (str == QLatin1String("s390")) return PanicModel::S390;
    return PanicModel::ISA;
}

} // namespace QVirt
