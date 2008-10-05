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

#include <ktexteditor/editor.h>
#include <ktexteditor/editorchooser.h>
#include <ktexteditor/view.h>
#include <ktexteditor/document.h>
#include <KXMLGUIFactory>
#include <QVBoxLayout>

#include <QGraphicsItem>
GraphEditWidget::GraphEditWidget(MainWindow *parent) : QWidget(parent){
  setupUi(this);
  _graphDocument = 0;

  _graphScene = new GraphScene(this);
  _graphicsView->setScene(_graphScene);

  _editor = KTextEditor::EditorChooser::editor();
  _txtEditScriptDocument = _editor->createDocument(0);
  _txtEditScriptView = qobject_cast<KTextEditor::View*>(_txtEditScriptDocument->createView(this));
    
   QVBoxLayout *layout = new QVBoxLayout;
   layout->addWidget(_txtEditScriptView);
  _tabEditor -> setLayout(layout);

  // MainWindow *mainWindow = qobject_cast<MainWindow*>(parent);
  // mainWindow->guiFactory()->addClient( _txtEditScriptView );
  
  /*
  connect( _txtEditScriptView, SIGNAL(focusIn(KTextEditor::View*)), 
      this, SLOT( scriptViewFocusIn(KTextEditor::View*)));
  connect( _txtEditScriptView, SIGNAL(focusOut(KTextEditor::View*)), 
      this, SLOT( scriptViewFocusOut(KTextEditor::View*)));
  */
  

}

void GraphEditWidget::setGraphDocument(libgraph::GraphDocument *gd){
  
  // if there is another document active, desactive it first.
  if ( _graphDocument != 0 ){
    QList<QGraphicsItem*> itemList = _graphScene->items();
    foreach(QGraphicsItem *i, itemList){
      _graphScene->removeItem(i);
      delete i;
    }
    int size = gd->size();
    for(int i = 0; i < size; i++){
      gd->at(i)->disconnect();
    } 
    _graphDocument -> setScript( _txtEditScriptDocument->text() );
  }
  
  int size = gd->size();
  for(int i = 0; i < size; i++){
    drawGraphOnScene( gd->at(i) );
  }
  
  _graphDocument = gd;
  _txtEditScriptDocument->setText( gd->script() );

}

void GraphEditWidget::drawGraphOnScene(libgraph::Graph *g){
  QList<libgraph::Node*> nodes = g->nodes();
  QList<libgraph::Edge*> edges = g->edges();

  foreach(libgraph::Node* node, nodes){
    createNode(node);
  }

  foreach(libgraph::Edge *edge, edges){

  }  
}

GraphScene *GraphEditWidget::scene() const{
  return _graphScene;
}

void GraphEditWidget::setGraph(libgraph::Graph *graph){
  if (_graph != 0)
  _graph = graph;
  connect(_graph, SIGNAL(nodeCreated(libgraph::Node*)), this, SLOT( createNode(libgraph::Node*)));
  kDebug() << "graph -> nodeCreated connected with GraphEdit -> createNode";
}

void GraphEditWidget::createNode(libgraph::Node *node){
  NodeItem *nodeitem = new NodeItem( node );
  _graphScene->addItem(nodeitem); 
  kDebug() << "Node Created on Screen";
}