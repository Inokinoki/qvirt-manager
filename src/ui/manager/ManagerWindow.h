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

#ifndef QVIRT_UI_MANAGER_MANAGERWINDOW_H
#define QVIRT_UI_MANAGER_MANAGERWINDOW_H

#include <QMainWindow>
#include <QTreeView>
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
#include "../models/ConnectionTreeModel.h"
#include "../widgets/ContextMenu.h"

namespace QVirt {

class KeyboardShortcuts;

/**
 * @brief Main manager window
 *
 * This is the main application window that displays:
 * - Expandable tree of connections with VMs as children
 * - VM list in the main area
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
    void onTreeSelectionChanged();
    void openConnectionDialog();
    void showPreferences();
    void showHostDetails();
    void showStoragePools();
    void showNetworks();
    void refresh();
    void onTreeContextMenu(const QPoint &pos);
    void onConnectToConnection();
    void onDisconnectFromConnection();
    void onEditConnection();
    void onDeleteConnection();
    void openConsole();
    void onConnectionStateChanged(Connection::State state);
    void onTreeItemDoubleClicked(const QModelIndex &index);

private:
    void setupUI();
    void setupToolbar();
    void setupMenus();
    void connectSignals();
    void setupKeyboardShortcuts();
    void updateVMControls();
    Connection* getCurrentConnection() const;
    Domain* getCurrentDomain() const;

    // UI components
    QSplitter *m_splitter;

    // Left panel (tree view with connections and VMs)
    QTreeView *m_treeView;
    ConnectionTreeModel *m_treeModel;

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
    QMenu *m_menuEdit;
    QMenu *m_menuView;
    QMenu *m_menuVM;
    QMenu *m_menuHelp;

    // Context menu and shortcuts
    KeyboardShortcuts *m_keyboardShortcuts;

    // Pending connections (URI -> Connection)
    QMap<QString, Connection *> m_connectingConnections;
};

} // namespace QVirt

#endif // QVIRT_UI_MANAGER_MANAGERWINDOW_H
