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

#include "SoundDevice.h"

namespace QVirt {

SoundDevice::SoundDevice(QObject *parent)
    : Device(parent)
{
}

QString SoundDevice::description() const
{
    QString modelStr = modelToString(m_model);
    return QString("%1 Audio Device").arg(modelStr);
}

QString SoundDevice::toXML() const
{
    QString xml;

    xml += QString("  <sound model='%1'/>\n").arg(modelToString(m_model));

    return xml;
}

bool SoundDevice::fromXML(const QString &xml)
{
    // Parse XML and populate properties
    // TODO: Implement proper XML parsing
    Q_UNUSED(xml);
    return false;
}

QString SoundDevice::modelToString(SoundModel model)
{
    switch (model) {
    case SoundModel::ICH6: return "ich6";
    case SoundModel::ICH9: return "ich9";
    case SoundModel::AC97: return "ac97";
    case SoundModel::ES1370: return "es1370";
    case SoundModel::SB16: return "sb16";
    case SoundModel::PCSpeaker: return "pcspk";
    case SoundModel::USB: return "usb";
    default: return "ich6";
    }
}

SoundDevice::SoundModel SoundDevice::stringToModel(const QString &str)
{
    QString lower = str.toLower();
    if (lower == "ich6") return SoundModel::ICH6;
    if (lower == "ich9") return SoundModel::ICH9;
    if (lower == "ac97") return SoundModel::AC97;
    if (lower == "es1370") return SoundModel::ES1370;
    if (lower == "sb16") return SoundModel::SB16;
    if (lower == "pcspk") return SoundModel::PCSpeaker;
    if (lower == "usb") return SoundModel::USB;
    return SoundModel::ICH6;
}

} // namespace QVirt
