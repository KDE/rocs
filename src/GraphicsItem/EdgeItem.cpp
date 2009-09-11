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


#include "EdgeItem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QPen>


#include "node.h"
#include "edge.h"
#include "graph.h"
#include <GraphScene.h>
#include <QGraphicsSimpleTextItem>
#include "NodeItem.h"


#include <KDebug>

EdgeItem::EdgeItem( Edge *edge, QGraphicsItem *parent)
        : QObject(0), QGraphicsLineItem(parent) {

    _edge = edge;
    _name = new QGraphicsSimpleTextItem(this);
    _value = new QGraphicsSimpleTextItem(this);

    setCacheMode(DeviceCoordinateCache);
    setZValue(0);
    setFlag(ItemIsSelectable);

    connectSignals();
    setPen(QPen(QBrush(QColor(_edge->color())), 1, Qt::SolidLine,Qt::RoundCap, Qt::RoundJoin));

    updatePos();
}

void EdgeItem::connectSignals() {
    connect(_edge, SIGNAL(updateNeeded()), this, SLOT(updateAttributes()));
    connect(_edge, SIGNAL(posChanged()), this, SLOT(updatePos()));
    connect(_edge, SIGNAL(removed()), this, SLOT(remove()));
}

EdgeItem::~EdgeItem() {}

void EdgeItem::remove() {
    dynamic_cast<GraphScene*>(scene())->removeGItem(this);
    deleteLater();
}

void EdgeItem::updatePos() {
    GraphScene* gScene = dynamic_cast<GraphScene*>(scene());
    if ( gScene && gScene->hideEdges()) {
        gScene->updateAfter(this);
    }
    QLine q(_edge->from()->x(), _edge->from()->y(),    _edge->to()->x(),  _edge->to()->y());
    qreal size = sqrt( pow(q.dx(), 2) + pow(q.dy(), 2));
    if (size   < 20) {
        setLine( QLine()) ;
    }
    else {
        setLine( q );
    }
    updateAttributes();
}

void EdgeItem::updateAttributes() {
    Qt::PenStyle s;
    if     (_edge->style() == "dash")    { s = Qt::DashLine;   }
    else if(_edge->style() == "dot")     { s = Qt::DotLine;    }
    else if(_edge->style() == "dash dot"){ s = Qt::DashDotLine;}
    else if(_edge->style() == "solid")   { s = Qt::SolidLine;  }
    else                                 { s = Qt::SolidLine;  }
    
    setPen(QPen(QBrush(QColor(_edge->color())), _edge->width(), s,Qt::RoundCap, Qt::RoundJoin));
    QLineF l = line();
    _value->hide();
    _name->hide();
    _value->setText(_edge->value());
    _name->setText(_edge->name());

    kDebug() << " Show Value: " << _edge->showValue();
    if (_edge->showValue()) {
        _value->show();
    }
    kDebug() << "Show Name : " << _edge->showName();
    if (_edge->showName()) {
        _name->show();
    }

    qreal x = l.x1() + l.dx()/2;
    qreal y = l.y1()  + l.dy()/2;

    _name->setPos(x,y);
    _value->setPos(x, y+14);
    update();
    kDebug() << "Chamou" << _value->text() << " " << _name->text();
}

#include "EdgeItem.moc"
