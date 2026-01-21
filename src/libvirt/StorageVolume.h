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

#ifndef QVIRT_LIBVIRT_STORAGEVOLUME_H
#define QVIRT_LIBVIRT_STORAGEVOLUME_H

#include <QObject>
#include <QString>

#include <libvirt/libvirt.h>

typedef struct _virStorageVol virStorageVol;

namespace QVirt {

class StoragePool;

/**
 * @brief Storage Volume Wrapper
 *
 * Represents a storage volume in libvirt
 */
class StorageVolume : public QObject
{
    Q_OBJECT

public:
    enum VolumeType {
        TypeFile = 0,
        TypeBlock = 1,
        TypeDir = 2,
        TypeNetwork = 3,
        TypeNetDir = 4
    };

    explicit StorageVolume(virStorageVol *vol, StoragePool *pool, QObject *parent = nullptr);
    ~StorageVolume() override;

    // Basic information
    QString name() const;
    QString key() const;
    QString path() const;
    VolumeType type() const;

    // Capacity information
    qint64 capacity() const;     // Total size in bytes
    qint64 allocation() const;   // Allocated size in bytes
    QString format() const;

    // Operations
    bool delete_(unsigned int flags = 0);
    bool wipe(unsigned int flags = 0);
    bool download(const QString &path, unsigned int flags = 0);
    bool upload(const QString &path, unsigned int flags = 0);
    StorageVolume *clone(const QString &name, unsigned int flags = 0);

    // XML operations
    QString getXMLDesc(unsigned int flags = 0);

    // Refresh volume info
    void updateInfo();

private:
    virStorageVol *m_volume;
    StoragePool *m_pool;

    QString m_name;
    QString m_key;
    QString m_path;
    VolumeType m_type;
    qint64 m_capacity;
    qint64 m_allocation;
    QString m_format;

    void parseXML(const QString &xml);
};

} // namespace QVirt

#endif // QVIRT_LIBVIRT_STORAGEVOLUME_H
