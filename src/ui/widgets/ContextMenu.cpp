/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 Inoki <veyx.shaw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version).
 */

#include "ContextMenu.h"
#include "../../libvirt/Domain.h"
#include "../../libvirt/EnumMapper.h"

#include <QAction>

namespace QVirt {

// Static shortcut name constants
const QString KeyboardShortcuts::NEW_VM = "new_vm";
const QString KeyboardShortcuts::DELETE_VM = "delete_vm";
const QString KeyboardShortcuts::START_VM = "start_vm";
const QString KeyboardShortcuts::STOP_VM = "stop_vm";
const QString KeyboardShortcuts::REBOOT_VM = "reboot_vm";
const QString KeyboardShortcuts::PAUSE_VM = "pause_vm";
const QString KeyboardShortcuts::REFRESH = "refresh";
const QString KeyboardShortcuts::OPEN_CONSOLE = "open_console";
const QString KeyboardShortcuts::PREFERENCES = "preferences";

// ============================================================================
// VMContextMenu
// ============================================================================

VMContextMenu::VMContextMenu(QObject *parent)
    : QObject(parent)
{
}

QMenu* VMContextMenu::createMenu(QWidget *parent, Domain *domain)
{
    auto *menu = new QMenu(parent);

    // VM Actions
    auto *startAction = menu->addAction("Start");
    startAction->setIcon(QIcon::fromTheme("media-playback-start"));
    connect(startAction, &QAction::triggered, this, &VMContextMenu::startRequested);

    auto *stopAction = menu->addAction("Stop");
    stopAction->setIcon(QIcon::fromTheme("media-playback-stop"));
    connect(stopAction, &QAction::triggered, this, &VMContextMenu::stopRequested);

    auto *rebootAction = menu->addAction("Reboot");
    rebootAction->setIcon(QIcon::fromTheme("view-refresh"));
    connect(rebootAction, &QAction::triggered, this, &VMContextMenu::rebootRequested);

    menu->addSeparator();

    auto *pauseAction = menu->addAction("Pause");
    pauseAction->setIcon(QIcon::fromTheme("media-playback-pause"));
    connect(pauseAction, &QAction::triggered, this, &VMContextMenu::pauseRequested);

    // Enable/disable based on VM state
    if (domain) {
        bool isRunning = (domain->state() == Domain::StateRunning);
        bool isPaused = (domain->state() == Domain::StatePaused);

        startAction->setEnabled(!isRunning && !isPaused);
        stopAction->setEnabled(isRunning || isPaused);
        rebootAction->setEnabled(isRunning);
        pauseAction->setEnabled(isRunning);
    }

    menu->addSeparator();

    // Advanced actions
    auto *consoleAction = menu->addAction("Open Console");
    consoleAction->setIcon(QIcon::fromTheme("video-display"));
    connect(consoleAction, &QAction::triggered, this, &VMContextMenu::openConsoleRequested);

    auto *detailsAction = menu->addAction("View Details");
    detailsAction->setIcon(QIcon::fromTheme("document-properties"));
    connect(detailsAction, &QAction::triggered, this, &VMContextMenu::viewDetailsRequested);

    auto *snapshotAction = menu->addAction("Take Snapshot");
    snapshotAction->setIcon(QIcon::fromTheme("camera-photo"));
    connect(snapshotAction, &QAction::triggered, this, &VMContextMenu::takeSnapshotRequested);

    menu->addSeparator();

    // Management actions
    auto *cloneAction = menu->addAction("Clone...");
    cloneAction->setIcon(QIcon::fromTheme("edit-copy"));
    connect(cloneAction, &QAction::triggered, this, &VMContextMenu::cloneRequested);

    auto *migrateAction = menu->addAction("Migrate...");
    migrateAction->setIcon(QIcon::fromTheme("system-run"));
    connect(migrateAction, &QAction::triggered, this, &VMContextMenu::migrateRequested);

    menu->addSeparator();

    // Destructive actions
    auto *deleteAction = menu->addAction("Delete...");
    deleteAction->setIcon(QIcon::fromTheme("edit-delete"));
    connect(deleteAction, &QAction::triggered, this, &VMContextMenu::deleteRequested);

    return menu;
}

// ============================================================================
// KeyboardShortcuts
// ============================================================================

KeyboardShortcuts::KeyboardShortcuts(QObject *parent)
    : QObject(parent)
{
}

void KeyboardShortcuts::setupShortcuts(QWidget *parent)
{
    // New VM: Ctrl+N
    createShortcut(parent, NEW_VM, "Ctrl+N", parent, SLOT(onNewVM()));

    // Delete VM: Delete
    createShortcut(parent, DELETE_VM, "Delete", parent, SLOT(onDeleteVM()));

    // Start VM: Ctrl+S
    createShortcut(parent, START_VM, "Ctrl+S", parent, SLOT(onVMStarted()));

    // Stop VM: Ctrl+T
    createShortcut(parent, STOP_VM, "Ctrl+T", parent, SLOT(onVMStopped()));

    // Reboot VM: Ctrl+R
    createShortcut(parent, REBOOT_VM, "Ctrl+R", parent, SLOT(onVMRebooted()));

    // Pause VM: Ctrl+P
    createShortcut(parent, PAUSE_VM, "Ctrl+P", parent, SLOT(onVMPaused()));

    // Refresh: F5
    createShortcut(parent, REFRESH, "F5", parent, SLOT(refresh()));

    // Open Console: Ctrl+O
    createShortcut(parent, OPEN_CONSOLE, "Ctrl+O", parent, SLOT(openConsole()));

    // Preferences: Ctrl+, (comma)
    createShortcut(parent, PREFERENCES, "Ctrl+,", parent, SLOT(showPreferences()));
}

void KeyboardShortcuts::createShortcut(QWidget *parent,
                                       const QString &name,
                                       const QString &keySeq,
                                       const QObject *receiver,
                                       const char *slot)
{
    auto *action = new QAction(parent);
    action->setShortcut(QKeySequence(keySeq));
    parent->addAction(action);

    connect(action, SIGNAL(triggered()), receiver, slot);

    m_shortcuts[name] = action;
}

QAction* KeyboardShortcuts::actionForShortcut(const QString &name) const
{
    return m_shortcuts.value(name, nullptr);
}

} // namespace QVirt
