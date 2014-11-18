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

void TestKernel::graphObjectAccessWithTypes()
{
    GraphDocumentPtr document = GraphDocument::create();
    NodeTypePtr nodeTypeB = NodeType::create(document);
    EdgeTypePtr edgeTypeB = EdgeType::create(document);
    NodePtr nodeA = Node::create(document);
    NodePtr nodeB = Node::create(document);
    NodePtr nodeC = Node::create(document);
    EdgePtr edgeAB = Edge::create(nodeA, nodeB);
    EdgePtr edgeBC = Edge::create(nodeB, nodeC);

    // setup types
    nodeA->setType(nodeTypeB);
    edgeAB->setType(edgeTypeB);

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;

    script = QString("Document.nodes(%1).length;").arg(nodeTypeB->id());
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    script = QString("Document.edges(%1).length;").arg(edgeTypeB->id());
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    // cleanup
    document->destroy();
}

void TestKernel::nodeAccessMethods()
{
    GraphDocumentPtr document = GraphDocument::create();
    NodePtr node = Node::create(document);
    node->setId(42);

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;

    script = "Document.node(42).id;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(42));

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

void TestKernel::edgeAccessMethodsWithTypes()
{
    GraphDocumentPtr document = GraphDocument::create();
    document->edgeTypes().first()->setDirection(EdgeType::Bidirectional);
    NodePtr nodeA = Node::create(document);
    NodePtr nodeB = Node::create(document);
    NodePtr nodeC = Node::create(document);
    EdgePtr edgeAB = Edge::create(nodeA, nodeB);
    EdgePtr edgeBC = Edge::create(nodeB, nodeC);

    // test edge type
    EdgeTypePtr edgeTypeB = EdgeType::create(document);
    edgeTypeB->setDirection(EdgeType::Unidirectional);
    edgeBC->setType(edgeTypeB);

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;

    script = QString("Document.nodes()[1].edges(%1).length;").arg(edgeTypeB->id());
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    script = QString("Document.nodes()[1].inEdges(%1).length;").arg(edgeTypeB->id());
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(0));

    script = QString("Document.nodes()[1].outEdges(%1).length;").arg(edgeTypeB->id());
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

void TestKernel::nodeDynamicProperties()
{
    GraphDocumentPtr document = GraphDocument::create();
    NodeTypePtr type = document->nodeTypes().first();
    NodePtr node = Node::create(document);

    type->addDynamicProperty("propertyA");
    type->addDynamicProperty("propertyB");
    type->addDynamicProperty("propertyC");

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;

    // property read-access from script
    node->setDynamicProperty("propertyA", "1");
    script = "Document.nodes()[0].propertyA;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toString().toInt(), 1);

    // property local write/read-access in script
    script = "Document.nodes()[0].propertyB = 2; Document.nodes()[0].propertyB";
    result = kernel.execute(document, script);
    QCOMPARE(node->dynamicProperty("propertyB").toInt(), 2);

    // property write-access from script
    script = "Document.nodes()[0].propertyC = 3";
    result = kernel.execute(document, script);
    QCOMPARE(node->dynamicProperty("propertyC").toInt(), 3);

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

void TestKernel::edgeDynamicProperties()
{
    GraphDocumentPtr document = GraphDocument::create();
    EdgeTypePtr type = document->edgeTypes().first();
    NodePtr nodeA = Node::create(document);
    NodePtr nodeB = Node::create(document);
    EdgePtr edge = Edge::create(nodeA, nodeB);

    type->addDynamicProperty("propertyA");
    type->addDynamicProperty("propertyB");
    type->addDynamicProperty("propertyC");

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;

    // property read-access from script
    edge->setDynamicProperty("propertyA", "1");
    script = "Document.edges()[0].propertyA;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toString().toInt(), 1);

    // property local write/read-access in script
    script = "Document.edges()[0].propertyB = 2; Document.edges()[0].propertyB";
    result = kernel.execute(document, script);
    QCOMPARE(edge->dynamicProperty("propertyB").toInt(), 2);

    // property write-access from script
    script = "Document.edges()[0].propertyC = 3";
    result = kernel.execute(document, script);
    QCOMPARE(edge->dynamicProperty("propertyC").toInt(), 3);

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

void TestKernel::edgeTypes()
{
    GraphDocumentPtr document = GraphDocument::create();
    document->edgeTypes().first()->setId(1);
    EdgeTypePtr typeB = EdgeType::create(document);
    typeB->setId(2);
    NodePtr nodeA = Node::create(document);
    NodePtr nodeB = Node::create(document);
    EdgePtr edge = Edge::create(nodeA, nodeB);

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;

    script = "Document.edges()[0].type;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toString().toInt(), 1);

    script = "Document.edges()[0].type = 2;";
    result = kernel.execute(document, script);
    QCOMPARE(edge->type()->id(), 2);

    script = "Document.edges()[0].type;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toString().toInt(), 2);

    // cleanup
    document->destroy();
}

void TestKernel::neighborships()
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

    // test with unidirectional edge
    script = "Document.nodes()[0].neighbors().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    script = "Document.nodes()[1].neighbors().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    script = "Document.nodes()[0].successors().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    script = "Document.nodes()[1].successors().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(0));

    script = "Document.nodes()[0].predecessors().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(0));

    script = "Document.nodes()[1].predecessors().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    // test with bidirectional edge
    document->edgeTypes().first()->setDirection(EdgeType::Bidirectional);
    script = "Document.nodes()[0].neighbors().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    script = "Document.nodes()[1].neighbors().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    script = "Document.nodes()[0].successors().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    script = "Document.nodes()[1].successors().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    script = "Document.nodes()[0].predecessors().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    script = "Document.nodes()[1].predecessors().length;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(1));

    // cleanup
    document->destroy();
}

void TestKernel::automaticScriptObjectPropertyGeneration()
{
    GraphDocumentPtr document = GraphDocument::create();
    NodePtr nodeA = Node::create(document);
    NodePtr nodeB = Node::create(document);
    EdgePtr edge = Edge::create(nodeA, nodeB);

    Kernel kernel;
    QString script;
    QScriptValue result;

    // For edges/nodes we can assign arbitrary dynamic properties during
    // script exection. However, they exist only during execution and are removed
    // at the end of the execution.
    script = "Document.nodes()[0].nonRegProp=1; Document.nodes()[0].nonRegProp;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toString().toInt(), 1);
    QCOMPARE(nodeA->dynamicProperty("nonRegProp").toInt(), 0);

    script = "Document.edges()[0].nonRegProp=1; Document.edges()[0].nonRegProp;";
    result = kernel.execute(document, script);
    QCOMPARE(result.toString().toInt(), 1);
    QCOMPARE(edge->dynamicProperty("nonRegProp").toInt(), 0);

    // cleanup
    document->destroy();
}

void TestKernel::createNode()
{
    GraphDocumentPtr document = GraphDocument::create();

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;

    script = "Document.createNode(0, 0);";
    result = kernel.execute(document, script);
    QCOMPARE(document->nodes().count(), 1);

    // cleanup
    document->destroy();
}

void TestKernel::createEdge()
{
    GraphDocumentPtr document = GraphDocument::create();
    NodePtr nodeA = Node::create(document);
    NodePtr nodeB = Node::create(document);

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;

    script = "Document.createEdge(Document.nodes()[0], Document.nodes()[1]);";
    result = kernel.execute(document, script);
    QCOMPARE(document->edges().count(), 1);

    // cleanup
    document->destroy();
}

void TestKernel::deleteNode()
{
    GraphDocumentPtr document = GraphDocument::create();
    Node::create(document);

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;
    QCOMPARE(document->nodes().count(), 1);

    script = "Document.remove(Document.nodes()[0]);";
    result = kernel.execute(document, script);
    QCOMPARE(document->nodes().count(), 0);

    // cleanup
    document->destroy();
}

void TestKernel::deleteEdge()
{
    GraphDocumentPtr document = GraphDocument::create();
    NodePtr nodeA = Node::create(document);
    NodePtr nodeB = Node::create(document);
    Edge::create(nodeA, nodeB);

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;
    QCOMPARE(document->edges().count(), 1);

    script = "Document.remove(Document.edges()[0]);";
    result = kernel.execute(document, script);
    QCOMPARE(document->edges().count(), 0);

    // cleanup
    document->destroy();
}

void TestKernel::distance()
{
    GraphDocumentPtr document = GraphDocument::create();
    NodePtr nodeA = Node::create(document);
    NodePtr nodeB = Node::create(document);
    EdgePtr edge = Edge::create(nodeA, nodeB);
    document->edgeTypes().first()->addDynamicProperty("dist");

    edge->setDynamicProperty("dist", "42");

    // test nodes
    Kernel kernel;
    QString script;
    QScriptValue result;

    script = "Document.nodes()[0].distance(\"dist\", Document.nodes())[1];";
    result = kernel.execute(document, script);
    QCOMPARE(result.toInteger(), qreal(42));

    // cleanup
    document->destroy();
}

QTEST_MAIN(TestKernel)
