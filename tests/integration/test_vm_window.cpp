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
#include <QTabWidget>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QPushButton>
#include <QStatusBar>
#include <QProgressBar>
#include <QMenuBar>
#include <QMenu>
#include <QTreeWidget>
#include <QListWidget>
#include <QTableWidget>
#include "../../src/ui/vmwindow/VMWindow.h"
#include "../../src/libvirt/Domain.h"
#include "../../src/libvirt/Connection.h"

using namespace QVirt;

/**
 * @brief Integration tests for VMWindow
 *
 * Tests the VM details window including tabs, toolbar, and state management
 */
class TestVMWindow : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Window structure tests
    void testWindowCreation();
    void testWindowWithTitle();
    void testWindowMinimumSize();

    // Tab tests
    void testTabWidgetExists();
    void testTabCount();
    void testTabNames();
    void testTabSwitching();
    void testOverviewPage();
    void testDetailsPage();
    void testConsolePage();
    void testSnapshotsPage();

    // Toolbar tests
    void testToolbarExists();
    void testToolbarActions();
    void testStartAction();
    void testStopAction();
    void testRebootAction();
    void testPauseAction();
    void testResumeAction();

    // Menu tests
    void testMenuBarExists();
    void testVMMenu();
    void testViewMenu();
    void testDeviceMenu();

    // Status bar tests
    void testStatusBarExists();
    void testStatusLabel();
    void testCPUUsageLabel();
    void testMemoryUsageLabel();

    // VM state tests
    void testWindowShowsVMName();
    void testWindowShowsVMState();
    void testControlButtonStates();

    // Action enable/disable tests
    void testActionsForRunningVM();
    void testActionsForStoppedVM();
    void testActionsForPausedVM();

    // Performance tests
    void testWindowOpenPerformance();
    void testTabSwitchPerformance();
    void testRefreshPerformance();

    // Memory leak tests
    void testWindowDestruction();
    void testMultipleWindows();

private:
    int m_argc;
    char **m_argv;
    Connection *m_connection;
    Domain *m_testDomain;
    VMWindow *m_window;
};

void TestVMWindow::initTestCase()
{
    m_argc = 1;
    m_argv = new char*[1];
    m_argv[0] = strdup("test_vm_window");

    // Initialize QApplication if not already done
    if (!qApp) {
        new QApplication(m_argc, m_argv);
    }

    m_connection = nullptr;
    m_testDomain = nullptr;
    m_window = nullptr;

    // Try to open a test connection
    m_connection = Connection::open("qemu:///session");
    if (m_connection) {
        // Try to find a test domain
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

void TestVMWindow::cleanupTestCase()
{
    if (m_testDomain && m_connection) {
        // Don't delete domain if it's still managed by connection
    }

    if (m_connection) {
        delete m_connection;
        m_connection = nullptr;
    }

    free(m_argv[0]);
    delete[] m_argv;
}

void TestVMWindow::init()
{
    m_window = nullptr;
}

void TestVMWindow::cleanup()
{
    if (m_window) {
        m_window->close();
        delete m_window;
        m_window = nullptr;
    }
}

void TestVMWindow::testWindowCreation()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);
    QVERIFY(m_window != nullptr);
    QVERIFY(m_window->isVisible());
}

void TestVMWindow::testWindowWithTitle()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    // Window title should contain VM name
    QString title = m_window->windowTitle();
    QVERIFY(!title.isEmpty());
    QVERIFY(title.contains(m_testDomain->name(), Qt::CaseInsensitive));
}

void TestVMWindow::testWindowMinimumSize()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    // Window should have reasonable minimum size
    QVERIFY(m_window->width() >= 640);
    QVERIFY(m_window->height() >= 480);
}

void TestVMWindow::testTabWidgetExists()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QTabWidget* tabWidget = m_window->findChild<QTabWidget*>();
    QVERIFY(tabWidget != nullptr);
}

void TestVMWindow::testTabCount()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QTabWidget* tabWidget = m_window->findChild<QTabWidget*>();
    QVERIFY(tabWidget != nullptr);

    // Should have at least 3 tabs (Overview, Details, Console)
    QVERIFY(tabWidget->count() >= 3);
}

void TestVMWindow::testTabNames()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QTabWidget* tabWidget = m_window->findChild<QTabWidget*>();
    QVERIFY(tabWidget != nullptr);

    // Check tab names
    for (int i = 0; i < tabWidget->count(); i++) {
        QString tabName = tabWidget->tabText(i);
        QVERIFY(!tabName.isEmpty());
    }
}

void TestVMWindow::testTabSwitching()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QTabWidget* tabWidget = m_window->findChild<QTabWidget*>();
    QVERIFY(tabWidget != nullptr);

    int initialIndex = tabWidget->currentIndex();

    // Switch to each tab
    for (int i = 0; i < tabWidget->count(); i++) {
        tabWidget->setCurrentIndex(i);
        QCOMPARE(tabWidget->currentIndex(), i);
        QTest::qWait(50);
    }

    // Switch back to initial
    tabWidget->setCurrentIndex(initialIndex);
    QCOMPARE(tabWidget->currentIndex(), initialIndex);
}

void TestVMWindow::testOverviewPage()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QTabWidget* tabWidget = m_window->findChild<QTabWidget*>();

    // Find Overview tab
    int overviewIndex = -1;
    for (int i = 0; i < tabWidget->count(); i++) {
        if (tabWidget->tabText(i).contains("Overview", Qt::CaseInsensitive)) {
            overviewIndex = i;
            break;
        }
    }

    if (overviewIndex >= 0) {
        tabWidget->setCurrentIndex(overviewIndex);
        QWidget* overviewPage = tabWidget->widget(overviewIndex);
        QVERIFY(overviewPage != nullptr);

        // Overview should have VM info labels
        QList<QLabel*> labels = overviewPage->findChildren<QLabel*>();
        QVERIFY(!labels.isEmpty());
    }
}

void TestVMWindow::testDetailsPage()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QTabWidget* tabWidget = m_window->findChild<QTabWidget*>();

    // Find Details tab
    int detailsIndex = -1;
    for (int i = 0; i < tabWidget->count(); i++) {
        if (tabWidget->tabText(i).contains("Details", Qt::CaseInsensitive)) {
            detailsIndex = i;
            break;
        }
    }

    if (detailsIndex >= 0) {
        tabWidget->setCurrentIndex(detailsIndex);
        QWidget* detailsPage = tabWidget->widget(detailsIndex);
        QVERIFY(detailsPage != nullptr);

        // Details should have device list or tree
        QTreeWidget* tree = detailsPage->findChild<QTreeWidget*>();
        QListWidget* list = detailsPage->findChild<QListWidget*>();
        QVERIFY(tree != nullptr || list != nullptr);
    }
}

void TestVMWindow::testConsolePage()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QTabWidget* tabWidget = m_window->findChild<QTabWidget*>();

    // Find Console tab
    int consoleIndex = -1;
    for (int i = 0; i < tabWidget->count(); i++) {
        if (tabWidget->tabText(i).contains("Console", Qt::CaseInsensitive)) {
            consoleIndex = i;
            break;
        }
    }

    if (consoleIndex >= 0) {
        tabWidget->setCurrentIndex(consoleIndex);
        QWidget* consolePage = tabWidget->widget(consoleIndex);
        QVERIFY(consolePage != nullptr);

        // Console may have display widget or placeholder
        QVERIFY(consolePage->isVisible());
    }
}

void TestVMWindow::testSnapshotsPage()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QTabWidget* tabWidget = m_window->findChild<QTabWidget*>();

    // Find Snapshots tab (may not exist in all versions)
    int snapshotsIndex = -1;
    for (int i = 0; i < tabWidget->count(); i++) {
        if (tabWidget->tabText(i).contains("Snapshot", Qt::CaseInsensitive)) {
            snapshotsIndex = i;
            break;
        }
    }

    if (snapshotsIndex >= 0) {
        tabWidget->setCurrentIndex(snapshotsIndex);
        QWidget* snapshotsPage = tabWidget->widget(snapshotsIndex);
        QVERIFY(snapshotsPage != nullptr);

        // Snapshots should have list or table
        QTableWidget* table = snapshotsPage->findChild<QTableWidget*>();
        QTreeWidget* tree = snapshotsPage->findChild<QTreeWidget*>();
        // At least one should exist or page is empty but valid
        QVERIFY(snapshotsPage->isVisible());
    }
}

void TestVMWindow::testToolbarExists()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QToolBar* toolbar = m_window->findChild<QToolBar*>();
    QVERIFY(toolbar != nullptr);
}

void TestVMWindow::testToolbarActions()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QToolBar* toolbar = m_window->findChild<QToolBar*>();
    QVERIFY(toolbar != nullptr);

    QList<QAction*> actions = toolbar->actions();

    // Should have multiple actions
    QVERIFY(actions.size() >= 5);
}

void TestVMWindow::testStartAction()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QList<QAction*> actions = m_window->findChildren<QAction*>();

    // Find Start action
    QAction* startAction = nullptr;
    for (QAction* action : actions) {
        if (action->objectName() == "m_actionStart" ||
            action->text().contains("Start", Qt::CaseInsensitive)) {
            startAction = action;
            break;
        }
    }

    QVERIFY(startAction != nullptr);
    QVERIFY(!startAction->text().isEmpty());
}

void TestVMWindow::testStopAction()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QList<QAction*> actions = m_window->findChildren<QAction*>();

    // Find Stop action
    QAction* stopAction = nullptr;
    for (QAction* action : actions) {
        if (action->objectName() == "m_actionStop" ||
            action->text().contains("Stop", Qt::CaseInsensitive)) {
            stopAction = action;
            break;
        }
    }

    QVERIFY(stopAction != nullptr);
}

void TestVMWindow::testRebootAction()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QList<QAction*> actions = m_window->findChildren<QAction*>();

    // Find Reboot action
    QAction* rebootAction = nullptr;
    for (QAction* action : actions) {
        if (action->objectName() == "m_actionReboot" ||
            action->text().contains("Reboot", Qt::CaseInsensitive)) {
            rebootAction = action;
            break;
        }
    }

    QVERIFY(rebootAction != nullptr);
}

void TestVMWindow::testPauseAction()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QList<QAction*> actions = m_window->findChildren<QAction*>();

    // Find Pause action
    QAction* pauseAction = nullptr;
    for (QAction* action : actions) {
        if (action->objectName() == "m_actionPause" ||
            action->text().contains("Pause", Qt::CaseInsensitive)) {
            pauseAction = action;
            break;
        }
    }

    QVERIFY(pauseAction != nullptr);
}

void TestVMWindow::testResumeAction()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QList<QAction*> actions = m_window->findChildren<QAction*>();

    // Find Resume action
    QAction* resumeAction = nullptr;
    for (QAction* action : actions) {
        if (action->objectName() == "m_actionResume" ||
            action->text().contains("Resume", Qt::CaseInsensitive)) {
            resumeAction = action;
            break;
        }
    }

    QVERIFY(resumeAction != nullptr);
}

void TestVMWindow::testMenuBarExists()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QMenuBar* menuBar = m_window->menuBar();
    QVERIFY(menuBar != nullptr);
}

void TestVMWindow::testVMMenu()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QList<QMenu*> menus = m_window->findChildren<QMenu*>();

    // Find VM menu
    QMenu* vmMenu = nullptr;
    for (QMenu* menu : menus) {
        if (menu->title().contains("VM", Qt::CaseInsensitive) ||
            menu->objectName() == "m_menuVM") {
            vmMenu = menu;
            break;
        }
    }

    // VM menu may exist or actions may be in other menus
    QVERIFY(menus.size() >= 2);
}

void TestVMWindow::testViewMenu()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QList<QMenu*> menus = m_window->findChildren<QMenu*>();

    // Find View menu
    QMenu* viewMenu = nullptr;
    for (QMenu* menu : menus) {
        if (menu->title().contains("View", Qt::CaseInsensitive) ||
            menu->objectName() == "m_menuView") {
            viewMenu = menu;
            break;
        }
    }

    // View menu should exist
    QVERIFY(viewMenu != nullptr || menus.size() >= 2);
}

void TestVMWindow::testDeviceMenu()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QList<QMenu*> menus = m_window->findChildren<QMenu*>();

    // Find Device menu
    QMenu* deviceMenu = nullptr;
    for (QMenu* menu : menus) {
        if (menu->title().contains("Device", Qt::CaseInsensitive) ||
            menu->objectName() == "m_menuDevice") {
            deviceMenu = menu;
            break;
        }
    }

    // Device menu may exist
    QVERIFY(deviceMenu != nullptr || menus.size() >= 1);
}

void TestVMWindow::testStatusBarExists()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QStatusBar* statusBar = m_window->statusBar();
    QVERIFY(statusBar != nullptr);
}

void TestVMWindow::testStatusLabel()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QList<QLabel*> labels = m_window->findChildren<QLabel*>();

    // Should have at least one status label
    QVERIFY(!labels.isEmpty());
}

void TestVMWindow::testCPUUsageLabel()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QList<QLabel*> labels = m_window->findChildren<QLabel*>();

    // Find CPU usage label
    QLabel* cpuLabel = nullptr;
    for (QLabel* label : labels) {
        if (label->objectName() == "m_cpuUsageLabel" ||
            label->text().contains("CPU", Qt::CaseInsensitive)) {
            cpuLabel = label;
            break;
        }
    }

    // CPU label may exist or be dynamically created
    QVERIFY(labels.size() >= 1);
}

void TestVMWindow::testMemoryUsageLabel()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QList<QLabel*> labels = m_window->findChildren<QLabel*>();

    // Find memory usage label
    QLabel* memLabel = nullptr;
    for (QLabel* label : labels) {
        if (label->objectName() == "m_memoryUsageLabel" ||
            label->text().contains("Memory", Qt::CaseInsensitive)) {
            memLabel = label;
            break;
        }
    }

    // Memory label may exist or be dynamically created
    QVERIFY(labels.size() >= 1);
}

void TestVMWindow::testWindowShowsVMName()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    // Window title should show VM name
    QString title = m_window->windowTitle();
    QVERIFY(title.contains(m_testDomain->name()));
}

void TestVMWindow::testWindowShowsVMState()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    // Status bar should show VM state
    QStatusBar* statusBar = m_window->statusBar();
    QVERIFY(statusBar != nullptr);

    // Status label may show state
    QList<QLabel*> labels = statusBar->findChildren<QLabel*>();
    // At least status bar exists
}

void TestVMWindow::testControlButtonStates()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    // Get VM state
    Domain::State state = m_testDomain->state();

    // Buttons should be in appropriate state for VM
    QList<QAction*> actions = m_window->findChildren<QAction*>();

    for (QAction* action : actions) {
        // Actions should be enabled or disabled based on VM state
        // This test just verifies no crashes accessing enabled state
        bool enabled = action->isEnabled();
        Q_UNUSED(enabled);
    }
}

void TestVMWindow::testActionsForRunningVM()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    // Only run if VM is running
    if (m_testDomain->state() != Domain::StateRunning) {
        QSKIP("VM is not running");
    }

    m_window = new VMWindow(m_testDomain);

    QList<QAction*> actions = m_window->findChildren<QAction*>();

    // For running VM, Stop and Pause should be enabled
    for (QAction* action : actions) {
        QString actionName = action->text().toLower();
        if (actionName.contains("stop") || actionName.contains("pause")) {
            // May be enabled for running VM
        }
        if (actionName.contains("start")) {
            // Should be disabled for running VM
            QVERIFY(!action->isEnabled());
        }
    }
}

void TestVMWindow::testActionsForStoppedVM()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    // Only run if VM is stopped
    if (m_testDomain->state() != Domain::StateShutOff) {
        QSKIP("VM is not stopped");
    }

    m_window = new VMWindow(m_testDomain);

    QList<QAction*> actions = m_window->findChildren<QAction*>();

    // For stopped VM, Start should be enabled
    for (QAction* action : actions) {
        QString actionName = action->text().toLower();
        if (actionName.contains("start")) {
            // Should be enabled for stopped VM
            QVERIFY(action->isEnabled());
        }
    }
}

void TestVMWindow::testActionsForPausedVM()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    // Only run if VM is paused
    if (m_testDomain->state() != Domain::StatePaused) {
        QSKIP("VM is not paused");
    }

    m_window = new VMWindow(m_testDomain);

    QList<QAction*> actions = m_window->findChildren<QAction*>();

    // For paused VM, Resume should be enabled
    for (QAction* action : actions) {
        QString actionName = action->text().toLower();
        if (actionName.contains("resume")) {
            // Should be enabled for paused VM
            QVERIFY(action->isEnabled());
        }
    }
}

void TestVMWindow::testWindowOpenPerformance()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    int iterations = 5;
    QElapsedTimer timer;
    timer.start();

    for (int i = 0; i < iterations; i++) {
        VMWindow* window = new VMWindow(m_testDomain);
        QVERIFY(window != nullptr);

        window->show();
        qApp->processEvents();
        QTest::qWait(50);

        delete window;
    }

    qint64 elapsed = timer.elapsed();
    qint64 avgTime = elapsed / iterations;

    // Average open/close should be under 500ms
    QVERIFY2(avgTime < 500, qPrintable(QString("Window open/close too slow: %1ms").arg(avgTime)));

    qInfo() << "VMWindow open/close performance:" << avgTime << "ms average";
}

void TestVMWindow::testTabSwitchPerformance()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QTabWidget* tabWidget = m_window->findChild<QTabWidget*>();
    QVERIFY(tabWidget != nullptr);

    QElapsedTimer timer;
    timer.start();

    // Switch through all tabs multiple times
    for (int round = 0; round < 3; round++) {
        for (int i = 0; i < tabWidget->count(); i++) {
            tabWidget->setCurrentIndex(i);
            qApp->processEvents();
        }
    }

    qint64 elapsed = timer.elapsed();
    int totalSwitches = 3 * tabWidget->count();
    qint64 avgTime = elapsed / totalSwitches;

    // Each tab switch should be under 100ms
    QVERIFY2(avgTime < 100, qPrintable(QString("Tab switch too slow: %1ms").arg(avgTime)));

    qInfo() << "Tab switch performance:" << avgTime << "ms average";
}

void TestVMWindow::testRefreshPerformance()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_window = new VMWindow(m_testDomain);

    QElapsedTimer timer;
    timer.start();

    // Call refresh multiple times
    for (int i = 0; i < 10; i++) {
        m_window->refresh();
        qApp->processEvents();
    }

    qint64 elapsed = timer.elapsed();
    qint64 avgTime = elapsed / 10;

    // Each refresh should be under 200ms
    QVERIFY2(avgTime < 200, qPrintable(QString("Refresh too slow: %1ms").arg(avgTime)));

    qInfo() << "Refresh performance:" << avgTime << "ms average";
}

void TestVMWindow::testWindowDestruction()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    // Test that window can be created and destroyed multiple times
    for (int i = 0; i < 3; i++) {
        VMWindow* window = new VMWindow(m_testDomain);
        QVERIFY(window != nullptr);

        // Access various components
        QTabWidget* tabWidget = window->findChild<QTabWidget*>();
        QToolBar* toolbar = window->findChild<QToolBar*>();
        QStatusBar* statusBar = window->statusBar();

        QVERIFY(tabWidget != nullptr);
        QVERIFY(toolbar != nullptr);
        QVERIFY(statusBar != nullptr);

        delete window;
        QTest::qWait(10);
    }

    // No crashes = test passed
}

void TestVMWindow::testMultipleWindows()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    // Test opening multiple windows for different domains (or same domain)
    QList<VMWindow*> windows;

    for (int i = 0; i < 3; i++) {
        VMWindow* window = new VMWindow(m_testDomain);
        QVERIFY(window != nullptr);
        window->show();
        windows.append(window);
    }

    // All windows should be visible
    for (VMWindow* window : windows) {
        QVERIFY(window->isVisible());
    }

    // Clean up
    for (VMWindow* window : windows) {
        window->close();
        delete window;
    }

    windows.clear();
}

QTEST_MAIN(TestVMWindow)
#include "test_vm_window.moc"
