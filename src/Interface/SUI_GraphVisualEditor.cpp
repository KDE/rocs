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
#include "SUI_GraphToolBoxWidget.h"

#include "graphicsitem_Node.h"
#include "graphicsitem_Edge.h"
#include "graphicsitem_OrientedEdge.h"

#include "graphDocument.h"
#include "graph.h"
#include "node.h"
#include "edge.h"

#include "SUI_GraphScene.h"

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

	QHBoxLayout *layout = 0;

	layout  = new QHBoxLayout();
	layout->setContentsMargins(0,0,0,0);
	//!################## CODE TO GENERATE THE ALLIGN TOOLBOX ##########################

	_spacingPropContainer = new QWidget(this);

	layout->addWidget(setupToolButton("allign-horizontal-bottom", "Allign nodes horizontally on the base", "alignHBottom()",_spacingPropContainer ));
	layout->addWidget(setupToolButton("allign-horizontal-middle", "Allign nodes horizontally on the middle", "alignHMiddle()",_spacingPropContainer));
	layout->addWidget(setupToolButton("allign-horizontal-top", "Allign nodes horizontally on the top", "alignHTop()",_spacingPropContainer));
	layout->addWidget(setupToolButton("allign-vertical-left", "Allign nodes vertically on the left", "alignVLeft()",_spacingPropContainer ));
	layout->addWidget(setupToolButton("allign-horizontal-middle", "Allign nodes vertically on the middle", "alignVMiddle()",_spacingPropContainer));
	layout->addWidget(setupToolButton("allign-horizontal-right", "Allign nodes vertically on the right", "alignVRight()",_spacingPropContainer));

	_spacingPropContainer->setLayout(layout);

	//!############################### CODE TO GENERATE THE GRAPH PROPERTIES TOOLBOX ####################################

	_graphToolBox = new GraphToolBoxWidget(this);
	
	//!############################### CODE TO GENERATE THE EDGE PROPERTIES TOOLBOX #####################################
	layout = new QHBoxLayout();
	layout->setContentsMargins(0,0,0,0);
	_edgePropContainer = new QWidget(this);
	_edgePropContainer -> setLayout(layout);

	//!################################ CODE TO GENERATE THE NODE PROPERTIES TOOLBOX ####################################
	layout = new QHBoxLayout();
	layout->setContentsMargins(0,0,0,0);
	_nodePropContainer = new QWidget(this);

	layout->addWidget(setupToolButton("set-start", "Set this node as 'start_node' on the engine.", "setStartNode()",_nodePropContainer));
	layout->addWidget(setupToolButton("set-end","Set this node as 'end_node' on the engine. if there's more than one end node, the 'end_node' is an array","setEndNode()",_nodePropContainer));

	_nodePropContainer -> setLayout(layout);

	//!############################### finishes the Toolbar.
	layout = new QHBoxLayout();
	_toolbar = new QWidget(this);
	layout->setSpacing(0);
layout->setContentsMargins(0,0,0,0);
	layout->addWidget( _graphToolBox );
	layout->addWidget( _nodePropContainer );
	layout->addWidget( _edgePropContainer );
	layout->addWidget( _spacingPropContainer );
	layout->addStretch();
	
	_toolbar->setLayout(layout);

	//!############################## Adds the GraphView on the screen.
	QVBoxLayout *vLayout = new QVBoxLayout();
	vLayout->setContentsMargins(0,0,0,0);
	_scene = new GraphScene();
	QGraphicsView *graphicsView = new QGraphicsView();
	graphicsView->setScene(_scene);
	vLayout -> addWidget( _toolbar );
	vLayout -> addWidget ( graphicsView );
	setLayout( vLayout );

}

QToolButton* GraphVisualEditor::setupToolButton(const QString& actionName, const QString& tooltip, const char* slot, QWidget *parent){
	QToolButton *tmpButton = 0;
	KAction *tmpAction = 0;

	tmpAction = new KAction(KIcon(actionName), tooltip, parent);
	connect( tmpAction, SIGNAL(triggered()), this, slot);
	tmpButton = new QToolButton(_spacingPropContainer);
	tmpButton -> setDefaultAction ( tmpAction );
	tmpButton -> setAutoRaise( true );
	return tmpButton;


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
  _scene->setSceneRect(QRectF(0,0, gd->width(), gd->height() ));
	_graphToolBox->setGraphDocument( gd );
}

void GraphVisualEditor::releaseGraphDocument(){
  QList<QGraphicsItem*> itemList = _scene->items();
  foreach(QGraphicsItem *i, itemList){
    _scene->removeItem(i);
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
    _scene->createNode(node);
  }

  foreach( Edge* edge, edges){
    _scene->createEdge(edge);
  }
}

void GraphVisualEditor::setGraph( Graph *graph){
  _graph = graph;
}

GraphScene* GraphVisualEditor::scene() const{
	return _scene;
}