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
#include <kmultitabbar.h>
#include <QStackedWidget>
#include <QPixmap>
#include <KDebug>

// UI RELATED INCLUDES

#include "SUI_PaletteBarWidget.h" 
#include "SUI_GraphLayersWidget.h"
#include "SUI_OpenedFilesWidget.h"
#include "SUI_GraphPropertiesWidget.h"
#include "SUI_GraphVisualEditor.h"
// #include "SUI_GraphScene.h"

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

MainWindow::MainWindow() : 
	KXmlGuiWindow(),
	_tabId(0)
	{
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
	QWidget *leftPanel = setupLeftPanel();
	setCentralWidget(leftPanel);
}

QWidget* MainWindow::setupLeftPanel(){
	//! constructing the Default Looking LeftSide menu.
	QWidget *toolBox = new QWidget( this );
	toolsTab = new KMultiTabBar(KMultiTabBar::Left, toolBox);
	toolsTab->setStyle(KMultiTabBar::KDEV3ICON);

	toolsTab->appendTab(QPixmap(), 0, "Files");
	toolsTab->appendTab(QPixmap(), 1, "Tools");
	toolsTab->appendTab(QPixmap(), 2, "Properties");
	toolsTab->appendTab(QPixmap(), 3, "Layers");

  _OpenedFiles     = new OpenedFilesWidget ( _documentModel, toolBox );
  _PaletteBar      = new PaletteBarWidget  ( toolBox );
  _GraphLayers     = new GraphLayersWidget ( toolBox );
  _GraphProperties = new GraphPropertiesWidget( toolBox ); 
	
	QStackedWidget *toolsStack = new QStackedWidget();
	toolsStack->addWidget( _OpenedFiles );
	toolsStack->addWidget( _PaletteBar );
	toolsStack->addWidget( _GraphProperties );
	toolsStack->addWidget( _GraphLayers );

	QHBoxLayout  *toolBoxLayout = new QHBoxLayout( toolBox );
	toolBoxLayout->addWidget(toolsTab);
	toolBoxLayout->addWidget(toolsStack);
	toolBox->setLayout(toolBoxLayout);

	for(int i = 0; i < 4; ++i){
		connect(toolsTab->tab(i), SIGNAL(clicked(int)), toolsStack, SLOT(setCurrentIndex(int)));
		connect(toolsTab->tab(i), SIGNAL(clicked(int)), this, SLOT(releaseLeftTabbarButton(int)));
	}
	toolsTab->setTab(0, true);
	return toolBox;
}
void MainWindow::releaseLeftTabbarButton(int index){
	if ( _tabId == index ){
		toolsTab->setTab( _tabId, true );
		return;
	}
	toolsTab->setTab( _tabId, false );
	_tabId = index;
}

void MainWindow::setupActions(){

  KStandardAction::quit ( this,    SLOT ( quit() ),        actionCollection() );
 // GraphScene *gc = _GraphEdit->scene();

 // _paletteActions = new KActionCollection(qobject_cast<QObject*>(this));
 // _paletteActions->addAction("pointer_action", new PointerAction(gc, this));  
 // _paletteActions->addAction("add_node_action", new AddNodeAction(gc, this));
 // _paletteActions->addAction("add_edge_action", new AddEdgeAction(gc, this));
 // _paletteActions->addAction("move_node_action", new MoveNodeAction(gc, this));
 // _paletteActions->addAction("single_selection_action", new SingleSelectAction(gc, this));

//   _PaletteBar->setActionCollection(_paletteActions);

  // Pointer Action is the first. 
//  _GraphEdit->scene()->setAction(qobject_cast<AbstractAction*>(_paletteActions->actions()[0]));


}

void MainWindow::setupSignals(){

  connect( _OpenedFiles, SIGNAL(activeDocumentChanged(GraphDocument*)),
	   _GraphLayers, SLOT(setGraphDocument(GraphDocument*)));

//  connect( _OpenedFiles, SIGNAL(activeDocumentChanged(GraphDocument*)),
//	   _GraphEdit,   SLOT(setGraphDocument(GraphDocument*)));

  connect( _OpenedFiles, SIGNAL(activeDocumentChanged(GraphDocument*)),
	   _PaletteBar, SLOT(setGraphDocument(GraphDocument*)));
  
  connect( _GraphLayers, SIGNAL(activeGraphChanged(Graph*)),
	   _PaletteBar, SLOT(setGraph(Graph*)));

  connect( _GraphLayers, SIGNAL(activeGraphChanged(Graph*)),
	   this, SLOT(setGraph(Graph*)));
  
 // connect( _paletteActions->action(4), SIGNAL(ItemSelectedChanged(QObject*)),
//	  _GraphProperties, SLOT(setDataSource(QObject*)));


 // foreach( QAction *action, _paletteActions->actions() ){
 //     connect( _GraphLayers, SIGNAL(activeGraphChanged(Graph*)),
 //     action, SLOT(setGraph(Graph*)));
 // }

}

void MainWindow::setGraph( Graph *g){

  _PaletteBar -> setGraph(g);
  // _GraphEdit -> setGraph(g);

}
