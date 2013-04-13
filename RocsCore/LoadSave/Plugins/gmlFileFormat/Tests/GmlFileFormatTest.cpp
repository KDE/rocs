/*
    This file is part of Rocs.
    Copyright 2010  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "GmlFileFormatTest.h"
#include "../GmlGrammar.h"
#include "../GmlFileFormatPlugin.h"
#include <QTest>
#include "Document.h"
#include "Pointer.h"
#include "DataStructure.h"
#include "DocumentManager.h"
#include <KDebug>
#include <KUrl>

void GmlFileFormatTest::parseTest()
{
    // create importer plugin
    GmlFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("example.gml"));
    importer.readFile();
    QVERIFY(importer.hasError() == false);
}

void GmlFileFormatTest::serializeTest()
{
    DocumentManager::self().addDocument(new Document("testSerialization"));
    Document* document = DocumentManager::self().activeDocument();
    QMap<QString, DataPtr> dataList;

    // Creates a simple Graph with 5 data elements and connect them with pointers.
    DataStructurePtr ds = document->activeDataStructure();
    ds->setProperty("name", "Graph1");
    dataList.insert("a", ds->createData("first node", 0));
    dataList.insert("b", ds->createData("b", 0));
    dataList.insert("c", ds->createData("c", 0));
    dataList.insert("d", ds->createData("d", 0));
    dataList.insert("e", ds->createData("e", 0));

    ds->createPointer(dataList["a"], dataList["b"], 0)->setProperty("value", "test value");
    ds->createPointer(dataList["b"], dataList["c"], 0);
    ds->createPointer(dataList["c"], dataList["d"], 0);
    ds->createPointer(dataList["d"], dataList["e"], 0);
    ds->createPointer(dataList["e"], dataList["a"], 0);

    // create exporter plugin
    GmlFileFormatPlugin serializer(this, QList<QVariant>());
    serializer.setFile(KUrl::fromLocalFile("test.gml"));
    serializer.writeFile(*document);
    QVERIFY(serializer.hasError() == false);
}

QTEST_MAIN(GmlFileFormatTest)
