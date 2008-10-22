#include "OrientedGraph.h"
#include "OrientedNode.h"
#include "Edge.h"

OrientedGraph::OrientedGraph(GraphDocument *parent)  : Graph(parent){
  _color = Qt::darkBlue;
}

QList< QList <int> > OrientedGraph::ajdMatrix(){
  QList< QList< int > > matrix;
  int i, j;
  int size = _nodes.size();

  for( i = 0; i < size; i++){
	QList<int> vector;
    matrix.append( vector );
    for( j = 0; j < size; j++){
      if ( _nodes[i]->isAdjacent( _nodes[j] )){
        matrix[i].append( _nodes[i]->adjEdges(_nodes[j]).size());
      }
    }
  }

  return matrix;
}

Node* OrientedGraph::addNode(qreal x, qreal y){
  Node *tmpNode = new OrientedNode(x, y, this);
  emit nodeCreated(tmpNode);
  return tmpNode;
}

