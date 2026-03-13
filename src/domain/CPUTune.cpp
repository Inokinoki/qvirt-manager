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

#include "CPUTune.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

//=============================================================================
// VCPUPin
//=============================================================================

VCPUPin::VCPUPin() = default;

VCPUPin::VCPUPin(const VCPUPin &other)
    : vcpu(other.vcpu)
    , cpuset(other.cpuset)
{
}

VCPUPin &VCPUPin::operator=(const VCPUPin &other)
{
    if (this != &other) {
        vcpu = other.vcpu;
        cpuset = other.cpuset;
    }
    return *this;
}

bool VCPUPin::isEmpty() const
{
    return cpuset.isEmpty();
}

//=============================================================================
// EmulatorPin
//=============================================================================

EmulatorPin::EmulatorPin() = default;

EmulatorPin::EmulatorPin(const EmulatorPin &other)
    : cpuset(other.cpuset)
{
}

EmulatorPin &EmulatorPin::operator=(const EmulatorPin &other)
{
    if (this != &other) {
        cpuset = other.cpuset;
    }
    return *this;
}

bool EmulatorPin::isEmpty() const
{
    return cpuset.isEmpty();
}

//=============================================================================
// IOThreadPin
//=============================================================================

IOThreadPin::IOThreadPin() = default;

IOThreadPin::IOThreadPin(const IOThreadPin &other)
    : iothread(other.iothread)
    , cpuset(other.cpuset)
{
}

IOThreadPin &IOThreadPin::operator=(const IOThreadPin &other)
{
    if (this != &other) {
        iothread = other.iothread;
        cpuset = other.cpuset;
    }
    return *this;
}

bool IOThreadPin::isEmpty() const
{
    return cpuset.isEmpty();
}

//=============================================================================
// CPUTune
//=============================================================================

CPUTune::CPUTune(QObject *parent)
    : BaseObject(parent)
{
}

void CPUTune::setShares(unsigned long long shares)
{
    if (m_shares != shares) {
        m_shares = shares;
        emit configChanged();
    }
}

void CPUTune::setPeriod(unsigned long long period)
{
    if (m_period != period) {
        m_period = period;
        emit configChanged();
    }
}

void CPUTune::setQuota(long long quota)
{
    if (m_quota != quota) {
        m_quota = quota;
        emit configChanged();
    }
}

void CPUTune::setRealtimePriority(int priority)
{
    if (m_realtimePriority != priority) {
        m_realtimePriority = priority;
        emit configChanged();
    }
}

void CPUTune::setVcpuPins(const QList<VCPUPin> &pins)
{
    m_vcpuPins = pins;
    emit configChanged();
}

void CPUTune::addVcpuPin(const VCPUPin &pin)
{
    m_vcpuPins.append(pin);
    emit configChanged();
}

void CPUTune::removeVcpuPin(int vcpu)
{
    for (int i = 0; i < m_vcpuPins.size(); ++i) {
        if (m_vcpuPins[i].vcpu == vcpu) {
            m_vcpuPins.removeAt(i);
            emit configChanged();
            break;
        }
    }
}

void CPUTune::setEmulatorPin(const EmulatorPin &pin)
{
    m_emulatorPin = pin;
    emit configChanged();
}

void CPUTune::setIothreadPins(const QList<IOThreadPin> &pins)
{
    m_iothreadPins = pins;
    emit configChanged();
}

void CPUTune::addIothreadPin(const IOThreadPin &pin)
{
    m_iothreadPins.append(pin);
    emit configChanged();
}

QString CPUTune::toXML() const
{
    if (isEmpty()) {
        return QString();
    }

    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("cputune");

    if (m_shares > 0) {
        writer.writeTextElement("shares", QString::number(m_shares));
    }
    if (m_period > 0) {
        writer.writeTextElement("period", QString::number(m_period));
    }
    if (m_quota >= 0) {
        writer.writeTextElement("quota", QString::number(m_quota));
    }
    if (m_realtimePriority >= 0) {
        writer.writeTextElement("realtime_priority", QString::number(m_realtimePriority));
    }

    for (const VCPUPin &pin : m_vcpuPins) {
        if (!pin.isEmpty()) {
            writer.writeStartElement("vcpupin");
            writer.writeAttribute("vcpu", QString::number(pin.vcpu));
            writer.writeAttribute("cpuset", pin.cpuset);
            writer.writeEndElement();
        }
    }

    if (!m_emulatorPin.isEmpty()) {
        writer.writeStartElement("emulatorpin");
        writer.writeAttribute("cpuset", m_emulatorPin.cpuset);
        writer.writeEndElement();
    }

    for (const IOThreadPin &pin : m_iothreadPins) {
        if (!pin.isEmpty()) {
            writer.writeStartElement("iothreadpin");
            writer.writeAttribute("iothread", QString::number(pin.iothread));
            writer.writeAttribute("cpuset", pin.cpuset);
            writer.writeEndElement();
        }
    }

    writer.writeEndElement();
    return xml;
}

bool CPUTune::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("shares")) {
                m_shares = reader.readElementText().toULongLong();
            } else if (reader.name() == QLatin1String("period")) {
                m_period = reader.readElementText().toULongLong();
            } else if (reader.name() == QLatin1String("quota")) {
                m_quota = reader.readElementText().toLongLong();
            } else if (reader.name() == QLatin1String("realtime_priority")) {
                m_realtimePriority = reader.readElementText().toInt();
            } else if (reader.name() == QLatin1String("vcpupin")) {
                VCPUPin pin;
                pin.vcpu = reader.attributes().value("vcpu").toString().toInt();
                pin.cpuset = reader.attributes().value("cpuset").toString();
                m_vcpuPins.append(pin);
            } else if (reader.name() == QLatin1String("emulatorpin")) {
                m_emulatorPin.cpuset = reader.attributes().value("cpuset").toString();
            } else if (reader.name() == QLatin1String("iothreadpin")) {
                IOThreadPin pin;
                pin.iothread = reader.attributes().value("iothread").toString().toInt();
                pin.cpuset = reader.attributes().value("cpuset").toString();
                m_iothreadPins.append(pin);
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("cputune")) {
            break;
        }
    }
    
    return !reader.hasError();
}

bool CPUTune::isEmpty() const
{
    if (m_shares > 0 || m_period > 0 || m_quota >= 0 || m_realtimePriority >= 0) {
        return false;
    }
    if (!m_emulatorPin.isEmpty()) {
        return false;
    }
    for (const VCPUPin &pin : m_vcpuPins) {
        if (!pin.isEmpty()) {
            return false;
        }
    }
    for (const IOThreadPin &pin : m_iothreadPins) {
        if (!pin.isEmpty()) {
            return false;
        }
    }
    return true;
}

} // namespace QVirt
