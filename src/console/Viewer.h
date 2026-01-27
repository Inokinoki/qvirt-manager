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

#ifndef QVIRT_CONSOLE_VIEWER_H
#define QVIRT_CONSOLE_VIEWER_H

#include <QWidget>
#include <QSize>

namespace QVirt {

/**
 * @brief Base class for console viewers
 *
 * This class provides an interface for console viewer implementations.
 * Subclasses can implement VNC, SPICE, or other console protocols.
 *
 * Note: Full implementation requires external libraries:
 * - VNC: libvncclient or GtkVnc
 * - SPICE: Spice GTK
 *
 * For now, this provides a placeholder that can be extended when
 * these dependencies are available.
 */
class Viewer : public QWidget
{
    Q_OBJECT

public:
    explicit Viewer(QWidget *parent = nullptr);
    virtual ~Viewer() = default;

    /**
     * @brief Connect to a console
     * @param host Host address
     * @param port Port number
     * @return true if connection initiated
     */
    virtual bool connectToHost(const QString &host, int port) = 0;

    /**
     * @brief Disconnect from console
     */
    virtual void disconnect() = 0;

    /**
     * @brief Send key sequence to VM
     * @param keys Key sequence
     */
    virtual void sendKeys(const QString &keys) = 0;

    /**
     * @brief Take screenshot
     * @return Screenshot image
     */
    virtual QPixmap takeScreenshot() = 0;

    /**
     * @brief Toggle fullscreen mode
     */
    virtual void toggleFullscreen() = 0;

    /**
     * @brief Get required size for viewer
     */
    virtual QSize sizeHint() const override;

signals:
    /**
     * @brief Emitted when connection state changes
     */
    void connectedChanged(bool connected);

    /**
     * @brief Emitted when an error occurs
     */
    void errorOccurred(const QString &message);

protected:
    bool m_connected = false;
};

} // namespace QVirt

#endif // QVIRT_CONSOLE_VIEWER_H
