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

#include "EnumMapper.h"
#include "StoragePool.h"
#include "Network.h"
#include "StorageVolume.h"
#include <QObject>

namespace QVirt {

QString EnumMapper::stateToString(virDomainState state)
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
        return stateToString(state);
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

QString EnumMapper::stateToString(int state)
{
    return domainStatusToString(state);
}

QString EnumMapper::poolStateToString(StoragePool::PoolState state)
{
    switch (state) {
    case StoragePool::StateInactive:
        return QObject::tr("Inactive");
    case StoragePool::StateRunning:
        return QObject::tr("Running");
    case StoragePool::StateDegraded:
        return QObject::tr("Degraded");
    default:
        return QObject::tr("Unknown");
    }
}

QString EnumMapper::poolTypeToString(StoragePool::PoolType type)
{
    switch (type) {
    case StoragePool::TypeDir:
        return QObject::tr("Directory");
    case StoragePool::TypeFS:
        return QObject::tr("Filesystem");
    case StoragePool::TypeNetFS:
        return QObject::tr("Network Filesystem");
    case StoragePool::TypeLogical:
        return QObject::tr("Logical Volume Manager");
    case StoragePool::TypeDisk:
        return QObject::tr("Physical Disk");
    case StoragePool::TypeISCSI:
        return QObject::tr("iSCSI");
    case StoragePool::TypeSCSI:
        return QObject::tr("SCSI");
    case StoragePool::TypeMPATH:
        return QObject::tr("Multipath");
    case StoragePool::TypeRBD:
        return QObject::tr("RBD (Ceph)");
    case StoragePool::TypeSheepdog:
        return QObject::tr("Sheepdog");
    case StoragePool::TypeGlusterFS:
        return QObject::tr("GlusterFS");
    case StoragePool::TypeZFS:
        return QObject::tr("ZFS");
    case StoragePool::TypeVStorage:
        return QObject::tr("VStorage");
    default:
        return QObject::tr("Unknown");
    }
}

QString EnumMapper::volumeTypeToString(StorageVolume::VolumeType type)
{
    switch (type) {
    case StorageVolume::TypeFile:
        return QObject::tr("File");
    case StorageVolume::TypeBlock:
        return QObject::tr("Block");
    case StorageVolume::TypeDir:
        return QObject::tr("Directory");
    case StorageVolume::TypeNetwork:
        return QObject::tr("Network");
    case StorageVolume::TypeNetDir:
        return QObject::tr("Network Directory");
    default:
        return QObject::tr("Unknown");
    }
}

QString EnumMapper::networkStateToString(Network::NetworkState state)
{
    switch (state) {
    case Network::StateInactive:
        return QObject::tr("Inactive");
    case Network::StateRunning:
        return QObject::tr("Running");
    default:
        return QObject::tr("Unknown");
    }
}

QString EnumMapper::forwardModeToString(Network::ForwardMode mode)
{
    switch (mode) {
    case Network::ForwardNAT:
        return QObject::tr("NAT");
    case Network::ForwardRoute:
        return QObject::tr("Route");
    case Network::ForwardBridge:
        return QObject::tr("Bridge");
    case Network::ForwardPrivate:
        return QObject::tr("Private");
    case Network::ForwardVEPA:
        return QObject::tr("VEPA");
    case Network::ForwardPassthrough:
        return QObject::tr("Passthrough");
    case Network::ForwardHostdev:
        return QObject::tr("Hostdev");
    default:
        return QObject::tr("Unknown");
    }
}

} // namespace QVirt
