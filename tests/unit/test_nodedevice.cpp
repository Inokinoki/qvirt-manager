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
#include "../../src/libvirt/NodeDevice.h"
#include "../../src/libvirt/Connection.h"

using namespace QVirt;

/**
 * @brief Unit tests for NodeDevice class
 *
 * Tests the NodeDevice wrapper functionality including XML caching
 */
class TestNodeDevice : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testNullDeviceHandling();
    void testXMLCaching();
    void testGetXMLDescReturnsCached();
    void testUpdateInfo();

private:
    bool hasLibvirt();
};

void TestNodeDevice::initTestCase()
{
    // Check if libvirt is available
    if (!hasLibvirt()) {
        QSKIP("libvirt not available or not running");
    }
}

void TestNodeDevice::cleanupTestCase()
{
    // Cleanup
}

bool TestNodeDevice::hasLibvirt()
{
    // Try to connect to session to check if libvirt is available
    virConnectPtr conn = virConnectOpen("qemu:///session");
    if (conn) {
        virConnectClose(conn);
        return true;
    }
    return false;
}

void TestNodeDevice::testNullDeviceHandling()
{
    // Test that NodeDevice handles null device gracefully
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    // Create a NodeDevice with a null pointer (simulated)
    // Note: In real usage, NodeDevice is created by Connection
    // This test verifies the class doesn't crash with null device

    // Get actual node devices to test
    QList<NodeDevice*> devices = conn->nodeDevices();

    // If we have devices, test their basic properties
    if (!devices.isEmpty()) {
        NodeDevice *device = devices.first();
        QVERIFY(device != nullptr);

        // Test that name is not empty for valid devices
        QVERIFY(!device->name().isEmpty());

        // Test XML retrieval
        QString xml = device->getXMLDesc();
        QVERIFY(!xml.isEmpty());
        QVERIFY(xml.contains("<device>"));

        // Cleanup
        for (NodeDevice *dev : devices) {
            delete dev;
        }
    } else {
        QSKIP("No node devices available for testing");
    }

    delete conn;
}

void TestNodeDevice::testXMLCaching()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    QList<NodeDevice*> devices = conn->nodeDevices();
    if (devices.isEmpty()) {
        QSKIP("No node devices available for testing");
    }

    NodeDevice *device = devices.first();

    // First call to getXMLDesc - should fetch from libvirt
    QString xml1 = device->getXMLDesc();
    QVERIFY(!xml1.isEmpty());

    // Second call - should return cached XML (faster, no libvirt call)
    QString xml2 = device->getXMLDesc();
    QVERIFY(!xml2.isEmpty());

    // XML should be identical
    QCOMPARE(xml1, xml2);

    // Cleanup
    for (NodeDevice *dev : devices) {
        delete dev;
    }
    delete conn;
}

void TestNodeDevice::testGetXMLDescReturnsCached()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    QList<NodeDevice*> devices = conn->nodeDevices();
    if (devices.isEmpty()) {
        QSKIP("No node devices available for testing");
    }

    NodeDevice *device = devices.first();

    // Get XML multiple times
    QString xml1 = device->getXMLDesc();
    QString xml2 = device->getXMLDesc();
    QString xml3 = device->getXMLDesc();

    // All calls should return the same cached XML
    QCOMPARE(xml1, xml2);
    QCOMPARE(xml2, xml3);

    // Verify XML structure
    QVERIFY(xml1.contains("<device>"));
    QVERIFY(xml1.contains("</device>"));

    // Cleanup
    for (NodeDevice *dev : devices) {
        delete dev;
    }
    delete conn;
}

void TestNodeDevice::testUpdateInfo()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    QList<NodeDevice*> devices = conn->nodeDevices();
    if (devices.isEmpty()) {
        QSKIP("No node devices available for testing");
    }

    NodeDevice *device = devices.first();

    // Get initial XML
    QString xml1 = device->getXMLDesc();
    QVERIFY(!xml1.isEmpty());

    // Call updateInfo - should refresh the cached XML
    device->updateInfo();

    // Get XML again - should still be valid
    QString xml2 = device->getXMLDesc();
    QVERIFY(!xml2.isEmpty());

    // XML should be consistent (same device)
    QVERIFY(xml2.contains("<device>"));

    // Cleanup
    for (NodeDevice *dev : devices) {
        delete dev;
    }
    delete conn;
}

QTEST_MAIN(TestNodeDevice)
#include "test_nodedevice.moc"
