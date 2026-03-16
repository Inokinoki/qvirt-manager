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

#include "VMFilterBar.h"
#include <QHBoxLayout>
#include <QLabel>

namespace QVirt {

VMFilterBar::VMFilterBar(QWidget *parent)
    : QWidget(parent)
    , m_searchEdit(nullptr)
    , m_stateCombo(nullptr)
    , m_favoritesBtn(nullptr)
    , m_currentState(FilterState::All)
    , m_showFavorites(false)
{
    setupUI();
}

void VMFilterBar::setupUI()
{
    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(8);

    // Search box
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText(tr("Search VMs..."));
    m_searchEdit->setClearButtonEnabled(true);
    connect(m_searchEdit, &QLineEdit::textChanged,
            this, &VMFilterBar::onSearchTextChanged);
    layout->addWidget(m_searchEdit, 1);

    // State filter
    m_stateCombo = new QComboBox(this);
    m_stateCombo->addItem(tr("All States"), static_cast<int>(FilterState::All));
    m_stateCombo->addItem(tr("Running"), static_cast<int>(FilterState::Running));
    m_stateCombo->addItem(tr("Paused"), static_cast<int>(FilterState::Paused));
    m_stateCombo->addItem(tr("Stopped"), static_cast<int>(FilterState::Stopped));
    connect(m_stateCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VMFilterBar::onStateChanged);
    layout->addWidget(m_stateCombo);

    // Favorites button
    m_favoritesBtn = new QToolButton(this);
    m_favoritesBtn->setText(tr("★"));
    m_favoritesBtn->setCheckable(true);
    m_favoritesBtn->setToolTip(tr("Show only favorites"));
    connect(m_favoritesBtn, &QToolButton::toggled,
            this, &VMFilterBar::onFavoritesToggled);
    layout->addWidget(m_favoritesBtn);
}

QString VMFilterBar::searchText() const
{
    return m_searchEdit->text();
}

VMFilterBar::FilterState VMFilterBar::filterState() const
{
    return m_currentState;
}

bool VMFilterBar::showOnlyFavorites() const
{
    return m_showFavorites;
}

void VMFilterBar::setSearchText(const QString &text)
{
    m_searchEdit->setText(text);
}

void VMFilterBar::setFilterState(FilterState state)
{
    m_currentState = state;
    for (int i = 0; i < m_stateCombo->count(); ++i) {
        if (m_stateCombo->itemData(i).toInt() == static_cast<int>(state)) {
            m_stateCombo->setCurrentIndex(i);
            break;
        }
    }
}

void VMFilterBar::setShowOnlyFavorites(bool show)
{
    m_showFavorites = show;
    m_favoritesBtn->setChecked(show);
}

void VMFilterBar::clear()
{
    m_searchEdit->clear();
    m_stateCombo->setCurrentIndex(0);
    m_favoritesBtn->setChecked(false);
}

void VMFilterBar::onSearchTextChanged()
{
    emit searchTextChanged(m_searchEdit->text());
    emit filterChanged();
}

void VMFilterBar::onStateChanged(int index)
{
    m_currentState = static_cast<FilterState>(m_stateCombo->itemData(index).toInt());
    emit stateFilterChanged(m_currentState);
    emit filterChanged();
}

void VMFilterBar::onFavoritesToggled(bool checked)
{
    m_showFavorites = checked;
    emit favoritesFilterChanged(checked);
    emit filterChanged();
}

} // namespace QVirt
