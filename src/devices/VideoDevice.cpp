/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 The QVirt-Manager Developers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "VideoDevice.h"

namespace QVirt {

VideoDevice::VideoDevice(QObject *parent)
    : Device(parent)
{
}

QString VideoDevice::description() const
{
    QString modelStr = modelToString(m_model);
    return QString("%1 Video Card").arg(modelStr);
}

void VideoDevice::setVram(int vramKB)
{
    if (m_vram != vramKB) {
        m_vram = vramKB;
        emit configChanged();
    }
}

QString VideoDevice::toXML() const
{
    QString xml;

    xml += "  <video>\n";
    xml += QString("    <model type='%1'").arg(modelToString(m_model));

    if (m_vram > 0) {
        xml += QString(" vram='%1'").arg(m_vram);
    }

    if (m_heads > 1) {
        xml += QString(" heads='%1'").arg(m_heads);
    }

    if (m_accel3D) {
        xml += " acceleration='3d'";
    } else {
        xml += " acceleration='2d'";
    }

    xml += "/>\n";
    xml += "  </video>\n";

    return xml;
}

bool VideoDevice::fromXML(const QString &xml)
{
    // Parse XML and populate properties
    // TODO: Implement proper XML parsing
    Q_UNUSED(xml);
    return false;
}

QString VideoDevice::modelToString(VideoModel model)
{
    switch (model) {
    case VideoModel::QXL: return "qxl";
    case VideoModel::Cirrus: return "cirrus";
    case VideoModel::VMVGA: return "vmvga";
    case VideoModel::Virtio: return "virtio";
    case VideoModel::VGA: return "vga";
    case VideoModel::QXL3: return "qxl3";
    case VideoModel::QXL4: return "qxl4";
    default: return "qxl";
    }
}

VideoDevice::VideoModel VideoDevice::stringToModel(const QString &str)
{
    QString lower = str.toLower();
    if (lower == "qxl") return VideoModel::QXL;
    if (lower == "cirrus") return VideoModel::Cirrus;
    if (lower == "vmvga") return VideoModel::VMVGA;
    if (lower == "virtio") return VideoModel::Virtio;
    if (lower == "vga") return VideoModel::VGA;
    if (lower == "qxl3") return VideoModel::QXL3;
    if (lower == "qxl4") return VideoModel::QXL4;
    return VideoModel::QXL;
}

} // namespace QVirt
