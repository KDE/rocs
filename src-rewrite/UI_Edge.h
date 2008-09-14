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

#ifndef UI_EDGE_H
#define UI_EDGE_H

#include <QGraphicsItem>
class Edge;
class GraphScene;
class NodeItem;

class EdgeItem : public QGraphicsItem
{
	public:
		EdgeItem ( QGraphicsItem *parent = 0 );
		enum { Type = UserType + 2 };
		enum { Default, Selected, Focused, Dragged };
		EdgeItem ( NodeItem *from, NodeItem *to, GraphScene *parent = 0 );
		~EdgeItem();
		void adjust();
		int type() const { return Type; }

	public slots:

	protected:
		Edge *_edge;

		QPainterPath shape() const;
		QRectF boundingRect() const;
		void paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
		void paintSelected();
		void paintFocused();
		void paintDefault();
		void paintName();
		void paintIndex();
		void paintLength();
	private:
		QPointF sourcePoint;
		QPointF destPoint;
		QLineF *myLine;


};

#endif
