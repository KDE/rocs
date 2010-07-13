/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "TestIncludeManager.h"
#include <QtTest>
#include <IncludeManager.h>
#include <KLocalizedString>
#include <QFile>

void TestIncludeManager::inittestcase() {
    IncludeManager man;
    QFile f("file.js");
    if ( f.open(QFile::WriteOnly) ){
        QTextStream tx(&f);
        tx << "include (otherFile.js)\n";
        tx << "include (file.js)";
    }
}


void TestIncludeManager::includeSimple() {
    IncludeManager man;
    QString str = QString("Not include file.js");
    QCOMPARE(man.include(str), str);
}



void TestIncludeManager::appendToTempPath() {
    IncludeManager man;
    QStringList inserted;
    QString path1("/home/something");
    QString path2("c:\\somePlace\\in\\Path");
    man.addPath(path1);
    man.addPath(path2 );
    QString str;
    man.include(str);
    QCOMPARE(man.tempPath(), inserted);
}



void TestIncludeManager::doublePathInsert() {
    IncludeManager man;
    QStringList inserted1;
    inserted1 << "/home/something";
    inserted1 << "c:\\somePlace\\in\\Path";
    man.addPath(inserted1);
    QStringList inserted2;
    inserted2 << "/home/something";
    man.addPath(inserted2);

    QCOMPARE(man.tempPath(), inserted1);
}


void TestIncludeManager::findInclude() {
    IncludeManager man;
    QString str = "include ( file.js)";

    QRegExp r("^\\s*include\\s*\\(\\s*\\w*.js\\s*\\)");
    QWARN(QString::number(str.indexOf(r)).toAscii());
    QWARN(r.cap().toAscii());
    QCOMPARE (man.include(str), i18n("debug(\"Cannot open file %1.\")\n").arg("otherFile.js"));
}




QTEST_MAIN ( TestIncludeManager )