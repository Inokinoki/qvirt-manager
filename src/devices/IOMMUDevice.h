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

#ifndef QVIRT_DEVICES_IOMMUDEVICE_H
#define QVIRT_DEVICES_IOMMUDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief IOMMU device for DMA remapping
 *
 * Represents an IOMMU device for VT-d, AMD-Vi, or ARM SMMU
 */
class IOMMUDevice : public Device
{
    Q_OBJECT

public:
    enum class IOMMUModel {
        Intel,
        AMD,
        SMMUV3,
        S390
    };
    Q_ENUM(IOMMUModel)

    explicit IOMMUDevice(QObject *parent = nullptr);
    ~IOMMUDevice() override = default;

    DeviceType deviceType() const override { return DeviceType::IOMMU; }
    QString deviceTypeName() const override { return QStringLiteral("IOMMU"); }

    IOMMUModel model() const { return m_model; }
    void setModel(IOMMUModel model);

    bool interruptRemapping() const { return m_interruptRemapping; }
    void setInterruptRemapping(bool enabled);

    bool cachingMode() const { return m_cachingMode; }
    void setCachingMode(bool enabled);

    bool awBits() const { return m_awBits; }
    void setAwBits(bool enabled);

    DeviceAddress address() const { return m_addr; }
    void setAddress(const DeviceAddress &addr);

    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    QString description() const override;

    static QString modelToString(IOMMUModel model);
    static IOMMUModel stringToModel(const QString &str);

private:
    IOMMUModel m_model = IOMMUModel::Intel;
    bool m_interruptRemapping = true;
    bool m_cachingMode = false;
    bool m_awBits = false;
    DeviceAddress m_addr;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_IOMMUDEVICE_H
