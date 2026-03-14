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

#ifndef QVIRT_UI_DIALOGS_MEMTUNEDIALOG_H
#define QVIRT_UI_DIALOGS_MEMTUNEDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>

#include "../../libvirt/Domain.h"

namespace QVirt {

/**
 * @brief Memory Tuning configuration dialog
 */
class MemTuneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MemTuneDialog(Domain *domain, QWidget *parent = nullptr);
    ~MemTuneDialog() override = default;

    void loadSettings();
    void saveSettings();

private:
    void setupUI();

    Domain *m_domain;

    QGroupBox *m_hardLimitGroup;
    QSpinBox *m_hardLimitSpin;
    QCheckBox *m_hardLimitUnlimited;

    QGroupBox *m_softLimitGroup;
    QSpinBox *m_softLimitSpin;
    QCheckBox *m_softLimitUnlimited;

    QGroupBox *mSwapGroup;
    QSpinBox *m_swapHardSpin;
    QCheckBox *m_swapUnlimited;

    QDialogButtonBox *m_buttonBox;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_MEMTUNEDIALOG_H
