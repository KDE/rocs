
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
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "UI_GraphScene.h"
#include "UI_MainWindow.h"
#include "GraphCollection.h"
#include "UI_Graph.h"
#include "Graph.h"
#include "UI_Node.h"
#include "Node.h"
#include <iostream>
#include <QGraphicsSceneMouseEvent>

GraphScene::GraphScene ( QObject* parent, GraphCollection *item ) : QGraphicsScene ( parent )
{
	_activeGraph = 0;

	mainWindow = qobject_cast< MainWindow* >( parent );
	setSceneRect( 0, 0, 1024, 768 );
	_graphCollection = item;
	changeActiveGraph( _graphCollection->createNewGraph() );
}

void GraphScene::changeActiveGraph(Graph *g)
{
	_activeGraph = g;
}

void GraphScene::setAction(int action){ 
	_action = action; 
	
}

void GraphScene::createGraphItem(Graph *g)
{
	changeActiveGraph(g);
	GraphItem *graphItem = new GraphItem();
	_graphCollectionItems.append(graphItem);
}

void GraphScene::removeGraphItem(int)
{
	
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if ( mouseEvent -> button( ) != Qt::LeftButton )
	{
		return;
	}

	QPointF pos = mouseEvent  ->  scenePos( );

	emit executeAction(_action, pos);

	QGraphicsScene::mousePressEvent( mouseEvent );
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

}

void GraphScene::createNodeItem(Node *n)
{

}
