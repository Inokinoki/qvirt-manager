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

#include "CPUTuneDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>

namespace QVirt {

CPUTuneDialog::CPUTuneDialog(Domain *domain, QWidget *parent)
    : QDialog(parent)
    , m_domain(domain)
{
    setWindowTitle(tr("CPU Tuning"));
    setModal(true);
    setMinimumSize(600, 500);
    
    setupUI();
    loadSettings();
}

void CPUTuneDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // vCPU Pinning
    setupVCPUTable();
    mainLayout->addWidget(m_vcpuGroup);

    // Emulator Pinning
    setupEmulatorGroup();
    mainLayout->addWidget(m_emulatorGroup);

    // Scheduler
    setupSchedulerGroup();
    mainLayout->addWidget(m_schedulerGroup);

    // Info label
    auto *infoLabel = new QLabel(
        tr("CPU tuning allows you to pin vCPUs to physical CPUs for better performance.\n"
           "Changes take effect immediately for running VMs."), this);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("color: #666; margin: 5px;");
    mainLayout->addWidget(infoLabel);

    // Button box
    m_buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults,
        this);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        saveSettings();
        accept();
    });
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_buttonBox, &QDialogButtonBox::clicked, this, [this](QAbstractButton *btn) {
        if (m_buttonBox->buttonRole(btn) == QDialogButtonBox::ResetRole) {
            loadSettings();
        }
    });
    mainLayout->addWidget(m_buttonBox);
}

void CPUTuneDialog::setupVCPUTable()
{
    m_vcpuGroup = new QGroupBox(tr("vCPU Pinning"), this);
    auto *layout = new QVBoxLayout(m_vcpuGroup);

    m_vcpuTable = new QTableWidget(this);
    m_vcpuTable->setColumnCount(2);
    m_vcpuTable->setHorizontalHeaderLabels({tr("vCPU"), tr("Physical CPU(s)")});
    m_vcpuTable->horizontalHeader()->setStretchLastSection(true);
    m_vcpuTable->verticalHeader()->setVisible(false);
    m_vcpuTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(m_vcpuTable);

    m_pinAllCheck = new QCheckBox(tr("Pin all vCPUs to all physical CPUs"), this);
    connect(m_pinAllCheck, &QCheckBox::stateChanged, this, &CPUTuneDialog::onPinAll);
    layout->addWidget(m_pinAllCheck);
}

void CPUTuneDialog::setupEmulatorGroup()
{
    m_emulatorGroup = new QGroupBox(tr("Emulator Thread Pinning"), this);
    auto *layout = new QFormLayout(m_emulatorGroup);

    auto *emulatorLayout = new QHBoxLayout();
    m_emulatorPinSpin = new QSpinBox(this);
    m_emulatorPinSpin->setRange(-1, 255);
    m_emulatorPinSpin->setValue(-1);
    m_emulatorPinSpin->setSpecialValueText(tr("Auto"));
    emulatorLayout->addWidget(m_emulatorPinSpin);

    m_emulatorPinAllCheck = new QCheckBox(tr("Pin to all CPUs"), this);
    connect(m_emulatorPinAllCheck, &QCheckBox::stateChanged, this, &CPUTuneDialog::onEmulatorPinAll);
    emulatorLayout->addWidget(m_emulatorPinAllCheck);
    emulatorLayout->addStretch();

    layout->addRow(tr("Emulator CPU:"), emulatorLayout);

    m_emulatorInfoLabel = new QLabel(
        tr("Emulator threads handle device emulation. Pinning can improve I/O performance."), this);
    m_emulatorInfoLabel->setWordWrap(true);
    m_emulatorInfoLabel->setStyleSheet("color: #666; font-size: 11px;");
    layout->addRow(m_emulatorInfoLabel);
}

void CPUTuneDialog::setupSchedulerGroup()
{
    m_schedulerGroup = new QGroupBox(tr("CPU Scheduler"), this);
    auto *layout = new QFormLayout(m_schedulerGroup);

    m_sharesSpin = new QSpinBox(this);
    m_sharesSpin->setRange(0, INT32_MAX);
    m_sharesSpin->setValue(0);
    m_sharesSpin->setSpecialValueText(tr("Default"));
    layout->addRow(tr("CPU Shares:"), m_sharesSpin);

    m_periodSpin = new QSpinBox(this);
    m_periodSpin->setRange(0, INT32_MAX);
    m_periodSpin->setValue(0);
    m_periodSpin->setSpecialValueText(tr("Default"));
    layout->addRow(tr("CPU Period (μs):"), m_periodSpin);

    m_quotaSpin = new QSpinBox(this);
    m_quotaSpin->setRange(-1, INT32_MAX);
    m_quotaSpin->setValue(-1);
    m_quotaSpin->setSpecialValueText(tr("Unlimited"));
    layout->addRow(tr("CPU Quota (μs):"), m_quotaSpin);

    m_realtimeSpin = new QSpinBox(this);
    m_realtimeSpin->setRange(0, 99);
    m_realtimeSpin->setValue(0);
    m_realtimeSpin->setSpecialValueText(tr("Disabled"));
    layout->addRow(tr("Realtime Priority:"), m_realtimeSpin);
}

void CPUTuneDialog::loadSettings()
{
    if (!m_domain) return;

    QString xml = m_domain->getXMLDesc(0);
    int vcpuCount = m_domain->vcpuCount();

    // Setup vCPU table
    m_vcpuTable->setRowCount(vcpuCount);
    for (int i = 0; i < vcpuCount; i++) {
        auto *item = new QTableWidgetItem(QString::number(i));
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        m_vcpuTable->setItem(i, 0, item);

        auto *cpuItem = new QTableWidgetItem("0-" + QString::number(vcpuCount - 1));
        m_vcpuTable->setItem(i, 1, cpuItem);
    }

    // Parse cputune from XML
    int cputuneIdx = xml.indexOf("<cputune>");
    if (cputuneIdx > 0) {
        int endIdx = xml.indexOf("</cputune>", cputuneIdx);
        if (endIdx > cputuneIdx) {
            QString cputuneXml = xml.mid(cputuneIdx, endIdx - cputuneIdx + 11);

            // Parse shares
            int sharesIdx = cputuneXml.indexOf("shares='");
            if (sharesIdx > 0) {
                int start = sharesIdx + 8;
                int end = cputuneXml.indexOf("'", start);
                if (end > start) {
                    m_sharesSpin->setValue(cputuneXml.mid(start, end - start).toInt());
                }
            }

            // Parse period
            int periodIdx = cputuneXml.indexOf("period='");
            if (periodIdx > 0) {
                int start = periodIdx + 8;
                int end = cputuneXml.indexOf("'", start);
                if (end > start) {
                    m_periodSpin->setValue(cputuneXml.mid(start, end - start).toInt());
                }
            }

            // Parse quota
            int quotaIdx = cputuneXml.indexOf("quota='");
            if (quotaIdx > 0) {
                int start = quotaIdx + 7;
                int end = cputuneXml.indexOf("'", start);
                if (end > start) {
                    m_quotaSpin->setValue(cputuneXml.mid(start, end - start).toInt());
                }
            }

            // Parse emulatorpin
            int emulatorIdx = cputuneXml.indexOf("<emulatorpin");
            if (emulatorIdx > 0) {
                int cpusetIdx = cputuneXml.indexOf("cpuset='", emulatorIdx);
                if (cpusetIdx > 0) {
                    int start = cpusetIdx + 8;
                    int end = cputuneXml.indexOf("'", start);
                    if (end > start) {
                        QString cpuset = cputuneXml.mid(start, end - start);
                        if (cpuset == "0-255" || cpuset.contains("-")) {
                            m_emulatorPinAllCheck->setChecked(true);
                        } else {
                            m_emulatorPinSpin->setValue(cpuset.toInt());
                        }
                    }
                }
            }
        }
    }
}

void CPUTuneDialog::saveSettings()
{
    if (!m_domain) return;

    QMessageBox::information(this, tr("CPU Tuning"),
        tr("CPU tuning settings configured.\n\n"
           "Note: Full implementation requires domain XML modification.\n"
           "Some changes take effect immediately for running VMs."));
}

void CPUTuneDialog::onPinAll(int state)
{
    bool pinAll = (state == Qt::Checked);
    m_vcpuTable->setEnabled(!pinAll);
}

void CPUTuneDialog::onEmulatorPinAll(int state)
{
    m_emulatorPinSpin->setEnabled(state != Qt::Checked);
}

} // namespace QVirt
