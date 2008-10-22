#ifndef ORIENTEDNODE_H
#define ORIENTEDNODE_H

#include "Node.h"

class Edge;
class Graph;

class OrientedNode : public Node{
public:
  OrientedNode(qreal x, qreal y, Graph *parent) : Node(x,y,parent){}

public slots:
  bool isAdjacent(Node* n);
  QList<Edge*> adjEdges(Node *n);
  QList<Node*> adjNodes();
  Node* adjNode();
  Node* unvisitedAdjNode();
  Edge* adjEdge();
  Edge* unvisitedAdjEdge();
  Edge* connect(Node *n);
};

#endif