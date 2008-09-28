#include "Tests.h"
#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include "GraphCollection.h"

#include <QtDebug>

QTEST_MAIN(Tests)

Tests::Tests() : QObject()
{
  connectSignalsGraphCollection(&_graphCollection);
}

Tests::~Tests()
{

}

void Tests::initTestCase()
{

}

void Tests::cleanupTestCase()
{

}

void Tests::init()
{

}

void Tests::cleanup()
{

}

    // Graph Collection Specific
void Tests::graphCollectionNameChanged(QString n)
{
  qDebug() << " Graph Collection Name = " << n ;
}

void Tests::graphCollectionGraphCreated(Graph *g)
{
  qDebug() << " New Graph Created ";
  connectSignalsGraph(g);
}

void Tests::graphCollectionGraphDestroyed(int index)
{
  qDebug() << "Graph at position " << index << "destroyed.";
}

// Graph Specific
void Tests::graphNameChanged(QString n)
{
  qDebug() << "Graph Name = " << n ;
}

void Tests::graphColorChanged(QColor c)
{
  qDebug() << "Graph Color changed." << c;
}

void Tests::graphNodeCreated(Node *n)
{
  qDebug() << "Node Created";
  connectSignalsNode(n);
}

void Tests::graphNodeRemoved(int index)
{
  qDebug() << "Node at position" << index << "removed";
}

void Tests::graphEdgeCreated(Edge *e)
{
  qDebug() << "Edge Created";
  connectSignalsEdge(e);
}

void Tests::graphEdgeRemoved(int index)
{
  qDebug() << "Edge at position" << index << "removed";
}

// Node Specific
void Tests::nodeNameChanged(QString n)
{
  qDebug() << "Node Name = " << n;
}

void Tests::nodePositionChanged(QPointF position)
{
  qDebug() << "node Position Changed, new Position: x" << position.x() << ", y " << position.y();
}

void Tests::nodeColorChanged(QColor n)
{
  qDebug() << "Node Color Changed" << n;
}

void Tests::nodeIndexChanged(int i)
{
  qDebug() << "Node index changed to " << i;
}

// Edge Specific
void Tests::edgeNameChanged(QString n)
{
  qDebug() << "Edge Name = " << n ;
}

void Tests::edgeLengthChanged(qreal l)
{
  qDebug() << "Edge Length changed to " << l;
}

void Tests::edgeVisitedChanged(bool v)
{
  if (v) qDebug() << "Edge Visited";
  else qDebug() << "Edge unvisited";
}

void Tests::edgeColorChanged(QColor c)
{
  qDebug() << "Edge Color Changed" << c;
}

void Tests::connectSignalsGraphCollection(GraphCollection *gc)
{
  connect( gc, SIGNAL(graphCreated(Graph*)), this, SLOT(graphCreated(Graph*)));
  connect( gc, SIGNAL(graphRemoved(int)),    this, SLOT(graphRemoved(int)));
  connect( gc, SIGNAL(nameChanged(QString)), this, SLOT(graphCollectionNameChanged(QString)));
}

void Tests::connectSignalsGraph(Graph *graph)
{
  connect( graph, SIGNAL(nameChanged(QString)), this, SLOT(graphNameChanged(QString)));
  connect( graph, SIGNAL(colorChanged(QColor)), this, SLOT(graphColorChanged(QColor)));
  connect( graph, SIGNAL(edgeCreated(Edge*)), 	this, SLOT(graphEdgeCreated(Edge*)));
  connect( graph, SIGNAL(nodeCreated(Node*)), 	this, SLOT(graphNodeCreated(Node*)));
  connect( graph, SIGNAL(edgeRemoved(int)), 	this, SLOT(graphEdgeRemoved(int)));
  connect( graph, SIGNAL(nodeRemoved(int)), 	this, SLOT(graphNodeRemoved(int)));
}

void Tests::connectSignalsNode(Node *node)
{
  connect( node, SIGNAL(nameChanged(QString)), 		this, SLOT(nodeNameChanged(QString)));
  connect( node, SIGNAL(colorChanged(QColor)), 		this, SLOT(nodeColorChanged(QColor)));
  connect( node, SIGNAL(positionChanged(QPointF)), 	this, SLOT(nodePositionChanged(QPointF)));
  connect( node, SIGNAL(indexChanged(int)), 		this, SLOT(nodeIndexChanged(int)));
}

void Tests::connectSignalsEdge(Edge *edge)
{
  connect( edge, SIGNAL(nameChanged(QString)), 	this, SLOT(edgeNameChanged(QString)));
  connect( edge, SIGNAL(lengthChanged(qreal)), 	this, SLOT(edgeLengthChanged(qreal)));
  connect( edge, SIGNAL(visitedChanged(bool)), 	this, SLOT(edgeVisitedChanged(bool)));
  connect( edge, SIGNAL(colorChanged(QColor)), 	this, SLOT(edgeColorChanged(QColor)));
}
