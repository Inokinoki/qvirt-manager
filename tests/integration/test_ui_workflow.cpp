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
#include <QApplication>
#include "../../src/core/Config.h"
#include "../../src/core/Engine.h"
#include "../../src/ui/manager/ManagerWindow.h"

using namespace QVirt;

/**
 * @brief Integration tests for UI workflow
 *
 * These tests the complete UI workflow
 */
class TestUIWorkflow : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testConfigInitialization();
    void testEngineSingleton();
    void testManagerWindowCreation();
    void testConfigPersistence();

private:
    int m_argc;
    char **m_argv;
};

void TestUIWorkflow::initTestCase()
{
    m_argc = 1;
    m_argv = new char*[1];
    m_argv[0] = strdup("test_ui_workflow");

    // Initialize QApplication if not already done
    if (!qApp) {
        new QApplication(m_argc, m_argv);
    }
}

void TestUIWorkflow::cleanupTestCase()
{
    free(m_argv[0]);
    delete[] m_argv;
}

void TestUIWorkflow::testConfigInitialization()
{
    Config *config = Config::instance();
    QVERIFY(config != nullptr);

    // Test default values
    QVERIFY(config->vmUpdateInterval() > 0);
    QVERIFY(config->cpuPollInterval() > 0);
}

void TestUIWorkflow::testEngineSingleton()
{
    Engine *engine = Engine::instance();
    QVERIFY(engine != nullptr);

    // Test singleton
    QCOMPARE(engine, Engine::instance());
}

void TestUIWorkflow::testManagerWindowCreation()
{
    // Test that manager window can be created
    ManagerWindow *window = new ManagerWindow();
    QVERIFY(window != nullptr);
    QVERIFY(!window->windowTitle().isEmpty());

    delete window;
}

void TestUIWorkflow::testConfigPersistence()
{
    Config *config = Config::instance();

    // Save some values
    config->addConnectionURI("qemu:///session");
    config->setVMUpdateInterval(3000);

    // Read back
    QStringList uris = config->connectionURIs();
    QVERIFY(uris.contains("qemu:///session"));
    QCOMPARE(config->vmUpdateInterval(), 3000);

    // Clear by removing the URI
    config->removeConnectionURI("qemu:///session");
    QVERIFY(!config->connectionURIs().contains("qemu:///session"));
}

QTEST_MAIN(TestUIWorkflow)
#include "test_ui_workflow.moc"
