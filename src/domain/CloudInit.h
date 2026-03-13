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

#ifndef QVIRT_DOMAIN_CLOUDINIT_H
#define QVIRT_DOMAIN_CLOUDINIT_H

#include "../core/BaseObject.h"
#include <QString>

namespace QVirt {

/**
 * @brief Cloud-init configuration
 *
 * Represents cloud-init user-data, network-data, and meta-data
 */
class CloudInit : public BaseObject
{
    Q_OBJECT

public:
    explicit CloudInit(QObject *parent = nullptr);
    ~CloudInit() override = default;

    QString userData() const { return m_userData; }
    void setUserData(const QString &data);

    QString networkData() const { return m_networkData; }
    void setNetworkData(const QString &data);

    QString metaData() const { return m_metaData; }
    void setMetaData(const QString &data);

    QString volumePath() const { return m_volumePath; }
    void setVolumePath(const QString &path);

    bool generateIso() const { return m_generateIso; }
    void setGenerateIso(bool generate);

    QString toXML() const;
    bool fromXML(const QString &xml);

    bool isEmpty() const;

signals:
    void configChanged();

private:
    QString m_userData;
    QString m_networkData;
    QString m_metaData;
    QString m_volumePath;
    bool m_generateIso = false;
};

} // namespace QVirt

#endif // QVIRT_DOMAIN_CLOUDINIT_H
