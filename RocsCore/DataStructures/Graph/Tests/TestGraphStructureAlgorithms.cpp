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

#include "TestGraphStructureAlgorithms.h"
#include "DataStructure.h"
#include "../GraphStructure.h"
#include "Data.h"
#include "Pointer.h"
#include "KrossBackend.h"
#include "QtScriptBackend.h"
#include <qtest_kde.h>

#include <kross/core/action.h>
#include <kross/core/manager.h>
#include <Document.h>
#include <DataStructureBackendManager.h>
#include <DocumentManager.h>

#include <cmath>

TestGraphStructureAlgorithms::TestGraphStructureAlgorithms()
{
    QVERIFY(DataStructureBackendManager::self().backends().count() > 0);
}

void TestGraphStructureAlgorithms::init()
{
    DataStructureBackendManager::self().setBackend("Graph");
    DocumentManager::self().newDocument();
}


void TestGraphStructureAlgorithms::cleanupTestCase()
{
    qDebug() << "Remove previous test case.";
    DocumentManager::self().removeDocument(DocumentManager::self().activeDocument());
}

void TestGraphStructureAlgorithms::testDistances()
{
    Document *document = DocumentManager::self().activeDocument();
    DataList dataList;

    document->pointerType(0)->setDirection(PointerType::Bidirectional);

    DataStructurePtr ds = document->addDataStructure("Dijkstra");
    boost::shared_ptr<Rocs::GraphStructure> graph = boost::static_pointer_cast<Rocs::GraphStructure>(ds);

    // need to set engine because GraphStructure::distances calls QScriptEngine::newArray
    ds->setEngine(new QScriptEngine(this));

    // create 3 nodes connected as follows:
    // x-x x
    int nodes = 3;
    dataList.clear();
    for (int i = 0; i < nodes; ++i) {
        dataList.append(graph->createData(QString(i), 0));
    }
    dataList[0]->createPointer(dataList[1]);

    // test distances from 0 to all others
    QScriptValue distances = graph->distances(dataList[0].get());
    QVERIFY2(distances.property(0).equals(QScriptValue(0)), "ERROR: distance is wrong");
    QVERIFY2(distances.property(1).equals(QScriptValue(1)), "ERROR: distance is wrong");
    QVERIFY2(distances.property(2).equals(QScriptValue(INFINITY)), "ERROR: distance is wrong");
}

void TestGraphStructureAlgorithms::testDijkstraBidirectional()
{
    Document *document = DocumentManager::self().activeDocument();
    DataList dataList;

    document->pointerType(0)->setDirection(PointerType::Bidirectional);

    DataStructurePtr ds = document->addDataStructure("Dijkstra");
    boost::shared_ptr<Rocs::GraphStructure> graph = boost::static_pointer_cast<Rocs::GraphStructure>(ds);

    // create line of length 9 and test values
    int nodes = 10;
    dataList.clear();

    for (int i = 0; i < nodes; ++i) {
        dataList.append(graph->createData(QString(i), 0));
    }
    for (int i = 0; i < nodes-1; ++i) {
        dataList[i]->createPointer(dataList[i+1]);
    }

    // test distances from 0 to all others
    QMap<DataPtr, PointerList> paths = graph->dijkstraShortestPaths(dataList.at(0));
    for (int i = 0; i < nodes; ++i) {
        QVERIFY2(paths[dataList[i]].length() == i, "ERROR: distance is wrong");
    }
    // test distances from n to all others
    paths = graph->dijkstraShortestPaths(dataList.at(nodes-1));
    for (int i = 0; i < nodes; ++i) {
        QVERIFY2(paths[dataList[nodes-i-1]].length() == i, "ERROR: distance is wrong");
    }
}

void TestGraphStructureAlgorithms::testDijkstraUnidirectional()
{
    Document *document = DocumentManager::self().activeDocument();
    DataList dataList;

    document->pointerType(0)->setDirection(PointerType::Unidirectional);

    DataStructurePtr ds = document->addDataStructure("Dijkstra");
    boost::shared_ptr<Rocs::GraphStructure> graph = boost::static_pointer_cast<Rocs::GraphStructure>(ds);

    // create circle of 10 nodes
    int nodes = 10;
    dataList.clear();

    for (int i = 0; i < nodes; ++i) {
        dataList.append(graph->createData(QString(i), 0));
    }
    for (int i = 0; i < nodes-1; ++i) {
        dataList[i]->createPointer(dataList[i+1]);
    }
    dataList[nodes-1]->createPointer(dataList[0]);

    // test distances from 0 to all others
    QMap<DataPtr, PointerList> paths = graph->dijkstraShortestPaths(dataList.at(0));
    for (int i = 0; i < nodes; ++i) {
        QVERIFY2(paths[dataList[i]].length() == i, "ERROR: distance is wrong");
    }
}

QTEST_KDEMAIN_CORE(TestGraphStructureAlgorithms)
