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
#include "../../src/libvirt/Connection.h"
#include "../../src/core/Config.h"

using namespace QVirt;

/**
 * @brief Integration tests for Connection lifecycle
 *
 * These tests require a working libvirt installation
 * Tests are designed to work with qemu:///session (user session)
 */
class TestConnectionLifecycle : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testOpenSessionConnection();
    void testOpenSystemConnection();
    void testConnectionProperties();
    void testListDomains();
    void testConnectionClose();

private:
    bool hasLibvirt();
};

void TestConnectionLifecycle::initTestCase()
{
    // Check if libvirt is available
    if (!hasLibvirt()) {
        QSKIP("libvirt not available or not running");
    }
}

void TestConnectionLifecycle::cleanupTestCase()
{
    // Cleanup
}

bool TestConnectionLifecycle::hasLibvirt()
{
    // Try to connect to session to check if libvirt is available
    virConnectPtr conn = virConnectOpen("qemu:///session");
    if (conn) {
        virConnectClose(conn);
        return true;
    }
    return false;
}

void TestConnectionLifecycle::testOpenSessionConnection()
{
    Connection *conn = Connection::open("qemu:///session");
    QVERIFY2(conn != nullptr, "Should be able to open session connection");

    if (conn) {
        QVERIFY(conn->isOpen());
        QVERIFY(!conn->uri().isEmpty());
        delete conn;
    }
}

void TestConnectionLifecycle::testOpenSystemConnection()
{
    // System connection may require root privileges
    Connection *conn = Connection::open("qemu:///system");

    // This might fail without root access, that's okay
    if (conn) {
        QVERIFY(conn->isOpen());
        delete conn;
    } else {
        QSKIP("System connection requires root privileges");
    }
}

void TestConnectionLifecycle::testConnectionProperties()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    // Test URI
    QVERIFY(!conn->uri().isEmpty());

    // Test hostname
    QVERIFY(!conn->hostname().isEmpty());

    // Test libvirt version (returns QString, not int)
    QString libvirtVer = conn->libvirtVersion();
    QVERIFY(!libvirtVer.isEmpty());

    delete conn;
}

void TestConnectionLifecycle::testListDomains()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    // Get all domains (active and inactive)
    QList<Domain*> domains = conn->domains();

    // Should get a valid list (even if empty)
    QVERIFY(domains != conn->domains());

    // Cleanup domains
    for (Domain *domain : domains) {
        delete domain;
    }

    delete conn;
}

void TestConnectionLifecycle::testConnectionClose()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    // Close connection
    conn->close();
    QVERIFY(!conn->isOpen());

    delete conn;
}

QTEST_MAIN(TestConnectionLifecycle)
#include "test_connection_lifecycle.moc"
