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

#include "SerialDevice.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

SerialDevice::SerialDevice(QObject *parent)
    : Device(parent)
{
}

void SerialDevice::setPort(int port)
{
    if (m_port != port) {
        m_port = port;
        emit configChanged();
    }
}

void SerialDevice::setCharDeviceType(CharDeviceType type)
{
    if (m_charDeviceType != type) {
        m_charDeviceType = type;
        emit configChanged();
    }
}

void SerialDevice::setSourcePath(const QString &path)
{
    if (m_sourcePath != path) {
        m_sourcePath = path;
        emit configChanged();
    }
}

void SerialDevice::setSourceHost(const QString &host)
{
    if (m_sourceHost != host) {
        m_sourceHost = host;
        emit configChanged();
    }
}

void SerialDevice::setSourceService(const QString &service)
{
    if (m_sourceService != service) {
        m_sourceService = service;
        emit configChanged();
    }
}

void SerialDevice::setTargetPort(const QString &port)
{
    if (m_targetPort != port) {
        m_targetPort = port;
        emit configChanged();
    }
}

void SerialDevice::setTargetName(const QString &name)
{
    if (m_targetName != name) {
        m_targetName = name;
        emit configChanged();
    }
}

void SerialDevice::setAddress(const DeviceAddress &addr)
{
    if (m_addr.type != addr.type || m_addr.toString() != addr.toString()) {
        m_addr = addr;
        emit configChanged();
    }
}

QString SerialDevice::toXML() const
{
    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("serial");
    writer.writeAttribute("type", charDeviceTypeToString(m_charDeviceType));

    if (!m_alias.isEmpty()) {
        writer.writeStartElement("alias");
        writer.writeAttribute("name", m_alias);
        writer.writeEndElement();
    }

    if (m_charDeviceType == CharDeviceType::PTY) {
        writer.writeStartElement("source");
        writer.writeAttribute("mode", "pty");
        writer.writeEndElement();
    } else if (m_charDeviceType == CharDeviceType::Dev || 
               m_charDeviceType == CharDeviceType::File ||
               m_charDeviceType == CharDeviceType::Pipe) {
        writer.writeStartElement("source");
        writer.writeAttribute("path", m_sourcePath);
        writer.writeEndElement();
    } else if (m_charDeviceType == CharDeviceType::TCP ||
               m_charDeviceType == CharDeviceType::UDP) {
        writer.writeStartElement("source");
        writer.writeAttribute("mode", "bind");
        writer.writeAttribute("host", m_sourceHost);
        writer.writeAttribute("service", m_sourceService);
        writer.writeEndElement();
    } else if (m_charDeviceType == CharDeviceType::Unix) {
        writer.writeStartElement("source");
        writer.writeAttribute("mode", "bind");
        writer.writeAttribute("path", m_sourcePath);
        writer.writeEndElement();
    } else if (m_charDeviceType == CharDeviceType::SpiceVMC ||
               m_charDeviceType == CharDeviceType::SpicePort) {
        writer.writeStartElement("source");
        writer.writeAttribute("channel", m_targetName);
        writer.writeEndElement();
    }

    writer.writeStartElement("target");
    writer.writeAttribute("port", QString::number(m_port));
    if (!m_targetName.isEmpty()) {
        writer.writeAttribute("name", m_targetName);
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
        case DeviceAddress::AddressType::ISA:
            writer.writeAttribute("type", "isa");
            if (!m_addr.iobase.isEmpty()) {
                writer.writeAttribute("iobase", m_addr.iobase);
            }
            if (!m_addr.irq.isEmpty()) {
                writer.writeAttribute("irq", m_addr.irq);
            }
            break;
        default:
            break;
        }
        writer.writeEndElement();
    }

    writer.writeEndElement();
    return xml;
}

bool SerialDevice::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("serial")) {
                QString typeStr = reader.attributes().value("type").toString();
                m_charDeviceType = stringToCharDeviceType(typeStr);
                
                QString alias = reader.attributes().value("alias").toString();
                if (!alias.isEmpty()) {
                    m_alias = alias;
                }
            } else if (reader.name() == QLatin1String("alias")) {
                m_alias = reader.attributes().value("name").toString();
            } else if (reader.name() == QLatin1String("source")) {
                m_sourcePath = reader.attributes().value("path").toString();
                m_sourceHost = reader.attributes().value("host").toString();
                m_sourceService = reader.attributes().value("service").toString();
                m_targetName = reader.attributes().value("channel").toString();
            } else if (reader.name() == QLatin1String("target")) {
                m_port = reader.attributes().value("port").toString().toInt();
                m_targetName = reader.attributes().value("name").toString();
            } else if (reader.name() == QLatin1String("address")) {
                QString typeStr = reader.attributes().value("type").toString();
                if (typeStr == QLatin1String("pci")) {
                    m_addr.type = DeviceAddress::AddressType::PCI;
                    m_addr.domain = reader.attributes().value("domain").toString().toUInt(nullptr, 16);
                    m_addr.pciBus = reader.attributes().value("bus").toString().toUInt(nullptr, 16);
                    m_addr.slot = reader.attributes().value("slot").toString().toUInt(nullptr, 16);
                    m_addr.function = reader.attributes().value("function").toString().toUInt(nullptr, 16);
                } else if (typeStr == QLatin1String("isa")) {
                    m_addr.type = DeviceAddress::AddressType::ISA;
                    m_addr.iobase = reader.attributes().value("iobase").toString();
                    m_addr.irq = reader.attributes().value("irq").toString();
                }
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("serial")) {
            break;
        }
    }
    
    return !reader.hasError();
}

QString SerialDevice::description() const
{
    QString desc = QString::fromLatin1("Serial (%1)").arg(charDeviceTypeToString(m_charDeviceType));
    
    if (!m_sourcePath.isEmpty()) {
        desc += QString::fromLatin1(": %1").arg(m_sourcePath);
    } else if (!m_sourceHost.isEmpty()) {
        desc += QString::fromLatin1(": %1:%2").arg(m_sourceHost).arg(m_sourceService);
    }
    
    return desc;
}

QString SerialDevice::charDeviceTypeToString(CharDeviceType type)
{
    switch (type) {
        case CharDeviceType::PTY: return QStringLiteral("pty");
        case CharDeviceType::Dev: return QStringLiteral("dev");
        case CharDeviceType::File: return QStringLiteral("file");
        case CharDeviceType::Pipe: return QStringLiteral("pipe");
        case CharDeviceType::TCP: return QStringLiteral("tcp");
        case CharDeviceType::UDP: return QStringLiteral("udp");
        case CharDeviceType::Unix: return QStringLiteral("unix");
        case CharDeviceType::SpiceVMC: return QStringLiteral("spicevmc");
        case CharDeviceType::SpicePort: return QStringLiteral("spiceport");
        case CharDeviceType::Nmdm: return QStringLiteral("nmdm");
        default: return QStringLiteral("pty");
    }
}

SerialDevice::CharDeviceType SerialDevice::stringToCharDeviceType(const QString &str)
{
    if (str == QLatin1String("dev")) return CharDeviceType::Dev;
    if (str == QLatin1String("file")) return CharDeviceType::File;
    if (str == QLatin1String("pipe")) return CharDeviceType::Pipe;
    if (str == QLatin1String("tcp")) return CharDeviceType::TCP;
    if (str == QLatin1String("udp")) return CharDeviceType::UDP;
    if (str == QLatin1String("unix")) return CharDeviceType::Unix;
    if (str == QLatin1String("spicevmc")) return CharDeviceType::SpiceVMC;
    if (str == QLatin1String("spiceport")) return CharDeviceType::SpicePort;
    if (str == QLatin1String("nmdm")) return CharDeviceType::Nmdm;
    return CharDeviceType::PTY;
}

} // namespace QVirt
