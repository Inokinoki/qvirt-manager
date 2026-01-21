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

#include "BaseUIObject.h"
#include <QUiLoader>
#include <QFile>
#include <QMessageBox>

namespace QVirt {

BaseUIObject::BaseUIObject(QObject *parent)
    : BaseObject(parent)
{
}

BaseUIObject::~BaseUIObject()
{
    cleanup();
}

void BaseUIObject::setTopLevelWidget(QWidget *widget)
{
    m_topLevel = widget;
}

QWidget *BaseUIObject::loadUi(const QString &uiFile, QWidget *parent)
{
    QString filePath = uiFile;
    if (!filePath.startsWith(":/")) {
        filePath = ":/ui/" + filePath;
    }

    QUiLoader loader;
    QFile file(filePath);

    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::critical(nullptr, tr("UI Load Error"),
                             tr("Failed to open UI file: %1").arg(filePath));
        return nullptr;
    }

    QWidget *widget = loader.load(&file, parent);
    file.close();

    if (widget) {
        m_topLevel = widget;
    } else {
        QMessageBox::critical(nullptr, tr("UI Load Error"),
                             tr("Failed to load UI file: %1").arg(filePath));
    }

    return widget;
}

} // namespace QVirt
