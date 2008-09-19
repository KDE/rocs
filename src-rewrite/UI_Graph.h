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

#include "Node.h"
#include <QGraphicsItem>

class Graph;
class NodeItem;
class EdgeItem;

class GraphItem :public QObject
{
	Q_OBJECT
	public:
		GraphItem ();
		void showIndexes ( bool show );
		void showNames ( bool show );
		void showLengths ( bool show );

		bool showLengthGlobal();
		bool showIndexGlobal();
		bool showNamesGlobal();
	private:
		QList<NodeItem*> _nodes;
		QList<EdgeItem*> _edges;

		bool _showIndex;
		bool _showLength;
		bool _showNames;
	
	public slots:
		void createNodeItem(NodeItem *n);
		void createEdgeItem(EdgeItem *e);
	signals:
		void nodeInserted(QPointF pos);
		void edgeInserted(QPointF pos1, QPointF pos2);
};

#endif
