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

#ifndef QVIRT_DOMAIN_MEMTUNE_H
#define QVIRT_DOMAIN_MEMTUNE_H

#include "../core/BaseObject.h"

namespace QVirt {

/**
 * @brief Memory tuning configuration (memtune)
 *
 * Represents memory tuning parameters for a VM
 */
class MemTune : public BaseObject
{
    Q_OBJECT

public:
    explicit MemTune(QObject *parent = nullptr);
    ~MemTune() override = default;

    unsigned long long hardLimit() const { return m_hardLimit; }
    void setHardLimit(unsigned long long limit);

    unsigned long long softLimit() const { return m_softLimit; }
    void setSoftLimit(unsigned long long limit);

    unsigned long long swapHardLimit() const { return m_swapHardLimit; }
    void setSwapHardLimit(unsigned long long limit);

    QString toXML() const;
    bool fromXML(const QString &xml);

    bool isEmpty() const;

signals:
    void configChanged();

private:
    unsigned long long m_hardLimit = 0;
    unsigned long long m_softLimit = 0;
    unsigned long long m_swapHardLimit = 0;
};

} // namespace QVirt

#endif // QVIRT_DOMAIN_MEMTUNE_H
