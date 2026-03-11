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

#ifndef QVIRT_UI_DIALOGS_MIGRATEDIALOG_H
#define QVIRT_UI_DIALOGS_MIGRATEDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>

#include "../../libvirt/Connection.h"
#include "../../libvirt/Domain.h"

namespace QVirt {

/**
 * @brief VM Migration Dialog
 *
 * Dialog for migrating VMs between hosts
 */
class MigrateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MigrateDialog(Domain *domain, QWidget *parent = nullptr);
    ~MigrateDialog() override = default;

    struct MigrationConfig {
        QString destinationUri;
        bool live = false;
        bool persistent = true;
        bool undefineSource = false;
        bool compressed = false;
        int bandwidth = 0;  // 0 = unlimited
        int maxDowntime = 30;  // milliseconds
    };

    MigrationConfig config() const { return m_config; }

private slots:
    void validateAndAccept();
    void onAdvancedToggled(bool checked);
    void onLiveMigrationToggled(bool checked);
    void onConnectionTypeChanged();

private:
    void setupUI();
    void setupBasicOptions();
    void setupAdvancedOptions();

    Domain *m_domain;
    MigrationConfig m_config;

    // Basic options
    QComboBox *m_connectionTypeCombo;
    QLineEdit *m_remoteHostEdit;
    QSpinBox *m_remotePortSpin;
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;

    // Migration mode
    QButtonGroup *m_migrationModeGroup;
    QRadioButton *m_liveRadio;
    QRadioButton *m_offlineRadio;

    // Advanced options
    QCheckBox *m_persistentCheck;
    QCheckBox *m_undefineSourceCheck;
    QCheckBox *m_compressedCheck;
    QCheckBox *m_allowUnsafeCheck;
    QSpinBox *m_bandwidthSpin;
    QSpinBox *m_maxDowntimeSpin;

    // Buttons
    QPushButton *m_btnMigrate;
    QPushButton *m_btnCancel;

    // Labels
    QLabel *m_statusLabel;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_MIGRATEDIALOG_H
