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

#include "test_kernel.h"
#include "libgraphtheory/kernel/kernel.h"
#include "libgraphtheory/graphdocument.h"
#include "libgraphtheory/nodetype.h"
#include "libgraphtheory/edgetype.h"
#include "libgraphtheory/node.h"
#include "libgraphtheory/edge.h"

#include <QTest>

void TestKernel::initTestCase()
{
    QVERIFY(GraphDocument::objects() == 0);
    QVERIFY(Node::objects() == 0);
    QVERIFY(Edge::objects() == 0);
}

void TestKernel::cleanupTestCase()
{
    QVERIFY(GraphDocument::objects() == 0);
    QVERIFY(Node::objects() == 0);
    QVERIFY(Edge::objects() == 0);
}

void TestKernel::engineSetup()
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

    // create kernel
    QString script = "return true;";
    Kernel kernel;
    QScriptValue result = kernel.execute(document, script);
    QCOMPARE(result.toBool(), true);

    document->destroy();
    document.reset();
    nodeA.reset();
    nodeB.reset();
    edge.reset();
    QCOMPARE(Edge::objects(), uint(0));
    QCOMPARE(Node::objects(), uint(0));
    QCOMPARE(GraphDocument::objects(), uint(0));
}

void TestKernel::graphObjectAccess()
{
    GraphDocumentPtr document = GraphDocument::create();
    NodePtr nodeA = Node::create(document);
    NodePtr nodeB = Node::create(document);
    EdgePtr edge = Edge::create(nodeA, nodeB);

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;

    script = "Document.nodes().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(2));

    script = "Document.edges().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    // cleanup
    document->destroy();
}

void TestKernel::edgeAccessMethods()
{
    GraphDocumentPtr document = GraphDocument::create();
    document->edgeTypes().first()->setDirection(EdgeType::Unidirectional);
    NodePtr nodeA = Node::create(document);
    NodePtr nodeB = Node::create(document);
    EdgePtr edge = Edge::create(nodeA, nodeB);

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;

    script = "Document.nodes()[0].edges().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    script = "Document.nodes()[0].inEdges().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(0));

    script = "Document.nodes()[0].outEdges().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    // cleanup
    document->destroy();
}

void TestKernel::nodeProperties()
{
    GraphDocumentPtr document = GraphDocument::create();
    document->edgeTypes().first()->setDirection(EdgeType::Unidirectional);
    NodePtr node = Node::create(document);
    node->setId(1);
    node->setX(20);
    node->setY(30);
    node->setColor("#ff0000");

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;

    script = "Document.nodes()[0].id;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toString().toInt(), node->id());

    script = "Document.nodes()[0].x;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toString().toDouble(), qreal(node->x()));

    script = "Document.nodes()[0].y;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toString().toDouble(), qreal(node->y()));

    script = "Document.nodes()[0].color;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toString(), QString("#ff0000"));

    // cleanup
    document->destroy();
}

void TestKernel::edgeProperties()
{
    GraphDocumentPtr document = GraphDocument::create();
    document->edgeTypes().first()->setDirection(EdgeType::Unidirectional);
    NodePtr nodeA = Node::create(document);
    NodePtr nodeB = Node::create(document);
    EdgePtr edge = Edge::create(nodeA, nodeB);

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;

    script = "Document.nodes()[0].edges()[0].from().id;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toString().toInt(), nodeA->id());

    script = "Document.nodes()[0].edges()[0].to().id;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toString().toInt(), nodeB->id());

    script = "Document.nodes()[0].edges()[0].directed();";
    result = kernel.execute(document, script);
    QCOMPARE(result.toBool(), true);

    // cleanup
    document->destroy();
}

void TestKernel::nodeTypes()
{
    GraphDocumentPtr document = GraphDocument::create();
    document->nodeTypes().first()->setId(1);
    NodeTypePtr typeB = NodeType::create(document);
    typeB->setId(2);
    NodePtr node = Node::create(document);

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;

    script = "Document.nodes()[0].type;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toString().toInt(), 1);

    script = "Document.nodes()[0].type = 2;";
    result = kernel.execute(document, script);
    QCOMPARE(node->type()->id(), 2);

    script = "Document.nodes()[0].type;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toString().toInt(), 2);

    // cleanup
    document->destroy();
}

QTEST_MAIN(TestKernel)
