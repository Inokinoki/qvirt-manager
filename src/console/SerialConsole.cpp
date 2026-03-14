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

#include "SerialConsole.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>

#ifdef QTERMWIDGET_FOUND
#include <QTermWidget>
#endif

namespace QVirt {

SerialConsole::SerialConsole(QWidget *parent)
    : QWidget(parent)
    , m_baudRate(115200)
    , m_connected(false)
    , m_wordWrap(true)
    , m_fontSize(10)
    , m_externalTerminalCmd("gnome-terminal --command=\"%s\"")
    , m_useExternalTerminal(true)
#ifdef QTERMWIDGET_FOUND
    , m_terminal(nullptr)
#else
    , m_ioDevice(nullptr)
    , m_placeholder(nullptr)
#endif
{
    setupUI();
}

SerialConsole::~SerialConsole()
{
    disconnect();
}

void SerialConsole::setupUI()
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // Show placeholder with external terminal option
    m_placeholder = new QWidget(this);
    m_placeholder->setMinimumSize(640, 480);
    m_placeholder->setStyleSheet("QWidget { background-color: #1a1a1a; }");
    
    auto *mainLabel = new QLabel(m_placeholder);
    mainLabel->setAlignment(Qt::AlignCenter);
    mainLabel->setStyleSheet("QLabel { color: #ccc; font-size: 16px; }");
    mainLabel->setText("Serial Console");
    
    auto *infoLabel = new QLabel(m_placeholder);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("QLabel { color: #888; margin: 20px; }");
    infoLabel->setWordWrap(true);
    infoLabel->setText(
        "Connect to serial console via external terminal\n\n"
        "The PTY path will be copied to clipboard and an external\n"
        "terminal will be launched automatically.\n\n"
        "Configure terminal command in Preferences."
    );
    
    m_placeholder->setLayout(new QVBoxLayout());
    m_placeholder->layout()->addWidget(mainLabel);
    m_placeholder->layout()->addWidget(infoLabel);
    m_placeholder->layout()->setAlignment(Qt::AlignCenter);
    
    layout->addWidget(m_placeholder);

    setFocusPolicy(Qt::StrongFocus);
}

void SerialConsole::connectToDevice(const QString &path, int baudRate)
{
    m_devicePath = path;
    m_baudRate = baudRate;

#ifdef QTERMWIDGET_FOUND
    // For PTY devices, we can use the terminal directly
    if (path.startsWith("/dev/pts/") || path.startsWith("/dev/tty")) {
        connectToPTY(path);
        return;
    }

    // For other serial devices, we'd need QSerialPort
    emit connectionError(tr("Serial device access requires QSerialPort"));
#else
    Q_UNUSED(path);
    Q_UNUSED(baudRate);
    emit connectionError(tr("Serial console not available - QTermWidget not found"));
#endif
}

void SerialConsole::disconnect()
{
#ifdef QTERMWIDGET_FOUND
    if (m_terminal) {
        m_terminal->closePtyProcess();
    }
#endif
    m_connected = false;
    m_readBuffer.clear();
    emit disconnected();
}

void SerialConsole::sendText(const QString &text)
{
    sendBytes(text.toUtf8());
}

void SerialConsole::sendBytes(const QByteArray &bytes)
{
#ifdef QTERMWIDGET_FOUND
    if (m_terminal && m_connected) {
        m_terminal->sendData(bytes);
    }
#else
    Q_UNUSED(bytes);
#endif
}

void SerialConsole::clearScreen()
{
#ifdef QTERMWIDGET_FOUND
    if (m_terminal) {
        m_terminal->clear();
    }
#endif
}

void SerialConsole::setWordWrap(bool wrap)
{
    m_wordWrap = wrap;
#ifdef QTERMWIDGET_FOUND
    if (m_terminal) {
        m_terminal->setWordWrap(wrap);
    }
#endif
}

void SerialConsole::setFontSize(int size)
{
    m_fontSize = size;
#ifdef QTERMWIDGET_FOUND
    if (m_terminal) {
        QFont font = m_terminal->getTerminalFont();
        font.setPointSize(size);
        m_terminal->setTerminalFont(font);
    }
#endif
}

void SerialConsole::setColorScheme(const QString &scheme)
{
    m_colorScheme = scheme;
#ifdef QTERMWIDGET_FOUND
    if (m_terminal) {
        m_terminal->setColorScheme(scheme);
    }
#endif
}

void SerialConsole::focusInEvent(QFocusEvent *event)
{
    QWidget::focusInEvent(event);
#ifdef QTERMWIDGET_FOUND
    if (m_terminal) {
        m_terminal->setFocus();
    }
#endif
}

void SerialConsole::onDataReceived()
{
#ifdef QTERMWIDGET_FOUND
    if (m_terminal) {
        QByteArray data = m_terminal->readAll();
        if (!data.isEmpty()) {
            emit dataReceived(data);
        }
    }
#endif
}

void SerialConsole::onErrorOccurred()
{
    emit connectionError(tr("Serial connection error"));
    disconnect();
}

void SerialConsole::setupPTY()
{
    // Setup PTY connection
}

void SerialConsole::updateTerminal()
{
    // Update terminal display
}

void SerialConsole::setExternalTerminalCommand(const QString &command)
{
    m_externalTerminalCmd = command;
}

void SerialConsole::setUseExternalTerminal(bool use)
{
    m_useExternalTerminal = use;
}

void SerialConsole::launchExternalTerminal()
{
    if (m_devicePath.isEmpty()) {
        emit connectionError("No PTY path configured");
        return;
    }

    // Copy PTY path to clipboard
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(m_devicePath);

    // Prepare command
    QString cmd = m_externalTerminalCmd;
    if (cmd.isEmpty()) {
        // Default to common terminals
        if (QProcess::execute("which gnome-terminal") == 0) {
            cmd = "gnome-terminal --command=\"%s\"";
        } else if (QProcess::execute("which konsole") == 0) {
            cmd = "konsole -e \"%s\"";
        } else if (QProcess::execute("which xterm") == 0) {
            cmd = "xterm -e \"%s\"";
        } else {
            cmd = "xterm -e \"%s\"";
        }
    }

    // Replace %s with PTY path
    QString fullCmd = cmd.replace("%s", m_devicePath);

    // Launch terminal
    QProcess::startDetached(fullCmd);

    m_connected = true;
    emit connected();
    
    emit connectionError(
        QString("External terminal launched for %1\n\n"
                "PTY path copied to clipboard: %2\n\n"
                "Command: %3")
            .arg(m_devicePath)
            .arg(m_devicePath)
            .arg(fullCmd)
    );
}

void SerialConsole::connectToPTY(const QString &ptyPath)
{
    m_devicePath = ptyPath;

    if (m_useExternalTerminal) {
        launchExternalTerminal();
    } else {
        // Try internal terminal if available
#ifdef QTERMWIDGET_FOUND
        if (m_terminal) {
            m_terminal->setPtyProcess(ptyPath.toUtf8().data());
            m_connected = true;
            emit connected();
        } else {
            emit connectionError("Internal terminal not initialized");
        }
#else
        emit connectionError("Internal terminal not available - enable external terminal");
#endif
    }
}

} // namespace QVirt
