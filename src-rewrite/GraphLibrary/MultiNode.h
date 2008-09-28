#ifndef MULTINODE_H
#define MULTINODE_H

#include "Node.h"

class MultiNode : public Node
{
public:
  MultiNode(QPointF position, Graph *parent);
  ~MultiNode();
  bool isConnected(Node *to);
  void addEdge(Edge *e);
};

#endif
