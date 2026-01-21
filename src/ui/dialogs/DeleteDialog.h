/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 The QVirt-Manager Developers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef QVIRT_UI_DIALOGS_DELETEDIALOG_H
#define QVIRT_UI_DIALOGS_DELETEDDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

#include "../../libvirt/Domain.h"

namespace QVirt {

/**
 * @brief Delete VM Dialog
 *
 * Dialog for confirming VM deletion with optional storage removal
 */
class DeleteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteDialog(Domain *domain, QWidget *parent = nullptr);
    ~DeleteDialog() override = default;

    bool deleteStorage() const { return m_deleteStorage; }
    bool forceDelete() const { return m_forceDelete; }

private slots:
    void onDeleteClicked();
    void onCancelClicked();

private:
    void setupUI();

    Domain *m_domain;

    // UI components
    QLabel *m_messageLabel;
    QLabel *m_nameLabel;
    QLabel *m_stateLabel;
    QLabel *m_pathLabel;

    QCheckBox *m_deleteStorageCheck;
    QCheckBox *m_forceDeleteCheck;

    QPushButton *m_btnDelete;
    QPushButton *m_btnCancel;

    bool m_deleteStorage = false;
    bool m_forceDelete = false;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_DELETEDDIALOG_H
