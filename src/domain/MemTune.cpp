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

#include "MemTune.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

MemTune::MemTune(QObject *parent)
    : BaseObject(parent)
{
}

void MemTune::setHardLimit(unsigned long long limit)
{
    if (m_hardLimit != limit) {
        m_hardLimit = limit;
        emit configChanged();
    }
}

void MemTune::setSoftLimit(unsigned long long limit)
{
    if (m_softLimit != limit) {
        m_softLimit = limit;
        emit configChanged();
    }
}

void MemTune::setSwapHardLimit(unsigned long long limit)
{
    if (m_swapHardLimit != limit) {
        m_swapHardLimit = limit;
        emit configChanged();
    }
}

QString MemTune::toXML() const
{
    if (isEmpty()) {
        return QString();
    }

    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("memtune");

    if (m_hardLimit > 0) {
        writer.writeTextElement("hard_limit", QString::number(m_hardLimit));
    }
    if (m_softLimit > 0) {
        writer.writeTextElement("soft_limit", QString::number(m_softLimit));
    }
    if (m_swapHardLimit > 0) {
        writer.writeTextElement("swap_hard_limit", QString::number(m_swapHardLimit));
    }

    writer.writeEndElement();
    return xml;
}

bool MemTune::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("hard_limit")) {
                m_hardLimit = reader.readElementText().toULongLong();
            } else if (reader.name() == QLatin1String("soft_limit")) {
                m_softLimit = reader.readElementText().toULongLong();
            } else if (reader.name() == QLatin1String("swap_hard_limit")) {
                m_swapHardLimit = reader.readElementText().toULongLong();
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("memtune")) {
            break;
        }
    }
    
    return !reader.hasError();
}

bool MemTune::isEmpty() const
{
    return m_hardLimit == 0 && m_softLimit == 0 && m_swapHardLimit == 0;
}

} // namespace QVirt
