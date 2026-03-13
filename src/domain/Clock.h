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

#ifndef QVIRT_DOMAIN_CLOCK_H
#define QVIRT_DOMAIN_CLOCK_H

#include "../core/BaseObject.h"
#include <QString>

namespace QVirt {

/**
 * @brief Timer configuration
 */
class Timer
{
public:
    Timer();
    Timer(const Timer &other);
    Timer &operator=(const Timer &other);

    QString name;
    QString tickPolicy;
    QString track;
    bool present = true;

    bool isEmpty() const;
};

/**
 * @brief Clock configuration
 *
 * Represents clock and timer settings for a VM
 */
class Clock : public BaseObject
{
    Q_OBJECT

public:
    explicit Clock(QObject *parent = nullptr);
    ~Clock() override = default;

    QString offset() const { return m_offset; }
    void setOffset(const QString &offset);

    Timer rtc() const { return m_rtc; }
    void setRtc(const Timer &timer);

    Timer hpet() const { return m_hpet; }
    void setHpet(const Timer &timer);

    Timer pit() const { return m_pit; }
    void setPit(const Timer &timer);

    bool kvmClock() const { return m_kvmClock; }
    void setKvmClock(bool enabled);

    QString toXML() const;
    bool fromXML(const QString &xml);

    bool isEmpty() const;

signals:
    void configChanged();

private:
    QString m_offset = "utc";
    Timer m_rtc;
    Timer m_hpet;
    Timer m_pit;
    bool m_kvmClock = true;
};

} // namespace QVirt

#endif // QVIRT_DOMAIN_CLOCK_H
