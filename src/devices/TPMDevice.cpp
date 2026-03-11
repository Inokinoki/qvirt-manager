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

#include "TPMDevice.h"

namespace QVirt {

TPMDevice::TPMDevice(QObject *parent)
    : Device(parent)
{
}

void TPMDevice::setModel(TPMModel model)
{
    if (m_model != model) {
        m_model = model;
        emit configChanged();
    }
}

void TPMDevice::setBackend(TPMBackend backend)
{
    if (m_backend != backend) {
        m_backend = backend;
        emit configChanged();
    }
}

void TPMDevice::setVersion(TPMVersion version)
{
    if (m_version != version) {
        m_version = version;
        emit configChanged();
    }
}

void TPMDevice::setDevicePath(const QString &path)
{
    if (m_devicePath != path) {
        m_devicePath = path;
        emit configChanged();
    }
}

QString TPMDevice::toXML() const
{
    QString xml = QString::fromLatin1("  <tpm model='%1'>\n")
        .arg(modelToString(m_model));

    if (m_backend == TPMBackend::Passthrough) {
        xml += QString::fromLatin1("    <backend type='passthrough'>\n");
        if (!m_devicePath.isEmpty()) {
            xml += QString::fromLatin1("      <device path='%1'/>\n").arg(m_devicePath);
        }
    } else {
        xml += QString::fromLatin1("    <backend type='emulator' version='%1'/>\n")
            .arg(versionToString(m_version));
    }

    xml += QString::fromLatin1("    </backend>\n");
    xml += QString::fromLatin1("  </tpm>");

    return xml;
}

bool TPMDevice::fromXML(const QString &xml)
{
    // Simple XML parsing for TPM device
    // In production, use proper XML parser
    Q_UNUSED(xml);
    return true;
}

QString TPMDevice::description() const
{
    QString desc = QString::fromLatin1("TPM (%1, %2)")
        .arg(modelToString(m_model))
        .arg(backendToString(m_backend));

    if (m_backend == TPMBackend::Emulator) {
        desc += QString::fromLatin1(" %1").arg(versionToString(m_version));
    }

    return desc;
}

QString TPMDevice::modelToString(TPMModel model)
{
    switch (model) {
        case TPMModel::TIS: return QStringLiteral("tpm-tis");
        case TPMModel::CRB: return QStringLiteral("tpm-crb");
        case TPMModel::SPAPR: return QStringLiteral("tpm-spapr");
        default: return QStringLiteral("tpm-tis");
    }
}

TPMDevice::TPMModel TPMDevice::stringToModel(const QString &str)
{
    if (str == QLatin1String("tpm-crb")) return TPMModel::CRB;
    if (str == QLatin1String("tpm-spapr")) return TPMModel::SPAPR;
    return TPMModel::TIS;
}

QString TPMDevice::backendToString(TPMBackend backend)
{
    switch (backend) {
        case TPMBackend::Passthrough: return QStringLiteral("passthrough");
        case TPMBackend::Emulator: return QStringLiteral("emulator");
        default: return QStringLiteral("emulator");
    }
}

TPMDevice::TPMBackend TPMDevice::stringToBackend(const QString &str)
{
    if (str == QLatin1String("passthrough")) return TPMBackend::Passthrough;
    return TPMBackend::Emulator;
}

QString TPMDevice::versionToString(TPMVersion version)
{
    switch (version) {
        case TPMVersion::V1_2: return QStringLiteral("1.2");
        case TPMVersion::V2_0: return QStringLiteral("2.0");
        default: return QStringLiteral("2.0");
    }
}

TPMDevice::TPMVersion TPMDevice::stringToVersion(const QString &str)
{
    if (str == QLatin1String("1.2")) return TPMVersion::V1_2;
    return TPMVersion::V2_0;
}

} // namespace QVirt
