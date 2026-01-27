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

#ifndef QVIRT_DEVICES_MEMBALLOONDEVICE_H
#define QVIRT_DEVICES_MEMBALLOONDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Memory balloon device
 *
 * Represents a virtio-memballoon device for dynamic memory management
 */
class MemballoonDevice : public Device
{
    Q_OBJECT

public:
    enum class MemballoonModel {
        Virtio,
        VirtioTraditional,
        VirtioNonTraditional,
        Xen,
        None
    };
    Q_ENUM(MemballoonModel)

    explicit MemballoonDevice(QObject *parent = nullptr);
    ~MemballoonDevice() override = default;

    DeviceType deviceType() const override { return DeviceType::Memballoon; }
    QString deviceTypeName() const override { return QStringLiteral("Memory Balloon"); }

    // Properties
    MemballoonModel model() const { return m_model; }
    void setModel(MemballoonModel model);

    bool autodeflate() const { return m_autodeflate; }
    void setAutodeflate(bool autodeflate);

    bool deflateOnOOM() const { return m_deflateOnOOM; }
    void setDeflateOnOOM(bool deflate);

    int period() const { return m_period; }
    void setPeriod(int period);

    // XML representation
    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    // Description
    QString description() const override;

    // Static helpers
    static QString modelToString(MemballoonModel model);
    static MemballoonModel stringToModel(const QString &str);

private:
    MemballoonModel m_model = MemballoonModel::Virtio;
    bool m_autodeflate = false;
    bool m_deflateOnOOM = false;
    int m_period = 0;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_MEMBALLOONDEVICE_H
