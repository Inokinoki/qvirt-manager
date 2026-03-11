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

#include "RNGDevice.h"

namespace QVirt {

RNGDevice::RNGDevice(QObject *parent)
    : Device(parent)
{
}

void RNGDevice::setModel(RNGModel model)
{
    if (m_model != model) {
        m_model = model;
        emit configChanged();
    }
}

void RNGDevice::setBackend(RNGBackend backend)
{
    if (m_backend != backend) {
        m_backend = backend;
        emit configChanged();
    }
}

void RNGDevice::setSourcePath(const QString &path)
{
    if (m_sourcePath != path) {
        m_sourcePath = path;
        emit configChanged();
    }
}

void RNGDevice::setPeriodBytes(int bytes)
{
    if (m_periodBytes != bytes) {
        m_periodBytes = bytes;
        emit configChanged();
    }
}

void RNGDevice::setMaxBytes(int bytes)
{
    if (m_maxBytes != bytes) {
        m_maxBytes = bytes;
        emit configChanged();
    }
}

QString RNGDevice::toXML() const
{
    QString xml = QString::fromLatin1("  <rng model='%1'>\n")
        .arg(modelToString(m_model));

    xml += QString::fromLatin1("    <backend type='%1'").arg(backendToString(m_backend));

    if (m_backend == RNGBackend::Random) {
        xml += QString::fromLatin1(">%1</backend>\n").arg(m_sourcePath);
    } else {
        if (!m_sourcePath.isEmpty()) {
            xml += QString::fromLatin1(" source='%1'").arg(m_sourcePath);
        }
        xml += QString::fromLatin1("/>\n");
    }

    if (m_periodBytes > 0 || m_maxBytes > 0) {
        xml += QString::fromLatin1("    <rate");
        if (m_periodBytes > 0) {
            xml += QString::fromLatin1(" period='%1'").arg(m_periodBytes);
        }
        if (m_maxBytes > 0) {
            xml += QString::fromLatin1(" bytes='%1'").arg(m_maxBytes);
        }
        xml += QString::fromLatin1("/>\n");
    }

    xml += QString::fromLatin1("  </rng>");

    return xml;
}

bool RNGDevice::fromXML(const QString &xml)
{
    Q_UNUSED(xml);
    return true;
}

QString RNGDevice::description() const
{
    return QString::fromLatin1("%1 (%2: %3)")
        .arg(modelToString(m_model))
        .arg(backendToString(m_backend))
        .arg(m_sourcePath);
}

QString RNGDevice::modelToString(RNGModel model)
{
    switch (model) {
        case RNGModel::Virtio: return QStringLiteral("virtio");
        case RNGModel::RNGRandom: return QStringLiteral("rng-random");
        case RNGModel::RNGEgd: return QStringLiteral("rng-egd");
        default: return QStringLiteral("virtio");
    }
}

RNGDevice::RNGModel RNGDevice::stringToModel(const QString &str)
{
    if (str == QLatin1String("rng-random")) return RNGModel::RNGRandom;
    if (str == QLatin1String("rng-egd")) return RNGModel::RNGEgd;
    return RNGModel::Virtio;
}

QString RNGDevice::backendToString(RNGBackend backend)
{
    switch (backend) {
        case RNGBackend::Random: return QStringLiteral("random");
        case RNGBackend::EGD: return QStringLiteral("egd");
        default: return QStringLiteral("random");
    }
}

RNGDevice::RNGBackend RNGDevice::stringToBackend(const QString &str)
{
    if (str == QLatin1String("egd")) return RNGBackend::EGD;
    return RNGBackend::Random;
}

} // namespace QVirt
