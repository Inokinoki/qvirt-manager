/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 Inoki <veyx.shaw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version).
 */

#ifndef QVIRT_CORE_PROGRESSDIALOG_H
#define QVIRT_CORE_PROGRESSDIALOG_H

#include <QProgressDialog>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QTimer>

#include "BaseUIObject.h"

namespace QVirt {

/**
 * @brief Progress Dialog for Long-Running Operations
 *
 * Shows progress for operations like VM migration, cloning, storage operations
 */
class ProgressDialog : public QProgressDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(const QString &title, const QString &labelText,
                           QWidget *parent = nullptr);
    ~ProgressDialog() override = default;

    void startJob();
    void updateProgress(int value, const QString &status = QString());
    void finishJob();
    void setError(const QString &error);

signals:
    void cancelRequested();

private slots:
    void onCancelClicked();

private:
    QLabel *m_statusLabel;
    QLabel *m_detailsLabel;
};

} // namespace QVirt

#endif // QVIRT_CORE_PROGRESSDIALOG_H
