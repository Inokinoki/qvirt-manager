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

#ifndef QVIRT_DOMAIN_FEATURES_H
#define QVIRT_DOMAIN_FEATURES_H

#include "../core/BaseObject.h"

namespace QVirt {

/**
 * @brief Feature flags configuration
 *
 * Represents ACPI, APIC, and other feature flags
 */
class Features : public BaseObject
{
    Q_OBJECT

public:
    explicit Features(QObject *parent = nullptr);
    ~Features() override = default;

    bool acpi() const { return m_acpi; }
    void setAcpi(bool enabled);

    bool apic() const { return m_apic; }
    void setApic(bool enabled);

    bool pae() const { return m_pae; }
    void setPae(bool enabled);

    bool hyperv() const { return m_hyperv; }
    void setHyperv(bool enabled);

    bool smm() const { return m_smm; }
    void setSmm(bool enabled);

    QString toXML() const;
    bool fromXML(const QString &xml);

    bool isEmpty() const;

signals:
    void configChanged();

private:
    bool m_acpi = true;
    bool m_apic = true;
    bool m_pae = false;
    bool m_hyperv = false;
    bool m_smm = false;
};

} // namespace QVirt

#endif // QVIRT_DOMAIN_FEATURES_H
