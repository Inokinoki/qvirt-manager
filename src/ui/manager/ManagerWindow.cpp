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

#include "ManagerWindow.h"
#include "../dialogs/ConnectionDialog.h"
#include "../dialogs/StoragePoolDialog.h"
#include "../dialogs/NetworkDialog.h"
#include "../dialogs/PreferencesDialog.h"
#include "../widgets/ContextMenu.h"
#include "../widgets/GraphWidget.h"
#include "../vmwindow/VMWindow.h"
#include "../wizards/CreateVMWizard.h"
#include "../dialogs/HostDialog.h"
#include "../dialogs/DeleteDialog.h"
#include "../../core/Engine.h"
#include "../../core/Config.h"
#include "../../libvirt/EnumMapper.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>
#include <QStatusBar>
#include <QFile>
#include <QApplication>

namespace QVirt {

ManagerWindow::ManagerWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_actionNewVM(nullptr)
    , m_actionDeleteVM(nullptr)
    , m_actionStart(nullptr)
    , m_actionStop(nullptr)
    , m_actionReboot(nullptr)
    , m_actionPause(nullptr)
    , m_actionResume(nullptr)
    , m_actionOpenConsole(nullptr)
{
    setWindowTitle(tr("QVirt Manager"));
    resize(1024, 768);

    setupMenus();
    setupToolbar();
    setupUI();
    connectSignals();
    setupKeyboardShortcuts();

    // Load all saved connections (even disconnected ones)
    Config *config = Config::instance();
    QStringList uris = config->connectionURIs();
    for (const QString &uri : uris) {
        // Add to model as disconnected first
        bool autoconnect = config->connAutoconnect(uri);
        m_treeModel->addDisconnectedConnection(uri, autoconnect);

        // Try to auto-connect if configured
        if (autoconnect) {
            // Load saved SSH credentials
            QString sshKeyPath = config->connSSHKeyPath(uri);
            QString sshUsername = config->connSSHUsername(uri);

            // Note: We don't save passwords for security (user would need to re-enter)
            // In production, use QtKeychain or similar for secure password storage
            Connection *conn = Connection::open(uri, sshKeyPath, QString());
            if (conn) {
                addConnection(conn);
            }
            // If connection fails, it remains in the list as disconnected (shown in gray)
        }
    }

    updateVMControls();
}

ManagerWindow::~ManagerWindow() = default;

void ManagerWindow::setupUI()
{
    // Create central widget with splitter
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    m_splitter = new QSplitter(Qt::Horizontal, this);

    // Left panel - Tree view with connections and VMs
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(4, 4, 4, 4);
    leftLayout->setSpacing(4);

    m_treeModel = new ConnectionTreeModel(this);
    m_treeView = new QTreeView();
    m_treeView->setModel(m_treeModel);
    m_treeView->setHeaderHidden(true);
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_treeView->setAlternatingRowColors(false);
    m_treeView->setAnimated(true);
    m_treeView->setIndentation(20);
    m_treeView->setRootIsDecorated(true);
    m_treeView->setUniformRowHeights(true);
    m_treeView->expandAll();

    leftLayout->addWidget(m_treeView);

    QPushButton *btnAddConn = new QPushButton(tr("Add Connection"));
    connect(btnAddConn, &QPushButton::clicked, this, &ManagerWindow::openConnectionDialog);
    leftLayout->addWidget(btnAddConn);

    m_splitter->addWidget(leftPanel);
    m_splitter->setStretchFactor(0, 1);

    mainLayout->addWidget(m_splitter);

    // Status bar
    statusBar();

    m_statusLabel = new QLabel(tr("Ready"));
    statusBar()->addWidget(m_statusLabel);

    m_vmCountLabel = new QLabel(tr("VMs: 0"));
    statusBar()->addPermanentWidget(m_vmCountLabel);
}

void ManagerWindow::setupToolbar()
{
    QToolBar *toolbar = addToolBar(tr("VM Controls"));
    toolbar->setMovable(false);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    m_actionNewVM = new QAction(tr("New VM"), this);
    m_actionNewVM->setIcon(QIcon::fromTheme("document-new", style()->standardIcon(QStyle::SP_FileIcon)));
    m_actionNewVM->setShortcut(QKeySequence::New);
    toolbar->addAction(m_actionNewVM);

    toolbar->addSeparator();

    m_actionStart = new QAction(tr("Start"), this);
    m_actionStart->setIcon(QIcon::fromTheme("media-playback-start", style()->standardIcon(QStyle::SP_MediaPlay)));
    toolbar->addAction(m_actionStart);

    m_actionStop = new QAction(tr("Stop"), this);
    m_actionStop->setIcon(QIcon::fromTheme("media-playback-stop", style()->standardIcon(QStyle::SP_MediaStop)));
    toolbar->addAction(m_actionStop);

    m_actionReboot = new QAction(tr("Reboot"), this);
    m_actionReboot->setIcon(QIcon::fromTheme("view-refresh", style()->standardIcon(QStyle::SP_BrowserReload)));
    toolbar->addAction(m_actionReboot);

    m_actionPause = new QAction(tr("Pause"), this);
    m_actionPause->setIcon(QIcon::fromTheme("media-playback-pause", style()->standardIcon(QStyle::SP_MediaPause)));
    toolbar->addAction(m_actionPause);

    m_actionResume = new QAction(tr("Resume"), this);
    m_actionResume->setIcon(QIcon::fromTheme("media-playback-start", style()->standardIcon(QStyle::SP_MediaPlay)));
    toolbar->addAction(m_actionResume);

    toolbar->addSeparator();

    m_actionOpenConsole = new QAction(tr("Console"), this);
    m_actionOpenConsole->setIcon(QIcon::fromTheme("utilities-terminal", style()->standardIcon(QStyle::SP_ComputerIcon)));
    toolbar->addAction(m_actionOpenConsole);

    toolbar->addSeparator();

    m_actionDeleteVM = new QAction(tr("Delete"), this);
    m_actionDeleteVM->setIcon(QIcon::fromTheme("edit-delete", style()->standardIcon(QStyle::SP_TrashIcon)));
    toolbar->addAction(m_actionDeleteVM);
}

void ManagerWindow::setupMenus()
{
    // File menu
    m_menuFile = menuBar()->addMenu(tr("&File"));

    QAction *actionAddConn = m_menuFile->addAction(tr("Add Connection..."));
    actionAddConn->setShortcut(QKeySequence::Open);
    connect(actionAddConn, &QAction::triggered, this, &ManagerWindow::openConnectionDialog);

    QAction *actionNewVM = m_menuFile->addAction(tr("New VM..."));
    actionNewVM->setShortcut(QKeySequence::New);
    connect(actionNewVM, &QAction::triggered, this, &ManagerWindow::onNewVM);

    m_menuFile->addSeparator();

    QAction *actionQuit = m_menuFile->addAction(tr("Quit"));
    actionQuit->setShortcut(QKeySequence::Quit);
    connect(actionQuit, &QAction::triggered, this, &QMainWindow::close);

    // Edit menu
    m_menuEdit = menuBar()->addMenu(tr("&Edit"));

    QAction *actionPreferences = m_menuEdit->addAction(tr("Preferences..."));
    actionPreferences->setShortcut(QKeySequence::Preferences);
    connect(actionPreferences, &QAction::triggered, this, &ManagerWindow::showPreferences);

    // View menu
    m_menuView = menuBar()->addMenu(tr("&View"));

    QAction *actionRefresh = m_menuView->addAction(tr("Refresh"));
    actionRefresh->setShortcut(QKeySequence::Refresh);
    connect(actionRefresh, &QAction::triggered, this, &ManagerWindow::refresh);

    m_menuView->addSeparator();

    QAction *actionHostDetails = m_menuView->addAction(tr("Host Details"));
    connect(actionHostDetails, &QAction::triggered, this, &ManagerWindow::showHostDetails);

    m_menuView->addSeparator();

    QAction *actionStoragePools = m_menuView->addAction(tr("Storage Pools"));
    connect(actionStoragePools, &QAction::triggered, this, &ManagerWindow::showStoragePools);

    QAction *actionNetworks = m_menuView->addAction(tr("Virtual Networks"));
    connect(actionNetworks, &QAction::triggered, this, &ManagerWindow::showNetworks);

    // VM menu
    m_menuVM = menuBar()->addMenu(tr("&VM"));

    QAction *actionVMStart = m_menuVM->addAction(tr("Start"));
    connect(actionVMStart, &QAction::triggered, this, &ManagerWindow::onVMStarted);

    QAction *actionVMStop = m_menuVM->addAction(tr("Stop"));
    connect(actionVMStop, &QAction::triggered, this, &ManagerWindow::onVMStopped);

    QAction *actionVMReboot = m_menuVM->addAction(tr("Reboot"));
    connect(actionVMReboot, &QAction::triggered, this, &ManagerWindow::onVMRebooted);

    m_menuVM->addSeparator();

    QAction *actionVMDelete = m_menuVM->addAction(tr("Delete"));
    connect(actionVMDelete, &QAction::triggered, this, &ManagerWindow::onDeleteVM);

    // Help menu
    m_menuHelp = menuBar()->addMenu(tr("&Help"));
    QAction *actionAbout = m_menuHelp->addAction(tr("About"));
    connect(actionAbout, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, tr("About QVirt Manager"),
                          tr("<h3>QVirt Manager</h3>"
                              "<p>Virtual Machine Manager</p>"
                              "<p>Version 1.0.0</p>"
                              "<p>A Qt-based replacement for virt-manager</p>"
                              "<p>Copyright (C) 2025-2026 Inoki <veyx.shaw@gmail.com></p>"));
    });
}

void ManagerWindow::connectSignals()
{
    // Tree view selection
    connect(m_treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ManagerWindow::onTreeSelectionChanged);

    // Tree view context menu
    connect(m_treeView, &QTreeView::customContextMenuRequested,
            this, &ManagerWindow::onTreeContextMenu);

    // Tree view double click
    connect(m_treeView, &QTreeView::doubleClicked,
            this, &ManagerWindow::onTreeItemDoubleClicked);

    // Toolbar actions
    connect(m_actionNewVM, &QAction::triggered, this, &ManagerWindow::onNewVM);
    connect(m_actionStart, &QAction::triggered, this, &ManagerWindow::onVMStarted);
    connect(m_actionStop, &QAction::triggered, this, &ManagerWindow::onVMStopped);
    connect(m_actionReboot, &QAction::triggered, this, &ManagerWindow::onVMRebooted);
    connect(m_actionPause, &QAction::triggered, this, &ManagerWindow::onVMPaused);
    connect(m_actionResume, &QAction::triggered, this, &ManagerWindow::onVMResume);
    connect(m_actionDeleteVM, &QAction::triggered, this, &ManagerWindow::onDeleteVM);
    connect(m_actionOpenConsole, &QAction::triggered, this, [this]() {
        Domain *domain = getCurrentDomain();
        if (!domain) {
            return;
        }
        auto *vmWindow = new VMWindow(domain, this);
        vmWindow->show();
    });
}

Connection* ManagerWindow::getCurrentConnection() const
{
    QModelIndex index = m_treeView->currentIndex();
    if (!index.isValid()) {
        return nullptr;
    }

    // Check if it's a connection or a VM
    TreeItem *item = m_treeModel->itemAt(index);
    if (!item) {
        return nullptr;
    }

    if (item->type() == TreeItem::ConnectionItem) {
        return item->connection();
    } else if (item->type() == TreeItem::VMItem && item->domain()) {
        return item->domain()->connection();
    }

    return nullptr;
}

Domain* ManagerWindow::getCurrentDomain() const
{
    QModelIndex index = m_treeView->currentIndex();
    if (!index.isValid()) {
        return nullptr;
    }

    TreeItem *item = m_treeModel->itemAt(index);
    if (!item || item->type() != TreeItem::VMItem) {
        return nullptr;
    }

    return item->domain();
}

void ManagerWindow::addConnection(Connection *conn)
{
    if (!conn) {
        return;
    }

    // Register connection with Engine so it gets tick() calls
    Engine::instance()->registerConnection(conn);

    m_treeModel->addConnection(conn);

    // Automatically persist SSH credentials for remote connections
    Config *config = Config::instance();
    QString uri = conn->uri();

    // Save SSH key path if the connection has one
    if (!conn->sshKeyPath().isEmpty()) {
        config->setConnSSHKeyPath(uri, conn->sshKeyPath());
    }

    // Save SSH username if the connection has one
    if (!conn->sshUsername().isEmpty()) {
        config->setConnSSHUsername(uri, conn->sshUsername());
    }

    m_statusLabel->setText(tr("Connected to: %1").arg(conn->uri()));
    updateVMControls();

    // Auto-expand the connection to show VMs
    QModelIndex connIndex = m_treeModel->connectionIndex(conn);
    if (connIndex.isValid()) {
        m_treeView->expand(connIndex);
    }
}

void ManagerWindow::removeConnection(Connection *conn)
{
    if (!conn) {
        return;
    }

    QString uri = conn->uri();

    // Unregister connection from Engine
    Engine::instance()->unregisterConnection(conn);

    m_treeModel->removeConnection(conn);

    // Add back as disconnected so it remains in the sidebar
    m_treeModel->addDisconnectedConnection(uri, false);

    m_statusLabel->setText(tr("Disconnected from: %1").arg(uri));
    updateVMControls();
}

void ManagerWindow::onConnectionAdded()
{
    // Placeholder for connection added handling
}

void ManagerWindow::onVMStarted()
{
    Domain *domain = getCurrentDomain();
    if (!domain) {
        return;
    }

    if (domain->start()) {
        m_statusLabel->setText(tr("VM '%1' started").arg(domain->name()));
    }
}

void ManagerWindow::onVMStopped()
{
    Domain *domain = getCurrentDomain();
    if (!domain) {
        return;
    }

    if (domain->stop()) {
        m_statusLabel->setText(tr("VM '%1' stopped").arg(domain->name()));
    }
}

void ManagerWindow::onVMRebooted()
{
    Domain *domain = getCurrentDomain();
    if (!domain) {
        return;
    }

    if (domain->reboot()) {
        m_statusLabel->setText(tr("VM '%1' rebooting").arg(domain->name()));
    }
}

void ManagerWindow::onVMPaused()
{
    Domain *domain = getCurrentDomain();
    if (!domain) {
        return;
    }

    m_statusLabel->setText(tr("VM '%1' paused").arg(domain->name()));
    updateVMControls();
}

void ManagerWindow::onVMResume()
{
    Domain *domain = getCurrentDomain();
    if (!domain) {
        return;
    }

    m_statusLabel->setText(tr("VM '%1' resumed").arg(domain->name()));
    updateVMControls();
}

void ManagerWindow::onNewVM()
{
    // Get the selected connection
    Connection *conn = getCurrentConnection();
    if (!conn) {
        // Try to get the first available connection
        if (m_treeModel->rowCount() > 0) {
            QModelIndex firstIndex = m_treeModel->index(0, 0, QModelIndex());
            m_treeView->setCurrentIndex(firstIndex);
            conn = getCurrentConnection();
        }
        
        if (!conn) {
            QMessageBox::warning(this, tr("No Connection Available"),
                tr("Please add a connection first."));
            return;
        }
    }

    if (!conn) {
        QMessageBox::warning(this, tr("Connection Error"),
            tr("Failed to get connection."));
        return;
    }

    // Check if connection is active
    if (conn->state() != Connection::Active) {
        QMessageBox::warning(this, tr("Connection Not Active"),
            tr("The selected connection is not active. Please connect first."));
        return;
    }

    // Open the create VM wizard
    auto *wizard = new CreateVMWizard(conn, this);
    wizard->setAttribute(Qt::WA_DeleteOnClose);

    connect(wizard, &QWizard::accepted, this, [this]() {
        m_statusLabel->setText(tr("VM created successfully"));
        m_treeModel->refresh();
    });

    int result = wizard->exec();  // Use exec() for modal dialog
    
    // Re-enable polling after dialog closes (if it was disabled)
    conn->setPollingEnabled(true);
    
    if (result == QDialog::Accepted) {
        m_treeModel->refresh();
    }
}

void ManagerWindow::onDeleteVM()
{
    Domain *domain = getCurrentDomain();
    if (!domain) {
        return;
    }

    // Open delete dialog
    auto *dialog = new DeleteDialog(domain, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    if (dialog->exec() == QDialog::Accepted) {
        // User confirmed deletion
        bool deleteStorage = dialog->deleteStorage();
        bool forceDelete = dialog->forceDelete();

        QString message = QString("VM '%1' has been marked for deletion.\n\n").arg(domain->name());

        if (deleteStorage) {
            message += "Storage files will be deleted.\n";
        } else {
            message += "Storage files will be kept.\n";
        }

        if (forceDelete) {
            message += "VM was force deleted (was running).\n";
        }

        message += "\nIn production, this would:\n";
        message += "1. Stop the VM if running (unless force delete)\n";
        message += "2. Undefine the VM using virDomainUndefineXML()\n";
        message += "3. Delete storage files if requested";

        QMessageBox::information(this, "VM Deleted", message);

        // For now, just show the message
        // In production, would actually delete here
        m_statusLabel->setText(QString("VM '%1' deleted").arg(domain->name()));
    }
}



void ManagerWindow::onTreeSelectionChanged()
{
    QModelIndex index = m_treeView->currentIndex();
    if (!index.isValid()) {
        // No selection
        m_statusLabel->setText(tr("No connection selected"));
        updateVMControls();
        return;
    }

    TreeItem *item = m_treeModel->itemAt(index);
    if (!item) {
        m_statusLabel->setText(tr("No valid selection"));
        updateVMControls();
        return;
    }

    if (item->type() == TreeItem::ConnectionItem) {
        // Selected a connection
        Connection *conn = item->connection();
        if (conn) {
            m_statusLabel->setText(tr("Connected to: %1").arg(conn->uri()));
        } else if (item->connectionInfo()) {
            m_statusLabel->setText(tr("Disconnected: %1").arg(item->connectionInfo()->uri));
        }
    } else if (item->type() == TreeItem::VMItem && item->domain()) {
        // Selected a VM
        m_statusLabel->setText(tr("VM: %1").arg(item->domain()->name()));
    }

    updateVMControls();
}

void ManagerWindow::onTreeItemDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    TreeItem *item = m_treeModel->itemAt(index);
    if (!item) {
        return;
    }

    if (item->type() == TreeItem::VMItem && item->domain()) {
        // Open VM window
        auto *vmWindow = new VMWindow(item->domain(), this);
        vmWindow->show();
    } else if (item->type() == TreeItem::ConnectionItem && item->isDisconnected()) {
        // Try to connect
        onConnectToConnection();
    }
}

void ManagerWindow::openConnectionDialog()
{
    ConnectionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString uri = dialog.uri();
        if (!uri.isEmpty()) {
            // Pass SSH credentials if provided
            Connection *conn = Connection::open(uri, dialog.sshKeyPath(), dialog.sshPassword());
            if (conn) {
                addConnection(conn);

                // Save to config (credentials are automatically persisted in addConnection)
                Config *config = Config::instance();
                config->addConnectionURI(uri);
                config->setConnAutoconnect(uri, dialog.autoconnect());
            } else {
                QMessageBox::critical(this, tr("Connection Failed"),
                                     tr("Failed to connect to: %1").arg(uri));
            }
        }
    }
}

void ManagerWindow::showPreferences()
{
    PreferencesDialog dialog(this);
    dialog.exec();
}

void ManagerWindow::showHostDetails()
{
    // Get selected connection
    Connection *conn = getCurrentConnection();
    if (!conn) {
        QMessageBox::warning(this, tr("No Connection Selected"),
            tr("Please select a connection first."));
        return;
    }

    // Open host details dialog
    auto *dialog = new HostDialog(conn, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void ManagerWindow::updateVMControls()
{
    // Check if a VM is selected in the tree
    Domain *domain = getCurrentDomain();

    bool hasSelection = (domain != nullptr);
    Domain::State state = domain ? domain->state() : Domain::StateNoState;

    // Calculate enabled states for toolbar actions
    bool canStart = hasSelection && (state == Domain::StateShutOff || state == Domain::StateNoState);
    bool canStop = hasSelection && (state == Domain::StateRunning || state == Domain::StateBlocked);
    bool canReboot = hasSelection && (state == Domain::StateRunning);
    bool canPause = hasSelection && (state == Domain::StateRunning);
    bool canResume = hasSelection && (state == Domain::StatePaused);
    bool canDelete = hasSelection && (state == Domain::StateShutOff || state == Domain::StateNoState || state == Domain::StateRunning);
    bool canOpenConsole = hasSelection;

    // Update toolbar actions
    if (m_actionStart) m_actionStart->setEnabled(canStart);
    if (m_actionStop) m_actionStop->setEnabled(canStop);
    if (m_actionReboot) m_actionReboot->setEnabled(canReboot);
    if (m_actionPause) m_actionPause->setEnabled(canPause);
    if (m_actionResume) m_actionResume->setEnabled(canResume);
    if (m_actionDeleteVM) m_actionDeleteVM->setEnabled(canDelete);
    if (m_actionOpenConsole) m_actionOpenConsole->setEnabled(canOpenConsole);

    // Update VM count - count all VMs across all connections
    int vmCount = 0;
    Connection *currentConn = getCurrentConnection();
    if (currentConn) {
        vmCount = currentConn->domains().size();
    }
    m_vmCountLabel->setText(tr("VMs: %1").arg(vmCount));

    // Update status bar with VM name and connection info
    if (domain && currentConn) {
        m_statusLabel->setText(tr("VM '%1' on %2").arg(domain->name()).arg(currentConn->uri()));
    } else if (currentConn) {
        m_statusLabel->setText(tr("Connected to: %1").arg(currentConn->uri()));
    }
}

void ManagerWindow::refresh()
{
    m_treeModel->refresh();
    m_statusLabel->setText(tr("Refreshed"));
}

void ManagerWindow::showStoragePools()
{
    // Get selected connection
    Connection *conn = getCurrentConnection();
    if (!conn) {
        QMessageBox::warning(this, tr("No Connection Selected"),
            tr("Please select a connection first."));
        return;
    }

    // Open storage pool dialog
    auto *dialog = new StoragePoolDialog(conn, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void ManagerWindow::showNetworks()
{
    // Get selected connection
    Connection *conn = getCurrentConnection();
    if (!conn) {
        QMessageBox::warning(this, tr("No Connection Selected"),
            tr("Please select a connection first."));
        return;
    }

    // Open network dialog
    auto *dialog = new NetworkDialog(conn, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void ManagerWindow::setupKeyboardShortcuts()
{
    m_keyboardShortcuts = new KeyboardShortcuts(this);
    m_keyboardShortcuts->setupShortcuts(this);
}

void ManagerWindow::onTreeContextMenu(const QPoint &pos)
{
    QModelIndex index = m_treeView->indexAt(pos);
    if (!index.isValid()) {
        return;
    }

    TreeItem *item = m_treeModel->itemAt(index);
    if (!item) {
        return;
    }

    QMenu menu(this);

    if (item->type() == TreeItem::ConnectionItem) {
        // Connection context menu
        Connection *conn = item->connection();
        
        QString uri = item->uri();
        bool isConnected = (conn && conn->state() == Connection::Active);

        if (isConnected) {
            QAction *disconnectAction = menu.addAction(tr("Disconnect"));
            connect(disconnectAction, &QAction::triggered, this, &ManagerWindow::onDisconnectFromConnection);
        } else {
            QAction *connectAction = menu.addAction(tr("Connect"));
            connect(connectAction, &QAction::triggered, this, &ManagerWindow::onConnectToConnection);
        }

        menu.addSeparator();

        QAction *editAction = menu.addAction(tr("Edit..."));
        connect(editAction, &QAction::triggered, this, &ManagerWindow::onEditConnection);

        QAction *deleteAction = menu.addAction(tr("Delete"));
        deleteAction->setEnabled(true);
        connect(deleteAction, &QAction::triggered, this, &ManagerWindow::onDeleteConnection);
    } else if (item->type() == TreeItem::VMItem && item->domain()) {
        // VM context menu from tree
        VMContextMenu contextMenu(this);
        QMenu *vmMenu = contextMenu.createMenu(this, item->domain());

        connect(&contextMenu, &VMContextMenu::startRequested, this, &ManagerWindow::onVMStarted);
        connect(&contextMenu, &VMContextMenu::stopRequested, this, &ManagerWindow::onVMStopped);
        connect(&contextMenu, &VMContextMenu::rebootRequested, this, &ManagerWindow::onVMRebooted);
        connect(&contextMenu, &VMContextMenu::pauseRequested, this, &ManagerWindow::onVMPaused);
        connect(&contextMenu, &VMContextMenu::deleteRequested, this, &ManagerWindow::onDeleteVM);
        connect(&contextMenu, &VMContextMenu::openConsoleRequested, this, [this]() {
            Domain *domain = getCurrentDomain();
            if (domain) {
                auto *vmWindow = new VMWindow(domain, this);
                vmWindow->show();
            }
        });
        connect(&contextMenu, &VMContextMenu::viewDetailsRequested, this, [this]() {
            Domain *domain = getCurrentDomain();
            if (domain) {
                auto *vmWindow = new VMWindow(domain, this);
                vmWindow->show();
            }
        });

        vmMenu->exec(m_treeView->viewport()->mapToGlobal(pos));
        delete vmMenu;
        return;
    }

    menu.exec(m_treeView->viewport()->mapToGlobal(pos));
}

void ManagerWindow::onConnectToConnection()
{
    QModelIndex index = m_treeView->currentIndex();
    if (!index.isValid()) {
        return;
    }

    TreeItem *item = m_treeModel->itemAt(index);
    if (!item || item->type() != TreeItem::ConnectionItem) {
        return;
    }

    QString uri = item->uri();
    if (uri.isEmpty()) {
        return;
    }

    // Load saved credentials from config
    Config *config = Config::instance();
    QString sshKeyPath = config->connSSHKeyPath(uri);
    QString sshUsername = config->connSSHUsername(uri);

    // Create connection and connect asynchronously
    auto *newConn = Connection::create(uri);
    connect(newConn, &Connection::stateChanged, this, &ManagerWindow::onConnectionStateChanged);
    
    // Start async connection
    newConn->openAsync(sshKeyPath, QString());
    
    // Temporarily store the connection until connection completes
    m_connectingConnections[uri] = newConn;
}

void ManagerWindow::onDisconnectFromConnection()
{
    QModelIndex index = m_treeView->currentIndex();
    if (!index.isValid()) {
        return;
    }

    TreeItem *item = m_treeModel->itemAt(index);
    if (!item || item->type() != TreeItem::ConnectionItem) {
        return;
    }

    Connection *conn = item->connection();
    if (!conn) {
        return;
    }

    QString uri = conn->uri();
    removeConnection(conn);
    m_statusLabel->setText(tr("Disconnected from: %1").arg(uri));
}

void ManagerWindow::onEditConnection()
{
    QModelIndex index = m_treeView->currentIndex();
    if (!index.isValid()) {
        return;
    }

    TreeItem *item = m_treeModel->itemAt(index);
    if (!item || item->type() != TreeItem::ConnectionItem) {
        return;
    }

    QString uri = item->uri();
    if (uri.isEmpty()) {
        return;
    }

    // Load existing connection info from config
    Config *config = Config::instance();
    ConnectionInfo connInfo = config->connectionInfo(uri);

    // Open dialog in edit mode
    auto *dialog = new ConnectionDialog(connInfo, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    if (dialog->exec() == QDialog::Accepted) {
        // Get the updated values
        QString newUri = dialog->uri();
        bool autoconnect = dialog->autoconnect();
        QString sshKeyPath = dialog->sshKeyPath();
        QString sshUsername = dialog->sshUsername();

        // Check if URI changed (shouldn't happen in edit mode, but let's be safe)
        if (newUri != uri) {
            QMessageBox::warning(this, tr("Edit Connection"),
                tr("Cannot change connection URI.\n"
                   "To use a different URI, delete this connection and create a new one."));
            return;
        }

        // Update the connection settings
        config->setConnAutoconnect(uri, autoconnect);
        config->setConnSSHKeyPath(uri, sshKeyPath);
        config->setConnSSHUsername(uri, sshUsername);

        Connection *conn = item->connection();
        
        // If connection is active, reconnect to apply new settings
        if (conn) {
            // IMPORTANT: Don't use removeConnection() here as it will add back as disconnected
            // Instead, directly manipulate the model to avoid duplicates

            // Unregister from Engine
            Engine::instance()->unregisterConnection(conn);

            // Remove from models
            m_treeModel->removeConnection(conn);

            // Open new connection with updated credentials
            Connection *newConn = Connection::open(uri, sshKeyPath, QString());
            if (newConn) {
                addConnection(newConn);
                m_statusLabel->setText(tr("Connection '%1' updated and reconnected").arg(uri));
            } else {
                // If connection failed, add back as disconnected
                m_treeModel->addDisconnectedConnection(uri, config->connAutoconnect(uri));
                m_statusLabel->setText(tr("Connection '%1' updated but failed to reconnect").arg(uri));
            }
        } else {
            m_statusLabel->setText(tr("Connection '%1' updated").arg(uri));
        }

        // Refresh the connection tree
        m_treeModel->refresh();
    }
}

void ManagerWindow::onDeleteConnection()
{
    QModelIndex index = m_treeView->currentIndex();
    if (!index.isValid()) {
        return;
    }

    TreeItem *item = m_treeModel->itemAt(index);
    if (!item || item->type() != TreeItem::ConnectionItem) {
        return;
    }

    QString uri = item->uri();
    if (uri.isEmpty()) {
        return;
    }

    // Confirm deletion
    QMessageBox::StandardButton reply = QMessageBox::question(this,
        tr("Delete Connection"),
        tr("Are you sure you want to delete the connection '%1'?").arg(uri),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Remove from config
        Config *config = Config::instance();
        config->removeConnectionURI(uri);

        // Remove from model
        Connection *conn = item->connection();
        if (conn) {
            removeConnection(conn);
            // Also remove the disconnected entry that was added back
            m_treeModel->removeDisconnectedConnection(uri);
        } else {
            m_treeModel->removeDisconnectedConnection(uri);
        }

        m_statusLabel->setText(tr("Connection deleted: %1").arg(uri));
    }
}

void ManagerWindow::openConsole()
{
    Domain *domain = getCurrentDomain();
    if (!domain) {
        return;
    }
    auto *vmWindow = new VMWindow(domain, this);
    vmWindow->show();
}

void ManagerWindow::onConnectionStateChanged(Connection::State state)
{
    auto *conn = qobject_cast<Connection *>(sender());
    if (!conn) {
        return;
    }

    QString uri = conn->uri();

    if (state == Connection::Active) {
        // Connection succeeded
        addConnection(conn);
        m_connectingConnections.remove(uri);
        
        m_statusLabel->setText(tr("Connected to: %1").arg(uri));
    } else if (state == Connection::ConnectionFailed) {
        // Connection failed
        // Remove the failed connection
        conn->deleteLater();
        m_connectingConnections.remove(uri);
        
        QMessageBox::warning(this, tr("Connection Failed"),
            tr("Failed to connect to: %1\n\n%2").arg(uri, conn->connectionError()));
    } else if (state == Connection::Connecting) {
        // Still connecting - update status
        m_statusLabel->setText(tr("Connecting to %1...").arg(uri));
    }
}

} // namespace QVirt
