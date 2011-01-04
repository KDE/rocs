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


#include "DataItem.h"
#include "GraphScene.h"
#include "Data.h"
#include "Pointer.h"
#include "DataStructure.h"
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

OrientedEdgeItem::OrientedEdgeItem( Pointer *pointer, QGraphicsItem *parent)
        : QObject(0), QGraphicsPathItem(parent)
{
    _pointer = pointer;
    _loop = (_pointer->from() == pointer->to());
    _index = _pointer->relativeIndex();
    _name = new QGraphicsSimpleTextItem(this);
    _value = new QGraphicsSimpleTextItem(this);
    setZValue( - _index);
    setFlag(ItemIsSelectable, true);
    connectSignals();
    updateAttributes();
    setPath(createCurves());
}

OrientedEdgeItem::~OrientedEdgeItem() {
//  dynamic_cast<GraphScene*>(scene())->removeGItem(this);
//  kDebug() << "Oriented Edge Removed";
}

void OrientedEdgeItem::connectSignals() {
    connect(_pointer, SIGNAL(changed()), this, SLOT(updatePos()));
    connect (_pointer, SIGNAL(removed()), this, SLOT(remove()));
}

QPolygonF OrientedEdgeItem::createArrow(const QPointF& Pos1, const QPointF& Pos2) const {
    QLineF line(Pos1, Pos2);
    qreal angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0) {
        angle = TwoPi - angle;
    }
    qreal arrowSize = 10;

    QPointF destArrowP1 = Pos2 + QPointF(sin(angle - PI_3) * arrowSize,         cos(angle - PI_3) * arrowSize);
    QPointF destArrowP2 = Pos2 + QPointF(sin(angle - Pi + PI_3) * arrowSize,    cos(angle - Pi + PI_3) * arrowSize);
    QPolygonF arrow(QPolygonF() <<  destArrowP1 << Pos2 << destArrowP2);

    /// Put the arrow on the center of the nodes.
    qreal x = Pos2.x() - Pos1.x();
    qreal y = Pos2.y() - Pos1.y();

    x -= x/2;
    y -= y/2;
    arrow.translate(-x, -y );
    return arrow;
}

QPainterPath OrientedEdgeItem::createLoop(QPointF pos) const {
    QPainterPath p;
    DataStructure *g = qobject_cast<DataStructure*>(_pointer->parent());
    qreal size = 30 + (20 * _index);
    qreal angle = atan2((pos.x() - g->relativeCenter().x()), (pos.y() - g->relativeCenter().y()));
    qreal posx = (pos.x()-(((size/2) * sin(angle)) * -1)-(size/2));
    qreal posy = (pos.y()+(((size/2) * cos(angle)))-(size/2));
    p.addEllipse( posx, posy, size, size);
    return p;
}

QPainterPath OrientedEdgeItem::createCurves() const {
    if (_pointer->from() == 0 || _pointer->to() == 0){
	_pointer->self_remove();
        return QPainterPath();
    }

    QPointF Pos1(_pointer->from()->x(), _pointer->from()->y());

    if ( _loop ) return createLoop(Pos1);

    QPointF Pos2(_pointer->to()->x(), _pointer->to()->y());
    QPolygonF arrow = createArrow(Pos1,  Pos2);

    if (Pos1.x() > Pos2.x()) {
        qSwap(Pos1, Pos2);
    }

//     if (! _pointer->dataStructure()->directed()){
// 	QPainterPath p;
// 	p.moveTo(Pos1);
// 	p.lineTo(Pos2);
// 	return p;
//     }

    qreal x = Pos2.x() - Pos1.x();
    qreal y = Pos2.y() - Pos1.y();
    qreal angle = atan2(y,x);

    /// Calculate the size of the inclination on the curve.
    qreal theta = angle + PI_2;
    qreal finalX = cos(theta);
    qreal finalY = sin(theta);
    int index = _index;

    if (index & 1) { // If the number is Odd.
        ++index;
        finalX *= (-1);
        finalY *= (-1);
    }

    qreal size = sqrt(pow((x*0.1),2) + pow((y*0.1),2)) * index;

    finalX *= size;
    finalY *= size;
    finalX += Pos1.x() + x/2;
    finalY += Pos1.y() + y/2;

    /// Draw the Arc.
    QPainterPath p;
    p.moveTo(Pos1);
    p.quadTo(finalX, finalY, Pos2.x(), Pos2.y());

    /// puts the arrow on its correct position
    QPointF middle = p.pointAtPercent(0.5);

    x = Pos1.x() + (Pos2.x() - Pos1.x())/2;
    y = Pos1.y() + (Pos2.y() - Pos1.y())/2;
    QLineF line2(QPointF(x,y) , middle);
    arrow.translate(+line2.dx() , +line2.dy());
    p.addPolygon(arrow);

    return p;
}

void OrientedEdgeItem::mousePressEvent(QGraphicsSceneMouseEvent */*event*/) {
}

void OrientedEdgeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent */*event*/) {
}

void OrientedEdgeItem::remove() {
    scene()->removeItem(this);
    deleteLater();
}

void OrientedEdgeItem::updatePos() {
    GraphScene* gScene = dynamic_cast<GraphScene*>(scene());
    if (gScene->hideEdges()) {
        gScene->updateAfter(this);
    }
    QLine q(_pointer->from()->x(), _pointer->from()->y(),    _pointer->to()->x(),  _pointer->to()->y());
    qreal size = sqrt( pow(q.dx(), 2) + pow(q.dy(), 2));
    if (_pointer->from() != _pointer->to() && size < 20  ) {
        setPath(QPainterPath());
    } else {
        setPath(createCurves());
    }
    updateAttributes();
}

void OrientedEdgeItem::updateAttributes() {
    Qt::PenStyle s;
    if     (_pointer->style() == "dash"	){ s = Qt::DashLine;    }
    else if(_pointer->style() == "dot"	){ s = Qt::DotLine;     }
    else if(_pointer->style() == "dash dot"){ s = Qt::DashDotLine; }
    else if(_pointer->style() == "solid"	){ s = Qt::SolidLine;   }
    else                                 { s = Qt::SolidLine;   }

    setPen(QPen(QBrush(QColor(_pointer->color())), _pointer->width(), s,Qt::RoundCap, Qt::RoundJoin));
    _value->hide();
    _name->hide();
    QPointF middle = path().pointAtPercent(0.5);
    _name->setText(_pointer->name());
    _value->setText(_pointer->value());

    if (_pointer->from() == _pointer->to()) {
        qreal x1 = boundingRect().x() + boundingRect().width() + 5;
        qreal y1 = boundingRect().y() + (boundingRect().height()/2) - 10;
        _name->setPos(x1, y1);
        _value->setPos(x1, y1+14);
    }
    else {
        _name->setPos(middle.x() - _name->boundingRect().width()/2, middle.y());
        _value->setPos(middle.x() - _name->boundingRect().width()/2, middle.y()-14);
    }

    if (_pointer->showValue()) {
        _value->show();
    }
    if (_pointer->showName()) {
        _name->show();
    }
    update();
}

void OrientedEdgeItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget){
  Q_UNUSED(option);
  Q_UNUSED(widget);
  
  if ( isSelected() ){
    painter->setPen(QPen(Qt::black, _pointer->width(),  Qt::DotLine));
  }
  painter->drawPath(createCurves());
}
