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

#ifndef QVIRT_UI_DIALOGS_HOSTDIALOG_H
#define QVIRT_UI_DIALOGS_HOSTDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QTableView>
#include <QLabel>
#include <QTextEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "../../libvirt/Connection.h"

namespace QVirt {

/**
 * @brief Host Details Dialog
 *
 * Shows information about the hypervisor host
 */
class HostDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HostDialog(Connection *conn, QWidget *parent = nullptr);
    ~HostDialog() override = default;

    void refresh();

private slots:
    void onRefresh();

private:
    void setupUI();
    void setupInfoPage();
    void setupPerformancePage();
    void setupDevicesPage();

    void updateInfo();
    void updatePerformance();
    void updateDevices();

    Connection *m_connection;

    // Tab widget
    QTabWidget *m_tabWidget;

    // Info page
    QLabel *m_hostnameLabel;
    QLabel *m_hypervisorLabel;
    QLabel *m_versionLabel;
    QLabel *m_archLabel;
    QLabel *m_cpusLabel;
    QLabel *m_memoryLabel;
    QLabel *m_threadsLabel;
    QLabel *m_clockSpeedLabel;
    QLabel *m_modelLabel;
    QLabel *m_capsLabel;
    QTextEdit *m_xmlCaps;

    // Performance page
    QProgressBar *m_cpuUsageBar;
    QProgressBar *m_memoryUsageBar;
    QLabel *m_runningVMsLabel;
    QLabel *m_totalVMsLabel;
    QLabel *m_activeInterfacesLabel;
    QLabel *m_totalInterfacesLabel;

    // Devices page
    QTableView *m_devicesTable;

    // Buttons
    QPushButton *m_btnRefresh;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_HOSTDIALOG_H
