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

#ifndef QVIRT_UI_DIALOGS_FIRMWAREDIALOG_H
#define QVIRT_UI_DIALOGS_FIRMWAREDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QDialogButtonBox>

#include "../../libvirt/Domain.h"

namespace QVirt {

/**
 * @brief Firmware configuration dialog
 *
 * Configure BIOS/UEFI firmware and Secure Boot settings
 */
class FirmwareDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FirmwareDialog(Domain *domain, QWidget *parent = nullptr);
    ~FirmwareDialog() override = default;

    void loadSettings();
    void saveSettings();

private slots:
    void onFirmwareTypeChanged(int index);
    void onSecureBootToggled(bool checked);
    void onBrowseNvram();

private:
    void setupUI();

    Domain *m_domain;

    QGroupBox *m_firmwareGroup;
    QComboBox *m_firmwareTypeCombo;
    QCheckBox *m_secureBootCheck;
    QComboBox *m_secureBootTemplateCombo;
    QLineEdit *m_nvramPathEdit;
    QPushButton *m_nvramBrowseBtn;
    QLabel *m_firmwareInfoLabel;

    QDialogButtonBox *m_buttonBox;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_FIRMWAREDIALOG_H
