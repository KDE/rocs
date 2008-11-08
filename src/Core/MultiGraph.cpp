#include "MultiGraph.h"
#include "MultiNode.h"
#include <QColor>


MultiGraph::MultiGraph(GraphDocument *parent)  : Graph(parent){
  _color = Qt::red;
}

Node* MultiGraph::addNode(qreal x, qreal y){
  Node *tmpNode = new MultiNode(x, y, this);
  emit nodeCreated(tmpNode);
  return tmpNode;
}
