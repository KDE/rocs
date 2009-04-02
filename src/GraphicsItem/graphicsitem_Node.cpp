/* This file is part of Rocs,
	 Copyright (C) 2008 by:
	 Tomaz Canabrava <tomaz.canabrava@gmail.com>
	 Ugo Sangiori <ugorox@gmail.com>

	 Rocs is free software; you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation; either version 2 of the License, or
	 (at your option) any later version.

	 Rocs is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with Step; if not, write to the Free Software
	 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
*/


#include "graphicsitem_Node.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QPen>
#include <QBrush>

#include "node.h"
#include "graph.h"
#include "graphicsitem_Edge.h"
#include "graphicsitem_OrientedEdge.h"
#include <KDebug>

NodeItem::NodeItem(Node *node, QGraphicsItem *parent)
		 : QObject(0), QGraphicsItem(parent)
{
	_node = node;
	QPointF pos( _node -> property("x").toDouble() ,_node->property("y").toDouble() );
	setPos( pos );
	setCacheMode(NoCache);
	setZValue(1);
	setFlag(ItemIsSelectable, true);

	connect (_node, SIGNAL(removed()), this, SLOT(removed()));
}

 QRectF NodeItem::boundingRect() const{ 
  return QRectF(-12, -12 , 25, 25); 
}

 QPainterPath NodeItem::shape() const{
	 QPainterPath path;
	 path.addEllipse(-10, -10, 20, 20);
	 return path;
 }

 void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *){

	if (isSelected()){
		kDebug() << "IT'S BEING DRAWNED!";
		QPen pen;
		pen.setStyle(Qt::DotLine);
		pen.setWidth(1);
		pen.setBrush(Qt::black);

		painter->setBrush(QBrush());
		painter->setPen(pen);
		painter->drawRect(QRectF(-11 , -11 , 24 , 24 ));
	}

	 painter->setPen(Qt::NoPen);
	 QColor color = _node->property("color").value<QColor>();
	 painter->setBrush( color.dark(240) );

	 painter->drawEllipse(-7, -7, 20, 20);
	 QRadialGradient gradient(-3, -3, 10);
	 if (option->state & QStyle::State_Sunken) {
		 gradient.setColorAt(0, color.light(240));
		 gradient.setColorAt(1, color);
	 } else {
		 gradient.setColorAt(0, color.light(240));
		 gradient.setColorAt(1, color);
	 }
	kDebug() << "Called";
	painter->setBrush(gradient);
	painter->setPen(QPen(color, 2));
	painter->drawEllipse(-10, -10, 20, 20);


 }

void NodeItem::updatePos(QPointF pos){
	_node->setProperty("x", pos.x());
	_node->setProperty("y", pos.y());
	setPos( pos );

	Graph *g = qobject_cast<Graph*>(_node->parent());
	if (!g->directed()){
		foreach(QGraphicsItem* i, _edges){
			EdgeItem *edgeItem = qgraphicsitem_cast<EdgeItem*>(i);
			edgeItem->updatePos();
		}
	}
	else{
		foreach(QGraphicsItem* i, _edges){
			OrientedEdgeItem *edgeItem = qgraphicsitem_cast<OrientedEdgeItem*>(i);
			edgeItem->updatePos();
		}
	}
}

void NodeItem::updateName(const QString& ){}
void NodeItem::updateVisited(bool ){}
void NodeItem::updateValue(qreal ){}
void NodeItem::updateTotal(qreal ){}
void NodeItem::updateColor(QColor ){}

void NodeItem::removed(){
	kDebug() << " Not Implemented Yet " << "removed";
}

void NodeItem::addEdge(QGraphicsItem *e){
	_edges.append(e);
}
#include "graphicsitem_Node.moc"
