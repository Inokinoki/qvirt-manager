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
#include "../../src/libvirt/Domain.h"

using namespace QVirt;

/**
 * @brief Integration tests for VM operations
 *
 * These tests require a working libvirt with test VMs
 */
class TestVMOperations : public QObject
{
    Q_OBJECT

public:
    TestVMOperations() : m_connection(nullptr), m_testDomain(nullptr) {}

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testDomainLookup();
    void testDomainInfo();
    void testDomainState();
    void testDomainXML();

private:
    Connection *m_connection;
    Domain *m_testDomain;
};

void TestVMOperations::initTestCase()
{
    m_connection = Connection::open("qemu:///session");
    if (!m_connection) {
        QSKIP("Could not open session connection");
    }

    // Try to find any domain for testing
    QList<Domain*> domains = m_connection->domains();
    if (!domains.isEmpty()) {
        m_testDomain = domains.first();
    } else {
        m_testDomain = nullptr;
    }
}

void TestVMOperations::cleanupTestCase()
{
    // Safely clean up, handling null pointers
    if (m_testDomain != nullptr) {
        delete m_testDomain;
        m_testDomain = nullptr;
    }

    if (m_connection != nullptr) {
        delete m_connection;
        m_connection = nullptr;
    }
}

void TestVMOperations::testDomainLookup()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    // Test lookup by name
    Domain *domainByName = m_connection->getDomain(m_testDomain->name());
    QVERIFY(domainByName != nullptr);
    QCOMPARE(domainByName->name(), m_testDomain->name());
    delete domainByName;

    // Test lookup by UUID
    Domain *domainByUUID = m_connection->getDomainByUUID(m_testDomain->uuid());
    QVERIFY(domainByUUID != nullptr);
    QCOMPARE(domainByUUID->uuid(), m_testDomain->uuid());
    delete domainByUUID;
}

void TestVMOperations::testDomainInfo()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    // Test basic properties
    QVERIFY(!m_testDomain->name().isEmpty());
    QVERIFY(!m_testDomain->uuid().isEmpty());
    QVERIFY(m_testDomain->vcpuCount() >= 0);
    QVERIFY(m_testDomain->maxMemory() > 0);
    QVERIFY(m_testDomain->currentMemory() > 0);
}

void TestVMOperations::testDomainState()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    // Test state - Domain::state() returns Domain::State enum
    Domain::State state = m_testDomain->state();

    // Test state is valid enum value
    QVERIFY(state == Domain::StateNoState ||
            state == Domain::StateRunning ||
            state == Domain::StateBlocked ||
            state == Domain::StatePaused ||
            state == Domain::StateShutdown ||
            state == Domain::StateShutOff ||
            state == Domain::StateCrashed ||
            state == Domain::StatePMSuspended);
}

void TestVMOperations::testDomainXML()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    // Test XML description
    QString xml = m_testDomain->getXMLDesc();
    QVERIFY(!xml.isEmpty());
    QVERIFY(xml.contains("<domain"));
    QVERIFY(xml.contains(m_testDomain->name()));
    QVERIFY(xml.contains(m_testDomain->uuid()));
}

QTEST_MAIN(TestVMOperations)
#include "test_vm_operations.moc"
