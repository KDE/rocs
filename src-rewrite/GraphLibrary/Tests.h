#ifndef TESTS_H
#define TESTS_H

#include <QtTest/QtTest>
#include "GraphCollection.h"

class Graph;
class Node;
class Edge;

class Tests : public QObject
{
  Q_OBJECT
  public:
    Tests();
    ~Tests();

  private slots:
    void initTestCase();

    // SIMPLE GRAPH TESTS
    void testCreatingSimpleGraph();
    void testManipulatingSimpleGraph();
    void testCreatingSimpleGraphNodes();
    void testManipulatingSimpleGraphNodes();
    void testCreatingSimpleGraphEdges();
    void testManipulatingSimpleGraphEdges();
    void testRemovingSimpleGraphNodes();
    void testRemovingSimpleGraphEdges();
    void testDestructingSimpleGraph();

    // MULTI GRAPH TESTS
    void testCreatingMultiGraph();
    void testManipulatingMultiGraph();
    void testCreatingMultiGraphNodes();
    void testManipulatingMultiGraphNodes();
    void testCreatingMultiGraphEdges();
    void testManipulatingMultiGraphEdges();
    void testRemovingMultiGraphNodes();
    void testRemovingMultiGraphEdges();
    void testDestructingMultiGraph();

    // DIGRAPH TESTS
    void testCreatingDigraph();
    void testManipulatingDigraph();
    void testCreatingDigraphNodes();
    void testManipulatingDigraphNodes();
    void testCreatingDigraphEdges();
    void testManipulatingDigraphEdges();
    void testRemovingDigraphNodes();
    void testRemovingDigraphEdges();
    void testDestructingDigraph();

    // TEST CASE SPECIFIC
    void cleanupTestCase();
    void init();
    void cleanup();

  public slots:
    // Graph Collection Specific
    void graphCollectionNameChanged(QString n);
    void graphCollectionGraphCreated(Graph *g);
    void graphCollectionGraphDestroyed(int index);
    void graphCollectionHeightChanged(qreal height);
    void graphCollectionWidthChanged(qreal width);

    // Graph Specific
    void graphNameChanged(QString n);
    void graphColorChanged(QColor c);
    void graphNodeCreated(Node *n);
    void graphNodeRemoved(int index);
    void graphEdgeCreated(Edge *e);
    void graphEdgeRemoved(int index);

    // Node Specific
    void nodeNameChanged(QString n);
    void nodePositionChanged(QPointF p);
    void nodeColorChanged(QColor n);
    void nodeIndexChanged(int i);

    // Edge Specific
    void edgeNameChanged(QString n);
    void edgeLengthChanged(qreal l);
    void edgeVisitedChanged(bool v);
    void edgeColorChanged(QColor c);
  private:
    GraphCollection _graphCollection;

    void connectSignalsGraphCollection(GraphCollection *gc);
    void connectSignalsGraph(Graph *g);
    void connectSignalsNode(Node *n);
    void connectSignalsEdge(Edge *e);
};

#endif
