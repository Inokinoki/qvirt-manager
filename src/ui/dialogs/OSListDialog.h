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

#ifndef QVIRT_UI_DIALOGS_OSLISTDIALOG_H
#define QVIRT_UI_DIALOGS_OSLISTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QCheckBox>
#include <QLabel>

#include "../../core/OSDatabase.h"

namespace QVirt {

/**
 * @brief OS Selection Dialog for VM creation
 *
 * Provides searchable list of operating systems with filtering options.
 */
class OSListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OSListDialog(QWidget *parent = nullptr);
    ~OSListDialog() override;

    /**
     * @brief Get selected OS
     * @return Selected OS information
     */
    OSInfo getSelectedOS() const;

    /**
     * @brief Set pre-selected OS
     * @param osId OS identifier to select
     */
    void setSelectedOS(const QString &osId);

    /**
     * @brief Enable/disable EOL OS filtering
     * @param filter Filter out EOL OSes
     */
    void setFilterEOL(bool filter);

signals:
    /**
     * @brief Emitted when OS selection changes
     */
    void osSelected(const OSInfo &os);

private slots:
    void onSearchTextChanged(const QString &text);
    void onEOLFilterToggled(bool checked);
    void onOSItemActivated(QListWidgetItem *item);

private:
    void setupUI();
    void loadOSList();
    void updateOSList();
    OSInfo itemToOSInfo(QListWidgetItem *item) const;

    QLineEdit *m_searchEdit;
    QListWidget *m_osList;
    QCheckBox *m_eolFilterCheck;
    QLabel *m_osInfoLabel;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;

    OSDatabase *m_osDb;
    QList<OSInfo> m_allOSes;
    bool m_filterEOL;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_OSLISTDIALOG_H
