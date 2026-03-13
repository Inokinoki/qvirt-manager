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

#include "Features.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

Features::Features(QObject *parent)
    : BaseObject(parent)
{
}

void Features::setAcpi(bool enabled)
{
    if (m_acpi != enabled) {
        m_acpi = enabled;
        emit configChanged();
    }
}

void Features::setApic(bool enabled)
{
    if (m_apic != enabled) {
        m_apic = enabled;
        emit configChanged();
    }
}

void Features::setPae(bool enabled)
{
    if (m_pae != enabled) {
        m_pae = enabled;
        emit configChanged();
    }
}

void Features::setHyperv(bool enabled)
{
    if (m_hyperv != enabled) {
        m_hyperv = enabled;
        emit configChanged();
    }
}

void Features::setSmm(bool enabled)
{
    if (m_smm != enabled) {
        m_smm = enabled;
        emit configChanged();
    }
}

QString Features::toXML() const
{
    if (isEmpty()) {
        return QString();
    }

    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("features");

    if (!m_acpi) {
        writer.writeStartElement("acpi");
        writer.writeAttribute("state", "off");
        writer.writeEndElement();
    }

    if (!m_apic) {
        writer.writeStartElement("apic");
        writer.writeAttribute("state", "off");
        writer.writeEndElement();
    }

    if (m_pae) {
        writer.writeEmptyElement("pae");
    }

    if (m_hyperv) {
        writer.writeEmptyElement("hyperv");
    }

    if (m_smm) {
        writer.writeStartElement("smm");
        writer.writeAttribute("state", "on");
        writer.writeEndElement();
    }

    writer.writeEndElement();
    return xml;
}

bool Features::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("acpi")) {
                QString state = reader.attributes().value("state").toString();
                m_acpi = (state != QLatin1String("off"));
            } else if (reader.name() == QLatin1String("apic")) {
                QString state = reader.attributes().value("state").toString();
                m_apic = (state != QLatin1String("off"));
            } else if (reader.name() == QLatin1String("pae")) {
                m_pae = true;
            } else if (reader.name() == QLatin1String("hyperv")) {
                m_hyperv = true;
            } else if (reader.name() == QLatin1String("smm")) {
                QString state = reader.attributes().value("state").toString();
                m_smm = (state == QLatin1String("on"));
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("features")) {
            break;
        }
    }
    
    return !reader.hasError();
}

bool Features::isEmpty() const
{
    return m_acpi && m_apic && !m_pae && !m_hyperv && !m_smm;
}

} // namespace QVirt
