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

#ifndef QVIRT_DOMAIN_BLKIO_H
#define QVIRT_DOMAIN_BLKIO_H

#include "../core/BaseObject.h"

namespace QVirt {

/**
 * @brief Block I/O tuning configuration (blkio)
 *
 * Represents block I/O tuning parameters for a VM
 */
class BlkIOTune : public BaseObject
{
    Q_OBJECT

public:
    explicit BlkIOTune(QObject *parent = nullptr);
    ~BlkIOTune() override = default;

    unsigned int weight() const { return m_weight; }
    void setWeight(unsigned int weight);

    unsigned long long readBytesSec() const { return m_readBytesSec; }
    void setReadBytesSec(unsigned long long bytes);

    unsigned long long writeBytesSec() const { return m_writeBytesSec; }
    void setWriteBytesSec(unsigned long long bytes);

    unsigned long long readIopsSec() const { return m_readIopsSec; }
    void setReadIopsSec(unsigned long long iops);

    unsigned long long writeIopsSec() const { return m_writeIopsSec; }
    void setWriteIopsSec(unsigned long long iops);

    QString toXML() const;
    bool fromXML(const QString &xml);

    bool isEmpty() const;

signals:
    void configChanged();

private:
    unsigned int m_weight = 0;
    unsigned long long m_readBytesSec = 0;
    unsigned long long m_writeBytesSec = 0;
    unsigned long long m_readIopsSec = 0;
    unsigned long long m_writeIopsSec = 0;
};

} // namespace QVirt

#endif // QVIRT_DOMAIN_BLKIO_H
