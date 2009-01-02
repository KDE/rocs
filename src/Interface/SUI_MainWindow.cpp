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

#include "SUI_MainWindow.h"  

// KDE Related Includes
#include <KLocale>
#include <KAction>
#include <KStandardAction>
#include <KActionCollection>
#include <kxmlguifactory.h>

// UI RELATED INCLUDES

#include "SUI_PaletteBarDockWidget.h" 
#include "SUI_GraphLayersDockWidget.h"
#include "SUI_OpenedFilesDockWidget.h"
#include "SUI_PropertiesArea.h"
#include "SUI_GraphEditWidget.h"
#include "SUI_GraphScene.h"

// MODEL Related Includes
#include "model_GraphDocument.h"
#include "model_GraphLayers.h"

// Graph Related Includes
#include "graphDocument.h"
#include "graph.h"

// Action Related Includes
#include "action_AbstractAction.h"
#include "action_Pointer.h"
#include "action_AddNode.h"
#include "action_AddEdge.h"
#include "action_MoveNode.h"
#include "action_SingleSelect.h"

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
  _GraphProperties = new GraphPropertiesDockWidget( this ); 
  _GraphEdit       = new GraphEditWidget( this );

  addDockWidget ( Qt::RightDockWidgetArea, _GraphProperties);
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
  _paletteActions->addAction("single_selection_action", new SingleSelectAction(gc, this));

  _PaletteBar->setActionCollection(_paletteActions);

  // Pointer Action is the first. 
  _GraphEdit->scene()->setAction(qobject_cast<AbstractAction*>(_paletteActions->actions()[0]));

}

void MainWindow::setupSignals(){
  connect( _OpenedFiles, SIGNAL(activeDocumentChanged(GraphDocument*)),
	   _GraphLayers, SLOT(setGraphDocument(GraphDocument*)));

  connect( _OpenedFiles, SIGNAL(activeDocumentChanged(GraphDocument*)),
	   _GraphEdit,   SLOT(setGraphDocument(GraphDocument*)));

  connect( _OpenedFiles, SIGNAL(activeDocumentChanged(GraphDocument*)),
	   _PaletteBar, SLOT(setGraphDocument(GraphDocument*)));
  
  connect( _GraphLayers, SIGNAL(activeGraphChanged(Graph*)),
	   _PaletteBar, SLOT(setGraph(Graph*)));

  connect( _GraphLayers, SIGNAL(activeGraphChanged(Graph*)),
	   this, SLOT(setGraph(Graph*)));
  
  connect( _paletteActions->action(4), SIGNAL(ItemSelectedChanged(QObject*)),
	  _GraphProperties, SLOT(setDataSource(QObject*)));


  foreach( QAction *action, _paletteActions->actions() ){
      connect( _GraphLayers, SIGNAL(activeGraphChanged(Graph*)),
      action, SLOT(setGraph(Graph*)));
  }
}

void MainWindow::setGraph( Graph *g){
  _PaletteBar -> setGraph(g);
  _GraphEdit -> setGraph(g);
}
