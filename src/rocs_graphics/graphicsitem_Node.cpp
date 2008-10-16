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

#include "graphicsitem_Node.h"
#include "libgraph_Node.h"
#include <KDebug>

NodeItem::NodeItem(libgraph::Node *node, QGraphicsItem *parent)
     : QObject(0), QGraphicsItem(parent)
{
    _node = node;
    setPos( _node -> position() );
    setCacheMode(DeviceCoordinateCache);
    setZValue(1);
  
  connect (_node, SIGNAL(nameChanged(QString)), 
      this, SLOT(changeName(QString)));
  connect (_node, SIGNAL(positionChanged(QPointF)), 
      this, SLOT(changePosition(QPointF)));
  connect (_node, SIGNAL(colorChanged(QColor)), 
      this, SLOT(changeColor(QColor)));
  connect (_node, SIGNAL(indexChanged(int)), 
      this, SLOT(changeIndex(int)));
  connect (_node, SIGNAL(visitedChanged(bool)), 
      this, SLOT(changeVisited(bool)));
  connect (_node, SIGNAL(removed()), 
      this, SLOT(removed()));
}

 QRectF NodeItem::boundingRect() const
 {
     qreal adjust = 2;
     return QRectF(-10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
 }

 QPainterPath NodeItem::shape() const
 {
     QPainterPath path;
     path.addEllipse(-10, -10, 20, 20);
     return path;
 }

 void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
 {
     painter->setPen(Qt::NoPen);
     painter->setBrush(Qt::darkGray);
     painter->drawEllipse(-7, -7, 20, 20);

     QRadialGradient gradient(-3, -3, 10);
     if (option->state & QStyle::State_Sunken) {
         gradient.setCenter(3, 3);
         gradient.setFocalPoint(3, 3);
         gradient.setColorAt(1, QColor(_node->color()).light(120));
         gradient.setColorAt(0, QColor(_node->color()));
     } else {
         gradient.setColorAt(0,_node->color());
         gradient.setColorAt(1,_node->color());
     }
     painter->setBrush(gradient);
     painter->setPen(QPen(Qt::black, 0));
     painter->drawEllipse(-10, -10, 20, 20);
 }

 void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mousePressEvent(event);
 }

 void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mouseReleaseEvent(event);
 }

void NodeItem::changeName(QString){
  kDebug() << " Not Implemented Yet " << "changeName";
}
  
void NodeItem::changePosition(QPointF position){
  QGraphicsItem::setPos(position);
  kDebug() << "Position of node" << _node->index() << "changed";
}
  

void NodeItem::changeColor(QColor){
  update();
  kDebug() << "Color of node" << _node ->index() << "changed";
}

void NodeItem::changeIndex(int){
  kDebug() << " Not Implemented Yet " << "changeIndex";
}


void NodeItem::changeVisited(bool){
  kDebug() << " Not Implemented Yet " << "changeVisited";
}

void NodeItem::removed(){
  kDebug() << " Not Implemented Yet " << "removed";
}

#include "graphicsitem_Node.moc"