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

#ifndef QVIRT_CORE_ERROR_H
#define QVIRT_CORE_ERROR_H

#include <QObject>
#include <QString>

namespace QVirt {

/**
 * @brief Error dialog utilities
 *
 * Provides static methods for showing various error dialogs
 *
 * Mirrors the Python vmmErrorDialog class from virt-manager
 */
class Error : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Show a simple error message
     * @param title Dialog title
     * @param message Error message
     */
    static void showError(const QString &title, const QString &message);

    /**
     * @brief Show a warning message
     * @param title Dialog title
     * @param message Warning message
     */
    static void showWarning(const QString &title, const QString &message);

    /**
     * @brief Show an info message
     * @param title Dialog title
     * @param message Info message
     */
    static void showInfo(const QString &title, const QString &message);

    /**
     * @brief Show an error with details
     * @param title Dialog title
     * @param message Summary message
     * @param details Detailed information
     */
    static void showErrorDetails(const QString &title,
                                const QString &message,
                                const QString &details);

    /**
     * @brief Show a validation error
     * @param message Validation error message
     */
    static void showValidationError(const QString &message);
};

} // namespace QVirt

#endif // QVIRT_CORE_ERROR_H
