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

#include "MemoryBacking.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

//=============================================================================
// Hugepage
//=============================================================================

Hugepage::Hugepage() = default;

Hugepage::Hugepage(const Hugepage &other)
    : size(other.size)
    , unit(other.unit)
    , nodeset(other.nodeset)
{
}

Hugepage &Hugepage::operator=(const Hugepage &other)
{
    if (this != &other) {
        size = other.size;
        unit = other.unit;
        nodeset = other.nodeset;
    }
    return *this;
}

bool Hugepage::isEmpty() const
{
    return size == 0 && unit.isEmpty();
}

//=============================================================================
// MemoryBacking
//=============================================================================

MemoryBacking::MemoryBacking(QObject *parent)
    : BaseObject(parent)
{
}

void MemoryBacking::setLocked(bool locked)
{
    if (m_locked != locked) {
        m_locked = locked;
        emit configChanged();
    }
}

void MemoryBacking::setShared(bool shared)
{
    if (m_shared != shared) {
        m_shared = shared;
        emit configChanged();
    }
}

void MemoryBacking::setDiscard(bool discard)
{
    if (m_discard != discard) {
        m_discard = discard;
        emit configChanged();
    }
}

void MemoryBacking::setHugepage(const Hugepage &page)
{
    m_hugepage = page;
    emit configChanged();
}

QString MemoryBacking::toXML() const
{
    if (isEmpty()) {
        return QString();
    }

    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("memoryBacking");

    if (m_locked) {
        writer.writeEmptyElement("locked");
    }

    if (m_shared) {
        writer.writeEmptyElement("shared");
    }

    if (m_discard) {
        writer.writeEmptyElement("discard");
    }

    if (!m_hugepage.isEmpty()) {
        writer.writeStartElement("hugepages");
        writer.writeStartElement("page");
        if (m_hugepage.size > 0) {
            writer.writeAttribute("size", QString::number(m_hugepage.size));
        }
        if (!m_hugepage.unit.isEmpty()) {
            writer.writeAttribute("unit", m_hugepage.unit);
        }
        if (!m_hugepage.nodeset.isEmpty()) {
            writer.writeAttribute("nodeset", m_hugepage.nodeset);
        }
        writer.writeEndElement();
        writer.writeEndElement();
    }

    writer.writeEndElement();
    return xml;
}

bool MemoryBacking::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("locked")) {
                m_locked = true;
            } else if (reader.name() == QLatin1String("shared")) {
                m_shared = true;
            } else if (reader.name() == QLatin1String("discard")) {
                m_discard = true;
            } else if (reader.name() == QLatin1String("page")) {
                m_hugepage.size = reader.attributes().value("size").toString().toULongLong();
                m_hugepage.unit = reader.attributes().value("unit").toString();
                m_hugepage.nodeset = reader.attributes().value("nodeset").toString();
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("memoryBacking")) {
            break;
        }
    }
    
    return !reader.hasError();
}

bool MemoryBacking::isEmpty() const
{
    return !m_locked && !m_shared && !m_discard && m_hugepage.isEmpty();
}

} // namespace QVirt
