/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

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

#include "TestDynamicProperties.h"

#include "DataStructureBase.h"
#include "node.h"
#include "edge.h"
#include "DynamicPropertiesList.h"

#include <QTest>
#include <DSPluginManager.h>


// void TestDynamicProperties::initTestCase()
// {
//   foreach(RRocs::DSPluginManager::New()->listOfDS()
// }
#include <graphDocument.h>


void TestDynamicProperties:: cleanup() {
    DynamicPropertiesList::New()->clear();
}


void TestDynamicProperties::addNodeDynamicProperty() {
    GraphDocument d("unnamed");
    DataType *g = d.addGraph("A graph");
    Datum *n = g->addNode ( "Node" );
    QByteArray property = "newProperty";
    n->addDynamicProperty ( property, QVariant ( 0 ) );
    QVERIFY2 ( n->property ( property ) != QVariant::Invalid, "Property not added." );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( n, property ) == Global, "Property isn't global." );

}

void TestDynamicProperties::addEdgeDynamicProperty() {
    GraphDocument d("unnamed");
    DataType *g = d.addGraph("A graph");
    g->setDirected ( true );
    Datum *n = g->addNode ( "Node" );
    Pointer *e = g->addEdge ( n, n );
    QByteArray property = "newProperty";
    e->addDynamicProperty ( property, QVariant ( 0 ) );
    QVERIFY2 ( e->property ( property ) != QVariant::Invalid, "Property not added." );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( e, property ) == Global, "Property isn't global." );



}

void TestDynamicProperties::addGraphDynamicProperty() {
    DynamicPropertiesList::New()->clear();
    GraphDocument d("unnamed");
    DataType *g = d.addGraph("A graph");
    QByteArray property = "newProperty";
    g->addDynamicProperty ( property );
    QVERIFY2 ( g->property ( property ) != QVariant::Invalid, "Property not added." );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( g, property ) == Unique, "Property isn't Unique." );

}

void TestDynamicProperties::addToAllNodes() {
    GraphDocument d("unnamed");
    DataType *g = d.addGraph("A graph");
    Datum *n = g->addNode ( "Node1" );
    Datum *n2 = g->addNode ( "Node2" );
//     g.add
    QByteArray property = "newProperty";
    g->addNodesDynamicProperty ( property, QVariant ( 0 ) );
    QVERIFY2 ( n->property ( property ) != QVariant::Invalid, "Property not added to Node1." );
    QVERIFY2 ( n2->property ( property ) != QVariant::Invalid, "Property not added to Node2." );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( n, property ) == Global, "Property isn't Global. (by node 1)" );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( n2, property ) == Global, "Property isn't Global. (by node 2)" );


}
void TestDynamicProperties::addToAllEdges() {
    GraphDocument d("unnamed");
    DataType *g = d.addGraph("A graph");
    g->setDirected ( true );
    Datum *n = g->addNode ( "Node" );
    Pointer *e = g->addEdge ( n,n );
    Pointer *e2 = g->addEdge ( n,n );
    QByteArray property = "newProperty";
    g->addEdgesDynamicProperty ( property, QVariant ( 0 ) );
    QVERIFY2 ( e->property ( property ) != QVariant::Invalid, "Property not added." );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( e, property ) == Global, "Property isn't Global. (by Edge 1)" );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( e2, property ) == Global, "Property isn't Global. (by Edge 2)" );

}

void TestDynamicProperties::removeNodeDynamicProperty() {
    GraphDocument d("unnamed");
    DataType *g = d.addGraph("A graph");
    g->setDirected ( true );
    Datum *n = g->addNode ( "Node" );
    QByteArray property = "newProperty";
    n->addDynamicProperty ( property, QVariant ( 0 ) );
    QVERIFY2 ( n->property ( property ) != QVariant::Invalid, "Property not added." );
    n->removeDynamicProperty ( property );
    QVERIFY2 ( n->property ( property ) == QVariant::Invalid, "Property not removed." );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( n, property ) == None, "Property isn't None." );
    QVERIFY2 ( n->dynamicPropertyNames().size() == 0, "Still having property." );

}

void TestDynamicProperties::removeEdgeDynamicProperty() {
    GraphDocument d("unnamed");
    DataType *g = d.addGraph("A graph");
    g->setDirected ( true );
    Datum *n = g->addNode ( "Node" );
    Pointer *e = g->addEdge ( n, n );
    QByteArray property = "newProperty";
    e->addDynamicProperty ( property, QVariant ( 0 ) );
    QVERIFY2 ( e->property ( property ) != QVariant::Invalid, "Property not added." );
    e->removeDynamicProperty ( property );
    QVERIFY2 ( e->property ( property ) == QVariant::Invalid, "Property not removed." );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( e, property ) == None, "Property isn't None." );
    QVERIFY2 ( e->dynamicPropertyNames().size() == 0, "Still having property." );

}
void TestDynamicProperties::removeGraphDynamicProperty() {
    GraphDocument d("unnamed");
    DataType *g = d.addGraph("A graph");
    g->setDirected ( true );
    QByteArray property = "newProperty";
    g->addDynamicProperty ( property, QVariant ( 0 ) );
    QVERIFY2 ( g->property ( property ) != QVariant::Invalid, "Property not added." );
    g->removeDynamicProperty ( property );
    QVERIFY2 ( g->property ( property ) == QVariant::Invalid, "Property not removed." );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( g, property ) == None, "Property isn't None." );
    QVERIFY2 ( g->dynamicPropertyNames().size() == 0, "Still having property." );

}

void TestDynamicProperties::removeToAllNodes() {
    GraphDocument d("unnamed");
    DataType *g = d.addGraph("A graph");
    Datum *n = g->addNode ( "Node 1" );
    Datum *n2 = g->addNode ( "Node 2" );
//     g.add
    QByteArray property = "newProperty";
    g->addNodesDynamicProperty ( property, QVariant ( 0 ) );
    QVERIFY2 ( n->property ( property ) != QVariant::Invalid, "Property not added." );
    QVERIFY2 ( n2->property ( property ) != QVariant::Invalid, "Property not added." );
    g->removeNodesDynamicProperty ( property );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( n, property ) == None, "Property isn't None." );
    QVERIFY2 ( n->property ( property ) == QVariant::Invalid, "Property not removed from node 1." );
    QVERIFY2 ( n2->property ( property ) == QVariant::Invalid, "Property not removedfrom node 1." );

}
void TestDynamicProperties::removeToAllEdges() {
    GraphDocument d("unnamed");
    DataType *g = d.addGraph("A graph");
    g->setDirected ( true );
    Datum *n = g->addNode ( "Node" );
    Pointer *e = g->addEdge ( n,n );
    Pointer *e2 = g->addEdge ( n,n );
    QByteArray property = "newProperty";
    g->addEdgesDynamicProperty ( property, QVariant ( 0 ) );

    QVERIFY2 ( e->property ( property ) != QVariant::Invalid, "Property not added to edge 2." );
    QVERIFY2 ( e2->property ( property ) != QVariant::Invalid, "Property not added to edge 2." );
    g->removeEdgesDynamicProperty ( property );
    QVERIFY2 ( e->property ( property ) == QVariant::Invalid, "Property not removed from edge 1." );
    QVERIFY2 ( e2->property ( property ) == QVariant::Invalid, "Property not removed from edge 2." );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( e, property ) == None, "Property isn't None." );

}

void TestDynamicProperties::MultipleProperties() {
    GraphDocument d("unnamed");
    DataType *g = d.addGraph("A graph");
    Datum *n1 = g->addNode ( "Node 1" );
    Datum *n2 = g->addNode ( "Node 2" );
    Datum *n3 = g->addNode ( "Node 3" );
    Pointer *e1 = g->addEdge ( n3, n1 );
    g->addEdge ( n2, n1 );
    g->addEdge ( n3, n2 );

    QByteArray property = "newProperty";
    g->addNodesDynamicProperty ( property, QVariant ( 0 ) );
    g->addEdgesDynamicProperty ( property, QVariant ( 0 ) );

    QVERIFY2 ( DynamicPropertiesList::New()->type ( n1, property ) == Global, "Property isn't global" );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( e1, property ) == Global, "Property isn't global" );

    n1->removeDynamicProperty ( property );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( n1, property ) == Multiple, "Property isn't Multiple" );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( e1, property ) == Multiple, "Property isn't Multiple" );

    n2->removeDynamicProperty ( property );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( n1, property ) == Unique, "Property isn't Unique" );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( e1, property ) == Unique, "Property isn't Unique" );


    n3->removeDynamicProperty ( property );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( n1, property ) == None, "Property isn't None" );
    QVERIFY2 ( DynamicPropertiesList::New()->type ( e1, property ) == None, "Property isn't Unique" );

}

void TestDynamicProperties::changeNames() {
  GraphDocument d("unnamed");
    DataType *g = d.addGraph("A graph");
    Datum *n1 = g->addNode ( "Node 1" );
    Datum *n2 = g->addNode ( "Node 2" );
    Pointer *e  = g->addEdge ( n1, n2 );

    QString property = "newProperty";
    g->addNodesDynamicProperty ( property, QVariant ( 0 ) );
    g->addEdgesDynamicProperty ( property, QVariant ( 0 ) );
    g->addDynamicProperty ( property, QVariant ( 0 ) );

    DynamicPropertiesList::New()->changePropertyName ( property, QString ( "newName_Node" ), n1 );
    DynamicPropertiesList::New()->changePropertyName ( property, QString ( "newName_Edge" ), e );
    DynamicPropertiesList::New()->changePropertyName ( property, QString ( "newName_Graph" ), g );

    QCOMPARE ( n1->dynamicPropertyNames() [0], QByteArray ( "newName_Node" ) );
    QCOMPARE ( n2->dynamicPropertyNames() [0], QByteArray ( "newName_Node" ) );
    QCOMPARE ( e->dynamicPropertyNames() [0], QByteArray ( "newName_Edge" ) );
    QCOMPARE ( g->dynamicPropertyNames() [0], QByteArray ( "newName_Graph" ) );

}

QTEST_MAIN ( TestDynamicProperties )
