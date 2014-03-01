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

#include "TestLinkedListStructure.h"
#include "DataStructure.h"
#include "../ListStructure.h"
#include "../ListNode.h"
#include "Data.h"
#include "Pointer.h"
#include "KrossBackend.h"
#include "QtScriptBackend.h"
#include <qtest_kde.h>

#include <Document.h>
#include <DataStructureBackendManager.h>
#include <DocumentManager.h>
#include <QDebug>

using namespace Rocs;

TestLinkedListStructure::TestLinkedListStructure()
{
    QVERIFY(DataStructureBackendManager::self().backends().count() > 0);
    DocumentManager::self().addDocument(new Document("test"));
}

void TestLinkedListStructure::listModificationTest()
{
    // test for the basic properties of the list structure
    DataStructureBackendManager::self().setBackend("LinkedList");
    DataStructurePtr ds = DocumentManager::self().activeDocument()->addDataStructure("AddDeleteTest");
    QList< boost::shared_ptr<ListNode> > dataList;

    // add one node
    dataList.append(boost::static_pointer_cast<ListNode>(ds->createData(QString(1),0)));
    QVERIFY2(!dataList.at(0)->next(), "ERROR: no next node expected");

    // add node, by this automatically add next pointer of last node in list
    dataList.append(boost::static_pointer_cast<ListNode>(ds->createData(QString(2),0)));
    QVERIFY2(dataList.at(1)->next()->identifier() == dataList.at(0)->identifier(), "ERROR: wrong next node");

    // remove all data elements
    foreach(DataPtr data, dataList) {
        data->remove();
    }
    boost::shared_ptr<Rocs::ListStructure> listStructure = boost::dynamic_pointer_cast<Rocs::ListStructure>(ds);

    QVERIFY2(ds->dataList(0).size() == 0, "ERROR: Not all data elements were deleted");
}


QTEST_KDEMAIN_CORE(TestLinkedListStructure)
