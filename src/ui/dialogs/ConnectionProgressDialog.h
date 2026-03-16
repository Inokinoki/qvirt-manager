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

#ifndef CONNECTIONPROGRESSDIALOG_H
#define CONNECTIONPROGRESSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QProgressBar>

namespace QVirt {

class ConnectionProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionProgressDialog(QWidget *parent = nullptr);
    ~ConnectionProgressDialog() override;

public slots:
    void updateStatus(const QString &status);

private:
    void setupUI();

    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
};

} // namespace QVirt

#endif // CONNECTIONPROGRESSDIALOG_H
