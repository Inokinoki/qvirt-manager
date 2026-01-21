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

#include "ManagerWindow.h"
#include "../dialogs/ConnectionDialog.h"
#include "../../core/Engine.h"
#include "../../core/Config.h"
#include "../../libvirt/EnumMapper.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QStatusBar>

namespace QVirt {

ManagerWindow::ManagerWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("QVirt Manager"));
    resize(1024, 768);

    setupMenus();
    setupToolbar();
    setupUI();
    connectSignals();

    // Auto-connect to default URI if configured
    Config *config = Config::instance();
    QStringList uris = config->connectionURIs();
    for (const QString &uri : uris) {
        if (config->connAutoconnect(uri)) {
            Connection *conn = Connection::open(uri);
            if (conn) {
                addConnection(conn);
            }
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

    m_splitter = new QSplitter(Qt::Horizontal, this);

    // Left panel - Connection list
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *connLabel = new QLabel(tr("Connections"));
    leftLayout->addWidget(connLabel);

    m_connectionModel = new ConnectionListModel(this);
    m_connectionList = new QTableView();
    m_connectionList->setModel(m_connectionModel);
    m_connectionList->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_connectionList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_connectionList->horizontalHeader()->setStretchLastSection(true);
    m_connectionList->verticalHeader()->setVisible(false);

    leftLayout->addWidget(m_connectionList);

    QPushButton *btnAddConn = new QPushButton(tr("Add Connection"));
    connect(btnAddConn, &QPushButton::clicked, this, &ManagerWindow::openConnectionDialog);
    leftLayout->addWidget(btnAddConn);

    m_splitter->addWidget(leftPanel);

    // Right panel - VM list with controls
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

    // VM list
    QLabel *vmLabel = new QLabel(tr("Virtual Machines"));
    rightLayout->addWidget(vmLabel);

    m_vmModel = new VMListModel(this);
    m_vmList = new QTableView();
    m_vmList->setModel(m_vmModel);
    m_vmList->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_vmList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_vmList->setAlternatingRowColors(true);

    // Set column widths
    m_vmList->setColumnWidth(0, 250);  // Name
    m_vmList->setColumnWidth(1, 100);  // State
    m_vmList->setColumnWidth(2, 80);   // CPU
    m_vmList->setColumnWidth(3, 100);  // Memory

    rightLayout->addWidget(m_vmList);

    // VM control buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    m_btnNewVM = new QPushButton(tr("New VM"));
    m_btnNewVM->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    m_btnStart = new QPushButton(tr("Start"));
    m_btnStart->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    m_btnStop = new QPushButton(tr("Stop"));
    m_btnStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    m_btnReboot = new QPushButton(tr("Reboot"));
    m_btnPause = new QPushButton(tr("Pause"));
    m_btnPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    m_btnResume = new QPushButton(tr("Resume"));
    m_btnResume->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    m_btnDeleteVM = new QPushButton(tr("Delete"));
    m_btnDeleteVM->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    m_btnOpenConsole = new QPushButton(tr("Console"));
    m_btnOpenConsole->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));

    buttonLayout->addWidget(m_btnNewVM);
    buttonLayout->addWidget(m_btnStart);
    buttonLayout->addWidget(m_btnStop);
    buttonLayout->addWidget(m_btnReboot);
    buttonLayout->addWidget(m_btnPause);
    buttonLayout->addWidget(m_btnResume);
    buttonLayout->addWidget(m_btnOpenConsole);
    buttonLayout->addWidget(m_btnDeleteVM);
    buttonLayout->addStretch();

    rightLayout->addLayout(buttonLayout);

    m_splitter->addWidget(rightPanel);
    m_splitter->setStretchFactor(0, 1);
    m_splitter->setStretchFactor(1, 3);

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
    QToolBar *toolbar = addToolBar(tr("Main Toolbar"));
    toolbar->setMovable(false);

    m_actionNewVM = new QAction(tr("New VM"), this);
    m_actionNewVM->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    m_actionNewVM->setShortcut(QKeySequence::New);
    toolbar->addAction(m_actionNewVM);

    toolbar->addSeparator();

    m_actionStart = new QAction(tr("Start"), this);
    m_actionStart->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    toolbar->addAction(m_actionStart);

    m_actionStop = new QAction(tr("Stop"), this);
    m_actionStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    toolbar->addAction(m_actionStop);

    m_actionReboot = new QAction(tr("Reboot"), this);
    toolbar->addAction(m_actionReboot);

    m_actionPause = new QAction(tr("Pause"), this);
    m_actionPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    toolbar->addAction(m_actionPause);

    m_actionResume = new QAction(tr("Resume"), this);
    m_actionResume->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    toolbar->addAction(m_actionResume);

    toolbar->addSeparator();

    m_actionOpenConsole = new QAction(tr("Open Console"), this);
    m_actionOpenConsole->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    toolbar->addAction(m_actionOpenConsole);

    toolbar->addSeparator();

    m_actionDeleteVM = new QAction(tr("Delete"), this);
    m_actionDeleteVM->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
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

    // View menu
    m_menuView = menuBar()->addMenu(tr("&View"));

    QAction *actionRefresh = m_menuView->addAction(tr("Refresh"));
    actionRefresh->setShortcut(QKeySequence::Refresh);
    connect(actionRefresh, &QAction::triggered, this, &ManagerWindow::refresh);

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
                              "<p>Copyright (C) 2025-2026 The QVirt-Manager Developers</p>"));
    });
}

void ManagerWindow::connectSignals()
{
    // VM list selection
    connect(m_vmList->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ManagerWindow::onVMSelectionChanged);

    // Connection list selection
    connect(m_connectionList->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ManagerWindow::onConnectionSelectionChanged);

    // Button connections
    connect(m_btnNewVM, &QPushButton::clicked, this, &ManagerWindow::onNewVM);
    connect(m_btnStart, &QPushButton::clicked, this, &ManagerWindow::onVMStarted);
    connect(m_btnStop, &QPushButton::clicked, this, &ManagerWindow::onVMStopped);
    connect(m_btnReboot, &QPushButton::clicked, this, &ManagerWindow::onVMRebooted);
    connect(m_btnPause, &QPushButton::clicked, this, &ManagerWindow::onVMPaused);
    connect(m_btnResume, &QPushButton::clicked, this, &ManagerWindow::onVMResume);
    connect(m_btnDeleteVM, &QPushButton::clicked, this, &ManagerWindow::onDeleteVM);
    connect(m_btnOpenConsole, &QPushButton::clicked, [this]() {
        m_statusLabel->setText(tr("Console viewer not yet implemented"));
    });

    // Toolbar actions
    connect(m_actionNewVM, &QAction::triggered, this, &ManagerWindow::onNewVM);
    connect(m_actionStart, &QAction::triggered, this, &ManagerWindow::onVMStarted);
    connect(m_actionStop, &QAction::triggered, this, &ManagerWindow::onVMStopped);
    connect(m_actionReboot, &QAction::triggered, this, &ManagerWindow::onVMRebooted);
    connect(m_actionPause, &QAction::triggered, this, &ManagerWindow::onVMPaused);
    connect(m_actionResume, &QAction::triggered, this, &ManagerWindow::onVMResume);
    connect(m_actionDeleteVM, &QAction::triggered, this, &ManagerWindow::onDeleteVM);
    connect(m_actionOpenConsole, &QAction::triggered, [this]() {
        m_statusLabel->setText(tr("Console viewer not yet implemented"));
    });
}

void ManagerWindow::addConnection(Connection *conn)
{
    if (!conn) {
        return;
    }

    m_connectionModel->addConnection(conn);
    m_vmModel->addConnection(conn);

    m_statusLabel->setText(tr("Connected to: %1").arg(conn->uri()));
    updateVMControls();
}

void ManagerWindow::removeConnection(Connection *conn)
{
    if (!conn) {
        return;
    }

    m_connectionModel->removeConnection(conn);
    m_vmModel->removeConnection(conn);

    m_statusLabel->setText(tr("Disconnected from: %1").arg(conn->uri()));
    updateVMControls();
}

void ManagerWindow::onConnectionAdded()
{
    // Placeholder for connection added handling
}

void ManagerWindow::onVMStarted()
{
    Domain *domain = m_vmModel->domainAt(m_vmList->currentIndex().row());
    if (!domain) {
        return;
    }

    if (domain->start()) {
        m_statusLabel->setText(tr("VM '%1' started").arg(domain->name()));
    }
}

void ManagerWindow::onVMStopped()
{
    Domain *domain = m_vmModel->domainAt(m_vmList->currentIndex().row());
    if (!domain) {
        return;
    }

    if (domain->stop()) {
        m_statusLabel->setText(tr("VM '%1' stopped").arg(domain->name()));
    }
}

void ManagerWindow::onVMRebooted()
{
    Domain *domain = m_vmModel->domainAt(m_vmList->currentIndex().row());
    if (!domain) {
        return;
    }

    if (domain->reboot()) {
        m_statusLabel->setText(tr("VM '%1' rebooting").arg(domain->name()));
    }
}

void ManagerWindow::onVMPaused()
{
    Domain *domain = m_vmModel->domainAt(m_vmList->currentIndex().row());
    if (!domain) {
        return;
    }

    m_statusLabel->setText(tr("VM '%1' paused").arg(domain->name()));
    updateVMControls();
}

void ManagerWindow::onVMResume()
{
    Domain *domain = m_vmModel->domainAt(m_vmList->currentIndex().row());
    if (!domain) {
        return;
    }

    m_statusLabel->setText(tr("VM '%1' resumed").arg(domain->name()));
    updateVMControls();
}

void ManagerWindow::onNewVM()
{
    m_statusLabel->setText(tr("New VM wizard not yet implemented"));
}

void ManagerWindow::onDeleteVM()
{
    Domain *domain = m_vmModel->domainAt(m_vmList->currentIndex().row());
    if (!domain) {
        return;
    }

    int ret = QMessageBox::warning(this, tr("Delete VM"),
                                   tr("Are you sure you want to delete VM '%1'?\n\n"
                                      "This will permanently remove the VM configuration.").arg(domain->name()),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        m_statusLabel->setText(tr("VM deletion not yet implemented"));
    }
}

void ManagerWindow::onVMSelectionChanged()
{
    updateVMControls();
}

void ManagerWindow::onConnectionSelectionChanged()
{
    updateVMControls();
}

void ManagerWindow::openConnectionDialog()
{
    ConnectionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString uri = dialog.uri();
        if (!uri.isEmpty()) {
            Connection *conn = Connection::open(uri);
            if (conn) {
                addConnection(conn);

                // Save to config
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
    m_statusLabel->setText(tr("Preferences not yet implemented"));
}

void ManagerWindow::updateVMControls()
{
    QModelIndex current = m_vmList->currentIndex();
    Domain *domain = m_vmModel->domainAt(current.row());

    bool hasSelection = (domain != nullptr);

    m_btnStart->setEnabled(hasSelection);
    m_btnStop->setEnabled(hasSelection);
    m_btnReboot->setEnabled(hasSelection);
    m_btnPause->setEnabled(hasSelection && domain && domain->state() == Domain::StateRunning);
    m_btnResume->setEnabled(false);  // Will be enabled when paused
    m_btnDeleteVM->setEnabled(hasSelection);
    m_btnOpenConsole->setEnabled(hasSelection && domain && domain->state() == Domain::StateRunning);

    // Update actions
    m_actionStart->setEnabled(m_btnStart->isEnabled());
    m_actionStop->setEnabled(m_btnStop->isEnabled());
    m_actionReboot->setEnabled(m_btnReboot->isEnabled());
    m_actionPause->setEnabled(m_btnPause->isEnabled());
    m_actionResume->setEnabled(m_btnResume->isEnabled());
    m_actionDeleteVM->setEnabled(m_btnDeleteVM->isEnabled());
    m_actionOpenConsole->setEnabled(m_btnOpenConsole->isEnabled());

    // Update VM count
    m_vmCountLabel->setText(tr("VMs: %1").arg(m_vmModel->domains().count()));
}

void ManagerWindow::refresh()
{
    m_vmModel->refresh();
    m_statusLabel->setText(tr("Refreshed"));
}

} // namespace QVirt
