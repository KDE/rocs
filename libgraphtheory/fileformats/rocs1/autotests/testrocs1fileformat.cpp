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
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "testrocs1fileformat.h"
#include "../rocs1fileformat.h"
#include "typenames.h"
#include "fileformats/fileformatinterface.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include <QtTest>

using namespace GraphTheory;

TestRocs1FileFormat::TestRocs1FileFormat()
{

}

void TestRocs1FileFormat::serializeUnserializeTest()
{
    GraphDocumentPtr document = GraphDocument::create();
    QMap<QString, NodePtr> nodes;

    // Creates a simple graph with 5 data elements and connect them with pointers.
    nodes.insert("a", Node::create(document));
    nodes["a"]->setDynamicProperty("label", "first node");
    nodes.insert("b", Node::create(document));
    nodes["b"]->setDynamicProperty("label", "b");
    nodes.insert("c", Node::create(document));
    nodes["c"]->setDynamicProperty("label", "c");
    nodes.insert("d", Node::create(document));
    nodes["d"]->setDynamicProperty("label", "d");
    nodes.insert("e", Node::create(document));
    nodes["e"]->setDynamicProperty("label", "e");

    Edge::create(nodes["a"], nodes["b"])->setDynamicProperty("label", "test value");
    Edge::create(nodes["b"], nodes["c"]);
    Edge::create(nodes["c"], nodes["d"]);
    Edge::create(nodes["d"], nodes["e"]);
    Edge::create(nodes["e"], nodes["a"]);

    // serialize to file
    Rocs1FileFormat format(this, QList<QVariant>());
    format.setFile(QUrl::fromLocalFile("test.graph"));
    format.writeFile(document);
    QVERIFY(format.hasError() == false);

    // unserialize and test properties
    // load document and test
    format.readFile();
    QVERIFY(!format.hasError());
    document = format.graphDocument();

    QCOMPARE(document->nodes().count(), 5);
    QCOMPARE(document->edges().count(), 5);
    foreach(NodePtr node, document->nodes()) {
        QCOMPARE(node->edges().count(), 2);
    }
}

void TestRocs1FileFormat::serializeUnserializeTypesTest()
{
    GraphDocumentPtr document = GraphDocument::create();
    QMap<QString, NodePtr> nodes;
    NodeTypePtr nodeType2 = NodeType::create(document);
    EdgeTypePtr edgeType2 = EdgeType::create(document);

    // add test data
    nodes.insert("a", Node::create(document));
    nodes["a"]->setDynamicProperty("label", "first node");
    nodes.insert("b", Node::create(document));
    nodes["b"]->setDynamicProperty("label", "b");
    Edge::create(nodes["a"], nodes["b"]);

    // serialize to file
    Rocs1FileFormat format(this, QList<QVariant>());
    format.setFile(QUrl::fromLocalFile("testtypes.graph"));
    format.writeFile(document);
    QVERIFY(format.hasError() == false);

    // unserialize and test properties
    // load document and test
    format.readFile();
    QVERIFY(!format.hasError());
    document = format.graphDocument();

    QCOMPARE(document->nodeTypes().count(), 2);
    QCOMPARE(document->edgeTypes().count(), 2);
}

//TODO move to Rocs project file test
// void TestRocs1FileFormat::projectLoadSaveTest()
// {
//     QTemporaryFile temp;
//     temp.setAutoRemove(false);
//     temp.open();
//
//     // prepare project and save
//     Project* testProject = new Project(QUrl::fromLocalFile(temp.fileName()));
//     testProject->setName("new test name");
//     testProject->addCodeFile(QUrl::fromLocalFile("/path/to/code.js"));
//     GraphTheory::GraphDocumentPtr graph = testProject->createGraphDocument();
//     graph->setDocumentUrl(QUrl::fromLocalFile("/path/to/graph.graph"));
//     testProject->writeProjectFile();
//
//     // load project
//     Project* testProject2 = new Project(QUrl::fromLocalFile(temp.fileName()));
//     QVERIFY2(testProject2->name().compare("new test name") == 0, "ERROR: project name changed");
//     QVERIFY(testProject2->codeFiles().count() == 1);
//     QVERIFY2(
//         testProject2->codeFiles().at(0).toLocalFile() == "/path/to/code.js",
//         "ERROR: path of code file changed"
//     );
//     QVERIFY2(
//         false,
// //         testProject2->graphFiles().at(0).toLocalFile() == "/path/to/graph.graph",
//         "ERROR: path of graph file changed"
//     );
//
//     delete testProject;
//     delete testProject2;
// }

QTEST_MAIN(TestRocs1FileFormat)
