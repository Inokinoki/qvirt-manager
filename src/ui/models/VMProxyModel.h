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

#ifndef QVIRT_UI_VMPROXYMODEL_H
#define QVIRT_UI_VMPROXYMODEL_H

#include "../../core/BaseObject.h"
#include <QSortFilterProxyModel>
#include <QString>
#include <QDateTime>

namespace QVirt {

/**
 * @brief Proxy model for VM list filtering and sorting
 *
 * Provides search, state filtering, and custom sorting for VM lists
 */
class VMProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    enum class SortColumn {
        Name,
        State,
        CPU,
        Memory,
        LastUsed
    };
    Q_ENUM(SortColumn)

    explicit VMProxyModel(QObject *parent = nullptr);
    ~VMProxyModel() override = default;

    // Filter settings
    QString filterText() const { return m_filterText; }
    void setFilterText(const QString &text);

    int filterState() const { return m_filterState; }
    void setFilterState(int state);

    bool showOnlyFavorites() const { return m_showFavorites; }
    void setShowOnlyFavorites(bool show);

    // Sort settings
    SortColumn sortColumn() const { return m_sortColumn; }
    void setSortColumn(SortColumn column);

    // Statistics
    int totalVMs() const;
    int runningVMs() const;
    int pausedVMs() const;
    int stoppedVMs() const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const override;

signals:
    void filterUpdated();
    void statisticsChanged();

private:
    bool matchesFilter(const QModelIndex &index) const;
    int getStateFromIndex(const QModelIndex &index) const;

    QString m_filterText;
    int m_filterState = -1;
    bool m_showFavorites = false;
    SortColumn m_sortColumn = SortColumn::Name;
};

} // namespace QVirt

#endif // QVIRT_UI_VMPROXYMODEL_H
