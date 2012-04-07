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
#include <qtest_kde.h>
#include <ktemporaryfile.h>

#include <Document.h>
#include <DataStructurePluginManager.h>
#include <DocumentManager.h>
#include <Project.h>

TestLoadSave::TestLoadSave()
{
    DocumentManager::self()->addDocument(new Document("test"));;
}


void TestLoadSave::serializeUnserializeTest()
{
    QMap<QString, DataPtr> dataList;
    // Creates a simple Graph with 5 data elements and connect them with pointers.
    DataStructurePtr ds = DocumentManager::self()->activeDocument()->activeDataStructure();

    ds->setProperty( "name", "Graph1" );
    dataList.insert( "a", ds->addData( "a" ) );
    dataList.insert( "b", ds->addData( "b" ) );
    dataList.insert( "c", ds->addData( "c" ) );
    dataList.insert( "d", ds->addData( "d" ) );
    dataList.insert( "e", ds->addData( "e" ) );

    ds->addPointer( dataList["a"], dataList["b"] );
    ds->addPointer( dataList["b"], dataList["c"] );
    ds->addPointer( dataList["c"], dataList["d"] );
    ds->addPointer( dataList["d"], dataList["e"] );
    ds->addPointer( dataList["e"], dataList["a"] );

    // serialize into file "serializetest.graph"
    DocumentManager::self()->activeDocument()->saveAs("serializetest");
    DocumentManager::self()->removeDocument(DocumentManager::self()->activeDocument());
    Document* testDoc = new Document("testDoc");

    // unserialize and test properties
    DocumentManager::self()->addDocument(testDoc);
    DocumentManager::self()->changeDocument(testDoc);
    DocumentManager::self()->activeDocument()->loadFromInternalFormat("serializetest.graph");

    // default data structure also present
    QVERIFY2 (DocumentManager::self()->activeDocument()->dataStructures().count() == 2, "ERROR: DataStructure not loaded");

    ds = DocumentManager::self()->activeDocument()->dataStructures().at(1);
    QVERIFY2( ds->dataList().size() == 5, "ERROR: Number of data is not 5 " );
    QVERIFY2( ds->pointers().size() == 5, "ERROR: Number of pointers is not 5 " );

    foreach( DataPtr n, ds->dataList() ) {
        QVERIFY2( n->out_pointers().size() == 1, "ERROR: Number of out pointers is not 1" );
        QVERIFY2( n->in_pointers().size() == 1, "ERROR: Number of in pointers is not 1" );
        QVERIFY2( n->adjacent_data().size() == 2, "ERROR: Number of Adjacent Nodes is not 2" );
        QVERIFY2( n->adjacent_pointers().size() == 2, "ERROR: Number of adjacent pointers is not 2" );
    }

}


void TestLoadSave::projectLoadSaveTest()
{
    KTemporaryFile temp;
    temp.setPrefix("rocsproject");
    temp.setSuffix(".tmp");
    temp.setAutoRemove(false);
    temp.open();

    // prepare project and save
    Project* testProject = new Project(temp.fileName());
    testProject->setName("new test name");
    testProject->addCodeFile("/path/to/code");
    testProject->addGraphFile("/path/to/graph");
    testProject->writeProjectFile();

    // load project
    Project* testProject2 = new Project(temp.fileName());
    QVERIFY2( testProject2->name().compare("new test name")==0, "ERROR: red filename differs.");
    QVERIFY( testProject2->codeFiles().count()==1);
    QVERIFY( testProject2->codeFiles().at(0).toLocalFile().compare("/path/to/code")==0);
    QVERIFY( testProject2->graphFiles().at(0).compare("/path/to/graph")==0);

    delete testProject;
    delete testProject2;
}


QTEST_KDEMAIN_CORE(TestLoadSave)
