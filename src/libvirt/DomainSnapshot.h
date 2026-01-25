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

#ifndef QVIRT_LIBVIRT_DOMAINSNAPSHOT_H
#define QVIRT_LIBVIRT_DOMAINSNAPSHOT_H

#include <QObject>
#include <QString>
#include <QDateTime>

#include <libvirt/libvirt.h>

// Windows.h defines 'state' as a macro which breaks our code
#ifdef _WIN32
#undef state
#endif

typedef struct _virDomainSnapshot virDomainSnapshot;

namespace QVirt {

class Domain;

/**
 * @brief Domain Snapshot Wrapper
 *
 * Represents a VM snapshot in libvirt
 */
class DomainSnapshot : public QObject
{
    Q_OBJECT

public:
    explicit DomainSnapshot(virDomainSnapshotPtr snapshot, Domain *domain, QObject *parent = nullptr);
    ~DomainSnapshot() override;

    // Basic information
    QString name() const;
    QString description() const;

    // Snapshot state
    virDomainState state() const;

    // Timestamp
    QDateTime creationTime() const;

    // Is current snapshot
    bool isCurrent() const;

    // Operations
    bool delete_(unsigned int flags = 0);
    bool revert(unsigned int flags = 0);

    // Parent/children
    DomainSnapshot *parent() const;
    QList<DomainSnapshot*> children() const;

    // XML operations
    QString getXMLDesc(unsigned int flags = 0);

    // Domain reference
    Domain *domain() const { return m_domain; }

private:
    virDomainSnapshotPtr m_snapshot;
    Domain *m_domain;

    QString m_name;
    QString m_description;
    virDomainState m_state;
    QDateTime m_creationTime;

    void parseXML(const QString &xml);
};

} // namespace QVirt

#endif // QVIRT_LIBVIRT_DOMAINSNAPSHOT_H
