#include "UI_GraphScene.h"
#include "UI_MainWindow.h"
#include "GraphCollection.h"
#include "UI_Graph.h"
#include "Graph.h"
#include <QGraphicsSceneMouseEvent>

GraphScene::GraphScene ( QObject* parent, GraphCollection *item ) : QGraphicsScene ( parent )
{
	_activeGraph = NULL;

	mainWindow = qobject_cast< MainWindow* >( parent );
	setSceneRect( 0, 0, 1024, 768 );
	_graphCollection = item;

	connect( _graphCollection, SIGNAL(graphCreated(Graph*)), 
		this, SLOT(createGraphItem(Graph*)));

	connect( _graphCollection, SIGNAL(graphRemoved(int)), 
		this, SLOT(removeGraphItem(int)));

	_graphCollection->createNewGraph();
}

void GraphScene::changeActiveGraph(Graph *g)
{
	if (_activeGraph != NULL ){
//		disconnect();
		//disconnect();
	}
	//connect( g, SIGNAL(), this, SLOT());
	connect( g, SIGNAL(nodeInserted(Node*)), this, SLOT(createNodeItem(Node*)));
	//connect( this, SIGNAL(), g, SLOT());
	connect( this, SIGNAL(insertNode(QPointF)), g, SLOT(addNode(QPointF)));
}

void GraphScene::createGraphItem(Graph *g)
{
	changeActiveGraph(g);
	GraphItem *graphItem = new GraphItem();
	_graphCollectionItems.append(graphItem);

	//! When the graph creates a new node, creates another on the screen.
	connect( g,        SIGNAL( nodeInserted( Node* )),
		graphItem, SLOT( createNodeItem( Node* )));

	//! Same for the edge.
	connect( g, SIGNAL(edgeInserted( Edge* )),
		graphItem, SLOT( createEdgeItem( Edge* )));

	//! if the node is created using the mouse and clicking on the screen, 
	//! send a message to the Graph to create a new one on the QPointF coordinates.
	connect(graphItem, SIGNAL(nodeInserted(QPointF)),
		g,         SLOT(createNode(QPointF)));

	//! Same to Edges. but this one is broken, search how to do that later.
	connect(graphItem, SIGNAL(edgeInserted(QPointF, QPointF)),
		g,         SLOT(createEdge(QPointF, QPointF)));

}

void GraphScene::removeGraphItem(int)
{
	
}

QRectF GraphScene::boudingRect() const
{
}

void GraphScene::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    case InsertCGraph     : createCGraph(pos);           break;*/

  }
  QGraphicsScene::mousePressEvent( mouseEvent );
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

}

void GraphScene::insertNode(QPointF pos)
{
	//emit(
}
