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

#ifndef QVIRT_DEVICES_SERIALDEVICE_H
#define QVIRT_DEVICES_SERIALDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Serial port device
 *
 * Represents a serial port device for VM console access
 */
class SerialDevice : public Device
{
    Q_OBJECT

public:
    enum class CharDeviceType {
        PTY,
        Dev,
        File,
        Pipe,
        TCP,
        UDP,
        Unix,
        SpiceVMC,
        SpicePort,
        Nmdm
    };
    Q_ENUM(CharDeviceType)

    explicit SerialDevice(QObject *parent = nullptr);
    ~SerialDevice() override = default;

    DeviceType deviceType() const override { return DeviceType::Serial; }
    QString deviceTypeName() const override { return QStringLiteral("Serial"); }

    int port() const { return m_port; }
    void setPort(int port);

    CharDeviceType charDeviceType() const { return m_charDeviceType; }
    void setCharDeviceType(CharDeviceType type);

    QString sourcePath() const { return m_sourcePath; }
    void setSourcePath(const QString &path);

    QString sourceHost() const { return m_sourceHost; }
    void setSourceHost(const QString &host);

    QString sourceService() const { return m_sourceService; }
    void setSourceService(const QString &service);

    QString targetPort() const { return m_targetPort; }
    void setTargetPort(const QString &port);

    QString targetName() const { return m_targetName; }
    void setTargetName(const QString &name);

    DeviceAddress address() const { return m_addr; }
    void setAddress(const DeviceAddress &addr);

    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    QString description() const override;

    static QString charDeviceTypeToString(CharDeviceType type);
    static CharDeviceType stringToCharDeviceType(const QString &str);

private:
    int m_port = 0;
    CharDeviceType m_charDeviceType = CharDeviceType::PTY;
    QString m_sourcePath;
    QString m_sourceHost;
    QString m_sourceService;
    QString m_targetPort;
    QString m_targetName;
    DeviceAddress m_addr;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_SERIALDEVICE_H
