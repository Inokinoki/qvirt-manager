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

#ifndef QVIRT_UI_DEVICE_GUESTAGENTDETAILS_H
#define QVIRT_UI_DEVICE_GUESTAGENTDETAILS_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

#include "../../libvirt/Domain.h"

namespace QVirt {

/**
 * @brief Guest Agent status and control widget
 *
 * Displays QEMU guest agent status and provides guest agent commands
 */
class GuestAgentDetails : public QWidget
{
    Q_OBJECT

public:
    explicit GuestAgentDetails(Domain *domain, QWidget *parent = nullptr);
    ~GuestAgentDetails() override = default;

    void refresh();

public slots:
    void onPingClicked();
    void onGetUserInfoClicked();
    void onGetNetworkInterfacesClicked();
    void onGetFSInfoClicked();
    void onShutdownClicked();

private:
    void setupUI();
    void updateStatus();
    void showResult(const QString &title, const QString &result);

    Domain *m_domain;

    QLabel *m_statusLabel;
    QLabel *m_versionLabel;
    QLabel *m_hostnameLabel;
    QLabel *m_osLabel;
    QLabel *m_ipAddressesLabel;
    QPushButton *m_pingBtn;
    QPushButton *m_userInfoBtn;
    QPushButton *m_networkBtn;
    QPushButton *m_fsInfoBtn;
    QPushButton *m_shutdownBtn;

    QGroupBox *m_statusGroup;
    QGroupBox *m_infoGroup;
    QGroupBox *m_commandsGroup;
};

} // namespace QVirt

#endif // QVIRT_UI_DEVICE_GUESTAGENTDETAILS_H
