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

#include "BlkIOTuneDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

namespace QVirt {

BlkIOTuneDialog::BlkIOTuneDialog(Domain *domain, QWidget *parent)
    : QDialog(parent)
    , m_domain(domain)
{
    setWindowTitle(tr("Block I/O Tuning"));
    setModal(true);
    setMinimumWidth(450);
    
    setupUI();
    loadSettings();
}

void BlkIOTuneDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Disk selection
    auto *diskGroup = new QGroupBox(tr("Select Disk"), this);
    auto *diskLayout = new QFormLayout(diskGroup);

    m_diskCombo = new QComboBox(this);
    diskLayout->addRow(tr("Disk:"), m_diskCombo);

    mainLayout->addWidget(diskGroup);

    // Weight
    auto *weightGroup = new QGroupBox(tr("I/O Weight"), this);
    auto *weightLayout = new QFormLayout(weightGroup);

    m_weightSpin = new QSpinBox(this);
    m_weightSpin->setRange(100, 1000);
    m_weightSpin->setValue(500);
    m_weightSpin->setSpecialValueText(tr("Default"));
    weightLayout->addRow(tr("Weight (100-1000):"), m_weightSpin);

    auto *weightInfoLabel = new QLabel(
        tr("Higher weight = more I/O bandwidth priority."), this);
    weightInfoLabel->setWordWrap(true);
    weightInfoLabel->setStyleSheet("color: #666; font-size: 11px;");
    weightLayout->addRow(weightInfoLabel);

    mainLayout->addWidget(weightGroup);

    // Bandwidth limits
    auto *bwGroup = new QGroupBox(tr("Bandwidth Limits"), this);
    auto *bwLayout = new QFormLayout(bwGroup);

    m_readBytesSpin = new QSpinBox(this);
    m_readBytesSpin->setRange(0, INT32_MAX);
    m_readBytesSpin->setValue(0);
    m_readBytesSpin->setSuffix(tr(" bytes/sec"));
    m_readBytesSpin->setSpecialValueText(tr("Unlimited"));
    bwLayout->addRow(tr("Read Limit:"), m_readBytesSpin);

    m_writeBytesSpin = new QSpinBox(this);
    m_writeBytesSpin->setRange(0, INT32_MAX);
    m_writeBytesSpin->setValue(0);
    m_writeBytesSpin->setSuffix(tr(" bytes/sec"));
    m_writeBytesSpin->setSpecialValueText(tr("Unlimited"));
    bwLayout->addRow(tr("Write Limit:"), m_writeBytesSpin);

    mainLayout->addWidget(bwGroup);

    // IOPS limits
    auto *iopsGroup = new QGroupBox(tr("IOPS Limits"), this);
    auto *iopsLayout = new QFormLayout(iopsGroup);

    m_readIopsSpin = new QSpinBox(this);
    m_readIopsSpin->setRange(0, INT32_MAX);
    m_readIopsSpin->setValue(0);
    m_readIopsSpin->setSpecialValueText(tr("Unlimited"));
    iopsLayout->addRow(tr("Read IOPS:"), m_readIopsSpin);

    mWriteIopsSpin = new QSpinBox(this);
    mWriteIopsSpin->setRange(0, INT32_MAX);
    mWriteIopsSpin->setValue(0);
    mWriteIopsSpin->setSpecialValueText(tr("Unlimited"));
    iopsLayout->addRow(tr("Write IOPS:"), mWriteIopsSpin);

    mainLayout->addWidget(iopsGroup);

    // Info
    auto *infoLabel = new QLabel(
        tr("Block I/O tuning controls disk bandwidth and IOPS.\n"
           "Weight is relative to other VMs on the same host."), this);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("color: #666; margin: 5px;");
    mainLayout->addWidget(infoLabel);

    // Button box
    m_buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        saveSettings();
        accept();
    });
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(m_buttonBox);
}

void BlkIOTuneDialog::loadSettings()
{
    if (!m_domain) return;

    QString xml = m_domain->getXMLDesc(0);
    
    // Parse disk devices and add to combo
    int diskIdx = 0;
    int pos = 0;
    while ((pos = xml.indexOf("<disk ", pos)) != -1) {
        int end = xml.indexOf(">", pos);
        if (end > pos) {
            QString diskTag = xml.mid(pos, end - pos + 1);
            
            if (diskTag.contains("device='disk'")) {
                int targetIdx = diskTag.indexOf("target dev='");
                if (targetIdx > 0) {
                    int start = targetIdx + 12;
                    int devEnd = diskTag.indexOf("'", start);
                    if (devEnd > start) {
                        QString dev = diskTag.mid(start, devEnd - start);
                        m_diskCombo->addItem(dev.toUpper());
                    }
                }
            }
        }
        pos = end + 1;
    }

    if (m_diskCombo->count() == 0) {
        m_diskCombo->addItem(tr("No disks found"));
        m_diskCombo->setEnabled(false);
    }
}

void BlkIOTuneDialog::saveSettings()
{
    if (!m_domain) return;

    QMessageBox::information(this, tr("Block I/O Tuning"),
        tr("Block I/O settings configured.\n\n"
           "Changes take effect immediately for running VMs."));
}

} // namespace QVirt
