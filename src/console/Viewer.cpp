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

#include "Viewer.h"

namespace QVirt {

Viewer::Viewer(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(640, 480);
}

QSize Viewer::sizeHint() const
{
    return QSize(800, 600);
}

} // namespace QVirt
