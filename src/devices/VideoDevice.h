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

#ifndef QVIRT_DEVICES_VIDEODEVICE_H
#define QVIRT_DEVICES_VIDEODEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Video card device
 *
 * Represents <video> element in domain XML
 */
class VideoDevice : public Device
{
    Q_OBJECT

public:
    enum class VideoModel {
        None,
        QXL,
        Cirrus,
        VMVGA,
        Virtio,
        VGA,
        QXL3,
        QXL4
    };
    Q_ENUM(VideoModel)

    explicit VideoDevice(QObject *parent = nullptr);
    ~VideoDevice() override = default;

    // Device interface
    DeviceType deviceType() const override { return DeviceType::Video; }
    QString deviceTypeName() const override { return QStringLiteral("video"); }
    QString description() const override;
    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    // Video properties
    VideoModel model() const { return m_model; }
    void setModel(VideoModel model) { m_model = model; }

    int vram() const { return m_vram; } // in KB
    void setVram(int vramKB);

    int heads() const { return m_heads; }
    void setHeads(int heads) { m_heads = heads; }

    bool accel3D() const { return m_accel3D; }
    void setAccel3D(bool enabled) { m_accel3D = enabled; }

    // Format helpers
    static QString modelToString(VideoModel model);
    static VideoModel stringToModel(const QString &str);

private:
    VideoModel m_model = VideoModel::QXL;
    int m_vram = 16384; // 16 MB default
    int m_heads = 1;
    bool m_accel3D = false;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_VIDEODEVICE_H
