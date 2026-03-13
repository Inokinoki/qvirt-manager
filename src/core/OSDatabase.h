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

#ifndef QVIRT_CORE_OSDATABASE_H
#define QVIRT_CORE_OSDATABASE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>

#ifdef HAVE_LIBOSINFO
#include <libosinfo/osinfo.h>
#endif

namespace QVirt {

/**
 * @brief Operating System information
 */
struct OSInfo {
    QString id;
    QString name;
    QString shortId;
    QString family;
    QString variant;
    QString version;
    bool isEol;
    bool isGeneric;
    int releaseDate;
    int endOfLifeDate;
    
    OSInfo() : isEol(false), isGeneric(false), releaseDate(0), endOfLifeDate(0) {}
};

/**
 * @brief OS Database for VM creation and configuration
 *
 * Provides OS detection and information using libosinfo when available,
 * with fallback to built-in OS list.
 *
 * Features:
 * - OS detection from install media (ISO/URL)
 * - OS filtering and search
 * - EOL warnings
 * - Default configuration suggestions
 */
class OSDatabase : public QObject
{
    Q_OBJECT

public:
    explicit OSDatabase(QObject *parent = nullptr);
    ~OSDatabase() override;

    /**
     * @brief Get singleton instance
     */
    static OSDatabase *instance();

    /**
     * @brief Get list of all available OSes
     * @param filterEol Filter out end-of-life OSes
     * @return List of OS information
     */
    QList<OSInfo> getOperatingSystems(bool filterEol = false) const;

    /**
     * @brief Search for OS by name or ID
     * @param query Search query
     * @param filterEol Filter out EOL OSes
     * @return Matching OS list
     */
    QList<OSInfo> searchOperatingSystems(const QString &query, bool filterEol = false) const;

    /**
     * @brief Get OS by ID
     * @param id OS identifier
     * @return OS information or empty OSInfo if not found
     */
    OSInfo getOperatingSystem(const QString &id) const;

    /**
     * @brief Detect OS from install media
     * @param isoPath Path to ISO file
     * @return Detected OS information or empty if not detected
     */
    OSInfo detectOSFromISO(const QString &isoPath) const;

    /**
     * @brief Detect OS from install URL
     * @param url Installation URL
     * @return Detected OS information or empty if not detected
     */
    OSInfo detectOSFromURL(const QString &url) const;

    /**
     * @brief Get default RAM for OS
     * @param osId OS identifier
     * @return Recommended RAM in MB
     */
    int getDefaultRAM(const QString &osId) const;

    /**
     * @brief Get default disk size for OS
     * @param osId OS identifier
     * @return Recommended disk size in GB
     */
    int getDefaultDiskSize(const QString &osId) const;

    /**
     * @brief Check if libosinfo is available
     */
    bool hasLibosinfoSupport() const { return m_hasLibosinfo; }

    /**
     * @brief Check if OS is generic
     * @param osId OS identifier
     * @return True if generic OS
     */
    bool isGeneric(const QString &osId) const;

    /**
     * @brief Check if OS is end-of-life
     * @param osId OS identifier
     * @return True if EOL
     */
    bool isEndOfLife(const QString &osId) const;

signals:
    /**
     * @brief Emitted when OS database is loaded
     */
    void databaseLoaded();

private:
    void loadBuiltInOSList();
#ifdef HAVE_LIBOSINFO
    void loadLibosinfoOSList();
    OSInfo convertOsinfoObject(OsinfoOs *os) const;
#endif

    static OSDatabase *s_instance;
    QList<OSInfo> m_osList;
    QMap<QString, OSInfo> m_osMap;
    bool m_hasLibosinfo;
    bool m_loaded;
};

} // namespace QVirt

#endif // QVIRT_CORE_OSDATABASE_H
