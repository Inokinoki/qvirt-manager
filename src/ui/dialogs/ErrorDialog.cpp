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

#include "ErrorDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDesktopServices>
#include <QUrl>
#include <QFontDatabase>

namespace QVirt {

ErrorDialog::ErrorDialog(QWidget *parent)
    : QDialog(parent)
    , m_iconLabel(nullptr)
    , m_titleLabel(nullptr)
    , m_messageLabel(nullptr)
    , m_detailsEdit(nullptr)
    , m_errorCodeLabel(nullptr)
    , m_suggestionsEdit(nullptr)
    , m_detailsBtn(nullptr)
    , m_retryBtn(nullptr)
    , m_helpBtn(nullptr)
    , m_closeBtn(nullptr)
    , m_errorType(ErrorType::Error)
    , m_detailsVisible(false)
{
    setupUI();
}

void ErrorDialog::setupUI()
{
    setModal(true);
    setMinimumWidth(500);
    setMinimumHeight(300);

    auto layout = new QVBoxLayout(this);
    layout->setSpacing(12);

    // Header with icon and title
    auto headerLayout = new QHBoxLayout();
    m_iconLabel = new QLabel(this);
    m_iconLabel->setFixedSize(48, 48);
    headerLayout->addWidget(m_iconLabel);

    m_titleLabel = new QLabel(tr("Error"), this);
    m_titleLabel->setFont(font());
    headerLayout->addWidget(m_titleLabel, 1);
    layout->addLayout(headerLayout);

    // Message
    m_messageLabel = new QLabel(this);
    m_messageLabel->setWordWrap(true);
    layout->addWidget(m_messageLabel);

    // Error code
    m_errorCodeLabel = new QLabel(this);
    m_errorCodeLabel->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    m_errorCodeLabel->setVisible(false);
    layout->addWidget(m_errorCodeLabel);

    // Details section
    m_detailsEdit = new QTextEdit(this);
    m_detailsEdit->setReadOnly(true);
    m_detailsEdit->setMaximumHeight(150);
    m_detailsEdit->setVisible(false);
    layout->addWidget(m_detailsEdit);

    // Suggestions section
    m_suggestionsEdit = new QTextEdit(this);
    m_suggestionsEdit->setReadOnly(true);
    m_suggestionsEdit->setMaximumHeight(100);
    m_suggestionsEdit->setVisible(false);
    layout->addWidget(m_suggestionsEdit);

    // Buttons
    auto buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_detailsBtn = new QPushButton(tr("Show Details"), this);
    connect(m_detailsBtn, &QPushButton::clicked, this, &ErrorDialog::toggleDetails);
    buttonLayout->addWidget(m_detailsBtn);

    m_retryBtn = new QPushButton(tr("Retry"), this);
    m_retryBtn->setVisible(false);
    connect(m_retryBtn, &QPushButton::clicked, this, &ErrorDialog::onRetryClicked);
    buttonLayout->addWidget(m_retryBtn);

    m_helpBtn = new QPushButton(tr("Help"), this);
    m_helpBtn->setVisible(false);
    connect(m_helpBtn, &QPushButton::clicked, this, &ErrorDialog::onHelpClicked);
    buttonLayout->addWidget(m_helpBtn);

    m_closeBtn = new QPushButton(tr("Close"), this);
    m_closeBtn->setDefault(true);
    connect(m_closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(m_closeBtn);

    layout->addLayout(buttonLayout);
}

void ErrorDialog::setTitle(const QString &title)
{
    m_titleLabel->setText(title);
}

void ErrorDialog::setMessage(const QString &message)
{
    m_messageLabel->setText(message);
}

void ErrorDialog::setDetails(const QString &details)
{
    m_detailsEdit->setText(details);
    m_detailsBtn->setEnabled(!details.isEmpty());
}

void ErrorDialog::setErrorType(ErrorType type)
{
    m_errorType = type;
    updateIcon();
}

void ErrorDialog::setErrorCode(const QString &code)
{
    if (!code.isEmpty()) {
        m_errorCodeLabel->setText(tr("Error Code: %1").arg(code));
        m_errorCodeLabel->setVisible(true);
    } else {
        m_errorCodeLabel->setVisible(false);
    }
}

void ErrorDialog::setSuggestions(const QStringList &suggestions)
{
    if (!suggestions.isEmpty()) {
        QString text;
        for (const QString &s : suggestions) {
            text += "• " + s + "\n";
        }
        m_suggestionsEdit->setText(text.trimmed());
        m_suggestionsEdit->setVisible(true);
    } else {
        m_suggestionsEdit->setVisible(false);
    }
}

void ErrorDialog::setShowDetailsButton(bool show)
{
    m_detailsBtn->setVisible(show);
}

void ErrorDialog::setRetryAction(const QString &action)
{
    if (!action.isEmpty()) {
        m_retryBtn->setText(action);
        m_retryBtn->setVisible(true);
    } else {
        m_retryBtn->setVisible(false);
    }
}

void ErrorDialog::setHelpUrl(const QString &url)
{
    m_helpUrl = url;
    m_helpBtn->setVisible(!url.isEmpty());
}

QString ErrorDialog::details() const
{
    return m_detailsEdit->toPlainText();
}

void ErrorDialog::showError(const QString &title, const QString &message, const QString &details)
{
    setTitle(title);
    setMessage(message);
    setDetails(details);
    setErrorType(ErrorType::Error);
    exec();
}

void ErrorDialog::showWarning(const QString &title, const QString &message)
{
    setTitle(title);
    setMessage(message);
    setErrorType(ErrorType::Warning);
    exec();
}

void ErrorDialog::showCritical(const QString &title, const QString &message, const QString &details)
{
    setTitle(title);
    setMessage(message);
    setDetails(details);
    setErrorType(ErrorType::Critical);
    exec();
}

void ErrorDialog::toggleDetails()
{
    m_detailsVisible = !m_detailsVisible;
    m_detailsEdit->setVisible(m_detailsVisible);
    m_detailsBtn->setText(m_detailsVisible ? tr("Hide Details") : tr("Show Details"));
    adjustSize();
}

void ErrorDialog::onRetryClicked()
{
    emit accepted();
}

void ErrorDialog::onHelpClicked()
{
    if (!m_helpUrl.isEmpty()) {
        QDesktopServices::openUrl(QUrl(m_helpUrl));
    }
}

void ErrorDialog::updateIcon()
{
    QString icon;
    switch (m_errorType) {
        case ErrorType::Critical:
            icon = "❌";
            break;
        case ErrorType::Error:
            icon = "⚠️";
            break;
        case ErrorType::Warning:
            icon = "⚡";
            break;
        case ErrorType::Info:
            icon = "ℹ️";
            break;
    }
    m_iconLabel->setText(icon);
}

void ErrorDialog::updateSuggestions()
{
    // Suggestions are updated via setSuggestions
}

} // namespace QVirt
