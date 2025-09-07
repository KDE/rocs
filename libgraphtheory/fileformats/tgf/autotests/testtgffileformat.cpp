// SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "testtgffileformat.h"
#include "../tgffileformat.h"
#include "edge.h"
#include "fileformats/fileformatinterface.h"
#include "graphdocument.h"
#include "node.h"
#include <QTest>

using namespace GraphTheory;

void TestTgfFileFormat::serializeUnserializeTest()
{
    GraphDocumentPtr document = GraphDocument::create();
    QCOMPARE(document->edgeTypes().first()->direction(), EdgeType::Direction::Unidirectional);

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
    TgfFileFormat serializer;
    serializer.setFile(QUrl::fromLocalFile("test.tgf"));
    serializer.writeFile(document);
    QVERIFY(serializer.hasError() == false);

    // create importer
    TgfFileFormat importer;
    importer.setFile(QUrl::fromLocalFile("test.tgf"));
    importer.readFile();
    QVERIFY(importer.hasError() == false);
    QVERIFY(importer.isGraphDocument());
    document = importer.graphDocument();

    // test imported values
    QVERIFY2(document->nodes().size() == 5, "ERROR: Number of data is not 5 ");
    QVERIFY2(document->edges().size() == 5, "ERROR: Number of pointers is not 5 ");
    const auto documentNodes = document->nodes();
    for (const NodePtr &node : documentNodes) {
        qDebug() << "Node" << node->dynamicProperty("label");
        for (const auto &e : node->outEdges()) {
            qDebug() << ".. out" << e->from()->dynamicProperty("label") << e->to()->dynamicProperty("label");
        }
        for (const auto &e : node->inEdges()) {
            qDebug() << ".. in " << e->from()->dynamicProperty("label") << e->to()->dynamicProperty("label");
        }
        QCOMPARE(node->outEdges().size(), 1);
        QCOMPARE(node->inEdges().size(), 1);
        QCOMPARE(node->edges().count(), 2);
    }
    QCOMPARE(document->nodes().first()->dynamicProperty("label").toString(), QString("first node"));
    QCOMPARE(document->edges().first()->dynamicProperty("label").toString(), QString("test value"));
}

QTEST_MAIN(TestTgfFileFormat);

#include "moc_testtgffileformat.cpp"
