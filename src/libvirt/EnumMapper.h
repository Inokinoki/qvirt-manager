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

#ifndef QVIRT_LIBVIRT_ENUMMAPPER_H
#define QVIRT_LIBVIRT_ENUMMAPPER_H

#include <QString>

#include <libvirt/libvirt.h>

namespace QVirt {

// Forward declaration to avoid circular dependency
class Domain;

/**
 * @brief Converts libvirt enums to/from strings
 *
 * Provides bidirectional conversion between libvit enum values
 * and their human-readable string representations.
 */
class EnumMapper
{
public:
    // Domain state
    static QString domainStateToString(virDomainState state);
    static virDomainState stringToDomainState(const QString &str);

    // Domain lifecycle actions
    static QString domainLifecycleActionToString(int action);

    // Device types
    static QString diskDeviceTypeToString(int type);
    static QString diskBusTypeToString(int type);
    static QString networkDeviceTypeToString(int type);

    // Error handling
    static QString virErrorToString(int code);

    // Helper to get human-readable status
    static QString domainStatusToString(virDomainState state);
    static QString domainStatusToString(int state);  // Generic overload for Domain::State
    static QString networkStatusToString(virNetworkPtr network);
    static QString storagePoolStatusToString(virStoragePoolPtr pool);

private:
    EnumMapper() = default;
};

} // namespace QVirt

#endif // QVIRT_LIBVIRT_ENUMMAPPER_H
