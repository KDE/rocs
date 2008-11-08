#include "MultiNode.h"
#include <KDebug>

Edge* MultiNode::connect(Node* n){
  Graph *g = qobject_cast<Graph*>(parent()); 
  if (! g->checkExistence(n) ) return 0;
  Edge *e = 0;
  e = new Edge(this, n, g);
  g->nodeCreatedEdge(e);
  
  kDebug() << "Simple Edge Created";
  return e;
}
