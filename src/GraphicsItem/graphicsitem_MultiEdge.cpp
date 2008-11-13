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

#include "graphicsitem_MultiEdge.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QPen>
#include <QGraphicsLineItem>
#include <QPainterPath>

#include "Node.h"
#include "Edge.h"
#include "Graph.h"
#include "math_constants.h"
#include <math.h>

#include "graphicsitem_Node.h"


#include <KDebug>
#include <QPen>

MultiEdgeItem::MultiEdgeItem( Edge *edge, QGraphicsItem *parent)
     : QObject(0), QGraphicsPathItem(parent)
{
  
  _edge = edge;
  _loop = (_edge->from() == edge->to()) ? true : false;
  _index = _edge->relativeIndex();

  setCacheMode(DeviceCoordinateCache);
  setZValue(0);
  
  connectSignals();
  setupPen();

  QPainterPath p = createCurves(_edge->from()->pos(), _edge->to()->pos(), _index);
  setPath(p);
  kDebug() << "Multi Edge Created, Index = " << _index;
}

void MultiEdgeItem::connectSignals(){
  connect (_edge, SIGNAL(changed()), this, SLOT(updateSlot()));
  connect (_edge, SIGNAL(removed()), this, SLOT(removed()));

  // ! Connect the Node connected to the edge's signals.
  connect (_edge->from(), SIGNAL(posChanged(QPointF)), this, SLOT(updatePos(QPointF)));
  connect (_edge->to(), SIGNAL(posChanged(QPointF)),   this, SLOT(updatePos(QPointF)));
}

void MultiEdgeItem::setupPen(){
  QPen pen;
  pen.setStyle(Qt::SolidLine);
  pen.setWidth(2);
  pen.setBrush(_edge->color());
  pen.setCapStyle(Qt::RoundCap);
  pen.setJoinStyle(Qt::RoundJoin);
  setPen( pen );    // ! Connect the Edge's Signals.
}

QPainterPath MultiEdgeItem::createCurves(QPointF Pos1, QPointF Pos2, int index){
  QPainterPath p;
  if ( _loop ){
      qreal size = 20 + (10 * _index);
      qreal hypotenuse = sqrt( pow(size, 2) + pow(size, 2)) / 2;
      qreal correctPos = ( hypotenuse - (size/2));
      p.addEllipse( Pos1.x() - correctPos , Pos1.y() - correctPos, size, size);
  }
  else if (_index == 0){
      p.moveTo(Pos1);
      p.lineTo(Pos2);
  }
  else{
    if (Pos1.x() > Pos2.x()){
      QPointF p3 = Pos2;  
      Pos2 = Pos1;
      Pos1 = p3;
    }

    qreal x = Pos2.x() - Pos1.x();
    qreal y = Pos2.y() - Pos1.y();

    qreal theta = atan2(y,x) + PI_2 ;  
    qreal finalX = cos(theta);
    qreal finalY = sin(theta);

    if (index & 1){ // If number is Odd.
      index++;
      finalX *= (-1);
      finalY *= (-1);
    }

    qreal size = sqrt(pow((x*0.1),2) + pow((y*0.1),2)) * index;
    
    finalX *= size;
    finalY *= size;
  
    finalX += Pos1.x() + (x)/2;
    finalY += Pos1.y() + (y)/2;

    p.moveTo(Pos1);
    p.quadTo(finalX, finalY, Pos2.x(), Pos2.y());
  }
  return p;
}

 void MultiEdgeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mousePressEvent(event);
 }

 void MultiEdgeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mouseReleaseEvent(event);
 }

void MultiEdgeItem::removed(){
  kDebug() << " Not Implemented Yet " << "removed";
}

void MultiEdgeItem::updatePos(QPointF){
  QPainterPath p = createCurves(_edge->from()->pos(), _edge->to()->pos(), _index);
  setPath(p);
}

void MultiEdgeItem::updateName(const QString& ){}
void MultiEdgeItem::updateVisited(bool ){}
void MultiEdgeItem::updateLength(qreal ){}
void MultiEdgeItem::updateValue(qreal ){}
void MultiEdgeItem::updateTotal(qreal ){}
void MultiEdgeItem::updateColor(QColor ){}

#include "graphicsitem_MultiEdge.moc"
