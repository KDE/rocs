/* 
    This file is part of Rocs.
    Copyright 2004-2010  Tomaz Canabrava <tomaz.canabrava@gmail.com>
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

#include "testGraph.h"
#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"
#include "KrossBackend.h"
#include "QtScriptBackend.h"
#include <qtest_kde.h>

#include <kross/core/action.h>
#include <kross/core/manager.h>
#include <Document.h>
#include <DataStructurePluginManager.h>


GraphTests::GraphTests(){

}

void GraphTests::initTestCase(){
    QVERIFY(DataStructurePluginManager::self()->pluginsList().count( )  > 0);
    _graphDocument = new Document("untitled");
}


void GraphTests::cleanupTestCase(){
    _graphDocument->deleteLater();
}

void GraphTests::cleanup(){
    _data.clear();
    for (int i=0; i<_graphDocument->dataStructures().count(); ++i){
        _graphDocument->dataStructures().at(i)->remove();
    }
    _graphDocument->dataStructures().clear();
}

void GraphTests::createPentagon(bool directed) {
    _graphDocument->addDataStructure("untitled1");
//     _graphDocument->dataStructures().at(0)->setDirected(directed);
    _graphDocument->dataStructures().at(0)->setProperty("name","Graph1");
    _data.insert("a",_graphDocument->dataStructures().at(0)->addData("a"));
    _data.insert("b",_graphDocument->dataStructures().at(0)->addData("b"));
    _data.insert("c",_graphDocument->dataStructures().at(0)->addData("c"));
    _data.insert("d",_graphDocument->dataStructures().at(0)->addData("d"));
    _data.insert("e",_graphDocument->dataStructures().at(0)->addData("e"));

    _graphDocument->dataStructures().at(0)->addPointer(_data["a"], _data["b"]);
    _graphDocument->dataStructures().at(0)->addPointer(_data["b"], _data["c"]);
    _graphDocument->dataStructures().at(0)->addPointer(_data["c"], _data["d"]);
    _graphDocument->dataStructures().at(0)->addPointer(_data["d"], _data["e"]);
    _graphDocument->dataStructures().at(0)->addPointer(_data["e"], _data["a"]);
}

void GraphTests::create3x3(bool directed) {
    /*   creates a 3x3 graph, play with some properties.... */
    _graphDocument->addDataStructure("untitled2");
//     _graphDocument->dataStructures().at(0)->setDirected(directed);
    _graphDocument->dataStructures().at(0)->setProperty("name", "Graph2");
    
    _data.insert("a",_graphDocument->dataStructures().at(0)->addData("a"));
    _data.insert("b",_graphDocument->dataStructures().at(0)->addData("b"));
    _data.insert("c",_graphDocument->dataStructures().at(0)->addData("c"));
    _data.insert("d",_graphDocument->dataStructures().at(0)->addData("d"));
    _data.insert("e",_graphDocument->dataStructures().at(0)->addData("e"));
    _data.insert("f",_graphDocument->dataStructures().at(0)->addData("f"));
    _data.insert("g",_graphDocument->dataStructures().at(0)->addData("g"));
    _data.insert("h",_graphDocument->dataStructures().at(0)->addData("h"));
    _data.insert("i",_graphDocument->dataStructures().at(0)->addData("i"));
    
    _graphDocument->dataStructures().at(0)->addPointer(_data["a"], _data["b"]);
    _graphDocument->dataStructures().at(0)->addPointer(_data["b"], _data["c"]);
    _graphDocument->dataStructures().at(0)->addPointer(_data["c"], _data["d"]);
    _graphDocument->dataStructures().at(0)->addPointer(_data["d"], _data["e"]);
    _graphDocument->dataStructures().at(0)->addPointer(_data["e"], _data["f"]);
    _graphDocument->dataStructures().at(0)->addPointer(_data["f"], _data["g"]);
    _graphDocument->dataStructures().at(0)->addPointer(_data["g"], _data["h"]);
    _graphDocument->dataStructures().at(0)->addPointer(_data["h"], _data["a"]);
    _graphDocument->dataStructures().at(0)->addPointer(_data["i"], _data["b"]);
    _graphDocument->dataStructures().at(0)->addPointer(_data["i"], _data["d"]);
    _graphDocument->dataStructures().at(0)->addPointer(_data["i"], _data["f"]);
    _graphDocument->dataStructures().at(0)->addPointer(_data["i"], _data["h"]);
}

void GraphTests::createSimpleGraph() {
    /* Creates a simple Graph with 5 datums and connects them with pointers. */
    createPentagon();

    QVERIFY2( _graphDocument->dataStructures().at(0)->dataList().size() == 5, "ERROR: Number of data is not 5 ");
    QVERIFY2( _graphDocument->dataStructures().at(0)->pointers().size() == 5, "ERROR: Number of pointers is not 5 ");

    foreach( DataPtr n, _graphDocument->dataStructures().at(0)->dataList() ) {
        QVERIFY2( n->out_pointers().size() == 1, "ERROR: Number of out pointers is not 1");
        QVERIFY2( n->in_pointers().size() == 1, "ERROR: Number of in pointers is not 1");
        QVERIFY2( n->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
        QVERIFY2( n->adjacent_pointers().size() == 2, "ERROR: Number of adjacent pointers is not 2");
    }
}

void GraphTests::manipulateSimpleGraph() {
    create3x3();

    QVERIFY2( _data["a"]->out_pointers().size()  == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["a"]->in_pointers().size()   == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["a"]->self_pointers().size() == 0, "ERROR: Number of data is not 0 ");
    QVERIFY2( _data["a"]->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _data["a"]->adjacent_pointers().size() == 2, "ERROR: Number of adjacent pointers is not 2");

    QVERIFY2( _data["b"]->out_pointers().size() == 1, "ERROR: Number of pointer is not 1 ");
    QVERIFY2( _data["b"]->in_pointers().size() == 2, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["b"]->self_pointers().size() == 0, "ERROR: Number of data is not 0 ");
    QVERIFY2( _data["b"]->adjacent_data().size() == 3, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _data["b"]->adjacent_pointers().size() == 3, "ERROR: Number of adjacent pointers is not 2");

    QVERIFY2( _data["i"]->out_pointers().size() == 4, "ERROR: Number of pointer is not 1 ");
    QVERIFY2( _data["i"]->in_pointers().size() == 0, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["i"]->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _data["i"]->adjacent_data().size() == 4, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _data["i"]->adjacent_pointers().size() == 4, "ERROR: Number of adjacent pointers is not 2");

    _graphDocument->dataStructures().at(0)->remove(_data["i"]);
    _data.remove("i"); //TODO only now the data is destroyed since a reference still exists

    QVERIFY2( _data["a"]->out_pointers().size()  == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["a"]->in_pointers().size()   == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["a"]->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _data["a"]->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _data["a"]->adjacent_pointers().size() == 2, "ERROR: Number of adjacent pointers is not 2");

    QVERIFY2( _data["b"]->out_pointers().size() == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["b"]->in_pointers().size() == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["b"]->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _data["b"]->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _data["b"]->adjacent_pointers().size() == 2, "ERROR: Number of adjacent pointers is not 2");

    QVERIFY2( _data["i"] == 0, "ERROR: Node  'i'  was not removed. ");

}

void GraphTests::createDirectedGraph() {
    /* Creates a simple Graph with 5 data and connects them with pointers. */
    createPentagon(true);
    QCOMPARE( _graphDocument->dataStructures().count(), 1);
    QVERIFY2( _graphDocument->dataStructures().at(0)->dataList().size() == 5, "ERROR: Number of data is not 5 ");
    QVERIFY2( _graphDocument->dataStructures().at(0)->pointers().size() == 5, "ERROR: Number of pointers is not 5 ");

    foreach( DataPtr n, _graphDocument->dataStructures().at(0)->dataList() ) {
        QVERIFY2( n->out_pointers().size() == 1, "ERROR: Number of out pointers is not 1");
        QVERIFY2( n->in_pointers().size() == 1, "ERROR: Number of in pointers is not 1");
        QVERIFY2( n->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
        QVERIFY2( n->adjacent_pointers().size() == 2, "ERROR: Number of adjacent pointers is not 2");
    }
}

void GraphTests::manipulateDirectedGraph() {
    /*   creates a 3x3 graph, play with some properties.... */
    create3x3(true);

    QVERIFY2( _data["a"]->out_pointers().size()  == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["a"]->in_pointers().size()   == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["a"]->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _data["a"]->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _data["a"]->adjacent_pointers().size() == 2, "ERROR: Number of adjacent pointers is not 2");

    QVERIFY2( _data["b"]->out_pointers().size() == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["b"]->in_pointers().size() == 2, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["b"]->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _data["b"]->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _data["b"]->adjacent_pointers().size() == 2, "ERROR: Number of adjacent pointers is not 2");

    QVERIFY2( _data["i"]->out_pointers().size() == 4, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["i"]->in_pointers().size() == 0, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["i"]->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _data["i"]->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _data["i"]->adjacent_pointers().size() == 2, "ERROR: Number of adjacent pointers is not 2");

    DataPtr n = _data["i"];
    _graphDocument->dataStructures().at(0)->remove(n);

    QVERIFY2( _data["a"]->out_pointers().size()  == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["a"]->in_pointers().size()   == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["a"]->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _data["a"]->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _data["a"]->adjacent_pointers().size() == 2, "ERROR: Number of adjacent pointers is not 2");

    QVERIFY2( _data["b"]->out_pointers().size() == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["b"]->in_pointers().size() == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["b"]->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _data["b"]->adjacent_data().size() == 1, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _data["b"]->adjacent_pointers().size() == 1, "ERROR: Number of adjacent pointers is not 2");

    QVERIFY2( _data["i"] == 0, "ERROR: Node  'i'  was not removed. ");

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

    QCOMPARE (_graphDocument->dataStructures().count(), 1);

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

    QVERIFY2 (_graphDocument->dataStructures().count() == 1, "ERROR: DataStructure not loaded");

    QVERIFY2( _graphDocument->dataStructures().at(0)->dataList().size() == 5, "ERROR: Number of data is not 5 ");
    QVERIFY2( _graphDocument->dataStructures().at(0)->pointers().size() == 5, "ERROR: Number of pointers is not 5 ");

    foreach( DataPtr n, _graphDocument->dataStructures().at(0)->dataList() ) {
        QVERIFY2( n->out_pointers().size() == 1, "ERROR: Number of out pointers is not 1");
        QVERIFY2( n->in_pointers().size() == 1, "ERROR: Number of in pointers is not 1");
        QVERIFY2( n->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
        QVERIFY2( n->adjacent_pointers().size() == 2, "ERROR: Number of adjacent pointers is not 2");
    }

    // sort out what is A,B,I
    

    QVERIFY2( _data["a"]->out_pointers().size()  == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["a"]->in_pointers().size()   == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["a"]->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _data["a"]->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _data["a"]->adjacent_pointers().size() == 2, "ERROR: Number of adjacent pointers is not 2");

    QVERIFY2( _data["b"]->out_pointers().size() == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["b"]->in_pointers().size() == 2, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["b"]->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _data["b"]->adjacent_data().size() == 3, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _data["b"]->adjacent_pointers().size() == 3, "ERROR: Number of adjacent pointers is not 2");

    QVERIFY2( _data["i"]->out_pointers().size() == 4, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["i"]->in_pointers().size() == 0, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["i"]->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _data["i"]->adjacent_data().size() == 4, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _data["i"]->adjacent_pointers().size() == 4, "ERROR: Number of adjacent pointers is not 2");

    DataPtr n = _data["i"];
    qDebug() << n->property("name");
    _graphDocument->dataStructures().at(0)->remove(n);

    QVERIFY2( _data["a"]->out_pointers().size()  == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["a"]->in_pointers().size()   == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["a"]->self_pointers().size() == 0, "ERROR: Number of pointers is not 0 ");
    QVERIFY2( _data["a"]->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _data["a"]->adjacent_pointers().size() == 2, "ERROR: Number of adjacent pointers is not 2");

    QVERIFY2( _data["b"]->out_pointers().size() == 1, "ERROR: Number of pointer is not 1 ");
    QVERIFY2( _data["b"]->in_pointers().size() == 1, "ERROR: Number of pointers is not 1 ");
    QVERIFY2( _data["b"]->self_pointers().size() == 0, "ERROR: Number of pointer is not 0 ");
    QVERIFY2( _data["b"]->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
    QVERIFY2( _data["b"]->adjacent_pointers().size() == 2, "ERROR: Number of adjacent pointers is not 2");

    QVERIFY2( _data["i"] == 0, "ERROR: Node  'i'  was not removed. ");
}

QTEST_KDEMAIN_CORE(GraphTests)
#include "testGraph.moc"
