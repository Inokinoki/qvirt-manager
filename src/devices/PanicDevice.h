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

#ifndef QVIRT_DEVICES_PANICDEVICE_H
#define QVIRT_DEVICES_PANICDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Panic device for VM crash notification
 *
 * Represents a panic device that notifies the host when the guest panics
 */
class PanicDevice : public Device
{
    Q_OBJECT

public:
    enum class PanicModel {
        ISA,
        PSpics,
        PVPanel,
        S390
    };
    Q_ENUM(PanicModel)

    explicit PanicDevice(QObject *parent = nullptr);
    ~PanicDevice() override = default;

    DeviceType deviceType() const override { return DeviceType::Panic; }
    QString deviceTypeName() const override { return QStringLiteral("Panic"); }

    PanicModel model() const { return m_model; }
    void setModel(PanicModel model);

    QString ioPort() const { return m_ioPort; }
    void setIoPort(const QString &port);

    QString mmioAddress() const { return m_mmioAddress; }
    void setMmioAddress(const QString &addr);

    QString mmioSize() const { return m_mmioSize; }
    void setMmioSize(const QString &size);

    DeviceAddress address() const { return m_addr; }
    void setAddress(const DeviceAddress &addr);

    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    QString description() const override;

    static QString modelToString(PanicModel model);
    static PanicModel stringToModel(const QString &str);

private:
    PanicModel m_model = PanicModel::ISA;
    QString m_ioPort;
    QString m_mmioAddress;
    QString m_mmioSize;
    DeviceAddress m_addr;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_PANICDEVICE_H
