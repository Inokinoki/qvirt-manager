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

#ifndef QVIRT_DEVICES_PARALLELDEVICE_H
#define QVIRT_DEVICES_PARALLELDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Parallel port device
 *
 * Represents a parallel port device for VM
 */
class ParallelDevice : public Device
{
    Q_OBJECT

public:
    enum class ParallelDeviceType {
        PTY,
        Dev,
        File,
        Pipe
    };
    Q_ENUM(ParallelDeviceType)

    explicit ParallelDevice(QObject *parent = nullptr);
    ~ParallelDevice() override = default;

    DeviceType deviceType() const override { return DeviceType::Parallel; }
    QString deviceTypeName() const override { return QStringLiteral("Parallel"); }

    int port() const { return m_port; }
    void setPort(int port);

    ParallelDeviceType parallelDeviceType() const { return m_parallelDeviceType; }
    void setParallelDeviceType(ParallelDeviceType type);

    QString sourcePath() const { return m_sourcePath; }
    void setSourcePath(const QString &path);

    DeviceAddress address() const { return m_addr; }
    void setAddress(const DeviceAddress &addr);

    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    QString description() const override;

    static QString parallelDeviceTypeToString(ParallelDeviceType type);
    static ParallelDeviceType stringToParallelDeviceType(const QString &str);

private:
    int m_port = 0;
    ParallelDeviceType m_parallelDeviceType = ParallelDeviceType::PTY;
    QString m_sourcePath;
    DeviceAddress m_addr;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_PARALLELDEVICE_H
