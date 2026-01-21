/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 The QVirt-Manager Developers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>

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

    // Initialize engine
    auto *engine = QVirt::Engine::instance();
    engine->init();

    // Show main window
    QVirt::ManagerWindow manager;
    manager.show();

    qDebug() << "QVirt Manager starting..."
             << "\n  Qt version:" << QT_VERSION_STR
             << "\n  This is Phase 1 - Foundation";

    return app.exec();
}
