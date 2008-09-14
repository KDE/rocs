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

#ifndef UI_GRAPH_H
#define UI_GRAPH_H

#include <QGraphicsItem>

class Graph;
class NodeItem;
class EdgeItem;

class GraphItem : public QGraphicsItem
{
	public:
		GraphItem ( Graph *graph, QGraphicsItem *parent = 0 );
		void showIndexes ( bool show );
		void showNames ( bool show );
		void showLengths ( bool show );

		bool showLengthGlobal();
		bool showIndexGlobal();
		bool showNamesGlobal();

	private:
		Graph *_graph;
		QList<NodeItem*> _nodes;
		QList<EdgeItem*> _edges;

		bool _showIndex;
		bool _showLength;
		bool _showNames;

};

#endif
