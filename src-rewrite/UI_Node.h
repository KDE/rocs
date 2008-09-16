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

class GraphItem;
class EdgeItem;
class Node;

class NodeItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	public:
		NodeItem (Node *node, GraphItem *Graph, QGraphicsItem *parent = 0 );
		QRectF bounds();
		enum{Type = UserType + 1};
		enum{ Default, Selected, Focused, Dragged};

	protected:
		Node *_node; //! the node that this particular drawing represents.
		GraphItem *_graphItem;
		int _action;
		bool _showIndex;
		bool _showName;

		QPainterPath shape() const;
		QRectF boundingRect() const;
		void paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
		void paintSelected();
		void paintFocused();
		void paintDragged();
		void paintAsBegin();
		void paintDefault();
		void paintAsEnd();
		void paintName();
		void paintIndex();

};
#endif
