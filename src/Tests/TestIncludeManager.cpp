/*
    This file is part of Rocs.
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>

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
#include <QtTest/QTest>
#include <QtCore/QFile>
#include "IncludeManager.h"
#include <KLocalizedString>
#include <KDebug>

void TestIncludeManager::inittestcase() {
    IncludeManager man;
    QFile f(QDir::currentPath()+"/file.js");
    if ( f.open(QFile::WriteOnly) ){
        QTextStream tx(&f);
        tx << "include (otherFile.js)\n";
        tx << "include (file.js)";
    }
    f.close();
}


void TestIncludeManager::includeSimple() {
    IncludeManager man;
    QString str = QString("Not include file.js");
    QCOMPARE(man.include(str), str);
}



void TestIncludeManager::appendToTempPath() {
    IncludeManager man;
    QString path1("/home/something/");
    QString path2("c:/somePlace/in/Path");
    man.addPath(path1);
    man.addPath(path2 );

    man.include(QString ());
    QVERIFY(man.tempPath().contains(path1));
//     kDebug () << man.tempPath();
    QVERIFY(man.tempPath().contains(path2+"/"));
}



void TestIncludeManager::doublePathInsert() {
    IncludeManager man;
    QStringList inserted1;
    inserted1 << "/home/something/";
    inserted1 << "c:\\somePlace\\in\\Path";
    man.addPath(inserted1);
    QStringList inserted2;
    inserted2 << "/home/something";
    man.addPath(inserted2);

    QCOMPARE(man.tempPath().count(inserted1.at(0)), 1);
    man.addPath(inserted2.at(0));
    QCOMPARE(man.tempPath().count(inserted1.at(0)), 1);

}


//FIXME not working test
void TestIncludeManager::findInclude() {
//     IncludeManager man;
//     QString str = "include ( file.js)";
// 
//     man.addPath(QDir::currentPath());
// 
//     QCOMPARE (man.include(str,QDir::currentPath(), "file.js"), QString("debug(\"Cannot open file %1.\")\n").arg("otherFile.js"));
}

void TestIncludeManager::ignoreComments()
{
    IncludeManager man;
    man.addPath(QDir::currentPath());
    QString str = "//include ( file.js)";
    QCOMPARE (man.include(str), QString(""));
    str = "/*include (file.js)*/";
    QCOMPARE (man.include(str), QString(""));
    str = "/*include (file.js)\n\n*/";
    QCOMPARE (man.include(str), QString(""));
}



QTEST_MAIN ( TestIncludeManager )
