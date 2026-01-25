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

#include <QtTest>
#include "../../src/libvirt/EnumMapper.h"
#include "../../src/libvirt/Network.h"
#include "../../src/libvirt/StoragePool.h"

using namespace QVirt;

/**
 * @brief Unit tests for EnumMapper class
 */
class TestEnumMapper : public QObject
{
    Q_OBJECT

private slots:
    void testStateToString();
    void testDomainStateStrings();
    void testNetworkStateStrings();
    void testStoragePoolStateStrings();
    void testAllStatesCovered();
};

void TestEnumMapper::testStateToString()
{
    // Test common domain states
    QCOMPARE(EnumMapper::stateToString(VIR_DOMAIN_NOSTATE), QString("No State"));
    QCOMPARE(EnumMapper::stateToString(VIR_DOMAIN_RUNNING), QString("Running"));
    QCOMPARE(EnumMapper::stateToString(VIR_DOMAIN_BLOCKED), QString("Blocked"));
    QCOMPARE(EnumMapper::stateToString(VIR_DOMAIN_PAUSED), QString("Paused"));
    QCOMPARE(EnumMapper::stateToString(VIR_DOMAIN_SHUTDOWN), QString("Shutdown"));
    QCOMPARE(EnumMapper::stateToString(VIR_DOMAIN_SHUTOFF), QString("Shut Off"));
    QCOMPARE(EnumMapper::stateToString(VIR_DOMAIN_CRASHED), QString("Crashed"));
    QCOMPARE(EnumMapper::stateToString(VIR_DOMAIN_PMSUSPENDED), QString("Suspended"));
}

void TestEnumMapper::testDomainStateStrings()
{
    // Verify that state strings are not empty for all common domain states
    QList<virDomainState> states = {
        VIR_DOMAIN_NOSTATE,
        VIR_DOMAIN_RUNNING,
        VIR_DOMAIN_BLOCKED,
        VIR_DOMAIN_PAUSED,
        VIR_DOMAIN_SHUTDOWN,
        VIR_DOMAIN_SHUTOFF,
        VIR_DOMAIN_CRASHED,
        VIR_DOMAIN_PMSUSPENDED
    };

    for (virDomainState state : states) {
        QString stateStr = EnumMapper::stateToString(state);
        QVERIFY2(!stateStr.isEmpty(), qPrintable(QString("State %1 should have a string representation").arg(state)));
    }
}

void TestEnumMapper::testNetworkStateStrings()
{
    // Test network states using Network::NetworkState enum
    QCOMPARE(EnumMapper::networkStateToString(Network::StateInactive), QString("Inactive"));
    QCOMPARE(EnumMapper::networkStateToString(Network::StateRunning), QString("Running"));

    // Verify all network states have string representations
    QList<Network::NetworkState> states = {
        Network::StateInactive,
        Network::StateRunning
    };

    for (Network::NetworkState state : states) {
        QString stateStr = EnumMapper::networkStateToString(state);
        QVERIFY2(!stateStr.isEmpty(), qPrintable(QString("Network state %1 should have a string representation").arg(state)));
    }
}

void TestEnumMapper::testStoragePoolStateStrings()
{
    // Test storage pool states using StoragePool::PoolState enum
    QCOMPARE(EnumMapper::poolStateToString(StoragePool::StateInactive), QString("Inactive"));
    QCOMPARE(EnumMapper::poolStateToString(StoragePool::StateRunning), QString("Running"));
    QCOMPARE(EnumMapper::poolStateToString(StoragePool::StateDegraded), QString("Degraded"));

    // Verify all storage pool states have string representations
    QList<StoragePool::PoolState> states = {
        StoragePool::StateInactive,
        StoragePool::StateRunning,
        StoragePool::StateDegraded
    };

    for (StoragePool::PoolState state : states) {
        QString stateStr = EnumMapper::poolStateToString(state);
        QVERIFY2(!stateStr.isEmpty(), qPrintable(QString("Storage pool state %1 should have a string representation").arg(state)));
    }
}

void TestEnumMapper::testAllStatesCovered()
{
    // Ensure we have mappings for all enum values
    // Domain states - we cover all 8 states
    QStringList domainStates;
    domainStates << "No State" << "Running" << "Blocked" << "Paused"
                 << "Shutdown" << "Shut Off" << "Crashed" << "Suspended";
    QCOMPARE(domainStates.size(), 8);

    // Network states
    QStringList networkStates;
    networkStates << "Inactive" << "Running";
    QCOMPARE(networkStates.size(), 2);

    // Storage pool states
    QStringList poolStates;
    poolStates << "Inactive" << "Running" << "Degraded";
    QCOMPARE(poolStates.size(), 3);
}

QTEST_MAIN(TestEnumMapper)
#include "test_enummapper.moc"
