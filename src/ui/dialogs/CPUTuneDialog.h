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

#ifndef QVIRT_UI_DIALOGS_CPUTUNEDIALOG_H
#define QVIRT_UI_DIALOGS_CPUTUNEDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QSpinBox>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>

#include "../../libvirt/Domain.h"

namespace QVirt {

/**
 * @brief CPU Tuning configuration dialog
 *
 * Configure vCPU pinning, emulator pinning, and CPU scheduling
 */
class CPUTuneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CPUTuneDialog(Domain *domain, QWidget *parent = nullptr);
    ~CPUTuneDialog() override = default;

    void loadSettings();
    void saveSettings();

private slots:
    void onPinAll(int state);
    void onEmulatorPinAll(int state);

private:
    void setupUI();
    void setupVCPUTable();
    void setupEmulatorGroup();
    void setupSchedulerGroup();

    Domain *m_domain;

    QGroupBox *m_vcpuGroup;
    QTableWidget *m_vcpuTable;
    QCheckBox *m_pinAllCheck;

    QGroupBox *m_emulatorGroup;
    QSpinBox *m_emulatorPinSpin;
    QCheckBox *m_emulatorPinAllCheck;
    QLabel *m_emulatorInfoLabel;

    QGroupBox *m_schedulerGroup;
    QSpinBox *m_sharesSpin;
    QSpinBox *m_periodSpin;
    QSpinBox *m_quotaSpin;
    QSpinBox *m_realtimeSpin;

    QDialogButtonBox *m_buttonBox;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_CPUTUNEDIALOG_H
