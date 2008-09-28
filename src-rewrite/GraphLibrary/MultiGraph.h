#ifndef MULTIGRAPH_H
#define MULTIGRAPH_H

#include "Graph.h"

class Edge;
class Node;

class MultiGraph : public Graph
{
public:
  MultiGraph();
  ~MultiGraph();
  Edge* createEdge(Node *from, Node *to);
  Node* createNode(QPointF position);
};

#endif
