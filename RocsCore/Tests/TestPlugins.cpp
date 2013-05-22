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
#include <qtest_kde.h>
#include "DataStructureBackendManager.h"
#include "Document.h"
#include "DocumentManager.h"
#include "DataStructure.h"
#include "DataStructureBackendInterface.h"
#include "CoreTypes.h"
#include <QDebug>

void TestPlugins::inittestcase()
{
    if (DataStructureBackendManager::self().backends().count() == 0) {
        QFAIL("No plugin of DS, no way to continue!");
    }
}


void TestPlugins::init()
{
    DocumentManager::self().newDocument();
}

void TestPlugins::cleanup()
{
    DocumentManager::self().removeDocument(DocumentManager::self().activeDocument());
}

void TestPlugins::standardPluginsLoaded()
{
    QList<QString> pluginNames = DataStructureBackendManager::self().backends();
    QVERIFY2(pluginNames.contains("Graph"), "Could not find Graph plugin");
    QVERIFY2(pluginNames.contains("LinkedList"), "Could not find LinkedList plugin");
    QVERIFY2(pluginNames.contains("RootedTree"), "Could not find Graph plugin");
}

void TestPlugins::createGraph()
{
    DataStructureBackendInterface * pl = DataStructureBackendManager::self().backend("Graph");
    QVERIFY2(pl,"Could create data structure of type Graph");

    DataStructureBackendManager::self().setBackend(pl->internalName());
    Document doc("TestDocument");
    DataStructurePtr ds = doc.addDataStructure();
    QCOMPARE(ds->metaObject()->className(), "Rocs::GraphStructure");
}

void TestPlugins::createList()
{
    DataStructureBackendInterface * pl = DataStructureBackendManager::self().backend("LinkedList");
    QVERIFY2(pl,"Could create data structure of type LinkedList");

    DataStructureBackendManager::self().setBackend(pl->internalName());
    Document doc("TestDocument");
    DataStructurePtr ds = doc.addDataStructure();
    QCOMPARE(ds->metaObject()->className(), "Rocs::ListStructure");
}

void TestPlugins::createRootedTree()
{
    DataStructureBackendInterface * pl = DataStructureBackendManager::self().backend("RootedTree");
    QVERIFY2(pl,"Could create data structure of type RootedTree");

    DataStructureBackendManager::self().setBackend(pl->internalName());
    Document doc("TestDocument");
    DataStructurePtr ds = doc.addDataStructure();
    QCOMPARE(ds->metaObject()->className(), "RootedTreeStructure");
}

void TestPlugins::convertGraphToLinkedList()
{
    DataStructureBackendInterface * plGraph = DataStructureBackendManager::self().backend("Graph");
    QVERIFY2(plGraph,"Graph plugin not found");

    DataStructureBackendInterface * plList = DataStructureBackendManager::self().backend("LinkedList");
    QVERIFY2(plList,"LinkedList plugin not found");

    DataStructureBackendManager::self().setBackend(plGraph->internalName());
    Document doc("TestDocument");

    //Create a simple graph
    DataStructurePtr tree = doc.addDataStructure("Graph1");
    DataPtr a = tree->createData("node1", 0);
    DataPtr b = tree->createData("node2", 0);
    DataPtr c = tree->createData("node3", 0);
    tree->createPointer(a, b, 0);
    tree->createPointer(a, c, 0);

    //Change plugin.
    DataStructureBackendManager::self().setBackend(plList->internalName());

    DataStructurePtr list = plList->convertToDataStructure(tree, &doc);

    QCOMPARE(list->dataList(0).count(), 3);
    //FIXME the conversion scripts from graphs to lists must be rewritten, they cannot even handle acyclic graphs gracefully
    QEXPECT_FAIL("", "wrong number of pointers after conversion.", Continue);
    QCOMPARE(list->pointers(0).count(), 1);
}

void TestPlugins::convertGraphToRootedTree()
{

    DataStructureBackendInterface *plGraph = DataStructureBackendManager::self().backend("Graph");
    QVERIFY2(plGraph,"Graph plugin not found");

    DataStructureBackendInterface *plTree = DataStructureBackendManager::self().backend("RootedTree");
    QVERIFY2(plTree,"Rooted plugin not found");

    DataStructureBackendManager::self().setBackend(plGraph->internalName());
    Document doc("TestDocument");
    //     connect(DSPluginManager::instance(), SIGNAL(changingDS(QString)), &doc, SLOT(convertToDS(QString)));
    //Create a simple graph
    DataStructurePtr tree = doc.addDataStructure("Graph1");
    DataPtr a = tree->createData("node1", 0);
    DataPtr b = tree->createData("node2", 0);
    DataPtr c = tree->createData("node3", 0);
    tree->createPointer(a, b, 0);
    tree->createPointer(a, c, 0);

    //Change plugin.
    DataStructureBackendManager::self().setBackend(plTree->internalName());

    DataStructurePtr list = plTree->convertToDataStructure(tree, &doc);

    QCOMPARE(list->dataList(0).count(), 3);
    QCOMPARE(list->pointers(0).count(), 4);
}


QTEST_KDEMAIN_CORE(TestPlugins)
