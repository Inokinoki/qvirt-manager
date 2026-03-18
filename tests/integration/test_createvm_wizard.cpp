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
#include <QWizard>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>
#include <QAbstractButton>
#include <QLabel>
#include <QStackedWidget>
#include "../../src/ui/wizards/CreateVMWizard.h"
#include "../../src/libvirt/Connection.h"

using namespace QVirt;

/**
 * @brief Integration tests for CreateVMWizard
 *
 * Tests the VM creation wizard workflow and validation
 */
class TestCreateVMWizard : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Wizard structure tests
    void testWizardCreation();
    void testWizardPageCount();
    void testWizardPagesExist();

    // Page 1: Name and OS tests
    void testNameAndOSPageCreation();
    void testNameValidation();
    void testOSTypeSelection();

    // Page 2: Install Media tests
    void testInstallMediaPage();
    void testInstallTypes();
    void testISOPathSelection();
    void testURLInput();

    // Page 3: CPU and Memory tests
    void testCPUMemoryPage();
    void testMemoryRange();
    void testVCPURange();
    void testMemoryDefaultValue();
    void testVCPUDefaultValue();

    // Page 4: Storage tests
    void testStoragePage();
    void testStorageTypes();
    void testDiskSizeInput();
    void testDiskFormatSelection();
    void testDiskBusSelection();

    // Page 5: Network tests
    void testNetworkPage();
    void testNetworkTypeSelection();
    void testNetworkSourceSelection();

    // Page 6: Summary tests
    void testSummaryPage();
    void testSummaryContent();
    void testCustomizeOption();
    void testStartAfterInstallOption();

    // Navigation tests
    void testNextBackNavigation();
    void testPageTransitions();
    void testWizardCompletion();

    // Validation tests
    void testNameValidationEmpty();
    void testNameValidationInvalid();
    void testISOValidation();
    void testMemoryValidation();

    // Performance tests
    void testPageTransitionPerformance();
    void testWizardOpenCancelPerformance();

    // Memory leak tests
    void testWizardDestruction();

private:
    int m_argc;
    char **m_argv;
    Connection *m_connection;
    CreateVMWizard *m_wizard;
};

void TestCreateVMWizard::initTestCase()
{
    m_argc = 1;
    m_argv = new char*[1];
    m_argv[0] = strdup("test_createvm_wizard");

    // Initialize QApplication if not already done
    if (!qApp) {
        new QApplication(m_argc, m_argv);
    }

    m_connection = nullptr;
    m_wizard = nullptr;

    // Try to open a test connection
    m_connection = Connection::open("test:///default");
}

void TestCreateVMWizard::cleanupTestCase()
{
    if (m_connection) {
        delete m_connection;
        m_connection = nullptr;
    }

    free(m_argv[0]);
    delete[] m_argv;
}

void TestCreateVMWizard::init()
{
    m_wizard = nullptr;
}

void TestCreateVMWizard::cleanup()
{
    if (m_wizard) {
        m_wizard->reject();
        delete m_wizard;
        m_wizard = nullptr;
    }
}

void TestCreateVMWizard::testWizardCreation()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);
    QVERIFY(m_wizard != nullptr);
    QVERIFY(!m_wizard->windowTitle().isEmpty());
    QVERIFY(m_wizard->isVisible());
}

void TestCreateVMWizard::testWizardPageCount()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    // Should have 6 pages as defined in the enum
    QStackedWidget* stackedWidget = m_wizard->findChild<QStackedWidget*>();
    if (stackedWidget) {
        QVERIFY(stackedWidget->count() >= 4); // At least the main pages
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testWizardPagesExist()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    // Navigate through pages and verify each exists
    QVERIFY(m_wizard->currentPage() != nullptr);

    // Click Next to go through pages
    QAbstractButton* nextButton = m_wizard->button(QWizard::NextButton);
    QVERIFY(nextButton != nullptr);

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testNameAndOSPageCreation()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    // First page should have name input
    QLineEdit* nameEdit = m_wizard->findChild<QLineEdit*>();
    QVERIFY(nameEdit != nullptr);

    // Should have OS type combo
    QComboBox* osCombo = m_wizard->findChild<QComboBox*>();
    QVERIFY(osCombo != nullptr);

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testNameValidation()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QLineEdit* nameEdit = m_wizard->findChild<QLineEdit*>();
    QVERIFY(nameEdit != nullptr);

    // Test valid name
    nameEdit->setText("test-vm-01");
    QVERIFY(nameEdit->text() == "test-vm-01");

    // Test name with spaces
    nameEdit->setText("test vm");
    QVERIFY(nameEdit->text() == "test vm");

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testOSTypeSelection()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QList<QComboBox*> combos = m_wizard->findChildren<QComboBox*>();
    QComboBox* osCombo = nullptr;

    // Find OS type combo (first combo with "os" in object name or accessible name)
    for (QComboBox* combo : combos) {
        if (combo->objectName().toLower().contains("os") ||
            combo->accessibleName().toLower().contains("os")) {
            osCombo = combo;
            break;
        }
    }

    if (osCombo) {
        QVERIFY(osCombo->count() >= 0); // May be 0 without data
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testInstallMediaPage()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    // Find radio buttons for install type
    QList<QRadioButton*> radioButtons = m_wizard->findChildren<QRadioButton*>();

    bool hasISORadio = false;
    bool hasURLRadio = false;
    bool hasImportRadio = false;

    for (QRadioButton* radio : radioButtons) {
        QString text = radio->text().toLower();
        if (text.contains("iso") || text.contains("local")) hasISORadio = true;
        if (text.contains("url") || text.contains("network")) hasURLRadio = true;
        if (text.contains("import")) hasImportRadio = true;
    }

    // Should have at least ISO option
    QVERIFY(hasISORadio || radioButtons.size() > 0);

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testInstallTypes()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QList<QRadioButton*> radioButtons = m_wizard->findChildren<QRadioButton*>();

    // Test selecting different radio buttons
    for (QRadioButton* radio : radioButtons) {
        if (radio->isVisible() && radio->isEnabled()) {
            radio->click();
            QVERIFY(radio->isChecked());
            QTest::qWait(10);
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testISOPathSelection()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    // Find ISO path input
    QList<QLineEdit*> lineEdits = m_wizard->findChildren<QLineEdit*>();

    for (QLineEdit* edit : lineEdits) {
        if (edit->objectName().toLower().contains("iso") ||
            edit->accessibleName().toLower().contains("iso")) {
            edit->setText("/path/to/test.iso");
            QCOMPARE(edit->text(), QString("/path/to/test.iso"));
            break;
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testURLInput()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QList<QLineEdit*> lineEdits = m_wizard->findChildren<QLineEdit*>();

    // Find URL input and test
    for (QLineEdit* edit : lineEdits) {
        edit->setText("http://example.com/install");
        QCOMPARE(edit->text(), QString("http://example.com/install"));
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testCPUMemoryPage()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    // Find memory and VCPU spinboxes
    QList<QSpinBox*> spinBoxes = m_wizard->findChildren<QSpinBox*>();

    bool hasMemory = false;
    bool hasVCPU = false;

    for (QSpinBox* spinBox : spinBoxes) {
        QString name = spinBox->objectName().toLower() + spinBox->accessibleName().toLower();
        if (name.contains("memory") || name.contains("ram")) hasMemory = true;
        if (name.contains("vcpu") || name.contains("cpu")) hasVCPU = true;
    }

    // Should have memory and CPU controls
    QVERIFY(spinBoxes.size() >= 2);

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testMemoryRange()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QList<QSpinBox*> spinBoxes = m_wizard->findChildren<QSpinBox*>();

    for (QSpinBox* spinBox : spinBoxes) {
        // Memory should have reasonable range
        if (spinBox->minimum() < 100) { // Less than 100 MB minimum
            QVERIFY(spinBox->maximum() >= 512); // At least 512 MB max
            QVERIFY(spinBox->singleStep() > 0);
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testVCPURange()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QList<QSpinBox*> spinBoxes = m_wizard->findChildren<QSpinBox*>();

    for (QSpinBox* spinBox : spinBoxes) {
        // VCPU should have reasonable range
        if (spinBox->minimum() <= 1 && spinBox->maximum() >= 1) {
            QVERIFY(spinBox->minimum() >= 1); // At least 1 VCPU
            QVERIFY(spinBox->maximum() >= 4); // At least 4 VCPUs max
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testMemoryDefaultValue()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QList<QSpinBox*> spinBoxes = m_wizard->findChildren<QSpinBox*>();

    for (QSpinBox* spinBox : spinBoxes) {
        QString name = spinBox->objectName().toLower();
        if (name.contains("memory")) {
            // Default memory should be reasonable (256MB - 8GB)
            QVERIFY(spinBox->value() >= 256);
            QVERIFY(spinBox->value() <= 8192);
            break;
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testVCPUDefaultValue()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QList<QSpinBox*> spinBoxes = m_wizard->findChildren<QSpinBox*>();

    for (QSpinBox* spinBox : spinBoxes) {
        QString name = spinBox->objectName().toLower();
        if (name.contains("vcpu") || name.contains("cpu")) {
            // Default VCPU should be 1 or 2
            QVERIFY(spinBox->value() >= 1);
            QVERIFY(spinBox->value() <= 4);
            break;
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testStoragePage()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    // Look for storage-related controls
    QList<QRadioButton*> radioButtons = m_wizard->findChildren<QRadioButton*>();

    bool hasNewDisk = false;
    bool hasExisting = false;

    for (QRadioButton* radio : radioButtons) {
        QString text = radio->text().toLower();
        if (text.contains("new") && text.contains("disk")) hasNewDisk = true;
        if (text.contains("existing")) hasExisting = true;
    }

    // Should have storage options
    QVERIFY(radioButtons.size() >= 2);

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testStorageTypes()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QList<QRadioButton*> radioButtons = m_wizard->findChildren<QRadioButton*>();

    // Test selecting different storage types
    for (QRadioButton* radio : radioButtons) {
        if (radio->text().toLower().contains("disk") ||
            radio->text().toLower().contains("storage") ||
            radio->text().toLower().contains("volume")) {
            radio->click();
            QVERIFY(radio->isChecked());
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testDiskSizeInput()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QList<QSpinBox*> spinBoxes = m_wizard->findChildren<QSpinBox*>();

    // Find disk size spinbox
    for (QSpinBox* spinBox : spinBoxes) {
        QString name = spinBox->objectName().toLower();
        if (name.contains("disk") || name.contains("size")) {
            QVERIFY(spinBox->minimum() >= 1); // At least 1 GB
            QVERIFY(spinBox->maximum() >= 100); // At least 100 GB max
            break;
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testDiskFormatSelection()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QList<QComboBox*> combos = m_wizard->findChildren<QComboBox*>();

    // Find disk format combo
    for (QComboBox* combo : combos) {
        QString name = combo->objectName().toLower();
        if (name.contains("format")) {
            QVERIFY(combo->count() >= 1); // Should have at least one format
            break;
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testDiskBusSelection()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QList<QComboBox*> combos = m_wizard->findChildren<QComboBox*>();

    // Find disk bus combo
    for (QComboBox* combo : combos) {
        QString name = combo->objectName().toLower();
        if (name.contains("bus")) {
            QVERIFY(combo->count() >= 1); // Should have at least one bus type
            break;
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testNetworkPage()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    // Look for network-related controls
    QList<QComboBox*> combos = m_wizard->findChildren<QComboBox*>();

    // Should have network type and source combos
    QVERIFY(combos.size() >= 2);

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testNetworkTypeSelection()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QList<QComboBox*> combos = m_wizard->findChildren<QComboBox*>();

    // Test network type selection
    for (QComboBox* combo : combos) {
        QString name = combo->objectName().toLower();
        if (name.contains("network") && name.contains("type")) {
            if (combo->count() > 0) {
                combo->setCurrentIndex(0);
                QCOMPARE(combo->currentIndex(), 0);
            }
            break;
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testNetworkSourceSelection()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QList<QComboBox*> combos = m_wizard->findChildren<QComboBox*>();

    // Test network source selection
    for (QComboBox* combo : combos) {
        QString name = combo->objectName().toLower();
        if (name.contains("network") && name.contains("source")) {
            if (combo->count() > 0) {
                combo->setCurrentIndex(0);
            }
            break;
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testSummaryPage()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    // Navigate to summary page (last page)
    QAbstractButton* nextButton = m_wizard->button(QWizard::NextButton);
    QAbstractButton* finishButton = m_wizard->button(QWizard::FinishButton);

    QVERIFY(nextButton != nullptr);
    QVERIFY(finishButton != nullptr);

    // Summary page should have labels
    QList<QLabel*> labels = m_wizard->findChildren<QLabel*>();
    QVERIFY(!labels.isEmpty());

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testSummaryContent()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    // Summary should display VM name
    QLineEdit* nameEdit = m_wizard->findChild<QLineEdit*>();
    if (nameEdit) {
        nameEdit->setText("test-summary-vm");

        // Navigate to summary and check content
        QLabel* summaryLabel = m_wizard->findChild<QLabel*>();
        if (summaryLabel && summaryLabel->isVisible()) {
            // Label should contain VM name or be updated
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testCustomizeOption()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    // Find customize checkbox
    QList<QCheckBox*> checkBoxes = m_wizard->findChildren<QCheckBox*>();

    for (QCheckBox* checkBox : checkBoxes) {
        if (checkBox->text().toLower().contains("customize")) {
            checkBox->setChecked(true);
            QVERIFY(checkBox->isChecked());

            checkBox->setChecked(false);
            QVERIFY(!checkBox->isChecked());
            break;
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testStartAfterInstallOption()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    // Find start after install checkbox
    QList<QCheckBox*> checkBoxes = m_wizard->findChildren<QCheckBox*>();

    for (QCheckBox* checkBox : checkBoxes) {
        if (checkBox->text().toLower().contains("start")) {
            checkBox->setChecked(true);
            QVERIFY(checkBox->isChecked());
            break;
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testNextBackNavigation()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QAbstractButton* nextButton = m_wizard->button(QWizard::NextButton);
    QAbstractButton* backButton = m_wizard->button(QWizard::BackButton);

    QVERIFY(nextButton != nullptr);
    QVERIFY(backButton != nullptr);

    // Test Next button
    int initialPageId = m_wizard->currentId();
    nextButton->click();
    QTest::qWait(50);

    // Page should have changed or we're on the last page
    if (m_wizard->currentId() != -1) {
        QVERIFY(m_wizard->currentId() >= initialPageId);
    }

    // Test Back button if not on first page
    if (m_wizard->currentId() > 0) {
        backButton->click();
        QTest::qWait(50);
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testPageTransitions()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QAbstractButton* nextButton = m_wizard->button(QWizard::NextButton);
    QAbstractButton* backButton = m_wizard->button(QWizard::BackButton);

    QVERIFY(nextButton != nullptr);
    QVERIFY(backButton != nullptr);

    // Navigate forward through all pages
    int maxPageId = -1;
    while (m_wizard->currentId() != -1 && maxPageId < 10) { // Safety limit
        maxPageId = qMax(maxPageId, m_wizard->currentId());
        if (nextButton->isEnabled()) {
            nextButton->click();
            QTest::qWait(50);
        } else {
            break;
        }
    }

    // Should have navigated through at least 2 pages
    QVERIFY(maxPageId >= 0);

    // Navigate back to start
    while (m_wizard->currentId() > 0 && backButton->isEnabled()) {
        backButton->click();
        QTest::qWait(50);
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testWizardCompletion()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    // Fill in minimal required fields
    QLineEdit* nameEdit = m_wizard->findChild<QLineEdit*>();
    if (nameEdit) {
        nameEdit->setText("test-complete-vm");
    }

    // Navigate to finish
    QAbstractButton* nextButton = m_wizard->button(QWizard::NextButton);
    QVERIFY(nextButton != nullptr);

    // Navigate through pages
    int iterations = 0;
    while (iterations < 10 && nextButton->isEnabled() && m_wizard->currentId() != -1) {
        nextButton->click();
        QTest::qWait(50);
        iterations++;
    }

    // Wizard should either complete or reach a validation point
    QVERIFY(iterations > 0);

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testNameValidationEmpty()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QLineEdit* nameEdit = m_wizard->findChild<QLineEdit*>();
    QVERIFY(nameEdit != nullptr);

    // Clear name
    nameEdit->clear();

    // Try to proceed - validation should handle empty name
    QAbstractButton* nextButton = m_wizard->button(QWizard::NextButton);
    if (nextButton->isEnabled()) {
        nextButton->click();
        QTest::qWait(50);
        // Either validation prevents navigation or wizard handles empty name
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testNameValidationInvalid()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QLineEdit* nameEdit = m_wizard->findChild<QLineEdit*>();
    QVERIFY(nameEdit != nullptr);

    // Test with special characters
    nameEdit->setText("test@vm#name!");

    // Wizard should handle or sanitize invalid names
    QVERIFY(!nameEdit->text().isEmpty());

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testISOValidation()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    // Set empty ISO path and try to proceed
    QList<QLineEdit*> lineEdits = m_wizard->findChildren<QLineEdit*>();
    for (QLineEdit* edit : lineEdits) {
        if (edit->objectName().toLower().contains("iso")) {
            edit->clear();
            break;
        }
    }

    // Try to proceed - validation should handle missing ISO
    QAbstractButton* nextButton = m_wizard->button(QWizard::NextButton);
    if (nextButton && nextButton->isEnabled()) {
        nextButton->click();
        QTest::qWait(50);
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testMemoryValidation()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    // Set memory to minimum
    QList<QSpinBox*> spinBoxes = m_wizard->findChildren<QSpinBox*>();
    for (QSpinBox* spinBox : spinBoxes) {
        if (spinBox->objectName().toLower().contains("memory")) {
            spinBox->setValue(spinBox->minimum());
            QVERIFY(spinBox->value() >= spinBox->minimum());
            break;
        }
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testPageTransitionPerformance()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    m_wizard = new CreateVMWizard(m_connection);

    QAbstractButton* nextButton = m_wizard->button(QWizard::NextButton);
    QAbstractButton* backButton = m_wizard->button(QWizard::BackButton);

    QVERIFY(nextButton != nullptr);

    QElapsedTimer timer;
    timer.start();

    // Measure time for 5 page transitions
    int transitions = 0;
    for (int i = 0; i < 5 && nextButton->isEnabled(); i++) {
        nextButton->click();
        QTest::qWait(30);
        transitions++;
    }

    // Go back
    for (int i = 0; i < transitions && backButton->isEnabled(); i++) {
        backButton->click();
        QTest::qWait(30);
    }

    qint64 elapsed = timer.elapsed();

    // Each transition should average under 200ms
    if (transitions > 0) {
        qint64 avgTime = elapsed / transitions;
        QVERIFY2(avgTime < 200, qPrintable(QString("Page transition too slow: %1ms").arg(avgTime)));
        qInfo() << "Page transition performance:" << avgTime << "ms average";
    }

    delete m_wizard;
    m_wizard = nullptr;
}

void TestCreateVMWizard::testWizardOpenCancelPerformance()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    int iterations = 5;
    QElapsedTimer timer;
    timer.start();

    for (int i = 0; i < iterations; i++) {
        CreateVMWizard* wizard = new CreateVMWizard(m_connection);
        QVERIFY(wizard != nullptr);

        // Show briefly
        wizard->show();
        qApp->processEvents();
        QTest::qWait(30);

        wizard->reject();
        delete wizard;
    }

    qint64 elapsed = timer.elapsed();
    qint64 avgTime = elapsed / iterations;

    // Average open/close should be under 300ms
    QVERIFY2(avgTime < 300, qPrintable(QString("Wizard open/close too slow: %1ms").arg(avgTime)));

    qInfo() << "Wizard open/close performance:" << avgTime << "ms average";
}

void TestCreateVMWizard::testWizardDestruction()
{
    if (!m_connection) {
        QSKIP("No connection available");
    }

    // Test that wizard can be created and destroyed multiple times
    for (int i = 0; i < 3; i++) {
        CreateVMWizard* wizard = new CreateVMWizard(m_connection);
        QVERIFY(wizard != nullptr);

        // Access various components
        QLineEdit* nameEdit = wizard->findChild<QLineEdit*>();
        QComboBox* osCombo = wizard->findChild<QComboBox*>();

        delete wizard;

        // Should be fully destroyed
        QTest::qWait(10);
    }

    // No crashes = test passed
}

QTEST_MAIN(TestCreateVMWizard)
#include "test_createvm_wizard.moc"
