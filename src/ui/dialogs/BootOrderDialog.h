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

#ifndef QVIRT_UI_DIALOGS_BOOTORDERDIALOG_H
#define QVIRT_UI_DIALOGS_BOOTORDERDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QDialogButtonBox>

#include "../../libvirt/Domain.h"

namespace QVirt {

/**
 * @brief Boot order configuration dialog
 */
class BootOrderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BootOrderDialog(Domain *domain, QWidget *parent = nullptr);
    ~BootOrderDialog() override = default;

    void loadSettings();
    void saveSettings();

private slots:
    void onMoveUp();
    void onMoveDown();
    void onBootMenuToggled(bool checked);

private:
    void setupUI();

    Domain *m_domain;

    QListWidget *m_bootList;
    QPushButton *m_moveUpBtn;
    QPushButton *m_moveDownBtn;
    QCheckBox *m_bootMenuCheck;
    QSpinBox *m_bootMenuTimeoutSpin;
    QDialogButtonBox *m_buttonBox;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_BOOTORDERDIALOG_H
