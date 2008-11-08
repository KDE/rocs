#ifndef ORIENTEDGRAPH_H
#define ORIENTEDGRAPH_H

#include "Graph.h"
#include "OrientedNode.h"

class OrientedGraph : public Graph{
  Q_OBJECT
  public:
    OrientedGraph(GraphDocument *parent = 0);

  public slots:
    Node* addNode(qreal x, qreal y);
    QList< QList <int> > ajdMatrix();

};
#endif
