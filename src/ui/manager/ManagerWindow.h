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

#ifndef QVIRT_UI_MANAGER_MANAGERWINDOW_H
#define QVIRT_UI_MANAGER_MANAGERWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>

#include "../../libvirt/Connection.h"
#include "../models/ConnectionListModel.h"
#include "../models/VMListModel.h"

namespace QVirt {

/**
 * @brief Main manager window
 *
 * This is the main application window that displays:
 * - List of connections
 * - List of VMs with their states
 * - VM controls (start, stop, reboot, etc.)
 */
class ManagerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagerWindow(QWidget *parent = nullptr);
    ~ManagerWindow() override;

    void addConnection(Connection *conn);
    void removeConnection(Connection *conn);

private slots:
    void onConnectionAdded();
    void onVMStarted();
    void onVMStopped();
    void onVMRebooted();
    void onVMPaused();
    void onVMResume();
    void onNewVM();
    void onDeleteVM();
    void onVMSelectionChanged();
    void onConnectionSelectionChanged();
    void openConnectionDialog();
    void showPreferences();
    void refresh();

private:
    void setupUI();
    void setupToolbar();
    void setupMenus();
    void connectSignals();
    void updateVMControls();

    // UI components
    QSplitter *m_splitter;

    // Left panel (connections)
    QTableView *m_connectionList;
    ConnectionListModel *m_connectionModel;

    // Right panel (VM list)
    QTableView *m_vmList;
    VMListModel *m_vmModel;

    // VM control buttons
    QPushButton *m_btnStart;
    QPushButton *m_btnStop;
    QPushButton *m_btnReboot;
    QPushButton *m_btnPause;
    QPushButton *m_btnResume;
    QPushButton *m_btnNewVM;
    QPushButton *m_btnDeleteVM;
    QPushButton *m_btnOpenConsole;

    // Status bar
    QLabel *m_statusLabel;
    QLabel *m_vmCountLabel;

    // Toolbar actions
    QAction *m_actionNewVM;
    QAction *m_actionDeleteVM;
    QAction *m_actionStart;
    QAction *m_actionStop;
    QAction *m_actionReboot;
    QAction *m_actionPause;
    QAction *m_actionResume;
    QAction *m_actionOpenConsole;

    // Menus
    QMenu *m_menuFile;
    QMenu *m_menuView;
    QMenu *m_menuVM;
    QMenu *m_menuHelp;
};

} // namespace QVirt

#endif // QVIRT_UI_MANAGER_MANAGERWINDOW_H
