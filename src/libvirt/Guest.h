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

#ifndef QVIRT_LIBVIRT_GUEST_H
#define QVIRT_LIBVIRT_GUEST_H

#include "../core/BaseObject.h"
#include <QString>
#include <QList>
#include <QUuid>

#include "devices/DiskDevice.h"
#include "devices/NetworkDevice.h"

namespace QVirt {

class Connection;

/**
 * @brief Guest configuration for VM creation
 *
 * Builds libvirt domain XML from configuration parameters
 */
class Guest : public BaseObject
{
    Q_OBJECT

public:
    explicit Guest(Connection *conn, QObject *parent = nullptr);
    ~Guest() override;

    // Basic properties
    QString name() const { return m_name; }
    void setName(const QString &name);

    QString uuid() const { return m_uuid; }
    void setUUID(const QString &uuid);
    void generateUUID();

    QString title() const { return m_title; }
    void setTitle(const QString &title);

    QString description() const { return m_description; }
    void setDescription(const QString &desc);

    // OS settings
    QString osType() const { return m_osType; }
    void setOSType(const QString &type);

    QString osArch() const { return m_osArch; }
    void setOSArch(const QString &arch);

    QString osMachine() const { return m_osMachine; }
    void setOSMachine(const QString &machine);

    bool uefiRequested() const { return m_uefiRequested; }
    void setUefiRequested(bool uefi) { m_uefiRequested = uefi; }

    // Resources
    quint64 memory() const { return m_memory; } // in KB
    void setMemory(quint64 memKB);

    quint64 currentMemory() const { return m_currentMemory; } // in KB
    void setCurrentMemory(quint64 memKB);

    int vcpus() const { return m_vcpus; }
    void setVCPUs(int count);

    int maxVcpus() const { return m_maxVcpus; }
    void setMaxVCPUs(int count);

    // Features
    bool acpiEnabled() const { return m_acpiEnabled; }
    void setACPIEnabled(bool enabled) { m_acpiEnabled = enabled; }

    bool apicEnabled() const { return m_apicEnabled; }
    void setAPICEnabled(bool enabled) { m_apicEnabled = enabled; }

    // Devices
    QList<DiskDevice*> disks() const { return m_disks; }
    void addDisk(DiskDevice *disk);
    void removeDisk(DiskDevice *disk);
    void clearDisks();

    QList<NetworkDevice*> networkInterfaces() const { return m_networkInterfaces; }
    void addNetworkInterface(NetworkDevice *nic);
    void removeNetworkInterface(NetworkDevice *nic);
    void clearNetworkInterfaces();

    // Graphics
    bool hasGraphics() const { return m_hasGraphics; }
    void setHasGraphics(bool has) { m_hasGraphics = has; }

    QString graphicsType() const { return m_graphicsType; } // "vnc" or "spice"
    void setGraphicsType(const QString &type);

    bool graphicsListenAddress() const { return m_graphicsListen; }
    void setGraphicsListenAddress(bool listen) { m_graphicsListen = listen; }

    // XML generation
    QString toXML() const;
    QString toXMLPretty() const; // Formatted with indentation

    // Connection
    Connection *connection() const { return m_connection; }

    // Validation
    bool validate() const;
    QString validationError() const { return m_validationError; }

signals:
    void configChanged();

private:
    QString buildOSSection() const;
    QString buildFeaturesSection() const;
    QString buildCPUSection() const;
    QString buildDevicesSection() const;

    Connection *m_connection;

    // Basic
    QString m_name;
    QString m_uuid;
    QString m_title;
    QString m_description;

    // OS
    QString m_osType;
    QString m_osArch;
    QString m_osMachine;
    bool m_uefiRequested = false;

    // Resources
    quint64 m_memory = 1048576;      // 1 GB in KB
    quint64 m_currentMemory = 1048576;
    int m_vcpus = 1;
    int m_maxVcpus = 16;

    // Features
    bool m_acpiEnabled = true;
    bool m_apicEnabled = true;

    // Devices
    QList<DiskDevice*> m_disks;
    QList<NetworkDevice*> m_networkInterfaces;

    // Graphics
    bool m_hasGraphics = true;
    QString m_graphicsType = "spice";
    bool m_graphicsListen = true;

    // Validation
    mutable QString m_validationError;
};

} // namespace QVirt

#endif // QVIRT_LIBVIRT_GUEST_H
