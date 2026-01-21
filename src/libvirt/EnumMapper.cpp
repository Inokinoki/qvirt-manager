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

#include "EnumMapper.h"
#include <QObject>

namespace QVirt {

QString EnumMapper::domainStateToString(virDomainState state)
{
    switch (state) {
    case VIR_DOMAIN_NOSTATE:
        return QObject::tr("No State");
    case VIR_DOMAIN_RUNNING:
        return QObject::tr("Running");
    case VIR_DOMAIN_BLOCKED:
        return QObject::tr("Blocked");
    case VIR_DOMAIN_PAUSED:
        return QObject::tr("Paused");
    case VIR_DOMAIN_SHUTDOWN:
        return QObject::tr("Shutdown");
    case VIR_DOMAIN_SHUTOFF:
        return QObject::tr("Shut Off");
    case VIR_DOMAIN_CRASHED:
        return QObject::tr("Crashed");
    case VIR_DOMAIN_PMSUSPENDED:
        return QObject::tr("Suspended");
    default:
        return QObject::tr("Unknown");
    }
}

QString EnumMapper::domainStatusToString(virDomainState state)
{
    // Simplified status for UI display
    switch (state) {
    case VIR_DOMAIN_RUNNING:
        return QObject::tr("Running");
    case VIR_DOMAIN_PAUSED:
        return QObject::tr("Paused");
    case VIR_DOMAIN_SHUTOFF:
        return QObject::tr("Stopped");
    case VIR_DOMAIN_SHUTDOWN:
        return QObject::tr("Shutting down");
    default:
        return domainStateToString(state);
    }
}

QString EnumMapper::domainStatusToString(int state)
{
    // Generic overload for Domain::State - cast to virDomainState
    return domainStatusToString(static_cast<virDomainState>(state));
}

virDomainState EnumMapper::stringToDomainState(const QString &str)
{
    QString lower = str.toLower();
    if (lower == "running" || lower == "on")
        return VIR_DOMAIN_RUNNING;
    if (lower == "paused" || lower == "paused")
        return VIR_DOMAIN_PAUSED;
    if (lower == "shut off" || lower == "off" || lower == "stopped")
        return VIR_DOMAIN_SHUTOFF;
    if (lower == "shutdown")
        return VIR_DOMAIN_SHUTDOWN;
    if (lower == "crashed")
        return VIR_DOMAIN_CRASHED;
    if (lower == "suspended" || lower == "pmsuspended")
        return VIR_DOMAIN_PMSUSPENDED;
    return VIR_DOMAIN_NOSTATE;
}

QString EnumMapper::domainLifecycleActionToString(int action)
{
    // Note: VIR_DOMAIN_LIFECYCLE_* constants may not be available in all libvirt versions
    // Using generic string representation
    switch (action) {
    case 0:
        return QObject::tr("Power Off");
    case 1:
        return QObject::tr("Reboot");
    case 2:
        return QObject::tr("Crash");
    case 3:
        return QObject::tr("Destroy");
    default:
        return QObject::tr("Action %1").arg(action);
    }
}

QString EnumMapper::diskDeviceTypeToString(int type)
{
    // These constants vary by libvirt version - return generic descriptions
    switch (type) {
    case 0:
        return QObject::tr("Disk");
    case 1:
        return QObject::tr("CDROM");
    case 2:
        return QObject::tr("Floppy");
    case 3:
        return QObject::tr("LUN");
    default:
        return QObject::tr("Device %1").arg(type);
    }
}

QString EnumMapper::diskBusTypeToString(int type)
{
    // Return generic descriptions - actual bus types may vary by libvirt version
    switch (type) {
    case 0:
        return "IDE";
    case 1:
        return "SCSI";
    case 2:
        return "virtio";
    case 3:
        return "xen";
    case 4:
        return "USB";
    case 5:
        return "SATA";
    case 6:
        return "SD";
    default:
        return "Unknown";
    }
}

QString EnumMapper::networkDeviceTypeToString(int type)
{
    // Return generic descriptions - actual types may vary by libvirt version
    switch (type) {
    case 0:
        return QObject::tr("Virtual Network");
    case 1:
        return QObject::tr("Bridge");
    case 2:
        return QObject::tr("Ethernet");
    case 3:
        return QObject::tr("Direct");
    case 4:
        return QObject::tr("User Mode");
    default:
        return QObject::tr("Type %1").arg(type);
    }
}

QString EnumMapper::networkStatusToString(virNetworkPtr network)
{
    if (!network) {
        return QObject::tr("Inactive");
    }

    // Use isActive instead of virNetworkInfo which has macro conflicts
    int active = virNetworkIsActive(network);
    return (active == 1) ?
        QObject::tr("Active") : QObject::tr("Inactive");
}

QString EnumMapper::storagePoolStatusToString(virStoragePoolPtr pool)
{
    if (!pool) {
        return QObject::tr("Inactive");
    }

    // Use isActive instead of virStoragePoolInfo which has macro conflicts
    int active = virStoragePoolIsActive(pool);

    if (active < 0) {
        return QObject::tr("Unknown");
    }

    return (active == 1) ?
        QObject::tr("Active") : QObject::tr("Inactive");
}

QString EnumMapper::virErrorToString(int code)
{
    // Simplified error handling - just return the code
    // In the future we can use virLastError in Domain/Connection classes
    return QObject::tr("Error code: %1").arg(code);
}

} // namespace QVirt
