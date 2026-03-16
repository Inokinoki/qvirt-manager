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

#ifndef QVIRT_UI_ERRORDIALOG_H
#define QVIRT_UI_ERRORDIALOG_H

#include <QDialog>
#include <QString>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>

namespace QVirt {

/**
 * @brief Enhanced error dialog with detailed information
 *
 * Shows error messages with optional technical details and actions
 */
class ErrorDialog : public QDialog
{
    Q_OBJECT

public:
    enum class ErrorType {
        Critical,
        Error,
        Warning,
        Info
    };
    Q_ENUM(ErrorType)

    explicit ErrorDialog(QWidget *parent = nullptr);
    ~ErrorDialog() override = default;

    void setTitle(const QString &title);
    void setMessage(const QString &message);
    void setDetails(const QString &details);
    void setErrorType(ErrorType type);
    void setErrorCode(const QString &code);
    void setSuggestions(const QStringList &suggestions);
    void setShowDetailsButton(bool show);
    void setRetryAction(const QString &action);

    QString details() const;
    void setHelpUrl(const QString &url);

public slots:
    void showError(const QString &title, const QString &message, const QString &details = QString());
    void showWarning(const QString &title, const QString &message);
    void showCritical(const QString &title, const QString &message, const QString &details = QString());

private slots:
    void toggleDetails();
    void onRetryClicked();
    void onHelpClicked();

private:
    void setupUI();
    void updateIcon();
    void updateSuggestions();

    QLabel *m_iconLabel;
    QLabel *m_titleLabel;
    QLabel *m_messageLabel;
    QTextEdit *m_detailsEdit;
    QLabel *m_errorCodeLabel;
    QTextEdit *m_suggestionsEdit;
    QPushButton *m_detailsBtn;
    QPushButton *m_retryBtn;
    QPushButton *m_helpBtn;
    QPushButton *m_closeBtn;

    ErrorType m_errorType;
    bool m_detailsVisible;
    QString m_helpUrl;
};

} // namespace QVirt

#endif // QVIRT_UI_ERRORDIALOG_H
