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

#ifndef QVIRT_UI_VMWINDOW_VMWINDOW_H
#define QVIRT_UI_VMWINDOW_VMWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QPushButton>

#include "../../libvirt/Domain.h"

namespace QVirt {

// Forward declarations
class OverviewPage;
class DetailsPage;

/**
 * @brief VM Details Window
 *
 * This window displays detailed information about a VM including:
 * - Overview: VM info, performance stats
 * - Details: Hardware device list
 * - Console: VNC/SPICE console (future)
 * - Snapshots: Snapshot management (future)
 */
class VMWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VMWindow(Domain *domain, QWidget *parent = nullptr);
    ~VMWindow() override;

    Domain *domain() const { return m_domain; }

    void refresh();
    void updateDomainState();

private slots:
    void onDomainStateChanged(Domain::State newState);
    void onDomainStatsUpdated();
    void onStartClicked();
    void onStopClicked();
    void onRebootClicked();
    void onPauseClicked();
    void onResumeClicked();
    void onForceOffClicked();
    void onRebootClicked();
    void onSaveClicked();
    void onRestoreSavedClicked();
    void onCloneVM();
    void onDeleteVM();
    void onMigrateVM();
    void onAddHardware();
    void onRefresh();
    void onTakeScreenshot();

private:
    void setupUI();
    void setupToolbar();
    void setupMenuBar();
    void connectSignals();
    void updateControlButtons();

    // UI components
    QTabWidget *m_tabWidget;

    // Pages
    QWidget *m_overviewPage;
    QWidget *m_detailsPage;
    QWidget *m_consolePage;
    QWidget *m_snapshotsPage;

    // Toolbar actions
    QAction *m_actionStart;
    QAction *m_actionStop;
    QAction *m_actionReboot;
    QAction *m_actionPause;
    QAction *m_actionResume;
    QAction *m_actionForceOff;
    QAction *m_actionSave;
    QAction *m_actionRestoreSaved;
    QAction *m_actionShutdown;
    QAction *m_actionClone;
    QAction *m_actionDelete;
    QAction *m_actionMigrate;
    QAction *m_actionAddHardware;
    QAction *m_actionRefresh;
    QAction *m_actionScreenshot;

    // Menu bar
    QMenu *m_menuVM;
    QMenu *m_menuView;
    QMenu *m_menuDevice;
    QMenu *m_menuHelp;

    // Status bar
    QLabel *m_statusLabel;
    QLabel *m_cpuUsageLabel;
    QLabel *m_memoryUsageLabel;

    // Domain
    Domain *m_domain;
};

} // namespace QVirt

#endif // QVIRT_UI_VMWINDOW_VMWINDOW_H
