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
#include "libgraph_Node.h"
#include "libgraph_Edge.h"
#include "graphicsitem_Node.h"
#include <KDebug>

EdgeItem::EdgeItem(libgraph::Edge *edge, QGraphicsItem *parent)
     : QObject(0), QGraphicsLineItem(parent)
{
    _edge = edge;
    setCacheMode(DeviceCoordinateCache);
    setZValue(0);
    
 _pen = new QPen();
 _pen->setStyle(Qt::SolidLine);
 _pen->setWidth(2);
 _pen->setBrush(Qt::black);
 _pen->setCapStyle(Qt::RoundCap);
 _pen->setJoinStyle(Qt::RoundJoin);
 setPen( (*_pen) );

  // ! Connect the Edge's Signals.
  connect (_edge, SIGNAL(nameChanged(QString)), 
      this, SLOT(changeName(QString)));
  connect (_edge, SIGNAL(colorChanged(QColor)), 
      this, SLOT(changeColor(QColor)));
  connect (_edge, SIGNAL(visitedChanged(bool)), 
      this, SLOT(changeVisited(bool)));
  connect (_edge, SIGNAL(removed()), 
      this, SLOT(removed()));

  // ! Connect the Node connected to the edge's signals.
  connect (_edge->from(), SIGNAL(positionChanged(QPointF)), 
      this, SLOT(update2()));
  connect (_edge->to(), SIGNAL(positionChanged(QPointF)), 
      this, SLOT(update2()));

  setLine(_edge->from()->position().x(), _edge->from()->position().y(),
	  _edge->to()->position().x(), _edge->to()->position().y());
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

void EdgeItem::changeName(QString){
  kDebug() << " Not Implemented Yet " << "changeName";
}

void EdgeItem::changeColor(QColor){
  update();
  kDebug() << "Color of edge changed";
}

void EdgeItem::changeVisited(bool){
  kDebug() << " Not Implemented Yet " << "changeVisited";
}

void EdgeItem::changeLength(qreal){
  kDebug() << " Not Implemented Yet " << "changeVisited";
}

void EdgeItem::removed(){
  kDebug() << " Not Implemented Yet " << "removed";
}

void EdgeItem::update2(){
    setLine(_edge->from()->position().x(), _edge->from()->position().y(),
	    _edge->to()->position().x(), _edge->to()->position().y());
}

#include "graphicsitem_Edge.moc"