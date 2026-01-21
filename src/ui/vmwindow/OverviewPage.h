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

#ifndef QVIRT_UI_VMWINDOW_OVERVIEWPAGE_H
#define QVIRT_UI_VMWINDOW_OVERVIEWPAGE_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QGroupBox>
#include <QProgressBar>

#include "../../libvirt/Domain.h"

namespace QVirt {

/**
 * @brief Overview page for VM details
 *
 * Displays VM information and performance statistics
 */
class OverviewPage : public QWidget
{
    Q_OBJECT

public:
    explicit OverviewPage(Domain *domain, QWidget *parent = nullptr);
    ~OverviewPage() override = default;

    void updateInfo();
    void updateStats();

private:
    void setupUI();

    // Domain reference
    Domain *m_domain;

    // VM Info section
    QGroupBox *m_infoGroup;
    QLabel *m_nameLabel;
    QLabel *m_nameValue;
    QLabel *m_uuidLabel;
    QLabel *m_uuidValue;
    QLabel *m_stateLabel;
    QLabel *m_stateValue;
    QLabel *m_titleLabel;
    QLabel *m_titleValue;
    QLabel *m_descLabel;
    QLabel *m_descValue;

    // Resources section
    QGroupBox *m_resourcesGroup;
    QLabel *m_cpuLabel;
    QLabel *m_cpuValue;
    QLabel *m_memoryLabel;
    QLabel *m_memoryValue;
    QLabel *m_maxMemoryLabel;
    QLabel *m_maxMemoryValue;

    // Performance section
    QGroupBox *m_performanceGroup;
    QLabel *m_cpuUsageLabel;
    QProgressBar *m_cpuUsageBar;
    QLabel *m_memoryUsageLabel;
    QProgressBar *m_memoryUsageBar;
};

} // namespace QVirt

#endif // QVIRT_UI_VMWINDOW_OVERVIEWPAGE_H
