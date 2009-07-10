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
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with Step; if not, write to the Free Software
	 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
*/


#include "NodeItem.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QPen>
#include <QBrush>

#include "node.h"
#include "graph.h"
#include "EdgeItem.h"
#include "OrientedEdgeItem.h"
#include <KDebug>
#include <QDebug>
#include <SUI_GraphScene.h>

NodeItem::NodeItem(Node *node, QGraphicsItem *parent)
        : QObject(0), QGraphicsItem(parent)
{
    _node = node;
    connect(_node, SIGNAL(posChanged()), this, SLOT(updatePos()));
    QPointF pos( _node -> x() ,_node->y() );
    setPos( pos );
    setZValue(1);
    setFlag(ItemIsSelectable);
    connect (_node, SIGNAL(removed()), this, SLOT(remove()));
}

QRectF NodeItem::boundingRect() const {
     if ( _node->begin() ){
         return QRectF(-52, -12 , 65, 25);
     }
    return QRectF(-12, -12 , 25, 25);
}

QPainterPath NodeItem::shape() const {
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
  setPos( QPointF(_node->x(), _node->y() ) );
    if (isSelected()) {
        QPen pen(Qt::black, 1, Qt::DotLine);

        painter->setBrush(QBrush());
        painter->setPen(pen);
        painter->drawRect(QRectF(-11 , -11 , 24 , 24 ));
    }
    if( _node->begin() ){
      painter->drawLine(-20, -10, 0, 0);
      painter->drawLine(-52, 0, 0, 0);
      painter->drawLine(-20, 10, 0, 0);
    }

    painter->setPen(Qt::NoPen);
    QColor color = QColor(_node->color());
    painter->setBrush( color.dark(240) );

    painter->drawEllipse(-7, -7, 20, 20);
    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setColorAt(0, color.light(240));
        gradient.setColorAt(1, color);
    } else {
        gradient.setColorAt(0, color.light(240));
        gradient.setColorAt(1, color);
    }

    painter->setBrush(gradient);
    painter->setPen(QPen(color, 2));
    painter->drawEllipse(-10, -10, 20, 20);
    if( _node->end() ){
      painter->setPen(Qt::black);
      painter->drawEllipse(-7, -7, 15, 15); 
    }
}

void NodeItem::updatePos() {  update(); }
void NodeItem::updateName(const QString& ) {}
void NodeItem::updateVisited(bool ) {}
void NodeItem::updateValue(qreal ) {}
void NodeItem::updateTotal(qreal ) {}
void NodeItem::updateColor(QColor ) {}

void NodeItem::remove() {
  if (scene() == 0) return;
  dynamic_cast<GraphScene*>(scene())->removeGItem(this);
  deleteLater();
}

#include "NodeItem.moc"
