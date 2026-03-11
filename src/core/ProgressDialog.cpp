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

#include "ProgressDialog.h"
#include <QHBoxLayout>

namespace QVirt {

ProgressDialog::ProgressDialog(const QString &title, const QString &labelText,
                               QWidget *parent)
    : QProgressDialog(parent)
    , m_statusLabel(nullptr)
    , m_detailsLabel(nullptr)
{
    setWindowTitle(title);
    setMinimumSize(400, 150);
    setMinimumDuration(500);  // Show after 500ms
    setRange(0, 100);
    setValue(0);

    // Create status label
    m_statusLabel = new QLabel(labelText);
    m_statusLabel->setWordWrap(true);

    // Create details label
    m_detailsLabel = new QLabel();
    m_detailsLabel->setWordWrap(true);
    m_detailsLabel->setStyleSheet("QLabel { color: #666; font-size: 10px; }");

    // Create layout for labels
    auto *labelLayout = new QVBoxLayout();
    labelLayout->addWidget(m_statusLabel);
    labelLayout->addWidget(m_detailsLabel);
    labelLayout->addStretch();

    // Replace the default label with our custom labels
    setLabel(m_statusLabel);

    // Connect cancel button
    connect(this, &QProgressDialog::canceled, this, &ProgressDialog::onCancelClicked);

    // Disable auto-close (we want to show completion)
    setAutoClose(false);
    setAutoReset(false);
}

void ProgressDialog::startJob()
{
    setValue(0);
    setCancelButtonText(tr("Cancel"));
    setEnabled(true);
    show();
}

void ProgressDialog::updateProgress(int value, const QString &status)
{
    setValue(value);

    if (!status.isEmpty()) {
        m_statusLabel->setText(status);
    }

    // Update details with percentage
    if (value >= 0 && value <= 100) {
        m_detailsLabel->setText(tr("%1% complete").arg(value));
    }
}

void ProgressDialog::finishJob()
{
    setValue(100);
    m_statusLabel->setText(tr("Operation completed successfully"));
    m_detailsLabel->setText(tr("100% complete"));
    setCancelButtonText(tr("Close"));

    // Auto-close after 2 seconds
    QTimer::singleShot(2000, this, [this]() {
        accept();
    });
}

void ProgressDialog::setError(const QString &error)
{
    m_statusLabel->setText(tr("Error"));
    m_statusLabel->setStyleSheet("QLabel { color: red; font-weight: bold; }");
    m_detailsLabel->setText(error);
    setCancelButtonText(tr("Close"));
}

void ProgressDialog::onCancelClicked()
{
    if (value() < 100) {
        // Job is still running - emit cancel request
        emit cancelRequested();
    } else {
        // Job is complete - just close
        accept();
    }
}

} // namespace QVirt
