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

// KDE Related Includes
#include <KLocale>
#include <KAction>
#include <KStandardAction>
#include <KActionCollection>
#include <kxmlguifactory.h>

// UI RELATED INCLUDES
#include "UI_MainWindow.h"  
#include "UI_PaletteBarDockWidget.h" 
#include "UI_GraphLayersDockWidget.h"
#include "UI_OpenedFilesDockWidget.h"
#include "UI_GraphEditWidget.h"
#include "UI_GraphScene.h"

// MODEL Related Includes
#include "model_GraphDocument.h"
#include "model_GraphLayers.h"

// Graph Related Includes
#include "libgraph_GraphDocument.h"
#include "libgraph_Graph.h"

// Action Related Includes
#include "action_AbstractAction.h"
#include "action_Pointer.h"
#include "action_AddNode.h"
#include "action_AddEdge.h"
#include "action_MoveNode.h"

MainWindow::MainWindow() : KXmlGuiWindow(){

  _documentModel = 0;
  _graphLayersModel = 0;

  setObjectName ( "Rocs" );

  setupModels();
  setupWidgets();
  setupGUI();
  setupActions();
  setupSignals();
}

void MainWindow::setupModels(){
  _documentModel = new GraphDocumentModel( &_documents );
  _graphLayersModel = new GraphLayersModel( 0 );
}
void MainWindow::setupWidgets(){
  setCorner ( Qt::TopLeftCorner,     Qt::LeftDockWidgetArea );
  setCorner ( Qt::BottomLeftCorner,  Qt::LeftDockWidgetArea );
  setCorner ( Qt::TopRightCorner,    Qt::RightDockWidgetArea );
  setCorner ( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

  _OpenedFiles     = new OpenedFilesDockWidget ( _documentModel, this );
  _PaletteBar      = new PaletteBarDockWidget  ( this );
  _GraphLayers     = new GraphLayersDockWidget ( this );
  _GraphEdit       = new GraphEditWidget( this );

  addDockWidget ( Qt::LeftDockWidgetArea, _PaletteBar );
  addDockWidget ( Qt::LeftDockWidgetArea, _GraphLayers );
  addDockWidget ( Qt::RightDockWidgetArea, _OpenedFiles );

  setCentralWidget ( _GraphEdit );
}

void MainWindow::setupActions(){
  KStandardAction::quit ( this,    SLOT ( quit() ),        actionCollection() );
  GraphScene *gc = _GraphEdit->scene();

  _paletteActions = new KActionCollection(qobject_cast<QObject*>(this));
  _paletteActions->addAction("pointer_action", new PointerAction(gc, this));  
  _paletteActions->addAction("add_node_action", new AddNodeAction(gc, this));
  _paletteActions->addAction("add_edge_action", new AddEdgeAction(gc, this));
  _paletteActions->addAction("move_node_action", new MoveNodeAction(gc, this));
  _PaletteBar->setActionCollection(_paletteActions);

  // Pointer Action is the first. 
  _GraphEdit->scene()->setAction(qobject_cast<AbstractAction*>(_paletteActions->actions()[0]));

}

void MainWindow::setupSignals(){
  connect( _OpenedFiles, SIGNAL(activeDocumentChanged(libgraph::GraphDocument*)),
	   _GraphLayers, SLOT(setGraphDocument(libgraph::GraphDocument*)));

  connect( _OpenedFiles, SIGNAL(activeDocumentChanged(libgraph::GraphDocument*)),
	   _GraphEdit,   SLOT(setGraphDocument(libgraph::GraphDocument*)));

  connect( _OpenedFiles, SIGNAL(activeDocumentChanged(libgraph::GraphDocument*)),
	   _PaletteBar, SLOT(setGraphDocument(libgraph::GraphDocument*)));
  
  connect( _GraphLayers, SIGNAL(activeGraphChanged(libgraph::Graph*)),
	   _PaletteBar, SLOT(setGraph(libgraph::Graph*)));

  connect( _GraphLayers, SIGNAL(activeGraphChanged(libgraph::Graph*)),
	   this, SLOT(setGraph(libgraph::Graph*)));
  
  foreach( QAction *action, _paletteActions->actions() ){
      connect( _GraphLayers, SIGNAL(activeGraphChanged(libgraph::Graph*)),
      action, SLOT(setGraph(libgraph::Graph*)));
  }
}

void MainWindow::setGraph(libgraph::Graph *g){
  _PaletteBar -> setGraph(g);
  _GraphEdit -> setGraph(g);
}