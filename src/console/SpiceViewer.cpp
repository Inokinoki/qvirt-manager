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

#include "SpiceViewer.h"
#include <QMessageBox>
#include <QPixmap>

namespace QVirt {

SpiceViewer::SpiceViewer(QWidget *parent)
    : Viewer(parent)
{
    auto *layout = new QVBoxLayout(this);

    m_displayLabel = new QLabel(
        "<html><body style='text-align:center; padding: 50px;'>"
        "<h2>SPICE Console Viewer</h2>"
        "<p>SPICE support requires external dependencies:</p>"
        "<ul>"
        "<li><b>Spice GTK</b> - SPICE protocol widget for GTK/Qt</li>"
        "<li><b>Spice Protocol</b> - SPICE communication library</li>"
        "</ul>"
        "<p>To enable SPICE support:</p>"
        "<ol>"
        "<li>Install Spice GTK development libraries</li>"
        "<li>Rebuild QVirt-Manager with SPICE support enabled</li>"
        "</ol>"
        "<p><i>For now, you can use external SPICE viewers like:</i></p>"
        "<ul>"
        "<li><b>virt-viewer</b> - Virtual Machine Viewer</li>"
        "<li><b>remote-viewer</b> - Remote display client</li>"
        "</ul>"
        "</body></html>"
    );
    m_displayLabel->setWordWrap(true);
    m_displayLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_displayLabel);

    m_connectBtn = new QPushButton("Launch External SPICE Viewer");
    layout->addWidget(m_connectBtn, 0, Qt::AlignCenter);

    connect(m_connectBtn, &QPushButton::clicked, this, [this]() {
        if (!m_host.isEmpty() && m_port > 0) {
            QString spiceCmd = QString("remote-viewer spice://%1:%2").arg(m_host).arg(m_port);
            QMessageBox::information(this, "SPICE Connection",
                QString("To connect manually, run:\n\n%1\n\n"
                    "Or use virt-viewer/remote-viewer.").arg(spiceCmd));
        } else {
            QMessageBox::information(this, "SPICE Viewer",
                "No SPICE connection configured.\n\n"
                "Please start the VM and configure SPICE display first.");
        }
    });
}

bool SpiceViewer::connectToHost(const QString &host, int port)
{
    m_host = host;
    m_port = port;

    // Placeholder - would establish actual SPICE connection here
    // when Spice GTK is available
    m_connected = false;
    emit connectedChanged(m_connected);

    emit errorOccurred("SPICE support requires Spice GTK library.\n"
                       "Please install dependencies and rebuild.");

    return false;
}

void SpiceViewer::disconnect()
{
    m_connected = false;
    emit connectedChanged(m_connected);
}

void SpiceViewer::sendKeys(const QString &keys)
{
    Q_UNUSED(keys);
    // Would send keys to SPICE session
}

QPixmap SpiceViewer::takeScreenshot()
{
    // Would capture SPICE screen
    return QPixmap();
}

void SpiceViewer::toggleFullscreen()
{
    // Would toggle fullscreen mode
}

void SpiceViewer::setTLSPort(int port)
{
    m_tlsPort = port;
}

} // namespace QVirt
