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

#ifndef QVIRT_UI_DIALOGS_CUSTOMIZEGUESTDIALOG_H
#define QVIRT_UI_DIALOGS_CUSTOMIZEGUESTDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QListWidget>
#include <QTextEdit>

namespace QVirt {

class Guest;
class DiskDevice;
class NetworkDevice;

/**
 * @brief Dialog for customizing VM configuration before creation
 *
 * Allows editing of Guest hardware settings before the VM is defined in libvirt
 */
class CustomizeGuestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomizeGuestDialog(Guest *guest, QWidget *parent = nullptr);
    ~CustomizeGuestDialog() override;

    Guest *guest() const { return m_guest; }

signals:
    void customizationAccepted(Guest *guest);
    void customizationRejected();

private slots:
    void onOkayClicked();
    void onCancelClicked();
    void onDiskAdded();
    void onDiskRemoved();
    void onNetworkAdded();
    void onNetworkRemoved();
    void onCurrentDiskChanged(int row);
    void onCurrentNetworkChanged(int row);

private:
    void setupUI();
    void setupOverviewTab();
    void setupCpuMemTab();
    void setupDisksTab();
    void setupNetworkTab();
    void setupBootTab();
    void populateDiskList();
    void populateNetworkList();
    void updateDiskDetails();
    void updateNetworkDetails();

    Guest *m_guest;

    // Tab widget
    QTabWidget *m_tabWidget;

    // Overview tab
    QLineEdit *m_nameEdit;
    QComboBox *m_osTypeCombo;
    QComboBox *m_osVersionCombo;
    QCheckBox *m_uefiCheck;

    // CPU/Memory tab
    QSpinBox *m_memorySpin;
    QSpinBox *m_maxMemorySpin;
    QSpinBox *m_vcpuSpin;
    QSpinBox *m_maxVcpuSpin;

    // Disks tab
    QListWidget *m_diskList;
    QComboBox *m_diskTypeCombo;
    QLineEdit *m_diskPathEdit;
    QPushButton *m_diskBrowseBtn;
    QSpinBox *m_diskSizeSpin;
    QComboBox *m_diskFormatCombo;
    QComboBox *m_diskBusCombo;
    QComboBox *m_diskDeviceCombo;
    QPushButton *m_addDiskBtn;
    QPushButton *m_removeDiskBtn;

    // Network tab
    QListWidget *m_networkList;
    QComboBox *m_networkTypeCombo;
    QComboBox *m_networkSourceCombo;
    QComboBox *m_networkModelCombo;
    QPushButton *m_addNetworkBtn;
    QPushButton *m_removeNetworkBtn;

    // Boot tab
    QListWidget *m_bootOrderList;
    QCheckBox *m_acpiCheck;
    QCheckBox *m_apicCheck;

    // Buttons
    QPushButton *m_okayBtn;
    QPushButton *m_cancelBtn;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_CUSTOMIZEGUESTDIALOG_H
