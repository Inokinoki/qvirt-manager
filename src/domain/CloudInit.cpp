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

#include "CloudInit.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

CloudInit::CloudInit(QObject *parent)
    : BaseObject(parent)
{
}

void CloudInit::setUserData(const QString &data)
{
    if (m_userData != data) {
        m_userData = data;
        emit configChanged();
    }
}

void CloudInit::setNetworkData(const QString &data)
{
    if (m_networkData != data) {
        m_networkData = data;
        emit configChanged();
    }
}

void CloudInit::setMetaData(const QString &data)
{
    if (m_metaData != data) {
        m_metaData = data;
        emit configChanged();
    }
}

void CloudInit::setVolumePath(const QString &path)
{
    if (m_volumePath != path) {
        m_volumePath = path;
        emit configChanged();
    }
}

void CloudInit::setGenerateIso(bool generate)
{
    if (m_generateIso != generate) {
        m_generateIso = generate;
        emit configChanged();
    }
}

QString CloudInit::toXML() const
{
    if (isEmpty()) {
        return QString();
    }

    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("cloudInit");

    if (!m_userData.isEmpty()) {
        writer.writeTextElement("userData", m_userData);
    }
    if (!m_networkData.isEmpty()) {
        writer.writeTextElement("networkData", m_networkData);
    }
    if (!m_metaData.isEmpty()) {
        writer.writeTextElement("metaData", m_metaData);
    }
    if (!m_volumePath.isEmpty()) {
        writer.writeTextElement("volumePath", m_volumePath);
    }
    if (m_generateIso) {
        writer.writeEmptyElement("generateIso");
    }

    writer.writeEndElement();
    return xml;
}

bool CloudInit::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("userData")) {
                m_userData = reader.readElementText();
            } else if (reader.name() == QLatin1String("networkData")) {
                m_networkData = reader.readElementText();
            } else if (reader.name() == QLatin1String("metaData")) {
                m_metaData = reader.readElementText();
            } else if (reader.name() == QLatin1String("volumePath")) {
                m_volumePath = reader.readElementText();
            } else if (reader.name() == QLatin1String("generateIso")) {
                m_generateIso = true;
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("cloudInit")) {
            break;
        }
    }
    
    return !reader.hasError();
}

bool CloudInit::isEmpty() const
{
    return m_userData.isEmpty() && m_networkData.isEmpty() &&
           m_metaData.isEmpty() && m_volumePath.isEmpty();
}

} // namespace QVirt
