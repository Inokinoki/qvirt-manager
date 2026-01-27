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

#ifndef QVIRT_DEVICES_RNGDEVICE_H
#define QVIRT_DEVICES_RNGDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Random Number Generator device
 *
 * Represents a virtual RNG device for VMs
 */
class RNGDevice : public Device
{
    Q_OBJECT

public:
    enum class RNGModel {
        Virtio,
        RNGRandom,
        RNGEgd
    };
    Q_ENUM(RNGModel)

    enum class RNGBackend {
        Random,
        EGD
    };
    Q_ENUM(RNGBackend)

    explicit RNGDevice(QObject *parent = nullptr);
    ~RNGDevice() override = default;

    DeviceType deviceType() const override { return DeviceType::RNG; }
    QString deviceTypeName() const override { return QStringLiteral("RNG"); }

    // Properties
    RNGModel model() const { return m_model; }
    void setModel(RNGModel model);

    RNGBackend backend() const { return m_backend; }
    void setBackend(RNGBackend backend);

    QString sourcePath() const { return m_sourcePath; }
    void setSourcePath(const QString &path);

    int periodBytes() const { return m_periodBytes; }
    void setPeriodBytes(int bytes);

    int maxBytes() const { return m_maxBytes; }
    void setMaxBytes(int bytes);

    // XML representation
    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    // Description
    QString description() const override;

    // Static helpers
    static QString modelToString(RNGModel model);
    static RNGModel stringToModel(const QString &str);
    static QString backendToString(RNGBackend backend);
    static RNGBackend stringToBackend(const QString &str);

private:
    RNGModel m_model = RNGModel::Virtio;
    RNGBackend m_backend = RNGBackend::Random;
    QString m_sourcePath = "/dev/urandom";
    int m_periodBytes = 0;
    int m_maxBytes = 0;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_RNGDEVICE_H
