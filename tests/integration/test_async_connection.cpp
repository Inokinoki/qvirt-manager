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
#include <QSignalSpy>
#include "../../src/libvirt/Connection.h"
#include "../../src/core/Config.h"

using namespace QVirt;

/**
 * @brief Integration tests for async Connection workflow
 *
 * Tests the async connection functionality including:
 * - openAsync() non-blocking behavior
 * - Signal emissions during connection lifecycle
 * - State transitions
 */
class TestAsyncConnection : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testAsyncConnectionCreation();
    void testAsyncConnectionSignals();
    void testCreateDisconnected();
    void testConnectionStateTransitions();
    void testAsyncConnectionNonBlocking();

private:
    bool hasLibvirt();
};

void TestAsyncConnection::initTestCase()
{
    // Check if libvirt is available
    if (!hasLibvirt()) {
        QSKIP("libvirt not available or not running");
    }
}

void TestAsyncConnection::cleanupTestCase()
{
    // Cleanup
}

bool TestAsyncConnection::hasLibvirt()
{
    // Try to connect to session to check if libvirt is available
    virConnectPtr conn = virConnectOpen("qemu:///session");
    if (conn) {
        virConnectClose(conn);
        return true;
    }
    return false;
}

void TestAsyncConnection::testAsyncConnectionCreation()
{
    // Test creating a connection using createDisconnected (for async workflow)
    Connection *conn = Connection::createDisconnected("qemu:///session");
    QVERIFY(conn != nullptr);
    QVERIFY(!conn->isOpen());
    QCOMPARE(conn->uri(), QString("qemu:///session"));
    QCOMPARE(conn->state(), Connection::Disconnected);

    delete conn;
}

void TestAsyncConnection::testAsyncConnectionSignals()
{
    // Create a disconnected connection for async testing
    Connection *conn = Connection::createDisconnected("qemu:///session");
    QVERIFY(conn != nullptr);

    // Set up signal spy to capture stateChanged signals
    QSignalSpy stateSpy(conn, &Connection::stateChanged);
    QVERIFY(stateSpy.isValid());

    // Set up signal spy for connectionProgress signals
    QSignalSpy progressSpy(conn, &Connection::connectionProgress);
    QVERIFY(progressSpy.isValid());

    // Start async connection
    conn->openAsync(QString(), QString());

    // Wait for the async operation to complete (max 5 seconds)
    bool completed = stateSpy.wait(5000);

    // We should have received at least one state change signal
    // (Connecting -> Active or Connecting -> ConnectionFailed)
    QVERIFY(completed);
    QVERIFY(stateSpy.count() >= 1);

    // Verify final state is either Active or ConnectionFailed
    Connection::State finalState = conn->state();
    QVERIFY(finalState == Connection::Active ||
            finalState == Connection::ConnectionFailed);

    delete conn;
}

void TestAsyncConnection::testCreateDisconnected()
{
    // Test creating a disconnected connection (for cached VM display)
    Connection *conn = Connection::createDisconnected("qemu:///session");
    QVERIFY(conn != nullptr);
    QVERIFY(!conn->isOpen());
    QCOMPARE(conn->state(), Connection::Disconnected);
    QCOMPARE(conn->uri(), QString("qemu:///session"));

    // Should be able to call methods without crashing
    QCOMPARE(conn->domains().count(), 0);
    QCOMPARE(conn->networks().count(), 0);
    QCOMPARE(conn->storagePools().count(), 0);

    delete conn;
}

void TestAsyncConnection::testConnectionStateTransitions()
{
    // Use createDisconnected to get a disconnected connection object
    Connection *conn = Connection::createDisconnected("qemu:///session");
    QVERIFY(conn != nullptr);

    QSignalSpy stateSpy(conn, &Connection::stateChanged);
    QVERIFY(stateSpy.isValid());

    // Initial state should be Disconnected
    QCOMPARE(conn->state(), Connection::Disconnected);

    // Start async connection
    conn->openAsync(QString(), QString());

    // Wait for state changes
    bool completed = stateSpy.wait(5000);
    QVERIFY(completed);

    // If connection succeeded, verify Active state
    if (conn->state() == Connection::Active) {
        QVERIFY(conn->isOpen());

        // Verify we can access connection properties
        QVERIFY(!conn->hostname().isEmpty());
        QVERIFY(!conn->libvirtVersion().isEmpty());

        // Close the connection
        conn->close();
        QVERIFY(!conn->isOpen());
        QCOMPARE(conn->state(), Connection::Disconnected);
    }
    // If connection failed, that's also acceptable for this test
    // (might fail due to libvirt not running, permissions, etc.)

    delete conn;
}

void TestAsyncConnection::testAsyncConnectionNonBlocking()
{
    // This test verifies that openAsync() returns immediately
    // and doesn't block the calling thread

    // Use createDisconnected to get a disconnected connection object
    Connection *conn = Connection::createDisconnected("qemu:///session");
    QVERIFY(conn != nullptr);

    QSignalSpy stateSpy(conn, &Connection::stateChanged);
    QVERIFY(stateSpy.isValid());

    // Record time before calling openAsync
    QTime startTime = QTime::currentTime();

    // Call openAsync - should return immediately
    conn->openAsync(QString(), QString());

    // Record time after calling openAsync
    QTime endTime = QTime::currentTime();

    // The call should return almost immediately (< 100ms)
    // The actual connection happens in a background thread
    int elapsedMs = startTime.msecsTo(endTime);
    QVERIFY2(elapsedMs < 100, QByteArray("openAsync() took too long: " +
                                          QByteArray::number(elapsedMs) +
                                          "ms, should be < 100ms"));

    // Wait for the async operation to complete
    stateSpy.wait(5000);

    delete conn;
}

QTEST_MAIN(TestAsyncConnection)
#include "test_async_connection.moc"
