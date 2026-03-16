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

#ifndef QVIRT_CONSOLE_SERIALCONSOLE_H
#define QVIRT_CONSOLE_SERIALCONSOLE_H

#include <QWidget>
#include <QString>
#include <QByteArray>
#include <QIODevice>

#ifdef QTERMWIDGET_FOUND
#include <QTermWidget>
#endif

namespace QVirt {

/**
 * @brief Serial console widget for VM serial ports
 *
 * Provides terminal emulation for serial console access
 */
class SerialConsole : public QWidget
{
    Q_OBJECT

public:
    explicit SerialConsole(QWidget *parent = nullptr);
    ~SerialConsole() override;

    QString devicePath() const { return m_devicePath; }
    int baudRate() const { return m_baudRate; }
    bool isConnected() const { return m_connected; }

    void connectToDevice(const QString &path, int baudRate = 115200);
    void connectToPTY(const QString &ptyPath);
    void disconnect();

    void sendText(const QString &text);
    void sendBytes(const QByteArray &bytes);

    void clearScreen();
    void setWordWrap(bool wrap);
    bool wordWrap() const { return m_wordWrap; }

    void setFontSize(int size);
    int fontSize() const { return m_fontSize; }

    void setColorScheme(const QString &scheme);
    QString colorScheme() const { return m_colorScheme; }

    /**
     * @brief Set external terminal command
     * @param command Command template (%s will be replaced with PTY path)
     */
    void setExternalTerminalCommand(const QString &command);
    QString externalTerminalCommand() const { return m_externalTerminalCmd; }

    /**
     * @brief Launch external terminal for PTY connection
     */
    void launchExternalTerminal();

    /**
     * @brief Check if external terminal is enabled
     */
    bool useExternalTerminal() const { return m_useExternalTerminal; }
    void setUseExternalTerminal(bool use);

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &error);
    void dataReceived(const QByteArray &data);

protected:
    void focusInEvent(QFocusEvent *event) override;

private slots:
    void onDataReceived();
    void onErrorOccurred();

private:
    void setupUI();
    void setupPTY();
    void updateTerminal();

    QString m_devicePath;
    int m_baudRate;
    bool m_connected;
    bool m_wordWrap;
    int m_fontSize;
    QString m_colorScheme;
    QString m_externalTerminalCmd;
    bool m_useExternalTerminal;

#ifdef QTERMWIDGET_FOUND
    QTermWidget *m_terminal;
#else
    QIODevice *m_ioDevice;
    QWidget *m_placeholder;
#endif
    QByteArray m_readBuffer;
};

} // namespace QVirt

#endif // QVIRT_CONSOLE_SERIALCONSOLE_H
