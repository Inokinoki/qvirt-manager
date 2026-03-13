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

#ifndef QVIRT_DEVICES_CHANNELDEVICE_H
#define QVIRT_DEVICES_CHANNELDEVICE_H

#include "Device.h"
#include <QDomElement>

namespace QVirt {

/**
 * @brief Channel device for VM communication
 *
 * Supports virtio-serial channels for guest-host communication.
 * Common uses:
 * - QEMU guest agent communication
 * - Spice WebDAV daemon
 * - virtio-serial console
 */
class ChannelDevice : public Device
{
    Q_OBJECT

public:
    explicit ChannelDevice(QObject *parent = nullptr);
    ~ChannelDevice() override;

    /**
     * @brief Device type enumeration
     */
    enum class ChannelType {
        VirtioSerial,    // virtio-serial channel
        Unix             // Unix socket
    };

    /**
     * @brief Get device type
     */
    DeviceType deviceType() const override { return DeviceType::Channel; }

    /**
     * @brief Get device type name
     */
    QString deviceTypeName() const override { return "Channel"; }

    /**
     * @brief Get channel type
     */
    ChannelType channelType() const { return m_channelType; }

    /**
     * @brief Set channel type
     */
    void setChannelType(ChannelType type);

    /**
     * @brief Get target type
     */
    QString targetType() const { return m_targetType; }

    /**
     * @brief Set target type (e.g., virtio-serial)
     */
    void setTargetType(const QString &type);

    /**
     * @brief Get target name
     */
    QString targetName() const { return m_targetName; }

    /**
     * @brief Set target name (e.g., org.qemu.guest_agent.0)
     */
    void setTargetName(const QString &name);

    /**
     * @brief Get source path
     */
    QString sourcePath() const { return m_sourcePath; }

    /**
     * @brief Set source path for Unix socket
     */
    void setSourcePath(const QString &path);

    /**
     * @brief Get source mode
     */
    QString sourceMode() const { return m_sourceMode; }

    /**
     * @brief Set source mode (bind, connect)
     */
    void setSourceMode(const QString &mode);

    /**
     * @brief Get device address
     */
    QString address() const { return m_address; }

    /**
     * @brief Set device address
     */
    void setAddress(const QString &addr);

    /**
     * @brief Clone the device
     */
    ChannelDevice *clone() const;

    /**
     * @brief Check if device configuration is valid
     */
    bool isValid() const;

    /**
     * @brief Get XML description
     */
    QString toXML() const override;

    /**
     * @brief Configure from XML
     */
    bool fromXML(const QString &xml) override;

    /**
     * @brief Get device description for UI
     */
    QString description() const override { return targetName(); }

signals:
    void configurationChanged();

private:
    ChannelType m_channelType;
    QString m_targetType;
    QString m_targetName;
    QString m_sourcePath;
    QString m_sourceMode;
    QString m_address;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_CHANNELDEVICE_H
