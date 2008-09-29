#ifndef MULTIGRAPH_H
#define MULTIGRAPH_H

#include "libgraph_Graph.h"

namespace libgraph
{

class Edge;
class Node;
class GraphDocument;

class MultiGraph : public Graph
{
public:
  MultiGraph(GraphDocument *parent);
  ~MultiGraph();
  Edge* createEdge(Node *from, Node *to);
  Node* createNode(QPointF position);
};

};
#endif
