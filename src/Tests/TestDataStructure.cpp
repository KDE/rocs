/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "TestDataStructure.h"


#include "TestDataStructure.h"
#include <QtTest>
#include <DSPluginManager.h>
#include <graphDocument.h>
#include <DSPluginInterface.h>

using namespace Rocs;

void TestDataStructure::inittestcase()
{
  if (DSPluginManager::New()->pluginsList().count() == 0){
    QFAIL("No plugin of DS, no way to continue!");
  }
}

void TestDataStructure::create(){
    DSPluginManager::New()->changeActiveDS(DSPluginManager::New()->pluginsList().at(1)->name());
    GraphDocument doc("teste");
    DataType * graph = doc.addGraph();
    QCOMPARE (graph->metaObject()->className(), "Rocs::GraphStructure");
}




void TestDataStructure::changeAndCreate(){
  DSPluginManager::New()->changeActiveDS(DSPluginManager::New()->pluginsList().at(0)->name());
  GraphDocument doc("teste");
  DataType * graph = doc.addGraph();
  QCOMPARE (graph->metaObject()->className(), "Rocs::ListStructure");

}

void TestDataStructure::convert()
{
    DSPluginManager::New()->changeActiveDS(DSPluginManager::New()->pluginsList().at(1)->name());
    GraphDocument doc("teste");
    QSignalSpy spy(DSPluginManager::New(), SIGNAL(changingDS(QString)));
//     connect(DSPluginManager::New(), SIGNAL(changingDS(QString)), &doc, SLOT(convertToDS(QString)));
    //Create a simple graph
    DataType * graph = doc.addGraph("Graph1");
    graph->addNode("node1");
    graph->addNode("node2");
    graph->addEdge("node1", "node2");
    graph = doc.addGraph("Graph2");
    graph->setDirected(true);
    graph->addNode("node1");
    graph->addNode("node2");
    graph->addEdge("node1", "node2");

    //Change plugin.
    DSPluginManager::New()->changeActiveDS(DSPluginManager::New()->pluginsList().at(0)->name());
    doc.convertToDS(DSPluginManager::New()->actualPlugin());
    GraphDocument * newDoc = &doc;

    QCOMPARE(newDoc->count(), 2);
    QVERIFY(newDoc->at(0)->directed());
    QVERIFY(newDoc->at(1)->directed());
    QCOMPARE(newDoc->at(0)->metaObject()->className(), "Rocs::ListStructure");
    QCOMPARE(newDoc->at(1)->metaObject()->className(), "Rocs::ListStructure");

    graph =  newDoc->at(0);
    QCOMPARE (graph->nodes().count(), 2);
    QCOMPARE (graph->edges().count(), 1);

    graph =  newDoc->at(1);
    QCOMPARE (graph->nodes().count(), 2);
    QCOMPARE (graph->edges().count(), 1);

}

QTEST_MAIN ( TestDataStructure )
