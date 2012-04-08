/*
    This file is part of Rocs.
    Copyright 2010       Tomaz Canabrava <tomaz.canabrava@gmail.com>
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

#include "TestDynamicProperties.h"

#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"
#include "DynamicPropertiesList.h"

#include <QTest>
#include <Document.h>
#include <DataStructurePluginManager.h>

TestDynamicProperties::TestDynamicProperties()
{
}

void TestDynamicProperties::initTestCase()
{
    QVERIFY2(DataStructurePluginManager::self()->pluginsList().count() > 0, "DataStruture Plugins not loaded");
    _document = new Document("untitled");
}

void TestDynamicProperties::cleanup()
{
    _document->deleteLater();
    DynamicPropertiesList::New()->clear();
}

void TestDynamicProperties::addNodeDynamicProperty()
{
    DataStructurePtr g = _document->addDataStructure("A graph");

    DataPtr n = g->addData("Node");
    QByteArray property = "newProperty";
    n->addDynamicProperty(property, QVariant(0));
    QVERIFY2(n->property(property) != QVariant::Invalid, "Property not added.");
    QVERIFY2(DynamicPropertiesList::New()->type(n.get(), property) == Global, "Property isn't global.");
}

void TestDynamicProperties::addEdgeDynamicProperty()
{
    DataStructurePtr g = _document->addDataStructure("A graph");

    DataPtr n1 = g->addData("Node1");
    DataPtr n2 = g->addData("Node2");
    PointerPtr e = g->addPointer(n1, n2);
    QVERIFY2(e->getPointer().get() != 0, "Pointer not created!");
    QByteArray property = "newProperty";
    e->addDynamicProperty(property, QVariant(0));
    QVERIFY2(e->property(property) != QVariant::Invalid, "Property not added.");
    QVERIFY2(DynamicPropertiesList::New()->type(e.get(), property) == Global, "Property isn't global.");
}

void TestDynamicProperties::addGraphDynamicProperty()
{
    DataStructurePtr g = _document->addDataStructure("A graph");

    QByteArray property = "newProperty";
    g->addDynamicProperty(property);
    QVERIFY2(g->property(property) != QVariant::Invalid, "Property not added.");
    QVERIFY2(DynamicPropertiesList::New()->type(g.get(), property) == Unique, "Property isn't Unique.");
}

void TestDynamicProperties::addToAllNodes()
{
    DataStructurePtr g = _document->addDataStructure("A graph");

    DataPtr n = g->addData("Node1");
    DataPtr n2 = g->addData("Node2");

    QByteArray property = "newProperty";
    g->addDataDynamicProperty(property);
    QVERIFY2(n->property(property) != QVariant::Invalid, "Property not added to Node1.");
    QVERIFY2(n2->property(property) != QVariant::Invalid, "Property not added to Node2.");
    QVERIFY2(DynamicPropertiesList::New()->type(n.get(), property) == Global, "Property isn't Global. (by node 1)");
    QVERIFY2(DynamicPropertiesList::New()->type(n2.get(), property) == Global, "Property isn't Global. (by node 2)");
}

void TestDynamicProperties::addToAllEdges()
{
    DataStructurePtr g = _document->addDataStructure("A graph");

    DataPtr n1 = g->addData("Node");
    DataPtr n2 = g->addData("Node");
    DataPtr n3 = g->addData("Node");
    PointerPtr e = g->addPointer(n1, n2);
    PointerPtr e2 = g->addPointer(n2, n3);
    PointerPtr e3 = g->addPointer(n1, n3);

    QByteArray property = "newProperty";
    g->addPointersDynamicProperty(property, QVariant(0));
    QVERIFY2(e->property(property) != QVariant::Invalid, "Property not added.");
    QVERIFY2(DynamicPropertiesList::New()->type(e.get(), property) == Global, "Property isn't Global. (by Edge 1)");
    QVERIFY2(DynamicPropertiesList::New()->type(e2.get(), property) == Global, "Property isn't Global. (by Edge 2)");
    QVERIFY2(DynamicPropertiesList::New()->type(e3.get(), property) == Global, "Property isn't Global. (by Edge 3)");
}

void TestDynamicProperties::removeNodeDynamicProperty()
{
    DataStructurePtr g = _document->addDataStructure("A graph");

    DataPtr n = g->addData("Node");
    QByteArray property = "newProperty";
    n->addDynamicProperty(property, QVariant(0));
    QVERIFY2(n->property(property) != QVariant::Invalid, "Property not added.");
    n->removeDynamicProperty(property);
    QVERIFY2(n->property(property) == QVariant::Invalid, "Property not removed.");
    QVERIFY2(DynamicPropertiesList::New()->type(n.get(), property) == None, "Property isn't None.");
    QVERIFY2(n->dynamicPropertyNames().size() == 0, "Still having property.");
}

void TestDynamicProperties::removeEdgeDynamicProperty()
{
    DataStructurePtr g = _document->addDataStructure("A graph");

    DataPtr n1 = g->addData("Node");
    DataPtr n2 = g->addData("Node");
    PointerPtr e = g->addPointer(n1, n2);
    QByteArray property = "newProperty";
    e->addDynamicProperty(property, QVariant(0));
    QVERIFY2(e->property(property) != QVariant::Invalid, "Property not added.");
    e->removeDynamicProperty(property);
    QVERIFY2(e->property(property) == QVariant::Invalid, "Property not removed.");
    QVERIFY2(DynamicPropertiesList::New()->type(e.get(), property) == None, "Property isn't None.");
    QVERIFY2(e->dynamicPropertyNames().size() == 0, "Still having property.");
}

void TestDynamicProperties::removeGraphDynamicProperty()
{
    DataStructurePtr g = _document->addDataStructure("A graph");

    QByteArray property = "newProperty";
    g->addDynamicProperty(property, QVariant(0));
    QVERIFY2(g->property(property) != QVariant::Invalid, "Property not added.");
    g->removeDynamicProperty(property);
    QVERIFY2(g->property(property) == QVariant::Invalid, "Property not removed.");
    QVERIFY2(DynamicPropertiesList::New()->type(g.get(), property) == None, "Property isn't None.");
    QVERIFY2(g->dynamicPropertyNames().size() == 0, "Still having property.");
}

void TestDynamicProperties::removeToAllNodes()
{
    DataStructurePtr g = _document->addDataStructure("A graph");

    DataPtr n = g->addData("Node 1");
    DataPtr n2 = g->addData("Node 2");

    QByteArray property = "newProperty";
    g->addDataDynamicProperty(property, QVariant(0));

    QVERIFY2(n->property(property) != QVariant::Invalid, "Property not added.");
    QVERIFY2(n2->property(property) != QVariant::Invalid, "Property not added.");

    g->removeDataDynamicProperty(property);
    QVERIFY2(n->property(property) == QVariant::Invalid, "Property not removed from node 1.");
    QVERIFY2(n2->property(property) == QVariant::Invalid, "Property not removedfrom node 2.");
    QVERIFY2(DynamicPropertiesList::New()->type(n.get(), property) == None, "Property isn't None.");
}

void TestDynamicProperties::removeToAllEdges()
{
    DataStructurePtr g = _document->addDataStructure("A graph");

    DataPtr n1 = g->addData("1");
    DataPtr n2 = g->addData("2");
    DataPtr n3 = g->addData("3");
    PointerPtr e = g->addPointer(n1, n2);
    PointerPtr e2 = g->addPointer(n2, n3);
    QByteArray property = "newProperty";
    g->addPointersDynamicProperty(property, QVariant(0));

    QVERIFY2(e->property(property) != QVariant::Invalid, "Property not added to edge 2.");
    QVERIFY2(e2->property(property) != QVariant::Invalid, "Property not added to edge 2.");
    g->removePointersDynamicProperty(property);
    QVERIFY2(e->property(property) == QVariant::Invalid, "Property not removed from edge 1.");
    QVERIFY2(e2->property(property) == QVariant::Invalid, "Property not removed from edge 2.");
    QVERIFY2(DynamicPropertiesList::New()->type(e.get(), property) == None, "Property isn't None.");
}

void TestDynamicProperties::MultipleProperties()
{
    DataStructurePtr g = _document->addDataStructure("A graph");

    DataPtr n1 = g->addData("Node 1");
    DataPtr n2 = g->addData("Node 2");
    DataPtr n3 = g->addData("Node 3");
    PointerPtr e1 = g->addPointer(n3, n1);
    PointerPtr e2 = g->addPointer(n2, n1);
    PointerPtr e3 = g->addPointer(n3, n2);

    QByteArray property = "newProperty";
    g->addDataDynamicProperty(property, QVariant(0));
    g->addPointersDynamicProperty(property, QVariant(0));

    QVERIFY2(DynamicPropertiesList::New()->type(n1.get(), property) == Global, "Property isn't global");
    QVERIFY2(DynamicPropertiesList::New()->type(e1.get(), property) == Global, "Property isn't global");

    n1->removeDynamicProperty(property);
    e1->removeDynamicProperty(property);
    QVERIFY2(DynamicPropertiesList::New()->type(n1.get(), property) == Multiple, "Property isn't Multiple");
    QVERIFY2(DynamicPropertiesList::New()->type(e1.get(), property) == Multiple, "Property isn't Multiple");

    n2->removeDynamicProperty(property);
    e2->removeDynamicProperty(property);
    QVERIFY2(DynamicPropertiesList::New()->type(n1.get(), property) == Unique, "Property isn't Unique");
    QVERIFY2(DynamicPropertiesList::New()->type(e1.get(), property) == Unique, "Property isn't Unique");

    n3->removeDynamicProperty(property);
    e3->removeDynamicProperty(property);
    QVERIFY2(DynamicPropertiesList::New()->type(n1.get(), property) == None, "Property isn't None");
    QVERIFY2(DynamicPropertiesList::New()->type(e1.get(), property) == None, "Property isn't Unique");
}

void TestDynamicProperties::changeNames()
{
    DataStructurePtr g = _document->addDataStructure("A graph");

    DataPtr n1 = g->addData("Node 1");
    DataPtr n2 = g->addData("Node 2");
    PointerPtr e  = g->addPointer(n1, n2);

    QString property = "newProperty";
    g->addDataDynamicProperty(property, QVariant(0));
    g->addPointersDynamicProperty(property, QVariant(0));
    g->addDynamicProperty(property, QVariant(0));

    DynamicPropertiesList::New()->changePropertyName(property, QString("newName_Node"), n1.get());
    DynamicPropertiesList::New()->changePropertyName(property, QString("newName_Edge"), e.get());
    DynamicPropertiesList::New()->changePropertyName(property, QString("newName_Graph"), g.get());

    QCOMPARE(n1->dynamicPropertyNames() [0], QByteArray("newName_Node"));
    QCOMPARE(n2->dynamicPropertyNames() [0], QByteArray("newName_Node"));
    QCOMPARE(e->dynamicPropertyNames() [0], QByteArray("newName_Edge"));
    QCOMPARE(g->dynamicPropertyNames() [0], QByteArray("newName_Graph"));
}

QTEST_MAIN(TestDynamicProperties)
