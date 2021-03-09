/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2008-2011 Tomaz Canabrava <tomaz.canabrava@gmail.com>
    SPDX-FileCopyrightText: 2008 Ugo Sangiori <ugorox@gmail.com>
    SPDX-FileCopyrightText: 2010 Wagner Reck <wagner.reck@gmail.com>
    SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <QApplication>
#include <KAboutData>
#include <KCrash>
#include <KLocalizedString>
#include <QCommandLineParser>
#include <QDir>

#include "rocsversion.h"
#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("rocs");

    KAboutData aboutData("rocs",
                         ki18nc("@title Displayed program name", "Rocs").toString(),
                         ROCS_VERSION_STRING);

    aboutData.addLicense(KAboutLicense::GPL_V2);
    aboutData.setShortDescription(ki18nc("@title Short program description", "Graph Theory Simulator").toString());

    // credits and authors (in alphabetical list)
    aboutData.addCredit(ki18nc("@info:credit", "(c) 2008-2016 Rocs Developers").toString());
    aboutData.addAuthor(ki18nc("@info:credit Developer name", "Andreas Cord-Landwehr").toString(),
                        ki18nc("@info:credit Role", "Maintainer").toString(),
                        "cordlandwehr@kde.org",
                        "https://cordlandwehr.wordpress.com");
    aboutData.addAuthor(ki18nc("@info:credit Developer name", "Tomaz Canabrava").toString(),
                        ki18nc("@info:credit Role", "Original Author").toString(),
                        "tcanabrava@kde.org",
                        "https://liveblue.wordpress.com");
    aboutData.addAuthor(ki18nc("@info:credit Developer name", "Wagner Reck").toString(),
                        ki18nc("@info:credit Role", "Developer").toString(),
                        "wagner.reck@gmail.com",
                        "https://wiglot.wordpress.com");

    app.setApplicationVersion(aboutData.version());

    KCrash::initialize();

    /**
     * Create command line parser and feed it with known options
     */
    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.setApplicationDescription(aboutData.shortDescription());

    parser.addPositionalArgument(QStringLiteral("project"), i18n("Project to open."), QStringLiteral("[url...]"));

    /**
     * do the command line parsing
     */
    parser.process(app);

    /**
     * handle standard options
     */
    aboutData.processCommandLine(&parser);
    const QStringList projectUrls = parser.positionalArguments();

    KAboutData::setApplicationData(aboutData);
    MainWindow *window = new MainWindow();
    window->show();

    // we can only process the first project
    if (projectUrls.count() > 0) {
        QString path = QDir::current().absoluteFilePath(projectUrls.first());
        window->openProject(QUrl::fromLocalFile(path));
    }

    return app.exec();
}
