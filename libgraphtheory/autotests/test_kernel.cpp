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
    kernel.execute(document, script);

    document->destroy();
    document.reset();
    nodeA.reset();
    nodeB.reset();
    edge.reset();
    QCOMPARE(Edge::objects(), uint(0));
    QCOMPARE(Node::objects(), uint(0));
    QCOMPARE(GraphDocument::objects(), uint(0));
}


QTEST_MAIN(TestKernel)
