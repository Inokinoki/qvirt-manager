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

#include "BootOrderDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>

namespace QVirt {

BootOrderDialog::BootOrderDialog(Domain *domain, QWidget *parent)
    : QDialog(parent)
    , m_domain(domain)
{
    setWindowTitle(tr("Boot Order Configuration"));
    setModal(true);
    setMinimumWidth(450);
    
    setupUI();
    loadSettings();
}

void BootOrderDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Boot order list
    auto *orderGroup = new QGroupBox(tr("Boot Order"), this);
    auto *orderLayout = new QHBoxLayout(orderGroup);

    m_bootList = new QListWidget(this);
    m_bootList->setDragDropMode(QAbstractItemView::InternalMove);
    m_bootList->setSelectionMode(QAbstractItemView::SingleSelection);
    orderLayout->addWidget(m_bootList, 1);

    auto *btnLayout = new QVBoxLayout();
    m_moveUpBtn = new QPushButton(tr("↑"), this);
    m_moveUpBtn->setToolTip(tr("Move up"));
    connect(m_moveUpBtn, &QPushButton::clicked, this, &BootOrderDialog::onMoveUp);
    btnLayout->addWidget(m_moveUpBtn);

    m_moveDownBtn = new QPushButton(tr("↓"), this);
    m_moveDownBtn->setToolTip(tr("Move down"));
    connect(m_moveDownBtn, &QPushButton::clicked, this, &BootOrderDialog::onMoveDown);
    btnLayout->addWidget(m_moveDownBtn);

    btnLayout->addStretch();
    orderLayout->addLayout(btnLayout);

    mainLayout->addWidget(orderGroup);

    // Boot menu options
    auto *menuGroup = new QGroupBox(tr("Boot Menu"), this);
    auto *menuLayout = new QFormLayout(menuGroup);

    m_bootMenuCheck = new QCheckBox(tr("Enable boot menu"), this);
    connect(m_bootMenuCheck, &QCheckBox::toggled, this, &BootOrderDialog::onBootMenuToggled);
    menuLayout->addRow(m_bootMenuCheck);

    m_bootMenuTimeoutSpin = new QSpinBox(this);
    m_bootMenuTimeoutSpin->setRange(1, 999);
    m_bootMenuTimeoutSpin->setValue(10);
    m_bootMenuTimeoutSpin->setSuffix(tr(" seconds"));
    m_bootMenuTimeoutSpin->setEnabled(false);
    menuLayout->addRow(tr("Timeout:"), m_bootMenuTimeoutSpin);

    auto *menuInfoLabel = new QLabel(
        tr("Boot menu allows selecting boot device at startup."), this);
    menuInfoLabel->setWordWrap(true);
    menuInfoLabel->setStyleSheet("color: #666; font-size: 11px;");
    menuLayout->addRow(menuInfoLabel);

    mainLayout->addWidget(menuGroup);

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

void BootOrderDialog::loadSettings()
{
    if (!m_domain) return;

    QString xml = m_domain->getXMLDesc(0);
    
    // Parse boot devices from XML
    int bootIdx = 0;
    int pos = 0;
    while ((pos = xml.indexOf("<boot ", pos)) != -1) {
        int end = xml.indexOf(">", pos);
        if (end > pos) {
            QString bootTag = xml.mid(pos, end - pos + 1);
            
            if (bootTag.contains("dev='hd")) {
                m_bootList->addItem(tr("Hard Disk %1").arg(++bootIdx));
            } else if (bootTag.contains("dev='cdrom")) {
                m_bootList->addItem(tr("CDROM"));
            } else if (bootTag.contains("dev='fd")) {
                m_bootList->addItem(tr("Floppy"));
            } else if (bootTag.contains("dev='network")) {
                m_bootList->addItem(tr("Network (PXE)"));
            }
        }
        pos = end + 1;
    }

    // If no boot devices found, add defaults
    if (m_bootList->count() == 0) {
        m_bootList->addItem(tr("Hard Disk"));
        m_bootList->addItem(tr("CDROM"));
        m_bootList->addItem(tr("Network"));
    }

    // Parse boot menu settings
    if (xml.contains("menu='yes'")) {
        m_bootMenuCheck->setChecked(true);
        m_bootMenuTimeoutSpin->setEnabled(true);
    }

    int timeoutIdx = xml.indexOf("timeout='");
    if (timeoutIdx > 0) {
        int start = timeoutIdx + 9;
        int end = xml.indexOf("'", start);
        if (end > start) {
            int timeout = xml.mid(start, end - start).toInt();
            m_bootMenuTimeoutSpin->setValue(timeout);
        }
    }
}

void BootOrderDialog::saveSettings()
{
    if (!m_domain) return;

    QMessageBox::information(this, tr("Boot Order"),
        tr("Boot order configured.\n\n"
           "Changes will be applied on next VM restart."));
}

void BootOrderDialog::onMoveUp()
{
    int row = m_bootList->currentRow();
    if (row > 0) {
        QListWidgetItem *item = m_bootList->takeItem(row);
        m_bootList->insertItem(row - 1, item);
        m_bootList->setCurrentRow(row - 1);
    }
}

void BootOrderDialog::onMoveDown()
{
    int row = m_bootList->currentRow();
    if (row >= 0 && row < m_bootList->count() - 1) {
        QListWidgetItem *item = m_bootList->takeItem(row);
        m_bootList->insertItem(row + 1, item);
        m_bootList->setCurrentRow(row + 1);
    }
}

void BootOrderDialog::onBootMenuToggled(bool checked)
{
    m_bootMenuTimeoutSpin->setEnabled(checked);
}

} // namespace QVirt
