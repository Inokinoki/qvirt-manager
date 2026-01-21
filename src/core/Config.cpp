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

} // namespace QVirt
