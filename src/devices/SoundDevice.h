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

#ifndef QVIRT_DEVICES_SOUNDDEVICE_H
#define QVIRT_DEVICES_SOUNDDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Sound/audio device
 *
 * Represents <sound> element in domain XML
 */
class SoundDevice : public Device
{
    Q_OBJECT

public:
    enum class SoundModel {
        None,
        ICH6,
        ICH9,
        AC97,
        ES1370,
        SB16,
        PCSpeaker,
        USB
    };
    Q_ENUM(SoundModel)

    explicit SoundDevice(QObject *parent = nullptr);
    ~SoundDevice() override = default;

    // Device interface
    DeviceType deviceType() const override { return DeviceType::Sound; }
    QString deviceTypeName() const override { return QStringLiteral("sound"); }
    QString description() const override;
    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    // Sound properties
    SoundModel model() const { return m_model; }
    void setModel(SoundModel model) { m_model = model; }

    // Format helpers
    static QString modelToString(SoundModel model);
    static SoundModel stringToModel(const QString &str);

private:
    SoundModel m_model = SoundModel::ICH6;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_SOUNDDEVICE_H
