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

#include "StorageVolume.h"
#include "StoragePool.h"
#include "../core/Error.h"

#include <QDomDocument>
#include <QDebug>

namespace QVirt {

StorageVolume::StorageVolume(virStorageVol *vol, StoragePool *pool, QObject *parent)
    : QObject(parent)
    , m_volume(vol)
    , m_pool(pool)
    , m_capacity(0)
    , m_allocation(0)
{
    if (!m_volume) {
        return;
    }

    // Get basic info
    const char *nameChars = virStorageVolGetName(m_volume);
    if (nameChars) {
        m_name = QString::fromUtf8(nameChars);
    }

    const char *keyChars = virStorageVolGetKey(m_volume);
    if (keyChars) {
        m_key = QString::fromUtf8(keyChars);
    }

    char *pathChars = virStorageVolGetPath(m_volume);
    if (pathChars) {
        m_path = QString::fromUtf8(pathChars);
        free(pathChars);
    }

    // Get and parse XML description
    char *xml = virStorageVolGetXMLDesc(m_volume, 0);
    if (xml) {
        parseXML(QString::fromUtf8(xml));
        free(xml);
    }
}

StorageVolume::~StorageVolume()
{
    if (m_volume) {
        virStorageVolFree(m_volume);
    }
}

QString StorageVolume::name() const
{
    return m_name;
}

QString StorageVolume::key() const
{
    return m_key;
}

QString StorageVolume::path() const
{
    return m_path;
}

StorageVolume::VolumeType StorageVolume::type() const
{
    return m_type;
}

qint64 StorageVolume::capacity() const
{
    return m_capacity;
}

qint64 StorageVolume::allocation() const
{
    return m_allocation;
}

QString StorageVolume::format() const
{
    return m_format;
}

bool StorageVolume::delete_(unsigned int flags)
{
    if (!m_volume) {
        return false;
    }

    if (virStorageVolDelete(m_volume, flags) == 0) {
        m_volume = nullptr;
        return true;
    }

    return false;
}

bool StorageVolume::wipe(unsigned int flags)
{
    if (!m_volume) {
        return false;
    }

    return virStorageVolWipe(m_volume, flags) == 0;
}

bool StorageVolume::download(const QString &path, unsigned int flags)
{
    Q_UNUSED(path);
    Q_UNUSED(flags);

    if (!m_volume) {
        return false;
    }

    // Download implementation with virStorageVolDownload requires stream management
    // This is reserved for future implementation when file download is needed
    return false;
}

bool StorageVolume::upload(const QString &path, unsigned int flags)
{
    Q_UNUSED(path);
    Q_UNUSED(flags);

    if (!m_volume) {
        return false;
    }

    // Upload implementation with virStorageVolUpload requires stream management
    // This is reserved for future implementation when file upload is needed
    return false;
}

StorageVolume *StorageVolume::clone(const QString &name, unsigned int flags)
{
    if (!m_volume || !m_pool) {
        return nullptr;
    }

    // Generate clone XML
    QString xml = QString("<volume>\n"
                          "  <name>%1</name>\n"
                          "  <capacity>%2</capacity>\n"
                          "  <target>\n"
                          "    <format type='%3'/>\n"
                          "  </target>\n"
                          "</volume>")
                      .arg(name)
                      .arg(m_capacity)
                      .arg(m_format);

    QByteArray xmlBytes = xml.toUtf8();
    virStorageVol *newVol = virStorageVolCreateXMLFrom(
        m_pool->virPool(), xmlBytes.constData(), m_volume, flags);

    if (newVol) {
        return new StorageVolume(newVol, m_pool, parent());
    }

    return nullptr;
}

QString StorageVolume::getXMLDesc(unsigned int flags)
{
    if (!m_volume) {
        return QString();
    }

    char *xml = virStorageVolGetXMLDesc(m_volume, flags);
    if (xml) {
        QString result = QString::fromUtf8(xml);
        free(xml);
        return result;
    }

    return QString();
}

void StorageVolume::updateInfo()
{
    if (!m_volume) {
        return;
    }

    // Refresh volume info by re-parsing XML
    char *xml = virStorageVolGetXMLDesc(m_volume, 0);
    if (xml) {
        parseXML(QString::fromUtf8(xml));
        free(xml);
    }
}

void StorageVolume::parseXML(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) {
        qWarning() << "Failed to parse volume XML";
        return;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "volume") {
        qWarning() << "Invalid volume XML";
        return;
    }

    // Parse type from target
    QDomNode targetNode = root.firstChildElement("target");
    if (!targetNode.isNull()) {
        QDomNode formatNode = targetNode.firstChildElement("format");
        if (!formatNode.isNull()) {
            m_format = formatNode.toElement().attribute("type");
        }
    }

    // Parse capacity
    QDomNode capacityNode = root.firstChildElement("capacity");
    if (!capacityNode.isNull()) {
        QString unit = capacityNode.toElement().attribute("unit", "bytes");
        m_capacity = capacityNode.toElement().text().toLongLong();

        // Convert to bytes if needed
        if (unit == "KiB") {
            m_capacity *= 1024;
        } else if (unit == "MiB") {
            m_capacity *= 1024 * 1024;
        } else if (unit == "GiB") {
            m_capacity *= 1024 * 1024 * 1024;
        } else if (unit == "TiB") {
            m_capacity *= 1024LL * 1024 * 1024 * 1024;
        }
    }

    // Parse allocation
    QDomNode allocationNode = root.firstChildElement("allocation");
    if (!allocationNode.isNull()) {
        QString unit = allocationNode.toElement().attribute("unit", "bytes");
        m_allocation = allocationNode.toElement().text().toLongLong();

        // Convert to bytes if needed
        if (unit == "KiB") {
            m_allocation *= 1024;
        } else if (unit == "MiB") {
            m_allocation *= 1024 * 1024;
        } else if (unit == "GiB") {
            m_allocation *= 1024 * 1024 * 1024;
        } else if (unit == "TiB") {
            m_allocation *= 1024LL * 1024 * 1024 * 1024;
        }
    }

    // Determine type from path/format
    if (m_path.contains("/dev/")) {
        m_type = TypeBlock;
    } else if (m_format == "dir") {
        m_type = TypeDir;
    } else if (m_format == "network") {
        m_type = TypeNetwork;
    } else {
        m_type = TypeFile;
    }
}

} // namespace QVirt
