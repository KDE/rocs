/***************************************************************************
 *   Copyright (C) 2005-2006 by Tomaz Canabrava and Ugo Sangiori           *
 *   tomaz.canabrava@gmail.com                                             *
 *   ugorox@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/
#include "UI_Node.h"
#include "Node.h"
#include "UI_Graph.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>


NodeItem::NodeItem (Graph *graph, QGraphicsItem *parent ) : QGraphicsItem ( parent )
{
	_action = 0;
	_graph = graph;
	
	//! Change those colors to make it configurable.
	_colorDefault  = Qt::yellow;
	_colorSelected = Qt::yellow; 
	_colorFocused  = Qt::yellow;
	_colorDragged  = Qt::yellow;

	setFlag(ItemIsMovable, true);
	setCacheMode(DeviceCoordinateCache);
	setZValue(1);
}

void NodeItem::paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	//! draw the Shadow
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::darkGray);
	painter->drawEllipse(-7, -7, 20, 20);

	QRadialGradient gradient(-3, -3, 10);
	QColor color = _colorDefault;


	switch ( _action )
	{
		case NodeItem::Selected : color = _colorSelected; break;
		case NodeItem::Focused  : color = _colorFocused;  break;
		case NodeItem::Dragged  : color = _colorDragged;  break;
		case NodeItem::Default  : color = _colorDefault;  break;
	}

	gradient.setColorAt(0, color.light(120));
	gradient.setColorAt(1, color.light(180));

	painter->setBrush(gradient);
	painter->setPen(QPen(Qt::black, 0));
	painter->drawEllipse(-10, -10, 20, 20);

/*	gradient.setColorAt(0, color.light(120));
	gradient.setColorAt(1, color.light(180));

	painter->setBrush(gradient);
  	painter->setPen(QPen(Qt::black, 0));
  	painter->drawEllipse(-10, -10, 20, 20); */
	
/*
	if ( _node -> isBegin() ) paintAsBegin();
	else if ( _node -> isEnd() ) paintAsEnd();

	if ( ( _showName ) || ( _graphItem -> showNamesGlobal() ) )  paintName();
	if ( ( _showIndex ) || ( _graphItem -> showIndexGlobal() ) ) paintIndex();
*/
}

void NodeItem::paintAsEnd()
{

}

void NodeItem::paintName()
{

}

void NodeItem::paintIndex()
{

}

QPainterPath NodeItem::shape() const
{
  QPainterPath path;
  path.addEllipse(-10, -10, 20, 20);
  return path;
}

QRectF NodeItem::boundingRect() const
{
  qreal adjust = 2;
  qreal x1 = -10 - adjust;
  qreal y2 = 23 + adjust;
  qreal x2 = y2 + 150;
  
  return QRectF(x1, x1, x2, y2);
}

void NodeItem::setName ( const QString& name )
{
	_name = name;
}

void NodeItem::setColor ( const QColor& color )
{
	_colorDefault = color;
}

void NodeItem::setXY ( qreal x, qreal y )
{
	_x = x;
	_y = y;
}

void NodeItem::addTo ( NodeItem *to )
{
	_connections.append ( to );
}