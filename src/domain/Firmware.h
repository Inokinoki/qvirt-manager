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

#ifndef QVIRT_DOMAIN_FIRMWARE_H
#define QVIRT_DOMAIN_FIRMWARE_H

#include "../core/BaseObject.h"
#include <QString>

namespace QVirt {

/**
 * @brief Firmware configuration
 *
 * Represents firmware settings (BIOS/UEFI, Secure Boot, etc.)
 */
class Firmware : public BaseObject
{
    Q_OBJECT

public:
    enum class FirmwareType {
        BIOS,
        UEFI,
        Custom
    };
    Q_ENUM(FirmwareType)

    enum class BiosType {
        SeaBIOS,
        OVMF,
        EDK2
    };
    Q_ENUM(BiosType)

    explicit Firmware(QObject *parent = nullptr);
    ~Firmware() override = default;

    FirmwareType type() const { return m_type; }
    void setType(FirmwareType type);

    BiosType biosType() const { return m_biosType; }
    void setBiosType(BiosType type);

    bool secureBoot() const { return m_secureBoot; }
    void setSecureBoot(bool enabled);

    QString loader() const { return m_loader; }
    void setLoader(const QString &path);

    QString nvram() const { return m_nvram; }
    void setNvram(const QString &path);

    QString nvramTemplate() const { return m_nvramTemplate; }
    void setNvramTemplate(const QString &path);

    bool smm() const { return m_smm; }
    void setSmm(bool enabled);

    QString toXML() const;
    bool fromXML(const QString &xml);

    bool isEmpty() const;

    static QString typeToString(FirmwareType type);
    static FirmwareType stringToType(const QString &str);
    static QString biosTypeToString(BiosType type);
    static BiosType stringToBiosType(const QString &str);

signals:
    void configChanged();

private:
    FirmwareType m_type = FirmwareType::BIOS;
    BiosType m_biosType = BiosType::SeaBIOS;
    bool m_secureBoot = false;
    bool m_smm = false;
    QString m_loader;
    QString m_nvram;
    QString m_nvramTemplate;
};

} // namespace QVirt

#endif // QVIRT_DOMAIN_FIRMWARE_H
