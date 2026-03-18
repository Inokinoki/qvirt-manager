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
#include <QElapsedTimer>
#include "../../src/core/Config.h"
#include "../../src/core/Engine.h"
#include "../../src/ui/manager/ManagerWindow.h"
#include "../../src/ui/vmwindow/VMWindow.h"
#include "../../src/ui/wizards/CreateVMWizard.h"
#include "../../src/ui/dialogs/CloneDialog.h"
#include "../../src/ui/dialogs/SnapshotDialog.h"
#include "../../src/libvirt/Connection.h"
#include "../../src/libvirt/Domain.h"

using namespace QVirt;

/**
 * @brief Performance benchmark tests
 *
 * Dedicated performance tests to identify bottlenecks and regressions
 */
class TestPerformanceBenchmarks : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // Config performance tests
    void testConfigReadPerformance();
    void testConfigWritePerformance();

    // Engine performance tests
    void testEngineSingletonAccess();
    void testEngineInitialization();

    // Connection performance tests
    void testConnectionOpenClose();
    void testConnectionDomainEnumeration();

    // Domain performance tests
    void testDomainXMLParse();
    void testDomainStateQuery();
    void testDomainInfoQuery();

    // UI Component creation performance
    void testManagerWindowCreationTime();
    void testVMWindowCreationTime();
    void testCreateVMWizardCreationTime();

    // Model performance tests
    void testConnectionTreeModelPerformance();
    void testVMListModelPerformance();

    // Event handling performance
    void testSignalSlotPerformance();
    void testEventProcessingLatency();

    // Memory allocation tests
    void testMemoryAllocation();
    void testObjectPoolPerformance();

    // Threading performance
    void testConcurrentAccess();
    void testBackgroundTaskPerformance();

    // Stress tests
    void testRapidWindowCreation();
    void testHighFrequencyUpdates();

private:
    int m_argc;
    char **m_argv;
    Connection *m_connection;
    Domain *m_testDomain;

    struct BenchmarkResult {
        QString testName;
        qint64 elapsedMs;
        qint64 avgMs;
        int iterations;
        bool passed;
    };

    void recordBenchmark(const QString& name, qint64 elapsed, int iterations, qint64 threshold);
    QList<BenchmarkResult> m_results;
};

void TestPerformanceBenchmarks::initTestCase()
{
    m_argc = 1;
    m_argv = new char*[1];
    m_argv[0] = strdup("test_performance");

    // Initialize QApplication if not already done
    if (!qApp) {
        new QApplication(m_argc, m_argv);
    }

    m_connection = nullptr;
    m_testDomain = nullptr;

    // Try to open a test connection
    m_connection = Connection::open("qemu:///session");
    if (m_connection) {
        QList<Domain*> domains = m_connection->domains();
        if (!domains.isEmpty()) {
            m_testDomain = domains.first();
        }
    }

    // If no real domain, try test driver
    if (!m_testDomain && m_connection) {
        delete m_connection;
        m_connection = Connection::open("test:///default");
        if (m_connection) {
            QList<Domain*> domains = m_connection->domains();
            if (!domains.isEmpty()) {
                m_testDomain = domains.first();
            }
        }
    }
}

void TestPerformanceBenchmarks::cleanupTestCase()
{
    // Print summary
    qInfo() << "\n=== Performance Benchmark Summary ===";
    for (const BenchmarkResult& result : m_results) {
        QString status = result.passed ? "PASS" : "FAIL";
        qInfo() << qPrintable(QString("[%1] %2: %3ms (avg: %4ms, %5 iterations)")
                              .arg(status)
                              .arg(result.testName)
                              .arg(result.elapsedMs)
                              .arg(result.avgMs)
                              .arg(result.iterations));
    }

    if (m_connection) {
        delete m_connection;
        m_connection = nullptr;
    }

    free(m_argv[0]);
    delete[] m_argv;
}

void TestPerformanceBenchmarks::recordBenchmark(const QString& name, qint64 elapsed, int iterations, qint64 threshold)
{
    BenchmarkResult result;
    result.testName = name;
    result.elapsedMs = elapsed;
    result.avgMs = iterations > 0 ? elapsed / iterations : elapsed;
    result.iterations = iterations;
    result.passed = result.avgMs < threshold;
    m_results.append(result);
}

void TestPerformanceBenchmarks::testConfigReadPerformance()
{
    Config *config = Config::instance();
    QVERIFY(config != nullptr);

    QElapsedTimer timer;
    timer.start();

    int iterations = 1000;
    for (int i = 0; i < iterations; i++) {
        // Read various config values
        int updateInterval = config->vmUpdateInterval();
        int cpuPollInterval = config->cpuPollInterval();
        QStringList uris = config->connectionURIs();
        Q_UNUSED(updateInterval);
        Q_UNUSED(cpuPollInterval);
        Q_UNUSED(uris);
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("Config Read", elapsed, iterations, 100);

    QVERIFY2(elapsed < 100, qPrintable(QString("Config read too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testConfigWritePerformance()
{
    Config *config = Config::instance();

    QElapsedTimer timer;
    timer.start();

    int iterations = 100;
    for (int i = 0; i < iterations; i++) {
        config->setVMUpdateInterval(1000 + i);
        config->setCPUPollInterval(500 + (i % 100));
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("Config Write", elapsed, iterations, 50);

    QVERIFY2(elapsed < 50, qPrintable(QString("Config write too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testEngineSingletonAccess()
{
    QElapsedTimer timer;
    timer.start();

    int iterations = 10000;
    for (int i = 0; i < iterations; i++) {
        Engine *engine = Engine::instance();
        Q_UNUSED(engine);
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("Engine Singleton Access", elapsed, iterations, 50);

    QVERIFY2(elapsed < 50, qPrintable(QString("Engine singleton access too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testEngineInitialization()
{
    QElapsedTimer timer;
    timer.start();

    // Engine should already be initialized, test subsequent access
    Engine *engine = Engine::instance();
    QVERIFY(engine != nullptr);

    qint64 elapsed = timer.elapsed();
    recordBenchmark("Engine Initialization", elapsed, 1, 100);

    QVERIFY2(elapsed < 100, qPrintable(QString("Engine initialization too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testConnectionOpenClose()
{
    QElapsedTimer timer;
    timer.start();

    int iterations = 10;
    for (int i = 0; i < iterations; i++) {
        Connection *conn = Connection::open("test:///default");
        if (conn) {
            delete conn;
        }
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("Connection Open/Close", elapsed, iterations, 500);

    QVERIFY2(elapsed < 500, qPrintable(QString("Connection open/close too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testConnectionDomainEnumeration()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    QElapsedTimer timer;
    timer.start();

    int iterations = 100;
    for (int i = 0; i < iterations; i++) {
        QList<Domain*> domains = m_connection->domains();
        Q_UNUSED(domains);
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("Connection Domain Enumeration", elapsed, iterations, 200);

    QVERIFY2(elapsed < 200, qPrintable(QString("Domain enumeration too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testDomainXMLParse()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    QElapsedTimer timer;
    timer.start();

    int iterations = 100;
    for (int i = 0; i < iterations; i++) {
        QString xml = m_testDomain->getXMLDesc();
        QVERIFY(!xml.isEmpty());
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("Domain XML Parse", elapsed, iterations, 300);

    QVERIFY2(elapsed < 300, qPrintable(QString("Domain XML parse too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testDomainStateQuery()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    QElapsedTimer timer;
    timer.start();

    int iterations = 1000;
    for (int i = 0; i < iterations; i++) {
        Domain::State state = m_testDomain->state();
        Q_UNUSED(state);
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("Domain State Query", elapsed, iterations, 100);

    QVERIFY2(elapsed < 100, qPrintable(QString("Domain state query too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testDomainInfoQuery()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    QElapsedTimer timer;
    timer.start();

    int iterations = 100;
    for (int i = 0; i < iterations; i++) {
        int vcpuCount = m_testDomain->vcpuCount();
        qint64 maxMemory = m_testDomain->maxMemory();
        qint64 currentMemory = m_testDomain->currentMemory();
        Q_UNUSED(vcpuCount);
        Q_UNUSED(maxMemory);
        Q_UNUSED(currentMemory);
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("Domain Info Query", elapsed, iterations, 200);

    QVERIFY2(elapsed < 200, qPrintable(QString("Domain info query too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testManagerWindowCreationTime()
{
    QElapsedTimer timer;
    timer.start();

    int iterations = 10;
    for (int i = 0; i < iterations; i++) {
        ManagerWindow *window = new ManagerWindow();
        window->show();
        qApp->processEvents();
        delete window;
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("ManagerWindow Creation", elapsed, iterations, 500);

    QVERIFY2(elapsed < 500, qPrintable(QString("ManagerWindow creation too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testVMWindowCreationTime()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    QElapsedTimer timer;
    timer.start();

    int iterations = 10;
    for (int i = 0; i < iterations; i++) {
        VMWindow *window = new VMWindow(m_testDomain);
        window->show();
        qApp->processEvents();
        delete window;
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("VMWindow Creation", elapsed, iterations, 500);

    QVERIFY2(elapsed < 500, qPrintable(QString("VMWindow creation too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testCreateVMWizardCreationTime()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    QElapsedTimer timer;
    timer.start();

    int iterations = 10;
    for (int i = 0; i < iterations; i++) {
        CreateVMWizard *wizard = new CreateVMWizard(m_connection);
        wizard->show();
        qApp->processEvents();
        wizard->reject();
        delete wizard;
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("CreateVMWizard Creation", elapsed, iterations, 300);

    QVERIFY2(elapsed < 300, qPrintable(QString("CreateVMWizard creation too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testConnectionTreeModelPerformance()
{
    QElapsedTimer timer;
    timer.start();

    int iterations = 100;
    for (int i = 0; i < iterations; i++) {
        // Access model through ManagerWindow
        ManagerWindow *window = new ManagerWindow();
        QTreeView *treeView = window->findChild<QTreeView*>();
        if (treeView && treeView->model()) {
            QAbstractItemModel *model = treeView->model();
            int rowCount = model->rowCount();
            for (int row = 0; row < rowCount; row++) {
                QModelIndex index = model->index(row, 0);
                QVariant data = model->data(index);
                Q_UNUSED(data);
            }
        }
        delete window;
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("ConnectionTreeModel Performance", elapsed, iterations, 500);

    QVERIFY2(elapsed < 500, qPrintable(QString("ConnectionTreeModel too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testVMListModelPerformance()
{
    QElapsedTimer timer;
    timer.start();

    int iterations = 100;
    for (int i = 0; i < iterations; i++) {
        // Access VM list model through ManagerWindow
        ManagerWindow *window = new ManagerWindow();
        QTableView *tableView = window->findChild<QTableView*>();
        if (tableView && tableView->model()) {
            QAbstractItemModel *model = tableView->model();
            int rowCount = model->rowCount();
            int columnCount = model->columnCount();
            for (int row = 0; row < rowCount; row++) {
                for (int col = 0; col < columnCount; col++) {
                    QModelIndex index = model->index(row, col);
                    QVariant data = model->data(index);
                    Q_UNUSED(data);
                }
            }
        }
        delete window;
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("VMListModel Performance", elapsed, iterations, 500);

    QVERIFY2(elapsed < 500, qPrintable(QString("VMListModel too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testSignalSlotPerformance()
{
    QElapsedTimer timer;
    timer.start();

    int iterations = 10000;

    // Create a test signal/slot scenario
    for (int i = 0; i < iterations; i++) {
        Config *config = Config::instance();
        config->setVMUpdateInterval(1000 + i);
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("Signal/Slot Performance", elapsed, iterations, 100);

    QVERIFY2(elapsed < 100, qPrintable(QString("Signal/slot too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testEventProcessingLatency()
{
    QElapsedTimer timer;
    timer.start();

    int iterations = 100;
    for (int i = 0; i < iterations; i++) {
        qApp->processEvents();
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("Event Processing Latency", elapsed, iterations, 50);

    QVERIFY2(elapsed < 50, qPrintable(QString("Event processing too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testMemoryAllocation()
{
    QElapsedTimer timer;
    timer.start();

    int iterations = 1000;
    QList<QObject*> objects;

    for (int i = 0; i < iterations; i++) {
        QObject *obj = new QObject();
        objects.append(obj);
    }

    // Clean up
    qDeleteAll(objects);
    objects.clear();

    qint64 elapsed = timer.elapsed();
    recordBenchmark("Memory Allocation", elapsed, iterations, 100);

    QVERIFY2(elapsed < 100, qPrintable(QString("Memory allocation too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testObjectPoolPerformance()
{
    // Test rapid object creation/deletion (simulating object pool behavior)
    QElapsedTimer timer;
    timer.start();

    int iterations = 100;
    for (int i = 0; i < iterations; i++) {
        ManagerWindow *window = new ManagerWindow();
        // Immediately delete without showing
        delete window;
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("Object Pool Performance", elapsed, iterations, 300);

    QVERIFY2(elapsed < 300, qPrintable(QString("Object pool too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testConcurrentAccess()
{
    // QtConcurrent tests disabled - QtConcurrent not available
    QSKIP("QtConcurrent not available");
}

void TestPerformanceBenchmarks::testBackgroundTaskPerformance()
{
    // QtConcurrent tests disabled - QtConcurrent not available
    QSKIP("QtConcurrent not available");
}

void TestPerformanceBenchmarks::testRapidWindowCreation()
{
    QElapsedTimer timer;
    timer.start();

    int iterations = 20;
    for (int i = 0; i < iterations; i++) {
        ManagerWindow *window = new ManagerWindow();
        window->show();
        window->raise();
        window->activateWindow();
        qApp->processEvents();
        delete window;
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("Rapid Window Creation", elapsed, iterations, 2000);

    // This is a stress test, allow more time
    QVERIFY2(elapsed < 2000, qPrintable(QString("Rapid window creation too slow: %1ms").arg(elapsed)));
}

void TestPerformanceBenchmarks::testHighFrequencyUpdates()
{
    QElapsedTimer timer;
    timer.start();

    Config *config = Config::instance();

    int iterations = 500;
    for (int i = 0; i < iterations; i++) {
        // Simulate high-frequency config updates
        config->setVMUpdateInterval(1000 + (i % 100));
        config->setCPUPollInterval(500 + (i % 50));

        if (i % 50 == 0) {
            qApp->processEvents();
        }
    }

    qint64 elapsed = timer.elapsed();
    recordBenchmark("High Frequency Updates", elapsed, iterations, 200);

    QVERIFY2(elapsed < 200, qPrintable(QString("High frequency updates too slow: %1ms").arg(elapsed)));
}

QTEST_MAIN(TestPerformanceBenchmarks)
#include "test_performance_benchmarks.moc"
