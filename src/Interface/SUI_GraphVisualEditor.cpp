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

#include "SUI_GraphVisualEditor.h"

#include "graphicsitem_Node.h"
#include "graphicsitem_Edge.h"
#include "graphicsitem_OrientedEdge.h"

#include "graphDocument.h"
#include "graph.h"
#include "node.h"
#include "edge.h"

#include "SUI_GraphView.h"

#include <QVBoxLayout>
#include <QPointF>
#include <QGraphicsItem>
#include <KDebug>
#include <QToolButton>
#include <KAction>
#include <KIcon>
#include <KComboBox>
#include <KColorButton>
#include <QGraphicsView>

#include "settings.h"


GraphVisualEditor::GraphVisualEditor(QWidget *parent) 
	: QWidget(parent),
	_topNode(0),
	_bottomNode(0),
	_leftNode(0),
	_rightNode(0){
	_graphDocument = 0;
	_graph = 0;

	setupWidgets();
}

void GraphVisualEditor::setupWidgets(){
	QToolButton *tmpButton = 0;
	KAction *tmpAction = 0;
	QHBoxLayout *layout = 0;

	layout  = new QHBoxLayout();
	
	//!################## CODE TO GENERATE THE ALLIGN TOOLBOX ##########################

	_spacingPropContainer = new QWidget(this);

	tmpAction = new KAction(KIcon("allign-horizontal-bottom"), "Allign nodes horizontally on the base", this);
	connect( tmpAction, SIGNAL(triggered()), this, SLOT(alignHBottom()));
	tmpButton = new QToolButton(_spacingPropContainer);
	tmpButton -> setDefaultAction ( tmpAction );
	tmpButton -> setAutoRaise( true );
	layout -> addWidget(tmpButton);

	tmpAction = new KAction(KIcon("allign-horizontal-middle"), "Allign nodes horizontally on the middle", this);
	connect( tmpAction, SIGNAL(triggered()), this, SLOT(alignHMiddle()));
	tmpButton = new QToolButton(_spacingPropContainer);
	tmpButton -> setDefaultAction ( tmpAction );
	tmpButton -> setAutoRaise( true );
	layout -> addWidget(tmpButton);

	tmpAction = new KAction(KIcon("allign-horizontal-top"), "Allign nodes horizontally on the top", this);
	connect( tmpAction, SIGNAL(triggered()), this, SLOT(alignHTop()));
	tmpButton = new QToolButton(_spacingPropContainer);
	tmpButton -> setDefaultAction ( tmpAction );
	tmpButton -> setAutoRaise( true );
	layout -> addWidget(tmpButton);

	tmpAction = new KAction(KIcon("allign-vertical-left"), "Allign nodes vertically on the left", this);
	connect( tmpAction, SIGNAL(triggered()), this, SLOT(alignVLeft()));
	tmpButton = new QToolButton(_spacingPropContainer);
	tmpButton -> setDefaultAction ( tmpAction );
	tmpButton -> setAutoRaise( true );
	layout -> addWidget(tmpButton);

	tmpAction = new KAction(KIcon("allign-vertical-middle"), "Allign nodes vertically on the middle", this);
	connect( tmpAction, SIGNAL(triggered()), this, SLOT(alignVMiddle()));
	tmpButton = new QToolButton(_spacingPropContainer);
	tmpButton -> setDefaultAction ( tmpAction );
	tmpButton -> setAutoRaise( true );
	layout -> addWidget(tmpButton);

	tmpAction = new KAction(KIcon("allign-vertical-right"), "Allign nodes vertically on the right", this);
	connect( tmpAction, SIGNAL(triggered()), this, SLOT(alignVRight()));
	tmpButton = new QToolButton(_spacingPropContainer);
	tmpButton -> setDefaultAction ( tmpAction );
	tmpButton -> setAutoRaise( true );
	layout -> addWidget(tmpButton);

	_spacingPropContainer->setLayout(layout);

	//!############################### CODE TO GENERATE THE GRAPH PROPERTIES TOOLBOX ####################################
	layout = new QHBoxLayout();
	_graphPropContainer = new QWidget(this);
	_graphComboBox = new KComboBox(_graphPropContainer);
	_colorButton = new KColorButton(_graphPropContainer);
	layout -> addWidget( _graphComboBox );
	layout -> addWidget( _colorButton );	
	_graphPropContainer -> setLayout(layout);

	//!############################### CODE TO GENERATE THE EDGE PROPERTIES TOOLBOX #####################################
	layout = new QHBoxLayout();
	_edgePropContainer = new QWidget(this);
	_edgePropContainer -> setLayout(layout);

	//!################################ CODE TO GENERATE THE NODE PROPERTIES TOOLBOX ####################################
	layout = new QHBoxLayout();
	_nodePropContainer = new QWidget(this);

	tmpAction = new KAction(KIcon("set-start"), "Set this node as 'start_node' on the engine.", this);
	connect( tmpAction, SIGNAL(triggered()), this, SLOT(setStartNode()));
	tmpButton = new QToolButton(_nodePropContainer);
	tmpButton -> setDefaultAction ( tmpAction );
	tmpButton -> setAutoRaise( true );
	layout -> addWidget(tmpButton);

	tmpAction = new KAction(KIcon("set-end"), "Set this node as 'end_node' on the engine. if there's more than one end node, the 'end_node' is an array", this);
	connect( tmpAction, SIGNAL(triggered()), this, SLOT(setEndNode()));
	tmpButton = new QToolButton(_nodePropContainer);
	tmpButton -> setDefaultAction ( tmpAction );
	tmpButton -> setAutoRaise( true );
	layout -> addWidget(tmpButton);
	_nodePropContainer -> setLayout(layout);

	//!############################### finishes the Toolbar.
	layout = new QHBoxLayout();
	_toolbar = new QWidget(this);
	layout->addWidget( _graphPropContainer );
	layout->addWidget( _nodePropContainer );
	layout->addWidget( _edgePropContainer );
	layout->addWidget( _spacingPropContainer );
	_toolbar->setLayout(layout);

	//!############################## Adds the GraphView on the screen.
	QVBoxLayout *vLayout = new QVBoxLayout();
	_view = new GraphView();
	QGraphicsView *graphicsView = new QGraphicsView();
	graphicsView->setScene(_view);
	vLayout -> addWidget( _toolbar );
	vLayout -> addWidget ( graphicsView );
	setLayout( vLayout );

}

void GraphVisualEditor::setGraphDocument( GraphDocument *gd){
  if ( _graphDocument != 0 ){
    releaseGraphDocument();
   }
  
  int size = gd->size();
  for(int i = 0; i < size; i++){
    drawGraphOnScene( gd->at(i) );	
  }
  
  _graphDocument = gd;
  _view->setSceneRect(QRectF(0,0, gd->width(), gd->height() ));
}

void GraphVisualEditor::releaseGraphDocument(){
  QList<QGraphicsItem*> itemList = _view->items();
  foreach(QGraphicsItem *i, itemList){
    _view->removeItem(i);
    delete i;
  }
  int size = _graphDocument->size();
  for(int i = 0; i < size; i++){
    _graphDocument->at(i)->disconnect(this);
  }
}

void GraphVisualEditor::drawGraphOnScene( Graph *g){
  QList< Node*> nodes = g->nodes();
  QList< Edge*> edges = g->edges();

  foreach( Node* node, nodes){
    _view->createNode(node);
  }

  foreach( Edge* edge, edges){
    _view->createEdge(edge);
  }
}

void GraphVisualEditor::setGraph( Graph *graph){
  _graph = graph;
}
