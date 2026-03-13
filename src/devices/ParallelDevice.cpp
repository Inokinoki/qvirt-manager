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

#include "ParallelDevice.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

ParallelDevice::ParallelDevice(QObject *parent)
    : Device(parent)
{
}

void ParallelDevice::setPort(int port)
{
    if (m_port != port) {
        m_port = port;
        emit configChanged();
    }
}

void ParallelDevice::setParallelDeviceType(ParallelDeviceType type)
{
    if (m_parallelDeviceType != type) {
        m_parallelDeviceType = type;
        emit configChanged();
    }
}

void ParallelDevice::setSourcePath(const QString &path)
{
    if (m_sourcePath != path) {
        m_sourcePath = path;
        emit configChanged();
    }
}

void ParallelDevice::setAddress(const DeviceAddress &addr)
{
    if (m_addr.type != addr.type || m_addr.toString() != addr.toString()) {
        m_addr = addr;
        emit configChanged();
    }
}

QString ParallelDevice::toXML() const
{
    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("parallel");
    writer.writeAttribute("type", parallelDeviceTypeToString(m_parallelDeviceType));

    if (!m_alias.isEmpty()) {
        writer.writeStartElement("alias");
        writer.writeAttribute("name", m_alias);
        writer.writeEndElement();
    }

    if (m_parallelDeviceType == ParallelDeviceType::PTY) {
        writer.writeStartElement("source");
        writer.writeAttribute("mode", "pty");
        writer.writeEndElement();
    } else if (m_parallelDeviceType == ParallelDeviceType::Dev ||
               m_parallelDeviceType == ParallelDeviceType::File ||
               m_parallelDeviceType == ParallelDeviceType::Pipe) {
        writer.writeStartElement("source");
        writer.writeAttribute("path", m_sourcePath);
        writer.writeEndElement();
    }

    writer.writeStartElement("target");
    writer.writeAttribute("port", QString::number(m_port));
    writer.writeEndElement();

    if (!m_addr.isEmpty()) {
        writer.writeStartElement("address");
        switch (m_addr.type) {
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

bool ParallelDevice::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("parallel")) {
                QString typeStr = reader.attributes().value("type").toString();
                m_parallelDeviceType = stringToParallelDeviceType(typeStr);
                
                QString alias = reader.attributes().value("alias").toString();
                if (!alias.isEmpty()) {
                    m_alias = alias;
                }
            } else if (reader.name() == QLatin1String("alias")) {
                m_alias = reader.attributes().value("name").toString();
            } else if (reader.name() == QLatin1String("source")) {
                m_sourcePath = reader.attributes().value("path").toString();
            } else if (reader.name() == QLatin1String("target")) {
                m_port = reader.attributes().value("port").toString().toInt();
            } else if (reader.name() == QLatin1String("address")) {
                QString typeStr = reader.attributes().value("type").toString();
                if (typeStr == QLatin1String("isa")) {
                    m_addr.type = DeviceAddress::AddressType::ISA;
                    m_addr.iobase = reader.attributes().value("iobase").toString();
                    m_addr.irq = reader.attributes().value("irq").toString();
                }
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("parallel")) {
            break;
        }
    }
    
    return !reader.hasError();
}

QString ParallelDevice::description() const
{
    QString desc = QString::fromLatin1("Parallel (%1)").arg(parallelDeviceTypeToString(m_parallelDeviceType));
    
    if (!m_sourcePath.isEmpty()) {
        desc += QString::fromLatin1(": %1").arg(m_sourcePath);
    }
    
    return desc;
}

QString ParallelDevice::parallelDeviceTypeToString(ParallelDeviceType type)
{
    switch (type) {
        case ParallelDeviceType::PTY: return QStringLiteral("pty");
        case ParallelDeviceType::Dev: return QStringLiteral("dev");
        case ParallelDeviceType::File: return QStringLiteral("file");
        case ParallelDeviceType::Pipe: return QStringLiteral("pipe");
        default: return QStringLiteral("pty");
    }
}

ParallelDevice::ParallelDeviceType ParallelDevice::stringToParallelDeviceType(const QString &str)
{
    if (str == QLatin1String("dev")) return ParallelDeviceType::Dev;
    if (str == QLatin1String("file")) return ParallelDeviceType::File;
    if (str == QLatin1String("pipe")) return ParallelDeviceType::Pipe;
    return ParallelDeviceType::PTY;
}

} // namespace QVirt
