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
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QPen>

#include "graphicsitem_Edge.h"
#include "Node.h"
#include "Edge.h"
#include "Graph.h"

#include "graphicsitem_Node.h"
#include <KDebug>

EdgeItem::EdgeItem( Edge *edge, QGraphicsItem *parent)
     : QObject(0), QGraphicsLineItem(parent)
{
  
  _edge = edge;
  
  setCacheMode(DeviceCoordinateCache);
  setZValue(0);
  Graph *g = qobject_cast<Graph*>(_edge->parent());     
  _pen = new QPen();
  _pen->setStyle(Qt::SolidLine);
  _pen->setWidth(2);
  _pen->setBrush(g->color());
  _pen->setCapStyle(Qt::RoundCap);
  _pen->setJoinStyle(Qt::RoundJoin);
  setPen( (*_pen) );

  // ! Connect the Edge's Signals.
  connect (_edge, SIGNAL(changed()), this, SLOT(updateSlot()));
  connect (_edge, SIGNAL(removed()), this, SLOT(removed()));

  // ! Connect the Node connected to the edge's signals.
  connect (_edge->from(), SIGNAL(posChanged(QPointF)), this, SLOT(updatePos(QPointF)));
  connect (_edge->to(), SIGNAL(posChanged(QPointF)),   this, SLOT(updatePos(QPointF)));

  setLine(_edge->from()->pos().x(), _edge->from()->pos().y(),
	  _edge->to()->pos().x(), _edge->to()->pos().y());
}

 void EdgeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mousePressEvent(event);
 }

 void EdgeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mouseReleaseEvent(event);
 }

void EdgeItem::removed(){
  kDebug() << " Not Implemented Yet " << "removed";
}

void EdgeItem::updatePos(QPointF){
    setLine(_edge->from()->pos().x(), _edge->from()->pos().y(),
	    _edge->to()->pos().x(), _edge->to()->pos().y());
}

void EdgeItem::updateName(const QString& ){}
void EdgeItem::updateVisited(bool ){}
void EdgeItem::updateLength(qreal ){}
void EdgeItem::updateValue(qreal ){}
void EdgeItem::updateTotal(qreal ){}
void EdgeItem::updateColor(QColor ){}

#include "graphicsitem_Edge.moc"
