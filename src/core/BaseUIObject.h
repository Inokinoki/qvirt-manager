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

#ifndef QVIRT_CORE_BASEUIOBJECT_H
#define QVIRT_CORE_BASEUIOBJECT_H

#include "BaseObject.h"
#include <QWidget>
#include <QString>
#include <QPointer>

namespace QVirt {

/**
 * @brief Base class for UI objects
 *
 * This class extends BaseObject with UI-specific functionality:
 * - Qt UI file loading from resources
 * - Top-level widget tracking
 * - Automatic cleanup of UI elements
 *
 * Mirrors the Python vmmGObjectUI class from virt-manager
 */
class BaseUIObject : public BaseObject
{
    Q_OBJECT

public:
    explicit BaseUIObject(QObject *parent = nullptr);
    virtual ~BaseUIObject();

    /**
     * @brief Get the top-level widget for this UI object
     * @return Top-level widget, or nullptr if not set
     */
    QWidget *topLevelWidget() const { return m_topLevel.data(); }

    /**
     * @brief Set the top-level widget
     * @param widget Widget to set as top-level
     */
    void setTopLevelWidget(QWidget *widget);

protected:
    /**
     * @brief Load a UI file from resources
     * @param uiFile Path to UI file (with or without :/ prefix)
     * @param parent Parent widget for loaded UI
     * @return Loaded widget, or nullptr on failure
     */
    QWidget *loadUi(const QString &uiFile, QWidget *parent = nullptr);

    QPointer<QWidget> m_topLevel;
};

} // namespace QVirt

#endif // QVIRT_CORE_BASEUIOBJECT_H
