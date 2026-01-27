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

#include "SmartcardDevice.h"

namespace QVirt {

SmartcardDevice::SmartcardDevice(QObject *parent)
    : Device(parent)
{
}

void SmartcardDevice::setMode(SmartcardMode mode)
{
    if (m_mode != mode) {
        m_mode = mode;
        emit configChanged();
    }
}

void SmartcardDevice::setType(SmartcardType type)
{
    if (m_type != type) {
        m_type = type;
        emit configChanged();
    }
}

void SmartcardDevice::setDatabase(const QString &database)
{
    if (m_database != database) {
        m_database = database;
        emit configChanged();
    }
}

void SmartcardDevice::setSourcePath(const QString &path)
{
    if (m_sourcePath != path) {
        m_sourcePath = path;
        emit configChanged();
    }
}

QString SmartcardDevice::toXML() const
{
    QString xml = QString::fromLatin1("  <smartcard mode='%1'>\n")
        .arg(modeToString(m_mode));

    if (m_mode == SmartcardMode::Passthrough) {
        xml += QString::fromLatin1("    <source path='%1'/>\n").arg(m_sourcePath);
        xml += QString::fromLatin1("    <type>%1</type>\n").arg(typeToString(m_type));
    } else if (m_mode == SmartcardMode::HostCertificates) {
        if (!m_database.isEmpty()) {
            xml += QString::fromLatin1("    <source>%1</source>\n").arg(m_database);
        }
        xml += QString::fromLatin1("    <type>%1</type>\n").arg(typeToString(m_type));
    } else {
        xml += QString::fromLatin1("    <type>%1</type>\n").arg(typeToString(m_type));
    }

    xml += QString::fromLatin1("  </smartcard>");

    return xml;
}

bool SmartcardDevice::fromXML(const QString &xml)
{
    Q_UNUSED(xml);
    return true;
}

QString SmartcardDevice::description() const
{
    return QString::fromLatin1("%1 (%2)")
        .arg(modeToString(m_mode))
        .arg(typeToString(m_type));
}

QString SmartcardDevice::modeToString(SmartcardMode mode)
{
    switch (mode) {
        case SmartcardMode::Host: return QStringLiteral("host");
        case SmartcardMode::HostCertificates: return QStringLiteral("host-certificates");
        case SmartcardMode::Passthrough: return QStringLiteral("passthrough");
        default: return QStringLiteral("host");
    }
}

SmartcardDevice::SmartcardMode SmartcardDevice::stringToMode(const QString &str)
{
    if (str == QLatin1String("host-certificates")) return SmartcardMode::HostCertificates;
    if (str == QLatin1String("passthrough")) return SmartcardMode::Passthrough;
    return SmartcardMode::Host;
}

QString SmartcardDevice::typeToString(SmartcardType type)
{
    switch (type) {
        case SmartcardType::TCP: return QStringLiteral("tcp");
        case SmartcardType::SpiceVMC: return QStringLiteral("spicevmc");
        default: return QStringLiteral("spicevmc");
    }
}

SmartcardDevice::SmartcardType SmartcardDevice::stringToType(const QString &str)
{
    if (str == QLatin1String("tcp")) return SmartcardType::TCP;
    return SmartcardType::SpiceVMC;
}

} // namespace QVirt
