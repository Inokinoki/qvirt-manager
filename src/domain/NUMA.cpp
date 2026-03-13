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

#include "NUMA.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

//=============================================================================
// NUMACell
//=============================================================================

NUMACell::NUMACell() = default;

NUMACell::NUMACell(const NUMACell &other)
    : id(other.id)
    , memory(other.memory)
    , cpus(other.cpus)
    , memAccess(other.memAccess)
{
}

NUMACell &NUMACell::operator=(const NUMACell &other)
{
    if (this != &other) {
        id = other.id;
        memory = other.memory;
        cpus = other.cpus;
        memAccess = other.memAccess;
    }
    return *this;
}

bool NUMACell::isEmpty() const
{
    return cpus.isEmpty() && memory == 0;
}

//=============================================================================
// NUMATune
//=============================================================================

NUMATune::NUMATune() = default;

NUMATune::NUMATune(const NUMATune &other)
    : mode(other.mode)
    , placement(other.placement)
{
}

NUMATune &NUMATune::operator=(const NUMATune &other)
{
    if (this != &other) {
        mode = other.mode;
        placement = other.placement;
    }
    return *this;
}

bool NUMATune::isEmpty() const
{
    return mode.isEmpty() && placement.isEmpty();
}

//=============================================================================
// NUMA
//=============================================================================

NUMA::NUMA(QObject *parent)
    : BaseObject(parent)
{
}

void NUMA::setCells(const QList<NUMACell> &cells)
{
    m_cells = cells;
    emit configChanged();
}

void NUMA::addCell(const NUMACell &cell)
{
    m_cells.append(cell);
    emit configChanged();
}

void NUMA::setTune(const NUMATune &tune)
{
    m_tune = tune;
    emit configChanged();
}

QString NUMA::toXML() const
{
    if (isEmpty()) {
        return QString();
    }

    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("numa");

    for (const NUMACell &cell : m_cells) {
        if (!cell.isEmpty()) {
            writer.writeStartElement("cell");
            writer.writeAttribute("id", QString::number(cell.id));
            if (cell.memory > 0) {
                writer.writeAttribute("memory", QString::number(cell.memory));
            }
            if (!cell.cpus.isEmpty()) {
                writer.writeAttribute("cpus", cell.cpus);
            }
            if (!cell.memAccess.isEmpty()) {
                writer.writeAttribute("memAccess", cell.memAccess);
            }
            writer.writeEndElement();
        }
    }

    if (!m_tune.isEmpty()) {
        writer.writeStartElement("tune");
        if (!m_tune.mode.isEmpty()) {
            writer.writeAttribute("mode", m_tune.mode);
        }
        if (!m_tune.placement.isEmpty()) {
            writer.writeAttribute("placement", m_tune.placement);
        }
        writer.writeEndElement();
    }

    writer.writeEndElement();
    return xml;
}

bool NUMA::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("cell")) {
                NUMACell cell;
                cell.id = reader.attributes().value("id").toString().toInt();
                cell.memory = reader.attributes().value("memory").toString().toULongLong();
                cell.cpus = reader.attributes().value("cpus").toString();
                cell.memAccess = reader.attributes().value("memAccess").toString();
                m_cells.append(cell);
            } else if (reader.name() == QLatin1String("tune")) {
                m_tune.mode = reader.attributes().value("mode").toString();
                m_tune.placement = reader.attributes().value("placement").toString();
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("numa")) {
            break;
        }
    }
    
    return !reader.hasError();
}

bool NUMA::isEmpty() const
{
    if (!m_tune.isEmpty()) {
        return false;
    }
    for (const NUMACell &cell : m_cells) {
        if (!cell.isEmpty()) {
            return false;
        }
    }
    return true;
}

} // namespace QVirt
