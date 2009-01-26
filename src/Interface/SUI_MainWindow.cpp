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
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with Step; if not, write to the Free Software
	 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
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
#include <KIcon>
#include <QSplitter>
#include <KDebug>
#include <ktexteditor/document.h>
#include <ktexteditor/view.h>
#include <ktexteditor/editor.h>
#include <ktexteditor/editorchooser.h>
#include <KTextBrowser>
#include <KMessageBox>
// UI RELATED INCLUDES

#include "SUI_PaletteBarWidget.h" 
#include "SUI_OpenedFilesWidget.h"
#include "SUI_GraphPropertiesWidget.h"
#include "SUI_GraphVisualEditor.h"
#include "SUI_GraphScene.h"

// MODEL Related Includes
#include "model_GraphDocument.h"

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

#include "settings.h" 

MainWindow::MainWindow() : 
	KXmlGuiWindow(),
	_leftTabId(0)
	{
	_documentModel = 0;
	
	setObjectName ( "Rocs" );

	setupModels();
	setupWidgets();
	setupGUI();
	setupActions();
	setupSignals();

	// this will create a new opened file by default.
	_OpenedFiles->on__btnNewFile_clicked();
}

MainWindow::~MainWindow(){
	Settings::setVSplitterSizeTop( _vSplitter->sizes()[0] );
	Settings::setVSplitterSizeBottom( _vSplitter->sizes()[1] );
	Settings::setHSplitterSizeLeft( _hSplitter->sizes()[0] );
	Settings::setHSplitterSizeRight( _hSplitter->sizes()[1] );

	Settings::self()->writeConfig();
	kDebug() << "Config	File Written";

}
void MainWindow::setupModels(){
	_documentModel = new GraphDocumentModel( &_documents );
}
void MainWindow::setupWidgets(){

	_leftTabBar = new KMultiTabBar(KMultiTabBar::Left, this);
	_leftTabBar->setStyle(KMultiTabBar::KDEV3ICON);

	_leftTabBar->appendTab(KIcon("document-open").pixmap(16), 0, "Files");
	_leftTabBar->appendTab(KIcon("applications-system").pixmap(16), 1, "Tools");
	_leftTabBar->appendTab(KIcon("document-properties").pixmap(16), 2, "Properties");

	QWidget *centralWidget = new QWidget(this);
	QHBoxLayout *l1 = new QHBoxLayout();

	_hSplitter = new QSplitter(this);

	QWidget *rightPanel = setupRightPanel();
	QWidget *leftPanel	= setupLeftPanel();
	_hSplitter->addWidget(leftPanel);
	_hSplitter->addWidget(rightPanel);
	_hSplitter->setSizes( QList<int>() << Settings::hSplitterSizeLeft() << Settings::hSplitterSizeRight());
	l1->addWidget( _leftTabBar);
	l1->addWidget( _hSplitter );

	centralWidget->setLayout(l1);

	setCentralWidget(centralWidget);
}

QWidget* MainWindow::setupRightPanel(){
// Top Area, The Graph Visual Editor.
	_graphVisualEditor = new GraphVisualEditor(this);

// Bottom Area, the rest. ( Script Editor, Debugger )
	KTextEditor::Editor *editor = KTextEditor::EditorChooser::editor();
	if (!editor){
		KMessageBox::error(this, i18n("A KDE Text Editor could not be found, \n please, check your installation"));
	}
	_scriptDoc = editor->createDocument(0);
	_docView = qobject_cast<KTextEditor::View*>(_scriptDoc->createView(this));
	
	_txtDebug = new KTextBrowser(this);

	QStackedWidget *toolsStack = new QStackedWidget();
	toolsStack->addWidget(_docView);
	toolsStack->addWidget(_txtDebug);

// Tab bar outside the main area, gee... I need a better way to document this.
	_bottomTabBar = new KMultiTabBar(KMultiTabBar::Bottom, this);
	_bottomTabBar->setStyle(KMultiTabBar::KDEV3ICON);
	_bottomTabBar->appendTab(KIcon("accessories-text-editor").pixmap(16), 0, "Editor");
	_bottomTabBar->appendTab(KIcon("debugger").pixmap(16), 1, "Debugger");
	_bottomTabBar->appendTab(KIcon("system-run").pixmap(16),2,"Run");

	// Connect the signals.
	connect(_bottomTabBar->tab(0), SIGNAL(clicked(int)), toolsStack, SLOT(setCurrentIndex(int)));
	connect(_bottomTabBar->tab(1), SIGNAL(clicked(int)), toolsStack, SLOT(setCurrentIndex(int)));
	connect(_bottomTabBar->tab(2), SIGNAL(clicked(int)), this, SLOT( executeScript()));

	// Configure the visual area.
	_vSplitter = new QSplitter(this);
	_vSplitter->setOrientation(Qt::Vertical);
	_vSplitter->addWidget(_graphVisualEditor);
	_vSplitter->addWidget(toolsStack);
	
	_vSplitter->setSizes( QList<int>() << Settings::vSplitterSizeTop() << Settings::vSplitterSizeBottom() );
	QWidget *widget = new QWidget( this );
	QVBoxLayout *l = new QVBoxLayout();
	l->addWidget(_vSplitter);
	l->addWidget(_bottomTabBar);
	widget->setLayout(l);
	return widget;
}

QWidget* MainWindow::setupLeftPanel(){
	//! constructing the Default Looking LeftSide menu.
	QWidget *toolBox = new QWidget( this );

	_OpenedFiles		 = new OpenedFilesWidget ( _documentModel, toolBox );
	_PaletteBar			= new PaletteBarWidget	( toolBox );
	_GraphProperties = new GraphPropertiesWidget( toolBox ); 

	QStackedWidget *toolsStack = new QStackedWidget();

	toolsStack->addWidget( _OpenedFiles );
	toolsStack->addWidget( _PaletteBar );
	toolsStack->addWidget( _GraphProperties );


	QHBoxLayout	*toolBoxLayout = new QHBoxLayout( toolBox );
	toolBoxLayout->addWidget(toolsStack);
	toolBox->setLayout(toolBoxLayout);

	for(int i = 0; i < 3; ++i){
		connect(_leftTabBar->tab(i), SIGNAL(clicked(int)), toolsStack, SLOT(setCurrentIndex(int)));
		connect(_leftTabBar->tab(i), SIGNAL(clicked(int)), this, SLOT(releaseLeftTabbarButton(int)));
	}
	_leftTabBar->setTab(0, true);
	return toolBox;
}
void MainWindow::releaseLeftTabbarButton(int index){
	if ( _leftTabId == index ){
		_leftTabBar->setTab( _leftTabId, true );
		return;
	}
	_leftTabBar->setTab( _leftTabId, false );
	_leftTabId = index;
}

void MainWindow::setupActions(){

	KStandardAction::quit ( this,		SLOT ( quit() ),				actionCollection() );
	GraphScene *gc = _graphVisualEditor->scene();

	_paletteActions = new KActionCollection(qobject_cast<QObject*>(this));
	_paletteActions->addAction("pointer_action", new PointerAction(gc, this));	
	_paletteActions->addAction("add_node_action", new AddNodeAction(gc, this));
	_paletteActions->addAction("add_edge_action", new AddEdgeAction(gc, this));
	_paletteActions->addAction("move_node_action", new MoveNodeAction(gc, this));
	_paletteActions->addAction("single_selection_action", new SingleSelectAction(gc, this));

	 _PaletteBar->setActionCollection(_paletteActions);

	// Pointer Action is the first. 
	gc -> setAction(qobject_cast<AbstractAction*>(_paletteActions->actions()[0]));

}

void MainWindow::setupSignals(){

	connect( _OpenedFiles, SIGNAL(activeDocumentChanged(GraphDocument*)),
		 _graphVisualEditor,	 SLOT(setGraphDocument(GraphDocument*)));

	// action here is "single selection action. please, change that to a more intuitive approach.
	connect( _paletteActions->action(4), SIGNAL(ItemSelectedChanged(QObject*)),
		_GraphProperties, SLOT(setDataSource(QObject*)));

}

void MainWindow::setGraph( Graph *g){
	if (_paletteActions == 0){
		kDebug() << "PALETTE = 0";
		return;
	}
	foreach( QAction *action, _paletteActions->actions() ){
		AbstractAction *absAction = qobject_cast<AbstractAction*>(action);
		absAction->setGraph(g);
	}
	_graphVisualEditor->scene()->setGraph(g);
	_graph = g;
}

Graph* MainWindow::graph() const{
	return _graph;
}

GraphScene* MainWindow::scene() const {
	return _graphVisualEditor->scene();
}
void MainWindow::executeScript(){

}
