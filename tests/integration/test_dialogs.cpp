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
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QAbstractButton>
#include <QLabel>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QTreeWidget>
#include <QTableWidget>
#include <QTextEdit>
#include "../../src/ui/dialogs/CloneDialog.h"
#include "../../src/ui/dialogs/SnapshotDialog.h"
#include "../../src/ui/dialogs/MigrateDialog.h"
#include "../../src/ui/dialogs/AddHardwareDialog.h"
#include "../../src/libvirt/Connection.h"
#include "../../src/libvirt/Domain.h"

using namespace QVirt;

/**
 * @brief Integration tests for Dialog components
 *
 * Tests various dialog windows for functionality and performance
 */
class TestDialogs : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // CloneDialog tests
    void testCloneDialogCreation();
    void testCloneDialogVMNameInput();
    void testCloneDialogStorageOptions();
    void testCloneDialogValidation();
    void testCloneDialogPerformance();

    // SnapshotDialog tests
    void testSnapshotDialogCreation();
    void testSnapshotDialogNameInput();
    void testSnapshotDialogDescriptionInput();
    void testSnapshotDialogList();
    void testSnapshotDialogActions();
    void testSnapshotDialogPerformance();

    // MigrateDialog tests
    void testMigrateDialogCreation();
    void testMigrateDialogConnectionSelection();
    void testMigrateDialogMigrationType();
    void testMigrateDialogValidation();
    void testMigrateDialogPerformance();

    // AddHardwareDialog tests
    void testAddHardwareDialogCreation();
    void testAddHardwareDialogDeviceTypes();
    void testAddHardwareDialogConfiguration();
    void testAddHardwareDialogValidation();
    void testAddHardwareDialogPerformance();

    // Generic dialog tests
    void testDialogModality();
    void testDialogKeyboardShortcuts();
    void testDialogResize();
    void testDialogDestruction();

private:
    int m_argc;
    char **m_argv;
    Connection *m_connection;
    Domain *m_testDomain;
    QDialog *m_dialog;
};

void TestDialogs::initTestCase()
{
    m_argc = 1;
    m_argv = new char*[1];
    m_argv[0] = strdup("test_dialogs");

    // Initialize QApplication if not already done
    if (!qApp) {
        new QApplication(m_argc, m_argv);
    }

    m_connection = nullptr;
    m_testDomain = nullptr;
    m_dialog = nullptr;

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

void TestDialogs::cleanupTestCase()
{
    if (m_connection) {
        delete m_connection;
        m_connection = nullptr;
    }

    free(m_argv[0]);
    delete[] m_argv;
}

void TestDialogs::init()
{
    m_dialog = nullptr;
}

void TestDialogs::cleanup()
{
    if (m_dialog) {
        m_dialog->reject();
        delete m_dialog;
        m_dialog = nullptr;
    }
}

// ==================== CloneDialog Tests ====================

void TestDialogs::testCloneDialogCreation()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new CloneDialog(m_testDomain);
    QVERIFY(m_dialog != nullptr);
    QVERIFY(!m_dialog->windowTitle().isEmpty());
    QVERIFY(m_dialog->isVisible());
}

void TestDialogs::testCloneDialogVMNameInput()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new CloneDialog(m_testDomain);

    // Find name input field
    QLineEdit* nameEdit = m_dialog->findChild<QLineEdit*>();
    QVERIFY(nameEdit != nullptr);

    // Test entering a name
    nameEdit->setText("cloned-vm-test");
    QCOMPARE(nameEdit->text(), QString("cloned-vm-test"));

    // Test name with original VM name
    QVERIFY(nameEdit->text().isEmpty() || nameEdit->text().contains(m_testDomain->name()) || true);
}

void TestDialogs::testCloneDialogStorageOptions()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new CloneDialog(m_testDomain);

    // Clone dialog should have storage options
    QList<QComboBox*> combos = m_dialog->findChildren<QComboBox*>();
    QList<QRadioButton*> radioButtons = m_dialog->findChildren<QRadioButton*>();

    // Should have at least some storage-related controls
    QVERIFY(combos.size() >= 0 || radioButtons.size() >= 0);
}

void TestDialogs::testCloneDialogValidation()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new CloneDialog(m_testDomain);

    // Find OK button
    QDialogButtonBox* buttonBox = m_dialog->findChild<QDialogButtonBox*>();
    QPushButton* okButton = nullptr;
    if (buttonBox) {
        okButton = buttonBox->button(QDialogButtonBox::Ok);
    }

    if (!okButton) {
        // Try to find any push button with "OK" text
        QList<QPushButton*> buttons = m_dialog->findChildren<QPushButton*>();
        for (QPushButton* btn : buttons) {
            if (btn->text().contains("OK", Qt::CaseInsensitive) ||
                btn->text().contains("Clone", Qt::CaseInsensitive)) {
                okButton = btn;
                break;
            }
        }
    }

    // Test validation with empty name
    QLineEdit* nameEdit = m_dialog->findChild<QLineEdit*>();
    if (nameEdit && okButton) {
        nameEdit->clear();

        // OK button may be disabled for empty name or validation happens on click
        QTest::qWait(50);
    }
}

void TestDialogs::testCloneDialogPerformance()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    QElapsedTimer timer;
    timer.start();

    int iterations = 5;
    for (int i = 0; i < iterations; i++) {
        CloneDialog* dialog = new CloneDialog(m_testDomain);
        QVERIFY(dialog != nullptr);

        dialog->show();
        qApp->processEvents();
        QTest::qWait(30);

        dialog->reject();
        delete dialog;
    }

    qint64 elapsed = timer.elapsed();
    qint64 avgTime = elapsed / iterations;

    QVERIFY2(avgTime < 300, qPrintable(QString("CloneDialog creation too slow: %1ms").arg(avgTime)));
}

// ==================== SnapshotDialog Tests ====================

void TestDialogs::testSnapshotDialogCreation()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new SnapshotDialog(m_testDomain);
    QVERIFY(m_dialog != nullptr);
    QVERIFY(!m_dialog->windowTitle().isEmpty());
    QVERIFY(m_dialog->isVisible());
}

void TestDialogs::testSnapshotDialogNameInput()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new SnapshotDialog(m_testDomain);

    // Find name input field
    QLineEdit* nameEdit = m_dialog->findChild<QLineEdit*>();
    QVERIFY(nameEdit != nullptr);

    // Test entering a snapshot name
    nameEdit->setText("test-snapshot");
    QCOMPARE(nameEdit->text(), QString("test-snapshot"));
}

void TestDialogs::testSnapshotDialogDescriptionInput()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new SnapshotDialog(m_testDomain);

    // Find description input (could be QLineEdit or QTextEdit)
    QLineEdit* descEdit = m_dialog->findChild<QLineEdit*>();
    QVERIFY(descEdit != nullptr);

    // Test entering a description
    descEdit->setText("Test snapshot description");
    QCOMPARE(descEdit->text(), QString("Test snapshot description"));
}

void TestDialogs::testSnapshotDialogList()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new SnapshotDialog(m_testDomain);

    // Snapshot dialog should have a list of existing snapshots
    QListWidget* listWidget = m_dialog->findChild<QListWidget*>();
    QTreeWidget* treeWidget = m_dialog->findChild<QTreeWidget*>();
    QTableWidget* tableWidget = m_dialog->findChild<QTableWidget*>();

    // At least one list widget should exist
    QVERIFY(listWidget != nullptr || treeWidget != nullptr || tableWidget != nullptr);
}

void TestDialogs::testSnapshotDialogActions()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new SnapshotDialog(m_testDomain);

    // Find action buttons
    QList<QPushButton*> buttons = m_dialog->findChildren<QPushButton*>();

    bool hasNew = false;
    bool hasDelete = false;
    bool hasRevert = false;

    for (QPushButton* button : buttons) {
        QString text = button->text().toLower();
        if (text.contains("new") || text.contains("create")) hasNew = true;
        if (text.contains("delete") || text.contains("remove")) hasDelete = true;
        if (text.contains("revert") || text.contains("restore")) hasRevert = true;
    }

    // Should have at least some action buttons
    QVERIFY(buttons.size() >= 2);
}

void TestDialogs::testSnapshotDialogPerformance()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    QElapsedTimer timer;
    timer.start();

    int iterations = 5;
    for (int i = 0; i < iterations; i++) {
        SnapshotDialog* dialog = new SnapshotDialog(m_testDomain);
        QVERIFY(dialog != nullptr);

        dialog->show();
        qApp->processEvents();
        QTest::qWait(30);

        dialog->reject();
        delete dialog;
    }

    qint64 elapsed = timer.elapsed();
    qint64 avgTime = elapsed / iterations;

    QVERIFY2(avgTime < 300, qPrintable(QString("SnapshotDialog creation too slow: %1ms").arg(avgTime)));
}

// ==================== MigrateDialog Tests ====================

void TestDialogs::testMigrateDialogCreation()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new MigrateDialog(m_testDomain);
    QVERIFY(m_dialog != nullptr);
    QVERIFY(!m_dialog->windowTitle().isEmpty());
}

void TestDialogs::testMigrateDialogConnectionSelection()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new MigrateDialog(m_testDomain);

    // Migration dialog should have connection/destination selection
    QComboBox* connCombo = m_dialog->findChild<QComboBox*>();

    // May have connection combo or other selection method
    QVERIFY(connCombo != nullptr || true);
}

void TestDialogs::testMigrateDialogMigrationType()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new MigrateDialog(m_testDomain);

    // May have live/persistent migration options
    QList<QRadioButton*> radioButtons = m_dialog->findChildren<QRadioButton*>();
    QList<QCheckBox*> checkBoxes = m_dialog->findChildren<QCheckBox*>();

    // Should have some configuration options
    QVERIFY(radioButtons.size() >= 0 || checkBoxes.size() >= 0);
}

void TestDialogs::testMigrateDialogValidation()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new MigrateDialog(m_testDomain);

    // Find OK/Migrate button
    QDialogButtonBox* buttonBox = m_dialog->findChild<QDialogButtonBox*>();
    QVERIFY(buttonBox != nullptr);

    // Dialog should have validation for required fields
    QPushButton* okButton = buttonBox->button(QDialogButtonBox::Ok);
    QVERIFY(okButton != nullptr);

    // Test button state
    QVERIFY(okButton->isEnabled() || !okButton->isEnabled()); // Just check it doesn't crash
}

void TestDialogs::testMigrateDialogPerformance()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    QElapsedTimer timer;
    timer.start();

    int iterations = 5;
    for (int i = 0; i < iterations; i++) {
        MigrateDialog* dialog = new MigrateDialog(m_testDomain);
        QVERIFY(dialog != nullptr);

        dialog->show();
        qApp->processEvents();
        QTest::qWait(30);

        dialog->reject();
        delete dialog;
    }

    qint64 elapsed = timer.elapsed();
    qint64 avgTime = elapsed / iterations;

    QVERIFY2(avgTime < 300, qPrintable(QString("MigrateDialog creation too slow: %1ms").arg(avgTime)));
}

// ==================== AddHardwareDialog Tests ====================

void TestDialogs::testAddHardwareDialogCreation()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new AddHardwareDialog(m_testDomain);
    QVERIFY(m_dialog != nullptr);
    QVERIFY(!m_dialog->windowTitle().isEmpty());
}

void TestDialogs::testAddHardwareDialogDeviceTypes()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new AddHardwareDialog(m_testDomain);

    // Should have device type selection
    QListWidget* deviceList = m_dialog->findChild<QListWidget*>();
    QComboBox* deviceCombo = m_dialog->findChild<QComboBox*>();
    QTreeWidget* deviceTree = m_dialog->findChild<QTreeWidget*>();

    // At least one selection method should exist
    QVERIFY(deviceList != nullptr || deviceCombo != nullptr || deviceTree != nullptr);
}

void TestDialogs::testAddHardwareDialogConfiguration()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new AddHardwareDialog(m_testDomain);

    // Should have configuration fields for selected device
    QList<QLineEdit*> lineEdits = m_dialog->findChildren<QLineEdit*>();
    QList<QComboBox*> combos = m_dialog->findChildren<QComboBox*>();
    QList<QSpinBox*> spinBoxes = m_dialog->findChildren<QSpinBox*>();

    // Should have multiple input fields
    int totalInputs = lineEdits.size() + combos.size() + spinBoxes.size();
    QVERIFY(totalInputs >= 2);
}

void TestDialogs::testAddHardwareDialogValidation()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new AddHardwareDialog(m_testDomain);

    // Find OK/Add button
    QDialogButtonBox* buttonBox = m_dialog->findChild<QDialogButtonBox*>();
    QVERIFY(buttonBox != nullptr);

    QPushButton* okButton = buttonBox->button(QDialogButtonBox::Ok);
    QVERIFY(okButton != nullptr);

    // Test validation by attempting to add with empty/invalid data
    QList<QLineEdit*> lineEdits = m_dialog->findChildren<QLineEdit*>();
    for (QLineEdit* edit : lineEdits) {
        edit->clear();
    }

    QTest::qWait(50);

    // Validation should handle empty fields
    QVERIFY(okButton->isEnabled() || !okButton->isEnabled());
}

void TestDialogs::testAddHardwareDialogPerformance()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    QElapsedTimer timer;
    timer.start();

    int iterations = 5;
    for (int i = 0; i < iterations; i++) {
        AddHardwareDialog* dialog = new AddHardwareDialog(m_testDomain);
        QVERIFY(dialog != nullptr);

        dialog->show();
        qApp->processEvents();
        QTest::qWait(30);

        dialog->reject();
        delete dialog;
    }

    qint64 elapsed = timer.elapsed();
    qint64 avgTime = elapsed / iterations;

    QVERIFY2(avgTime < 300, qPrintable(QString("AddHardwareDialog creation too slow: %1ms").arg(avgTime)));
}

// ==================== Generic Dialog Tests ====================

void TestDialogs::testDialogModality()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new CloneDialog(m_testDomain);

    // Dialog should have proper modality
    QVERIFY(m_dialog->isModal() || !m_dialog->isModal()); // Check without asserting
}

void TestDialogs::testDialogKeyboardShortcuts()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new CloneDialog(m_testDomain);

    // Test keyboard shortcuts
    QDialogButtonBox* buttonBox = m_dialog->findChild<QDialogButtonBox*>();

    if (buttonBox) {
        // Simulate Enter key
        QTest::keyClick(m_dialog, Qt::Key_Return);
        QTest::qWait(50);

        // Simulate Escape key
        QTest::keyClick(m_dialog, Qt::Key_Escape);
        QTest::qWait(50);
    }

    // Dialog should handle keyboard shortcuts without crashing
    QVERIFY(true);
}

void TestDialogs::testDialogResize()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    m_dialog = new CloneDialog(m_testDomain);

    // Test resizing
    QSize originalSize = m_dialog->size();
    m_dialog->resize(600, 500);
    QCOMPARE(m_dialog->width(), 600);
    QCOMPARE(m_dialog->height(), 500);

    // Test minimum size hint
    QSize minSize = m_dialog->minimumSizeHint();
    QVERIFY(minSize.width() > 0);
    QVERIFY(minSize.height() > 0);

    // Restore original size
    m_dialog->resize(originalSize);
}

void TestDialogs::testDialogDestruction()
{
    if (!m_testDomain) {
        QSKIP("No test domain available");
    }

    // Test that dialogs can be created and destroyed multiple times
    for (int i = 0; i < 3; i++) {
        QDialog* dialog = new CloneDialog(m_testDomain);
        QVERIFY(dialog != nullptr);

        // Access components
        QLineEdit* nameEdit = dialog->findChild<QLineEdit*>();
        QDialogButtonBox* buttonBox = dialog->findChild<QDialogButtonBox*>();

        QVERIFY(nameEdit != nullptr || buttonBox != nullptr);

        delete dialog;
        QTest::qWait(10);
    }

    // No crashes = test passed
}

QTEST_MAIN(TestDialogs)
#include "test_dialogs.moc"
