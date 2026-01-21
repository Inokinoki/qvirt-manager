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

#ifndef QVIRT_DEVICES_DISKDEVICE_H
#define QVIRT_DEVICES_DISKDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Disk device representation
 *
 * Represents <disk> element in domain XML
 */
class DiskDevice : public Device
{
    Q_OBJECT

public:
    enum class DiskType {
        File,
        Block,
        Dir,
        Network,
        Volume
    };
    Q_ENUM(DiskType)

    enum class DeviceType {
        Disk,
        CDROM,
        Floppy,
        LUN
    };
    Q_ENUM(DeviceType)

    enum class BusType {
        None,
        IDE,
        SCSI,
        Virtio,
        SATA,
        USB,
        SD,
        FDC
    };
    Q_ENUM(BusType)

    enum class CacheMode {
        Default,
        None,
        Writethrough,
        Writeback,
        Directsync,
        Unsafe
    };
    Q_ENUM(CacheMode)

    enum class IOMode {
        Default,
        Threads,
        Native
    };
    Q_ENUM(IOMode)

    explicit DiskDevice(QObject *parent = nullptr);
    ~DiskDevice() override = default;

    // Device interface
    ::QVirt::Device::DeviceType deviceType() const override { return ::QVirt::Device::DeviceType::Disk; }
    QString deviceTypeName() const override { return QStringLiteral("disk"); }
    QString description() const override;
    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    // Disk properties
    DiskType diskType() const { return m_diskType; }
    void setDiskType(DiskType type) { m_diskType = type; }

    DeviceType device() const { return m_device; }
    void setDevice(DeviceType dev) { m_device = dev; }

    BusType bus() const { return m_bus; }
    void setBus(BusType bus) { m_bus = bus; }

    QString target() const { return m_target; }
    void setTarget(const QString &target);

    QString source() const { return m_source; }
    void setSource(const QString &source);

    QString driverName() const { return m_driverName; }
    void setDriverName(const QString &name) { m_driverName = name; }

    QString driverType() const { return m_driverType; }
    void setDriverType(const QString &type) { m_driverType = type; }

    CacheMode cacheMode() const { return m_cacheMode; }
    void setCacheMode(CacheMode mode) { m_cacheMode = mode; }

    bool readonly() const { return m_readonly; }
    void setReadonly(bool ro) { m_readonly = ro; }

    bool shareable() const { return m_shareable; }
    void setShareable(bool shareable) { m_shareable = shareable; }

    qint64 capacity() const { return m_capacity; }
    void setCapacity(qint64 bytes) { m_capacity = bytes; }

    DeviceAddress address() const { return m_address; }
    void setAddress(const DeviceAddress &addr) { m_address = addr; }

    int bootOrder() const { return m_bootOrder; }
    void setBootOrder(int order) { m_bootOrder = order; }

    // Format helpers
    static QString busTypeToString(BusType bus);
    static BusType stringToBusType(const QString &str);

    static QString deviceTypeToString(DeviceType dev);
    static DeviceType stringToDeviceType(const QString &str);

    static QString cacheModeToString(CacheMode mode);
    static CacheMode stringToCacheMode(const QString &str);

private:
    DiskType m_diskType = DiskType::File;
    DeviceType m_device = DeviceType::Disk;
    BusType m_bus = BusType::None;
    QString m_target;
    QString m_source;
    QString m_driverName = "qemu";
    QString m_driverType;
    CacheMode m_cacheMode = CacheMode::Default;
    IOMode m_ioMode = IOMode::Default;
    bool m_readonly = false;
    bool m_shareable = false;
    qint64 m_capacity = 0;
    DeviceAddress m_address;
    int m_bootOrder = 0;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_DISKDEVICE_H
