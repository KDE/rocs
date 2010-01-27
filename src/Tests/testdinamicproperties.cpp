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

#include <QTest>

  void TestDinamicProperties::addNodeDinamicProperty(){
    Graph g;
    Node *n = g.addNode("Node");
    QByteArray property = "newProperty";
    n->addDinamicProperty(property, QVariant(0));
    QVERIFY2(n->property(property) != QVariant::Invalid, "Property not added.");
      
  }
  
  void TestDinamicProperties::addEdgeDinamicProperty(){
    Graph g;
    g.setDirected(true);
    Node *n = g.addNode("Node");
    Edge *e = g.addEdge(n, n);
    QByteArray property = "newProperty";
    e->addDinamicProperty(property, QVariant(0));
    QVERIFY2(e->property(property) != QVariant::Invalid, "Property not added.");
    
    
  }
  
  void TestDinamicProperties::addGraphDinamicProperty(){
    Graph g;
    QByteArray property = "newProperty";
    g.addDinamicProperty(property);
    QVERIFY2(g.property(property) != QVariant::Invalid, "Property not added.");
    
  }
  
  void TestDinamicProperties::addToAllNodes(){
    Graph g;
    Node *n = g.addNode("Node");
//     g.add
    QByteArray property = "newProperty";
    g.addNodesDinamicProperty(property, QVariant(0));
    QVERIFY2(n->property(property) != QVariant::Invalid, "Property not added.");
    
  }
  void TestDinamicProperties::addToAllEdges(){
    Graph g;
    g.setDirected(true);
    Node *n = g.addNode("Node");
    Edge *e = g.addEdge(n,n);
    QByteArray property = "newProperty";
    g.addEdgesDinamicProperty(property, QVariant(0));
    //QVERIFY2(n->addDinamicProperty(property, QVariant(0) ), "Can't add property");
    QVERIFY2(e->property(property) != QVariant::Invalid, "Property not added.");
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
  }
  void TestDinamicProperties::removeGraphDinamicProperty(){
    Graph g;
    g.setDirected(true);
    QByteArray property = "newProperty";
    g.addDinamicProperty(property, QVariant(0));
    QVERIFY2(g.property(property) != QVariant::Invalid, "Property not added.");
    g.removeDinamicProperty(property);
    QVERIFY2(g.property(property) == QVariant::Invalid, "Property not removed.");
  }
  
  void TestDinamicProperties::removeToAllNodes(){
     Graph g;
    Node *n = g.addNode("Node");
//     g.add
    QByteArray property = "newProperty";
    g.addNodesDinamicProperty(property, QVariant(0));
    QVERIFY2(n->property(property) != QVariant::Invalid, "Property not added.");
    g.removeNodesDinamicProperty(property);
    QVERIFY2(n->property(property) == QVariant::Invalid, "Property not removed.");
  }
  void TestDinamicProperties::removeToAllEdges(){
    Graph g;
    g.setDirected(true);
    Node *n = g.addNode("Node");
    Edge *e = g.addEdge(n,n);
    QByteArray property = "newProperty";
    g.addEdgesDinamicProperty(property, QVariant(0));
    
    QVERIFY2(e->property(property) != QVariant::Invalid, "Property not added.");
    g.removeEdgesDinamicProperty(property);
    QVERIFY2(e->property(property) == QVariant::Invalid, "Property not removed.");
    
  }

QTEST_MAIN(TestDinamicProperties)