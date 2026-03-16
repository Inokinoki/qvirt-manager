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
#include "../../console/VNCViewer.h"
#include "../../console/SpiceViewer.h"

#include <QMessageBox>
#include <QInputDialog>

namespace QVirt {

ConsolePage::ConsolePage(Domain *domain, QWidget *parent)
    : QWidget(parent)
    , m_domain(domain)
    , m_connected(false)
    , m_fullscreen(false)
    , m_vncViewer(nullptr)
    , m_spiceViewer(nullptr)
    , m_currentViewer(nullptr)
{
    setupUI();
    setupToolbar();
    setupViewerStack();
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

void ConsolePage::setupViewerStack()
{
    // Create viewer widgets
    m_vncViewer = new VNCViewer(this);
    m_spiceViewer = new SpiceViewer(this);
    
    // Connect viewer signals
    connect(m_vncViewer, &VNCViewer::connectedChanged, this, [this](bool connected) {
        if (connected) {
            onViewerConnected();
        } else {
            onViewerDisconnected();
        }
    });
    connect(m_vncViewer, &VNCViewer::errorOccurred, this, &ConsolePage::onViewerError);
    
    connect(m_spiceViewer, &SpiceViewer::connectedChanged, this, [this](bool connected) {
        if (connected) {
            onViewerConnected();
        } else {
            onViewerDisconnected();
        }
    });
    connect(m_spiceViewer, &SpiceViewer::errorOccurred, this, &ConsolePage::onViewerError);
    
    // Add viewers to stack
    m_viewStack->addWidget(m_vncViewer);
    m_viewStack->addWidget(m_spiceViewer);
}

void ConsolePage::setupToolbar()
{
    m_toolbar = new ConsoleToolbar(this);
    
    // Connect toolbar signals
    connect(m_toolbar, &ConsoleToolbar::fullscreenToggled, this, &ConsolePage::onFullscreenToggled);
    connect(m_toolbar, &ConsoleToolbar::sendKeyRequested, this, &ConsolePage::onSendKeyRequested);
    connect(m_toolbar, &ConsoleToolbar::screenshotRequested, this, &ConsolePage::onScreenshot);
    connect(m_toolbar, &ConsoleToolbar::resizeGuestChanged, this, &ConsolePage::onResizeGuestChanged);
    connect(m_toolbar, &ConsoleToolbar::disconnectRequested, this, &ConsolePage::onDisconnectClicked);
    connect(m_toolbar, &ConsoleToolbar::usbRedirectionRequested, this, &ConsolePage::onUSBRedirectionRequested);
    connect(m_toolbar, &ConsoleToolbar::fileTransferRequested, this, &ConsolePage::onFileTransferRequested);
    connect(m_toolbar, &ConsoleToolbar::scaleFitRequested, this, &ConsolePage::onScaleFitRequested);
    connect(m_toolbar, &ConsoleToolbar::scaleFillRequested, this, &ConsolePage::onScaleFillRequested);
    connect(m_toolbar, &ConsoleToolbar::scale100Requested, this, &ConsolePage::onScale100Requested);

    // Connect action for toolbar
    m_actionConnect = new QAction("Connect", this);
    m_actionConnect->setIcon(QIcon::fromTheme("network-connect"));
    m_actionConnect->setToolTip("Connect to the console");
    connect(m_actionConnect, &QAction::triggered, this, &ConsolePage::onConnectClicked);
    m_toolbar->insertAction(m_toolbar->actions().first(), m_actionConnect);

    m_mainLayout->insertWidget(0, m_toolbar);
}

void ConsolePage::setupInfoView()
{
    auto *layout = new QVBoxLayout(m_infoWidget);
    layout->setContentsMargins(20, 20, 20, 20);

    // Title
    auto *titleLabel = new QLabel("Console Information", m_infoWidget);
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
    if (m_domain->state() != Domain::StateRunning) {
        QMessageBox::warning(this, "Cannot Connect",
            "The virtual machine must be running to connect to the console.");
        return;
    }

    // Get graphics type and connection info
    QString xml = m_domain->getXMLDesc(0);
    QString host = "127.0.0.1";
    int port = 0;
    int tlsPort = 0;
    QString password;

    // Parse VNC configuration
    if (m_graphicsType == "VNC") {
        // Extract port
        int portIdx = xml.indexOf("port='");
        if (portIdx > 0) {
            int start = portIdx + 6;
            int end = xml.indexOf("'", start);
            if (end > start) {
                port = xml.mid(start, end - start).toInt();
            }
        }
        // Extract listen address
        int addrIdx = xml.indexOf("listen='");
        if (addrIdx > 0) {
            int start = addrIdx + 8;
            int end = xml.indexOf("'", start);
            if (end > start) {
                host = xml.mid(start, end - start);
            }
        }
        // Extract password
        int passwdIdx = xml.indexOf("passwd='");
        if (passwdIdx > 0) {
            int start = passwdIdx + 8;
            int end = xml.indexOf("'", start);
            if (end > start) {
                password = xml.mid(start, end - start);
            }
        }

        // Connect VNC viewer
        if (port > 0 && m_vncViewer) {
            m_viewStack->setCurrentWidget(m_vncViewer);
            if (!password.isEmpty()) {
                m_vncViewer->setPassword(password);
            }
            m_vncViewer->connectToHost(host, port);
            m_statusLabel->setText("Connecting to VNC...");
        }
    } else if (m_graphicsType == "SPICE") {
        // Extract SPICE port
        int portIdx = xml.indexOf("port='");
        if (portIdx > 0) {
            int start = portIdx + 6;
            int end = xml.indexOf("'", start);
            if (end > start) {
                port = xml.mid(start, end - start).toInt();
            }
        }
        // Extract TLS port
        int tlsIdx = xml.indexOf("tlsPort='");
        if (tlsIdx > 0) {
            int start = tlsIdx + 9;
            int end = xml.indexOf("'", start);
            if (end > start) {
                tlsPort = xml.mid(start, end - start).toInt();
            }
        }
        // Extract listen address
        int addrIdx = xml.indexOf("listen='");
        if (addrIdx > 0) {
            int start = addrIdx + 8;
            int end = xml.indexOf("'", start);
            if (end > start) {
                host = xml.mid(start, end - start);
            }
        }

        // Connect SPICE viewer
        if (port > 0 && m_spiceViewer) {
            m_viewStack->setCurrentWidget(m_spiceViewer);
            if (!password.isEmpty()) {
                m_spiceViewer->setPassword(password);
            }
            if (tlsPort > 0) {
                m_spiceViewer->setTLSPort(tlsPort);
            }
            m_spiceViewer->connectToHost(host, port);
            m_statusLabel->setText("Connecting to SPICE...");
        }
    } else {
        QMessageBox::warning(this, "No Console",
            "No VNC or SPICE graphics device configured for this VM.");
        return;
    }
}

void ConsolePage::disconnectConsole()
{
    if (m_vncViewer) {
        m_vncViewer->disconnect();
    }
    if (m_spiceViewer) {
        m_spiceViewer->disconnect();
    }

    m_connected = false;
    m_statusLabel->setText("Disconnected");
    m_viewStack->setCurrentWidget(m_infoWidget);
    m_toolbar->setConnected(false);

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
    // Enable/disable auto-resize on viewers
    if (m_vncViewer) {
        m_vncViewer->setScalingEnabled(checked);
    }
    if (m_spiceViewer) {
        m_spiceViewer->setScalingEnabled(checked);
    }
    m_statusLabel->setText(QString("Resize guest: %1").arg(checked ? "Enabled" : "Disabled"));
}

void ConsolePage::onSendKeyRequested(const QString &keyCombo)
{
    if (!m_connected) {
        return;
    }
    
    if (m_graphicsType == "VNC" && m_vncViewer) {
        m_vncViewer->sendKeys(keyCombo);
    } else if (m_graphicsType == "SPICE" && m_spiceViewer) {
        m_spiceViewer->sendKeys(keyCombo);
    }
    m_statusLabel->setText("Sent key: " + keyCombo);
}

void ConsolePage::onUSBRedirectionRequested()
{
    if (m_spiceViewer) {
        // Show USB redirection dialog
        QMessageBox::information(this, "USB Redirection",
            "USB Redirection\n\n"
            "Select USB devices to redirect to the VM.\n\n"
            "Note: Requires SPICE protocol and usbredir configuration.");
    }
}

void ConsolePage::onFileTransferRequested()
{
    QMessageBox::information(this, "File Transfer",
        "File Transfer\n\n"
        "Drag and drop files to transfer them to the VM.\n\n"
        "Note: Requires SPICE protocol and spice-webdavd in guest.");
}

void ConsolePage::onScaleFitRequested()
{
    if (m_vncViewer) {
        m_vncViewer->setScalingEnabled(true);
    }
    if (m_spiceViewer) {
        m_spiceViewer->setScalingEnabled(true);
    }
    m_toolbar->setScalingEnabled(true);
}

void ConsolePage::onScaleFillRequested()
{
    if (m_vncViewer) {
        m_vncViewer->setScalingEnabled(true);
    }
    if (m_spiceViewer) {
        m_spiceViewer->setScalingEnabled(true);
    }
    m_toolbar->setScalingEnabled(true);
}

void ConsolePage::onScale100Requested()
{
    if (m_vncViewer) {
        m_vncViewer->setScalingEnabled(false);
    }
    if (m_spiceViewer) {
        m_spiceViewer->setScalingEnabled(false);
    }
    m_toolbar->setScalingEnabled(false);
}

void ConsolePage::onViewerError(const QString &message)
{
    m_statusLabel->setText("Error: " + message);
    QMessageBox::warning(this, "Console Error", message);
    disconnectConsole();
}

void ConsolePage::onViewerConnected()
{
    m_connected = true;
    m_statusLabel->setText("Connected to " + m_graphicsType + " console");
    m_toolbar->setConnected(true);
    m_toolbar->setViewerType(m_graphicsType);
    emit consoleConnected();
}

void ConsolePage::onViewerDisconnected()
{
    m_connected = false;
    m_statusLabel->setText("Disconnected");
    m_toolbar->setConnected(false);
    m_viewStack->setCurrentWidget(m_infoWidget);
    emit consoleDisconnected();
}

} // namespace QVirt
