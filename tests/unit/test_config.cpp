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
#include "../../src/core/Config.h"

using namespace QVirt;

/**
 * @brief Unit tests for Config class
 */
class TestConfig : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();

    void testSingleton();
    void testConnectionURIs();
    void testAutoconnect();
    void testDefaultValue();
    void testClear();
    void testPollingIntervals();
    void testConsoleSettings();

private:
    Config *m_config;
};

void TestConfig::initTestCase()
{
    m_config = Config::instance();
}

void TestConfig::cleanupTestCase()
{
    // Don't delete the singleton
}

void TestConfig::init()
{
    // Clear config before each test by removing all URIs
    QStringList uris = m_config->connectionURIs();
    for (const QString &uri : uris) {
        m_config->removeConnectionURI(uri);
    }
}

void TestConfig::testSingleton()
{
    QVERIFY(m_config != nullptr);
    QCOMPARE(m_config, Config::instance());
}

void TestConfig::testConnectionURIs()
{
    // Test adding multiple URIs
    m_config->addConnectionURI("qemu:///system");
    m_config->addConnectionURI("qemu:///session");
    m_config->addConnectionURI("qemu+tcp://host/system");

    QStringList uris = m_config->connectionURIs();
    QVERIFY(uris.contains("qemu:///system"));
    QVERIFY(uris.contains("qemu:///session"));
    QVERIFY(uris.contains("qemu+tcp://host/system"));

    // Test removing URI
    m_config->removeConnectionURI("qemu:///system");
    QVERIFY(!m_config->connectionURIs().contains("qemu:///system"));
}

void TestConfig::testAutoconnect()
{
    m_config->addConnectionURI("qemu:///system");
    m_config->setConnAutoconnect("qemu:///system", true);

    QVERIFY(m_config->connAutoconnect("qemu:///system"));
    QVERIFY(!m_config->connAutoconnect("qemu:///session"));

    // Test setting to false
    m_config->setConnAutoconnect("qemu:///system", false);
    QVERIFY(!m_config->connAutoconnect("qemu:///system"));
}

void TestConfig::testDefaultValue()
{
    m_config->setDefaultStoragePath("/var/lib/libvirt/images");
    QCOMPARE(m_config->defaultStoragePath(), QString("/var/lib/libvirt/images"));

    m_config->setConfirmForceOff(true);
    QVERIFY(m_config->confirmForceOff());

    m_config->setConfirmForceOff(false);
    QVERIFY(!m_config->confirmForceOff());
}

void TestConfig::testClear()
{
    m_config->addConnectionURI("qemu:///system");
    m_config->setDefaultStoragePath("/tmp");

    // Manually clear the config
    m_config->removeConnectionURI("qemu:///system");
    m_config->setDefaultStoragePath("");

    QVERIFY(m_config->connectionURIs().isEmpty());
    QVERIFY(m_config->defaultStoragePath().isEmpty());
}

void TestConfig::testPollingIntervals()
{
    // Test VM update interval
    m_config->setVMUpdateInterval(2000);
    QCOMPARE(m_config->vmUpdateInterval(), 2000);

    // Test CPU polling interval
    m_config->setCPUPollInterval(1000);
    QCOMPARE(m_config->cpuPollInterval(), 1000);

    // Test disk polling interval
    m_config->setDiskPollInterval(3000);
    QCOMPARE(m_config->diskPollInterval(), 3000);

    // Test network polling interval
    m_config->setNetworkPollInterval(2000);
    QCOMPARE(m_config->networkPollInterval(), 2000);
}

void TestConfig::testConsoleSettings()
{
    // Test console scaling
    m_config->setConsoleScale(true);
    QVERIFY(m_config->consoleScale());

    // Test resize guest setting
    m_config->setConsoleResizeGuest(true);
    QVERIFY(m_config->consoleResizeGuest());

    // Test auto redirect USB
    m_config->setConsoleRedirectUSB(true);
    QVERIFY(m_config->consoleRedirectUSB());

    // Test grab keyboard combination
    m_config->setConsoleKeyCombo("Ctrl+Alt");
    QCOMPARE(m_config->consoleKeyCombo(), QString("Ctrl+Alt"));
}

QTEST_MAIN(TestConfig)
#include "test_config.moc"
