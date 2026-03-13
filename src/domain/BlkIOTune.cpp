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

#include "BlkIOTune.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

BlkIOTune::BlkIOTune(QObject *parent)
    : BaseObject(parent)
{
}

void BlkIOTune::setWeight(unsigned int weight)
{
    if (m_weight != weight) {
        m_weight = weight;
        emit configChanged();
    }
}

void BlkIOTune::setReadBytesSec(unsigned long long bytes)
{
    if (m_readBytesSec != bytes) {
        m_readBytesSec = bytes;
        emit configChanged();
    }
}

void BlkIOTune::setWriteBytesSec(unsigned long long bytes)
{
    if (m_writeBytesSec != bytes) {
        m_writeBytesSec = bytes;
        emit configChanged();
    }
}

void BlkIOTune::setReadIopsSec(unsigned long long iops)
{
    if (m_readIopsSec != iops) {
        m_readIopsSec = iops;
        emit configChanged();
    }
}

void BlkIOTune::setWriteIopsSec(unsigned long long iops)
{
    if (m_writeIopsSec != iops) {
        m_writeIopsSec = iops;
        emit configChanged();
    }
}

QString BlkIOTune::toXML() const
{
    if (isEmpty()) {
        return QString();
    }

    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("blkio");

    if (m_weight > 0) {
        writer.writeTextElement("weight", QString::number(m_weight));
    }
    if (m_readBytesSec > 0) {
        writer.writeTextElement("read_bytes_sec", QString::number(m_readBytesSec));
    }
    if (m_writeBytesSec > 0) {
        writer.writeTextElement("write_bytes_sec", QString::number(m_writeBytesSec));
    }
    if (m_readIopsSec > 0) {
        writer.writeTextElement("read_iops_sec", QString::number(m_readIopsSec));
    }
    if (m_writeIopsSec > 0) {
        writer.writeTextElement("write_iops_sec", QString::number(m_writeIopsSec));
    }

    writer.writeEndElement();
    return xml;
}

bool BlkIOTune::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("weight")) {
                m_weight = reader.readElementText().toUInt();
            } else if (reader.name() == QLatin1String("read_bytes_sec")) {
                m_readBytesSec = reader.readElementText().toULongLong();
            } else if (reader.name() == QLatin1String("write_bytes_sec")) {
                m_writeBytesSec = reader.readElementText().toULongLong();
            } else if (reader.name() == QLatin1String("read_iops_sec")) {
                m_readIopsSec = reader.readElementText().toULongLong();
            } else if (reader.name() == QLatin1String("write_iops_sec")) {
                m_writeIopsSec = reader.readElementText().toULongLong();
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("blkio")) {
            break;
        }
    }
    
    return !reader.hasError();
}

bool BlkIOTune::isEmpty() const
{
    return m_weight == 0 && m_readBytesSec == 0 && m_writeBytesSec == 0 &&
           m_readIopsSec == 0 && m_writeIopsSec == 0;
}

} // namespace QVirt
