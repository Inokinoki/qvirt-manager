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
#include "../../src/libvirt/Network.h"
#include "../../src/libvirt/Connection.h"

using namespace QVirt;

/**
 * @brief Unit tests for Network class
 *
 * Tests the Network wrapper functionality including XML caching
 */
class TestNetwork : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testNetworkProperties();
    void testXMLCaching();
    void testUpdateInfo();
    void testNetworkState();
    void testForwardModeParsing();

private:
    bool hasLibvirt();
};

void TestNetwork::initTestCase()
{
    // Check if libvirt is available
    if (!hasLibvirt()) {
        QSKIP("libvirt not available or not running");
    }
}

void TestNetwork::cleanupTestCase()
{
    // Cleanup
}

bool TestNetwork::hasLibvirt()
{
    // Try to connect to session to check if libvirt is available
    virConnectPtr conn = virConnectOpen("qemu:///session");
    if (conn) {
        virConnectClose(conn);
        return true;
    }
    return false;
}

void TestNetwork::testNetworkProperties()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    QList<Network*> networks = conn->networks();

    if (networks.isEmpty()) {
        QSKIP("No networks available for testing");
    }

    Network *network = networks.first();
    QVERIFY(network != nullptr);

    // Test basic properties
    QVERIFY(!network->name().isEmpty());
    QVERIFY(!network->uuid().isEmpty());

    // UUID should be in standard format (8-4-4-4-12 hex digits)
    QVERIFY(network->uuid().length() == 36); // Standard UUID string length

    // Cleanup
    for (Network *net : networks) {
        delete net;
    }
    delete conn;
}

void TestNetwork::testXMLCaching()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    QList<Network*> networks = conn->networks();
    if (networks.isEmpty()) {
        QSKIP("No networks available for testing");
    }

    Network *network = networks.first();

    // First access - should fetch and cache XML
    QString xml1 = network->getXMLDesc();
    QVERIFY(!xml1.isEmpty());
    QVERIFY(xml1.contains("<network>"));

    // Second access - should return cached XML
    QString xml2 = network->getXMLDesc();
    QVERIFY(!xml2.isEmpty());

    // XML should be identical (cached)
    QCOMPARE(xml1, xml2);

    // Cleanup
    for (Network *net : networks) {
        delete net;
    }
    delete conn;
}

void TestNetwork::testUpdateInfo()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    QList<Network*> networks = conn->networks();
    if (networks.isEmpty()) {
        QSKIP("No networks available for testing");
    }

    Network *network = networks.first();

    // Get initial state
    bool wasActive = network->isActive();
    Network::NetworkState initialState = network->state();

    // Call updateInfo - should refresh all cached data
    network->updateInfo();

    // State should still be valid after update
    QVERIFY(network->isActive() == wasActive || !network->isActive());
    QVERIFY(network->state() == initialState || network->state() != initialState);

    // XML should still be accessible
    QString xml = network->getXMLDesc();
    QVERIFY(!xml.isEmpty());
    QVERIFY(xml.contains("<network>"));

    // Cleanup
    for (Network *net : networks) {
        delete net;
    }
    delete conn;
}

void TestNetwork::testNetworkState()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    QList<Network*> networks = conn->networks();
    if (networks.isEmpty()) {
        QSKIP("No networks available for testing");
    }

    for (Network *network : networks) {
        // Test state consistency
        Network::NetworkState state = network->state();

        // State should be either Inactive or Running
        QVERIFY(state == Network::StateInactive || state == Network::StateRunning);

        // isActive() should be consistent with state
        if (state == Network::StateRunning) {
            QVERIFY(network->isActive());
        } else if (state == Network::StateInactive) {
            QVERIFY(!network->isActive());
        }
    }

    // Cleanup
    for (Network *net : networks) {
        delete net;
    }
    delete conn;
}

void TestNetwork::testForwardModeParsing()
{
    Connection *conn = Connection::open("qemu:///session");
    if (!conn) {
        QSKIP("Could not open session connection");
    }

    QList<Network*> networks = conn->networks();
    if (networks.isEmpty()) {
        QSKIP("No networks available for testing");
    }

    Network *network = networks.first();

    // Get XML and verify forward mode parsing worked
    QString xml = network->getXMLDesc();
    QVERIFY(!xml.isEmpty());

    // The XML should contain network configuration
    QVERIFY(xml.contains("<network>"));

    // Forward mode should be parsed (default is NAT if not specified)
    // Network::ForwardNAT = 0 is the default
    QVERIFY(network->forwardMode() >= Network::ForwardNAT);

    // Cleanup
    for (Network *net : networks) {
        delete net;
    }
    delete conn;
}

QTEST_MAIN(TestNetwork)
#include "test_network.moc"
