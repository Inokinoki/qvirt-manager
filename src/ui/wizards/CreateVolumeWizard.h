/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 Inoki <veyx.shaw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version).
 */

#ifndef QVIRT_UI_WIZARDS_CREATEVOLUMEDIALOG_H
#define QVIRT_UI_WIZARDS_CREATEVOLUMEDIALOG_H

#include <QWizard>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "../../libvirt/Connection.h"

namespace QVirt {

/**
 * @brief Page 1: Volume Name
 */
class VolumeNamePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit VolumeNamePage(QWidget *parent = nullptr);

    bool validatePage() override;

private:
    QLineEdit *m_nameEdit;
};

/**
 * @brief Page 2: Volume Size and Format
 */
class VolumeConfigPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit VolumeConfigPage(QWidget *parent = nullptr);

    qint64 getSizeBytes() const { return m_sizeSpin->value() * 1024 * 1024 * 1024; }
    QString getFormat() const { return m_formatCombo->currentData().toString(); }
    int getAllocation() const { return m_allocationSpin->value(); }

private slots:
    void onFormatChanged(int index);

private:
    QSpinBox *m_sizeSpin;
    QComboBox *m_formatCombo;
    QSpinBox *m_allocationSpin;
    QLabel *m_sizeLabel;
};

/**
 * @brief Page 3: Volume Options
 */
class VolumeOptionsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit VolumeOptionsPage(QWidget *parent = nullptr);

    bool getBackingStore() const { return m_backingCheck->isChecked(); }
    bool getReadOnly() const { return m_readOnlyCheck->isChecked(); }
    bool getSparse() const { return m_sparseCheck->isChecked(); }

private:
    QCheckBox *m_backingCheck;
    QCheckBox *m_readOnlyCheck;
    QCheckBox *m_sparseCheck;
};

/**
 * @brief Page 4: Finish
 */
class VolumeFinishPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit VolumeFinishPage(QWidget *parent = nullptr);

    void initializePage() override;
};

/**
 * @brief Wizard for creating a new storage volume
 */
class CreateVolumeWizard : public QWizard
{
    Q_OBJECT

public:
    explicit CreateVolumeWizard(Connection *conn, StoragePool *pool, QWidget *parent = nullptr);
    ~CreateVolumeWizard() override = default;

    QString getXML() const;

private:
    void setupPages();

    Connection *m_connection;
    StoragePool *m_pool;
    QString m_volumeXML;
};

} // namespace QVirt

#endif // QVIRT_UI_WIZARDS_CREATEVOLUMEDIALOG_H
