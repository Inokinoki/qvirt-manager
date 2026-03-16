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

#include "ExportStatsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QFileDialog>
#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>

namespace QVirt {

ExportStatsDialog::ExportStatsDialog(QWidget *parent)
    : QDialog(parent)
    , m_format(ExportFormat::CSV)
{
    setupUI();
}

void ExportStatsDialog::setupUI()
{
    setWindowTitle(tr("Export Performance Statistics"));
    setModal(true);
    setMinimumWidth(450);

    auto layout = new QVBoxLayout(this);

    // VM name (read-only)
    auto formLayout = new QFormLayout();
    
    auto vmNameEdit = new QLineEdit(this);
    vmNameEdit->setReadOnly(true);
    formLayout->addRow(tr("Virtual Machine:"), vmNameEdit);

    // Output path
    auto pathLayout = new QHBoxLayout();
    auto pathEdit = new QLineEdit(this);
    pathEdit->setPlaceholderText(tr("Output file path..."));
    QPushButton *browseBtn = new QPushButton(tr("Browse..."), this);
    connect(browseBtn, &QPushButton::clicked, this, &ExportStatsDialog::onBrowseClicked);
    pathLayout->addWidget(pathEdit);
    pathLayout->addWidget(browseBtn);
    formLayout->addRow(tr("Output File:"), pathLayout);

    // Format selection
    auto formatCombo = new QComboBox(this);
    formatCombo->addItem(tr("CSV (Comma Separated)"), static_cast<int>(ExportFormat::CSV));
    formatCombo->addItem(tr("JSON"), static_cast<int>(ExportFormat::JSON));
    formatCombo->addItem(tr("PDF Document"), static_cast<int>(ExportFormat::PDF));
    connect(formatCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this, formatCombo]() {
        m_format = static_cast<ExportFormat>(formatCombo->currentData().toInt());
    });
    formLayout->addRow(tr("Format:"), formatCombo);

    layout->addLayout(formLayout);

    // Date range
    auto dateGroup = new QGroupBox(tr("Date Range"), this);
    auto dateLayout = new QVBoxLayout(dateGroup);

    auto startEdit = new QDateTimeEdit(this);
    startEdit->setCalendarPopup(true);
    startEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    startEdit->setDateTime(QDateTime::currentDateTime().addDays(-7));
    
    auto endEdit = new QDateTimeEdit(this);
    endEdit->setCalendarPopup(true);
    endEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    endEdit->setDateTime(QDateTime::currentDateTime());

    auto dateRangeLayout = new QHBoxLayout();
    dateRangeLayout->addWidget(new QLabel(tr("From:"), this));
    dateRangeLayout->addWidget(startEdit);
    dateRangeLayout->addWidget(new QLabel(tr("To:"), this));
    dateRangeLayout->addWidget(endEdit);
    dateLayout->addLayout(dateRangeLayout);

    layout->addWidget(dateGroup);

    // Metrics selection
    auto metricsGroup = new QGroupBox(tr("Metrics to Export"), this);
    auto metricsLayout = new QVBoxLayout(metricsGroup);

    auto cpuCheck = new QCheckBox(tr("CPU Usage"), this);
    cpuCheck->setChecked(true);
    auto memCheck = new QCheckBox(tr("Memory Usage"), this);
    memCheck->setChecked(true);
    auto diskCheck = new QCheckBox(tr("Disk I/O"), this);
    diskCheck->setChecked(true);
    auto netCheck = new QCheckBox(tr("Network I/O"), this);
    netCheck->setChecked(true);

    metricsLayout->addWidget(cpuCheck);
    metricsLayout->addWidget(memCheck);
    metricsLayout->addWidget(diskCheck);
    metricsLayout->addWidget(netCheck);

    layout->addWidget(metricsGroup);

    // Buttons
    auto buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    QPushButton *cancelBtn = new QPushButton(tr("Cancel"), this);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelBtn);

    QPushButton *exportBtn = new QPushButton(tr("Export"), this);
    exportBtn->setDefault(true);
    connect(exportBtn, &QPushButton::clicked, this, &ExportStatsDialog::onExportClicked);
    buttonLayout->addWidget(exportBtn);

    layout->addLayout(buttonLayout);
}

void ExportStatsDialog::setVMName(const QString &name)
{
    m_vmName = name;
}

QString ExportStatsDialog::outputPath() const
{
    return m_outputPath;
}

ExportStatsDialog::ExportFormat ExportStatsDialog::format() const
{
    return m_format;
}

QDateTime ExportStatsDialog::startDate() const
{
    return m_startDate;
}

QDateTime ExportStatsDialog::endDate() const
{
    return m_endDate;
}

void ExportStatsDialog::setOutputPath(const QString &path)
{
    m_outputPath = path;
}

void ExportStatsDialog::setFormat(ExportFormat fmt)
{
    m_format = fmt;
}

void ExportStatsDialog::setDateRange(const QDateTime &start, const QDateTime &end)
{
    m_startDate = start;
    m_endDate = end;
}

void ExportStatsDialog::setAvailableMetrics(const QStringList &metrics)
{
    Q_UNUSED(metrics);
}

void ExportStatsDialog::onBrowseClicked()
{
    QString filter;
    switch (m_format) {
        case ExportFormat::CSV:
            filter = tr("CSV Files (*.csv)");
            break;
        case ExportFormat::JSON:
            filter = tr("JSON Files (*.json)");
            break;
        case ExportFormat::PDF:
            filter = tr("PDF Files (*.pdf)");
            break;
    }

    QString path = QFileDialog::getSaveFileName(this, tr("Export Statistics"),
                                                 QString(), filter);
    if (!path.isEmpty()) {
        m_outputPath = path;
    }
}

void ExportStatsDialog::onExportClicked()
{
    emit exportRequested();
    accept();
}

} // namespace QVirt
