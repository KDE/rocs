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


#include "graphicsitem_Edge.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QPen>


#include "node.h"
#include "edge.h"
#include "graph.h"
#include <SUI_GraphScene.h>


#include "graphicsitem_Node.h"


#include <KDebug>

EdgeItem::EdgeItem( Edge *edge, QGraphicsItem *parent)
        : QObject(0), QGraphicsLineItem(parent) {

    _edge = edge;
  
    setCacheMode(DeviceCoordinateCache);
    setZValue(0);
    setFlag(ItemIsSelectable);

    connectSignals();
    setupPen();
    updatePos();
}

void EdgeItem::connectSignals() {
    connect(_edge, SIGNAL(posChanged()), this, SLOT(updatePos()));
    connect (_edge, SIGNAL(removed()), this, SLOT(remove()));
}

void EdgeItem::setupPen() {
    _pen = new QPen();
    _pen->setStyle(Qt::SolidLine);
    _pen->setWidth(1);
    _pen->setBrush(_edge->color());
    _pen->setCapStyle(Qt::RoundCap);
    _pen->setJoinStyle(Qt::RoundJoin);
    setPen( (*_pen) );
}

void EdgeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
 }

void EdgeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
 }

void EdgeItem::remove() {
    dynamic_cast<GraphScene*>(scene())->removeGItem(this);
    deleteLater();
}

void EdgeItem::updatePos() {
    qDebug() << "Calling Everything!";
    setLine( _edge->from()->x(), _edge->from()->y(),    _edge->to()->x(),  _edge->to()->y());
    update();
}

void EdgeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
  if (isSelected()) {
	_pen->setStyle(Qt::DotLine);
    }
    else{
	_pen->setStyle(Qt::SolidLine);
    }
    painter->setPen((*_pen));
    painter->drawLine(line());
}

void EdgeItem::updateName(const QString& ) {}
void EdgeItem::updateVisited(bool ) {}
void EdgeItem::updateLength(qreal ) {}
void EdgeItem::updateValue(qreal ) {}
void EdgeItem::updateTotal(qreal ) {}
void EdgeItem::updateColor(QColor ) {}

#include "graphicsitem_Edge.moc"
