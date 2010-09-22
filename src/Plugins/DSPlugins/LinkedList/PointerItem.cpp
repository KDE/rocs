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

#include "PointerItem.h"

#include "NodeItem.h"
#include "GraphScene.h"
#include "Data.h"
#include "Pointer.h"
#include "DataType.h"
#include "math_constants.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QLine>
#include <QPolygonF>
#include <QtAlgorithms>
#include <KDebug>
#include <math.h>
#include <QGraphicsSimpleTextItem>
using namespace Rocs::LinkedList;


PointerItem::PointerItem( Pointer *edge, QGraphicsItem *parent)
        : QObject(0), QGraphicsPathItem(parent)
{
    _edge = edge;
    _loop = (_edge->from() == edge->to());
    _index = _edge->relativeIndex();
    _name = new QGraphicsSimpleTextItem(this);
    _value = new QGraphicsSimpleTextItem(this);
    setZValue( - _index);
    setFlag(ItemIsSelectable, true);
    connectSignals();
    updateAttributes();
    setPath(createCurves());
}

PointerItem::~PointerItem() {
//  dynamic_cast<GraphScene*>(scene())->removeGItem(this);
//  kDebug() << "Oriented Edge Removed";
}

void PointerItem::connectSignals() {
    connect(_edge, SIGNAL(changed()), this, SLOT(updatePos()));
    connect (_edge, SIGNAL(removed()), this, SLOT(remove()));
}

QPolygonF Rocs::LinkedList::PointerItem::createEndArrow() const{
  Datum * to = _edge->to();
  QPointF pos1(to->x()-to->width()*40-20, to->y());
  QPointF pos2(to->x()-to->width()*40, to->y());
  QLineF line (pos1, pos2);
  qreal arrowSize = 10.0;
  QPointF destArrowP1 = pos2 + QPointF(sin(- PI_3) * arrowSize,         cos(- PI_3) * arrowSize);
  QPointF destArrowP2 = pos2 + QPointF(sin(- Pi + PI_3) * arrowSize,    cos(- Pi + PI_3) * arrowSize);
  QPolygonF arrow(QPolygonF() << pos1 <<  pos2 << destArrowP2 << destArrowP1 << pos2);


  return arrow;

}

QPolygonF Rocs::LinkedList::PointerItem::createPath(const QPointF& pos1, const QPointF& pos2) const{
  QPolygonF path;

  if (pos1.x() > pos2.x()){
      qreal mid = (pos2.y() + pos1.y()) /2;
      path.append(pos1);
      path.append(QPointF(pos1.x(), mid));
      path.append(QPointF(pos2.x(), mid));
      path.append(pos2);
  }else{
      qreal mid = (pos2.x() + pos1.x()) /2;
      path.append(pos1);
      path.append(QPointF(mid, pos1.y()));
      path.append(QPointF(mid, pos2.y()));
      path.append(pos2);
  }
  return path;


}



QPainterPath PointerItem::createCurves() const {
    if (_edge->from() == 0 || _edge->to() == 0){
//         _edge->self_remove();
        return QPainterPath();
    }

    QPointF Pos1(_edge->from()->x()+_edge->to()->width()*40 + 20, _edge->from()->y());


    QPointF Pos2(_edge->to()->x()-_edge->to()->width()*40 - 20, _edge->to()->y());
    QPolygonF arrow = createEndArrow();

	QPainterPath s;
    s.moveTo(_edge->from()->x(), _edge->from()->y());
	s.lineTo(Pos1);
	s.addPolygon(createPath(Pos1,Pos2));
    s.addPolygon(arrow);
	return s;

}

void PointerItem::mousePressEvent(QGraphicsSceneMouseEvent */*event*/) {
}

void PointerItem::mouseReleaseEvent(QGraphicsSceneMouseEvent */*event*/) {
}

void PointerItem::remove() {
    scene()->removeItem(this);
    deleteLater();
}

void PointerItem::updatePos() {
//     GraphScene* gScene = dynamic_cast<GraphScene*>(scene());
//     if (gScene->hideEdges()) {
//         gScene->updateAfter(this);
//     }
    if (_edge->from() == 0 || _edge->to() == 0){
//         _edge->self_remove();
        return;
    }
    QLine q(_edge->from()->x(), _edge->from()->y(),    _edge->to()->x(),  _edge->to()->y());
    qreal size = sqrt( pow(qreal(q.dx()), 2) + pow(qreal(q.dy()), 2));
    if (_edge->from() != _edge->to() && size < 20  ) {
        setPath(QPainterPath());
    } else {
        setPath(createCurves());
    }
    updateAttributes();
}

void PointerItem::updateAttributes() {
    Qt::PenStyle s;
    if     (_edge->style() == "dash"	){ s = Qt::DashLine;    }
    else if(_edge->style() == "dot"	){ s = Qt::DotLine;     }
    else if(_edge->style() == "dash dot"){ s = Qt::DashDotLine; }
    else if(_edge->style() == "solid"	){ s = Qt::SolidLine;   }
    else                                 { s = Qt::SolidLine;   }

    setPen(QPen(QBrush(QColor(_edge->color())), _edge->width(), s,Qt::RoundCap, Qt::RoundJoin));
    _value->hide();
    _name->hide();
    QPointF middle = path().pointAtPercent(0.5);
    _name->setText(_edge->name());
    _value->setText(_edge->value());

    if (_edge->from() == _edge->to()) {
        qreal x1 = boundingRect().x() + boundingRect().width() + 5;
        qreal y1 = boundingRect().y() + (boundingRect().height()/2) - 10;
        _name->setPos(x1, y1);
        _value->setPos(x1, y1+14);
    }
    else {
        _name->setPos(middle.x() - _name->boundingRect().width()/2, middle.y());
        _value->setPos(middle.x() - _name->boundingRect().width()/2, middle.y()-14);
    }

    if (_edge->showValue()) {
        _value->show();
    }
    if (_edge->showName()) {
        _name->show();
    }
    update();
}

void PointerItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/){
  if ( isSelected() ){
    painter->setPen(QPen(Qt::black, _edge->width(),  Qt::DotLine));
  }
  painter->drawPath(createCurves());
}

#include "PointerItem.moc"
