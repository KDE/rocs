/*
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2014       Andreas Cord-Landwehr <cordlandwehr@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
    KLocalizedString::setApplicationDomain("rocs");

    KAboutData aboutData("rocs",
                         ki18nc("@title Displayed program name", "Rocs").toString(),
                         ROCS_VERSION_STRING);

    aboutData.addLicense(KAboutLicense::GPL_V2);
    aboutData.setShortDescription(ki18nc("@title Short program description", "Graph Theory Simulator").toString());

    // credits and authors (in alphabetical list)
    aboutData.addCredit(ki18nc("@info:credit", "(c) 2008-2015 Rocs Developers").toString());
    aboutData.addAuthor(ki18nc("@info:credit Developer name", "Andreas Cord-Landwehr").toString(),
                        ki18nc("@info:credit Role", "Maintainer").toString(),
                        "cordlandwehr@kde.org",
                        "http://cordlandwehr.wordpress.com");
    aboutData.addAuthor(ki18nc("@info:credit Developer name", "Tomaz Canabrava").toString(),
                        ki18nc("@info:credit Role", "Original Author").toString(),
                        "tcanabrava@kde.org",
                        "http://liveblue.wordpress.com");
    aboutData.addAuthor(ki18nc("@info:credit Developer name", "Wagner Reck").toString(),
                        ki18nc("@info:credit Role", "Developer").toString(),
                        "wagner.reck@gmail.com",
                        "http://wiglot.wordpress.com");

    QApplication app(argc, argv);
    app.setApplicationVersion(aboutData.version());

    KCrash::initialize();

    /**
     * Create command line parser and feed it with known options
     */
    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.setApplicationDescription(aboutData.shortDescription());
    parser.addHelpOption();
    parser.addVersionOption();

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
