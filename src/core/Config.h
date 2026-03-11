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

#ifndef QVIRT_CORE_CONFIG_H
#define QVIRT_CORE_CONFIG_H

#include <QObject>
#include <QSettings>
#include <QStringList>
#include <QMap>
#include <QSize>

namespace QVirt {

/**
 * @brief Connection information structure
 */
struct ConnectionInfo
{
    QString uri;
    bool autoconnect = false;
    QString sshKeyPath;
    QString sshUsername;

    ConnectionInfo() = default;
    explicit ConnectionInfo(const QString &uri) : uri(uri) {}
};

/**
 * @brief Application configuration manager
 *
 * Manages application settings using QSettings
 *
 * Mirrors the Python vmmConfig class from virt-manager
 */
class Config : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Get the singleton instance
     * @return Config instance
     */
    static Config *instance();

    // Connection settings
    void addConnectionURI(const QString &uri);
    void removeConnectionURI(const QString &uri);
    QStringList connectionURIs() const;

    ConnectionInfo connectionInfo(const QString &uri) const;
    bool hasConnection(const QString &uri) const;

    void setConnAutoconnect(const QString &uri, bool autoconnect);
    bool connAutoconnect(const QString &uri) const;

    // SSH credentials for connections
    void setConnSSHKeyPath(const QString &uri, const QString &keyPath);
    QString connSSHKeyPath(const QString &uri) const;

    void setConnSSHUsername(const QString &uri, const QString &username);
    QString connSSHUsername(const QString &uri) const;

    // Per-VM settings
    void setVMWindowSize(const QString &uri, const QString &uuid, const QSize &size);
    QSize vmWindowSize(const QString &uri, const QString &uuid, const QSize &defaultSize = QSize(800, 600)) const;

    // General settings
    void setConsoleResizeGuest(bool enable);
    bool consoleResizeGuest() const;

    void setUpdateInterval(int seconds);
    int updateInterval() const;

    void setXMLEDitorEnabled(bool enable);
    bool xmlEditorEnabled() const;

    // Confirmation settings
    void setConfirmForceOff(bool confirm);
    bool confirmForceOff() const;

    void setConfirmDelete(bool confirm);
    bool confirmDelete() const;

    // Connection settings
    void setAutoconnectOnStartup(bool autoconnect);
    bool autoconnectOnStartup() const;

    // Storage settings
    void setDefaultStoragePath(const QString &path);
    QString defaultStoragePath() const;

    // Polling settings
    void setVMUpdateInterval(int seconds);
    int vmUpdateInterval() const;

    void setCPUPollInterval(int seconds);
    int cpuPollInterval() const;

    void setDiskPollInterval(int seconds);
    int diskPollInterval() const;

    void setNetworkPollInterval(int seconds);
    int networkPollInterval() const;

    // Console settings
    void setConsoleScale(bool scale);
    bool consoleScale() const;

    void setConsoleKeyCombo(const QString &combo);
    QString consoleKeyCombo() const;

    void setConsoleRedirectUSB(bool redirect);
    bool consoleRedirectUSB() const;

signals:
    void valueChanged(const QString &key);

private:
    Config();
    ~Config() override = default;
    Config(const Config &) = delete;
    Config &operator=(const Config &) = delete;

    static Config *s_instance;

    QSettings m_settings;
};

} // namespace QVirt

#endif // QVIRT_CORE_CONFIG_H
