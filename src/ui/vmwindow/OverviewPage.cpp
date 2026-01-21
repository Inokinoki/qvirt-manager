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

#include "OverviewPage.h"
#include "../../libvirt/EnumMapper.h"

namespace QVirt {

OverviewPage::OverviewPage(Domain *domain, QWidget *parent)
    : QWidget(parent)
    , m_domain(domain)
{
    setupUI();
    updateInfo();
}

void OverviewPage::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // VM Info Group
    m_infoGroup = new QGroupBox("Virtual Machine Information", this);
    auto *infoLayout = new QGridLayout(m_infoGroup);
    infoLayout->setSpacing(5);
    infoLayout->setColumnStretch(1, 1);

    int row = 0;

    // Name
    m_nameLabel = new QLabel("Name:", m_infoGroup);
    m_nameValue = new QLabel(m_infoGroup);
    infoLayout->addWidget(m_nameLabel, row, 0);
    infoLayout->addWidget(m_nameValue, row, 1);
    row++;

    // UUID
    m_uuidLabel = new QLabel("UUID:", m_infoGroup);
    m_uuidValue = new QLabel(m_infoGroup);
    m_uuidValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
    infoLayout->addWidget(m_uuidLabel, row, 0);
    infoLayout->addWidget(m_uuidValue, row, 1);
    row++;

    // State
    m_stateLabel = new QLabel("State:", m_infoGroup);
    m_stateValue = new QLabel(m_infoGroup);
    infoLayout->addWidget(m_stateLabel, row, 0);
    infoLayout->addWidget(m_stateValue, row, 1);
    row++;

    // Title
    m_titleLabel = new QLabel("Title:", m_infoGroup);
    m_titleValue = new QLabel(m_infoGroup);
    m_titleValue->setWordWrap(true);
    infoLayout->addWidget(m_titleLabel, row, 0);
    infoLayout->addWidget(m_titleValue, row, 1);
    row++;

    // Description
    m_descLabel = new QLabel("Description:", m_infoGroup);
    m_descValue = new QLabel(m_infoGroup);
    m_descValue->setWordWrap(true);
    infoLayout->addWidget(m_descLabel, row, 0);
    infoLayout->addWidget(m_descValue, row, 1);
    infoLayout->setRowStretch(row, 1);

    mainLayout->addWidget(m_infoGroup);

    // Resources Group
    m_resourcesGroup = new QGroupBox("Resources", this);
    auto *resourcesLayout = new QGridLayout(m_resourcesGroup);
    resourcesLayout->setSpacing(5);
    resourcesLayout->setColumnStretch(1, 1);

    row = 0;

    // CPUs
    m_cpuLabel = new QLabel("CPUs:", m_resourcesGroup);
    m_cpuValue = new QLabel(m_resourcesGroup);
    resourcesLayout->addWidget(m_cpuLabel, row, 0);
    resourcesLayout->addWidget(m_cpuValue, row, 1);
    row++;

    // Memory
    m_memoryLabel = new QLabel("Memory:", m_resourcesGroup);
    m_memoryValue = new QLabel(m_resourcesGroup);
    resourcesLayout->addWidget(m_memoryLabel, row, 0);
    resourcesLayout->addWidget(m_memoryValue, row, 1);
    row++;

    // Max Memory
    m_maxMemoryLabel = new QLabel("Max Memory:", m_resourcesGroup);
    m_maxMemoryValue = new QLabel(m_resourcesGroup);
    resourcesLayout->addWidget(m_maxMemoryLabel, row, 0);
    resourcesLayout->addWidget(m_maxMemoryValue, row, 1);

    mainLayout->addWidget(m_resourcesGroup);

    // Performance Group
    m_performanceGroup = new QGroupBox("Performance", this);
    auto *perfLayout = new QGridLayout(m_performanceGroup);
    perfLayout->setSpacing(5);

    row = 0;

    // CPU Usage
    m_cpuUsageLabel = new QLabel("CPU Usage:", m_performanceGroup);
    m_cpuUsageBar = new QProgressBar(m_performanceGroup);
    m_cpuUsageBar->setRange(0, 100);
    m_cpuUsageBar->setTextVisible(true);
    perfLayout->addWidget(m_cpuUsageLabel, row, 0);
    perfLayout->addWidget(m_cpuUsageBar, row, 1);
    row++;

    // Memory Usage
    m_memoryUsageLabel = new QLabel("Memory Usage:", m_performanceGroup);
    m_memoryUsageBar = new QProgressBar(m_performanceGroup);
    m_memoryUsageBar->setRange(0, 100);
    m_memoryUsageBar->setTextVisible(true);
    perfLayout->addWidget(m_memoryUsageLabel, row, 0);
    perfLayout->addWidget(m_memoryUsageBar, row, 1);

    mainLayout->addWidget(m_performanceGroup);

    // Add stretch to push everything to the top
    mainLayout->addStretch();
}

void OverviewPage::updateInfo()
{
    // Update VM info
    m_nameValue->setText(m_domain->name());
    m_uuidValue->setText(m_domain->uuid());
    m_stateValue->setText(EnumMapper::stateToString(m_domain->state()));

    QString title = m_domain->title();
    m_titleValue->setText(title.isEmpty() ? "-" : title);
    m_titleValue->setVisible(!title.isEmpty());
    m_titleLabel->setVisible(!title.isEmpty());

    QString desc = m_domain->description();
    m_descValue->setText(desc.isEmpty() ? "-" : desc);
    m_descValue->setVisible(!desc.isEmpty());
    m_descLabel->setVisible(!desc.isEmpty());

    // Update resources
    m_cpuValue->setText(QString::number(m_domain->vcpuCount()));

    quint64 memKB = m_domain->currentMemory();
    m_memoryValue->setText(QString("%1 MB").arg(memKB / 1024));

    quint64 maxMemKB = m_domain->maxMemory();
    m_maxMemoryValue->setText(QString("%1 MB").arg(maxMemKB / 1024));

    updateStats();
}

void OverviewPage::updateStats()
{
    // Update CPU usage
    float cpuPercent = m_domain->cpuUsage();
    m_cpuUsageBar->setValue(static_cast<int>(cpuPercent));
    m_cpuUsageBar->setFormat(QString("%1%").arg(cpuPercent, 0, 'f', 1));

    // Calculate memory usage percentage
    quint64 currentMem = m_domain->currentMemory();
    quint64 maxMem = m_domain->maxMemory();
    float memPercent = maxMem > 0 ? (static_cast<float>(currentMem) / maxMem) * 100.0f : 0.0f;

    m_memoryUsageBar->setValue(static_cast<int>(memPercent));
    m_memoryUsageBar->setFormat(QString("%1 MB (%2%)")
        .arg(currentMem / 1024)
        .arg(memPercent, 0, 'f', 1));
}

} // namespace QVirt
