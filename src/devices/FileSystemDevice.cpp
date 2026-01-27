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

#include "FileSystemDevice.h"

namespace QVirt {

FileSystemDevice::FileSystemDevice(QObject *parent)
    : Device(parent)
{
}

void FileSystemDevice::setMountTag(const QString &tag)
{
    if (m_mountTag != tag) {
        m_mountTag = tag;
        emit configChanged();
    }
}

void FileSystemDevice::setSourcePath(const QString &path)
{
    if (m_sourcePath != path) {
        m_sourcePath = path;
        emit configChanged();
    }
}

void FileSystemDevice::setTargetDir(const QString &dir)
{
    if (m_targetDir != dir) {
        m_targetDir = dir;
        emit configChanged();
    }
}

void FileSystemDevice::setDriverType(DriverType type)
{
    if (m_driverType != type) {
        m_driverType = type;
        emit configChanged();
    }
}

void FileSystemDevice::setAccessMode(AccessMode mode)
{
    if (m_accessMode != mode) {
        m_accessMode = mode;
        emit configChanged();
    }
}

void FileSystemDevice::setWriteMode(WriteMode mode)
{
    if (m_writeMode != mode) {
        m_writeMode = mode;
        emit configChanged();
    }
}

void FileSystemDevice::setDriverFormat(const QString &format)
{
    if (m_driverFormat != format) {
        m_driverFormat = format;
        emit configChanged();
    }
}

void FileSystemDevice::setReadonly(bool readonly)
{
    if (m_readonly != readonly) {
        m_readonly = readonly;
        emit configChanged();
    }
}

QString FileSystemDevice::toXML() const
{
    QString xml = QString::fromLatin1("  <filesystem type='mount' accessmode='%1'>\n")
        .arg(accessModeToString(m_accessMode));

    xml += QString::fromLatin1("    <source dir='%1'/>\n").arg(m_sourcePath);
    xml += QString::fromLatin1("    <target dir='%1'/>\n").arg(m_targetDir);

    if (m_driverType != DriverType::None) {
        xml += QString::fromLatin1("    <driver type='%1'").arg(driverTypeToString(m_driverType));
        if (!m_driverFormat.isEmpty()) {
            xml += QString::fromLatin1(" format='%1'").arg(m_driverFormat);
        }
        if (m_writeMode == WriteMode::Defer) {
            xml += QString::fromLatin1(" wrpolicy='defer'");
        }
        xml += QString::fromLatin1("/>\n");
    }

    if (m_readonly) {
        xml += QString::fromLatin1("    <readonly/>\n");
    }

    xml += QString::fromLatin1("  </filesystem>");

    return xml;
}

bool FileSystemDevice::fromXML(const QString &xml)
{
    Q_UNUSED(xml);
    return true;
}

QString FileSystemDevice::description() const
{
    return QString::fromLatin1("%1: %2 -> %3")
        .arg(driverTypeToString(m_driverType))
        .arg(m_sourcePath)
        .arg(m_mountTag.isEmpty() ? m_targetDir : m_mountTag);
}

QString FileSystemDevice::driverTypeToString(DriverType type)
{
    switch (type) {
        case DriverType::None: return QStringLiteral("none");
        case DriverType::VirtioFS: return QStringLiteral("virtiofs");
        case DriverType::Virtio9P: return QStringLiteral("9p");
        default: return QStringLiteral("virtiofs");
    }
}

FileSystemDevice::DriverType FileSystemDevice::stringToDriverType(const QString &str)
{
    if (str == QLatin1String("9p")) return DriverType::Virtio9P;
    if (str == QLatin1String("none")) return DriverType::None;
    return DriverType::VirtioFS;
}

QString FileSystemDevice::accessModeToString(AccessMode mode)
{
    switch (mode) {
        case AccessMode::Passthrough: return QStringLiteral("passthrough");
        case AccessMode::Mapped: return QStringLiteral("mapped");
        case AccessMode::Squash: return QStringLiteral("squash");
        case AccessMode::None: return QStringLiteral("none");
        default: return QStringLiteral("passthrough");
    }
}

FileSystemDevice::AccessMode FileSystemDevice::stringToAccessMode(const QString &str)
{
    if (str == QLatin1String("mapped")) return AccessMode::Mapped;
    if (str == QLatin1String("squash")) return AccessMode::Squash;
    if (str == QLatin1String("none")) return AccessMode::None;
    return AccessMode::Passthrough;
}

QString FileSystemDevice::writeModeToString(WriteMode mode)
{
    switch (mode) {
        case WriteMode::Immediate: return QStringLiteral("immediate");
        case WriteMode::Defer: return QStringLiteral("defer");
        default: return QStringLiteral("immediate");
    }
}

FileSystemDevice::WriteMode FileSystemDevice::stringToWriteMode(const QString &str)
{
    if (str == QLatin1String("defer")) return WriteMode::Defer;
    return WriteMode::Immediate;
}

} // namespace QVirt
