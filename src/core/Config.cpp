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

#include "Config.h"

namespace QVirt {

Config *Config::s_instance = nullptr;

Config::Config()
    : QObject(nullptr)
    , m_settings("qvirt-manager", "qvirt-manager")
{
}

Config *Config::instance()
{
    if (!s_instance) {
        s_instance = new Config();
    }
    return s_instance;
}

// Connection settings
void Config::addConnectionURI(const QString &uri)
{
    QStringList uris = connectionURIs();
    if (!uris.contains(uri)) {
        uris.append(uri);
        m_settings.setValue("Connections/uris", uris);
        emit valueChanged("Connections/uris");
    }
}

void Config::removeConnectionURI(const QString &uri)
{
    QStringList uris = connectionURIs();
    uris.removeAll(uri);
    m_settings.setValue("Connections/uris", uris);
    emit valueChanged("Connections/uris");
}

QStringList Config::connectionURIs() const
{
    return m_settings.value("Connections/uris", QStringList()).toStringList();
}

void Config::setConnAutoconnect(const QString &uri, bool autoconnect)
{
    m_settings.setValue(QString("Connection/%1/autoconnect").arg(uri), autoconnect);
    emit valueChanged(QString("Connection/%1/autoconnect").arg(uri));
}

bool Config::connAutoconnect(const QString &uri) const
{
    return m_settings.value(QString("Connection/%1/autoconnect").arg(uri), false).toBool();
}

// Per-VM settings
void Config::setVMWindowSize(const QString &uri, const QString &uuid, const QSize &size)
{
    QString key = QString("VMs/%1/%2/windowSize").arg(uri, uuid);
    m_settings.setValue(key, size);
    emit valueChanged(key);
}

QSize Config::vmWindowSize(const QString &uri, const QString &uuid, const QSize &defaultSize) const
{
    QString key = QString("VMs/%1/%2/windowSize").arg(uri, uuid);
    return m_settings.value(key, defaultSize).toSize();
}

// General settings
void Config::setConsoleResizeGuest(bool enable)
{
    m_settings.setValue("General/consoleResizeGuest", enable);
    emit valueChanged("General/consoleResizeGuest");
}

bool Config::consoleResizeGuest() const
{
    return m_settings.value("General/consoleResizeGuest", false).toBool();
}

void Config::setUpdateInterval(int seconds)
{
    m_settings.setValue("General/updateInterval", seconds);
    emit valueChanged("General/updateInterval");
}

int Config::updateInterval() const
{
    return m_settings.value("General/updateInterval", 2).toInt();
}

void Config::setXMLEDitorEnabled(bool enable)
{
    m_settings.setValue("General/xmlEditorEnabled", enable);
    emit valueChanged("General/xmlEditorEnabled");
}

bool Config::xmlEditorEnabled() const
{
    return m_settings.value("General/xmlEditorEnabled", true).toBool();
}

// Confirmation settings
void Config::setConfirmForceOff(bool confirm)
{
    m_settings.setValue("Confirmation/forceOff", confirm);
    emit valueChanged("Confirmation/forceOff");
}

bool Config::confirmForceOff() const
{
    return m_settings.value("Confirmation/forceOff", true).toBool();
}

void Config::setConfirmDelete(bool confirm)
{
    m_settings.setValue("Confirmation/delete", confirm);
    emit valueChanged("Confirmation/delete");
}

bool Config::confirmDelete() const
{
    return m_settings.value("Confirmation/delete", true).toBool();
}

// Connection settings
void Config::setAutoconnectOnStartup(bool autoconnect)
{
    m_settings.setValue("Connections/autoconnectOnStartup", autoconnect);
    emit valueChanged("Connections/autoconnectOnStartup");
}

bool Config::autoconnectOnStartup() const
{
    return m_settings.value("Connections/autoconnectOnStartup", true).toBool();
}

// Storage settings
void Config::setDefaultStoragePath(const QString &path)
{
    m_settings.setValue("Storage/defaultPath", path);
    emit valueChanged("Storage/defaultPath");
}

QString Config::defaultStoragePath() const
{
    return m_settings.value("Storage/defaultPath", "/var/lib/libvirt/images").toString();
}

// Polling settings
void Config::setVMUpdateInterval(int seconds)
{
    m_settings.setValue("Polling/vmUpdateInterval", seconds);
    emit valueChanged("Polling/vmUpdateInterval");
}

int Config::vmUpdateInterval() const
{
    return m_settings.value("Polling/vmUpdateInterval", 2).toInt();
}

void Config::setCPUPollInterval(int seconds)
{
    m_settings.setValue("Polling/cpuPollInterval", seconds);
    emit valueChanged("Polling/cpuPollInterval");
}

int Config::cpuPollInterval() const
{
    return m_settings.value("Polling/cpuPollInterval", 1).toInt();
}

void Config::setDiskPollInterval(int seconds)
{
    m_settings.setValue("Polling/diskPollInterval", seconds);
    emit valueChanged("Polling/diskPollInterval");
}

int Config::diskPollInterval() const
{
    return m_settings.value("Polling/diskPollInterval", 5).toInt();
}

void Config::setNetworkPollInterval(int seconds)
{
    m_settings.setValue("Polling/networkPollInterval", seconds);
    emit valueChanged("Polling/networkPollInterval");
}

int Config::networkPollInterval() const
{
    return m_settings.value("Polling/networkPollInterval", 3).toInt();
}

// Console settings
void Config::setConsoleScale(bool scale)
{
    m_settings.setValue("Console/scale", scale);
    emit valueChanged("Console/scale");
}

bool Config::consoleScale() const
{
    return m_settings.value("Console/scale", true).toBool();
}

void Config::setConsoleKeyCombo(const QString &combo)
{
    m_settings.setValue("Console/keyCombo", combo);
    emit valueChanged("Console/keyCombo");
}

QString Config::consoleKeyCombo() const
{
    return m_settings.value("Console/keyCombo", "ctrl+alt").toString();
}

void Config::setConsoleRedirectUSB(bool redirect)
{
    m_settings.setValue("Console/redirectUSB", redirect);
    emit valueChanged("Console/redirectUSB");
}

bool Config::consoleRedirectUSB() const
{
    return m_settings.value("Console/redirectUSB", false).toBool();
}

} // namespace QVirt
