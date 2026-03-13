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

#include "IOThread.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

//=============================================================================
// IOThread
//=============================================================================

IOThread::IOThread()
{
}

IOThread::IOThread(const IOThread &other)
    : m_id(other.m_id)
    , m_cpuset(other.m_cpuset)
{
}

IOThread &IOThread::operator=(const IOThread &other)
{
    if (this != &other) {
        m_id = other.m_id;
        m_cpuset = other.m_cpuset;
    }
    return *this;
}

void IOThread::setId(int id)
{
    m_id = id;
}

void IOThread::setCpuset(const QString &cpuset)
{
    m_cpuset = cpuset;
}

QString IOThread::toXML() const
{
    if (isEmpty()) {
        return QString();
    }

    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.writeStartElement("iothread");
    writer.writeAttribute("id", QString::number(m_id));
    if (!m_cpuset.isEmpty()) {
        writer.writeAttribute("cpuset", m_cpuset);
    }
    writer.writeEndElement();
    return xml;
}

bool IOThread::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("iothread")) {
                m_id = reader.attributes().value("id").toString().toInt();
                m_cpuset = reader.attributes().value("cpuset").toString();
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("iothread")) {
            break;
        }
    }
    
    return !reader.hasError();
}

bool IOThread::isEmpty() const
{
    return m_id <= 0;
}

//=============================================================================
// IOThreadConfig
//=============================================================================

IOThreadConfig::IOThreadConfig(QObject *parent)
    : BaseObject(parent)
{
}

void IOThreadConfig::setThreads(const QList<IOThread> &threads)
{
    m_threads = threads;
    emit configChanged();
}

void IOThreadConfig::addThread(const IOThread &thread)
{
    m_threads.append(thread);
    emit configChanged();
}

void IOThreadConfig::removeThread(int id)
{
    for (int i = 0; i < m_threads.size(); ++i) {
        if (m_threads[i].id() == id) {
            m_threads.removeAt(i);
            emit configChanged();
            break;
        }
    }
}

QString IOThreadConfig::toXML() const
{
    if (isEmpty()) {
        return QString();
    }

    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("iothreads");

    for (const IOThread &thread : m_threads) {
        if (!thread.isEmpty()) {
            xml += thread.toXML();
        }
    }

    writer.writeEndElement();
    return xml;
}

bool IOThreadConfig::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("iothread")) {
                IOThread thread;
                thread.fromXML(xml);
                m_threads.append(thread);
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("iothreads")) {
            break;
        }
    }
    
    return !reader.hasError();
}

bool IOThreadConfig::isEmpty() const
{
    return m_threads.isEmpty();
}

} // namespace QVirt
