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

#ifndef QVIRT_UI_WIZARDS_CREATENETWORKDIALOG_H
#define QVIRT_UI_WIZARDS_CREATENETWORKDIALOG_H

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
 * @brief Page 1: Network Name
 */
class NetworkNamePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit NetworkNamePage(QWidget *parent = nullptr);

    bool validatePage() override;

private:
    QLineEdit *m_nameEdit;
};

/**
 * @brief Page 2: Network Mode
 */
class NetworkModePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit NetworkModePage(QWidget *parent = nullptr);

private slots:
    void updateDescription();

private:
    QComboBox *m_modeCombo;
};

/**
 * @brief Page 3: IPv4 Configuration
 */
class NetworkIPv4Page : public QWizardPage
{
    Q_OBJECT

public:
    explicit NetworkIPv4Page(QWidget *parent = nullptr);

private:
    QCheckBox *m_ipv4EnabledCheck;
    QLineEdit *m_networkEdit;
    QCheckBox *m_dhcp4Check;
};

/**
 * @brief Page 4: IPv6 Configuration
 */
class NetworkIPv6Page : public QWizardPage
{
    Q_OBJECT

public:
    explicit NetworkIPv6Page(QWidget *parent = nullptr);

private:
    QCheckBox *m_ipv6EnabledCheck;
    QLineEdit *m_prefixEdit;
    QCheckBox *m_dhcp6Check;
};

/**
 * @brief Page 5: Forwarding Configuration
 */
class NetworkForwardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit NetworkForwardPage(QWidget *parent = nullptr);

private:
    QComboBox *m_forwardModeCombo;
    QLineEdit *m_devEdit;
};

/**
 * @brief Page 6: Finish
 */
class NetworkFinishPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit NetworkFinishPage(QWidget *parent = nullptr);
};

/**
 * @brief Wizard for creating a new virtual network
 */
class CreateNetworkWizard : public QWizard
{
    Q_OBJECT

public:
    explicit CreateNetworkWizard(Connection *conn, QWidget *parent = nullptr);
    ~CreateNetworkWizard() override = default;

    QString getXML() const;

private:
    void setupPages();

    Connection *m_connection;
    QString m_networkXML;
};

} // namespace QVirt

#endif // QVIRT_UI_WIZARDS_CREATENETWORKDIALOG_H
