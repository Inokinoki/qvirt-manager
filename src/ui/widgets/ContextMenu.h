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

#ifndef QVIRT_UI_WIDGETS_CONTEXTMENU_H
#define QVIRT_UI_WIDGETS_CONTEXTMENU_H

#include <QObject>
#include <QAction>
#include <QMenu>

namespace QVirt {

class Domain;
class Connection;

/**
 * @brief Context menu manager for VM list
 *
 * Provides right-click context menus for VM operations
 */
class VMContextMenu : public QObject
{
    Q_OBJECT

public:
    explicit VMContextMenu(QObject *parent = nullptr);
    ~VMContextMenu() override = default;

    QMenu* createMenu(QWidget *parent, Domain *domain);

signals:
    void startRequested();
    void stopRequested();
    void rebootRequested();
    void pauseRequested();
    void resumeRequested();
    void deleteRequested();
    void cloneRequested();
    void migrateRequested();
    void openConsoleRequested();
    void viewDetailsRequested();
    void takeSnapshotRequested();
};

/**
 * @brief Keyboard shortcut manager
 *
 * Manages keyboard shortcuts for common operations
 */
class KeyboardShortcuts : public QObject
{
    Q_OBJECT

public:
    explicit KeyboardShortcuts(QObject *parent = nullptr);
    ~KeyboardShortcuts() override = default;

    void setupShortcuts(QWidget *parent);
    QAction* actionForShortcut(const QString &name) const;

    // Shortcut names
    static const QString NEW_VM;
    static const QString DELETE_VM;
    static const QString START_VM;
    static const QString STOP_VM;
    static const QString REBOOT_VM;
    static const QString PAUSE_VM;
    static const QString REFRESH;
    static const QString OPEN_CONSOLE;
    static const QString PREFERENCES;

private:
    void createShortcut(QWidget *parent,
                       const QString &name,
                       const QString &keySeq,
                       const QObject *receiver,
                       const char *slot);

    QMap<QString, QAction*> m_shortcuts;
};

} // namespace QVirt

#endif // QVIRT_UI_WIDGETS_CONTEXTMENU_H
