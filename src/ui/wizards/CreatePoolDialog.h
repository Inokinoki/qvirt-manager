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

#ifndef QVIRT_UI_WIZARDS_CREATEPOOLDIALOG_H
#define QVIRT_UI_WIZARDS_CREATEPOOLDIALOG_H

#include <QWizard>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "../../libvirt/Connection.h"

namespace QVirt {

/**
 * @brief Page 1: Pool Name
 */
class PoolNamePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit PoolNamePage(QWidget *parent = nullptr);

    bool validatePage() override;

private:
    QLineEdit *m_nameEdit;
};

/**
 * @brief Page 2: Pool Type
 */
class PoolTypePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit PoolTypePage(QWidget *parent = nullptr);

private slots:
    void updateDescription();

private:
    QComboBox *m_typeCombo;
};

/**
 * @brief Page 3: Target Path
 */
class PoolTargetPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit PoolTargetPage(QWidget *parent = nullptr);

    void initializePage() override;
    bool validatePage() override;

private:
    QLineEdit *m_pathEdit;
    QLineEdit *m_sourceEdit;
    QLineEdit *m_hostEdit;
    QLineEdit *m_deviceEdit;
};

/**
 * @brief Page 4: Build Options
 */
class PoolBuildPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit PoolBuildPage(QWidget *parent = nullptr);

    QCheckBox *buildPoolCheck() const { return m_buildPoolCheck; }
    QCheckBox *startPoolCheck() const { return m_startPoolCheck; }

private:
    QCheckBox *m_buildPoolCheck;
    QCheckBox *m_startPoolCheck;
};

/**
 * @brief Wizard for creating a new storage pool
 */
class CreatePoolDialog : public QWizard
{
    Q_OBJECT

public:
    explicit CreatePoolDialog(Connection *conn, QWidget *parent = nullptr);
    ~CreatePoolDialog() override = default;

    QString getXML() const;

private:
    void setupPages();
    void buildXML();

    Connection *m_connection;
    QString m_poolXML;
};

} // namespace QVirt

#endif // QVIRT_UI_WIZARDS_CREATEPOOLDIALOG_H
