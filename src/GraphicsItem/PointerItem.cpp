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

PointerItem::PointerItem( Pointer *pointer, QGraphicsItem *parent)
        : QObject(0), QGraphicsPathItem(parent)
{
    _pointer = pointer;
    _index = _pointer->relativeIndex();
    _name = new QGraphicsSimpleTextItem(this);
    _value = new QGraphicsSimpleTextItem(this);
    setZValue( - _index);
    setFlag(ItemIsSelectable, true);
    connectSignals();
    updateAttributes();
}

PointerItem::~PointerItem() {
  //dynamic_cast<GraphScene*>(scene())->removeGItem(this);
  qDebug() << "Oriented Edge Removed";
}

void PointerItem::connectSignals() {
    connect(_pointer, SIGNAL(changed()), this, SLOT(updatePos()));
    connect (_pointer, SIGNAL(removed()), this, SLOT(remove()));
}

void PointerItem::mousePressEvent(QGraphicsSceneMouseEvent */*event*/) {
}

void PointerItem::mouseReleaseEvent(QGraphicsSceneMouseEvent */*event*/) {
}

void PointerItem::remove() {
    qDebug() << "Removing the oriented edge.";
    if (scene()){
        scene()->removeItem(this);
    }
    deleteLater();
}

void PointerItem::updatePos() {
    QLine q(_pointer->from()->x(), _pointer->from()->y(),    _pointer->to()->x(),  _pointer->to()->y());
    qreal size = sqrt( pow(q.dx(), 2) + pow(q.dy(), 2));
    if (_pointer->from() != _pointer->to() && size < 20  ) {
        setPath(QPainterPath());
    } else {
        setPath(createCurves());
    }
    updateAttributes();
}

void PointerItem::updateAttributes() {
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

void PointerItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget){
  Q_UNUSED(option);
  Q_UNUSED(widget);
  
  if ( isSelected() ){
    painter->setPen(QPen(Qt::black, _pointer->width(),  Qt::DotLine));
  }
  QGraphicsPathItem::paint(painter, option, widget);
}
