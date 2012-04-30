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

#include "TestPlugins.h"

#include <QtTest/QSignalSpy>
#include <QtTest/QTest>
#include "DataStructurePluginManager.h"
#include "Document.h"
#include "DataStructure.h"
#include "DataStructurePluginInterface.h"
#include "Rocs_Typedefs.h"
#include <QDebug>

void TestPlugins::inittestcase()
{
    if (DataStructurePluginManager::self()->pluginsList().count() == 0) {
        QFAIL("No plugin of DS, no way to continue!");
    }
}

void TestPlugins::standardPluginsLoaded()
{
    QList<QString> pluginNames;
    foreach (DataStructurePluginInterface* plugin, DataStructurePluginManager::self()->pluginsList()) {
         pluginNames.append(plugin->internalName());
    }
    QVERIFY2(pluginNames.contains("Graph"), "Could not found Graph plugin");
    QVERIFY2(pluginNames.contains("Linked list"), "Could not found LinkedList plugin");
    QVERIFY2(pluginNames.contains("RootedTree"), "Could not found Graph plugin");
}

void TestPlugins::createGraph()
{
    DataStructurePluginInterface * pl = DataStructurePluginManager::self()->plugin("Graph");
    QVERIFY2(pl,"Could create data structure of type Graph");

    DataStructurePluginManager::self()->setDataStructurePlugin(pl->name());
    Document doc("TestDocument");
    DataStructurePtr ds = doc.addDataStructure();
    QCOMPARE(ds->metaObject()->className(), "Rocs::GraphStructure");
}

void TestPlugins::createList()
{
    DataStructurePluginInterface * pl = DataStructurePluginManager::self()->plugin("Linked list");
    QVERIFY2(pl,"Could create data structure of type LinkedList");

    DataStructurePluginManager::self()->setDataStructurePlugin(pl->name());
    Document doc("TestDocument");
    DataStructurePtr ds = doc.addDataStructure();
    QCOMPARE(ds->metaObject()->className(), "Rocs::ListStructure");
}

void TestPlugins::createRootedTree()
{
    DataStructurePluginInterface * pl = DataStructurePluginManager::self()->plugin("RootedTree");
    QVERIFY2(pl,"Could create data structure of type RootedTree");

    DataStructurePluginManager::self()->setDataStructurePlugin(pl->name());
    Document doc("TestDocument");
    DataStructurePtr ds = doc.addDataStructure();
    QCOMPARE(ds->metaObject()->className(), "RootedTreeStructure");
}

void TestPlugins::convertGraphToLinkedList()
{
    DataStructurePluginInterface * plGraph = DataStructurePluginManager::self()->plugin("Graph");

    QVERIFY2(plGraph,"Graph plugin not found");

    DataStructurePluginInterface * plList = DataStructurePluginManager::self()->plugin("Linked list");

    QVERIFY2(plList,"Linked list plugin not found");

    DataStructurePluginManager::self()->setDataStructurePlugin(plGraph->name());
    Document doc("TestDocument");
//     connect(DSPluginManager::instance(), SIGNAL(changingDS(QString)), &doc, SLOT(convertToDS(QString)));
    //Create a simple graph
    DataStructurePtr tree = doc.addDataStructure("Graph1");
    tree->addData("node1");
    tree->addData("node2");
    tree->addData("node3");
    tree->addPointer("node1", "node2");
    tree->addPointer("node1", "node3");

    //Change plugin.
    DataStructurePluginManager::self()->setDataStructurePlugin(plList->name());

    DataStructurePtr list = plList->convertToDataStructure(tree, &doc);

    QCOMPARE(list->dataList().count(), 3);
    QCOMPARE(list->pointers().count(), 1);
}

QTEST_MAIN(TestPlugins)
