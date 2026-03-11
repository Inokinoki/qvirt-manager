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

#ifndef QVIRT_DEVICES_TPMDEVICE_H
#define QVIRT_DEVICES_TPMDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief TPM (Trusted Platform Module) device
 *
 * Represents a TPM device for VMs
 */
class TPMDevice : public Device
{
    Q_OBJECT

public:
    enum class TPMModel {
        TIS,
        CRB,
        SPAPR
    };
    Q_ENUM(TPMModel)

    enum class TPMBackend {
        Passthrough,
        Emulator
    };
    Q_ENUM(TPMBackend)

    enum class TPMVersion {
        V1_2,
        V2_0
    };
    Q_ENUM(TPMVersion)

    explicit TPMDevice(QObject *parent = nullptr);
    ~TPMDevice() override = default;

    DeviceType deviceType() const override { return DeviceType::TPM; }
    QString deviceTypeName() const override { return QStringLiteral("TPM"); }

    // TPM properties
    TPMModel model() const { return m_model; }
    void setModel(TPMModel model);

    TPMBackend backend() const { return m_backend; }
    void setBackend(TPMBackend backend);

    TPMVersion version() const { return m_version; }
    void setVersion(TPMVersion version);

    QString devicePath() const { return m_devicePath; }
    void setDevicePath(const QString &path);

    // XML representation
    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    // Description
    QString description() const override;

    // Static helpers
    static QString modelToString(TPMModel model);
    static TPMModel stringToModel(const QString &str);
    static QString backendToString(TPMBackend backend);
    static TPMBackend stringToBackend(const QString &str);
    static QString versionToString(TPMVersion version);
    static TPMVersion stringToVersion(const QString &str);

private:
    TPMModel m_model = TPMModel::TIS;
    TPMBackend m_backend = TPMBackend::Emulator;
    TPMVersion m_version = TPMVersion::V2_0;
    QString m_devicePath;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_TPMDEVICE_H
