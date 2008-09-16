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
	_activeGraph = NULL;

	mainWindow = qobject_cast< MainWindow* >( parent );
	setSceneRect( 0, 0, 1024, 768 );
	_graphCollection = item;
	changeActiveGraph( _graphCollection->createNewGraph() );
}

void GraphScene::changeActiveGraph(Graph *g)
{
	_activeGraph = g;
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
  if ( mouseEvent -> button( ) != Qt::LeftButton ) return;

  QPointF pos = mouseEvent  ->  scenePos( );
  switch ( action )
  {
    case AddNode       : insertNode( pos );           break;

/*  case InsertEdge       : beginInsertEdge( pos );      break;
    case InsertEdgeArrow1 : beginInsertEdge( pos, 1 );   break;
    case InsertEdgeArrow2 : beginInsertEdge( pos, 2 );   break;
    case SelectItem       : selectItem( pos );           break;
    case SquareSelect     : beginSquareSelect( pos );    break;
    case InsertKGraph     : createKGraph(pos);           break;
    case InsertCGraph     : createCGraph(pos);           break; */

  }
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