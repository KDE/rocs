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

#include "SUI_GraphEditWidget.h"
#include "SUI_GraphScene.h"
#include "SUI_MainWindow.h"

#include "graphicsitem_Node.h"
#include "graphicsitem_Edge.h"
#include "graphicsitem_OrientedEdge.h"

#include "graphDocument.h"
#include "graph.h"
#include "node.h"
#include "edge.h"

#ifdef USING_QTSCRIPT
#include "qtScriptBackend.h"
#else
#include "krossScriptBackend.h"
#include <kross/core/manager.h>
#endif

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

#ifdef USING_QTSCRIPT
  _cmbEngine->addItem(QString("JavaScript"));
#else
  foreach(QString s, Kross::Manager::self().interpreters()){
    _cmbEngine->addItem(s);
  }
#endif

}

void GraphEditWidget::setGraphDocument( GraphDocument *gd){
  if ( _graphDocument != 0 ){
    releaseGraphDocument();
   }
  
  int size = gd->size();
  for(int i = 0; i < size; i++){
    connectGraphSignals( gd->at(i) );
    drawGraphOnScene( gd->at(i) );	
  }
  
  _graphDocument = gd;
  _graphScene->setSceneRect(gd->width(), gd->height() );
}

void GraphEditWidget::releaseGraphDocument(){
  QList<QGraphicsItem*> itemList = _graphScene->items();
  foreach(QGraphicsItem *i, itemList){
    _graphScene->removeItem(i);
    delete i;
  }
  int size = _graphDocument->size();
  for(int i = 0; i < size; i++){
    _graphDocument->at(i)->disconnect(this);
  }
}

void GraphEditWidget::drawGraphOnScene( Graph *g){
  QList< Node*> nodes = g->nodes();
  QList< Edge*> edges = g->edges();

  foreach( Node* node, nodes){
    _graphScene->createNode(node);
  }

  foreach( Edge* edge, edges){
    _graphScene->createEdge(edge);
  }
}

GraphScene *GraphEditWidget::scene() const{
  return _graphScene;
}

void GraphEditWidget::setGraph( Graph *graph){
  _graph = graph;
  connectGraphSignals(graph);
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

void GraphEditWidget::connectGraphSignals( Graph *graph){
  connect(graph,SIGNAL(nodeRemoved(int)),this,SLOT(removeNode(int)));
  connect(graph,SIGNAL(edgeRemoved(int)),this,SLOT(removeEdge(int)));
}


void GraphEditWidget::on__btnRunScript_clicked(){

 if ( _graphDocument == 0 ) return;
#ifdef USING_QTSCRIPT
  QtScriptBackend e( _graphDocument, _txtDebug );
  e.evaluate( _txtEditScriptDocument -> text() );
#else
  KrossBackend e( _txtDebug) ;
  e.setDocument( _graphDocument );
  e.setEngine( _cmbEngine->currentText());
  e.execute( _txtEditScriptDocument -> text() );
#endif
}
