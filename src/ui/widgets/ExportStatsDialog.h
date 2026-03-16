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

#ifndef QVIRT_UI_EXPORTSTATSDIALOG_H
#define QVIRT_UI_EXPORTSTATSDIALOG_H

#include <QDialog>
#include <QString>
#include <QDateTime>

namespace QVirt {

/**
 * @brief Dialog for exporting VM performance statistics
 */
class ExportStatsDialog : public QDialog
{
    Q_OBJECT

public:
    enum class ExportFormat {
        CSV,
        JSON,
        PDF
    };
    Q_ENUM(ExportFormat)

    explicit ExportStatsDialog(QWidget *parent = nullptr);
    ~ExportStatsDialog() override = default;

    void setVMName(const QString &name);

    QString outputPath() const;
    ExportFormat format() const;
    QDateTime startDate() const;
    QDateTime endDate() const;

    void setOutputPath(const QString &path);
    void setFormat(ExportFormat fmt);
    void setDateRange(const QDateTime &start, const QDateTime &end);

    void setAvailableMetrics(const QStringList &metrics);

signals:
    void exportRequested();

private slots:
    void onBrowseClicked();
    void onExportClicked();

private:
    void setupUI();

    QString m_vmName;
    QString m_outputPath;
    ExportFormat m_format;
    QDateTime m_startDate;
    QDateTime m_endDate;
};

} // namespace QVirt

#endif // QVIRT_UI_EXPORTSTATSDIALOG_H
