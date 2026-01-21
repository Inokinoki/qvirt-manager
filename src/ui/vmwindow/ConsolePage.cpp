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

#include "ConsolePage.h"
#include "../../libvirt/EnumMapper.h"

#include <QMessageBox>
#include <QInputDialog>

namespace QVirt {

ConsolePage::ConsolePage(Domain *domain, QWidget *parent)
    : QWidget(parent)
    , m_domain(domain)
    , m_connected(false)
    , m_fullscreen(false)
{
    setupUI();
    setupToolbar();
    setupInfoView();
    setupPlaceholderView();
    updateConsoleInfo();
}

void ConsolePage::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // Create view stack for different console states
    m_viewStack = new QStackedWidget(this);

    m_infoWidget = new QWidget(m_viewStack);
    m_placeholderWidget = new QWidget(m_viewStack);

    m_viewStack->addWidget(m_infoWidget);
    m_viewStack->addWidget(m_placeholderWidget);

    m_mainLayout->addWidget(m_viewStack);

    // Status bar at bottom
    auto *statusLayout = new QHBoxLayout();
    statusLayout->setContentsMargins(5, 5, 5, 5);

    m_statusLabel = new QLabel("Not connected", this);
    statusLayout->addWidget(m_statusLabel);
    statusLayout->addStretch();

    m_mainLayout->addLayout(statusLayout);
}

void ConsolePage::setupToolbar()
{
    m_toolbar = new QToolBar("Console Toolbar", this);
    m_toolbar->setMovable(false);

    // Connect/Disconnect actions
    m_actionConnect = new QAction("Connect", this);
    m_actionConnect->setIcon(QIcon::fromTheme("network-connect"));
    m_actionConnect->setToolTip("Connect to the console");
    connect(m_actionConnect, &QAction::triggered, this, &ConsolePage::onConnectClicked);
    m_toolbar->addAction(m_actionConnect);

    m_actionDisconnect = new QAction("Disconnect", this);
    m_actionDisconnect->setIcon(QIcon::fromTheme("network-disconnect"));
    m_actionDisconnect->setToolTip("Disconnect from the console");
    m_actionDisconnect->setEnabled(false);
    connect(m_actionDisconnect, &QAction::triggered, this, &ConsolePage::onDisconnectClicked);
    m_toolbar->addAction(m_actionDisconnect);

    m_toolbar->addSeparator();

    // Fullscreen action
    m_actionFullscreen = new QAction("Fullscreen", this);
    m_actionFullscreen->setIcon(QIcon::fromTheme("view-fullscreen"));
    m_actionFullscreen->setToolTip("Toggle fullscreen mode");
    m_actionFullscreen->setCheckable(true);
    m_actionFullscreen->setEnabled(false);
    connect(m_actionFullscreen, &QAction::toggled, this, &ConsolePage::onFullscreenToggled);
    m_toolbar->addAction(m_actionFullscreen);

    // Send key combination
    m_actionSendKey = new QAction("Send Key", this);
    m_actionSendKey->setIcon(QIcon::fromTheme("preferences-desktop-keyboard-shortcuts"));
    m_actionSendKey->setToolTip("Send key combination to VM");
    m_actionSendKey->setEnabled(false);
    connect(m_actionSendKey, &QAction::triggered, this, &ConsolePage::onSendKey);
    m_toolbar->addAction(m_actionSendKey);

    // Screenshot
    m_actionScreenshot = new QAction("Screenshot", this);
    m_actionScreenshot->setIcon(QIcon::fromTheme("camera-photo"));
    m_actionScreenshot->setToolTip("Take a screenshot");
    m_actionScreenshot->setEnabled(false);
    connect(m_actionScreenshot, &QAction::triggered, this, &ConsolePage::onScreenshot);
    m_toolbar->addAction(m_actionScreenshot);

    m_toolbar->addSeparator();

    // Resize guest
    m_actionResizeGuest = new QAction("Resize Guest", this);
    m_actionResizeGuest->setToolTip("Allow console to resize guest resolution");
    m_actionResizeGuest->setCheckable(true);
    m_actionResizeGuest->setEnabled(false);
    connect(m_actionResizeGuest, &QAction::toggled, this, &ConsolePage::onResizeGuestChanged);
    m_toolbar->addAction(m_actionResizeGuest);

    m_mainLayout->insertWidget(0, m_toolbar);
}

void ConsolePage::setupInfoView()
{
    auto *layout = new QVBoxLayout(m_infoWidget);
    layout->setContentsMargins(20, 20, 20, 20);

    // Title
    auto *titleLabel = new QLabel("Console Information", m_infoWidget);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    layout->addWidget(titleLabel);

    layout->addSpacing(20);

    // Graphics type
    m_graphicsTypeLabel = new QLabel("Type: ", m_infoWidget);
    layout->addWidget(m_graphicsTypeLabel);

    // Listen address
    m_addressLabel = new QLabel("Address: ", m_infoWidget);
    layout->addWidget(m_addressLabel);

    // Port
    m_portLabel = new QLabel("Port: ", m_infoWidget);
    layout->addWidget(m_portLabel);

    layout->addSpacing(20);

    // Information text
    m_infoText = new QTextEdit(m_infoWidget);
    m_infoText->setReadOnly(true);
    m_infoText->setMaximumHeight(200);
    m_infoText->setHtml(
        "<p><b>Console Viewer Status:</b></p>"
        "<p>The console viewer allows you to interact with your virtual machine's "
        "graphical display.</p>"
        "<p><b>Supported types:</b></p>"
        "<ul>"
        "<li>VNC - Virtual Network Computing</li>"
        "<li>SPICE - Simple Protocol for Independent Computing Environments</li>"
        "</ul>"
        "<p><b>Note:</b> Full console viewer implementation requires integration with "
        "VNC or SPICE client libraries (libvncclient, Spice GTK). This is a placeholder "
        "that shows the console configuration.</p>"
    );
    layout->addWidget(m_infoText);

    layout->addStretch();
}

void ConsolePage::setupPlaceholderView()
{
    auto *layout = new QVBoxLayout(m_placeholderWidget);
    layout->setContentsMargins(20, 20, 20, 20);

    auto *label = new QLabel("Console Viewer", m_placeholderWidget);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 18px; font-weight: bold; padding: 50px;");
    layout->addWidget(label);

    auto *infoLabel = new QLabel(
        "The VM appears to be running. Connect to view the console.\n\n"
        "Note: Full console viewer implementation requires:\n"
        "- VNC: libvncclient or GtkVnc\n"
        "- SPICE: Spice GTK\n\n"
        "This placeholder shows the console configuration.",
        m_placeholderWidget
    );
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);

    layout->addStretch();
}

void ConsolePage::updateConsoleInfo()
{
    QString graphicsType = getGraphicsType();

    // Parse graphics XML to get details
    QString xml = m_domain->getXMLDesc(0);

    // Simple parsing for display info
    QString type, address, port;
    type = graphicsType;

    if (xml.contains("type='vnc'")) {
        type = "VNC";
        // Extract port if available
        int portIdx = xml.indexOf("port='");
        if (portIdx > 0) {
            int start = portIdx + 6;
            int end = xml.indexOf("'", start);
            port = xml.mid(start, end - start);
        }
        // Extract listen address
        int addrIdx = xml.indexOf("listen='");
        if (addrIdx > 0) {
            int start = addrIdx + 8;
            int end = xml.indexOf("'", start);
            address = xml.mid(start, end - start);
        }
    } else if (xml.contains("type='spice'")) {
        type = "SPICE";
        // Extract port for spice
        int portIdx = xml.indexOf("port='");
        if (portIdx > 0) {
            int start = portIdx + 6;
            int end = xml.indexOf("'", start);
            port = xml.mid(start, end - start);
        }
    }

    m_graphicsTypeLabel->setText("Type: " + (type.isEmpty() ? "Unknown" : type));
    m_addressLabel->setText("Address: " + (address.isEmpty() ? "0.0.0.0" : address));
    m_portLabel->setText("Port: " + (port.isEmpty() ? "Auto" : port));

    // Update connection status
    if (m_domain->state() == Domain::StateRunning) {
        m_statusLabel->setText("VM is running - Ready to connect");
        m_actionConnect->setEnabled(true);
    } else {
        m_statusLabel->setText("VM is not running");
        m_actionConnect->setEnabled(false);
    }
}

QString ConsolePage::getGraphicsType() const
{
    // Get domain XML and check for graphics devices
    QString xml = m_domain->getXMLDesc(0);

    if (xml.contains("<graphics type='vnc'")) {
        return "VNC";
    } else if (xml.contains("<graphics type='spice'")) {
        return "SPICE";
    } else if (xml.contains("<graphics")) {
        return "Other";
    }

    return "None";
}

void ConsolePage::refresh()
{
    updateConsoleInfo();
}

void ConsolePage::connectConsole()
{
    // Placeholder for actual console connection
    // In a full implementation, this would:
    // 1. Get the graphics device configuration
    // 2. Create appropriate viewer (VNC or SPICE)
    // 3. Establish connection to the display
    // 4. Embed the viewer widget

    m_connected = true;
    m_statusLabel->setText("Connected (placeholder)");
    m_viewStack->setCurrentWidget(m_placeholderWidget);

    // Update UI state
    m_actionConnect->setEnabled(false);
    m_actionDisconnect->setEnabled(true);
    m_actionFullscreen->setEnabled(true);
    m_actionSendKey->setEnabled(true);
    m_actionScreenshot->setEnabled(true);
    m_actionResizeGuest->setEnabled(true);

    emit consoleConnected();
}

void ConsolePage::disconnectConsole()
{
    // Placeholder for disconnection
    m_connected = false;
    m_statusLabel->setText("Disconnected");
    m_viewStack->setCurrentWidget(m_infoWidget);

    // Update UI state
    m_actionConnect->setEnabled(m_domain->state() == Domain::StateRunning);
    m_actionDisconnect->setEnabled(false);
    m_actionFullscreen->setEnabled(false);
    m_actionSendKey->setEnabled(false);
    m_actionScreenshot->setEnabled(false);
    m_actionResizeGuest->setEnabled(false);
    m_actionFullscreen->setChecked(false);

    emit consoleDisconnected();
}

void ConsolePage::onConnectClicked()
{
    if (m_domain->state() != Domain::StateRunning) {
        QMessageBox::warning(this, "Cannot Connect",
            "The virtual machine must be running to connect to the console.");
        return;
    }

    connectConsole();
}

void ConsolePage::onDisconnectClicked()
{
    disconnectConsole();
}

void ConsolePage::onFullscreenToggled(bool checked)
{
    Q_UNUSED(checked);

    if (m_fullscreen) {
        // Exit fullscreen
        showNormal();
        m_fullscreen = false;
    } else {
        // Enter fullscreen
        showFullScreen();
        m_fullscreen = true;
    }
}

void ConsolePage::onSendKey()
{
    bool ok;
    QString key = QInputDialog::getItem(
        this,
        "Send Key Combination",
        "Select key combination to send:",
        {"Ctrl+Alt+Delete", "Ctrl+Alt+BackSpace", "Print Screen"},
        0,
        false,
        &ok
    );

    if (ok && !key.isEmpty()) {
        // Placeholder for actual key sending
        // In a full implementation, this would send the key combination
        // through the viewer widget
        m_statusLabel->setText("Sent key: " + key + " (placeholder)");
    }
}

void ConsolePage::onScreenshot()
{
    // Placeholder for screenshot functionality
    // In a full implementation, this would capture the console display
    QMessageBox::information(this, "Screenshot",
        "Screenshot functionality placeholder.\n\n"
        "In a full implementation, this would capture the current "
        "console display and save it to a file.");
}

void ConsolePage::onResizeGuestChanged(bool checked)
{
    // Placeholder for resize guest functionality
    // In a full implementation, this would enable/disable
    // automatic resizing of the guest display resolution
    m_statusLabel->setText(QString("Resize guest: %1").arg(checked ? "Enabled" : "Disabled"));
}

} // namespace QVirt
