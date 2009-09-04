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
#include <QGraphicsSimpleTextItem>

NodeItem::NodeItem(Node *node, QGraphicsItem *parent, bool fade)
        : QObject(0), QGraphicsItem(parent){
    _node = node;
    _opacity = 1;
    _timeLine = 0;
    connect(_node, SIGNAL(posChanged()), this, SLOT(updatePos()));
    connect(_node, SIGNAL(updateNeeded()), this, SLOT(updateAttributes()));
    
    QPointF pos( _node -> x() ,_node->y() );
    setPos( pos );
    _oldWidth = node->width();
    setZValue(1);
    setFlag(ItemIsSelectable);
    _name = new QGraphicsSimpleTextItem(this);
    _value = new QGraphicsSimpleTextItem(this);
    
    connect (_node, SIGNAL(removed()), this, SLOT(deleteItem()));
    if (fade){
      _opacity = 0;
      _timeLine = new QTimeLine(500, this);
      _timeLine->setFrameRange(0, 50);
      connect(_timeLine, SIGNAL(frameChanged(int)), this, SLOT(removeOpacity()));
      _timeLine->start();
    }
    
    setupTextAttributes();
    
    
}

void NodeItem::setupTextAttributes(){
  qreal x1 = boundingRect().x() + boundingRect().width() + 5;
  qreal y1 = boundingRect().y() + (boundingRect().height()/2) - 10;
  
  if (_node -> showName() ){
    _name->setText("Name: "+_node->name());
    _name->setPos(x1,y1);
  }
  if (_node->showValue() ){
    _value->setText("Value: "+_node->value());
    _value->setPos(x1,y1 + 12);
  }
}

void NodeItem::updateAttributes(){
  setupTextAttributes();
  update();
  kDebug() << _name->text() << _name->scenePos() << _name->pos();
  kDebug() << _value->text() << _value->scenePos() << _value->pos();
  
  
}

NodeItem::~NodeItem(){
 
}

void NodeItem::removeOpacity(){
  if ( _opacity  < 1.0 ) _opacity += 0.1;
  if ( _opacity  > 1.0 ) _opacity = 1.0;
  update();
}

void NodeItem::addOpacity(){
  kDebug() << "Removing Node." << _oldWidth;
  if ( _opacity > 0.0 ) _opacity -= 0.1;
  if ( _opacity < 0.0 ) _opacity = 0.0;
  update();
}

void NodeItem::deleteItem() {
  _node = 0;
  if (!_timeLine ) delete _timeLine;
  _timeLine = new QTimeLine(250, this);
  _timeLine->setFrameRange(0, 25);
  connect(_timeLine, SIGNAL(finished()), this, SLOT(removeFromScene()));
  connect(_timeLine, SIGNAL(frameChanged(int)), this, SLOT(addOpacity()));
  _timeLine->start();
}

void NodeItem::removeFromScene(){
  qobject_cast<GraphScene*>(scene())->removeGItem(this);
  deleteLater();
}

QRectF NodeItem::boundingRect() const {
  qreal x1 = -12, y1 = -12, x2 = 25, y2 = 25;
  if (( _node && _node->begin() ) || ( _removingBeginFlag )){
    x1 -= 40;
    x2 += 40;
  }
  return QRectF(x1 * _oldWidth, y1* _oldWidth, x2 * _oldWidth, y2 * _oldWidth);
}

QPainterPath NodeItem::shape() const {
    QPainterPath path;
    qreal z = _node->width();
    path.addEllipse(-10 * z, -10 * z, 20 * z, 20 * z);
    return path;
}

void NodeItem::startUpSizing(){
  _isUpSizing = 1;
  _oldWidth = _node->width() + 0.25;
}

void NodeItem::endUpSizing(){
  _isUpSizing = 0;
  _oldWidth = _node->width();
}

void NodeItem::startDownSizing(){
  _isDownSizing = 1;
  _oldWidth = _node->width();

}
void NodeItem::endDownSizing(){
  _isDownSizing = 0;
  _oldWidth = _node->width();
}
    
void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
  if (_node){
    _color = _node->color(); 
  }
  
  QColor c = _color;
  c.setAlphaF(0);
  painter->setPen(QPen(c));
  painter->setBrush(QBrush(c));
  painter->drawRect(QRectF(-11 * _oldWidth, -11 * _oldWidth, 24 * _oldWidth, 24 * _oldWidth));
    
  if (isSelected()) {
    QPen pen(Qt::black, 1, Qt::DotLine);
    painter->setBrush(QBrush());
    painter->setPen(pen);
    painter->drawRect(QRectF(-11 * _oldWidth , -11 * _oldWidth , 24 * _oldWidth , 24 * _oldWidth ));
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

    painter->drawEllipse(-7 * _oldWidth, -7 * _oldWidth, 20 * _oldWidth, 20 * _oldWidth);
    QRadialGradient gradient(-3 * _oldWidth, -3 * _oldWidth, 10 * _oldWidth);
    
    if (option->state & QStyle::State_Sunken) {
        gradient.setColorAt(0, _color.light(240));
        gradient.setColorAt(1, _color);
    } else {
        gradient.setColorAt(0, _color.light(240));
        gradient.setColorAt(1, _color);
    }

    painter->setBrush(gradient);
    painter->setPen(QPen(_color, 2));
    painter->drawEllipse(-10 * _oldWidth, -10 * _oldWidth, 20 * _oldWidth, 20 * _oldWidth);
    if(_node && _node->end() ){
      QColor c(Qt::black);
      c.setAlphaF(_opacity);
      painter->setPen(c);
      painter->drawEllipse(-7 * _oldWidth, -7 * _oldWidth, 15 * _oldWidth, 15 * _oldWidth); 
    }
    painter->setBrush(QBrush());
    painter->drawRect(boundingRect());
}

void NodeItem::updatePos() { 
    setPos( QPointF(_node->x(), _node->y() ) ); 
}

void NodeItem::remove() {
  if (scene() == 0) return;
  dynamic_cast<GraphScene*>(scene())->removeGItem(this);
  deleteLater();
}

#include "NodeItem.moc"
