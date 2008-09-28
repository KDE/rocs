#ifndef DINODE_H
#define DINODE_H

#include "Node.h"

class Edge;
class Graph;

class DiNode : public Node 
{
  public:
    DiNode(QPointF position, Graph *parent);
    ~DiNode();
    bool isConnected(Node *to);
    void addEdge(Edge *e);
};

#endif
