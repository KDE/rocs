/***************************************************************************
 *   Copyright (C) 2008 by Tomaz Canabrava and Ugo Sangiori                *
 *   tomaz.canabrava@gmail.com                                             *
 *   ugorox@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KDebug>
#include <KLocale>
#include <KLocalizedString>
#include <QByteArray>

#include "Interface/MainWindow.h"

int main ( int argc, char *argv[] ) {
    KAboutData aboutData ("rocs",
                          "rocs",
                          ki18n ( "Rocs" ),
                          "1.5",
                          ki18n ( "Graph Theory Tool" ),
                          KAboutData::License_GPL,
                          ki18n ( "(c) 2009" ),
                          ki18n ( "Thanks to Ugo Sangiori" ),
                          "",
                          "submit@bugs.kde.org" );
			  
    aboutData.addAuthor(ki18n("Tomaz Canabrava"), ki18n("Developer"), "tcanabrava@kde.org", "http://liveblue.wordpress.com");
    aboutData.addAuthor(ki18n("Wagner Reck"), ki18n("Developer"), "wagner.reck@gmail.com", "http://wiglot.wordpress.com");

    KCmdLineArgs::init ( argc, argv, &aboutData );
    KApplication app;
    
    MainWindow *window = new MainWindow();
    window->show();

    return app.exec();
}
