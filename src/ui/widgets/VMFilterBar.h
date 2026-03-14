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

#ifndef QVIRT_UI_VMFILTERBAR_H
#define QVIRT_UI_VMFILTERBAR_H

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QComboBox>
#include <QToolButton>

namespace QVirt {

/**
 * @brief VM filter bar widget
 *
 * Provides search and filter functionality for VM lists
 */
class VMFilterBar : public QWidget
{
    Q_OBJECT

public:
    enum class FilterState {
        All,
        Running,
        Paused,
        Stopped
    };
    Q_ENUM(FilterState)

    explicit VMFilterBar(QWidget *parent = nullptr);
    ~VMFilterBar() override = default;

    QString searchText() const;
    FilterState filterState() const;
    bool showOnlyFavorites() const;

    void setSearchText(const QString &text);
    void setFilterState(FilterState state);
    void setShowOnlyFavorites(bool show);

    void clear();

signals:
    void filterChanged();
    void searchTextChanged(const QString &text);
    void stateFilterChanged(FilterState state);
    void favoritesFilterChanged(bool showOnly);

private slots:
    void onSearchTextChanged();
    void onStateChanged(int index);
    void onFavoritesToggled(bool checked);

private:
    void setupUI();

    QLineEdit *m_searchEdit;
    QComboBox *m_stateCombo;
    QToolButton *m_favoritesBtn;
    FilterState m_currentState;
    bool m_showFavorites;
};

} // namespace QVirt

#endif // QVIRT_UI_VMFILTERBAR_H
