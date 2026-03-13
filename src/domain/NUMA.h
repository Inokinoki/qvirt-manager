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

#ifndef QVIRT_DOMAIN_NUMA_H
#define QVIRT_DOMAIN_NUMA_H

#include "../core/BaseObject.h"
#include <QString>
#include <QList>

namespace QVirt {

/**
 * @brief NUMA cell configuration
 */
class NUMACell
{
public:
    NUMACell();
    NUMACell(const NUMACell &other);
    NUMACell &operator=(const NUMACell &other);

    int id = 0;
    unsigned long long memory = 0;
    QString cpus;
    QString memAccess;

    bool isEmpty() const;
};

/**
 * @brief NUMA tuning configuration
 */
class NUMATune
{
public:
    NUMATune();
    NUMATune(const NUMATune &other);
    NUMATune &operator=(const NUMATune &other);

    QString mode;
    QString placement;

    bool isEmpty() const;
};

/**
 * @brief NUMA topology configuration
 *
 * Represents NUMA topology for a VM
 */
class NUMA : public BaseObject
{
    Q_OBJECT

public:
    explicit NUMA(QObject *parent = nullptr);
    ~NUMA() override = default;

    QList<NUMACell> cells() const { return m_cells; }
    void setCells(const QList<NUMACell> &cells);
    void addCell(const NUMACell &cell);

    NUMATune tune() const { return m_tune; }
    void setTune(const NUMATune &tune);

    QString toXML() const;
    bool fromXML(const QString &xml);

    bool isEmpty() const;

signals:
    void configChanged();

private:
    QList<NUMACell> m_cells;
    NUMATune m_tune;
};

} // namespace QVirt

#endif // QVIRT_DOMAIN_NUMA_H
