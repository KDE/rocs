#ifndef MULTINODE_H
#define MULTINODE_H

#include "Node.h"

class Edge;
class Graph;

class MultiNode : public Node{
public:
  MultiNode(qreal x, qreal y, Graph *parent) : Node(x,y,parent){}

public slots:
  Edge* connect(Node *n);
};

#endif