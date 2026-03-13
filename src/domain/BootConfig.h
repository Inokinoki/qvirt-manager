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

#ifndef QVIRT_DOMAIN_BOOTCONFIG_H
#define QVIRT_DOMAIN_BOOTCONFIG_H

#include "../core/BaseObject.h"
#include <QString>
#include <QList>

namespace QVirt {

/**
 * @brief Boot device configuration
 */
class BootDevice
{
public:
    BootDevice();
    BootDevice(const BootDevice &other);
    BootDevice &operator=(const BootDevice &other);

    enum class DeviceType {
        HardDisk,
        CDROM,
        Floppy,
        Network,
        USB
    };

    DeviceType type = DeviceType::HardDisk;
    int order = 0;
    QString dev;

    bool isEmpty() const;
};

/**
 * @brief Boot configuration
 *
 * Represents boot order and boot menu settings
 */
class BootConfig : public BaseObject
{
    Q_OBJECT

public:
    explicit BootConfig(QObject *parent = nullptr);
    ~BootConfig() override = default;

    QList<BootDevice> devices() const { return m_devices; }
    void setDevices(const QList<BootDevice> &devices);
    void addDevice(const BootDevice &device);

    bool bootMenu() const { return m_bootMenu; }
    void setBootMenu(bool enabled);

    int bootMenuTimeout() const { return m_bootMenuTimeout; }
    void setBootMenuTimeout(int timeout);

    QString toXML() const;
    bool fromXML(const QString &xml);

    bool isEmpty() const;

signals:
    void configChanged();

private:
    QList<BootDevice> m_devices;
    bool m_bootMenu = false;
    int m_bootMenuTimeout = -1;
};

} // namespace QVirt

#endif // QVIRT_DOMAIN_BOOTCONFIG_H
