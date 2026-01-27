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

#include "VNCViewer.h"
#include <QMessageBox>
#include <QPixmap>

namespace QVirt {

VNCViewer::VNCViewer(QWidget *parent)
    : Viewer(parent)
{
    auto *layout = new QVBoxLayout(this);

    m_displayLabel = new QLabel(
        "<html><body style='text-align:center; padding: 50px;'>"
        "<h2>VNC Console Viewer</h2>"
        "<p>VNC support requires external dependencies:</p>"
        "<ul>"
        "<li><b>libvncclient</b> - Lightweight VNC client library</li>"
        "<li><b>GtkVnc</b> - GTK VNC widget</li>"
        "</ul>"
        "<p>To enable VNC support:</p>"
        "<ol>"
        "<li>Install one of the above libraries</li>"
        "<li>Rebuild QVirt-Manager with VNC support enabled</li>"
        "</ol>"
        "<p><i>For now, you can use external VNC viewers like:</i></p>"
        "<ul>"
        "<li><b>vinagre</b> - GNOME VNC client</li>"
        "<li><b>krdc</b> - KDE Remote Desktop Client</li>"
        "<li><b>remmina</b> - Remote desktop client</li>"
        "<li><b>vncviewer</b> - TigerVNC client</li>"
        "</ul>"
        "</body></html>"
    );
    m_displayLabel->setWordWrap(true);
    m_displayLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_displayLabel);

    m_connectBtn = new QPushButton("Launch External VNC Viewer");
    layout->addWidget(m_connectBtn, 0, Qt::AlignCenter);

    connect(m_connectBtn, &QPushButton::clicked, this, [this]() {
        if (!m_host.isEmpty() && m_port > 0) {
            QString vncCmd = QString("vncviewer %1:%2").arg(m_host).arg(m_port);
            QMessageBox::information(this, "VNC Connection",
                QString("To connect manually, run:\n\n%1\n\n"
                    "Or use your preferred VNC client.").arg(vncCmd));
        } else {
            QMessageBox::information(this, "VNC Viewer",
                "No VNC connection configured.\n\n"
                "Please start the VM and configure VNC display first.");
        }
    });
}

bool VNCViewer::connectToHost(const QString &host, int port)
{
    m_host = host;
    m_port = port;

    // Placeholder - would establish actual VNC connection here
    // when libvncclient or GtkVnc is available
    m_connected = false;
    emit connectedChanged(m_connected);

    emit errorOccurred("VNC support requires libvncclient or GtkVnc library.\n"
                       "Please install dependencies and rebuild.");

    return false;
}

void VNCViewer::disconnect()
{
    m_connected = false;
    emit connectedChanged(m_connected);
}

void VNCViewer::sendKeys(const QString &keys)
{
    Q_UNUSED(keys);
    // Would send keys to VNC session
}

QPixmap VNCViewer::takeScreenshot()
{
    // Would capture VNC screen
    return QPixmap();
}

void VNCViewer::toggleFullscreen()
{
    // Would toggle fullscreen mode
}

} // namespace QVirt
