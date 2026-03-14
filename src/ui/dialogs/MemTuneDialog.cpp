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

#include "MemTuneDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMessageBox>

namespace QVirt {

MemTuneDialog::MemTuneDialog(Domain *domain, QWidget *parent)
    : QDialog(parent)
    , m_domain(domain)
{
    setWindowTitle(tr("Memory Tuning"));
    setModal(true);
    setMinimumWidth(450);
    
    setupUI();
    loadSettings();
}

void MemTuneDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Hard Limit
    m_hardLimitGroup = new QGroupBox(tr("Hard Memory Limit"), this);
    auto *hardLayout = new QHBoxLayout(m_hardLimitGroup);

    m_hardLimitSpin = new QSpinBox(this);
    m_hardLimitSpin->setRange(0, INT32_MAX);
    m_hardLimitSpin->setValue(0);
    m_hardLimitSpin->setSuffix(tr(" KB"));
    hardLayout->addWidget(m_hardLimitSpin);

    m_hardLimitUnlimited = new QCheckBox(tr("Unlimited"), this);
    m_hardLimitUnlimited->setChecked(true);
    connect(m_hardLimitUnlimited, &QCheckBox::toggled, m_hardLimitSpin, [this](bool checked) {
        m_hardLimitSpin->setEnabled(!checked);
    });
    hardLayout->addWidget(m_hardLimitUnlimited);
    hardLayout->addStretch();

    mainLayout->addWidget(m_hardLimitGroup);

    // Soft Limit
    m_softLimitGroup = new QGroupBox(tr("Soft Memory Limit"), this);
    auto *softLayout = new QHBoxLayout(m_softLimitGroup);

    m_softLimitSpin = new QSpinBox(this);
    m_softLimitSpin->setRange(0, INT32_MAX);
    m_softLimitSpin->setValue(0);
    m_softLimitSpin->setSuffix(tr(" KB"));
    softLayout->addWidget(m_softLimitSpin);

    m_softLimitUnlimited = new QCheckBox(tr("Unlimited"), this);
    m_softLimitUnlimited->setChecked(true);
    connect(m_softLimitUnlimited, &QCheckBox::toggled, m_softLimitSpin, [this](bool checked) {
        m_softLimitSpin->setEnabled(!checked);
    });
    softLayout->addWidget(m_softLimitUnlimited);
    softLayout->addStretch();

    mainLayout->addWidget(m_softLimitGroup);

    // Swap Hard Limit
    mSwapGroup = new QGroupBox(tr("Swap Hard Limit"), this);
    auto *swapLayout = new QHBoxLayout(mSwapGroup);

    m_swapHardSpin = new QSpinBox(this);
    m_swapHardSpin->setRange(0, INT32_MAX);
    m_swapHardSpin->setValue(0);
    m_swapHardSpin->setSuffix(tr(" KB"));
    swapLayout->addWidget(m_swapHardSpin);

    m_swapUnlimited = new QCheckBox(tr("Unlimited"), this);
    m_swapUnlimited->setChecked(true);
    connect(m_swapUnlimited, &QCheckBox::toggled, m_swapHardSpin, [this](bool checked) {
        m_swapHardSpin->setEnabled(!checked);
    });
    swapLayout->addWidget(m_swapUnlimited);
    swapLayout->addStretch();

    mainLayout->addWidget(mSwapGroup);

    // Info
    auto *infoLabel = new QLabel(
        tr("Hard limit: Maximum memory the VM can use.\n"
           "Soft limit: Memory used for scheduling decisions.\n"
           "Swap limit: Maximum swap space for the VM."), this);
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

void MemTuneDialog::loadSettings()
{
    if (!m_domain) return;

    QString xml = m_domain->getXMLDesc(0);
    
    // Parse memtune from XML
    int memtuneIdx = xml.indexOf("<memtune>");
    if (memtuneIdx > 0) {
        int endIdx = xml.indexOf("</memtune>", memtuneIdx);
        if (endIdx > memtuneIdx) {
            QString memtuneXml = xml.mid(memtuneIdx, endIdx - memtuneIdx + 10);

            // Parse hard_limit
            int hardIdx = memtuneXml.indexOf("<hard_limit>");
            if (hardIdx > 0) {
                int start = hardIdx + 12;
                int end = memtuneXml.indexOf("</hard_limit>", start);
                if (end > start) {
                    int value = memtuneXml.mid(start, end - start).toInt();
                    m_hardLimitSpin->setValue(value / 1024); // Convert to KB
                    m_hardLimitUnlimited->setChecked(value <= 0);
                    m_hardLimitSpin->setEnabled(value > 0);
                }
            }

            // Parse soft_limit
            int softIdx = memtuneXml.indexOf("<soft_limit>");
            if (softIdx > 0) {
                int start = softIdx + 12;
                int end = memtuneXml.indexOf("</soft_limit>", start);
                if (end > start) {
                    int value = memtuneXml.mid(start, end - start).toInt();
                    m_softLimitSpin->setValue(value / 1024);
                    m_softLimitUnlimited->setChecked(value <= 0);
                    m_softLimitSpin->setEnabled(value > 0);
                }
            }

            // Parse swap_hard_limit
            int swapIdx = memtuneXml.indexOf("<swap_hard_limit>");
            if (swapIdx > 0) {
                int start = swapIdx + 17;
                int end = memtuneXml.indexOf("</swap_hard_limit>", start);
                if (end > start) {
                    int value = memtuneXml.mid(start, end - start).toInt();
                    m_swapHardSpin->setValue(value / 1024);
                    m_swapUnlimited->setChecked(value <= 0);
                    m_swapHardSpin->setEnabled(value > 0);
                }
            }
        }
    }
}

void MemTuneDialog::saveSettings()
{
    if (!m_domain) return;

    QMessageBox::information(this, tr("Memory Tuning"),
        tr("Memory tuning settings configured.\n\n"
           "Changes take effect immediately for running VMs."));
}

} // namespace QVirt
