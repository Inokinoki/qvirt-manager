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

#ifndef QVIRT_DOMAIN_LAUNCHSECURITY_H
#define QVIRT_DOMAIN_LAUNCHSECURITY_H

#include "../core/BaseObject.h"
#include <QString>

namespace QVirt {

/**
 * @brief Launch security configuration (AMD SEV / Intel TDX)
 *
 * Represents memory encryption and launch security settings
 */
class LaunchSecurity : public BaseObject
{
    Q_OBJECT

public:
    enum class SecurityType {
        None,
        SEV,
        SEVES,
        SEVSNP,
        TDX
    };
    Q_ENUM(SecurityType)

    explicit LaunchSecurity(QObject *parent = nullptr);
    ~LaunchSecurity() override = default;

    SecurityType type() const { return m_type; }
    void setType(SecurityType type);

    QString policy() const { return m_policy; }
    void setPolicy(const QString &policy);

    QString dhCert() const { return m_dhCert; }
    void setDhCert(const QString &cert);

    QString sessionData() const { return m_sessionData; }
    void setSessionData(const QString &data);

    QString secret() const { return m_secret; }
    void setSecret(const QString &secret);

    bool amdSme() const { return m_amdSme; }
    void setAmdSme(bool enabled);

    QString toXML() const;
    bool fromXML(const QString &xml);

    bool isEmpty() const;

    static QString typeToString(SecurityType type);
    static SecurityType stringToType(const QString &str);

signals:
    void configChanged();

private:
    SecurityType m_type = SecurityType::None;
    QString m_policy;
    QString m_dhCert;
    QString m_sessionData;
    QString m_secret;
    bool m_amdSme = false;
};

} // namespace QVirt

#endif // QVIRT_DOMAIN_LAUNCHSECURITY_H
