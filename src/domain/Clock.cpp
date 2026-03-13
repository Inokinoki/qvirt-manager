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

#include "Clock.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

//=============================================================================
// Timer
//=============================================================================

Timer::Timer() = default;

Timer::Timer(const Timer &other)
    : name(other.name)
    , tickPolicy(other.tickPolicy)
    , track(other.track)
    , present(other.present)
{
}

Timer &Timer::operator=(const Timer &other)
{
    if (this != &other) {
        name = other.name;
        tickPolicy = other.tickPolicy;
        track = other.track;
        present = other.present;
    }
    return *this;
}

bool Timer::isEmpty() const
{
    return name.isEmpty();
}

//=============================================================================
// Clock
//=============================================================================

Clock::Clock(QObject *parent)
    : BaseObject(parent)
{
}

void Clock::setOffset(const QString &offset)
{
    if (m_offset != offset) {
        m_offset = offset;
        emit configChanged();
    }
}

void Clock::setRtc(const Timer &timer)
{
    m_rtc = timer;
    emit configChanged();
}

void Clock::setHpet(const Timer &timer)
{
    m_hpet = timer;
    emit configChanged();
}

void Clock::setPit(const Timer &timer)
{
    m_pit = timer;
    emit configChanged();
}

void Clock::setKvmClock(bool enabled)
{
    if (m_kvmClock != enabled) {
        m_kvmClock = enabled;
        emit configChanged();
    }
}

QString Clock::toXML() const
{
    if (isEmpty()) {
        return QString();
    }

    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("clock");
    writer.writeAttribute("offset", m_offset);

    if (!m_rtc.isEmpty()) {
        writer.writeStartElement("timer");
        writer.writeAttribute("name", "rtc");
        if (!m_rtc.tickPolicy.isEmpty()) {
            writer.writeAttribute("tickpolicy", m_rtc.tickPolicy);
        }
        if (!m_rtc.track.isEmpty()) {
            writer.writeAttribute("track", m_rtc.track);
        }
        writer.writeEndElement();
    }

    if (!m_hpet.isEmpty()) {
        writer.writeStartElement("timer");
        writer.writeAttribute("name", "hpet");
        if (!m_hpet.tickPolicy.isEmpty()) {
            writer.writeAttribute("tickpolicy", m_hpet.tickPolicy);
        }
        if (!m_hpet.present) {
            writer.writeAttribute("present", "no");
        }
        writer.writeEndElement();
    }

    if (!m_pit.isEmpty()) {
        writer.writeStartElement("timer");
        writer.writeAttribute("name", "pit");
        if (!m_pit.tickPolicy.isEmpty()) {
            writer.writeAttribute("tickpolicy", m_pit.tickPolicy);
        }
        writer.writeEndElement();
    }

    if (!m_kvmClock) {
        writer.writeStartElement("timer");
        writer.writeAttribute("name", "kvmclock");
        writer.writeAttribute("present", "no");
        writer.writeEndElement();
    }

    writer.writeEndElement();
    return xml;
}

bool Clock::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("clock")) {
                m_offset = reader.attributes().value("offset").toString();
            } else if (reader.name() == QLatin1String("timer")) {
                QString name = reader.attributes().value("name").toString();
                Timer timer;
                timer.name = name;
                timer.tickPolicy = reader.attributes().value("tickpolicy").toString();
                timer.track = reader.attributes().value("track").toString();
                QString present = reader.attributes().value("present").toString();
                timer.present = (present != QLatin1String("no"));

                if (name == QLatin1String("rtc")) {
                    m_rtc = timer;
                } else if (name == QLatin1String("hpet")) {
                    m_hpet = timer;
                } else if (name == QLatin1String("pit")) {
                    m_pit = timer;
                } else if (name == QLatin1String("kvmclock")) {
                    m_kvmClock = timer.present;
                }
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("clock")) {
            break;
        }
    }
    
    return !reader.hasError();
}

bool Clock::isEmpty() const
{
    return m_offset == "utc" && m_rtc.isEmpty() && m_hpet.isEmpty() &&
           m_pit.isEmpty() && m_kvmClock;
}

} // namespace QVirt
