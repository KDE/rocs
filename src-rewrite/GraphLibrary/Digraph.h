#ifndef DIGRAPH_H
#define DIGRAPH_H

#include "Graph.h"

class Node;
class Edge;
class QPointF;

class Digraph : public Graph
{
public:
  Digraph(GraphCollection *parent);
  ~Digraph();
  Edge* addEdge(Node *from, Node *to);
  Node* createNode(QPointF position);
};

#endif
