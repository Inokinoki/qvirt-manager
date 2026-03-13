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

#include "OSDatabase.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>

#ifdef HAVE_LIBOSINFO
#include <libosinfo/osinfo_loader.h>
#endif

namespace QVirt {

OSDatabase *OSDatabase::s_instance = nullptr;

OSDatabase::OSDatabase(QObject *parent)
    : QObject(parent)
    , m_hasLibosinfo(false)
    , m_loaded(false)
{
#ifdef HAVE_LIBOSINFO
    // Initialize libosinfo
    if (osinfo_loader_init(nullptr)) {
        m_hasLibosinfo = true;
        loadLibosinfoOSList();
    } else {
        qWarning() << "Failed to initialize libosinfo, using built-in list";
        loadBuiltInOSList();
    }
#else
    qInfo() << "libosinfo not available, using built-in OS list";
    loadBuiltInOSList();
#endif

    m_loaded = true;
    emit databaseLoaded();
}

OSDatabase::~OSDatabase()
{
#ifdef HAVE_LIBOSINFO
    if (m_hasLibosinfo) {
        // libosinfo cleanup handled by library
    }
#endif
}

OSDatabase *OSDatabase::instance()
{
    if (!s_instance) {
        s_instance = new OSDatabase();
    }
    return s_instance;
}

QList<OSInfo> OSDatabase::getOperatingSystems(bool filterEol) const
{
    if (filterEol) {
        QList<OSInfo> filtered;
        for (const OSInfo &os : m_osList) {
            if (!os.isEol) {
                filtered.append(os);
            }
        }
        return filtered;
    }
    return m_osList;
}

QList<OSInfo> OSDatabase::searchOperatingSystems(const QString &query, bool filterEol) const
{
    QList<OSInfo> results;
    QString lowerQuery = query.toLower();

    for (const OSInfo &os : m_osList) {
        if (filterEol && os.isEol) {
            continue;
        }

        if (os.name.toLower().contains(lowerQuery) ||
            os.id.toLower().contains(lowerQuery) ||
            os.shortId.toLower().contains(lowerQuery)) {
            results.append(os);
        }
    }

    return results;
}

OSInfo OSDatabase::getOperatingSystem(const QString &id) const
{
    if (m_osMap.contains(id)) {
        return m_osMap[id];
    }
    return OSInfo();
}

OSInfo OSDatabase::detectOSFromISO(const QString &isoPath) const
{
#ifdef HAVE_LIBOSINFO
    if (!m_hasLibosinfo) {
        return OSInfo();
    }

    // Use libosinfo to detect OS from ISO
    OsinfoMedia *media = osinfo_loader_load_from_media(isoPath.toUtf8().constData(), nullptr);
    if (media) {
        OsinfoOs *os = osinfo_media_get_os(media);
        if (os) {
            return convertOsinfoObject(os);
        }
    }
#else
    Q_UNUSED(isoPath);
#endif

    // Fallback: Try to detect from ISO filename
    QFileInfo fileInfo(isoPath);
    QString fileName = fileInfo.fileName().toLower();

    // Common patterns
    if (fileName.contains("ubuntu")) {
        return getOperatingSystem("ubuntu");
    } else if (fileName.contains("fedora")) {
        return getOperatingSystem("fedora");
    } else if (fileName.contains("debian")) {
        return getOperatingSystem("debian");
    } else if (fileName.contains("centos")) {
        return getOperatingSystem("centos");
    } else if (fileName.contains("windows") && fileName.contains("server")) {
        return getOperatingSystem("winserver");
    } else if (fileName.contains("windows")) {
        return getOperatingSystem("win");
    }

    return OSInfo();
}

OSInfo OSDatabase::detectOSFromURL(const QString &url) const
{
#ifdef HAVE_LIBOSINFO
    if (!m_hasLibosinfo) {
        return OSInfo();
    }

    // libosinfo can detect OS from some URLs
    // This would require network access and is more complex
    // For now, we'll use URL pattern matching
    Q_UNUSED(url);
#endif

    // Detect from URL pattern
    QString lowerUrl = url.toLower();

    if (lowerUrl.contains("ubuntu")) {
        return getOperatingSystem("ubuntu");
    } else if (lowerUrl.contains("fedora")) {
        return getOperatingSystem("fedora");
    } else if (lowerUrl.contains("debian")) {
        return getOperatingSystem("debian");
    } else if (lowerUrl.contains("centos")) {
        return getOperatingSystem("centos");
    } else if (lowerUrl.contains("archlinux")) {
        return getOperatingSystem("archlinux");
    }

    return OSInfo();
}

int OSDatabase::getDefaultRAM(const QString &osId) const
{
    OSInfo os = getOperatingSystem(osId);
    
    // Generic defaults based on OS family
    if (os.family.contains("windows")) {
        if (osId.contains("win10") || osId.contains("win11") || osId.contains("win201")) {
            return 4096; // Modern Windows needs more RAM
        }
        return 2048;
    } else if (os.family.contains("linux")) {
        if (os.isGeneric) {
            return 1024;
        }
        // Most modern Linux distros
        return 2048;
    }

    return 1024; // Default fallback
}

int OSDatabase::getDefaultDiskSize(const QString &osId) const
{
    OSInfo os = getOperatingSystem(osId);
    
    if (os.family.contains("windows")) {
        if (osId.contains("win10") || osId.contains("win11")) {
            return 64; // Windows 10/11 need more space
        }
        return 40;
    } else if (os.family.contains("linux")) {
        if (os.isGeneric) {
            return 10;
        }
        return 20; // Most Linux distros
    }

    return 10; // Default fallback
}

bool OSDatabase::isGeneric(const QString &osId) const
{
    OSInfo os = getOperatingSystem(osId);
    return os.isGeneric;
}

bool OSDatabase::isEndOfLife(const QString &osId) const
{
    OSInfo os = getOperatingSystem(osId);
    return os.isEol;
}

void OSDatabase::loadBuiltInOSList()
{
    // Built-in OS list when libosinfo is not available
    // This is a minimal list - libosinfo provides much more
    
    QList<OSInfo> builtInOS;
    
    // Generic OSes
    OSInfo genericLinux;
    genericLinux.id = "generic-linux";
    genericLinux.name = "Generic Linux";
    genericLinux.shortId = "linux";
    genericLinux.family = "linux";
    genericLinux.isGeneric = true;
    genericLinux.isEol = false;
    builtInOS.append(genericLinux);
    
    OSInfo genericWindows;
    genericWindows.id = "generic-windows";
    genericWindows.name = "Generic Windows";
    genericWindows.shortId = "windows";
    genericWindows.family = "windows";
    genericWindows.isGeneric = true;
    genericWindows.isEol = false;
    builtInOS.append(genericWindows);
    
    // Popular Linux distributions
    QStringList linuxDistros = {
        "ubuntu", "fedora", "debian", "centos", "rhel", 
        "suse", "opensuse", "archlinux", "gentoo", "mint"
    };
    
    for (const QString &distro : linuxDistros) {
        OSInfo os;
        os.id = distro;
        os.name = distro.toLower() == "rhel" ? "Red Hat Enterprise Linux" :
                  distro.toLower() == "suse" ? "SUSE Linux Enterprise" :
                  distro.toLower().replace(0, 1, distro[0].toUpper());
        os.shortId = distro;
        os.family = "linux";
        os.isGeneric = false;
        os.isEol = false;
        builtInOS.append(os);
    }
    
    // Windows versions
    QStringList windowsVersions = {
        "win11", "win10", "win8.1", "win8", "win7",
        "winserver2022", "winserver2019", "winserver2016"
    };
    
    for (const QString &win : windowsVersions) {
        OSInfo os;
        os.id = win;
        QString winName = win;
        os.name = win.contains("server") ? 
                  "Windows Server " + winName.remove("winserver") :
                  "Windows " + winName.remove("win").replace(".", " ");
        os.shortId = win;
        os.family = "windows";
        os.isGeneric = false;
        // Mark older versions as EOL
        os.isEol = win.contains("win7") || win.contains("win8") || 
                   win.contains("winserver2012") || win.contains("winserver2008");
        builtInOS.append(os);
    }
    
    // BSD
    OSInfo freebsd;
    freebsd.id = "freebsd";
    freebsd.name = "FreeBSD";
    freebsd.shortId = "freebsd";
    freebsd.family = "bsd";
    freebsd.isGeneric = false;
    freebsd.isEol = false;
    builtInOS.append(freebsd);
    
    m_osList = builtInOS;
    
    // Build map
    for (const OSInfo &os : m_osList) {
        m_osMap[os.id] = os;
    }
    
    qDebug() << "Loaded" << m_osList.size() << "OSes from built-in list";
}

#ifdef HAVE_LIBOSINFO
void OSDatabase::loadLibosinfoOSList()
{
    OsinfoDatabase *db = osinfo_database_new();
    OsinfoList *osList = nullptr;
    
    // Filter to get all OSes
    OsinfoFilter *filter = osinfo_filter_new();
    osinfo_database_apply_filter(db, filter, &osList);
    
    gint n = osinfo_list_get_length(osList);
    for (gint i = 0; i < n; i++) {
        OsinfoOs *os = OSINFO_OS(osinfo_list_get_product(osList, i));
        if (os) {
            OSInfo info = convertOsinfoObject(os);
            m_osList.append(info);
            m_osMap[info.id] = info;
        }
    }
    
    g_object_unref(osList);
    g_object_unref(filter);
    g_object_unref(db);
    
    qDebug() << "Loaded" << m_osList.size() << "OSes from libosinfo";
}

OSInfo OSDatabase::convertOsinfoObject(OsinfoOs *os) const
{
    OSInfo info;
    
    // Get basic properties
    const gchar *id = osinfo_entity_get_id(OSINFO_ENTITY(os));
    const gchar *name = osinfo_entity_get_name(OSINFO_ENTITY(os));
    const gchar *shortId = osinfo_entity_get_short_id(OSINFO_ENTITY(os));
    
    info.id = QString::fromUtf8(id ? id : "");
    info.name = QString::fromUtf8(name ? name : "");
    info.shortId = QString::fromUtf8(shortId ? shortId : "");
    
    // Get family
    OsinfoProductFamily *family = osinfo_os_get_family(os);
    if (family) {
        info.family = QString::fromUtf8(osinfo_entity_get_name(OSINFO_ENTITY(family)));
    }
    
    // Get version
    const gchar *version = osinfo_os_get_version(os);
    info.version = QString::fromUtf8(version ? version : "");
    
    // Check if generic
    info.isGeneric = osinfo_os_is_generic(os);
    
    // Get release date
    GDate *releaseDate = osinfo_os_get_release_date(os);
    if (releaseDate) {
        info.releaseDate = g_date_get_julian(releaseDate);
    }
    
    // Check EOL
    GDate *eolDate = osinfo_os_get_end_of_life_date(os);
    if (eolDate) {
        info.endOfLifeDate = g_date_get_julian(eolDate);
        info.isEol = (g_date_get_julian(eolDate) < g_date_get_julian_today());
    }
    
    return info;
}
#endif

} // namespace QVirt
