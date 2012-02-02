/*
    This file is part of Rocs.
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>

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

#include "TestDataStructure.h"

#include <QtTest/QSignalSpy>
#include <QtTest/QTest>
#include "DataStructurePluginManager.h"
#include "Document.h"
#include "DataStructure.h"
#include "DataStructurePluginInterface.h"
#include "Rocs_Typedefs.h"

void TestDataStructure::inittestcase()
{
  if (DataStructurePluginManager::self()->pluginsList().count() == 0){
    QFAIL("No plugin of DS, no way to continue!");
  }
}

void TestDataStructure::create(){
    DataStructurePluginManager::self()->setDataStructurePlugin(DataStructurePluginManager::self()->pluginsList().at(1)->name());
    Document doc("TestDocument");
    DataStructurePtr ds = doc.addDataStructure();
    QCOMPARE (ds->metaObject()->className(), "Rocs::DataStructure");
}




void TestDataStructure::changeAndCreate(){
    DataStructurePluginManager::self()->setDataStructurePlugin(DataStructurePluginManager::self()->pluginsList().at(0)->name());
    Document doc("TestDocument");
    DataStructurePtr ds = doc.addDataStructure();
    QCOMPARE (ds->metaObject()->className(), "Rocs::ListStructure");
}

void TestDataStructure::convert()
{
    DataStructurePluginManager::self()->setDataStructurePlugin(DataStructurePluginManager::self()->pluginsList().at(1)->name());
    Document doc("TestDocument");
    QSignalSpy spy(DataStructurePluginManager::self(), SIGNAL(changingDS(QString)));
//     connect(DSPluginManager::instance(), SIGNAL(changingDS(QString)), &doc, SLOT(convertToDS(QString)));
    //Create a simple graph
    DataStructurePtr graph = doc.addDataStructure("Graph1");
    graph->addData("node1");
    graph->addData("node2");
    graph->addPointer("node1", "node2");
    graph = doc.addDataStructure("Graph2");
//     graph->setDirected(true);
    graph->addData("node1");
    graph->addData("node2");
    graph->addPointer("node1", "node2");

    //Change plugin.
    DataStructurePluginManager::self()->setDataStructurePlugin(DataStructurePluginManager::self()->pluginsList().at(0)->name());
//     QVERIFY (DataStructurePluginManager::self()->pluginsList().at(0)->name() == DataStructurePluginManager::self()->actualPlugin());
//     doc.convertToDS(DataStructurePluginManager::self()->actualPlugin());
    Document * newDoc = &doc;

    QCOMPARE(newDoc->dataStructures().count(), 2);
//     QVERIFY(newDoc->at(0)->directed());
//     QVERIFY(newDoc->at(1)->directed());
    QCOMPARE(newDoc->dataStructures().at(0)->metaObject()->className(), "Rocs::ListStructure");
    QCOMPARE(newDoc->dataStructures().at(1)->metaObject()->className(), "Rocs::ListStructure");

    graph =  newDoc->dataStructures().at(0);
    QCOMPARE (graph->dataList().count(), 3);
    QCOMPARE (graph->pointers().count(), 1);

    graph =  newDoc->dataStructures().at(1);
    QCOMPARE (graph->dataList().count(), 3);
    QCOMPARE (graph->pointers().count(), 1);

}

QTEST_MAIN ( TestDataStructure )
