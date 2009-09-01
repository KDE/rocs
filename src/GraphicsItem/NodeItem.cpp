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
#include <QThread>

#include "node.h"
#include "graph.h"
#include "EdgeItem.h"
#include "OrientedEdgeItem.h"
#include <KDebug>
#include <QDebug>
#include <GraphScene.h>
#include <QTimeLine>

NodeItem::NodeItem(Node *node, QGraphicsItem *parent)
        : QObject(0), QGraphicsItem(parent){
    _node = node;
    _opacity = 0;
    _timeLine = 0;
    connect(_node, SIGNAL(posChanged()), this, SLOT(updatePos()));
    QPointF pos( _node -> x() ,_node->y() );
    setPos( pos );
    _oldZ = node->z();
    setZValue(1);
    setFlag(ItemIsSelectable);
    connect (_node, SIGNAL(removed()), this, SLOT(deleteItem()));
    _timeLine = new QTimeLine(500, this);
    _timeLine->setFrameRange(0, 50);
    connect(_timeLine, SIGNAL(frameChanged(int)), this, SLOT(removeOpacity()));
    _timeLine->start();
}

NodeItem::~NodeItem(){
 
}

void NodeItem::removeOpacity(){
  if ( _opacity  < 1.0 ) _opacity += 0.1;
  if ( _opacity  > 1.0 ) _opacity = 1.0;
  update();
}

void NodeItem::addOpacity(){
  kDebug() << "Add Opacity Running";
  if ( _opacity > 0.0 ) _opacity -= 0.1;
  if ( _opacity < 0.0 ) _opacity = 0.0;
  update();
}

void NodeItem::deleteItem() {
  _node = 0;
  if (!_timeLine ) delete _timeLine;
  _timeLine = new QTimeLine(500, this);
  _timeLine->setFrameRange(0, 50);
  connect(_timeLine, SIGNAL(finished()), this, SLOT(removeFromScene()));
  connect(_timeLine, SIGNAL(frameChanged(int)), this, SLOT(addOpacity()));
  _timeLine->start();
}

void NodeItem::removeFromScene(){
  qobject_cast<GraphScene*>(scene())->removeGItem(this);
  deleteLater();
}

QRectF NodeItem::boundingRect() const {
  qreal z = 1;
  qreal x1 = -12, y1 = -12, x2 = 25, y2 = 25;
  if (_node) z = _node->z();

  if (( _node && _node->begin() ) || ( _removingBeginFlag )){
    x1 -= 40;
    x2 += 40;
  }
  if(isDownSizing) {
    return QRectF(x1 * _oldZ, y1* _oldZ, x2 * _oldZ, y2 * _oldZ);
  }
  return QRectF(x1 * z, y1 * z , x2 * z, y2 * z);
}

QPainterPath NodeItem::shape() const {
    QPainterPath path;
    qreal z = _node->z();
    path.addEllipse(-10 * z, -10 * z, 20 * z, 20 * z);
    return path;
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
  qreal z = _oldZ;
  if (_node){
    z = _node->z();
    _color = _node->color(); 
  }
  
  if(z > _oldZ) _oldZ = z;
  
  if(isDownSizing) {
      QBrush brush(Qt::white);
      painter->setBrush(brush);
      painter->drawRect(QRectF(-11 * z, -11 * z, 24 * z, 24 * z));
  }
    
  if (isSelected()) {
        QPen pen(Qt::black, 1, Qt::DotLine);
        painter->setBrush(QBrush());
        painter->setPen(pen);
        painter->drawRect(QRectF(-11 * z , -11 * z , 24 * z , 24 * z ));
  }
  if( _node && _node->begin() ){
    if (_removingBeginFlag == false) _removingBeginFlag = true;
    painter->drawLine(-20, -10, 0, 0);
    painter->drawLine(-52, 0, 0, 0);
    painter->drawLine(-20, 10, 0, 0);
  }
  else if (_removingBeginFlag) _removingBeginFlag = false;
    painter->setPen(Qt::NoPen);
    
    _color.setAlphaF(_opacity);
    painter->setBrush( _color.dark(240) );

    painter->drawEllipse(-7 * z, -7 * z, 20 * z, 20 * z);
    QRadialGradient gradient(-3 * z, -3 * z, 10 * z);
    
    if (option->state & QStyle::State_Sunken) {
        gradient.setColorAt(0, _color.light(240));
        gradient.setColorAt(1, _color);
    } else {
        gradient.setColorAt(0, _color.light(240));
        gradient.setColorAt(1, _color);
    }

    painter->setBrush(gradient);
    painter->setPen(QPen(_color, 2));
    painter->drawEllipse(-10 * z, -10 * z, 20 * z, 20 * z);
    if(_node && _node->end() ){
      QColor c(Qt::black);
      c.setAlphaF(_opacity);
      painter->setPen(c);
      painter->drawEllipse(-7 * z, -7 * z, 15 * z, 15 * z); 
    }
}

void NodeItem::updatePos() {  setPos( QPointF(_node->x(), _node->y() ) ); }
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
