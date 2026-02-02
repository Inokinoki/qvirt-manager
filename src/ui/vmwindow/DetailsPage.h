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

#ifndef QVIRT_UI_VMWINDOW_DETAILSPAGE_H
#define QVIRT_UI_VMWINDOW_DETAILSPAGE_H

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>
#include <QTextEdit>
#include <QGroupBox>

#include "../../libvirt/Domain.h"

namespace QVirt {

/**
 * @brief Details page showing VM hardware
 *
 * Displays a tree of all VM devices with ability to add/remove
 */
class DetailsPage : public QWidget
{
    Q_OBJECT

public:
    explicit DetailsPage(Domain *domain, QWidget *parent = nullptr);
    ~DetailsPage() override = default;

    void refresh();

private slots:
    void onDeviceSelected(QTreeWidgetItem *item, int column);
    void onAddHardware();
    void onRemoveHardware();

private:
    void setupUI();
    void populateDeviceTree();
    QTreeWidgetItem* addDeviceCategory(const QString &name, const QString &icon);
    QTreeWidgetItem* addDevice(QTreeWidgetItem *parent, const QString &name, const QString &details);
    QString getDeviceXML(const QString &categoryName);

    // Domain reference
    Domain *m_domain;

    // UI components
    QSplitter *m_splitter;
    QTreeWidget *m_deviceTree;
    QTextEdit *m_deviceDetails;

    // Buttons
    QPushButton *m_btnAddHardware;
    QPushButton *m_btnRemoveHardware;
    QPushButton *m_btnRefresh;

    // Currently selected device info
    QString m_currentDeviceXML;
};

} // namespace QVirt

#endif // QVIRT_UI_VMWINDOW_DETAILSPAGE_H
