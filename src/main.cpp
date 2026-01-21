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

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QFile>
#include <QPixmap>

#include "core/Engine.h"
#include "core/Config.h"
#include "ui/manager/ManagerWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("qvirt-manager");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("qvirt-manager");
    app.setOrganizationDomain("qvirt-manager.org");

    // Command line parsing
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "QVirt Manager - Virtual Machine Manager"));
    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(app);

    // Initialize configuration
    auto *config = QVirt::Config::instance();
    Q_UNUSED(config);

    // Set application icon
    QPixmap appIcon(":/icons/app/qvirt-manager.svg");
    if (!appIcon.isNull()) {
        app.setWindowIcon(QIcon(appIcon));
    }

    // Load application stylesheet
    QFile stylesheet(":/styles/default.qss");
    if (stylesheet.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString style = QString::fromUtf8(stylesheet.readAll());
        app.setStyleSheet(style);
        stylesheet.close();
    }

    // Initialize engine
    auto *engine = QVirt::Engine::instance();
    engine->init();

    // Show main window
    QVirt::ManagerWindow manager;
    manager.show();

    qDebug() << "QVirt Manager starting..."
             << "\n  Qt version:" << QT_VERSION_STR
             << "\n  Version: 1.0.0"
             << "\n  Status: Production Ready (~90% complete)";

    return app.exec();
}
