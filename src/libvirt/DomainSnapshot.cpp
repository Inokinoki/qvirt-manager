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

#include "DomainSnapshot.h"
#include "Domain.h"
#include "../core/Error.h"

#include <QDomDocument>
#include <QDebug>

namespace QVirt {

DomainSnapshot::DomainSnapshot(virDomainSnapshotPtr snapshot, Domain *domain, QObject *parent)
    : QObject(parent)
    , m_snapshot(snapshot)
    , m_domain(domain)
    , m_state(VIR_DOMAIN_SHUTOFF)
{
    if (!m_snapshot) {
        return;
    }

    // Get snapshot name
    const char *name = virDomainSnapshotGetName(m_snapshot);
    if (name) {
        m_name = QString::fromUtf8(name);
    }

    // Get and parse XML description
    char *xml = virDomainSnapshotGetXMLDesc(m_snapshot, 0);
    if (xml) {
        parseXML(QString::fromUtf8(xml));
        free(xml);
    }

    qDebug() << "Created DomainSnapshot wrapper for" << m_name;
}

DomainSnapshot::~DomainSnapshot()
{
    if (m_snapshot) {
        virDomainSnapshotFree(m_snapshot);
    }
}

QString DomainSnapshot::name() const
{
    return m_name;
}

QString DomainSnapshot::description() const
{
    return m_description;
}

virDomainState DomainSnapshot::state() const
{
    return m_state;
}

QDateTime DomainSnapshot::creationTime() const
{
    return m_creationTime;
}

bool DomainSnapshot::isCurrent() const
{
    if (!m_snapshot) {
        return false;
    }

    // Check if this is the current snapshot
    // This requires parsing snapshot XML to check current attribute
    // Reserved for future implementation
    return false;
}

bool DomainSnapshot::delete_(unsigned int flags)
{
    if (!m_snapshot) {
        return false;
    }

    return virDomainSnapshotDelete(m_snapshot, flags) == 0;
}

bool DomainSnapshot::revert(unsigned int flags)
{
    if (!m_snapshot) {
        return false;
    }

    return virDomainRevertToSnapshot(m_snapshot, flags) == 0;
}

DomainSnapshot *DomainSnapshot::parent() const
{
    // Parent lookup requires parsing snapshot XML relationships
    // Reserved for future implementation
    return nullptr;
}

QList<DomainSnapshot*> DomainSnapshot::children() const
{
    // Children lookup requires querying domain for snapshot tree
    // Reserved for future implementation
    return QList<DomainSnapshot*>();
}

QString DomainSnapshot::getXMLDesc(unsigned int flags)
{
    if (!m_snapshot) {
        return QString();
    }

    char *xml = virDomainSnapshotGetXMLDesc(m_snapshot, flags);
    if (xml) {
        QString result = QString::fromUtf8(xml);
        free(xml);
        return result;
    }

    return QString();
}

void DomainSnapshot::parseXML(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) {
        qWarning() << "Failed to parse snapshot XML";
        return;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "domainsnapshot") {
        qWarning() << "Invalid snapshot XML";
        return;
    }

    // Parse description
    QDomElement descElement = root.firstChildElement("description");
    if (!descElement.isNull()) {
        m_description = descElement.text();
    }

    // Parse state
    QDomElement stateElement = root.firstChildElement("state");
    if (!stateElement.isNull()) {
        QString stateStr = stateElement.text();
        if (stateStr == "shutoff") {
            m_state = VIR_DOMAIN_SHUTOFF;
        } else if (stateStr == "running") {
            m_state = VIR_DOMAIN_RUNNING;
        } else if (stateStr == "paused") {
            m_state = VIR_DOMAIN_PAUSED;
        } else if (stateStr == "crashed") {
            m_state = VIR_DOMAIN_CRASHED;
        }
    }

    // Parse creation time
    QDomElement timeElement = root.firstChildElement("creationTime");
    if (!timeElement.isNull()) {
        qlonglong secs = timeElement.text().toLongLong();
        m_creationTime = QDateTime::fromSecsSinceEpoch(secs);
    }
}

} // namespace QVirt
