/***************************************************************************
 * main.cpp
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/

#include "testGraph.h"
#include "graph.h"
#include "node.h"
#include "edge.h"
#include "krossBackend.h"
#include "qtScriptBackend.h"
#include "graphDocument.h"
#include <qtest_kde.h>

#include <kross/core/action.h>
#include <kross/core/manager.h>
#include <QVariant>


GraphTests::GraphTests()
        : _graphDocument("untitled", 200, 300) {

}

void GraphTests::init() {
    foreach(Graph *g, _graphDocument) {
        delete g;
    }
    _graphDocument.clear();
}

void GraphTests::createPentagon(bool directed) {
    _graphDocument.addGraph("untitled1");
    _graphDocument[0]->setDirected(directed);
    _graphDocument[0]->setProperty("name","Graph1");
    _graphDocument[0]->addNode("a");
    _graphDocument[0]->addNode("b");
    _graphDocument[0]->addNode("c");
    _graphDocument[0]->addNode("d");
    _graphDocument[0]->addNode("e");

    _graphDocument[0]->addEdge("a", "b");
    _graphDocument[0]->addEdge("b", "c");
    _graphDocument[0]->addEdge("c", "d");
    _graphDocument[0]->addEdge("d", "e");
    _graphDocument[0]->addEdge("e", "a");
}

void GraphTests::create3x3(bool directed) {
    /*   creates a 3x3 graph, play with some properties.... */
    _graphDocument.addGraph("untitled2");
    _graphDocument[0]->setDirected(directed);
    _graphDocument[0]->setProperty("name", "Graph2");
    _graphDocument[0]->addNode("a");
    _graphDocument[0]->addNode("b");
    _graphDocument[0]->addNode("c");
    _graphDocument[0]->addNode("d");
    _graphDocument[0]->addNode("e");
    _graphDocument[0]->addNode("f");
    _graphDocument[0]->addNode("g");
    _graphDocument[0]->addNode("h");
    _graphDocument[0]->addNode("i");

    _graphDocument[0]->addEdge("a", "b");
    _graphDocument[0]->addEdge("b", "c");
    _graphDocument[0]->addEdge("c", "d");
    _graphDocument[0]->addEdge("d", "e");
    _graphDocument[0]->addEdge("e", "f");
    _graphDocument[0]->addEdge("f", "g");
    _graphDocument[0]->addEdge("g", "h");
    _graphDocument[0]->addEdge("h", "a");
    _graphDocument[0]->addEdge("i", "b");
    _graphDocument[0]->addEdge("i", "d");
    _graphDocument[0]->addEdge("i", "f");
    _graphDocument[0]->addEdge("i", "h");
}

void GraphTests::createSimpleGraph() {
    /* Creates a simple Graph with 5 nodes and connects them with edges. */
    createPentagon();

    QVERIFY2( _graphDocument[0]->nodes().size() == 5, "ERROR: Number of nodes is not 5 ");
    QVERIFY2( _graphDocument[0]->edges().size() == 5, "ERROR: Number of edges is not 5 ");


    foreach( Node *n, _graphDocument[0]->nodes() ) {
        QVERIFY2( n->out_edges().size() == 1, "ERROR: Number of out edges is not 1");
        QVERIFY2( n->in_edges().size() == 1, "ERROR: Number of in edges is not 1");
        QVERIFY2( n->adjacent_nodes().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
        QVERIFY2( n->adjacent_edges().size() == 2, "ERRORR: Number of adjacent edges is not 2");
    }

}

void GraphTests::manipulateSimpleGraph() {
    create3x3();

    QVERIFY2( _graphDocument[0]->node("a")->out_edges().size()  == 1, "ERROR: Number of nodes is not 1 ");
    QVERIFY2( _graphDocument[0]->node("a")->in_edges().size()   == 1, "ERROR: Number of edges is not 1 ");
    QVERIFY2( _graphDocument[0]->node("a")->self_edges().size() == 0, "ERROR: Number of nodes is not 0 ");
    QVERIFY2( _graphDocument[0]->node("a")->adjacent_nodes().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument[0]->node("a")->adjacent_edges().size() == 2, "ERRORR: Number of adjacent edges is not 2");

    QVERIFY2( _graphDocument[0]->node("b")->out_edges().size() == 1, "ERROR: Number of nodes is not 1 ");
    QVERIFY2( _graphDocument[0]->node("b")->in_edges().size() == 2, "ERROR: Number of edges is not 1 ");
    QVERIFY2( _graphDocument[0]->node("b")->self_edges().size() == 0, "ERROR: Number of nodes is not 0 ");
    QVERIFY2( _graphDocument[0]->node("b")->adjacent_nodes().size() == 3, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument[0]->node("b")->adjacent_edges().size() == 3, "ERRORR: Number of adjacent edges is not 2");

    QVERIFY2( _graphDocument[0]->node("i")->out_edges().size() == 4, "ERROR: Number of nodes is not 1 ");
    QVERIFY2( _graphDocument[0]->node("i")->in_edges().size() == 0, "ERROR: Number of edges is not 1 ");
    QVERIFY2( _graphDocument[0]->node("i")->self_edges().size() == 0, "ERROR: Number of nodes is not 0 ");
    QVERIFY2( _graphDocument[0]->node("i")->adjacent_nodes().size() == 4, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument[0]->node("i")->adjacent_edges().size() == 4, "ERRORR: Number of adjacent edges is not 2");

    Node *n = _graphDocument[0]->node("i");
    qDebug() << n->property("name");
    _graphDocument[0]->remove(n);

    QVERIFY2( _graphDocument[0]->node("a")->out_edges().size()  == 1, "ERROR: Number of nodes is not 1 ");
    QVERIFY2( _graphDocument[0]->node("a")->in_edges().size()   == 1, "ERROR: Number of edges is not 1 ");
    QVERIFY2( _graphDocument[0]->node("a")->self_edges().size() == 0, "ERROR: Number of nodes is not 0 ");
    QVERIFY2( _graphDocument[0]->node("a")->adjacent_nodes().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument[0]->node("a")->adjacent_edges().size() == 2, "ERRORR: Number of adjacent edges is not 2");

    QVERIFY2( _graphDocument[0]->node("b")->out_edges().size() == 1, "ERROR: Number of nodes is not 1 ");
    QVERIFY2( _graphDocument[0]->node("b")->in_edges().size() == 1, "ERROR: Number of edges is not 1 ");
    QVERIFY2( _graphDocument[0]->node("b")->self_edges().size() == 0, "ERROR: Number of nodes is not 0 ");
    QVERIFY2( _graphDocument[0]->node("b")->adjacent_nodes().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument[0]->node("b")->adjacent_edges().size() == 2, "ERRORR: Number of adjacent edges is not 2");

    QVERIFY2( _graphDocument[0]->node("i") == 0, "ERROR: Node  'i'  was not removed. ");

}

void GraphTests::createDirectedGraph() {
//   /* Creates a simple Graph with 5 nodes and connects them with edges. */
    createPentagon(true);
    QVERIFY2( _graphDocument[0]->nodes().size() == 5, "ERROR: Number of nodes is not 5 ");
    QVERIFY2( _graphDocument[0]->edges().size() == 5, "ERROR: Number of edges is not 5 ");

    foreach( Node *n, _graphDocument[0]->nodes() ) {
        QVERIFY2( n->out_edges().size() == 1, "ERROR: Number of out edges is not 1");
        QVERIFY2( n->in_edges().size() == 1, "ERROR: Number of in edges is not 1");
        QVERIFY2( n->adjacent_nodes().size() == 1, "ERROR: Number of Adjacent Nodes is not 2");
        QVERIFY2( n->adjacent_edges().size() == 1, "ERRORR: Number of adjacent edges is not 2");
    }

}

void GraphTests::manipulateDirectedGraph() {
    /*   creates a 3x3 graph, play with some properties.... */
    create3x3(true);

    QVERIFY2( _graphDocument[0]->node("a")->out_edges().size()  == 1, "ERROR: Number of nodes is not 1 ");
    QVERIFY2( _graphDocument[0]->node("a")->in_edges().size()   == 1, "ERROR: Number of edges is not 1 ");
    QVERIFY2( _graphDocument[0]->node("a")->self_edges().size() == 0, "ERROR: Number of nodes is not 0 ");
    QVERIFY2( _graphDocument[0]->node("a")->adjacent_nodes().size() == 1, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument[0]->node("a")->adjacent_edges().size() == 1, "ERRORR: Number of adjacent edges is not 2");

    QVERIFY2( _graphDocument[0]->node("b")->out_edges().size() == 1, "ERROR: Number of nodes is not 1 ");
    QVERIFY2( _graphDocument[0]->node("b")->in_edges().size() == 2, "ERROR: Number of edges is not 1 ");
    QVERIFY2( _graphDocument[0]->node("b")->self_edges().size() == 0, "ERROR: Number of nodes is not 0 ");
    QVERIFY2( _graphDocument[0]->node("b")->adjacent_nodes().size() == 1, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument[0]->node("b")->adjacent_edges().size() == 1, "ERRORR: Number of adjacent edges is not 2");

    QVERIFY2( _graphDocument[0]->node("i")->out_edges().size() == 4, "ERROR: Number of nodes is not 1 ");
    QVERIFY2( _graphDocument[0]->node("i")->in_edges().size() == 0, "ERROR: Number of edges is not 1 ");
    QVERIFY2( _graphDocument[0]->node("i")->self_edges().size() == 0, "ERROR: Number of nodes is not 0 ");
    QVERIFY2( _graphDocument[0]->node("i")->adjacent_nodes().size() == 4, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument[0]->node("i")->adjacent_edges().size() == 4, "ERRORR: Number of adjacent edges is not 2");

    Node *n = _graphDocument[0]->node("i");
    _graphDocument[0]->remove(n);

    QVERIFY2( _graphDocument[0]->node("a")->out_edges().size()  == 1, "ERROR: Number of nodes is not 1 ");
    QVERIFY2( _graphDocument[0]->node("a")->in_edges().size()   == 1, "ERROR: Number of edges is not 1 ");
    QVERIFY2( _graphDocument[0]->node("a")->self_edges().size() == 0, "ERROR: Number of nodes is not 0 ");
    QVERIFY2( _graphDocument[0]->node("a")->adjacent_nodes().size() == 1, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument[0]->node("a")->adjacent_edges().size() == 1, "ERRORR: Number of adjacent edges is not 2");

    QVERIFY2( _graphDocument[0]->node("b")->out_edges().size() == 1, "ERROR: Number of nodes is not 1 ");
    QVERIFY2( _graphDocument[0]->node("b")->in_edges().size() == 1, "ERROR: Number of edges is not 1 ");
    QVERIFY2( _graphDocument[0]->node("b")->self_edges().size() == 0, "ERROR: Number of nodes is not 0 ");
    QVERIFY2( _graphDocument[0]->node("b")->adjacent_nodes().size() == 1, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument[0]->node("b")->adjacent_edges().size() == 1, "ERRORR: Number of adjacent edges is not 2");

    QVERIFY2( _graphDocument[0]->node("i") == 0, "ERROR: Node  'i'  was not removed. ");

}
/*
void GraphTests::testKrossQtjs(){
//   createPentagon();
//
//   QVariantList graphLists;
//   graphLists.append( QVariant::fromValue( _graphDocument[0] ) );
//
//   KrossBackend *k = new KrossBackend(&graphLists);
//   k->setBackend("javascript");
//   k->loadFile("krossjstests.js");
//
//   Kross::Action *action = k->execute();
//   QVariant result;
// 	result = action->callFunction("testGraphName");
//   QVERIFY2( result.toString() == "Graph1", "ERROR, Name of graph cannot be retrieved.");
// 	result = action->callFunction("testGraphPropertyName");
//   QVERIFY2( result.toString() == "Graph1", "ERROR, Name of graph cannot be retrieved.");

}

void GraphTests::testKrossJs(){
  createPentagon();

  QVariantList graphLists;
  graphLists.append( QVariant::fromValue( _graphDocument[0] ) );

  KrossBackend *k = new KrossBackend(&graphLists);
  k->setBackend("javascript");
  k->loadFile("krossjstests.js");

  Kross::Action *action = k->execute();
  QVariant result;
 //	result = action->callFunction("testGraphName");
 // QVERIFY2( result.toString() == "Graph1", "ERROR, Name of graph cannot be retrieved.");
// 	result = action->callFunction("testGraphPropertyName");
//  QVERIFY2( result.toString() == "Graph1", "ERROR, Name of graph cannot be retrieved.");
	result = action->callFunction("testGraphNodeSize");
	QVERIFY2( result.toString().toAscii()  == "5", "ERROR, size of graph is unknown");
}

void GraphTests::testKrossPy(){
  createPentagon();

  QVariantList graphLists;
  graphLists.append( QVariant::fromValue( _graphDocument[0] ) );

  KrossBackend *k = new KrossBackend(&graphLists);
  k->setBackend("python");
  k->loadFile("krosspytests.py");

  Kross::Action *action = k->execute();
  QVariant result;
	//result = action->callFunction("testGraphPropertyName");
	//QVERIFY2(result.toString().toAscii() == "Graph1", "ERROR: Name of the graph cannot be retrieved.");
	result = action->callFunction("testNumberOfNodes");
	QVERIFY2(result.toInt() == 5, "ERROR: Number of nodes cannot be retrieved.");
	//result = action->callFunction("getANode");
	//QVERIFY2(result.toString().toAscii() == "a", "ERROR: node cannot be retrieved.");

}

void GraphTests::testKrossRb(){
//   QFAIL("Ruby crashes the app");
//   createPentagon();
//
//   QVariantList graphLists;
//   graphLists.append( QVariant::fromValue( _graphDocument[0] ) );
//
//   KrossBackend *k = new KrossBackend(&graphLists);
//   k->setBackend("ruby");
//   k->loadFile("krossrbtests.rb");
//
//   // Kross::Action *action = k->execute();
//   // QVariant result = action->callFunction("getGraphName");
//   // qDebug()  << "Result" << result.toString().toAscii();
//
}*/

void GraphTests::testQtScript() {
    createPentagon();
    QtScriptBackend *engine = new QtScriptBackend(_graphDocument,  0);

    QScriptValue results = engine->evaluate("graphs.length");
    QVERIFY2(results.toNumber() == 1, "Error: number of graphs is not 1.");

    results = engine->evaluate("graphs[0].list_nodes().length");
    QVERIFY2(results.toNumber()  == 5, "Error: number of nodes is not 5");

    results = engine->evaluate("graphs[0].list_edges().length");
    QVERIFY2(results.toNumber()  == 5, "Error: number of nodes is not 5");

    QString script = "";
    script += "var graph = graphs[0]; \n ";
    script += "var node1 = graph.list_nodes()[0]; \n ";
    script += "var node2 = graph.list_nodes()[1]; \n ";
    script += "node1 != node2";
    results = engine->evaluate(script);
    QVERIFY2(results.toBoolean() == true, "ERROR: nodes are equal but they shouldn't be.");

    script.clear();
    script += "var graph = graphs[0]; \n ";
    script += "var node1 = graph.list_nodes()[0]; \n ";
    script += "var node2 = graph.list_nodes()[1]; \n ";
    script += "node1.adj_edges().length";
    results = engine->evaluate(script);
    QVERIFY2( results.toNumber() == 2, results.toString().toAscii());
}

void GraphTests::saveTestFile() {
    createPentagon();
    _graphDocument.saveAsInternalFormat("/home/tomaz/pentagono.rif");
}

void  GraphTests::loadTestFile() {
    _graphDocument.loadFromInternalFormat("/home/tumaix/twographs.rif");

    QVERIFY2( _graphDocument[0]->nodes().size() == 5, "ERROR: Number of nodes is not 5 ");
    QVERIFY2( _graphDocument[0]->edges().size() == 5, "ERROR: Number of edges is not 5 ");

    foreach( Node *n, _graphDocument[0]->nodes() ) {
        QVERIFY2( n->out_edges().size() == 1, "ERROR: Number of out edges is not 1");
        QVERIFY2( n->in_edges().size() == 1, "ERROR: Number of in edges is not 1");
        QVERIFY2( n->adjacent_nodes().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
        QVERIFY2( n->adjacent_edges().size() == 2, "ERRORR: Number of adjacent edges is not 2");
    }


    QVERIFY2( _graphDocument[0]->node("a")->out_edges().size()  == 1, "ERROR: Number of nodes is not 1 ");
    QVERIFY2( _graphDocument[0]->node("a")->in_edges().size()   == 1, "ERROR: Number of edges is not 1 ");
    QVERIFY2( _graphDocument[0]->node("a")->self_edges().size() == 0, "ERROR: Number of nodes is not 0 ");
    QVERIFY2( _graphDocument[0]->node("a")->adjacent_nodes().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument[0]->node("a")->adjacent_edges().size() == 2, "ERRORR: Number of adjacent edges is not 2");

    QVERIFY2( _graphDocument[0]->node("b")->out_edges().size() == 1, "ERROR: Number of nodes is not 1 ");
    QVERIFY2( _graphDocument[0]->node("b")->in_edges().size() == 2, "ERROR: Number of edges is not 1 ");
    QVERIFY2( _graphDocument[0]->node("b")->self_edges().size() == 0, "ERROR: Number of nodes is not 0 ");
    QVERIFY2( _graphDocument[0]->node("b")->adjacent_nodes().size() == 3, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument[0]->node("b")->adjacent_edges().size() == 3, "ERRORR: Number of adjacent edges is not 2");

    QVERIFY2( _graphDocument[0]->node("i")->out_edges().size() == 4, "ERROR: Number of nodes is not 1 ");
    QVERIFY2( _graphDocument[0]->node("i")->in_edges().size() == 0, "ERROR: Number of edges is not 1 ");
    QVERIFY2( _graphDocument[0]->node("i")->self_edges().size() == 0, "ERROR: Number of nodes is not 0 ");
    QVERIFY2( _graphDocument[0]->node("i")->adjacent_nodes().size() == 4, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument[0]->node("i")->adjacent_edges().size() == 4, "ERRORR: Number of adjacent edges is not 2");

    Node *n = _graphDocument[0]->node("i");
    qDebug() << n->property("name");
    _graphDocument[0]->remove(n);

    QVERIFY2( _graphDocument[0]->node("a")->out_edges().size()  == 1, "ERROR: Number of nodes is not 1 ");
    QVERIFY2( _graphDocument[0]->node("a")->in_edges().size()   == 1, "ERROR: Number of edges is not 1 ");
    QVERIFY2( _graphDocument[0]->node("a")->self_edges().size() == 0, "ERROR: Number of nodes is not 0 ");
    QVERIFY2( _graphDocument[0]->node("a")->adjacent_nodes().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument[0]->node("a")->adjacent_edges().size() == 2, "ERRORR: Number of adjacent edges is not 2");

    QVERIFY2( _graphDocument[0]->node("b")->out_edges().size() == 1, "ERROR: Number of nodes is not 1 ");
    QVERIFY2( _graphDocument[0]->node("b")->in_edges().size() == 1, "ERROR: Number of edges is not 1 ");
    QVERIFY2( _graphDocument[0]->node("b")->self_edges().size() == 0, "ERROR: Number of nodes is not 0 ");
    QVERIFY2( _graphDocument[0]->node("b")->adjacent_nodes().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument[0]->node("b")->adjacent_edges().size() == 2, "ERRORR: Number of adjacent edges is not 2");

    QVERIFY2( _graphDocument[0]->node("i") == 0, "ERROR: Node  'i'  was not removed. ");
}

QTEST_KDEMAIN_CORE(GraphTests)
#include "testGraph.moc"
