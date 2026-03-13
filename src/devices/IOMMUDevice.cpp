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

#include "IOMMUDevice.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

IOMMUDevice::IOMMUDevice(QObject *parent)
    : Device(parent)
{
}

void IOMMUDevice::setModel(IOMMUModel model)
{
    if (m_model != model) {
        m_model = model;
        emit configChanged();
    }
}

void IOMMUDevice::setInterruptRemapping(bool enabled)
{
    if (m_interruptRemapping != enabled) {
        m_interruptRemapping = enabled;
        emit configChanged();
    }
}

void IOMMUDevice::setCachingMode(bool enabled)
{
    if (m_cachingMode != enabled) {
        m_cachingMode = enabled;
        emit configChanged();
    }
}

void IOMMUDevice::setAwBits(bool enabled)
{
    if (m_awBits != enabled) {
        m_awBits = enabled;
        emit configChanged();
    }
}

void IOMMUDevice::setAddress(const DeviceAddress &addr)
{
    if (m_addr.type != addr.type || m_addr.toString() != addr.toString()) {
        m_addr = addr;
        emit configChanged();
    }
}

QString IOMMUDevice::toXML() const
{
    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("iommu");
    writer.writeAttribute("model", modelToString(m_model));

    if (!m_alias.isEmpty()) {
        writer.writeStartElement("alias");
        writer.writeAttribute("name", m_alias);
        writer.writeEndElement();
    }

    if (m_model == IOMMUModel::Intel) {
        writer.writeStartElement("intel");
        writer.writeAttribute("on", "on");
        if (m_interruptRemapping) {
            writer.writeAttribute("intr_remapping", "on");
        }
        if (m_cachingMode) {
            writer.writeAttribute("caching_mode", "on");
        }
        if (m_awBits) {
            writer.writeAttribute("aw_bits", "48");
        }
        writer.writeEndElement();
    } else if (m_model == IOMMUModel::AMD) {
        writer.writeStartElement("amd");
        writer.writeAttribute("on", "on");
        if (m_interruptRemapping) {
            writer.writeAttribute("intr_remapping", "on");
        }
        writer.writeEndElement();
    } else if (m_model == IOMMUModel::SMMUV3) {
        writer.writeStartElement("smmuv3");
        writer.writeAttribute("on", "on");
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

bool IOMMUDevice::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("iommu")) {
                QString modelStr = reader.attributes().value("model").toString();
                m_model = stringToModel(modelStr);
                
                QString alias = reader.attributes().value("alias").toString();
                if (!alias.isEmpty()) {
                    m_alias = alias;
                }
            } else if (reader.name() == QLatin1String("alias")) {
                m_alias = reader.attributes().value("name").toString();
            } else if (reader.name() == QLatin1String("intel")) {
                QString intr = reader.attributes().value("intr_remapping").toString();
                m_interruptRemapping = (intr != QLatin1String("off"));
                QString caching = reader.attributes().value("caching_mode").toString();
                m_cachingMode = (caching == QLatin1String("on"));
                QString aw = reader.attributes().value("aw_bits").toString();
                m_awBits = (aw == QLatin1String("48"));
            } else if (reader.name() == QLatin1String("amd")) {
                QString intr = reader.attributes().value("intr_remapping").toString();
                m_interruptRemapping = (intr != QLatin1String("off"));
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
        
        if (reader.isEndElement() && reader.name() == QLatin1String("iommu")) {
            break;
        }
    }
    
    return !reader.hasError();
}

QString IOMMUDevice::description() const
{
    return QString::fromLatin1("IOMMU (%1)").arg(modelToString(m_model));
}

QString IOMMUDevice::modelToString(IOMMUModel model)
{
    switch (model) {
        case IOMMUModel::Intel: return QStringLiteral("intel");
        case IOMMUModel::AMD: return QStringLiteral("amd");
        case IOMMUModel::SMMUV3: return QStringLiteral("smmuv3");
        case IOMMUModel::S390: return QStringLiteral("s390");
        default: return QStringLiteral("intel");
    }
}

IOMMUDevice::IOMMUModel IOMMUDevice::stringToModel(const QString &str)
{
    if (str == QLatin1String("amd")) return IOMMUModel::AMD;
    if (str == QLatin1String("smmuv3")) return IOMMUModel::SMMUV3;
    if (str == QLatin1String("s390")) return IOMMUModel::S390;
    return IOMMUModel::Intel;
}

} // namespace QVirt
