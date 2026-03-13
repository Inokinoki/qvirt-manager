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

#include "Firmware.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

Firmware::Firmware(QObject *parent)
    : BaseObject(parent)
{
}

void Firmware::setType(FirmwareType type)
{
    if (m_type != type) {
        m_type = type;
        emit configChanged();
    }
}

void Firmware::setBiosType(BiosType type)
{
    if (m_biosType != type) {
        m_biosType = type;
        emit configChanged();
    }
}

void Firmware::setSecureBoot(bool enabled)
{
    if (m_secureBoot != enabled) {
        m_secureBoot = enabled;
        emit configChanged();
    }
}

void Firmware::setLoader(const QString &path)
{
    if (m_loader != path) {
        m_loader = path;
        emit configChanged();
    }
}

void Firmware::setNvram(const QString &path)
{
    if (m_nvram != path) {
        m_nvram = path;
        emit configChanged();
    }
}

void Firmware::setNvramTemplate(const QString &path)
{
    if (m_nvramTemplate != path) {
        m_nvramTemplate = path;
        emit configChanged();
    }
}

void Firmware::setSmm(bool enabled)
{
    if (m_smm != enabled) {
        m_smm = enabled;
        emit configChanged();
    }
}

QString Firmware::toXML() const
{
    if (isEmpty()) {
        return QString();
    }

    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("os");

    if (m_type == FirmwareType::UEFI || m_type == FirmwareType::Custom) {
        writer.writeStartElement("firmware");
        if (m_secureBoot) {
            writer.writeAttribute("secureBoot", "enabled");
        }
        writer.writeEndElement();
    }

    if (m_type == FirmwareType::UEFI || !m_loader.isEmpty()) {
        writer.writeStartElement("loader");
        if (m_type == FirmwareType::UEFI) {
            writer.writeAttribute("type", "pflash");
        }
        if (!m_loader.isEmpty()) {
            writer.writeCharacters(m_loader);
        }
        writer.writeEndElement();
    }

    if (!m_nvram.isEmpty() || !m_nvramTemplate.isEmpty()) {
        writer.writeStartElement("nvram");
        if (!m_nvram.isEmpty()) {
            writer.writeCharacters(m_nvram);
        }
        if (!m_nvramTemplate.isEmpty()) {
            writer.writeAttribute("template", m_nvramTemplate);
        }
        writer.writeEndElement();
    }

    if (m_smm) {
        writer.writeStartElement("features");
        writer.writeStartElement("smm");
        writer.writeAttribute("state", "on");
        writer.writeEndElement();
        writer.writeEndElement();
    }

    writer.writeEndElement();
    return xml;
}

bool Firmware::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("firmware")) {
                QString secureBoot = reader.attributes().value("secureBoot").toString();
                m_secureBoot = (secureBoot == QLatin1String("enabled"));
            } else if (reader.name() == QLatin1String("loader")) {
                QString type = reader.attributes().value("type").toString();
                if (type == QLatin1String("pflash")) {
                    m_type = FirmwareType::UEFI;
                }
                m_loader = reader.readElementText();
            } else if (reader.name() == QLatin1String("nvram")) {
                m_nvram = reader.readElementText();
                m_nvramTemplate = reader.attributes().value("template").toString();
            } else if (reader.name() == QLatin1String("smm")) {
                QString state = reader.attributes().value("state").toString();
                m_smm = (state == QLatin1String("on"));
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("os")) {
            break;
        }
    }
    
    return !reader.hasError();
}

bool Firmware::isEmpty() const
{
    return m_type == FirmwareType::BIOS && !m_secureBoot && !m_smm &&
           m_loader.isEmpty() && m_nvram.isEmpty() && m_nvramTemplate.isEmpty();
}

QString Firmware::typeToString(FirmwareType type)
{
    switch (type) {
        case FirmwareType::UEFI: return QStringLiteral("uefi");
        case FirmwareType::Custom: return QStringLiteral("custom");
        default: return QStringLiteral("bios");
    }
}

Firmware::FirmwareType Firmware::stringToType(const QString &str)
{
    if (str == QLatin1String("uefi")) return FirmwareType::UEFI;
    if (str == QLatin1String("custom")) return FirmwareType::Custom;
    return FirmwareType::BIOS;
}

QString Firmware::biosTypeToString(BiosType type)
{
    switch (type) {
        case BiosType::OVMF: return QStringLiteral("ovmf");
        case BiosType::EDK2: return QStringLiteral("edk2");
        default: return QStringLiteral("seabios");
    }
}

Firmware::BiosType Firmware::stringToBiosType(const QString &str)
{
    if (str == QLatin1String("ovmf")) return BiosType::OVMF;
    if (str == QLatin1String("edk2")) return BiosType::EDK2;
    return BiosType::SeaBIOS;
}

} // namespace QVirt
