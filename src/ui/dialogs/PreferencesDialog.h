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

#ifndef QVIRT_UI_DIALOGS_PREFERENCESDIALOG_H
#define QVIRT_UI_DIALOGS_PREFERENCESDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

namespace QVirt {

/**
 * @brief Preferences Dialog
 *
 * Dialog for configuring application preferences
 */
class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog() override = default;

private slots:
    void onApplyClicked();
    void onResetClicked();
    void onTabChanged(int index);

private:
    void setupUI();
    void setupGeneralTab();
    void setupPollingTab();
    void setupConsoleTab();
    void applySettings();
    void resetSettings();

    QTabWidget *m_tabWidget;

    // General settings
    QCheckBox *m_confirmForceOffCheck;
    QCheckBox *m_confirmDeleteCheck;
    QCheckBox *m_autoconnectCheck;
    QLineEdit *m_defaultStoragePathEdit;

    // Polling settings
    QSpinBox *m_vmUpdateIntervalSpin;
    QSpinBox *m_cpuPollIntervalSpin;
    QSpinBox *m_diskPollIntervalSpin;
    QSpinBox *m_netPollIntervalSpin;

    // Console settings
    QCheckBox *m_consoleScaleCheck;
    QCheckBox *m_consoleResizeGuestCheck;
    QComboBox *m_consoleKeyComboCombo;
    QCheckBox *m_consoleRedirectUSBCheck;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_PREFERENCESDIALOG_H
