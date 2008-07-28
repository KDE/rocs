#include "Group.h"
#include "DiagramScene.h"
#include "GraphEngine.h"

Group::Group(DiagramScene *scene, QList<Node*> nodeList)
{
  diagramScene = scene;
  if (nodeList.size() == 0) return;
  this->nodeList = nodeList;
  foreach(Node *n, nodeList){
    n->setGroup(this);
  }
  box = 0;

  rePaint();
}

Group::~Group(){
	foreach(Node *n, nodeList){
		n -> setGroup(NULL);
	}
	nodeList.clear();
	delete box;
	diagramScene = 0;
}

QScriptValue Group::nodes(){
  QScriptValue node_list = diagramScene -> engine -> newArray();

  //! Puts every Edge inside of the scene on the edges array;
  int length =  diagramScene -> engine -> evaluate("nodes.length;").toInt32();
  QList<Node*> selectedNodes = diagramScene -> getSelectedNodes();
  foreach(Node *_n, nodeList)
  {
    for(int i = 0; i < length; i++)
    {
      QString n = "nodes["+QString::number(i)+"];";
      QScriptValue node =  diagramScene -> engine -> evaluate(n);
      Node *_x = qobject_cast<Node*>(node.toQObject());

      if (_n == _x)
      {
        node_list.property("push").call(node_list, QScriptValueList() << node);
      }
    }
  }
  return node_list;
}

void Group::removeNode(Node *n)
{
  nodeList.removeAt(nodeList.indexOf(n));
  if (nodeList.size() != 0){
    rePaint();
	return;	
  }
  
  diagramScene->removeGroup(this);
  delete this;
}

void Group::rePaint(){

  double x1, y1, x2, y2;
  Node *n1 = nodeList.first();
  x1 = n1->pos().x();
  y1 = n1->pos().y();
  x2 = x1;
  y2 = y1;

  foreach(Node *n, nodeList)
  {
    if (x1 > n -> pos().x()) x1 = n -> pos().x(); // menor x
    if (y1 > n -> pos().y()) y1 = n -> pos().y(); // menor y
    if (x2 < n -> pos().x()) x2 = n -> pos().x(); // maior x
    if (y2 < n -> pos().y()) y2 = n -> pos().y(); // maior y
  }
  box = new QRectF(0,0, x2 - x1 + 40, y2 - y1+ 40);
  setRect((*box));
  setPos(x1-20,y1-20);
}
