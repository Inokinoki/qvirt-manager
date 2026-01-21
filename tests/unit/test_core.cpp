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
#include "../../src/core/BaseObject.h"
#include "../../src/core/Config.h"

using namespace QVirt;

/**
 * @brief Unit tests for BaseObject class
 */
class TestBaseObject : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testConstruction();
    void testObjectName();
    void testParentChild();

private:
    BaseObject *m_baseObject;
};

void TestBaseObject::initTestCase()
{
    m_baseObject = nullptr;
}

void TestBaseObject::cleanupTestCase()
{
    delete m_baseObject;
    m_baseObject = nullptr;
}

void TestBaseObject::testConstruction()
{
    m_baseObject = new BaseObject();
    QVERIFY(m_baseObject != nullptr);
}

void TestBaseObject::testObjectName()
{
    m_baseObject = new BaseObject();
    m_baseObject->setObjectName("TestObject");
    QCOMPARE(m_baseObject->objectName(), QString("TestObject"));
}

void TestBaseObject::testParentChild()
{
    auto *parent = new BaseObject();
    auto *child = new BaseObject(parent);

    QCOMPARE(child->parent(), parent);

    delete parent;  // Child should be deleted automatically
}

/**
 * @brief Unit tests for Config class
 */
class TestConfig : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testSingleton();
    void testConnectionURIs();
    void testAutoconnect();
    void testDefaultValue();
    void testClear();

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

void TestConfig::testSingleton()
{
    QVERIFY(m_config != nullptr);
    QCOMPARE(m_config, Config::instance());
}

void TestConfig::testConnectionURIs()
{
    m_config->clear();

    QStringList uris;
    uris << "qemu:///system" << "qemu:///session";

    m_config->setConnectionURIs(uris);
    QCOMPARE(m_config->connectionURIs(), uris);
}

void TestConfig::testAutoconnect()
{
    m_config->clear();

    m_config->addConnectionURI("qemu:///system");
    m_config->setConnAutoconnect("qemu:///system", true);

    QVERIFY(m_config->connAutoconnect("qemu:///system"));
    QVERIFY(!m_config->connAutoconnect("qemu:///session"));
}

void TestConfig::testDefaultValue()
{
    m_config->clear();

    m_config->setDefaultStoragePath("/var/lib/libvirt/images");
    QCOMPARE(m_config->defaultStoragePath(), QString("/var/lib/libvirt/images"));
}

void TestConfig::testClear()
{
    m_config->addConnectionURI("qemu:///system");
    m_config->clear();

    QVERIFY(m_config->connectionURIs().isEmpty());
}

QTEST_MAIN(TestBaseObject)
#include "test_core.moc"
