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

#ifndef QVIRT_DEVICES_SMARTCARDDEVICE_H
#define QVIRT_DEVICES_SMARTCARDDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Smartcard device
 *
 * Represents a smartcard/CCID device for VMs
 */
class SmartcardDevice : public Device
{
    Q_OBJECT

public:
    enum class SmartcardMode {
        Host,
        HostCertificates,
        Passthrough
    };
    Q_ENUM(SmartcardMode)

    enum class SmartcardType {
        TCP,
        SpiceVMC
    };
    Q_ENUM(SmartcardType)

    explicit SmartcardDevice(QObject *parent = nullptr);
    ~SmartcardDevice() override = default;

    DeviceType deviceType() const override { return DeviceType::Smartcard; }
    QString deviceTypeName() const override { return QStringLiteral("Smartcard"); }

    // Properties
    SmartcardMode mode() const { return m_mode; }
    void setMode(SmartcardMode mode);

    SmartcardType type() const { return m_type; }
    void setType(SmartcardType type);

    QString database() const { return m_database; }
    void setDatabase(const QString &database);

    QString sourcePath() const { return m_sourcePath; }
    void setSourcePath(const QString &path);

    // XML representation
    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    // Description
    QString description() const override;

    // Static helpers
    static QString modeToString(SmartcardMode mode);
    static SmartcardMode stringToMode(const QString &str);
    static QString typeToString(SmartcardType type);
    static SmartcardType stringToType(const QString &str);

private:
    SmartcardMode m_mode = SmartcardMode::Host;
    SmartcardType m_type = SmartcardType::SpiceVMC;
    QString m_database;
    QString m_sourcePath;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_SMARTCARDDEVICE_H
