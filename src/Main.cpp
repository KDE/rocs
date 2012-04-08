/*
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>


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

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KDebug>
#include <KLocale>
#include <KLocalizedString>
#include <QByteArray>

#include "rocsversion.h"
#include "Interface/MainWindow.h"
#include "Core/DataStructurePluginManager.h"


int main(int argc, char *argv[])
{
    KAboutData aboutData("rocs",
                         "rocs",
                         ki18n("Rocs"),
                         ROCS_VERSION_STR,
                         ki18n("Graph Theory Tool"),
                         KAboutData::License_GPL,
                         ki18n("(c) 2009-2011 Rocs Developers"),
                         ki18n("Rocs - Data Structure Analysis")
                        );

    aboutData.addAuthor(ki18n("Tomaz Canabrava"), ki18n("Developer"), "tcanabrava@kde.org", "http://liveblue.wordpress.com");
    aboutData.addAuthor(ki18n("Wagner Reck"), ki18n("Developer"), "wagner.reck@gmail.com", "http://wiglot.wordpress.com");
    aboutData.addAuthor(ki18n("Andreas Cord-Landwehr"), ki18n("Developer"), "cola@uni-paderborn.de", "http://cordlandwehr.wordpress.com");

    KCmdLineArgs::init(argc, argv, &aboutData);
    KApplication app;

    if (DataStructurePluginManager::self()->listOfDataStructures().count() == 0) {
        KMessageBox::detailedError(0,
                                   i18n("No Data Structure plugins found on your system. Exiting."),
                                   i18n("Rocs needs at least one data structure plugin to continue.\n"
                                        "Try running kbuildsycoca4 from a console and after execute Rocs again."));

    } else {
        MainWindow *window = new MainWindow();
        window->show();
        return app.exec();
    }
    return 0;
}
