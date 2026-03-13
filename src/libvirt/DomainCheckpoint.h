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

#ifndef QVIRT_LIBVIRT_DOMAINCHECKPOINT_H
#define QVIRT_LIBVIRT_DOMAINCHECKPOINT_H

#include "../core/BaseObject.h"
#include <QString>
#include <QDateTime>

#ifdef LIBVIRT_FOUND
#include <libvirt/libvirt.h>
#endif

namespace QVirt {

class Connection;

/**
 * @brief Domain checkpoint wrapper
 *
 * Represents a VM checkpoint for save/restore functionality
 */
class DomainCheckpoint : public BaseObject
{
    Q_OBJECT

public:
    explicit DomainCheckpoint(Connection *conn, void *checkpointHandle, QObject *parent = nullptr);
    explicit DomainCheckpoint(QObject *parent = nullptr);
    ~DomainCheckpoint() override;

    QString name() const { return m_name; }
    void setName(const QString &name);

    QString description() const { return m_description; }
    void setDescription(const QString &desc);

    QDateTime creationTime() const { return m_creationTime; }
    void setCreationTime(const QDateTime &time);

    QString parentName() const { return m_parentName; }
    void setParentName(const QString &name);

#ifdef LIBVIRT_FOUND
    void *handle() const { return m_checkpointHandle; }
#endif

    bool isReverting() const { return m_reverting; }
    void setReverting(bool reverting);

    QString toXML() const;
    bool fromXML(const QString &xml);

signals:
    void stateChanged();

private:
    QString m_name;
    QString m_description;
    QDateTime m_creationTime;
    QString m_parentName;
#ifdef LIBVIRT_FOUND
    void *m_checkpointHandle = nullptr;
#endif
    bool m_reverting = false;
};

} // namespace QVirt

#endif // QVIRT_LIBVIRT_DOMAINCHECKPOINT_H
