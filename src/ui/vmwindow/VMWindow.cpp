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

#include "VMWindow.h"
#include "OverviewPage.h"
#include "DetailsPage.h"
#include "ConsolePage.h"
#include "SnapshotsPage.h"

#include "../../core/Engine.h"
#include "../../core/Error.h"

#include "../dialogs/CloneDialog.h"
#include "../dialogs/DeleteDialog.h"
#include "../dialogs/AddHardwareDialog.h"
#include "../../devices/Device.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QStatusBar>
#include <QMenuBar>

namespace QVirt {

VMWindow::VMWindow(Domain *domain, QWidget *parent)
    : QMainWindow(parent)
    , m_domain(domain)
{
    setAttribute(Qt::WA_DeleteOnClose);

    // Set window title
    setWindowTitle(QString("%1 - Virt Manager").arg(m_domain->name()));

    setupUI();
    setupToolbar();
    setupMenuBar();
    connectSignals();

    // Initial update
    updateDomainState();

    // Set window size
    resize(900, 700);
}

VMWindow::~VMWindow()
{
    // QObject parent-child system handles cleanup
}

void VMWindow::setupUI()
{
    // Create central widget with tab widget
    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);

    // Create pages
    m_overviewPage = new OverviewPage(m_domain, this);
    m_detailsPage = new DetailsPage(m_domain, this);
    m_consolePage = new ConsolePage(m_domain, this);
    m_snapshotsPage = new SnapshotsPage(m_domain, this);

    // Add pages to tab widget
    m_tabWidget->addTab(m_overviewPage, "Overview");
    m_tabWidget->addTab(m_detailsPage, "Details");
    m_tabWidget->addTab(m_consolePage, "Console");
    m_tabWidget->addTab(m_snapshotsPage, "Snapshots");

    // Status bar
    m_statusLabel = new QLabel(this);
    m_cpuUsageLabel = new QLabel(this);
    m_memoryUsageLabel = new QLabel(this);

    statusBar()->addWidget(m_statusLabel, 1);
    statusBar()->addPermanentWidget(m_cpuUsageLabel);
    statusBar()->addPermanentWidget(m_memoryUsageLabel);
}

void VMWindow::setupToolbar()
{
    auto *toolbar = addToolBar("VM Controls");
    toolbar->setMovable(false);

    // Run actions
    m_actionStart = new QAction("Start", this);
    m_actionStart->setIcon(QIcon::fromTheme("media-playback-start"));
    m_actionStart->setToolTip("Start the VM");
    toolbar->addAction(m_actionStart);

    m_actionShutdown = new QAction("Shutdown", this);
    m_actionShutdown->setIcon(QIcon::fromTheme("media-playback-stop"));
    m_actionShutdown->setToolTip("Gracefully shutdown the VM");
    toolbar->addAction(m_actionShutdown);

    m_actionReboot = new QAction("Reboot", this);
    m_actionReboot->setIcon(QIcon::fromTheme("view-refresh"));
    m_actionReboot->setToolTip("Reboot the VM");
    toolbar->addAction(m_actionReboot);

    m_actionForceOff = new QAction("Force Off", this);
    m_actionForceOff->setIcon(QIcon::fromTheme("process-stop"));
    m_actionForceOff->setToolTip("Force power off the VM");
    toolbar->addAction(m_actionForceOff);

    toolbar->addSeparator();

    // Pause/Resume
    m_actionPause = new QAction("Pause", this);
    m_actionPause->setIcon(QIcon::fromTheme("media-playback-pause"));
    m_actionPause->setToolTip("Pause the VM");
    toolbar->addAction(m_actionPause);

    m_actionResume = new QAction("Resume", this);
    m_actionResume->setIcon(QIcon::fromTheme("media-playback-start"));
    m_actionResume->setToolTip("Resume the paused VM");
    toolbar->addAction(m_actionResume);

    toolbar->addSeparator();

    // Save/Restore
    m_actionSave = new QAction("Save", this);
    m_actionSave->setIcon(QIcon::fromTheme("document-save"));
    m_actionSave->setToolTip("Save VM state to file");
    toolbar->addAction(m_actionSave);

    toolbar->addSeparator();

    // Other actions
    m_actionScreenshot = new QAction("Screenshot", this);
    m_actionScreenshot->setIcon(QIcon::fromTheme("camera-photo"));
    toolbar->addAction(m_actionScreenshot);

    m_actionRefresh = new QAction("Refresh", this);
    m_actionRefresh->setIcon(QIcon::fromTheme("view-refresh"));
    toolbar->addAction(m_actionRefresh);
}

void VMWindow::setupMenuBar()
{
    // VM Menu
    m_menuVM = menuBar()->addMenu("VM");

    m_menuVM->addAction(m_actionStart);
    m_menuVM->addAction(m_actionShutdown);
    m_menuVM->addAction(m_actionReboot);
    m_menuVM->addAction(m_actionForceOff);
    m_menuVM->addSeparator();
    m_menuVM->addAction(m_actionPause);
    m_menuVM->addAction(m_actionResume);
    m_menuVM->addSeparator();
    m_menuVM->addAction(m_actionSave);
    m_menuVM->addAction(m_actionRestoreSaved);
    m_menuVM->addSeparator();
    m_menuVM->addAction(m_actionClone);
    m_menuVM->addAction(m_actionDelete);
    m_menuVM->addAction(m_actionMigrate);

    // View Menu
    m_menuView = menuBar()->addMenu("View");
    m_menuView->addAction(m_actionRefresh);
    m_menuView->addAction(m_actionScreenshot);

    // Device Menu
    m_menuDevice = menuBar()->addMenu("Device");
    m_actionAddHardware = new QAction("Add Hardware", this);
    m_menuDevice->addAction(m_actionAddHardware);

    // Help Menu
    m_menuHelp = menuBar()->addMenu("Help");
    auto *aboutAction = m_menuHelp->addAction("About");
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "About QVirt-Manager",
            "QVirt-Manager v1.0.0\n\n"
            "A Qt-based virtual machine manager\n\n"
            "Copyright (C) 2025-2026 Inoki <veyx.shaw@gmail.com>");
    });
}

void VMWindow::connectSignals()
{
    // Connect to domain signals
    connect(m_domain, &Domain::stateChanged, this, &VMWindow::onDomainStateChanged);
    connect(m_domain, &Domain::statsUpdated, this, &VMWindow::onDomainStatsUpdated);

    // Connect toolbar actions
    connect(m_actionStart, &QAction::triggered, this, &VMWindow::onStartClicked);
    connect(m_actionShutdown, &QAction::triggered, this, &VMWindow::onStopClicked);
    connect(m_actionReboot, &QAction::triggered, this, &VMWindow::onRebootClicked);
    connect(m_actionForceOff, &QAction::triggered, this, &VMWindow::onForceOffClicked);
    connect(m_actionPause, &QAction::triggered, this, &VMWindow::onPauseClicked);
    connect(m_actionResume, &QAction::triggered, this, &VMWindow::onResumeClicked);
    connect(m_actionSave, &QAction::triggered, this, &VMWindow::onSaveClicked);
    connect(m_actionRefresh, &QAction::triggered, this, &VMWindow::onRefresh);
    connect(m_actionScreenshot, &QAction::triggered, this, &VMWindow::onTakeScreenshot);

    // Connect menu actions
    connect(m_actionClone, &QAction::triggered, this, &VMWindow::onCloneVM);
    connect(m_actionDelete, &QAction::triggered, this, &VMWindow::onDeleteVM);
    connect(m_actionMigrate, &QAction::triggered, this, &VMWindow::onMigrateVM);
    connect(m_actionAddHardware, &QAction::triggered, this, &VMWindow::onAddHardware);
}

void VMWindow::updateDomainState()
{
    // Update status bar
    QString statusStr;
    switch (m_domain->state()) {
    case Domain::StateRunning:
        statusStr = "Running";
        break;
    case Domain::StatePaused:
        statusStr = "Paused";
        break;
    case Domain::StateShutOff:
        statusStr = "Shut Off";
        break;
    case Domain::StateShutdown:
        statusStr = "Shutting Down";
        break;
    case Domain::StateCrashed:
        statusStr = "Crashed";
        break;
    default:
        statusStr = "Unknown";
    }

    m_statusLabel->setText(QString("Status: %1").arg(statusStr));

    // Update stats
    m_cpuUsageLabel->setText(QString("CPU: %1%").arg(m_domain->cpuUsage(), 0, 'f', 1));
    m_memoryUsageLabel->setText(QString("Memory: %1 MB").arg(m_domain->currentMemory() / 1024));

    // Update control buttons
    updateControlButtons();

    // Update pages
    if (m_overviewPage) {
        static_cast<OverviewPage*>(m_overviewPage)->updateInfo();
    }
}

void VMWindow::updateControlButtons()
{
    bool isRunning = (m_domain->state() == Domain::StateRunning);
    bool isPaused = (m_domain->state() == Domain::StatePaused);
    bool isActive = isRunning || isPaused;
    bool isInactive = (m_domain->state() == Domain::StateShutOff);

    m_actionStart->setEnabled(isInactive);
    m_actionShutdown->setEnabled(isRunning);
    m_actionReboot->setEnabled(isRunning);
    m_actionForceOff->setEnabled(isActive);
    m_actionPause->setEnabled(isRunning);
    m_actionResume->setEnabled(isPaused);
    m_actionSave->setEnabled(isRunning);
    m_actionScreenshot->setEnabled(isRunning);
}

void VMWindow::refresh()
{
    m_domain->updateInfo();
    updateDomainState();
}

void VMWindow::onDomainStateChanged(Domain::State newState)
{
    Q_UNUSED(newState);
    updateDomainState();
}

void VMWindow::onDomainStatsUpdated()
{
    m_cpuUsageLabel->setText(QString("CPU: %1%").arg(m_domain->cpuUsage(), 0, 'f', 1));
    m_memoryUsageLabel->setText(QString("Memory: %1 MB").arg(m_domain->currentMemory() / 1024));

    if (m_overviewPage) {
        static_cast<OverviewPage*>(m_overviewPage)->updateStats();
    }
}

void VMWindow::onStartClicked()
{
    if (!m_domain->start()) {
        Error::showError("Failed to start VM", m_domain->name());
    }
}

void VMWindow::onStopClicked()
{
    if (!m_domain->shutdown()) {
        Error::showError("Failed to shutdown VM", m_domain->name());
    }
}

void VMWindow::onRebootClicked()
{
    if (!m_domain->reboot()) {
        Error::showError("Failed to reboot VM", m_domain->name());
    }
}

void VMWindow::onPauseClicked()
{
    QMessageBox::information(this, "Not Implemented", "Pause functionality will be implemented soon");
}

void VMWindow::onResumeClicked()
{
    QMessageBox::information(this, "Not Implemented", "Resume functionality will be implemented soon");
}

void VMWindow::onForceOffClicked()
{
    auto reply = QMessageBox::question(this, "Force Off",
        "Are you sure you want to force power off the VM? This may cause data loss.",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (!m_domain->destroy()) {
            Error::showError("Failed to force off VM", m_domain->name());
        }
    }
}

void VMWindow::onSaveClicked()
{
    QMessageBox::information(this, "Not Implemented", "Save functionality will be implemented soon");
}

void VMWindow::onRestoreSavedClicked()
{
    QMessageBox::information(this, "Not Implemented", "Restore saved state functionality will be implemented soon");
}

void VMWindow::onCloneVM()
{
    CloneDialog dialog(m_domain, this);
    if (dialog.exec() == QDialog::Accepted) {
        // Clone VM logic would go here
        // For now, just show a message with the clone parameters
        QString info = QString("Clone Configuration:\n")
            + QString("  Source: %1\n").arg(m_domain->name())
            + QString("  Clone Name: %1\n").arg(dialog.cloneName())
            + QString("  Clone Storage: %1\n").arg(dialog.cloneStorage() ? "Yes" : "No")
            + QString("  Generate MACs: %1\n").arg(dialog.generateMACs() ? "Yes" : "No");

        QMessageBox::information(this, "Clone VM (Placeholder)",
            info + "\n\nClone functionality requires libvirt XML manipulation. "
            "This dialog shows the clone configuration interface.");
    }
}

void VMWindow::onDeleteVM()
{
    DeleteDialog dialog(m_domain, this);
    if (dialog.exec() == QDialog::Accepted) {
        // Delete VM logic would go here
        QMessageBox::information(this, "Delete VM (Placeholder)",
            QString("VM '%1' would be deleted here.\n\n")
                .arg(m_domain->name()) +
            "Delete functionality requires:\n"
            "- Undefining the VM from libvirt\n"
            "- Optionally deleting storage volumes\n"
            "- Cleaning up related resources");
    }
}

void VMWindow::onMigrateVM()
{
    QMessageBox::information(this, "Not Implemented", "Migrate VM functionality will be implemented in Phase 8");
}

void VMWindow::onAddHardware()
{
    AddHardwareDialog dialog(m_domain, this);
    if (dialog.exec() == QDialog::Accepted) {
        Device *device = dialog.getCreatedDevice();
        if (device) {
            QMessageBox::information(this, "Add Hardware (Placeholder)",
                QString("Device created: %1\n\n")
                    .arg(device->deviceTypeName()) +
                "Device attachment requires:\n"
                "- Modifying VM XML\n"
                "- Attaching device to running VM (if hotpluggable)\n"
                "- Persisting the configuration");
            delete device;
        }
    }
}

void VMWindow::onRefresh()
{
    refresh();
}

void VMWindow::onTakeScreenshot()
{
    QMessageBox::information(this, "Not Implemented", "Screenshot functionality will be implemented when console viewer is ready");
}

} // namespace QVirt
