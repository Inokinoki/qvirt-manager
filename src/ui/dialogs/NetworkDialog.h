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

#ifndef QVIRT_UI_DIALOGS_NETWORKDIALOG_H
#define QVIRT_UI_DIALOGS_NETWORKDIALOG_H

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
#include <QCheckBox>
#include <QTextEdit>
#include <QFormLayout>
#include <QMenu>

#include "../../libvirt/Connection.h"
#include "../../libvirt/Network.h"

namespace QVirt {

/**
 * @brief Network Management Dialog
 *
 * Dialog for browsing and managing virtual networks
 */
class NetworkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetworkDialog(Connection *conn, QWidget *parent = nullptr);
    ~NetworkDialog() override = default;

private slots:
    void onNetworkSelected();
    void onNetworkStart();
    void onNetworkStop();
    void onNetworkDelete();
    void onNetworkRefresh();
    void onCreateNetwork();
    void onRefresh();
    void onContextMenuRequested(const QPoint &pos);

private:
    void setupUI();
    void setupNetworksTab();
    void updateNetworkList();
    void updateNetworkInfo();
    void createNetwork();

    Connection *m_connection;
    Network *m_currentNetwork;

    // Tab widget
    QTabWidget *m_tabWidget;

    // Networks tab
    QTableView *m_networkList;
    QLabel *m_networkInfoLabel;
    QPushButton *m_btnNetworkStart;
    QPushButton *m_btnNetworkStop;
    QPushButton *m_btnNetworkDelete;
    QPushButton *m_btnNetworkRefresh;
    QPushButton *m_btnCreateNetwork;
    QPushButton *m_btnRefreshNetworks;

    // Context menu
    QMenu *m_contextMenu;
    QAction *m_actionStart;
    QAction *m_actionStop;
    QAction *m_actionDelete;
    QAction *m_actionRefresh;
};

/**
 * @brief Create Virtual Network Dialog
 *
 * Dialog for creating new virtual networks
 */
class CreateNetworkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateNetworkDialog(Connection *conn, QWidget *parent = nullptr);
    ~CreateNetworkDialog() override = default;

    QString networkName() const { return m_nameEdit->text(); }
    QString networkMode() const;
    QString networkDevice() const { return m_deviceEdit->text(); }
    QString ipAddress() const { return m_ipEdit->text(); }
    QString netmask() const { return m_netmaskEdit->text(); }
    bool dhcpEnabled() const { return m_dhcpCheck->isChecked(); }
    QString dhcpStart() const { return m_dhcpStartEdit->text(); }
    QString dhcpEnd() const { return m_dhcpEndEdit->text(); }
    bool ipv6Enabled() const { return m_ipv6Check->isChecked(); }

private slots:
    void validate();
    void onModeChanged(int index);
    void onDHCPEnabledChanged(int state);

private:
    void setupUI();

    Connection *m_connection;

    QLineEdit *m_nameEdit;
    QComboBox *m_modeCombo;
    QLineEdit *m_deviceEdit;
    QLineEdit *m_ipEdit;
    QLineEdit *m_netmaskEdit;
    QCheckBox *m_dhcpCheck;
    QLineEdit *m_dhcpStartEdit;
    QLineEdit *m_dhcpEndEdit;
    QCheckBox *m_ipv6Check;
    QPushButton *m_btnOK;

    QLabel *m_deviceLabel;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_NETWORKDIALOG_H
