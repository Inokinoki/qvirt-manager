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

#ifndef QVIRT_UI_DIALOGS_CLONEDIALOG_H
#define QVIRT_UI_DIALOGS_CLONEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

#include "../../libvirt/Domain.h"

namespace QVirt {

/**
 * @brief Clone VM Dialog
 *
 * Dialog for cloning an existing VM
 */
class CloneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CloneDialog(Domain *sourceDomain, QWidget *parent = nullptr);
    ~CloneDialog() override = default;

    QString cloneName() const { return m_nameEdit->text(); }
    bool cloneStorage() const { return m_cloneStorageCheck->isChecked(); }
    QString storagePath() const { return m_storagePathEdit->text(); }
    bool generateMACs() const { return m_generateMACsCheck->isChecked(); }

private slots:
    void validate();
    void onStorageBrowse();
    void onCloneStorageChanged(int state);

private:
    void setupUI();

    Domain *m_sourceDomain;

    QLineEdit *m_nameEdit;
    QCheckBox *m_cloneStorageCheck;
    QLineEdit *m_storagePathEdit;
    QPushButton *m_btnBrowseStorage;
    QCheckBox *m_generateMACsCheck;
    QPushButton *m_btnOK;

    // Source VM info
    QLabel *m_sourceNameLabel;
    QLabel *m_sourceUUIDLabel;
    QLabel *m_sourceStateLabel;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_CLONEDIALOG_H
