#include "Tests.h"
#include "MultiNode.h"
#include "MultiGraph.h"
#include "Edge.h"

void Tests::testCreatingMultiGraph()
{
  QVERIFY2(_graphCollection.size() == 0, "Initial size of graphCollections is not zero.");

  _graphCollection.addGraph(GraphCollection::multi);
  QVERIFY2(_graphCollection.size() == 1, "After inserting the first value, size of GraphCollection is not 1.");

  _graphCollection.addGraph(GraphCollection::multi);
  QVERIFY2(_graphCollection.size() == 2, "After inserting the second value, size of GraphCollection is not 2.");

  Graph *graph1 = _graphCollection[0];
  

  QVERIFY2(graph1->nodes().size() == 0, "Number of nodes on the newly created graph is not zero.");
  QVERIFY2(graph1->edges().size() == 0, "Number of edges on the newly created graph is not zero.");

  Graph *graph2 = _graphCollection[1];
  

  QVERIFY2(graph2->nodes().size() == 0, "Number of nodes on the newly created graph is not zero.");
  QVERIFY2(graph2->edges().size() == 0, "Number of edges on the newly created graph is not zero.");
}

void Tests::testManipulatingMultiGraph()
{
  Graph *graph1 = _graphCollection[0];
  Graph *graph2 = _graphCollection[1];

  graph1 -> setName ("Graph 1");
  QVERIFY2(graph1->name() == QString("Graph 1"), "Name of the graph is different than the setted one.");

  graph2 -> setName ("Graph 2");
  QVERIFY2(graph2 -> name() == QString("Graph 2"), "Name of the graph is different than the setted one.");

  graph1 -> setColor(Qt::red);
  QVERIFY2(graph1 -> color() == QColor(Qt::red), "Color of the graph is different than the setted one.");

  graph2 -> setColor(Qt::darkGray);
  QVERIFY2(graph2 -> color() == QColor(Qt::darkGray), "Color of the graph is different than the setted one.");

  graph1 -> setName("Tumaix");
  QVERIFY2(graph1 -> name() == QString("Tumaix"), "Name of the graph is different than the setted one.");

  graph2 -> setName("Xanadu");
  QVERIFY2(graph2 -> name() == QString("Xanadu"), "Name of the graph is different than the setted one.");

  graph1 -> setColor(Qt::green);
  QVERIFY2(graph1 -> color() == QColor(Qt::green), "Color of the graph is different than the setted one.");

  graph2 -> setColor(Qt::magenta);
  QVERIFY2(graph2 -> color() == QColor(Qt::magenta), "Color of the graph is different than the setted one.");
}

void Tests::testCreatingMultiGraphNodes()
{

  // Creating a K5, 50px distance of each node.
  /*
      1 51 101 151 201
  1   [ ][ ][*][ ][ ]
  51  [ ][ ][ ][ ][ ]
  101 [*][ ][ ][ ][*]
  151 [ ][ ][ ][ ][ ]
  201 [ ][*][ ][*][ ] 
  */
  Graph *graph1 = _graphCollection[0];
  graph1 -> createNode( QPointF(101, 1));
  graph1 -> createNode( QPointF(1, 101));
  graph1 -> createNode( QPointF(201, 101));
  graph1 -> createNode( QPointF(51, 201));
  graph1 -> createNode( QPointF(151, 201));
  QVERIFY2(graph1->nodes().size() == 5, "5 Nodes added, but the size of the Node List is not 5");

  // Creating a 3x3 grid.
/*
      1 51 101
 251 [][][]
 301 [][][]
 351 [][][]
*/

  Graph *graph2 = _graphCollection[1];
  Node *tmp = 0;

  tmp = graph2 -> createNode( QPointF(1, 	251) );
  QVERIFY2(tmp->index() == 0, "Index of the first node is not 0");

  tmp = graph2 -> createNode( QPointF(51, 	251) );
  QVERIFY2(tmp->index() == 1, "Index of the second node is not 1");

  tmp = graph2 -> createNode( QPointF(101, 	251) );
  QVERIFY2(tmp->index() == 2, "Index of the third node is not 2"); 
  
  tmp = graph2 -> createNode( QPointF(1, 	301) );
  QVERIFY2(tmp->index() == 3, "Index of the forth node is not 3" );

  graph2 -> createNode( QPointF(51, 	301) );
  graph2 -> createNode( QPointF(101, 	301) );
  graph2 -> createNode( QPointF(1, 	351) );
  graph2 -> createNode( QPointF(51, 	351) );
  graph2 -> createNode( QPointF(101, 	351) );
  
  QVERIFY2(graph2->nodes().size() == 9, "Total of nodes different than 9");

/* Test forcing a Non Simple Graph 
* 1 -> 3, 1->2, 1->4.
* 2 -> 3, 2->3, 2->4, 2-> 4
* 3 -> 5
* 4 -> 5
*/
  Graph *graph = new MultiGraph();
  _graphCollection << graph;
  graph->createNode( QPointF(0,50) );
  graph->createNode( QPointF(100, 50) );
  graph->createNode( QPointF(200, 0) );
  graph->createNode( QPointF(200, 150) );
  graph->createNode( QPointF(300, 50) );
  
  QList<Node*> nodes = graph->nodes();

  graph->createEdge( nodes[0], nodes[1] );
  graph->createEdge( nodes[0], nodes[2] );
  graph->createEdge( nodes[0], nodes[3] );
  graph->createEdge( nodes[1], nodes[2] );
  graph->createEdge( nodes[1], nodes[2] );
  graph->createEdge( nodes[1], nodes[3] );
  graph->createEdge( nodes[1], nodes[3] );
  graph->createEdge( nodes[2], nodes[4] );
  graph->createEdge( nodes[3], nodes[4] );

  QVERIFY2(graph->nodes().size() == 5, "Number of nodes does not equals 5");
  QVERIFY2(graph->edges().size() == 9, "Number of edges is wrong. ");
  QVERIFY2(nodes[1]->edges().size() == 5, "node 2 has more edges than it should.");
}

void Tests::testManipulatingMultiGraphNodes()
{

  Graph *graph1 = _graphCollection[0];

  Node *n1 = graph1->nodes()[0];
  Node *n2 = graph1->nodes()[2];

  n1->setName("Node 1");
  QVERIFY2(n1->name()  == QString("Node 1"), "Name of the first node different than setted one.");
  QVERIFY2(n1->index() == 0, "Index of the first node different than 0");

  n2->setName("Node 2");
  QVERIFY2(n2->name()  == QString("Node 2"), "NAme of the second node different than setted.");
  QVERIFY2(n2->index() == 2, "Index of the tirdh node different than 2");

  QVERIFY2(n1->edges().size() == 0, "Number of edges on the newly created node different than 0");
  QVERIFY2(n2->edges().size() == 0, "Number of edges on the newly created node different than 0");

  n1->setColor(Qt::red);
  QVERIFY2(n1->color() == QColor(Qt::red), "Color different than setted");

  n2->setColor(Qt::blue);
  QVERIFY2(n2->color() == QColor(Qt::blue), "Color different than setted");

  QPointF position = n1->position();
  n1->setPosition(QPointF(0,0));
  QVERIFY2(n1->position() == QPointF(0,0), "Position different than setted");
  n1->setPosition(position);
  QVERIFY2(n1->position() == position, "Position different than setted");

  QVERIFY2(n1->isConnected(n2) == false, "Nodes Are Connected (for some strange reason");
}

void Tests::testCreatingMultiGraphEdges()
{

// Creating a K5, 50px distance of each node.
/*
      1 51 101 151 201
 1   [ ][ ][*][ ][ ]
 51  [ ][ ][ ][ ][ ]
 101 [*][ ][ ][ ][*]
 151 [ ][ ][ ][ ][ ]
 201 [ ][*][ ][*][ ] 
*/

  // This code will generate Kn Graphs, remember to copy it to the 
  Graph *graph1 = _graphCollection[0];
  graph1 -> transformIntoKGraph();
  QVERIFY2(graph1->edges().size() == 10, "number of edges on K5 is different than 10");

// Creating a 3x3 grid.
/*
      1 51 101
 251 [0][1][2]
 301 [3][4][5]
 351 [6][7][8]
*/

  Graph *graph2 = _graphCollection[1];
  QList<Node*> nodeList = graph2->nodes();
  graph2->createEdge(nodeList[0], nodeList[1]);
  graph2->createEdge(nodeList[0], nodeList[3]);
  graph2->createEdge(nodeList[1], nodeList[4]);
  graph2->createEdge(nodeList[1], nodeList[2]);
  graph2->createEdge(nodeList[2], nodeList[5]);
  graph2->createEdge(nodeList[3], nodeList[4]);
  graph2->createEdge(nodeList[3], nodeList[6]);
  graph2->createEdge(nodeList[4], nodeList[5]);
  graph2->createEdge(nodeList[4], nodeList[7]);
  graph2->createEdge(nodeList[5], nodeList[8]);
  graph2->createEdge(nodeList[6], nodeList[7]);
  graph2->createEdge(nodeList[7], nodeList[8]);
  QVERIFY2(graph2->edges().size() == 12, "12 edges inserted, but size is wrong.");
}

void Tests::testManipulatingMultiGraphEdges()
{

  Graph *graph = _graphCollection[0];
  Edge *e = graph->edges()[0];
  e->setName("Estroncio");
  QVERIFY2(e->name() == QString("Estroncio"), "Name of the edge is not the one setted");
  
  e->setColor(Qt::red);
  QVERIFY2(e->color() == QColor(Qt::red), "Color of the Edge is not the one Setted");

  e->setVisited(true);
  QVERIFY2(e->visited() == true, "Edge is not being visited");
  
  e->unsetVisited();
  QVERIFY2(e->visited() == false, "Edge is not being unset");

  e->setLength(10);
  QVERIFY2(e->length() == 10, "Edge Length is not being set");

  e->disconnectNodes();
  QVERIFY2( (e->to() == 0) && (e->from() == 0), "Edge is not being disconnected");
  
  e->connectNodes();
  QVERIFY2( (e->to() != 0) && (e->from() != 0), "Edge is not being connected");
}


void Tests::testRemovingMultiGraphNodes()
{

  Graph *graph1 = _graphCollection[0];
  Graph *graph2 = _graphCollection[1];

  Node *node = graph1->nodes()[0];
  QVERIFY2(node->index() == 0, "Index of first node is not zero");
  
  graph1->removeNode(node->index());
  
  QVERIFY2(graph1->nodes().size() == 4,  "After removing 1 of 5 nodes, the result is not 4");
  QVERIFY2(graph1->edges().size() != 10, "After removing one of the edges, the result is still the original.");
  QVERIFY2(graph1->edges().size() == 6, "After removing one of the edges, the result is not 6, but should be");
  QVERIFY2(graph1->nodes()[0]->edges().size() == 3, "Should be 3, but it's not");
  
  node = graph2 -> nodes()[4];
  graph2->removeNode(node);
  QVERIFY2(graph2->nodes().size() == 8,  "After removing 1 of 5 nodes, the result is not 4");
  QVERIFY2(graph2->edges().size() != 12, "After removing one of the edges, the result is still the original.");
  QVERIFY2(graph2->edges().size() == 8, "After removing one of the edges, the result is not 6, but should be");
  QVERIFY2(graph2->nodes()[0]->edges().size() == 2, "Should be 3, but it's not");

}

void Tests::testRemovingMultiGraphEdges()
{

  Edge *e = _graphCollection[1]->nodes()[0]->edges()[0];
  
  Node *n1 = e->to();
  Node *n2 = e->from();
  
  QVERIFY2(n1->edges().size() == 2, "Size of edges differs from expected");
  QVERIFY2(n2->edges().size() == 2, "Size of edges differs from expected");
  QVERIFY2(_graphCollection[1]->edges().size() == 8, "size of edges differs from expected");

  n1->removeEdge(e);
  QVERIFY2(n1->edges().size() == 1, "After removing one edge, number of edges is not the one expected.");
  QVERIFY2(n2->edges().size() == 1, "After removing one edge, number of edges is not the one expected.");
  QVERIFY2(_graphCollection[1]->edges().size() == 7, "After removing one edge, number of edges is not the one expected.");

  n2->removeEdge(e);
  QVERIFY2(n1->edges().size() == 1,"After removing one edge, number of edges is not the one expected.");
  QVERIFY2(n2->edges().size() == 1, "After removing one edge, number of edges is not the one expected.");
  QVERIFY2(_graphCollection[1]->edges().size() == 7, "After removing one edge, number of edges is not the one expected.");

}

void Tests::testDestructingMultiGraph()
{
  _graphCollection.clear();
}