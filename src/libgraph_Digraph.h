#ifndef DIGRAPH_H
#define DIGRAPH_H

#include "libgraph_Graph.h"

class QPointF;

namespace libgraph{

class Node;
class Edge;


class Digraph : public Graph
{
public:
  /*! Default Constructor */
  Digraph(GraphDocument *parent);
  
  /*! Default Destructor */
  ~Digraph();

  /*! Add a edge between two nodes */
  Edge* createEdge(Node *from, Node *to);

  /*! Creates a new Node on Position position */
  Node* createNode(QPointF position);
};

};
#endif
