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

#include "ConnectionProgressDialog.h"

#include <QVBoxLayout>
#include <QApplication>

namespace QVirt {

ConnectionProgressDialog::ConnectionProgressDialog(QWidget *parent)
    : QDialog(parent)
    , m_statusLabel(nullptr)
    , m_progressBar(nullptr)
{
    setWindowTitle(tr("Connecting..."));
    setModal(true);
    setSizeGripEnabled(false);
    setWindowFlag(Qt::WindowCloseButtonHint, false);
    setMinimumWidth(400);

    setupUI();
}

ConnectionProgressDialog::~ConnectionProgressDialog() = default;

void ConnectionProgressDialog::setupUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    // Status label
    m_statusLabel = new QLabel(tr("Connecting..."));
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_statusLabel);

    // Progress bar (indeterminate)
    m_progressBar = new QProgressBar();
    m_progressBar->setRange(0, 0);  // Indeterminate mode
    m_progressBar->setTextVisible(false);
    m_progressBar->setFixedHeight(8);
    layout->addWidget(m_progressBar);
}

void ConnectionProgressDialog::updateStatus(const QString &status)
{
    m_statusLabel->setText(status);
    // Process events to ensure UI updates immediately
    QApplication::processEvents();
}

} // namespace QVirt
