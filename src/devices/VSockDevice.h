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

#ifndef QVIRT_DEVICES_VSOCKDEVICE_H
#define QVIRT_DEVICES_VSOCKDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief VirtIO VSOCK device for VM-host socket communication
 *
 * Represents a VSOCK device that enables socket communication between guest and host
 */
class VSockDevice : public Device
{
    Q_OBJECT

public:
    enum class CIDAssignment {
        Auto,
        Manual
    };
    Q_ENUM(CIDAssignment)

    explicit VSockDevice(QObject *parent = nullptr);
    ~VSockDevice() override = default;

    DeviceType deviceType() const override { return DeviceType::VSock; }
    QString deviceTypeName() const override { return QStringLiteral("VSOCK"); }

    CIDAssignment cidAssignment() const { return m_cidAssignment; }
    void setCidAssignment(CIDAssignment assignment);

    unsigned int cid() const { return m_cid; }
    void setCid(unsigned int cid);

    DeviceAddress address() const { return m_addr; }
    void setAddress(const DeviceAddress &addr);

    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    QString description() const override;

    static QString cidAssignmentToString(CIDAssignment assignment);
    static CIDAssignment stringToCidAssignment(const QString &str);

private:
    CIDAssignment m_cidAssignment = CIDAssignment::Auto;
    unsigned int m_cid = 0;
    DeviceAddress m_addr;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_VSOCKDEVICE_H
