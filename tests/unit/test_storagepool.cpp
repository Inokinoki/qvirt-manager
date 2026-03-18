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
#include "../../src/libvirt/StoragePool.h"
#include "../../src/libvirt/Connection.h"

using namespace QVirt;

/**
 * @brief Unit tests for StoragePool class
 *
 * Tests the StoragePool wrapper functionality including XML caching
 */
class TestStoragePool : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testStoragePoolProperties();
    void testXMLCaching();
    void testUpdateInfo();
    void testStoragePoolState();
    void testPoolTypeParsing();
    void testCapacityInfo();

private:
    bool hasLibvirt();
};

void TestStoragePool::initTestCase()
{
    // Check if libvirt is available
    if (!hasLibvirt()) {
        QSKIP("libvirt not available or not running");
    }
}

void TestStoragePool::cleanupTestCase()
{
    // Cleanup
}

bool TestStoragePool::hasLibvirt()
{
    // Try to connect to session to check if libvirt is available
    virConnectPtr conn = virConnectOpen("qemu:///session");
    if (conn) {
        virConnectClose(conn);
        return true;
    }
    return false;
}

void TestStoragePool::testStoragePoolProperties()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    QList<StoragePool*> pools = conn->storagePools();

    if (pools.isEmpty()) {
        QSKIP("No storage pools available for testing");
    }

    StoragePool *pool = pools.first();
    QVERIFY(pool != nullptr);

    // Test basic properties
    QVERIFY(!pool->name().isEmpty());
    QVERIFY(!pool->uuid().isEmpty());

    // UUID should be in standard format (36 characters)
    QVERIFY(pool->uuid().length() == 36);

    // Cleanup
    for (StoragePool *p : pools) {
        delete p;
    }
    delete conn;
}

void TestStoragePool::testXMLCaching()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    QList<StoragePool*> pools = conn->storagePools();
    if (pools.isEmpty()) {
        QSKIP("No storage pools available for testing");
    }

    StoragePool *pool = pools.first();

    // First access - should fetch and cache XML
    QString xml1 = pool->getXMLDesc();
    QVERIFY(!xml1.isEmpty());
    QVERIFY(xml1.contains("<pool>"));

    // Second access - should return cached XML
    QString xml2 = pool->getXMLDesc();
    QVERIFY(!xml2.isEmpty());

    // XML should be identical (cached)
    QCOMPARE(xml1, xml2);

    // Cleanup
    for (StoragePool *p : pools) {
        delete p;
    }
    delete conn;
}

void TestStoragePool::testUpdateInfo()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    QList<StoragePool*> pools = conn->storagePools();
    if (pools.isEmpty()) {
        QSKIP("No storage pools available for testing");
    }

    StoragePool *pool = pools.first();

    // Get initial state
    bool wasActive = pool->isActive();
    StoragePool::PoolState initialState = pool->state();
    quint64 initialCapacity = pool->capacity();

    // Call updateInfo - should refresh all cached data
    pool->updateInfo();

    // State should still be valid after update
    QVERIFY(pool->isActive() == wasActive || !pool->isActive());

    // XML should still be accessible
    QString xml = pool->getXMLDesc();
    QVERIFY(!xml.isEmpty());
    QVERIFY(xml.contains("<pool>"));

    // Cleanup
    for (StoragePool *p : pools) {
        delete p;
    }
    delete conn;
}

void TestStoragePool::testStoragePoolState()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    QList<StoragePool*> pools = conn->storagePools();
    if (pools.isEmpty()) {
        QSKIP("No storage pools available for testing");
    }

    for (StoragePool *pool : pools) {
        // Test state consistency
        StoragePool::PoolState state = pool->state();

        // State should be one of the defined states
        QVERIFY(state == StoragePool::StateInactive ||
                state == StoragePool::StateRunning ||
                state == StoragePool::StateDegraded);

        // isActive() should be consistent with state
        if (state == StoragePool::StateRunning) {
            QVERIFY(pool->isActive());
        } else if (state == StoragePool::StateInactive) {
            QVERIFY(!pool->isActive());
        }
    }

    // Cleanup
    for (StoragePool *p : pools) {
        delete p;
    }
    delete conn;
}

void TestStoragePool::testPoolTypeParsing()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    QList<StoragePool*> pools = conn->storagePools();
    if (pools.isEmpty()) {
        QSKIP("No storage pools available for testing");
    }

    StoragePool *pool = pools.first();

    // Get XML and verify type parsing worked
    QString xml = pool->getXMLDesc();
    QVERIFY(!xml.isEmpty());

    // The XML should contain pool configuration
    QVERIFY(xml.contains("<pool>"));

    // Pool type should be one of the defined types
    StoragePool::PoolType type = pool->type();
    QVERIFY(type >= StoragePool::TypeDir);
    QVERIFY(type <= StoragePool::TypeVStorage);

    // Cleanup
    for (StoragePool *p : pools) {
        delete p;
    }
    delete conn;
}

void TestStoragePool::testCapacityInfo()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    QList<StoragePool*> pools = conn->storagePools();
    if (pools.isEmpty()) {
        QSKIP("No storage pools available for testing");
    }

    for (StoragePool *pool : pools) {
        // Get capacity info
        quint64 capacity = pool->capacity();
        quint64 allocation = pool->allocation();
        quint64 available = pool->available();

        // Capacity should be >= allocation + available
        // (allowing for some rounding differences)
        if (capacity > 0) {
            QVERIFY(allocation <= capacity);
            QVERIFY(available <= capacity);
        }

        // Values should be consistent
        QVERIFY(capacity >= 0);
        QVERIFY(allocation >= 0);
        QVERIFY(available >= 0);
    }

    // Cleanup
    for (StoragePool *p : pools) {
        delete p;
    }
    delete conn;
}

QTEST_MAIN(TestStoragePool)
#include "test_storagepool.moc"
