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

NodeItem::NodeItem ( GraphItem *graphItem, Node *node, QGraphicsItem *parent ) : QGraphicsItem ( parent )
{

}

void NodeItem::paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	switch ( _action )
	{
		case Selected: paintSelected(); break;
		case Focused:  paintFocused();  break;
		case Dragged:  paintDragged();  break;
		Default:       paintDefault();  break;
	}

	if ( _node -> isBegin() ) paintAsBegin();
	else if ( _node -> isEnd() ) paintAsEnd();

	if ( ( _showName ) || ( _graphItem -> showNamesGlobal() ) )  paintName();
	if ( ( _showIndex ) || ( _graphItem -> showIndexGlobal() ) ) paintIndex();
}

void NodeItem::paintSelected()
{

}

void NodeItem::paintFocused()
{

}

void NodeItem::paintDragged()
{

}

void NodeItem::paintAsBegin()
{

}

void NodeItem::paintDefault()
{

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

}

QRectF NodeItem::boundingRect() const
{

}
