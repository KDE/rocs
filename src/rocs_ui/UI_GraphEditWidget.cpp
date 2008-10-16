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

#include "UI_GraphEditWidget.h"
#include "UI_GraphScene.h"
#include "UI_MainWindow.h"

#include "graphicsitem_Node.h"
#include "graphicsitem_Edge.h"

#include "libgraph_GraphDocument.h"
#include "libgraph_Graph.h"
#include "libgraph_Node.h"
#include "libgraph_Edge.h"

#include "kross_rocsengine.h"

#include <ktexteditor/editor.h>
#include <ktexteditor/editorchooser.h>
#include <ktexteditor/view.h>
#include <ktexteditor/document.h>
#include <KXMLGUIFactory>
#include <QVBoxLayout>
#include <QPointF>

#include <QGraphicsItem>
#include <KDebug>

#include "settings.h"

GraphEditWidget::GraphEditWidget(MainWindow *parent) : QWidget(parent){
  setupUi(this);
  _graphDocument = 0;
  _graph = 0;
  _graphScene = new GraphScene(this);
  _graphicsView->setScene(_graphScene);

  if ( ! Settings::fastGraphics() ) _graphicsView->setRenderHint(QPainter::Antialiasing);

  _editor = KTextEditor::EditorChooser::editor();
  _txtEditScriptDocument = _editor->createDocument(0);
  _txtEditScriptView = qobject_cast<KTextEditor::View*>(_txtEditScriptDocument->createView(this));
    
   QVBoxLayout *layout = new QVBoxLayout;
   layout->addWidget(_txtEditScriptView);
  _tabEditor -> setLayout(layout);
  kDebug() << "GraphEditWidget Created";
}

void GraphEditWidget::setGraphDocument(libgraph::GraphDocument *gd){
  
  // if there is another document active, desactive it first.
  if ( _graphDocument != 0 ){
    QList<QGraphicsItem*> itemList = _graphScene->items();
    foreach(QGraphicsItem *i, itemList){
      _graphScene->removeItem(i);
      delete i;
    }
    kDebug() << "document " << _graphDocument -> name() << "being disconnected";
    int size = gd->size();
    for(int i = 0; i < size; i++){
      gd->at(i)->disconnect();
      kDebug() << "graph at " << i << "Disconnected.";
    } 
    _graphDocument -> setScript( _txtEditScriptDocument->text() );
    kDebug() << "script text saved on graph document " << _graphDocument -> name();
  }
  
  kDebug() << "document" << gd->name() << "setted as active.";
  int size = gd->size();
  for(int i = 0; i < size; i++){
    connectGraphSignals( gd->at(i) );
    drawGraphOnScene( gd->at(i) );
  }
  
  _graphDocument = gd;
  _txtEditScriptDocument->setText( gd->script() );
  _graphScene->setSceneRect(gd->width(), gd->height() );
  
}

void GraphEditWidget::drawGraphOnScene(libgraph::Graph *g){
  QList<libgraph::Node*> nodes = g->nodes();
  QList<libgraph::Edge*> edges = g->edges();

  foreach(libgraph::Node* node, nodes){
    createNode(node);
  }

  foreach(libgraph::Edge* edge, edges){
    createEdge(edge);
  }
  kDebug() << "Graph drawn on screen"; 
}

GraphScene *GraphEditWidget::scene() const{
  return _graphScene;
}

void GraphEditWidget::setGraph(libgraph::Graph *graph){
  _graph = graph;
  connectGraphSignals(graph);
  kDebug() << "new active graph set.";
}

void GraphEditWidget::createNode(libgraph::Node *node){
  NodeItem *nodeitem = new NodeItem( node );
  _graphScene->addItem(nodeitem); 
  kDebug() << "Node Created on Screen";
  kDebug() << "Number of Nodes on Screen:"  << _graphScene->items().size() - 4;
}

void GraphEditWidget::createEdge(libgraph::Edge* edge)
{
  EdgeItem *edgeItem = new EdgeItem( edge );
  _graphScene->addItem(edgeItem);
  kDebug() << "Should Insert an edge on screen, NOT IMPLEMENTED YET";
}

void GraphEditWidget::removeNode(int )
{
  kDebug() << "Should remove a node and associated edges, NOT IMPLEMENTED YET";
}

void GraphEditWidget::removeEdge(int )
{
  kDebug() << "Should remove an edge, NOT IMPLEMENTED  YET";
}

void GraphEditWidget::graphColorChanged(const QColor&)
{
  kDebug() << "Should change the graph's outlines color. NOT IMPLEMENTED YET";
}

void GraphEditWidget::connectGraphSignals(libgraph::Graph *graph){
  connect(graph, SIGNAL( nodeCreated( libgraph::Node* ) ), 
	  this,   SLOT( createNode( libgraph::Node* ) ) 
  );
  connect(graph, SIGNAL( edgeCreated( libgraph::Edge* ) ), 
	  this,   SLOT( createEdge( libgraph::Edge* ) ) 
  );
  connect(graph, SIGNAL( nodeRemoved( int ) ), 
	  this,   SLOT( removeNode( int ) ) 
  );
  connect(graph, SIGNAL( edgeRemoved( int ) ), 
	  this,   SLOT( removeEdge( int ) ) 
  );
  kDebug() << "Graph Signals Connected with GraphEdit";
}


void GraphEditWidget::on__btnRunScript_clicked(){

  kDebug() << "Button to run Script Clicked" ;
  if ( _graphDocument == 0 ) return;

  kross_rocsengine e( _txtDebug) ;
  e.setDocument( _graphDocument );
  e.setEngine( _cmbEngine->currentText());
  e.execute( _txtEditScriptDocument -> text() );
  

}