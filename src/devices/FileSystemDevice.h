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

#ifndef QVIRT_DEVICES_FILESYSTEMDEVICE_H
#define QVIRT_DEVICES_FILESYSTEMDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Filesystem device for directory sharing
 *
 * Represents a filesystem passthrough device (e.g., virtio-fs)
 */
class FileSystemDevice : public Device
{
    Q_OBJECT

public:
    enum class DriverType {
        None,
        VirtioFS,
        Virtio9P
    };
    Q_ENUM(DriverType)

    enum class AccessMode {
        Passthrough,
        Mapped,
        Squash,
        None
    };
    Q_ENUM(AccessMode)

    enum class WriteMode {
        Immediate,
        Defer
    };
    Q_ENUM(WriteMode)

    explicit FileSystemDevice(QObject *parent = nullptr);
    ~FileSystemDevice() override = default;

    DeviceType deviceType() const override { return DeviceType::FileSystem; }
    QString deviceTypeName() const override { return QStringLiteral("Filesystem"); }

    // Mount properties
    QString mountTag() const { return m_mountTag; }
    void setMountTag(const QString &tag);

    QString sourcePath() const { return m_sourcePath; }
    void setSourcePath(const QString &path);

    QString targetDir() const { return m_targetDir; }
    void setTargetDir(const QString &dir);

    // Driver properties
    DriverType driverType() const { return m_driverType; }
    void setDriverType(DriverType type);

    AccessMode accessMode() const { return m_accessMode; }
    void setAccessMode(AccessMode mode);

    WriteMode writeMode() const { return m_writeMode; }
    void setWriteMode(WriteMode mode);

    QString driverFormat() const { return m_driverFormat; }
    void setDriverFormat(const QString &format);

    bool readonly() const { return m_readonly; }
    void setReadonly(bool readonly);

    // XML representation
    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    // Description
    QString description() const override;

    // Static helpers
    static QString driverTypeToString(DriverType type);
    static DriverType stringToDriverType(const QString &str);
    static QString accessModeToString(AccessMode mode);
    static AccessMode stringToAccessMode(const QString &str);
    static QString writeModeToString(WriteMode mode);
    static WriteMode stringToWriteMode(const QString &str);

private:
    QString m_mountTag;
    QString m_sourcePath;
    QString m_targetDir;
    DriverType m_driverType = DriverType::VirtioFS;
    AccessMode m_accessMode = AccessMode::Passthrough;
    WriteMode m_writeMode = WriteMode::Immediate;
    QString m_driverFormat;
    bool m_readonly = false;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_FILESYSTEMDEVICE_H
