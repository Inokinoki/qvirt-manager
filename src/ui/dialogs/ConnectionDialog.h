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

#ifndef QVIRT_UI_DIALOGS_CONNECTIONDIALOG_H
#define QVIRT_UI_DIALOGS_CONNECTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

namespace QVirt {

/**
 * @brief Dialog for adding a new libvirt connection
 *
 * Placeholder for Phase 2 implementation
 */
class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = nullptr);
    ~ConnectionDialog() override;

    QString uri() const { return m_uri; }
    bool autoconnect() const { return m_autoconnect; }

private slots:
    void validateAndAccept();

private:
    void setupUI();

    QLineEdit *m_uriEdit;
    QComboBox *m_typeCombo;
    QCheckBox *m_autoconnectCheck;
    QPushButton *m_btnOK;
    QPushButton *m_btnCancel;

    QString m_uri;
    bool m_autoconnect;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_CONNECTIONDIALOG_H
