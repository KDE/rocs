/***************************************************************************
 *   Copyright (C) 2005-2006 by Anne-Marie Mahfouf   *
 *   tomaz.canabrava@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

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
