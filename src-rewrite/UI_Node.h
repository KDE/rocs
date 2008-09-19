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

#ifndef UI_NODE_H
#define UI_NODE_H

#include <QGraphicsItem>

class Graph;
class EdgeItem;
class Node;

class NodeItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	public:
		NodeItem (Graph *Graph, QGraphicsItem *parent = 0 );
		QRectF bounds();
		enum{Type = UserType + 1};

		enum{ 	Default 	= 0,	// 0000000
			Selected 	= 1,	// 0000001
			Focused 	= 2,	// 0000010
			Dragged 	= 4,	// 0000100
			Begin 		= 8,	// 0801000
			End 		= 16,	// 0010000
			Name 		= 32,	// 0100000
			Index 		= 64	// 1000000
		};
		int type() const { return Type; }
		void setIndex(int i) { _index = i; }
		int index() const {return _index; }

		//! Change this here later to a MVC structure.
		void addTo ( NodeItem *to );
		void setName ( const QString& name );
		void setColor ( const QColor& color );
		void setXY ( qreal x, qreal y );
	protected:
		unsigned char _action;
		
		Graph *_graph;
		QString _name;
		int _index;
		qreal _x;
		qreal _y;

		QList<NodeItem*> _connections;
		QList<EdgeItem*> _edges;

		QColor _colorFocused;
		QColor _colorDragged;
		QColor _colorSelected;
		QColor _colorDefault;

		//! Methods
		QPainterPath shape() const;
		QRectF boundingRect() const;
		void paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
		void paintSelected(); // 0000000
		void paintFocused();  // 0000001
		void paintDragged();  // 0000010
		void paintAsBegin();  // 0000100
		void paintDefault();  // 0001000
		void paintAsEnd();    // 0010000
		void paintName();     // 0100000
		void paintIndex();    // 1000000

};
#endif
