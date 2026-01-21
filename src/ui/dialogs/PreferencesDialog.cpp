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

#include "PreferencesDialog.h"
#include "../../core/Config.h"

#include <QMessageBox>
#include <QFileDialog>

namespace QVirt {

PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Preferences");
    resize(700, 500);

    setupUI();
}

void PreferencesDialog::setupUI()
{
    auto *layout = new QVBoxLayout(this);

    // Create tab widget
    m_tabWidget = new QTabWidget(this);

    setupGeneralTab();
    setupPollingTab();
    setupConsoleTab();

    layout->addWidget(m_tabWidget);

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    auto *btnReset = new QPushButton("Reset to Defaults", this);
    auto *btnApply = new QPushButton("Apply", this);
    auto *btnClose = new QPushButton("Close", this);

    connect(btnReset, &QPushButton::clicked, this, &PreferencesDialog::onResetClicked);
    connect(btnApply, &QPushButton::clicked, this, &PreferencesDialog::onApplyClicked);
    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);

    buttonLayout->addWidget(btnReset);
    buttonLayout->addWidget(btnApply);
    buttonLayout->addWidget(btnClose);

    layout->addLayout(buttonLayout);

    // Connect tab change
    connect(m_tabWidget, &QTabWidget::currentChanged, this, &PreferencesDialog::onTabChanged);
}

void PreferencesDialog::setupGeneralTab()
{
    auto *tab = new QWidget(m_tabWidget);
    auto *layout = new QVBoxLayout(tab);

    // Confirmation settings
    auto *confirmGroup = new QGroupBox("Confirmation", tab);
    auto *confirmLayout = new QVBoxLayout(confirmGroup);

    m_confirmForceOffCheck = new QCheckBox(
        "Confirm force powering off VMs", confirmGroup);
    m_confirmForceOffCheck->setChecked(true);
    confirmLayout->addWidget(m_confirmForceOffCheck);

    m_confirmDeleteCheck = new QCheckBox(
        "Confirm deleting VMs and storage", confirmGroup);
    m_confirmDeleteCheck->setChecked(true);
    confirmLayout->addWidget(m_confirmDeleteCheck);

    layout->addWidget(confirmGroup);

    // Connection settings
    auto *connectionGroup = new QGroupBox("Connections", tab);
    auto *connectionLayout = new QVBoxLayout(connectionGroup);

    m_autoconnectCheck = new QCheckBox(
        "Auto-connect to saved connections on startup", connectionGroup);
    m_autoconnectCheck->setChecked(true);
    connectionLayout->addWidget(m_autoconnectCheck);

    layout->addWidget(connectionGroup);

    // Storage settings
    auto *storageGroup = new QGroupBox("Storage", tab);
    auto *storageLayout = new QFormLayout(storageGroup);

    m_defaultStoragePathEdit = new QLineEdit(tab);
    m_defaultStoragePathEdit->setText("/var/lib/libvirt/images");
    storageLayout->addRow("Default Storage Path:", m_defaultStoragePathEdit);

    auto *btnBrowse = new QPushButton("Browse...", tab);
    storageLayout->addRow("", btnBrowse);
    connect(btnBrowse, &QPushButton::clicked, [this]() {
        QString path = QFileDialog::getExistingDirectory(this, "Select Default Storage Directory");
        if (!path.isEmpty()) {
            m_defaultStoragePathEdit->setText(path);
        }
    });

    layout->addWidget(storageGroup);

    layout->addStretch();

    m_tabWidget->addTab(tab, "General");
}

void PreferencesDialog::setupPollingTab()
{
    auto *tab = new QWidget(m_tabWidget);
    auto *layout = new QVBoxLayout(tab);

    auto *pollGroup = new QGroupBox("Polling Intervals", tab);
    auto *pollLayout = new QFormLayout(pollGroup);

    // VM update interval
    m_vmUpdateIntervalSpin = new QSpinBox(tab);
    m_vmUpdateIntervalSpin->setRange(1, 60);
    m_vmUpdateIntervalSpin->setValue(2);
    m_vmUpdateIntervalSpin->setSuffix(" seconds");
    m_vmUpdateIntervalSpin->setToolTip("How often to refresh VM list and states");
    pollLayout->addRow("VM Update Interval:", m_vmUpdateIntervalSpin);

    // CPU poll interval
    m_cpuPollIntervalSpin = new QSpinBox(tab);
    m_cpuPollIntervalSpin->setRange(1, 60);
    m_cpuPollIntervalSpin->setValue(1);
    m_cpuPollIntervalSpin->setSuffix(" seconds");
    m_cpuPollIntervalSpin->setToolTip("How often to poll CPU statistics");
    pollLayout->addRow("CPU Stats Interval:", m_cpuPollIntervalSpin);

    // Disk poll interval
    m_diskPollIntervalSpin = new QSpinBox(tab);
    m_diskPollIntervalSpin->setRange(1, 60);
    m_diskPollIntervalSpin->setValue(5);
    m_diskPollIntervalSpin->setSuffix(" seconds");
    m_diskPollIntervalSpin->setToolTip("How often to poll disk statistics");
    pollLayout->addRow("Disk Stats Interval:", m_diskPollIntervalSpin);

    // Network poll interval
    m_netPollIntervalSpin = new QSpinBox(tab);
    m_netPollIntervalSpin->setRange(1, 60);
    m_netPollIntervalSpin->setValue(3);
    m_netPollIntervalSpin->setSuffix(" seconds");
    m_netPollIntervalSpin->setToolTip("How often to poll network statistics");
    pollLayout->addRow("Network Stats Interval:", m_netPollIntervalSpin);

    layout->addWidget(pollGroup);

    auto *infoLabel = new QLabel(
        "<b>Note:</b> Lower polling intervals provide more real-time updates "
        "but consume more system resources.",
        tab);
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);

    layout->addStretch();

    m_tabWidget->addTab(tab, "Polling");
}

void PreferencesDialog::setupConsoleTab()
{
    auto *tab = new QWidget(m_tabWidget);
    auto *layout = new QVBoxLayout(tab);

    auto *consoleGroup = new QGroupBox("Console Display", tab);
    auto *consoleLayout = new QVBoxLayout(consoleGroup);

    m_consoleScaleCheck = new QCheckBox(
        "Scale console display to window size", consoleGroup);
    m_consoleScaleCheck->setChecked(true);
    consoleLayout->addWidget(m_consoleScaleCheck);

    m_consoleResizeGuestCheck = new QCheckBox(
        "Allow console display to resize guest", consoleGroup);
    m_consoleResizeGuestCheck->setChecked(false);
    consoleLayout->addWidget(m_consoleResizeGuestCheck);

    layout->addWidget(consoleGroup);

    auto *keyGroup = new QGroupBox("Keyboard Grab", tab);
    auto *keyLayout = new QFormLayout(keyGroup);

    m_consoleKeyComboCombo = new QComboBox(tab);
    m_consoleKeyComboCombo->addItem("Ctrl+Alt", "ctrl+alt");
    m_consoleKeyComboCombo->addItem("Ctrl+Alt+Shift", "ctrl+alt+shift");
    m_consoleKeyComboCombo->setCurrentIndex(0);
    keyLayout->addRow("Key Combination:", m_consoleKeyComboCombo);

    layout->addWidget(keyGroup);

    auto *usbGroup = new QGroupBox("USB Redirection", tab);
    auto *usbLayout = new QVBoxLayout(usbGroup);

    m_consoleRedirectUSBCheck = new QCheckBox(
        "Redirect USB devices to console when connected", usbGroup);
    m_consoleRedirectUSBCheck->setChecked(false);
    usbLayout->addWidget(m_consoleRedirectUSBCheck);

    layout->addWidget(usbGroup);

    layout->addStretch();

    m_tabWidget->addTab(tab, "Console");
}

void PreferencesDialog::onApplyClicked()
{
    applySettings();
    QMessageBox::information(this, "Preferences",
        "Settings have been applied successfully.");
}

void PreferencesDialog::onResetClicked()
{
    auto reply = QMessageBox::question(this, "Reset Preferences",
        "Are you sure you want to reset all preferences to their default values?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        resetSettings();
    }
}

void PreferencesDialog::onTabChanged(int index)
{
    Q_UNUSED(index);
    // Can be used for tab-specific initialization
}

void PreferencesDialog::applySettings()
{
    Config *config = Config::instance();

    // General settings
    config->setConfirmForceOff(m_confirmForceOffCheck->isChecked());
    config->setConfirmDelete(m_confirmDeleteCheck->isChecked());
    config->setAutoconnectOnStartup(m_autoconnectCheck->isChecked());
    config->setDefaultStoragePath(m_defaultStoragePathEdit->text());

    // Polling settings
    config->setVMUpdateInterval(m_vmUpdateIntervalSpin->value());
    config->setCPUPollInterval(m_cpuPollIntervalSpin->value());
    config->setDiskPollInterval(m_diskPollIntervalSpin->value());
    config->setNetworkPollInterval(m_netPollIntervalSpin->value());

    // Console settings
    config->setConsoleScale(m_consoleScaleCheck->isChecked());
    config->setConsoleResizeGuest(m_consoleResizeGuestCheck->isChecked());
    config->setConsoleKeyCombo(m_consoleKeyComboCombo->currentData().toString());
    config->setConsoleRedirectUSB(m_consoleRedirectUSBCheck->isChecked());
}

void PreferencesDialog::resetSettings()
{
    // Reset to defaults
    m_confirmForceOffCheck->setChecked(true);
    m_confirmDeleteCheck->setChecked(true);
    m_autoconnectCheck->setChecked(true);
    m_defaultStoragePathEdit->setText("/var/lib/libvirt/images");

    m_vmUpdateIntervalSpin->setValue(2);
    m_cpuPollIntervalSpin->setValue(1);
    m_diskPollIntervalSpin->setValue(5);
    m_netPollIntervalSpin->setValue(3);

    m_consoleScaleCheck->setChecked(true);
    m_consoleResizeGuestCheck->setChecked(false);
    m_consoleKeyComboCombo->setCurrentIndex(0);
    m_consoleRedirectUSBCheck->setChecked(false);
}

} // namespace QVirt
