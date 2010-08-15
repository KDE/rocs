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
#include "DataType.h"
#include "Data.h"
#include "Pointer.h"
#include "krossBackend.h"
#include "qtScriptBackend.h"
#include <qtest_kde.h>

#include <kross/core/action.h>
#include <kross/core/manager.h>
#include <graphDocument.h>
#include <DSPluginManager.h>

using namespace Rocs;


GraphTests::GraphTests(){

}

void GraphTests::initTestCase(){

QVERIFY(DSPluginManager::instance()->pluginsList().count( )  > 0);
_graphDocument = new DataTypeDocument("untitled");
}


void GraphTests::cleanupTestCase(){
  _graphDocument->deleteLater();
}
void GraphTests::cleanup(){
  for (int i = 0 ; i < _graphDocument->count(); ++i){
//     foreach(DataType *g, _graphDocument) {
        _graphDocument->at(i)->deleteLater();;
    }
    _graphDocument->clear();
}

void GraphTests::createPentagon(bool directed) {
    _graphDocument->addDataType("untitled1");
    _graphDocument->at(0)->setDirected(directed);
    _graphDocument->at(0)->setProperty("name","Graph1");
    _graphDocument->at(0)->addDatum("a");
    _graphDocument->at(0)->addDatum("b");
    _graphDocument->at(0)->addDatum("c");
    _graphDocument->at(0)->addDatum("d");
    _graphDocument->at(0)->addDatum("e");

    _graphDocument->at(0)->addPointer("a", "b");
    _graphDocument->at(0)->addPointer("b", "c");
    _graphDocument->at(0)->addPointer("c", "d");
    _graphDocument->at(0)->addPointer("d", "e");
    _graphDocument->at(0)->addPointer("e", "a");
}

void GraphTests::create3x3(bool directed) {
    /*   creates a 3x3 graph, play with some properties.... */
    _graphDocument->addDataType("untitled2");
    _graphDocument->at(0)->setDirected(directed);
    _graphDocument->at(0)->setProperty("name", "Graph2");
    _graphDocument->at(0)->addDatum("a");
    _graphDocument->at(0)->addDatum("b");
    _graphDocument->at(0)->addDatum("c");
    _graphDocument->at(0)->addDatum("d");
    _graphDocument->at(0)->addDatum("e");
    _graphDocument->at(0)->addDatum("f");
    _graphDocument->at(0)->addDatum("g");
    _graphDocument->at(0)->addDatum("h");
    _graphDocument->at(0)->addDatum("i");

    _graphDocument->at(0)->addPointer("a", "b");
    _graphDocument->at(0)->addPointer("b", "c");
    _graphDocument->at(0)->addPointer("c", "d");
    _graphDocument->at(0)->addPointer("d", "e");
    _graphDocument->at(0)->addPointer("e", "f");
    _graphDocument->at(0)->addPointer("f", "g");
    _graphDocument->at(0)->addPointer("g", "h");
    _graphDocument->at(0)->addPointer("h", "a");
    _graphDocument->at(0)->addPointer("i", "b");
    _graphDocument->at(0)->addPointer("i", "d");
    _graphDocument->at(0)->addPointer("i", "f");
    _graphDocument->at(0)->addPointer("i", "h");
}

void GraphTests::createSimpleGraph() {
    /* Creates a simple Graph with 5 datums and connects them with pointers. */
    createPentagon();

    QVERIFY2( _graphDocument->at(0)->data().size() == 5, "ERROR: Number of data is not 5 ");
    QVERIFY2( _graphDocument->at(0)->pointers().size() == 5, "ERROR: Number of pointers is not 5 ");




    foreach( Datum *n, _graphDocument->at(0)->data() ) {
        QVERIFY2( n->out_pointers().size() == 1, "ERROR: Number of out pointers is not 1");
        QVERIFY2( n->in_pointers().size() == 1, "ERROR: Number of in pointers is not 1");
        QVERIFY2( n->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
        QVERIFY2( n->adjacent_pointers().size() == 2, "ERRORR: Number of adjacent pointers is not 2");
    }
}

void GraphTests::manipulateSimpleGraph() {
    create3x3();

    QVERIFY2( _graphDocument->at(0)->datum("a")->out_pointers().size()  == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->in_pointers().size()   == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->self_pointers().size() == 0, "ERROR: Number of data is not 0 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument->at(0)->datum("a")->adjacent_pointers().size() == 2, "ERRORR: Number of adjacent pointers is not 2");

    QVERIFY2( _graphDocument->at(0)->datum("b")->out_pointers().size() == 1, "ERROR: Number of pointer is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->in_pointers().size() == 2, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->self_pointers().size() == 0, "ERROR: Number of data is not 0 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->adjacent_data().size() == 3, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument->at(0)->datum("b")->adjacent_pointers().size() == 3, "ERRORR: Number of adjacent pointers is not 2");

    QVERIFY2( _graphDocument->at(0)->datum("i")->out_pointers().size() == 4, "ERROR: Number of pointer is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("i")->in_pointers().size() == 0, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("i")->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _graphDocument->at(0)->datum("i")->adjacent_data().size() == 4, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument->at(0)->datum("i")->adjacent_pointers().size() == 4, "ERRORR: Number of adjacent pointers is not 2");

    Datum *n = _graphDocument->at(0)->datum("i");
    qDebug() << n->property("name");
    _graphDocument->at(0)->remove(n);

    QVERIFY2( _graphDocument->at(0)->datum("a")->out_pointers().size()  == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->in_pointers().size()   == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument->at(0)->datum("a")->adjacent_pointers().size() == 2, "ERRORR: Number of adjacent pointers is not 2");

    QVERIFY2( _graphDocument->at(0)->datum("b")->out_pointers().size() == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->in_pointers().size() == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument->at(0)->datum("b")->adjacent_pointers().size() == 2, "ERRORR: Number of adjacent pointers is not 2");

    QVERIFY2( _graphDocument->at(0)->datum("i") == 0, "ERROR: Node  'i'  was not removed. ");

}

void GraphTests::createDirectedGraph() {
//   /* Creates a simple Graph with 5 data and connects them with pointers. */
    createPentagon(true);
    QCOMPARE( _graphDocument->count(), 1);
    QVERIFY2( _graphDocument->at(0)->data().size() == 5, "ERROR: Number of data is not 5 ");
    QVERIFY2( _graphDocument->at(0)->pointers().size() == 5, "ERROR: Number of pointers is not 5 ");

    foreach( Datum *n, _graphDocument->at(0)->data() ) {
        QVERIFY2( n->out_pointers().size() == 1, "ERROR: Number of out pointers is not 1");
        QVERIFY2( n->in_pointers().size() == 1, "ERROR: Number of in pointers is not 1");
        QVERIFY2( n->adjacent_data().size() == 1, "ERROR: Number of Adjacent Nodes is not 2");
        QVERIFY2( n->adjacent_pointers().size() == 1, "ERRORR: Number of adjacent pointers is not 2");
    }

}

void GraphTests::manipulateDirectedGraph() {
    /*   creates a 3x3 graph, play with some properties.... */
    create3x3(true);

    QVERIFY2( _graphDocument->at(0)->datum("a")->out_pointers().size()  == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->in_pointers().size()   == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->adjacent_data().size() == 1, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument->at(0)->datum("a")->adjacent_pointers().size() == 1, "ERRORR: Number of adjacent pointers is not 2");

    QVERIFY2( _graphDocument->at(0)->datum("b")->out_pointers().size() == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->in_pointers().size() == 2, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->adjacent_data().size() == 1, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument->at(0)->datum("b")->adjacent_pointers().size() == 1, "ERRORR: Number of adjacent pointers is not 2");

    QVERIFY2( _graphDocument->at(0)->datum("i")->out_pointers().size() == 4, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("i")->in_pointers().size() == 0, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("i")->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _graphDocument->at(0)->datum("i")->adjacent_data().size() == 4, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument->at(0)->datum("i")->adjacent_pointers().size() == 4, "ERRORR: Number of adjacent pointers is not 2");

    Datum *n = _graphDocument->at(0)->datum("i");
    _graphDocument->at(0)->remove(n);

    QVERIFY2( _graphDocument->at(0)->datum("a")->out_pointers().size()  == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->in_pointers().size()   == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->adjacent_data().size() == 1, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument->at(0)->datum("a")->adjacent_pointers().size() == 1, "ERRORR: Number of adjacent pointers is not 2");

    QVERIFY2( _graphDocument->at(0)->datum("b")->out_pointers().size() == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->in_pointers().size() == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->adjacent_data().size() == 1, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument->at(0)->datum("b")->adjacent_pointers().size() == 1, "ERRORR: Number of adjacent pointers is not 2");

    QVERIFY2( _graphDocument->at(0)->datum("i") == 0, "ERROR: Node  'i'  was not removed. ");

}
/*
void GraphTests::testKrossQtjs(){
//   createPentagon();
//
//   QVariantList graphLists;
//   graphLists.append( QVariant::fromValue( _graphDocument->at(0) ) );
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
  graphLists.append( QVariant::fromValue( _graphDocument->at(0) ) );

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
  graphLists.append( QVariant::fromValue( _graphDocument->at(0) ) );

  KrossBackend *k = new KrossBackend(&graphLists);
  k->setBackend("python");
  k->loadFile("krosspytests.py");

  Kross::Action *action = k->execute();
  QVariant result;
	//result = action->callFunction("testGraphPropertyName");
	//QVERIFY2(result.toString().toAscii() == "Graph1", "ERROR: Name of the graph cannot be retrieved.");
	result = action->callFunction("testNumberOfNodes");
	QVERIFY2(result.toInt() == 5, "ERROR: Number of data cannot be retrieved.");
	//result = action->callFunction("getANode");
	//QVERIFY2(result.toString().toAscii() == "a", "ERROR: node cannot be retrieved.");

}

void GraphTests::testKrossRb(){
//   QFAIL("Ruby crashes the app");
//   createPentagon();
//
//   QVariantList graphLists;
//   graphLists.append( QVariant::fromValue( _graphDocument->at(0) ) );
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

    QCOMPARE (_graphDocument->count(), 1);

    QFAIL("#########    Need to reimplemt this test case! ########");

// /*    QtScriptBackend *engine = _graphDocument->engineBackend();
// //     engine->setDocument(&_graphDocument);
// //     _graphDocument->setEngineBackend(engine);
//
//     engine->setScript(QString("graphs.length"), &_graphDocument);
//     //QScriptValue results =
//     engine->start();*/
//     QVERIFY2(results.toNumber() == 1, "Error: number of graphs is not 1.");
//
//     results = engine->engine()->evaluate("graphs[0].list_datums().length");
//     QVERIFY2(results.toNumber()  == 5, "Error: number of datums is not 5");
//
//     results = engine->engine()->evaluate("graphs[0].list_pointers().length");
//     QVERIFY2(results.toNumber()  == 5, "Error: number of pointers is not 5");
//
//     QString script = "";
//     script += "var graph = graphs[0]; \n ";
//     script += "var node1 = graph.list_data()[0]; \n ";
//     script += "var node2 = graph.list_data()[1]; \n ";
//     script += "node1 != node2";
//     results = engine->engine()->evaluate(script);
//     QVERIFY2(results.toBoolean() == true, "ERROR: data are equal but they shouldn't be.");
//
//     script.clear();
//     script += "var graph = graphs[0]; \n ";
//     script += "var node1 = graph.list_data()[0]; \n ";
//     script += "var node2 = graph.list_data()[1]; \n ";
//     script += "node1.adj_edges().length";
//     results = engine->engine()->evaluate(script);
//     QVERIFY2( results.toNumber() == 2, results.toString().toAscii());
}

void GraphTests::saveTestFile() {
    createPentagon();
    _graphDocument->saveAsInternalFormat("pentagono");
}

void  GraphTests::loadTestFile() {
    _graphDocument->loadFromInternalFormat("pentagono.datatype");

    QCOMPARE (_graphDocument->count(), 1);

    QVERIFY2( _graphDocument->at(0)->data().size() == 5, "ERROR: Number of data is not 5 ");
    QVERIFY2( _graphDocument->at(0)->pointers().size() == 5, "ERROR: Number of pointers is not 5 ");

    foreach( Datum *n, _graphDocument->at(0)->data() ) {
        QVERIFY2( n->out_pointers().size() == 1, "ERROR: Number of out pointers is not 1");
        QVERIFY2( n->in_pointers().size() == 1, "ERROR: Number of in pointers is not 1");
        QVERIFY2( n->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
        QVERIFY2( n->adjacent_pointers().size() == 2, "ERRORR: Number of adjacent pointers is not 2");
    }


    QVERIFY2( _graphDocument->at(0)->datum("a")->out_pointers().size()  == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->in_pointers().size()   == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument->at(0)->datum("a")->adjacent_pointers().size() == 2, "ERRORR: Number of adjacent pointers is not 2");

    QVERIFY2( _graphDocument->at(0)->datum("b")->out_pointers().size() == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->in_pointers().size() == 2, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->adjacent_data().size() == 3, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument->at(0)->datum("b")->adjacent_pointers().size() == 3, "ERRORR: Number of adjacent pointers is not 2");

    QVERIFY2( _graphDocument->at(0)->datum("i")->out_pointers().size() == 4, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("i")->in_pointers().size() == 0, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("i")->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _graphDocument->at(0)->datum("i")->adjacent_data().size() == 4, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument->at(0)->datum("i")->adjacent_pointers().size() == 4, "ERRORR: Number of adjacent pointers is not 2");

    Datum *n = _graphDocument->at(0)->datum("i");
    qDebug() << n->property("name");
    _graphDocument->at(0)->remove(n);

    QVERIFY2( _graphDocument->at(0)->datum("a")->out_pointers().size()  == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->in_pointers().size()   == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _graphDocument->at(0)->datum("a")->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument->at(0)->datum("a")->adjacent_pointers().size() == 2, "ERRORR: Number of adjacent pointers is not 2");

    QVERIFY2( _graphDocument->at(0)->datum("b")->out_pointers().size() == 1, "ERROR: Number of pointer is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->in_pointers().size() == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->self_pointers().size() == 0, "ERROR: Number of pointer is not 0 ");
    QVERIFY2( _graphDocument->at(0)->datum("b")->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _graphDocument->at(0)->datum("b")->adjacent_pointers().size() == 2, "ERRORR: Number of adjacent pointers is not 2");

    QVERIFY2( _graphDocument->at(0)->datum("i") == 0, "ERROR: Node  'i'  was not removed. ");
}

QTEST_KDEMAIN_CORE(GraphTests)
#include "testGraph.moc"
