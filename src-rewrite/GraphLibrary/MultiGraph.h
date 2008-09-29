#ifndef MULTIGRAPH_H
#define MULTIGRAPH_H

#include "Graph.h"

class Edge;
class Node;
class GraphCollection;

class MultiGraph : public Graph
{
public:
  MultiGraph(GraphCollection *parent);
  ~MultiGraph();
  Edge* createEdge(Node *from, Node *to);
  Node* createNode(QPointF position);
};

#endif
