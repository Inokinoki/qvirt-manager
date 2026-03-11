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

#ifndef QVIRT_CONSOLE_SPICEVIEWER_H
#define QVIRT_CONSOLE_SPICEVIEWER_H

#include "Viewer.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace QVirt {

/**
 * @brief SPICE Console Viewer
 *
 * This is a placeholder implementation that displays a message
 * indicating the need for Spice GTK dependencies.
 *
 * To implement full SPICE support:
 * 1. Install Spice GTK development libraries
 * 2. Add dependency detection to CMakeLists.txt
 * 3. Implement SPICE protocol handling using Spice GTK
 * 4. Handle keyboard/mouse events
 * 5. Implement display updates
 * 6. Add USB redirection support
 */
class SpiceViewer : public Viewer
{
    Q_OBJECT

public:
    explicit SpiceViewer(QWidget *parent = nullptr);
    ~SpiceViewer() override = default;

    bool connectToHost(const QString &host, int port) override;
    void disconnect() override;
    void sendKeys(const QString &keys) override;
    QPixmap takeScreenshot() override;
    void toggleFullscreen() override;

    /**
     * @brief Set SPICE TLS port
     */
    void setTLSPort(int port);

private:
    QLabel *m_displayLabel;
    QPushButton *m_connectBtn;
    QString m_host;
    int m_port = 0;
    int m_tlsPort = 0;
};

} // namespace QVirt

#endif // QVIRT_CONSOLE_SPICEVIEWER_H
