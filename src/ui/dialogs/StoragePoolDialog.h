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

#ifndef QVIRT_UI_DIALOGS_STORAGEPOOLDIALOG_H
#define QVIRT_UI_DIALOGS_STORAGEPOOLDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QMenu>
#include <QAction>

#include "../../libvirt/Connection.h"
#include "../../libvirt/StoragePool.h"
#include "../../libvirt/StorageVolume.h"

namespace QVirt {

/**
 * @brief Storage Pool Management Dialog
 *
 * Dialog for browsing and managing storage pools and volumes
 */
class StoragePoolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StoragePoolDialog(Connection *conn, QWidget *parent = nullptr);
    ~StoragePoolDialog() override = default;

private slots:
    void onPoolSelected();
    void onVolumeSelected();
    void onPoolStart();
    void onPoolStop();
    void onPoolDelete();
    void onPoolRefresh();
    void onVolumeCreate();
    void onVolumeDelete();
    void onVolumeDownload();
    void onVolumeUpload();
    void onVolumeClone();
    void onVolumeWipe();
    void onRefresh();
    void onContextMenuRequested(const QPoint &pos);

private:
    void setupUI();
    void setupPoolsTab();
    void setupVolumesTab();
    void updatePoolList();
    void updateVolumeList();
    void updatePoolInfo();
    void updateVolumeInfo();
    void createVolume();

    Connection *m_connection;
    StoragePool *m_currentPool;
    StorageVolume *m_currentVolume;

    // Tab widget
    QTabWidget *m_tabWidget;

    // Pools tab
    QTableView *m_poolList;
    QLabel *m_poolInfoLabel;
    QPushButton *m_btnPoolStart;
    QPushButton *m_btnPoolStop;
    QPushButton *m_btnPoolDelete;
    QPushButton *m_btnPoolRefresh;
    QPushButton *m_btnRefreshPools;

    // Volumes tab
    QTableView *m_volumeList;
    QLabel *m_volumeInfoLabel;
    QPushButton *m_btnVolumeCreate;
    QPushButton *m_btnVolumeDelete;
    QPushButton *m_btnVolumeDownload;
    QPushButton *m_btnVolumeUpload;
    QPushButton *m_btnVolumeClone;
    QPushButton *m_btnVolumeWipe;
    QPushButton *m_btnRefreshVolumes;

    // Context menu
    QMenu *m_contextMenu;
    QAction *m_actionStart;
    QAction *m_actionStop;
    QAction *m_actionDelete;
    QAction *m_actionRefresh;
};

/**
 * @brief Create Volume Dialog
 *
 * Dialog for creating new storage volumes
 */
class CreateVolumeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateVolumeDialog(StoragePool *pool, QWidget *parent = nullptr);
    ~CreateVolumeDialog() override = default;

    QString volumeName() const { return m_nameEdit->text(); }
    qint64 volumeSize() const { return m_sizeSpin->value() * 1024 * 1024; } // MB to bytes
    QString volumeFormat() const { return m_formatCombo->currentText(); }
    qint64 volumeAllocation() const { return m_allocationSpin->value() * 1024 * 1024; }

private slots:
    void validate();
    void onFormatChanged(int index);

private:
    void setupUI();

    StoragePool *m_pool;

    QLineEdit *m_nameEdit;
    QSpinBox *m_sizeSpin;
    QSpinBox *m_allocationSpin;
    QComboBox *m_formatCombo;
    QLabel *m_maxSizeLabel;
    QPushButton *m_btnOK;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_STORAGEPOOLDIALOG_H
