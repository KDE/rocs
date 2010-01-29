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

#include "testdinamicproperties.h"

#include "graph.h"
#include "node.h"
#include "edge.h"
#include "dinamicpropertieslist.h"

#include <QTest>


void TestDinamicProperties:: cleanup(){
    DinamicPropertiesList::New()->clear();
}


  void TestDinamicProperties::addNodeDinamicProperty(){
    Graph g;
    Node *n = g.addNode("Node");
    QByteArray property = "newProperty";
    n->addDinamicProperty(property, QVariant(0));
    QVERIFY2(n->property(property) != QVariant::Invalid, "Property not added.");
    QVERIFY2(DinamicPropertiesList::New()->type(n, property) == Global, "Property isn't global.");

  }
  
  void TestDinamicProperties::addEdgeDinamicProperty(){
    
    Graph g;
    g.setDirected(true);
    Node *n = g.addNode("Node");
    Edge *e = g.addEdge(n, n);
    QByteArray property = "newProperty";
    e->addDinamicProperty(property, QVariant(0));
    QVERIFY2(e->property(property) != QVariant::Invalid, "Property not added.");
    QVERIFY2(DinamicPropertiesList::New()->type(e, property) == Global, "Property isn't global.");

    
    
  }
  
  void TestDinamicProperties::addGraphDinamicProperty(){
    DinamicPropertiesList::New()->clear();
    Graph g;
    QByteArray property = "newProperty";
    g.addDinamicProperty(property);
    QVERIFY2(g.property(property) != QVariant::Invalid, "Property not added.");   
    QVERIFY2(DinamicPropertiesList::New()->type(&g, property) == Unique, "Property isn't Unique.");
    
  }
  
  void TestDinamicProperties::addToAllNodes(){
    Graph g;
    Node *n = g.addNode("Node1");
    Node *n2 = g.addNode("Node2");
//     g.add
    QByteArray property = "newProperty";
    g.addNodesDinamicProperty(property, QVariant(0));
    QVERIFY2(n->property(property) != QVariant::Invalid, "Property not added to Node1.");
    QVERIFY2(n2->property(property) != QVariant::Invalid, "Property not added to Node2.");
    QVERIFY2(DinamicPropertiesList::New()->type(n, property) == Global, "Property isn't Global. (by node 1)");
    QVERIFY2(DinamicPropertiesList::New()->type(n2, property) == Global, "Property isn't Global. (by node 2)");

        
  }
  void TestDinamicProperties::addToAllEdges(){
    Graph g;
    g.setDirected(true);
    Node *n = g.addNode("Node");
    Edge *e = g.addEdge(n,n);
    Edge *e2 = g.addEdge(n,n);
    QByteArray property = "newProperty";
    g.addEdgesDinamicProperty(property, QVariant(0));
    QVERIFY2(e->property(property) != QVariant::Invalid, "Property not added.");
    QVERIFY2(DinamicPropertiesList::New()->type(e, property) == Global, "Property isn't Global. (by Edge 1)");
    QVERIFY2(DinamicPropertiesList::New()->type(e2, property) == Global, "Property isn't Global. (by Edge 2)");

  }

  void TestDinamicProperties::removeNodeDinamicProperty(){
    Graph g;
    g.setDirected(true);
    Node *n = g.addNode("Node");
    QByteArray property = "newProperty";
    n->addDinamicProperty(property, QVariant(0));
    QVERIFY2(n->property(property) != QVariant::Invalid, "Property not added.");
    n->removeDinamicProperty(property);
    QVERIFY2(n->property(property) == QVariant::Invalid, "Property not removed.");
    QVERIFY2(DinamicPropertiesList::New()->type(n, property) == None, "Property isn't None.");
    QVERIFY2(n->dynamicPropertyNames().size() == 0, "Still having property.");
    
  }
   
  void TestDinamicProperties::removeEdgeDinamicProperty(){
    Graph g;
    g.setDirected(true);
    Node *n = g.addNode("Node");
    Edge *e = g.addEdge(n, n);
    QByteArray property = "newProperty";
    e->addDinamicProperty(property, QVariant(0));
    QVERIFY2(e->property(property) != QVariant::Invalid, "Property not added.");
    e->removeDinamicProperty(property);
    QVERIFY2(e->property(property) == QVariant::Invalid, "Property not removed.");
    QVERIFY2(DinamicPropertiesList::New()->type(e, property) == None, "Property isn't None.");
    QVERIFY2(e->dynamicPropertyNames().size() == 0, "Still having property.");
    
  }
  void TestDinamicProperties::removeGraphDinamicProperty(){
    Graph g;
    g.setDirected(true);
    QByteArray property = "newProperty";
    g.addDinamicProperty(property, QVariant(0));
    QVERIFY2(g.property(property) != QVariant::Invalid, "Property not added.");
    g.removeDinamicProperty(property);
    QVERIFY2(g.property(property) == QVariant::Invalid, "Property not removed.");
    QVERIFY2(DinamicPropertiesList::New()->type(&g, property) == None, "Property isn't None.");
    QVERIFY2(g.dynamicPropertyNames().size() == 0, "Still having property.");
    
  }
  
  void TestDinamicProperties::removeToAllNodes(){
     Graph g;
    Node *n = g.addNode("Node 1");
    Node *n2 = g.addNode("Node 2");
//     g.add
    QByteArray property = "newProperty";
    g.addNodesDinamicProperty(property, QVariant(0));
    QVERIFY2(n->property(property) != QVariant::Invalid, "Property not added.");
    QVERIFY2(n2->property(property) != QVariant::Invalid, "Property not added.");
    g.removeNodesDinamicProperty(property);
    QVERIFY2(DinamicPropertiesList::New()->type(n, property) == None, "Property isn't None.");
    QVERIFY2(n->property(property) == QVariant::Invalid, "Property not removed from node 1.");
    QVERIFY2(n2->property(property) == QVariant::Invalid, "Property not removedfrom node 1.");
    
  }
  void TestDinamicProperties::removeToAllEdges(){
    Graph g;
    g.setDirected(true);
    Node *n = g.addNode("Node");
    Edge *e = g.addEdge(n,n);
    Edge *e2 = g.addEdge(n,n);
    QByteArray property = "newProperty";
    g.addEdgesDinamicProperty(property, QVariant(0));
    
    QVERIFY2(e->property(property) != QVariant::Invalid, "Property not added to edge 2.");
    QVERIFY2(e2->property(property) != QVariant::Invalid, "Property not added to edge 2.");
    g.removeEdgesDinamicProperty(property);
    QVERIFY2(e->property(property) == QVariant::Invalid, "Property not removed from edge 1.");
    QVERIFY2(e2->property(property) == QVariant::Invalid, "Property not removed from edge 2.");
    QVERIFY2(DinamicPropertiesList::New()->type(e, property) == None, "Property isn't None.");
    
  }
  
  void TestDinamicProperties::MultipleProperties(){
    Graph g;
    Node *n1 = g.addNode("Node 1");
    Node *n2 = g.addNode("Node 2");
    Node *n3 = g.addNode("Node 3");
    QByteArray property = "newProperty";
    g.addNodesDinamicProperty(property, QVariant(0));
    
    QVERIFY2(DinamicPropertiesList::New()->type(n1, property) == Global, "Property isn't global");
    
    n1->removeDinamicProperty(property);
    QVERIFY2(DinamicPropertiesList::New()->type(n1, property) == Multiple, "Property isn't Multiple");
    
    n2->removeDinamicProperty(property);
    QVERIFY2(DinamicPropertiesList::New()->type(n1, property) == Unique, "Property isn't Unique");
    
    n3->removeDinamicProperty(property);
    QVERIFY2(DinamicPropertiesList::New()->type(n1, property) == None, "Property isn't None");
    
  }
  
  void TestDinamicProperties::changeNames(){
      Graph g;
    Node *n1 = g.addNode("Node 1");
    Node *n2 = g.addNode("Node 2");
    Edge *e  = g.addEdge(n1, n2);
    
    QString property = "newProperty";
    g.addNodesDinamicProperty(property, QVariant(0));
    g.addEdgesDinamicProperty(property, QVariant(0));
    g.addDinamicProperty(property, QVariant(0));
    
    DinamicPropertiesList::New()->changePropertyName(property, QString("newName_Node"), n1);
    DinamicPropertiesList::New()->changePropertyName(property, QString("newName_Edge"), e);
    DinamicPropertiesList::New()->changePropertyName(property, QString("newName_Graph"), &g);
    
    QCOMPARE(n1->dynamicPropertyNames()[0], QByteArray("newName_Node"));
    QCOMPARE(n2->dynamicPropertyNames()[0], QByteArray("newName_Node"));
    QCOMPARE( e->dynamicPropertyNames()[0], QByteArray("newName_Edge"));
    QCOMPARE(  g.dynamicPropertyNames()[0], QByteArray("newName_Graph"));
    
  }

QTEST_MAIN(TestDinamicProperties)