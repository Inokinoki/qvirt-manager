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

#ifndef QVIRT_DOMAIN_CPUTUNE_H
#define QVIRT_DOMAIN_CPUTUNE_H

#include "../core/BaseObject.h"
#include <QString>
#include <QList>

namespace QVirt {

/**
 * @brief vCPU pinning information
 */
class VCPUPin
{
public:
    VCPUPin();
    VCPUPin(const VCPUPin &other);
    VCPUPin &operator=(const VCPUPin &other);

    int vcpu = 0;
    QString cpuset;

    bool isEmpty() const;
};

/**
 * @brief Emulator thread pinning
 */
class EmulatorPin
{
public:
    EmulatorPin();
    EmulatorPin(const EmulatorPin &other);
    EmulatorPin &operator=(const EmulatorPin &other);

    QString cpuset;

    bool isEmpty() const;
};

/**
 * @brief I/O thread pinning
 */
class IOThreadPin
{
public:
    IOThreadPin();
    IOThreadPin(const IOThreadPin &other);
    IOThreadPin &operator=(const IOThreadPin &other);

    int iothread = 0;
    QString cpuset;

    bool isEmpty() const;
};

/**
 * @brief CPU tuning configuration (cputune)
 *
 * Represents CPU tuning parameters for a VM
 */
class CPUTune : public BaseObject
{
    Q_OBJECT

public:
    explicit CPUTune(QObject *parent = nullptr);
    ~CPUTune() override = default;

    // CPU shares/period/quota
    unsigned long long shares() const { return m_shares; }
    void setShares(unsigned long long shares);

    unsigned long long period() const { return m_period; }
    void setPeriod(unsigned long long period);

    long long quota() const { return m_quota; }
    void setQuota(long long quota);

    int realtimePriority() const { return m_realtimePriority; }
    void setRealtimePriority(int priority);

    // vCPU pinning
    QList<VCPUPin> vcpuPins() const { return m_vcpuPins; }
    void setVcpuPins(const QList<VCPUPin> &pins);
    void addVcpuPin(const VCPUPin &pin);
    void removeVcpuPin(int vcpu);

    // Emulator pinning
    EmulatorPin emulatorPin() const { return m_emulatorPin; }
    void setEmulatorPin(const EmulatorPin &pin);

    // I/O thread pinning
    QList<IOThreadPin> iothreadPins() const { return m_iothreadPins; }
    void setIothreadPins(const QList<IOThreadPin> &pins);
    void addIothreadPin(const IOThreadPin &pin);

    QString toXML() const;
    bool fromXML(const QString &xml);

    bool isEmpty() const;

signals:
    void configChanged();

private:
    unsigned long long m_shares = 0;
    unsigned long long m_period = 0;
    long long m_quota = -1;
    int m_realtimePriority = -1;
    QList<VCPUPin> m_vcpuPins;
    EmulatorPin m_emulatorPin;
    QList<IOThreadPin> m_iothreadPins;
};

} // namespace QVirt

#endif // QVIRT_DOMAIN_CPUTUNE_H
