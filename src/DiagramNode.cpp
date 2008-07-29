/***************************************************************************
 *   Copyright (C) 2005-2006 by Tomaz Canabrava and Ugo Sangiori           *
 *   tomaz.canabrava@gmail.com                                             *
 *   ugorox@gmail.com                                                      *
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

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "DiagramEdge.h"
#include "DiagramNode.h"
#include "DiagramScene.h"
#include "Group.h"
#include "GraphEngine.h"

Node::Node(DiagramScene *diagramScene)     : diagramScene(diagramScene)
{
  selected     = false;
  isStartPoint = false;
  isEndPoint   = false;
  visited      = false;

  showIndex = true;

  ellipse = NULL;
  group   = NULL;

  setFlag(ItemIsMovable, true);
  setCacheMode(DeviceCoordinateCache);
  setZValue(1);

  colour = Qt::yellow;
}

Node::~Node(){
	if (ellipse) delete ellipse;
	
	while(edgeList.size()){
	    Edge *e = edgeList.first();
		if (e != 0) {
		  edgeList.removeAll(e);
		  delete e;
		  e = 0;
		}
	}
	
	if (hasGroup()){
		removeGroup();
	} 
	diagramScene -> removeNode(this);
}

bool Node::operator==(Node* n)
{
	if (this == n) return true;
	return false;
}

void Node::remove(){
	delete this;
}
bool Node::hasGroup(){
	if (group != NULL) return true;
	return false;
}
Group* Node::getGroup() const{
    return group; 
}

void Node::setGroup(Group *g){
	group = g;
}

void Node::removeGroup(){
	group->removeNode(this);
	group = NULL;
}

void Node::addEdge(Edge *edge)
{
  edgeList << edge;
  edge->adjust();
}

QRectF Node::boundingRect() const
{
  qreal adjust = 2;
  qreal x1 = -10 - adjust;
  qreal y2 = 23 + adjust;
  qreal x2 = y2 + 150;
  
  return QRectF(x1, x1, x2, y2);
}

QPainterPath Node::shape() const
{
  QPainterPath path;
  path.addEllipse(-10, -10, 20, 20);
  return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
  //! draw the Shadow
  painter->setPen(Qt::NoPen);
  painter->setBrush(Qt::darkGray);
  painter->drawEllipse(-7, -7, 20, 20);
  
  //! Draw the cute Ellipse with the gradient.
  QRadialGradient gradient(-3, -3, 10);
  if (option->state & QStyle::State_Sunken) 
  {
    gradient.setColorAt(0, colour.light(120));
    gradient.setColorAt(1, colour.light(180));
  } 
  else
  {
    gradient.setColorAt(1, colour.light(120));
    gradient.setColorAt(0, colour.light(180));
  }

  if(selected)
  {
	painter->setPen(Qt::DashLine);
	painter->setBrush(Qt::NoBrush);
	painter->drawRect(-10, -10, 20, 20);
  }

  painter->setBrush(gradient);
  painter->setPen(QPen(Qt::black, 0));
  painter->drawEllipse(-10, -10, 20, 20);

  //! Draw the letter `S` on the start and `E` on End.
  painter->setPen(QPen(Qt::black, 2));

  if (isStartPoint)    painter->drawText(-5,5,"S");
  else if (isEndPoint) painter->drawText(-5,5,"E");  
  else if ( showIndex ) painter->drawText(-5, 5, QString::number(index));

  if (name.length() > 0)
  {
    qreal adjust = 2;
    qreal x1 = -10 - adjust;
    qreal y1 = x1 - 5;
    qreal x2 = 23 + adjust;
    qreal y2 = x2;
    
    qreal xName = x2 - 11;
    qreal yName = y2 - 22;
    
    ellipse = new QRectF(x1, y1, x2, y2);
    painter->drawText(xName,yName, name);
  }
}

void Node::setName(QString s){
  name = s;
  update();
}

QScriptValue Node::edges()
{
	if (diagramScene -> engine == NULL) return QScriptValue();
	
	QScriptValue edgelist = diagramScene -> engine->newArray();
	int length = diagramScene -> engine -> evaluate("edges.length;").toInt32();
	
	foreach(Edge *e, edgeList)
	{
		for(int i = 0; i < length; i++)
		{
			QString n = "edges["+QString::number(i)+"];";
			QScriptValue edge = diagramScene -> engine -> evaluate(n);
			Edge *x = qobject_cast<Edge*>(edge.toQObject());
			
			if (x == e) edgelist.property("push").call(edgelist, QScriptValueList() << edge);
		} 
	}
	return edgelist;
}

void Node::removeEdge(Edge *e)
{
	edgeList.removeAt(edgeList.indexOf(e));
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
     switch (change) 
     {
     case ItemPositionHasChanged:
         foreach (Edge *edge, edgeList) edge->adjust();
     break;
     default: break;
     };

     return QGraphicsItem::itemChange(change, value);
 }

 void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mousePressEvent(event);
 }

 void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mouseReleaseEvent(event);
 }
