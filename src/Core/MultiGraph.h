#ifndef MULTIGRAPH_H
#define MULTIGRAPH_H

#include "Graph.h"
#include "MultiNode.h"

class MultiGraph : public Graph{
  Q_OBJECT
  public:
    MultiGraph(GraphDocument *parent = 0);

  public slots:
    Node* addNode(qreal x, qreal y);

};

#endif