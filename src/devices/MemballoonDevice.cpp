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

#include "MemballoonDevice.h"

namespace QVirt {

MemballoonDevice::MemballoonDevice(QObject *parent)
    : Device(parent)
{
}

void MemballoonDevice::setModel(MemballoonModel model)
{
    if (m_model != model) {
        m_model = model;
        emit configChanged();
    }
}

void MemballoonDevice::setAutodeflate(bool autodeflate)
{
    if (m_autodeflate != autodeflate) {
        m_autodeflate = autodeflate;
        emit configChanged();
    }
}

void MemballoonDevice::setDeflateOnOOM(bool deflate)
{
    if (m_deflateOnOOM != deflate) {
        m_deflateOnOOM = deflate;
        emit configChanged();
    }
}

void MemballoonDevice::setPeriod(int period)
{
    if (m_period != period) {
        m_period = period;
        emit configChanged();
    }
}

QString MemballoonDevice::toXML() const
{
    QString xml = QString::fromLatin1("  <memballoon model='%1'").arg(modelToString(m_model));

    QStringList attrs;
    if (m_autodeflate) {
        attrs.append(QStringLiteral("autodeflate='on'"));
    }
    if (m_deflateOnOOM) {
        attrs.append(QStringLiteral("deflate-on-oom='on'"));
    }
    if (m_period > 0) {
        attrs.append(QString::fromLatin1("period='%1'").arg(m_period));
    }

    if (!attrs.isEmpty()) {
        xml += QString::fromLatin1(" %1").arg(attrs.join(QLatin1String(" ")));
    }

    xml += QString::fromLatin1("/>");

    return xml;
}

bool MemballoonDevice::fromXML(const QString &xml)
{
    Q_UNUSED(xml);
    return true;
}

QString MemballoonDevice::description() const
{
    return QString::fromLatin1("%1")
        .arg(modelToString(m_model));
}

QString MemballoonDevice::modelToString(MemballoonModel model)
{
    switch (model) {
        case MemballoonModel::Virtio: return QStringLiteral("virtio");
        case MemballoonModel::VirtioTraditional: return QStringLiteral("virtio-transitional");
        case MemballoonModel::VirtioNonTraditional: return QStringLiteral("virtio-non-transitional");
        case MemballoonModel::Xen: return QStringLiteral("xen");
        case MemballoonModel::None: return QStringLiteral("none");
        default: return QStringLiteral("virtio");
    }
}

MemballoonDevice::MemballoonModel MemballoonDevice::stringToModel(const QString &str)
{
    if (str == QLatin1String("virtio-transitional")) return MemballoonModel::VirtioTraditional;
    if (str == QLatin1String("virtio-non-transitional")) return MemballoonModel::VirtioNonTraditional;
    if (str == QLatin1String("xen")) return MemballoonModel::Xen;
    if (str == QLatin1String("none")) return MemballoonModel::None;
    return MemballoonModel::Virtio;
}

} // namespace QVirt
