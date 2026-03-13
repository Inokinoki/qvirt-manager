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

#include "OSListDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QApplication>
#include <QPushButton>

namespace QVirt {

OSListDialog::OSListDialog(QWidget *parent)
    : QDialog(parent)
    , m_filterEOL(false)
{
    setWindowTitle("Select Operating System");
    setMinimumSize(600, 500);
    
    m_osDb = OSDatabase::instance();
    
    setupUI();
    loadOSList();
}

OSListDialog::~OSListDialog()
{
}

void OSListDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    
    // Search box
    auto *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Search:"));
    
    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("Type to search for an operating system...");
    m_searchEdit->setClearButtonEnabled(true);
    searchLayout->addWidget(m_searchEdit, 1);
    
    mainLayout->addLayout(searchLayout);
    
    // OS list
    m_osList = new QListWidget();
    m_osList->setAlternatingRowColors(true);
    connect(m_osList, &QListWidget::itemActivated,
            this, &OSListDialog::onOSItemActivated);
    connect(m_osList, &QListWidget::currentItemChanged,
            this, [this]() {
                if (m_osList->currentItem()) {
                    OSInfo os = itemToOSInfo(m_osList->currentItem());
                    emit osSelected(os);
                }
            });
    mainLayout->addWidget(m_osList, 1);
    
    // OS info label
    m_osInfoLabel = new QLabel();
    m_osInfoLabel->setWordWrap(true);
    m_osInfoLabel->setStyleSheet("QLabel { color: #666; padding: 5px; }");
    mainLayout->addWidget(m_osInfoLabel);
    
    // Filter options
    auto *filterLayout = new QHBoxLayout();
    m_eolFilterCheck = new QCheckBox("Hide end-of-life operating systems");
    m_eolFilterCheck->setChecked(m_filterEOL);
    connect(m_eolFilterCheck, &QCheckBox::toggled,
            this, &OSListDialog::onEOLFilterToggled);
    filterLayout->addWidget(m_eolFilterCheck);
    filterLayout->addStretch();
    mainLayout->addLayout(filterLayout);
    
    // Buttons
    auto *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    m_okButton = buttonBox->button(QDialogButtonBox::Ok);
    m_cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    
    m_okButton->setEnabled(false);
    
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
    
    // Connect search
    connect(m_searchEdit, &QLineEdit::textChanged,
            this, &OSListDialog::onSearchTextChanged);
}

void OSListDialog::loadOSList()
{
    m_allOSes = m_osDb->getOperatingSystems(m_filterEOL);
    updateOSList();
}

void OSListDialog::updateOSList()
{
    m_osList->clear();
    
    QString searchText = m_searchEdit->text().toLower();
    
    for (const OSInfo &os : m_allOSes) {
        // Apply search filter
        if (!searchText.isEmpty()) {
            if (!os.name.toLower().contains(searchText) &&
                !os.id.toLower().contains(searchText) &&
                !os.shortId.toLower().contains(searchText)) {
                continue;
            }
        }
        
        auto *item = new QListWidgetItem(os.name);
        item->setData(Qt::UserRole, os.id);
        
        // Mark EOL OSes
        if (os.isEol) {
            item->setForeground(QColor("#999"));
            item->setText(os.name + " (End of Life)");
        }
        
        // Mark generic OSes
        if (os.isGeneric) {
            item->setText(os.name + " (Generic)");
        }
        
        m_osList->addItem(item);
    }
}

OSInfo OSListDialog::getSelectedOS() const
{
    QListWidgetItem *item = m_osList->currentItem();
    if (!item) {
        return OSInfo();
    }
    return itemToOSInfo(item);
}

void OSListDialog::setSelectedOS(const QString &osId)
{
    for (int i = 0; i < m_osList->count(); ++i) {
        QListWidgetItem *item = m_osList->item(i);
        if (item && item->data(Qt::UserRole).toString() == osId) {
            m_osList->setCurrentItem(item);
            m_okButton->setEnabled(true);
            break;
        }
    }
}

void OSListDialog::setFilterEOL(bool filter)
{
    m_filterEOL = filter;
    m_eolFilterCheck->setChecked(filter);
    loadOSList();
}

OSInfo OSListDialog::itemToOSInfo(QListWidgetItem *item) const
{
    if (!item) {
        return OSInfo();
    }
    QString osId = item->data(Qt::UserRole).toString();
    return m_osDb->getOperatingSystem(osId);
}

void OSListDialog::onSearchTextChanged(const QString &text)
{
    Q_UNUSED(text);
    updateOSList();
}

void OSListDialog::onEOLFilterToggled(bool checked)
{
    m_filterEOL = checked;
    m_allOSes = m_osDb->getOperatingSystems(m_filterEOL);
    updateOSList();
}

void OSListDialog::onOSItemActivated(QListWidgetItem *item)
{
    if (item) {
        OSInfo os = itemToOSInfo(item);
        if (!os.id.isEmpty()) {
            accept();
        }
    }
}

} // namespace QVirt
