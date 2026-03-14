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

#ifndef QVIRT_UI_DIALOGS_BLKIOTUNEDIALOG_H
#define QVIRT_UI_DIALOGS_BLKIOTUNEDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>

#include "../../libvirt/Domain.h"

namespace QVirt {

/**
 * @brief Block I/O Tuning configuration dialog
 */
class BlkIOTuneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BlkIOTuneDialog(Domain *domain, QWidget *parent = nullptr);
    ~BlkIOTuneDialog() override = default;

    void loadSettings();
    void saveSettings();

private:
    void setupUI();

    Domain *m_domain;

    QComboBox *m_diskCombo;
    QSpinBox *m_weightSpin;
    QSpinBox *m_readBytesSpin;
    QSpinBox *m_writeBytesSpin;
    QSpinBox *m_readIopsSpin;
    QSpinBox *mWriteIopsSpin;

    QDialogButtonBox *m_buttonBox;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_BLKIOTUNEDIALOG_H
