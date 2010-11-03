/*
    A Roc's test Case
    Copyright (C) 2010  Wagner Reck <wagner.reck@gmail.com>

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
#include <DataType.h>
#include <DSPluginInterface.h>

using namespace Rocs;

void TestDataStructure::inittestcase()
{
  if (DSPluginManager::instance()->pluginsList().count() == 0){
    QFAIL("No plugin of DS, no way to continue!");
  }
}

void TestDataStructure::create(){
    DSPluginManager::instance()->changeActiveDS(DSPluginManager::instance()->pluginsList().at(1)->name());
    DataTypeDocument doc("teste");
    DataType * graph = doc.addDataType();
    QCOMPARE (graph->metaObject()->className(), "Rocs::GraphStructure");
}




void TestDataStructure::changeAndCreate(){
  DSPluginManager::instance()->changeActiveDS(DSPluginManager::instance()->pluginsList().at(0)->name());
  DataTypeDocument doc("teste");
  DataType * graph = doc.addDataType();
  QCOMPARE (graph->metaObject()->className(), "Rocs::ListStructure");

}

void TestDataStructure::convert()
{
    DSPluginManager::instance()->changeActiveDS(DSPluginManager::instance()->pluginsList().at(1)->name());
    DataTypeDocument doc("teste");
    QSignalSpy spy(DSPluginManager::instance(), SIGNAL(changingDS(QString)));
//     connect(DSPluginManager::instance(), SIGNAL(changingDS(QString)), &doc, SLOT(convertToDS(QString)));
    //Create a simple graph
    DataType * graph = doc.addDataType("Graph1");
    graph->addDatum("node1");
    graph->addDatum("node2");
    graph->addPointer("node1", "node2");
    graph = doc.addDataType("Graph2");
    graph->setDirected(true);
    graph->addDatum("node1");
    graph->addDatum("node2");
    graph->addPointer("node1", "node2");

    //Change plugin.
    DSPluginManager::instance()->changeActiveDS(DSPluginManager::instance()->pluginsList().at(0)->name());
    QVERIFY (DSPluginManager::instance()->pluginsList().at(0)->name() == DSPluginManager::instance()->actualPlugin());
    doc.convertToDS(DSPluginManager::instance()->actualPlugin());
    DataTypeDocument * newDoc = &doc;

    QCOMPARE(newDoc->count(), 2);
    QVERIFY(newDoc->at(0)->directed());
    QVERIFY(newDoc->at(1)->directed());
    QCOMPARE(newDoc->at(0)->metaObject()->className(), "Rocs::ListStructure");
    QCOMPARE(newDoc->at(1)->metaObject()->className(), "Rocs::ListStructure");

    graph =  newDoc->at(0);
    QCOMPARE (graph->data().count(), 3);
    QCOMPARE (graph->pointers().count(), 1);

    graph =  newDoc->at(1);
    QCOMPARE (graph->data().count(), 3);
    QCOMPARE (graph->pointers().count(), 1);

}

QTEST_MAIN ( TestDataStructure )
