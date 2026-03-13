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

#ifndef QVIRT_DOMAIN_MEMORYBACKING_H
#define QVIRT_DOMAIN_MEMORYBACKING_H

#include "../core/BaseObject.h"
#include <QString>

namespace QVirt {

/**
 * @brief Hugepage configuration
 */
class Hugepage
{
public:
    Hugepage();
    Hugepage(const Hugepage &other);
    Hugepage &operator=(const Hugepage &other);

    unsigned long long size = 0;
    QString unit;
    QString nodeset;

    bool isEmpty() const;
};

/**
 * @brief Memory backing configuration
 *
 * Represents memory backing options (hugepages, locked, etc.)
 */
class MemoryBacking : public BaseObject
{
    Q_OBJECT

public:
    explicit MemoryBacking(QObject *parent = nullptr);
    ~MemoryBacking() override = default;

    bool locked() const { return m_locked; }
    void setLocked(bool locked);

    bool shared() const { return m_shared; }
    void setShared(bool shared);

    bool discard() const { return m_discard; }
    void setDiscard(bool discard);

    Hugepage hugepage() const { return m_hugepage; }
    void setHugepage(const Hugepage &page);

    QString toXML() const;
    bool fromXML(const QString &xml);

    bool isEmpty() const;

signals:
    void configChanged();

private:
    bool m_locked = false;
    bool m_shared = false;
    bool m_discard = false;
    Hugepage m_hugepage;
};

} // namespace QVirt

#endif // QVIRT_DOMAIN_MEMORYBACKING_H
