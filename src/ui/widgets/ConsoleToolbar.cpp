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

#include "ConsoleToolbar.h"
#include <QInputDialog>
#include <QMessageBox>

namespace QVirt {

ConsoleToolbar::ConsoleToolbar(QWidget *parent)
    : QToolBar("Console Toolbar", parent)
{
    setMovable(false);
    setFloatable(false);
    setupUI();
}

void ConsoleToolbar::setupUI()
{
    setIconSize(QSize(16, 16));

    // Fullscreen action
    m_actionFullscreen = new QAction(QIcon::fromTheme("view-fullscreen", QIcon::fromTheme("fullscreen")), "Fullscreen", this);
    m_actionFullscreen->setToolTip("Toggle fullscreen mode (F11)");
    m_actionFullscreen->setShortcut(Qt::Key_F11);
    m_actionFullscreen->setCheckable(true);
    m_actionFullscreen->setEnabled(false);
    connect(m_actionFullscreen, &QAction::toggled, this, &ConsoleToolbar::fullscreenToggled);
    connect(m_actionFullscreen, &QAction::toggled, this, [this](bool checked) { m_fullscreen = checked; });
    addAction(m_actionFullscreen);

    addSeparator();

    // Scaling options
    m_actionScale = new QAction(QIcon::fromTheme("zoom-fit-best"), "Scale", this);
    m_actionScale->setToolTip("Display scaling options");
    m_actionScale->setEnabled(false);
    addAction(m_actionScale);

    m_scaleCombo = new QComboBox(this);
    m_scaleCombo->setToolTip("Select scaling mode");
    m_scaleCombo->setEditable(false);
    m_scaleCombo->addItems({"Fit Window", "Fill Window", "100%", "50%", "200%"});
    m_scaleCombo->setCurrentIndex(0);
    m_scaleCombo->setEnabled(false);
    connect(m_scaleCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
            this, &ConsoleToolbar::onScaleModeChanged);
    connect(m_scaleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                switch (index) {
                case 0: emit scaleFitRequested(); break;
                case 1: emit scaleFillRequested(); break;
                case 2: emit scale100Requested(); break;
                default: break;
                }
            });
    addWidget(m_scaleCombo);

    addSeparator();

    // Send Key menu
    m_actionSendKey = new QAction(QIcon::fromTheme("preferences-desktop-keyboard-shortcuts"), "Send Key", this);
    m_actionSendKey->setToolTip("Send key combination to VM");
    m_actionSendKey->setEnabled(false);
    connect(m_actionSendKey, &QAction::triggered, this, &ConsoleToolbar::showSendKeyMenu);
    addAction(m_actionSendKey);

    setupSendKeyMenu();

    // Screenshot
    m_actionScreenshot = new QAction(QIcon::fromTheme("camera-photo", QIcon::fromTheme("screenshot")), "Screenshot", this);
    m_actionScreenshot->setToolTip("Take a screenshot");
    m_actionScreenshot->setShortcut(QKeySequence::Save);
    m_actionScreenshot->setEnabled(false);
    connect(m_actionScreenshot, &QAction::triggered, this, &ConsoleToolbar::screenshotRequested);
    addAction(m_actionScreenshot);

    addSeparator();

    // Resize Guest
    m_actionResizeGuest = new QAction(QIcon::fromTheme("transform-scale"), "Auto Resize", this);
    m_actionResizeGuest->setToolTip("Allow console to resize guest resolution");
    m_actionResizeGuest->setCheckable(true);
    m_actionResizeGuest->setChecked(false);
    m_actionResizeGuest->setEnabled(false);
    connect(m_actionResizeGuest, &QAction::toggled, this, &ConsoleToolbar::resizeGuestChanged);
    addAction(m_actionResizeGuest);

    // USB Redirection (SPICE only)
    m_actionUSBRedir = new QAction(QIcon::fromTheme("media-usb"), "USB", this);
    m_actionUSBRedir->setToolTip("USB device redirection");
    m_actionUSBRedir->setEnabled(false);
    connect(m_actionUSBRedir, &QAction::triggered, this, &ConsoleToolbar::usbRedirectionRequested);
    addAction(m_actionUSBRedir);
    m_actionUSBRedir->setVisible(false);

    setupUSBMenu();

    // File Transfer (SPICE only)
    m_actionFileTransfer = new QAction(QIcon::fromTheme("document-send"), "File Transfer", this);
    m_actionFileTransfer->setToolTip("File transfer (drag and drop)");
    m_actionFileTransfer->setEnabled(false);
    connect(m_actionFileTransfer, &QAction::triggered, this, &ConsoleToolbar::fileTransferRequested);
    addAction(m_actionFileTransfer);
    m_actionFileTransfer->setVisible(false);

    addSeparator();

    // Disconnect
    m_actionDisconnect = new QAction(QIcon::fromTheme("network-disconnect", QIcon::fromTheme("disconnect")), "Disconnect", this);
    m_actionDisconnect->setToolTip("Disconnect from console");
    m_actionDisconnect->setEnabled(false);
    connect(m_actionDisconnect, &QAction::triggered, this, &ConsoleToolbar::disconnectRequested);
    addAction(m_actionDisconnect);
}

void ConsoleToolbar::setupSendKeyMenu()
{
    m_sendKeyMenu = new QMenu(this);

    auto *ctrlAltDel = m_sendKeyMenu->addAction("Ctrl+Alt+Delete");
    ctrlAltDel->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_Delete));
    connect(ctrlAltDel, &QAction::triggered, this, [this]() {
        emit sendKeyRequested("Ctrl+Alt+Delete");
    });

    auto *ctrlAltBack = m_sendKeyMenu->addAction("Ctrl+Alt+Backspace");
    ctrlAltBack->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_Backspace));
    connect(ctrlAltBack, &QAction::triggered, this, [this]() {
        emit sendKeyRequested("Ctrl+Alt+Backspace");
    });

    auto *printScreen = m_sendKeyMenu->addAction("Print Screen");
    connect(printScreen, &QAction::triggered, this, [this]() {
        emit sendKeyRequested("Print Screen");
    });

    m_sendKeyMenu->addSeparator();

    // Function keys
    auto *funcMenu = m_sendKeyMenu->addMenu("Function Keys");
    for (int i = 1; i <= 12; i++) {
        auto *action = funcMenu->addAction(QString("F%1").arg(i));
        connect(action, &QAction::triggered, this, [this, i]() {
            emit sendKeyRequested(QString("F%1").arg(i));
        });
    }

    m_sendKeyMenu->addSeparator();

    // Ctrl+Alt+Fx
    auto *ctrlAltFMenu = m_sendKeyMenu->addMenu("Ctrl+Alt+F");
    for (int i = 1; i <= 12; i++) {
        auto *action = ctrlAltFMenu->addAction(QString("F%1").arg(i));
        connect(action, &QAction::triggered, this, [this, i]() {
            emit sendKeyRequested(QString("Ctrl+Alt+F%1").arg(i));
        });
    }

    m_sendKeyMenu->addSeparator();

    // Custom key
    auto *customAction = m_sendKeyMenu->addAction("Custom...");
    connect(customAction, &QAction::triggered, this, [this]() {
        bool ok;
        QString key = QInputDialog::getText(this, "Send Key",
            "Enter key combination (e.g., Ctrl+Alt+T):", QLineEdit::Normal, QString(), &ok);
        if (ok && !key.isEmpty()) {
            emit sendKeyRequested(key);
        }
    });
}

void ConsoleToolbar::setupUSBMenu()
{
    m_usbMenu = new QMenu(this);

    auto *refreshAction = m_usbMenu->addAction("Refresh Devices");
    connect(refreshAction, &QAction::triggered, this, &ConsoleToolbar::usbRedirectionRequested);

    m_usbMenu->addSeparator();
    m_usbMenu->addAction("No USB devices connected");
}

void ConsoleToolbar::setViewerType(const QString &type)
{
    m_viewerType = type;

    // Show SPICE-specific actions only for SPICE
    bool isSpice = (type.toLower() == "spice");
    m_actionUSBRedir->setVisible(isSpice);
    m_actionFileTransfer->setVisible(isSpice);
}

void ConsoleToolbar::setConnected(bool connected)
{
    m_actionFullscreen->setEnabled(connected);
    m_actionScale->setEnabled(connected);
    m_scaleCombo->setEnabled(connected);
    m_actionSendKey->setEnabled(connected);
    m_actionScreenshot->setEnabled(connected);
    m_actionResizeGuest->setEnabled(connected);
    m_actionDisconnect->setEnabled(connected);

    if (m_viewerType.toLower() == "spice") {
        m_actionUSBRedir->setEnabled(connected);
        m_actionFileTransfer->setEnabled(connected);
    }
}

void ConsoleToolbar::setScalingEnabled(bool enabled)
{
    m_scalingEnabled = enabled;
    m_scaleCombo->setEnabled(enabled && m_actionScale->isEnabled());
}

bool ConsoleToolbar::isScalingEnabled() const
{
    return m_scalingEnabled;
}

void ConsoleToolbar::setFullscreen(bool fullscreen)
{
    m_fullscreen = fullscreen;
    m_actionFullscreen->setChecked(fullscreen);
    m_actionFullscreen->setIcon(QIcon::fromTheme(
        fullscreen ? "view-restore" : "view-fullscreen",
        fullscreen ? QIcon::fromTheme("restore") : QIcon::fromTheme("fullscreen")));
}

void ConsoleToolbar::setUSBRedirectionEnabled(bool enabled)
{
    m_usbRedirectionEnabled = enabled;
}

bool ConsoleToolbar::isUSBRedirectionEnabled() const
{
    return m_usbRedirectionEnabled;
}

void ConsoleToolbar::onScaleModeChanged(const QString &mode)
{
    emit scalingChanged(mode);
}

void ConsoleToolbar::onSendKeyTriggered()
{
    showSendKeyMenu();
}

void ConsoleToolbar::showSendKeyMenu()
{
    m_sendKeyMenu->exec(mapToGlobal(m_actionSendKey->isEnabled() ?
        widgetForAction(m_actionSendKey)->geometry().bottomLeft() :
        rect().bottomLeft()));
}

} // namespace QVirt
