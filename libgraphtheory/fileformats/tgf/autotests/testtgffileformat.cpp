/*
 *  Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "testtgffileformat.h"
#include "../tgffileformat.h"
#include "fileformats/fileformatinterface.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include <QtTest>

using namespace GraphTheory;

TestTgfFileFormat::TestTgfFileFormat()
{
}

void TestTgfFileFormat::serializeUnserializeTest()
{
    GraphDocumentPtr document = GraphDocument::create();
    document->nodeTypes().first()->addDynamicProperty("label");
    document->edgeTypes().first()->addDynamicProperty("label");
    QMap<QString, NodePtr> dataList;

    // Creates a simple Graph with 5 data elements and connect them with pointers.
    dataList.insert("a", Node::create(document));
    dataList["a"]->setDynamicProperty("label", "first node");
    dataList.insert("b", Node::create(document));
    dataList["b"]->setDynamicProperty("label", "b");
    dataList.insert("c", Node::create(document));
    dataList["c"]->setDynamicProperty("label", "c");
    dataList.insert("d", Node::create(document));
    dataList["d"]->setDynamicProperty("label", "d");
    dataList.insert("e", Node::create(document));
    dataList["e"]->setDynamicProperty("label", "e");

    Edge::create(dataList["a"], dataList["b"])->setDynamicProperty("label", "test value");
    Edge::create(dataList["b"], dataList["c"]);
    Edge::create(dataList["c"], dataList["d"]);
    Edge::create(dataList["d"], dataList["e"]);
    Edge::create(dataList["e"], dataList["a"]);

    // create exporter plugin
    TgfFileFormat serializer(this, QList<QVariant>());
    serializer.setFile(QUrl::fromLocalFile("test.tgf"));
    serializer.writeFile(document);
    QVERIFY(serializer.hasError() == false);

    // create importer
    TgfFileFormat importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("test.tgf"));
    importer.readFile();
    QVERIFY(importer.hasError() == false);
    QVERIFY(importer.isGraphDocument());
    document = importer.graphDocument();

    // test imported values
    QVERIFY2(document->nodes().size() == 5, "ERROR: Number of data is not 5 ");
    QVERIFY2(document->edges().size() == 5, "ERROR: Number of pointers is not 5 ");
    foreach(NodePtr node, document->nodes()) {
        QCOMPARE(node->outEdges().size(), 1);
        QCOMPARE(node->inEdges().size(), 1);
        QCOMPARE(node->edges().count(), 2);
    }
    QCOMPARE(document->nodes().first()->dynamicProperty("label").toString(), QString("first node"));
    QCOMPARE(document->edges().first()->dynamicProperty("label").toString(), QString("test value"));
}

QTEST_MAIN(TestTgfFileFormat);
