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

#include "TestScriptEngine.h"
#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"
#include "Document.h"
#include "DocumentManager.h"
#include "QtScriptBackend.h"

#include <qtest_kde.h>

TestScriptEngine::TestScriptEngine()
{
    DocumentManager::self()->addDocument(new Document("test"));;
}

// simple test if script starts and ends
void TestScriptEngine::simpleStart()
{
    DataStructurePtr ds = DataStructure::create(DocumentManager::self()->activeDocument());
    DocumentManager::self()->activeDocument()->setActiveDataStructure(ds);
    DataList dataList;

    Document* graphDoc = DocumentManager::self()->activeDocument();
    QtScriptBackend *engine = graphDoc->engineBackend();

    engine->setScript(QString("for (var i=0; i<10; i++) ;"), graphDoc);
    //QScriptValue results =
    engine->execute();
//     QVERIFY2(results.toNumber() == 1, "Error: number of graphs is not 1.");
}

// test basic operations on pointers and data elements
void TestScriptEngine::basicOperationsGraph()
{
    Document* graphDoc = DocumentManager::self()->activeDocument();
    QtScriptBackend *engine = graphDoc->engineBackend();
    DataStructurePtr ds;
    QString result;

    // create test data structure
    ds = graphDoc->addDataStructure("test");
    DataPtr a = ds->addData("a");
    DataPtr b = ds->addData("b");
    DataPtr c = ds->addData("c");
    PointerPtr connection = ds->addPointer(a, b);
    int type1 = graphDoc->registerDataType("type1");
    graphDoc->setActiveDataStructure(ds);

    // test name
    engine->setScript(QString("test.name;"), graphDoc);
    result = engine->execute();
    QVERIFY(result == "test");

    // test type
    engine->setScript(QString("test.list_nodes()[0].type();"), graphDoc);
    result = engine->execute();
    QVERIFY(result == QString::number(0));

    // change type
    engine->setScript(QString("var a = test.list_nodes()[0]; a.set_type(%1); a.type();").arg(type1), graphDoc);
    result = engine->execute();
    QVERIFY(result == QString::number(type1));

    // add and remove pointer
    engine->setScript(QString("var x = test.add_edge(test.list_nodes()[1], test.list_nodes()[2]); test.list_edges().length;"), graphDoc);
    result = engine->execute();
    QVERIFY(result == QString::number(2));
    engine->setScript(QString("var x = test.list_edges()[1]; x.remove(); test.list_edges().length;"), graphDoc);
    result = engine->execute();
    QVERIFY(result == QString::number(1));

    // remove data elements
    engine->setScript(QString("test.list_nodes()[0].remove(); test.list_nodes().length;"), graphDoc);
    result = engine->execute();
    QVERIFY(result == QString::number(2));
}

// tests if stop action stops execution
void TestScriptEngine::startStop()
{
    //FIXME; for 4.10 add threading support for this test
//     DataStructurePtr ds = DataStructure::create(DocumentManager::self()->activeDocument());
//     DocumentManager::self()->activeDocument()->setActiveDataStructure(ds);
//     DataList dataList;
//
//     Document* graphDoc = DocumentManager::self()->activeDocument();
//     QtScriptBackend *engine = graphDoc->engineBackend();
//
//     // start infinite loop
//     engine->setScript(QString("while (true) ;"), graphDoc);
//     engine->execute();
//
//     // stop execution
//     engine->stop();
}


void TestScriptEngine::ignoreComments()
{
    Document* graphDoc = DocumentManager::self()->activeDocument();
    QtScriptBackend *engine = graphDoc->engineBackend();

    QString test;
    QScriptValue result;

    DataStructurePtr ds = DataStructure::create(DocumentManager::self()->activeDocument());
    DocumentManager::self()->activeDocument()->setActiveDataStructure(ds);
    DataList dataList;

    // start engine
    test = QString("// broken");
    result = engine->engine()->evaluate(test);
    QVERIFY2(!result.toString().contains("Error"), "Comment was not ignored, script with syntax error was executed.");
    result = engine->engine()->evaluate(engine->includeManager().include(test));
    QVERIFY2(!result.toString().contains("Error"), "Comment in file with processed includes was not ignored, script with syntax error was executed.");

    test = QString("//broken");
    result = engine->engine()->evaluate(test);
    QVERIFY2(!result.toString().contains("Error"), "Comment was not ignored, script with syntax error was executed.");
    result = engine->engine()->evaluate(engine->includeManager().include(test));
    QVERIFY2(!result.toString().contains("Error"), "Comment in file with processed includes was not ignored, script with syntax error was executed.");

    test = QString("/* broken */");
    result = engine->engine()->evaluate(test);
    QVERIFY2(!result.toString().contains("Error"), "Comment was not ignored, script with syntax error was executed.");
    result = engine->engine()->evaluate(engine->includeManager().include(test));
    QVERIFY2(!result.toString().contains("Error"), "Comment in file with processed includes was not ignored, script with syntax error was executed.");

    test = QString("/*\n broken\n */");
    result = engine->engine()->evaluate(test);
    QVERIFY2(!result.toString().contains("Error"), "Comment was not ignored, script with syntax error was executed.");
    result = engine->engine()->evaluate(engine->includeManager().include(test));
    QVERIFY2(!result.toString().contains("Error"), "Comment in file with processed includes was not ignored, script with syntax error was executed.");

    test = QString("/**\n * broken\n */");
    result = engine->engine()->evaluate(test);
    QVERIFY2(!result.toString().contains("Error"), "Comment was not ignored, script with syntax error was executed.");
    result = engine->engine()->evaluate(engine->includeManager().include(test));
    QVERIFY2(!result.toString().contains("Error"), "Comment in file with processed includes was not ignored, script with syntax error was executed.");
}

void TestScriptEngine::useOfDynamicProperties()
{
    DocumentManager::self()->addDocument(new Document("test"));
    DataStructurePtr ds = DataStructure::create(DocumentManager::self()->activeDocument());
    DocumentManager::self()->activeDocument()->setActiveDataStructure(ds);
    Document* graphDoc = DocumentManager::self()->activeDocument();

    DataStructurePtr d = graphDoc->activeDataStructure();
    d->setName("myGraph");
    DataPtr n1 =d->addData("Node1");
    DataPtr n2 = d->addData("Node2");
    PointerPtr e1 = d->addPointer(n1, n2);

    QString test;
    QScriptValue result;

    // start engine
    test = QString("nodes = myGraph.list_nodes()\n"
           "edges = myGraph.list_edges()\n"
           "for (var i = 0; i < nodes.length; ++i ){\n"
           "    nodes[i].add_property(\"MyProperty\", 0)\n"
           "}\n"
           "for (var i = 0; i < edges.length; ++i ){\n"
           "    edges[i].add_property(\"MyProperty\", 0)\n"
           "}\n"
           "nodes[0].MyProperty = 2\n"
           "nodes[1].MyProperty = nodes[0].MyProperty + 2\n"
           "edges[0].MyProperty = 2\n"
           "myGraph.add_property(\"MyProperty\", 2)\n");

    QtScriptBackend *engine = graphDoc->engineBackend();

    engine->setScript(test, graphDoc);
    engine->execute();

    QVERIFY2 (n1->property("MyProperty").isValid(), "Node 1 property is invalid!");
    QVERIFY2 (n2->property("MyProperty").isValid(), "Node 2 property is invalid!");
    QVERIFY2 (e1->property("MyProperty").isValid(), "Edge 1 property is invalid!");
    QVERIFY2 (d->property("MyProperty").isValid(), "DS property is invalid!");

    QCOMPARE (n1->property("MyProperty").toInt(), 2);
    QCOMPARE (n2->property("MyProperty").toInt(), 4);
    QCOMPARE (e1->property("MyProperty").toInt(), 2);
    QCOMPARE (d->property("MyProperty").toInt(), 2);
}


QTEST_KDEMAIN_CORE(TestScriptEngine)
