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

#include "basic.h"
#include "libgraphtheory/graphdocument.h"
#include "libgraphtheory/nodetype.h"
#include "libgraphtheory/edgetype.h"
#include "libgraphtheory/node.h"
#include "libgraphtheory/edge.h"

#include <QTest>

void BasicTest::initTestCase()
{
    QVERIFY(GraphDocument::objects() == 0);
    QVERIFY(Node::objects() == 0);
    QVERIFY(Edge::objects() == 0);
}

void BasicTest::cleanupTestCase()
{
    QVERIFY(GraphDocument::objects() == 0);
    QVERIFY(Node::objects() == 0);
    QVERIFY(Edge::objects() == 0);
}

void BasicTest::testDocumentCreateDelete()
{
    GraphDocumentPtr document;
    NodePtr nodeA, nodeB;
    EdgePtr edge;

    QVERIFY(GraphDocument::objects() == 0);
    QVERIFY(Node::objects() == 0);
    QVERIFY(Edge::objects() == 0);

    // test destroy graph document
    document = GraphDocument::create();
    nodeA = Node::create(document);
    nodeB = Node::create(document);
    edge = Edge::create(nodeA, nodeB);

    document->destroy();
    document.reset();
    nodeA.reset();
    nodeB.reset();
    edge.reset();
    QVERIFY(GraphDocument::objects() == 0);
    QVERIFY(Node::objects() == 0);
    QVERIFY(Edge::objects() == 0);
}

void BasicTest::testNodeCreateDelete()
{
    GraphDocumentPtr document;
    NodePtr nodeA, nodeB;
    EdgePtr edge;

    QVERIFY(GraphDocument::objects() == 0);
    QVERIFY(Node::objects() == 0);
    QVERIFY(Edge::objects() == 0);

    // test destroy graph document
    document = GraphDocument::create();
    nodeA = Node::create(document);
    nodeB = Node::create(document);
    edge = Edge::create(nodeA, nodeB);

    nodeA->destroy();
    nodeA.reset();
    edge.reset();
    QVERIFY(Node::objects() == 1);
    QVERIFY(Edge::objects() == 0);

    document->destroy();
}

void BasicTest::testEdgeCreateDelete()
{
    GraphDocumentPtr document;
    NodePtr nodeA, nodeB;
    EdgePtr edge;

    QVERIFY(GraphDocument::objects() == 0);
    QVERIFY(Node::objects() == 0);
    QVERIFY(Edge::objects() == 0);

    // test destroy graph document
    document = GraphDocument::create();
    nodeA = Node::create(document);
    nodeB = Node::create(document);
    edge = Edge::create(nodeA, nodeB);

    edge->destroy();
    edge.reset();
    QVERIFY(Edge::objects() == 0);

    document->destroy();
}

void BasicTest::testNodeTypeCreateDelete()
{
    GraphDocumentPtr document = GraphDocument::create();
    NodePtr node = Node::create(document);

    NodeTypePtr typeA = document->nodeTypes().first();
    NodeTypePtr typeB = NodeType::create(document);

    // check that 2 created types + default type exist
    QCOMPARE(document->nodeTypes().length(), 2);

    // check changing of node type of a node
    QCOMPARE(node->type(), typeA);
    QVERIFY(document->nodes(typeA).length() == 1);
    QVERIFY(document->nodes(typeB).length() == 0);
    node->setType(typeB);
    QCOMPARE(node->type(), typeB);
    QVERIFY(document->nodes(typeA).length() == 0);
    QVERIFY(document->nodes(typeB).length() == 1);

    document->destroy();
    //TODO test type deletes
}

void BasicTest::testEdgeTypeCreateDelete()
{
    GraphDocumentPtr document = GraphDocument::create();
    NodePtr from = Node::create(document);
    NodePtr to = Node::create(document);
    EdgePtr edge = Edge::create(from, to);

    EdgeTypePtr typeA = document->edgeTypes().first();
    EdgeTypePtr typeB = EdgeType::create(document);

    // check that 2 created types + default type exist
    QCOMPARE(document->edgeTypes().length(), 2);

    // check changing of node type of a node
    QCOMPARE(edge->type(), typeA);
    QCOMPARE(document->edges(typeA).length(), 1);
    QCOMPARE(document->edges(typeB).length(), 0);
    edge->setType(typeB);
    QCOMPARE(edge->type(), typeB);
    QCOMPARE(document->edges(typeA).length(), 0);
    QCOMPARE(document->edges(typeB).length(),  1);

    document->destroy();

    //TODO test type deletes
}

QTEST_MAIN(BasicTest)
