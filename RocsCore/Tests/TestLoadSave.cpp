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

#include "TestLoadSave.h"
#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"
#include "libgraphtheory/typenames.h"
#include "libgraphtheory/graphdocument.h"

#include <QtTest/QtTest>
#include <QTemporaryFile>

#include <Document.h>
#include <DocumentManager.h>
#include <Project.h>

TestLoadSave::TestLoadSave()
{
    DocumentManager::self().addDocument(new Document("test"));;
}


void TestLoadSave::serializeUnserializeTest()
{
    DocumentManager::self().addDocument(new Document("testSerialization"));
    Document *document = DocumentManager::self().activeDocument();
    QMap<QString, DataPtr> dataList;

    // register additional properties
    document->dataType(0)->addProperty("testproperty", "default");
    document->pointerType(0)->addProperty("testproperty", "default");

    // Creates a simple Graph with 5 data elements and connect them with pointers.
    DataStructurePtr ds = document->activeDataStructure();
    ds->setProperty("name", "Graph1");
    dataList.insert("a", ds->createData("a", 0));
    dataList.insert("b", ds->createData("b", 0));
    dataList.insert("c", ds->createData("c", 0));
    dataList.insert("d", ds->createData("d", 0));
    dataList.insert("e", ds->createData("e", 0));

    ds->createPointer(dataList["a"], dataList["b"], 0);
    ds->createPointer(dataList["b"], dataList["c"], 0);
    ds->createPointer(dataList["c"], dataList["d"], 0);
    ds->createPointer(dataList["d"], dataList["e"], 0);
    ds->createPointer(dataList["e"], dataList["a"], 0);

    // serialize into file "serializetest.graph"
    DocumentManager::self().activeDocument()->saveAs("serializetest");
    DocumentManager::self().removeDocument(DocumentManager::self().activeDocument());
    Document* testDoc = new Document("testDoc");

    // unserialize and test properties
    DocumentManager::self().addDocument(testDoc);
    DocumentManager::self().changeDocument(testDoc);
    DocumentManager::self().openDocument(QUrl::fromLocalFile("serializetest.graph"));

    // default data structure also present
    QVERIFY2(DocumentManager::self().activeDocument()->dataStructures().count() == 1, "ERROR: DataStructure not loaded");

    document = DocumentManager::self().activeDocument();
    QVERIFY(document->dataType(0)->propertyDefaultValue("testproperty").toString() == "default");
    QVERIFY(document->pointerType(0)->propertyDefaultValue("testproperty").toString() == "default");

    ds = document->dataStructures().at(0);
    QVERIFY2(ds->dataList(0).size() == 5, "ERROR: Number of data is not 5 ");
    QVERIFY2(ds->pointers(0).size() == 5, "ERROR: Number of pointers is not 5 ");

    foreach(DataPtr n, ds->dataList(0)) {
        QVERIFY2(n->outPointerList().size() == 1, "ERROR: Number of out pointers is not 1");
        QVERIFY2(n->inPointerList().size() == 1, "ERROR: Number of in pointers is not 1");
        QVERIFY2(n->adjacentDataList().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
        QVERIFY2(n->pointerList().size() == 2, "ERROR: Number of adjacent pointers is not 2");
    }
}


void TestLoadSave::serializeUnserializeTypesTest()
{
    QMap<QString, DataPtr> dataList;

    // start with new document!
    DocumentManager::self().addDocument(new Document("testTypes"));
    Document *document = DocumentManager::self().activeDocument();
    DataStructurePtr ds = DocumentManager::self().activeDocument()->activeDataStructure();

    // register 2nd data and pointer type
    int dataTypeID = document->registerDataType("testDatatype");
    int pointerTypeID = document->registerPointerType("testPointertype");

    // add test data
    ds->setProperty("name", "Graph1");
    dataList.insert("a", ds->createData("a", dataTypeID));
    dataList.insert("b", ds->createData("b", dataTypeID));
    ds->createPointer(dataList["a"], dataList["b"], pointerTypeID);

    // serialize into file "serializetest.graph"
    DocumentManager::self().activeDocument()->saveAs("serializetest");
    DocumentManager::self().removeDocument(DocumentManager::self().activeDocument());
    Document* testDoc = new Document("testDoc");

    // unserialize and test properties
    DocumentManager::self().addDocument(testDoc);
    DocumentManager::self().changeDocument(testDoc);
    DocumentManager::self().openDocument(QUrl::fromLocalFile("serializetest.graph"));
    document = DocumentManager::self().activeDocument();
    ds = document->dataStructures().at(0);

    // default data structure also present
    QVERIFY2(document->dataType(dataTypeID), "ERROR: Data type not present");
    QVERIFY2(document->pointerType(pointerTypeID), "ERROR: Pointer type not present");
    QVERIFY2(ds->dataList(dataTypeID).size() > 0, "ERROR: type of data element changed");
    QVERIFY2(ds->pointers(pointerTypeID).size() > 0, "ERROR: type of pointer changed");
}

void TestLoadSave::projectLoadSaveTest()
{
    QTemporaryFile temp;
    temp.setAutoRemove(false);
    temp.open();

    // prepare project and save
    Project* testProject = new Project(QUrl::fromLocalFile(temp.fileName()));
    testProject->setName("new test name");
    testProject->addCodeFile(QUrl::fromLocalFile("/path/to/code.js"));
    GraphTheory::GraphDocumentPtr graph = testProject->createGraphDocument();
    graph->setDocumentUrl(QUrl::fromLocalFile("/path/to/graph.graph"));
    testProject->writeProjectFile();

    // load project
    Project* testProject2 = new Project(QUrl::fromLocalFile(temp.fileName()));
    QVERIFY2(testProject2->name().compare("new test name") == 0, "ERROR: project name changed");
    QVERIFY(testProject2->codeFiles().count() == 1);
    QVERIFY2(
        testProject2->codeFiles().at(0).toLocalFile() == "/path/to/code.js",
        "ERROR: path of code file changed"
    );
    QVERIFY2(
        false,
//         testProject2->graphFiles().at(0).toLocalFile() == "/path/to/graph.graph",
        "ERROR: path of graph file changed"
    );

    delete testProject;
    delete testProject2;
}

void TestLoadSave::loadMultipleLayerGraphTest()
{
    DocumentManager::self().openDocument(QUrl::fromLocalFile("TestGraphFiles/multipleLayerGraph.graph"));
    Document *document = DocumentManager::self().activeDocument();

    QVERIFY2(document->pointerTypeList().length() == 2, "two pointer types expected");
    QVERIFY2(!document->activeDataStructure()->dataList(0).first()->property("name").toString().isEmpty(), "property must be present");
    QVERIFY2(!document->activeDataStructure()->dataList(0).first()->property("target").toString().isEmpty(), "property must be present");
}

QTEST_MAIN(TestLoadSave)
