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

#include "VMProxyModel.h"

namespace QVirt {

VMProxyModel::VMProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_filterState(-1)
    , m_showFavorites(false)
    , m_sortColumn(SortColumn::Name)
{
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void VMProxyModel::setFilterText(const QString &text)
{
    if (m_filterText != text) {
        m_filterText = text;
        invalidateFilter();
        emit filterUpdated();
    }
}

void VMProxyModel::setFilterState(int state)
{
    if (m_filterState != state) {
        m_filterState = state;
        invalidateFilter();
        emit filterUpdated();
        emit statisticsChanged();
    }
}

void VMProxyModel::setShowOnlyFavorites(bool show)
{
    if (m_showFavorites != show) {
        m_showFavorites = show;
        invalidateFilter();
        emit filterUpdated();
    }
}

void VMProxyModel::setSortColumn(SortColumn column)
{
    if (m_sortColumn != column) {
        m_sortColumn = column;
        sort(0, sortOrder());
        emit filterUpdated();
    }
}

int VMProxyModel::totalVMs() const
{
    if (sourceModel()) {
        return sourceModel()->rowCount();
    }
    return 0;
}

int VMProxyModel::runningVMs() const
{
    int count = 0;
    if (sourceModel()) {
        for (int i = 0; i < sourceModel()->rowCount(); ++i) {
            QModelIndex idx = sourceModel()->index(i, 0);
            if (getStateFromIndex(idx) == 1) {
                count++;
            }
        }
    }
    return count;
}

int VMProxyModel::pausedVMs() const
{
    int count = 0;
    if (sourceModel()) {
        for (int i = 0; i < sourceModel()->rowCount(); ++i) {
            QModelIndex idx = sourceModel()->index(i, 0);
            if (getStateFromIndex(idx) == 2) {
                count++;
            }
        }
    }
    return count;
}

int VMProxyModel::stoppedVMs() const
{
    int count = 0;
    if (sourceModel()) {
        for (int i = 0; i < sourceModel()->rowCount(); ++i) {
            QModelIndex idx = sourceModel()->index(i, 0);
            if (getStateFromIndex(idx) == 3 || getStateFromIndex(idx) == 0) {
                count++;
            }
        }
    }
    return count;
}

bool VMProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    return matchesFilter(index);
}

bool VMProxyModel::matchesFilter(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return false;
    }

    // Search text filter
    if (!m_filterText.isEmpty()) {
        bool matchesText = false;
        for (int col = 0; col < sourceModel()->columnCount(index.parent()); ++col) {
            QModelIndex colIndex = sourceModel()->index(index.row(), col, index.parent());
            QString value = sourceModel()->data(colIndex, Qt::DisplayRole).toString();
            if (value.contains(m_filterText, Qt::CaseInsensitive)) {
                matchesText = true;
                break;
            }
        }
        if (!matchesText) {
            return false;
        }
    }

    // State filter
    if (m_filterState >= 0) {
        int state = getStateFromIndex(index);
        if (state != m_filterState) {
            return false;
        }
    }

    // Favorites filter
    if (m_showFavorites) {
        bool isFavorite = sourceModel()->data(index, Qt::UserRole + 1).toBool();
        if (!isFavorite) {
            return false;
        }
    }

    return true;
}

bool VMProxyModel::lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const
{
    switch (m_sortColumn) {
        case SortColumn::Name: {
            QString leftName = sourceModel()->data(sourceLeft, Qt::DisplayRole).toString();
            QString rightName = sourceModel()->data(sourceRight, Qt::DisplayRole).toString();
            return QString::localeAwareCompare(leftName, rightName) < 0;
        }
        case SortColumn::State: {
            int leftState = getStateFromIndex(sourceLeft);
            int rightState = getStateFromIndex(sourceRight);
            if (leftState != rightState) {
                return leftState < rightState;
            }
            return sourceLeft.row() < sourceRight.row();
        }
        case SortColumn::CPU: {
            double leftCPU = sourceModel()->data(sourceLeft, Qt::UserRole).toDouble();
            double rightCPU = sourceModel()->data(sourceRight, Qt::UserRole).toDouble();
            return leftCPU < rightCPU;
        }
        case SortColumn::Memory: {
            qlonglong leftMem = sourceModel()->data(sourceLeft, Qt::UserRole + 2).toLongLong();
            qlonglong rightMem = sourceModel()->data(sourceRight, Qt::UserRole + 2).toLongLong();
            return leftMem < rightMem;
        }
        case SortColumn::LastUsed: {
            QDateTime leftTime = sourceModel()->data(sourceLeft, Qt::UserRole + 3).toDateTime();
            QDateTime rightTime = sourceModel()->data(sourceRight, Qt::UserRole + 3).toDateTime();
            return leftTime > rightTime;
        }
        default:
            break;
    }
    return QSortFilterProxyModel::lessThan(sourceLeft, sourceRight);
}

int VMProxyModel::getStateFromIndex(const QModelIndex &index) const
{
    QVariant stateData = sourceModel()->data(index, Qt::UserRole + 4);
    return stateData.toInt();
}

} // namespace QVirt
