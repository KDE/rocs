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


#include "LinkedListPointerItem.h"

#include "DataItem.h"
#include "GraphScene.h"
#include "Data.h"
#include "Pointer.h"
#include "DataStructure.h"
#include "math_constants.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>
#include <QLine>
#include <QPolygonF>
#include <QtAlgorithms>
#include <KDebug>
#include <math.h>


LinkedListPointerItem::LinkedListPointerItem( Pointer *edge, QGraphicsItem *parent)
        : PointerItem(edge,parent)
{
    setPath(createCurves());
}

LinkedListPointerItem::~LinkedListPointerItem(){}


QPolygonF LinkedListPointerItem::createEndArrow() const{
  Data * to = pointer()->to();
  QPointF pos1(to->x()-to->width()*40-20, to->y());
  QPointF pos2(to->x()-to->width()*40, to->y());
  QLineF line (pos1, pos2);
  qreal arrowSize = 10.0;
  QPointF destArrowP1 = pos2 + QPointF(sin(- PI_3) * arrowSize,         cos(- PI_3) * arrowSize);
  QPointF destArrowP2 = pos2 + QPointF(sin(- Pi + PI_3) * arrowSize,    cos(- Pi + PI_3) * arrowSize);

  return QPolygonF() << pos1 <<  pos2 << destArrowP2 << destArrowP1 << pos2;

}

QPolygonF LinkedListPointerItem::createPath(const QPointF& pos1, const QPointF& pos2) const{
  QPolygonF path;
  path.append(pos1);
  if (pos1.x() > pos2.x()){
      qreal mid = (pos2.y() + pos1.y()) /2;
      path.append(QPointF(pos1.x(), mid));
      path.append(QPointF(pos2.x(), mid));
  }else{
      qreal mid = (pos2.x() + pos1.x()) /2;
      path.append(QPointF(mid, pos1.y()));
      path.append(QPointF(mid, pos2.y()));
  }
  path.append(pos2);
  return path;
}

QPainterPath LinkedListPointerItem::createCurves()  {
    if (pointer()->from() == 0 || pointer()->to() == 0){
        return QPainterPath();
    }

    QPointF Pos1(pointer()->from()->x()+pointer()->to()->width()*40 + 20, pointer()->from()->y());
    QPointF Pos2(pointer()->to()->x()-pointer()->to()->width()*40 - 20, pointer()->to()->y());

    QPainterPath s;
    s.moveTo(pointer()->from()->x(), pointer()->from()->y());
    s.lineTo(Pos1);
    s.addPolygon(createPath(Pos1,Pos2));
    s.addPolygon(createEndArrow());
    return s;

}

#include "LinkedListPointerItem.moc"
