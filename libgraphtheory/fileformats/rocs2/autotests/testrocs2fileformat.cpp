/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#include "testrocs2fileformat.h"
#include "../rocs2fileformat.h"
#include "fileformats/fileformatinterface.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include "edgetypestyle.h"
#include "nodetypestyle.h"
#include <QtTest>

using namespace GraphTheory;

TestRocs2FileFormat::TestRocs2FileFormat()
{
}

// test serialization and import of edge and node types
void TestRocs2FileFormat::documentTypesTest()
{
    GraphDocumentPtr document = GraphDocument::create();

    // setup node type
    document->nodeTypes().first()->setId(1);
    document->nodeTypes().first()->setName("testName");
    document->nodeTypes().first()->style()->setColor(QColor("#ff0000"));
    document->nodeTypes().first()->addDynamicProperty("label");

    // setup edge type
    document->edgeTypes().first()->setId(1);
    document->edgeTypes().first()->setName("testName");
    document->edgeTypes().first()->style()->setColor(QColor("#ff0000"));
    document->edgeTypes().first()->addDynamicProperty("label");
    document->edgeTypes().first()->setDirection(EdgeType::Bidirectional);

    // create exporter plugin
    Rocs2FileFormat serializer(this, QList<QVariant>());
    serializer.setFile(QUrl::fromLocalFile("test.graph2"));
    serializer.writeFile(document);
    QVERIFY(serializer.hasError() == false);

    // create importer
    Rocs2FileFormat importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("test.graph2"));
    importer.readFile();
    QVERIFY(importer.hasError() == false);
    QVERIFY(importer.isGraphDocument());
    GraphDocumentPtr importDocument = importer.graphDocument();

    // test node type
    QCOMPARE(importDocument->nodeTypes().count(), 1);
    QCOMPARE(importDocument->nodeTypes().first()->id(), 1);
    QCOMPARE(importDocument->nodeTypes().first()->name(), QString("testName"));
    QCOMPARE(importDocument->nodeTypes().first()->style()->color().name(), QString("#ff0000"));

    // test edge type
    QCOMPARE(importDocument->edgeTypes().count(), 1);
    QCOMPARE(importDocument->edgeTypes().first()->id(), 1);
    QCOMPARE(importDocument->edgeTypes().first()->name(), QString("testName"));
    QCOMPARE(importDocument->edgeTypes().first()->style()->color().name(), QString("#ff0000"));
    QCOMPARE(importDocument->edgeTypes().first()->direction(), EdgeType::Bidirectional);
}

// test if nodes, edges, and the graph structure itself is re-imported correctly
void TestRocs2FileFormat::nodeAndEdgeTest()
{
    GraphDocumentPtr document = GraphDocument::create();
    document->nodeTypes().first()->addDynamicProperty("label");
    document->nodeTypes().first()->setId(1);
    document->edgeTypes().first()->addDynamicProperty("label");
    document->edgeTypes().first()->setId(1);

    QMap<QString, NodePtr> dataList;
    dataList.insert("a", Node::create(document));
    dataList["a"]->setDynamicProperty("label", "first node");
    dataList["a"]->setId(1);
    dataList["a"]->setX(20);
    dataList["a"]->setY(20);

    dataList.insert("b", Node::create(document));
    dataList["b"]->setDynamicProperty("label", "b");
    dataList["b"]->setId(2);

    Edge::create(dataList["a"], dataList["b"])->setDynamicProperty("label", "test value");

    // create exporter plugin
    Rocs2FileFormat serializer(this, QList<QVariant>());
    serializer.setFile(QUrl::fromLocalFile("test.graph2"));
    serializer.writeFile(document);
    QVERIFY(serializer.hasError() == false);

    // create importer
    Rocs2FileFormat importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("test.graph2"));
    importer.readFile();
    QVERIFY(importer.hasError() == false);
    QVERIFY(importer.isGraphDocument());
    GraphDocumentPtr importDocument = importer.graphDocument();

    // test imported values
    QCOMPARE(importDocument->nodes().size(), 2);

    // find node with ID 1 and then test it
    NodePtr testNode;
    foreach (NodePtr node, importDocument->nodes()) {
        if (node->id() == 1) {
            testNode = node;
        }
    }
    QVERIFY2(testNode, "No node with ID 1 found");
    QCOMPARE(testNode->x(), qreal(20));
    QCOMPARE(testNode->y(), qreal(20));
    QCOMPARE(testNode->dynamicProperty("label").toString(), QString("first node"));

    // test edge
    QCOMPARE(importDocument->edges().count(), 1);
    EdgePtr testEdge = importDocument->edges().first();
    QCOMPARE(testEdge->dynamicProperty("label").toString(), QString("test value"));
    QVERIFY(testEdge->from() == testNode);
}

// regression test: test parsing of format version 1
void TestRocs2FileFormat::parseVersion1Format()
{
    Rocs2FileFormat importer(this, QList<QVariant>());
    importer.setFile(QUrl::fromLocalFile("importtest_v1.graph2"));
    importer.readFile();
    QVERIFY2(importer.hasError() == false, importer.errorString().toStdString().c_str());
}

QTEST_MAIN(TestRocs2FileFormat);
