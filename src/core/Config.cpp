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
#include <QDateTime>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QUuid>

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

ConnectionInfo Config::connectionInfo(const QString &uri) const
{
    ConnectionInfo info(uri);
    info.autoconnect = connAutoconnect(uri);
    info.sshKeyPath = connSSHKeyPath(uri);
    info.sshUsername = connSSHUsername(uri);
    return info;
}

bool Config::hasConnection(const QString &uri) const
{
    return connectionURIs().contains(uri);
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

void Config::setConnSSHKeyPath(const QString &uri, const QString &keyPath)
{
    m_settings.setValue(QString("Connection/%1/sshKeyPath").arg(uri), keyPath);
    emit valueChanged(QString("Connection/%1/sshKeyPath").arg(uri));
}

QString Config::connSSHKeyPath(const QString &uri) const
{
    return m_settings.value(QString("Connection/%1/sshKeyPath").arg(uri), QString()).toString();
}

void Config::setConnSSHUsername(const QString &uri, const QString &username)
{
    m_settings.setValue(QString("Connection/%1/sshUsername").arg(uri), username);
    emit valueChanged(QString("Connection/%1/sshUsername").arg(uri));
}

QString Config::connSSHUsername(const QString &uri) const
{
    return m_settings.value(QString("Connection/%1/sshUsername").arg(uri), QString()).toString();
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

// Helper to convert URI to filesystem-safe UUID-like name
QString Config::sanitizeUriToFilename(const QString &uri)
{
    // Generate a UUID-like hash from the URI using MD5 (16 bytes = 32 hex chars)
    // This creates a consistent, short, filesystem-safe identifier
    QByteArray hash = QCryptographicHash::hash(uri.toUtf8(), QCryptographicHash::Md5);
    QString hex = hash.toHex();

    // Format as UUID-like string: 8-4-4-4-12 pattern
    // Example: 550e8400-e29b-41d4-a716-446655440000
    QString uuid;
    uuid.reserve(36);
    uuid.append(hex.left(8));
    uuid.append('-');
    uuid.append(hex.mid(8, 4));
    uuid.append('-');
    uuid.append(hex.mid(12, 4));
    uuid.append('-');
    uuid.append(hex.mid(16, 4));
    uuid.append('-');
    uuid.append(hex.mid(20));

    return uuid;
}

// Get the cache directory for a connection
QDir Config::getVMCacheDir(const QString &uri) const
{
    // Use QStandardPaths::AppDataLocation for application data
    QString baseDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(baseDir);

    // Create all parent directories if they don't exist
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qWarning() << "Failed to create base directory:" << baseDir;
        }
    }

    // Create vm_cache subdirectory
    if (!dir.exists("vm_cache")) {
        if (!dir.mkdir("vm_cache")) {
            qWarning() << "Failed to create vm_cache directory";
        }
    }
    dir.cd("vm_cache");

    // Create connection-specific subdirectory using sanitized URI
    QString connDirName = sanitizeUriToFilename(uri);
    if (!dir.exists(connDirName)) {
        if (!dir.mkdir(connDirName)) {
            qWarning() << "Failed to create connection cache directory:" << connDirName;
        }
    }
    dir.cd(connDirName);

    return dir;
}

// VM Cache - save/load VM information per connection using XML files
void Config::saveVMCache(const QString &uri, const QString &uuid, const VMCacheInfo &info)
{
    QDir cacheDir = getVMCacheDir(uri);

    // Debug output
    qDebug() << "Saving VM cache for URI:" << uri << "UUID:" << uuid << "to:" << cacheDir.path();

    if (!cacheDir.exists()) {
        qWarning() << "Cache directory does not exist after creation:" << cacheDir.path();
        return;
    }

    // Create index file if it doesn't exist
    QString indexPath = cacheDir.filePath("index.xml");
    QDomDocument indexDoc;
    QStringList uuids;

    QFile indexFile(indexPath);
    if (indexFile.open(QIODevice::ReadOnly)) {
        indexDoc.setContent(&indexFile);
        QDomElement root = indexDoc.documentElement();
        if (!root.isNull()) {
            QDomNodeList uuidNodes = root.elementsByTagName("uuid");
            for (int i = 0; i < uuidNodes.count(); i++) {
                uuids.append(uuidNodes.at(i).toElement().text());
            }
        }
        indexFile.close();
    } else {
        // Create new index document
        QDomProcessingInstruction pi = indexDoc.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
        indexDoc.appendChild(pi);
        QDomElement root = indexDoc.createElement("vmindex");
        indexDoc.appendChild(root);
    }

    // Add UUID to index if not present
    bool isNew = !uuids.contains(uuid);
    if (isNew) {
        QDomElement root = indexDoc.documentElement();
        QDomElement uuidElem = indexDoc.createElement("uuid");
        uuidElem.appendChild(indexDoc.createTextNode(uuid));
        root.appendChild(uuidElem);
    }
    // Always update timestamp
    QDomElement root = indexDoc.documentElement();
    QDomElement timestampElem = indexDoc.createElement("lastUpdated");
    timestampElem.appendChild(indexDoc.createTextNode(QString::number(QDateTime::currentMSecsSinceEpoch())));
    // Remove old timestamp if exists and add new one
    QDomNodeList oldTimestamps = root.elementsByTagName("lastUpdated");
    if (!oldTimestamps.isEmpty()) {
        root.replaceChild(timestampElem, oldTimestamps.at(0));
    } else {
        root.appendChild(timestampElem);
    }

    QFile writeIndexFile(indexPath);
    if (writeIndexFile.open(QIODevice::WriteOnly)) {
        QTextStream out(&writeIndexFile);
        out.setEncoding(QStringConverter::Utf8);
        indexDoc.save(out, 2);
        writeIndexFile.close();
        qDebug() << "Updated index file, new VM:" << isNew;
    } else {
        qWarning() << "Failed to write index file:" << writeIndexFile.errorString();
    }

    // Save VM info to individual XML file
    QString vmFilePath = cacheDir.filePath(uuid + ".xml");
    QDomDocument vmDoc;

    // Create processing instruction and root element
    QDomProcessingInstruction pi = vmDoc.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
    vmDoc.appendChild(pi);

    QDomElement vmRoot = vmDoc.createElement("vmcache");
    vmDoc.appendChild(vmRoot);

    // Add metadata elements
    auto addTextElement = [&](const QString &tag, const QString &value) {
        QDomElement elem = vmDoc.createElement(tag);
        elem.appendChild(vmDoc.createTextNode(value));
        vmRoot.appendChild(elem);
    };

    addTextElement("name", info.name);
    addTextElement("uuid", info.uuid);
    addTextElement("state", QString::number(info.state));
    addTextElement("description", info.description);
    addTextElement("title", info.title);
    addTextElement("memory", QString::number(info.memory));
    addTextElement("currentMemory", QString::number(info.currentMemory));
    addTextElement("vcpuCount", QString::number(info.vcpuCount));
    addTextElement("maxVcpuCount", QString::number(info.maxVcpuCount));
    addTextElement("lastUpdated", QString::number(info.lastUpdated));

    // Add the full libvirt XML description as a CDATA section to preserve formatting
    QDomElement xmlDescElem = vmDoc.createElement("xmldesc");
    QDomCDATASection cdata = vmDoc.createCDATASection(info.xmlDesc);
    xmlDescElem.appendChild(cdata);
    vmRoot.appendChild(xmlDescElem);

    QFile vmFile(vmFilePath);
    if (vmFile.open(QIODevice::WriteOnly)) {
        QTextStream out(&vmFile);
        out.setEncoding(QStringConverter::Utf8);
        vmDoc.save(out, 2);
        vmFile.close();
        qDebug() << "Saved VM cache to:" << vmFilePath;
    } else {
        qWarning() << "Failed to write VM cache file:" << vmFilePath << "Error:" << vmFile.errorString();
    }

    emit valueChanged(QString("VMCache/%1").arg(uri));
}

VMCacheInfo Config::loadVMCache(const QString &uri, const QString &uuid) const
{
    VMCacheInfo info;
    QDir cacheDir = getVMCacheDir(uri);
    QString vmFilePath = cacheDir.filePath(uuid + ".xml");

    QFile vmFile(vmFilePath);
    if (vmFile.open(QIODevice::ReadOnly)) {
        QDomDocument doc;
        if (doc.setContent(&vmFile)) {
            QDomElement root = doc.documentElement();

            auto getElementText = [&](const QString &tag) -> QString {
                QDomNodeList nodes = root.elementsByTagName(tag);
                return nodes.isEmpty() ? QString() : nodes.at(0).toElement().text();
            };

            info.name = getElementText("name");
            info.uuid = getElementText("uuid");
            info.state = getElementText("state").toInt();
            info.description = getElementText("description");
            info.title = getElementText("title");
            info.memory = getElementText("memory").toULongLong();
            info.currentMemory = getElementText("currentMemory").toULongLong();
            info.vcpuCount = getElementText("vcpuCount").toInt();
            info.maxVcpuCount = getElementText("maxVcpuCount").toInt();
            info.lastUpdated = getElementText("lastUpdated").toLongLong();

            // Get XML description from CDATA section
            QDomNodeList xmlDescNodes = root.elementsByTagName("xmldesc");
            if (!xmlDescNodes.isEmpty()) {
                QDomElement xmlDescElem = xmlDescNodes.at(0).toElement();
                QDomNode child = xmlDescElem.firstChild();
                if (child.isCDATASection()) {
                    info.xmlDesc = child.toCDATASection().data();
                } else {
                    info.xmlDesc = child.toText().data();
                }
            }
        }
        vmFile.close();
    }

    return info;
}

QList<VMCacheInfo> Config::loadAllVMCache(const QString &uri) const
{
    QList<VMCacheInfo> vmList;
    QDir cacheDir = getVMCacheDir(uri);

    qDebug() << "=== loadAllVMCache ===";
    qDebug() << "URI:" << uri;
    qDebug() << "Cache directory:" << cacheDir.path();

    // Read index file
    QString indexPath = cacheDir.filePath("index.xml");
    QFile indexFile(indexPath);
    qDebug() << "Index file path:" << indexPath;
    qDebug() << "Index file exists:" << (indexFile.exists() ? "YES" : "NO");

    if (!indexFile.exists()) {
        qDebug() << "Index file does NOT exist!";
        return vmList;
    }

    if (indexFile.open(QIODevice::ReadOnly)) {
        QDomDocument doc;
        if (doc.setContent(&indexFile)) {
            QDomElement root = doc.documentElement();
            QDomNodeList uuidNodes = root.elementsByTagName("uuid");
            qDebug() << "Found" << uuidNodes.count() << "UUIDs in index file";

            for (int i = 0; i < uuidNodes.count(); i++) {
                QString uuid = uuidNodes.at(i).toElement().text();
                qDebug() << "  Loading VM with UUID:" << uuid;
                VMCacheInfo info = loadVMCache(uri, uuid);
                if (!info.name.isEmpty()) {
                    qDebug() << "    Loaded VM:" << info.name;
                    vmList.append(info);
                } else {
                    qDebug() << "    FAILED to load VM (empty name)";
                }
            }
        }
        indexFile.close();
    }

    qDebug() << "Total VMs loaded:" << vmList.count();
    return vmList;
}

void Config::removeVMCache(const QString &uri, const QString &uuid)
{
    QDir cacheDir = getVMCacheDir(uri);
    QString vmFilePath = cacheDir.filePath(uuid + ".xml");

    // Remove VM file
    QFile::remove(vmFilePath);

    // Update index
    QString indexPath = cacheDir.filePath("index.xml");
    QFile indexFile(indexPath);
    if (indexFile.exists() && indexFile.open(QIODevice::ReadOnly)) {
        QDomDocument doc;
        if (doc.setContent(&indexFile)) {
            QDomElement root = doc.documentElement();
            QDomNodeList uuidNodes = root.elementsByTagName("uuid");

            // Find and remove the UUID node
            for (int i = 0; i < uuidNodes.count(); i++) {
                if (uuidNodes.at(i).toElement().text() == uuid) {
                    uuidNodes.at(i).parentNode().removeChild(uuidNodes.at(i));
                    break;
                }
            }

            // Update timestamp
            QDomElement timestampElem = doc.createElement("lastUpdated");
            timestampElem.appendChild(doc.createTextNode(QString::number(QDateTime::currentMSecsSinceEpoch())));
            QDomNodeList oldTimestamps = root.elementsByTagName("lastUpdated");
            if (!oldTimestamps.isEmpty()) {
                root.replaceChild(timestampElem, oldTimestamps.at(0));
            } else {
                root.appendChild(timestampElem);
            }

            if (indexFile.open(QIODevice::WriteOnly)) {
                QTextStream out(&indexFile);
                out.setEncoding(QStringConverter::Utf8);
                doc.save(out, 2);
                indexFile.close();
            }
        }
    }

    emit valueChanged(QString("VMCache/%1").arg(uri));
}

void Config::clearVMCache(const QString &uri)
{
    QDir cacheDir = getVMCacheDir(uri);

    // Remove all XML files in the cache directory
    QFileInfoList files = cacheDir.entryInfoList(QStringList() << "*.xml", QDir::Files);
    for (const QFileInfo &file : files) {
        QFile::remove(file.filePath());
    }

    emit valueChanged(QString("VMCache/%1").arg(uri));
}

QStringList Config::cachedVMUUIDs(const QString &uri) const
{
    QStringList uuids;
    QDir cacheDir = getVMCacheDir(uri);

    // Read index file
    QString indexPath = cacheDir.filePath("index.xml");
    QFile indexFile(indexPath);
    if (indexFile.exists() && indexFile.open(QIODevice::ReadOnly)) {
        QDomDocument doc;
        if (doc.setContent(&indexFile)) {
            QDomElement root = doc.documentElement();
            QDomNodeList uuidNodes = root.elementsByTagName("uuid");
            for (int i = 0; i < uuidNodes.count(); i++) {
                uuids.append(uuidNodes.at(i).toElement().text());
            }
        }
        indexFile.close();
    }

    return uuids;
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
