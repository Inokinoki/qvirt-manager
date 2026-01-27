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

#ifndef QVIRT_DEVICES_HOSTDEVICE_H
#define QVIRT_DEVICES_HOSTDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Host device passthrough
 *
 * Represents a host device (USB, PCI, etc.) passed through to a VM
 */
class HostDevice : public Device
{
    Q_OBJECT

public:
    enum class HostDeviceType {
        USB,
        PCI,
        SCSI,
        SCSIHost,
        SCSIWWID,
        MDev
    };
    Q_ENUM(HostDeviceType)

    enum class USBMode {
        Bus,
        Device
    };

    explicit HostDevice(QObject *parent = nullptr);
    ~HostDevice() override = default;

    DeviceType deviceType() const override { return DeviceType::HostDevice; }
    QString deviceTypeName() const override { return QStringLiteral("Host Device"); }

    // Device type
    HostDeviceType hostDeviceType() const { return m_deviceType; }
    void setHostDeviceType(HostDeviceType type);

    // USB properties
    USBMode usbMode() const { return m_usbMode; }
    void setUsbMode(USBMode mode);

    int usbBus() const { return m_usbBus; }
    void setUsbBus(int bus);

    int usbDevice() const { return m_usbDevice; }
    void setUsbDevice(int device);

    QString usbVendorId() const { return m_usbVendorId; }
    void setUsbVendorId(const QString &id);

    QString usbProductId() const { return m_usbProductId; }
    void setUsbProductId(const QString &id);

    // PCI properties
    QString pciDomain() const { return m_pciDomain; }
    void setPciDomain(const QString &domain);

    QString pciBus() const { return m_pciBus; }
    void setPciBus(const QString &bus);

    QString pciSlot() const { return m_pciSlot; }
    void setPciSlot(const QString &slot);

    QString pciFunction() const { return m_pciFunction; }
    void setPciFunction(const QString &function);

    // SCSI properties
    QString scsiAdapter() const { return m_scsiAdapter; }
    void setScsiAdapter(const QString &adapter);

    QString scsiBus() const { return m_scsiBus; }
    void setScsiBus(const QString &bus);

    QString scsiTarget() const { return m_scsiTarget; }
    void setScsiTarget(const QString &target);

    QString scsiLun() const { return m_scsiLun; }
    void setScsiLun(const QString &lun);

    QString scsiWWID() const { return m_scsiWWID; }
    void setScsiWWID(const QString &wwid);

    // MDev properties
    QString mdevUUID() const { return m_mdevUUID; }
    void setMdevUUID(const QString &uuid);

    QString mdevAddress() const { return m_mdevAddress; }
    void setMdevAddress(const QString &address);

    // XML representation
    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    // Description
    QString description() const override;

    // Static helpers
    static QString typeToString(HostDeviceType type);
    static HostDeviceType stringToType(const QString &str);

private:
    HostDeviceType m_deviceType = HostDeviceType::USB;

    // USB
    USBMode m_usbMode = USBMode::Device;
    int m_usbBus = 0;
    int m_usbDevice = 0;
    QString m_usbVendorId;
    QString m_usbProductId;

    // PCI
    QString m_pciDomain = "0";
    QString m_pciBus;
    QString m_pciSlot;
    QString m_pciFunction;

    // SCSI
    QString m_scsiAdapter;
    QString m_scsiBus;
    QString m_scsiTarget;
    QString m_scsiLun;
    QString m_scsiWWID;

    // MDev
    QString m_mdevUUID;
    QString m_mdevAddress;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_HOSTDEVICE_H
