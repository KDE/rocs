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

void TestDynamicProperties::removeNodeDynamicProperty()
{
    DataStructurePtr g = _document->addDataStructure("A graph");

    DataPtr n = g->addData("Node");
    QByteArray property = "newProperty";
    int propertyNumber = n->dynamicPropertyNames().size();

    n->addDynamicProperty(property, QVariant(0));
    QVERIFY2(n->property(property) != QVariant::Invalid, "Property not added.");
    QVERIFY(n->dynamicPropertyNames().size() == propertyNumber+1);

    n->removeDynamicProperty(property);
    QVERIFY2(n->property(property) == QVariant::Invalid, "Property not removed.");
    QVERIFY2(DynamicPropertiesList::New()->type(n.get(), property) == None, "Property isn't None.");

    QVERIFY2(n->dynamicPropertyNames().size() == propertyNumber, "Still having property.");
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

void TestDynamicProperties::changeNames()
{
    DataStructurePtr g = _document->addDataStructure("A graph");

    DataPtr n1 = g->addData("Node 1");
    DataPtr n2 = g->addData("Node 2");
    PointerPtr e  = g->addPointer(n1, n2);

    QString property = "newProperty";
    n1->addDynamicProperty(property, QVariant(0));
    n2->addDynamicProperty(property, QVariant(0));
    g->addPointersDynamicProperty(property, QVariant(0));
    g->addDynamicProperty(property, QVariant(0));

    DynamicPropertiesList::New()->changePropertyName(property, QString("newName_Node"), n1.get());
    DynamicPropertiesList::New()->changePropertyName(property, QString("newName_Edge"), e.get());
    DynamicPropertiesList::New()->changePropertyName(property, QString("newName_Graph"), g.get());

    QVERIFY(n1->dynamicPropertyNames().contains(QByteArray("newName_Node")));
    QVERIFY(n2->dynamicPropertyNames().contains(QByteArray("newName_Node")));
    QCOMPARE(e->dynamicPropertyNames()[0], QByteArray("newName_Edge"));
    QCOMPARE(g->dynamicPropertyNames()[0], QByteArray("newName_Graph"));
}

QTEST_MAIN(TestDynamicProperties)
