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

#include "LaunchSecurity.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

LaunchSecurity::LaunchSecurity(QObject *parent)
    : BaseObject(parent)
{
}

void LaunchSecurity::setType(SecurityType type)
{
    if (m_type != type) {
        m_type = type;
        emit configChanged();
    }
}

void LaunchSecurity::setPolicy(const QString &policy)
{
    if (m_policy != policy) {
        m_policy = policy;
        emit configChanged();
    }
}

void LaunchSecurity::setDhCert(const QString &cert)
{
    if (m_dhCert != cert) {
        m_dhCert = cert;
        emit configChanged();
    }
}

void LaunchSecurity::setSessionData(const QString &data)
{
    if (m_sessionData != data) {
        m_sessionData = data;
        emit configChanged();
    }
}

void LaunchSecurity::setSecret(const QString &secret)
{
    if (m_secret != secret) {
        m_secret = secret;
        emit configChanged();
    }
}

void LaunchSecurity::setAmdSme(bool enabled)
{
    if (m_amdSme != enabled) {
        m_amdSme = enabled;
        emit configChanged();
    }
}

QString LaunchSecurity::toXML() const
{
    if (isEmpty()) {
        return QString();
    }

    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("launchSecurity");
    writer.writeAttribute("type", typeToString(m_type));

    if (m_type == SecurityType::SEV || m_type == SecurityType::SEVES ||
        m_type == SecurityType::SEVSNP) {
        writer.writeStartElement("sev");
        if (!m_policy.isEmpty()) {
            writer.writeAttribute("policy", m_policy);
        }
        if (!m_dhCert.isEmpty()) {
            writer.writeAttribute("dhCert", m_dhCert);
        }
        if (!m_sessionData.isEmpty()) {
            writer.writeAttribute("sessionData", m_sessionData);
        }
        if (!m_secret.isEmpty()) {
            writer.writeAttribute("secret", m_secret);
        }
        if (m_amdSme) {
            writer.writeAttribute("amdSme", "yes");
        }
        writer.writeEndElement();
    } else if (m_type == SecurityType::TDX) {
        writer.writeEmptyElement("tdx");
    }

    writer.writeEndElement();
    return xml;
}

bool LaunchSecurity::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("launchSecurity")) {
                QString typeStr = reader.attributes().value("type").toString();
                m_type = stringToType(typeStr);
            } else if (reader.name() == QLatin1String("sev")) {
                m_policy = reader.attributes().value("policy").toString();
                m_dhCert = reader.attributes().value("dhCert").toString();
                m_sessionData = reader.attributes().value("sessionData").toString();
                m_secret = reader.attributes().value("secret").toString();
                QString sme = reader.attributes().value("amdSme").toString();
                m_amdSme = (sme == QLatin1String("yes"));
            } else if (reader.name() == QLatin1String("tdx")) {
                m_type = SecurityType::TDX;
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("launchSecurity")) {
            break;
        }
    }
    
    return !reader.hasError();
}

bool LaunchSecurity::isEmpty() const
{
    return m_type == SecurityType::None;
}

QString LaunchSecurity::typeToString(SecurityType type)
{
    switch (type) {
        case SecurityType::SEV: return QStringLiteral("sev");
        case SecurityType::SEVES: return QStringLiteral("sev-es");
        case SecurityType::SEVSNP: return QStringLiteral("sev-snp");
        case SecurityType::TDX: return QStringLiteral("tdx");
        default: return QString();
    }
}

LaunchSecurity::SecurityType LaunchSecurity::stringToType(const QString &str)
{
    if (str == QLatin1String("sev")) return SecurityType::SEV;
    if (str == QLatin1String("sev-es")) return SecurityType::SEVES;
    if (str == QLatin1String("sev-snp")) return SecurityType::SEVSNP;
    if (str == QLatin1String("tdx")) return SecurityType::TDX;
    return SecurityType::None;
}

} // namespace QVirt
