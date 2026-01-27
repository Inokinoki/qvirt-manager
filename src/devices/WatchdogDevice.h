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

#ifndef QVIRT_DEVICES_WATCHDOGDEVICE_H
#define QVIRT_DEVICES_WATCHDOGDEVICE_H

#include "Device.h"
#include <QString>

namespace QVirt {

/**
 * @brief Watchdog device
 *
 * Represents a hardware watchdog device for VMs
 */
class WatchdogDevice : public Device
{
    Q_OBJECT

public:
    enum class WatchdogModel {
        I6300ESB,
        IB700,
        Diag288,
        ITCo,
        ASpeed
    };
    Q_ENUM(WatchdogModel)

    enum class WatchdogAction {
        Reset,
        Shutdown,
        Poweroff,
        Pause,
        None,
        Dump,
        InjectNMI
    };
    Q_ENUM(WatchdogAction)

    explicit WatchdogDevice(QObject *parent = nullptr);
    ~WatchdogDevice() override = default;

    DeviceType deviceType() const override { return DeviceType::Watchdog; }
    QString deviceTypeName() const override { return QStringLiteral("Watchdog"); }

    // Properties
    WatchdogModel model() const { return m_model; }
    void setModel(WatchdogModel model);

    WatchdogAction action() const { return m_action; }
    void setAction(WatchdogAction action);

    // XML representation
    QString toXML() const override;
    bool fromXML(const QString &xml) override;

    // Description
    QString description() const override;

    // Static helpers
    static QString modelToString(WatchdogModel model);
    static WatchdogModel stringToModel(const QString &str);
    static QString actionToString(WatchdogAction action);
    static WatchdogAction stringToAction(const QString &str);

private:
    WatchdogModel m_model = WatchdogModel::I6300ESB;
    WatchdogAction m_action = WatchdogAction::Reset;
};

} // namespace QVirt

#endif // QVIRT_DEVICES_WATCHDOGDEVICE_H
