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

#include "Error.h"
#include <QMessageBox>
#include <QWidget>

namespace QVirt {

void Error::showError(const QString &title, const QString &message)
{
    QMessageBox::critical(nullptr, title, message);
}

void Error::showWarning(const QString &title, const QString &message)
{
    QMessageBox::warning(nullptr, title, message);
}

void Error::showInfo(const QString &title, const QString &message)
{
    QMessageBox::information(nullptr, title, message);
}

void Error::showErrorDetails(const QString &title,
                            const QString &message,
                            const QString &details)
{
    QMessageBox msgBox(QMessageBox::Critical, title, message);
    msgBox.setDetailedText(details);
    msgBox.exec();
}

void Error::showValidationError(const QString &message)
{
    QMessageBox::warning(nullptr, tr("Validation Error"), message);
}

} // namespace QVirt
