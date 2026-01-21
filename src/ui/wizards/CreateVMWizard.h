/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 The QVirt-Manager Developers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef QVIRT_UI_WIZARDS_CREATEVMWIZARD_H
#define QVIRT_UI_WIZARDS_CREATEVMWIZARD_H

#include <QWizard>
#include <QWizardPage>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>

#include "../../libvirt/Connection.h"

namespace QVirt {

/**
 * @brief VM Creation Wizard
 *
 * Multi-step wizard for creating new virtual machines
 */
class CreateVMWizard : public QWizard
{
    Q_OBJECT

public:
    enum WizardPage {
        Page_NameAndOS,
        Page_InstallMedia,
        Page_CPU_Memory,
        Page_Storage,
        Page_Network,
        Page_Summary
    };

    explicit CreateVMWizard(Connection *conn, QWidget *parent = nullptr);
    ~CreateVMWizard() override;

    Connection *connection() const { return m_connection; }

    // Getter methods for wizard data
    QString vmName() const;
    QString osType() const;
    int memoryMB() const;
    int vcpus() const;
    QString diskPath() const;
    qint64 diskSizeGB() const;
    QString networkType() const;
    QString networkSource() const;
    bool startAfterInstall() const;

private slots:
    void onCurrentIdChanged(int id);
    void validateCurrentPage();

private:
    void setupPages();
    void setupConnections();

    Connection *m_connection;
};

/**
 * @brief Page 1: Name and OS Selection
 */
class NameAndOSPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit NameAndOSPage(CreateVMWizard *wizard, Connection *conn);

    void initializePage() override;
    bool validatePage() override;
    int nextId() const override { return CreateVMWizard::Page_InstallMedia; }

private:
    void setupUI();
    void loadOSTypes();

    CreateVMWizard *m_wizard;
    Connection *m_connection;

    QLineEdit *m_nameEdit;
    QComboBox *m_osTypeCombo;
    QComboBox *m_osVersionCombo;
    QLabel *m_osIcon;
};

/**
 * @brief Page 2: Installation Media
 */
class InstallMediaPage : public QWizardPage
{
    Q_OBJECT

public:
    enum class InstallType {
        LocalISO,
        URL,
        Import,
        PXE,
        Container
    };
    Q_ENUM(InstallType)

    explicit InstallMediaPage(CreateVMWizard *wizard, Connection *conn);

    void initializePage() override;
    bool validatePage() override;
    int nextId() const override { return CreateVMWizard::Page_CPU_Memory; }

    QString getISOPath() const;
    QString getURL() const;
    QString getImportPath() const;
    InstallType getInstallType() const { return m_installType; }

private slots:
    void onInstallTypeChanged();
    void onBrowseISO();
    void onBrowseImport();

private:
    void setupUI();
    void updateUIForType();

    CreateVMWizard *m_wizard;
    Connection *m_connection;

    InstallType m_installType = InstallType::LocalISO;

    QRadioButton *m_radioISO;
    QRadioButton *m_radioURL;
    QRadioButton *m_radioImport;
    QRadioButton *m_radioPXE;
    QRadioButton *m_radioContainer;

    QLineEdit *m_isoPathEdit;
    QPushButton *m_isoBrowseBtn;
    QLineEdit *m_urlEdit;
    QLineEdit *m_importPathEdit;
    QPushButton *m_importBrowseBtn;
    QLineEdit *m_pxeEdit;
};

/**
 * @brief Page 3: CPU and Memory Configuration
 */
class CPUMemoryPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit CPUMemoryPage(CreateVMWizard *wizard, Connection *conn);

    void initializePage() override;
    bool validatePage() override;
    int nextId() const override { return CreateVMWizard::Page_Storage; }

    int memoryMB() const { return m_memorySpin->value(); }
    int vcpus() const { return m_vcpuSpin->value(); }

private:
    void setupUI();

    CreateVMWizard *m_wizard;
    Connection *m_connection;

    QSpinBox *m_memorySpin;
    QSpinBox *m_maxMemorySpin;
    QSpinBox *m_vcpuSpin;
    QSpinBox *m_maxVcpuSpin;
};

/**
 * @brief Page 4: Storage Configuration
 */
class StoragePage : public QWizardPage
{
    Q_OBJECT

public:
    enum class StorageType {
        NewDisk,
        ExistingDisk,
        NoStorage
    };
    Q_ENUM(StorageType)

    explicit StoragePage(CreateVMWizard *wizard, Connection *conn);

    void initializePage() override;
    bool validatePage() override;
    int nextId() const override { return CreateVMWizard::Page_Network; }

    QString diskPath() const;
    qint64 diskSizeGB() const;
    StorageType storageType() const { return m_storageType; }

private slots:
    void onStorageTypeChanged();
    void onBrowseDisk();
    void onCreateVolume();

private:
    void setupUI();
    void updateUIForType();

    CreateVMWizard *m_wizard;
    Connection *m_connection;

    StorageType m_storageType = StorageType::NewDisk;

    QRadioButton *m_radioNewDisk;
    QRadioButton *m_radioExisting;
    QRadioButton *m_radioNoStorage;

    QLineEdit *m_diskPathEdit;
    QPushButton *m_browseBtn;
    QSpinBox *m_diskSizeSpin;
    QComboBox *m_diskFormatCombo;
    QComboBox *m_diskBusCombo;
};

/**
 * @brief Page 5: Network Configuration
 */
class NetworkPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit NetworkPage(CreateVMWizard *wizard, Connection *conn);

    void initializePage() override;
    bool validatePage() override;
    int nextId() const override { return CreateVMWizard::Page_Summary; }

    QString networkType() const;
    QString networkSource() const;

private slots:
    void onNetworkTypeChanged(int index);

private:
    void setupUI();
    void updateUIForType();

    CreateVMWizard *m_wizard;
    Connection *m_connection;

    QComboBox *m_networkTypeCombo;
    QComboBox *m_networkSourceCombo;
    QComboBox *m_deviceModelCombo;
};

/**
 * @brief Page 6: Summary and Installation
 */
class SummaryPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit SummaryPage(CreateVMWizard *wizard, Connection *conn);

    void initializePage() override;
    bool validatePage() override;
    int nextId() const override { return -1; } // Last page

private:
    void setupUI();
    void updateSummary();

    CreateVMWizard *m_wizard;
    Connection *m_connection;

    QLabel *m_summaryLabel;
    QCheckBox *m_startCheck;
};

} // namespace QVirt

#endif // QVIRT_UI_WIZARDS_CREATEVMWIZARD_H
