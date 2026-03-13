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

#include "DomainCheckpoint.h"
#include "Connection.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#ifdef LIBVIRT_FOUND
#include <libvirt/libvirt.h>
#endif

namespace QVirt {

DomainCheckpoint::DomainCheckpoint(Connection *conn, void *checkpointHandle, QObject *parent)
    : BaseObject(parent)
#ifdef LIBVIRT_FOUND
    , m_checkpointHandle(checkpointHandle)
#endif
{
    Q_UNUSED(conn);
}

DomainCheckpoint::DomainCheckpoint(QObject *parent)
    : BaseObject(parent)
{
}

DomainCheckpoint::~DomainCheckpoint()
{
#ifdef LIBVIRT_FOUND
    if (m_checkpointHandle) {
        virDomainCheckpointFree(reinterpret_cast<virDomainCheckpointPtr>(m_checkpointHandle));
        m_checkpointHandle = nullptr;
    }
#endif
}

void DomainCheckpoint::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit stateChanged();
    }
}

void DomainCheckpoint::setDescription(const QString &desc)
{
    if (m_description != desc) {
        m_description = desc;
        emit stateChanged();
    }
}

void DomainCheckpoint::setCreationTime(const QDateTime &time)
{
    if (m_creationTime != time) {
        m_creationTime = time;
        emit stateChanged();
    }
}

void DomainCheckpoint::setParentName(const QString &name)
{
    if (m_parentName != name) {
        m_parentName = name;
        emit stateChanged();
    }
}

void DomainCheckpoint::setReverting(bool reverting)
{
    if (m_reverting != reverting) {
        m_reverting = reverting;
        emit stateChanged();
    }
}

QString DomainCheckpoint::toXML() const
{
    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("domainCheckpoint");

    writer.writeTextElement("name", m_name);

    if (!m_description.isEmpty()) {
        writer.writeTextElement("description", m_description);
    }

    if (m_creationTime.isValid()) {
        writer.writeTextElement("creationTime", m_creationTime.toString(Qt::ISODate));
    }

    if (!m_parentName.isEmpty()) {
        writer.writeTextElement("parent", m_parentName);
    }

    writer.writeEndElement();
    return xml;
}

bool DomainCheckpoint::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("name")) {
                m_name = reader.readElementText();
            } else if (reader.name() == QLatin1String("description")) {
                m_description = reader.readElementText();
            } else if (reader.name() == QLatin1String("creationTime")) {
                m_creationTime = QDateTime::fromString(reader.readElementText(), Qt::ISODate);
            } else if (reader.name() == QLatin1String("parent")) {
                m_parentName = reader.readElementText();
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("domainCheckpoint")) {
            break;
        }
    }
    
    return !reader.hasError();
}

} // namespace QVirt
