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

#include "WatchdogDevice.h"

namespace QVirt {

WatchdogDevice::WatchdogDevice(QObject *parent)
    : Device(parent)
{
}

void WatchdogDevice::setModel(WatchdogModel model)
{
    if (m_model != model) {
        m_model = model;
        emit configChanged();
    }
}

void WatchdogDevice::setAction(WatchdogAction action)
{
    if (m_action != action) {
        m_action = action;
        emit configChanged();
    }
}

QString WatchdogDevice::toXML() const
{
    return QString::fromLatin1("  <watchdog model='%1' action='%2'/>")
        .arg(modelToString(m_model))
        .arg(actionToString(m_action));
}

bool WatchdogDevice::fromXML(const QString &xml)
{
    Q_UNUSED(xml);
    return true;
}

QString WatchdogDevice::description() const
{
    return QString::fromLatin1("%1 (Action: %2)")
        .arg(modelToString(m_model))
        .arg(actionToString(m_action));
}

QString WatchdogDevice::modelToString(WatchdogModel model)
{
    switch (model) {
        case WatchdogModel::I6300ESB: return QStringLiteral("i6300esb");
        case WatchdogModel::IB700: return QStringLiteral("ib700");
        case WatchdogModel::Diag288: return QStringLiteral("diag288");
        case WatchdogModel::ITCo: return QStringLiteral("itco");
        case WatchdogModel::ASpeed: return QStringLiteral("aspeed");
        default: return QStringLiteral("i6300esb");
    }
}

WatchdogDevice::WatchdogModel WatchdogDevice::stringToModel(const QString &str)
{
    if (str == QLatin1String("ib700")) return WatchdogModel::IB700;
    if (str == QLatin1String("diag288")) return WatchdogModel::Diag288;
    if (str == QLatin1String("itco")) return WatchdogModel::ITCo;
    if (str == QLatin1String("aspeed")) return WatchdogModel::ASpeed;
    return WatchdogModel::I6300ESB;
}

QString WatchdogDevice::actionToString(WatchdogAction action)
{
    switch (action) {
        case WatchdogAction::Reset: return QStringLiteral("reset");
        case WatchdogAction::Shutdown: return QStringLiteral("shutdown");
        case WatchdogAction::Poweroff: return QStringLiteral("poweroff");
        case WatchdogAction::Pause: return QStringLiteral("pause");
        case WatchdogAction::None: return QStringLiteral("none");
        case WatchdogAction::Dump: return QStringLiteral("dump");
        case WatchdogAction::InjectNMI: return QStringLiteral("inject-nmi");
        default: return QStringLiteral("reset");
    }
}

WatchdogDevice::WatchdogAction WatchdogDevice::stringToAction(const QString &str)
{
    if (str == QLatin1String("shutdown")) return WatchdogAction::Shutdown;
    if (str == QLatin1String("poweroff")) return WatchdogAction::Poweroff;
    if (str == QLatin1String("pause")) return WatchdogAction::Pause;
    if (str == QLatin1String("none")) return WatchdogAction::None;
    if (str == QLatin1String("dump")) return WatchdogAction::Dump;
    if (str == QLatin1String("inject-nmi")) return WatchdogAction::InjectNMI;
    return WatchdogAction::Reset;
}

} // namespace QVirt
