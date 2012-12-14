/*
    This file is part of Rocs.
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "TestTgfFileFormatPlugin.h"
#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"
#include "DocumentManager.h"
#include <qtest_kde.h>
#include "../TgfFileFormatPlugin.h"


TestTgfFileFormatPlugin::TestTgfFileFormatPlugin()
{
}

void TestTgfFileFormatPlugin::serializeUnserializeTest()
{
    DocumentManager::self().addDocument(new Document("testSerialization"));
    Document* document = DocumentManager::self().activeDocument();
    QMap<QString, DataPtr> dataList;

    // Creates a simple Graph with 5 data elements and connect them with pointers.
    DataStructurePtr ds = document->activeDataStructure();
    ds->setProperty("name", "Graph1");
    dataList.insert("a", ds->addData("first node"));
    dataList.insert("b", ds->addData("b"));
    dataList.insert("c", ds->addData("c"));
    dataList.insert("d", ds->addData("d"));
    dataList.insert("e", ds->addData("e"));

    ds->addPointer(dataList["a"], dataList["b"])->setProperty("value", "test value");
    ds->addPointer(dataList["b"], dataList["c"]);
    ds->addPointer(dataList["c"], dataList["d"]);
    ds->addPointer(dataList["d"], dataList["e"]);
    ds->addPointer(dataList["e"], dataList["a"]);

    // create exporter plugin
    TgfFileFormatPlugin serializer(this, QList<QVariant>());
    serializer.setFile(KUrl::fromLocalFile("test.tgf"));
    serializer.writeFile(*document);
    QVERIFY(serializer.hasError() == false);

    // create importer
    TgfFileFormatPlugin importer(this, QList<QVariant>());
    importer.setFile(KUrl::fromLocalFile("test.tgf"));
    importer.readFile();
    QVERIFY(importer.hasError() == false);
    QVERIFY(importer.isGraphDocument());
    Document* testDoc = importer.graphDocument();
    ds = testDoc->dataStructures().at(0);

    // test imported values
    QVERIFY(ds);
    QVERIFY2(ds->dataList().size() == 5, "ERROR: Number of data is not 5 ");
    QVERIFY2(ds->pointers().size() == 5, "ERROR: Number of pointers is not 5 ");
    foreach(DataPtr n, ds->dataList()) {
        QVERIFY2(n->outPointerList().size() == 1, "ERROR: Number of out pointers is not 1");
        QVERIFY2(n->inPointerList().size() == 1, "ERROR: Number of in pointers is not 1");
        QVERIFY2(n->adjacentDataList().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
        QVERIFY2(n->pointerList().size() == 2, "ERROR: Number of adjacent pointers is not 2");
    }
    QVERIFY(ds->dataList().at(0)->property("name").toString() == "first node");
    QVERIFY(ds->pointers().at(0)->property("value").toString() == "test value");
}

QTEST_MAIN(TestTgfFileFormatPlugin);
