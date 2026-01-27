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

#ifndef QVIRT_CONSOLE_VNCVIEWER_H
#define QVIRT_CONSOLE_VNCVIEWER_H

#include "Viewer.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace QVirt {

/**
 * @brief VNC Console Viewer
 *
 * This is a placeholder implementation that displays a message
 * indicating the need for libvncclient or GtkVnc dependencies.
 *
 * To implement full VNC support:
 * 1. Install libvncclient or GtkVnc development libraries
 * 2. Add dependency detection to CMakeLists.txt
 * 3. Implement VNC protocol handling using the library
 * 4. Handle keyboard/mouse events
 * 5. Implement display updates
 */
class VNCViewer : public Viewer
{
    Q_OBJECT

public:
    explicit VNCViewer(QWidget *parent = nullptr);
    ~VNCViewer() override = default;

    bool connectToHost(const QString &host, int port) override;
    void disconnect() override;
    void sendKeys(const QString &keys) override;
    QPixmap takeScreenshot() override;
    void toggleFullscreen() override;

private:
    QLabel *m_displayLabel;
    QPushButton *m_connectBtn;
    QString m_host;
    int m_port = 0;
};

} // namespace QVirt

#endif // QVIRT_CONSOLE_VNCVIEWER_H
