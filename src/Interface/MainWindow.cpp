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

#include "MainWindow.h"
//Qt related includes

// KDE Related Includes
#include <KActionCollection>
#include <KIcon>
#include <QSplitter>
#include <KDebug>
#include <KApplication>
#include <KTextBrowser>
#include <KMessageBox>
#include <KApplication>
#include <QGraphicsView>
#include <klocalizedstring.h>
#include <KStatusBar>

#include <kfiledialog.h>
#include "TabWidget.h"

// UI RELATED INCLUDES

#include "GraphLayers.h"
#include "GraphVisualEditor.h"
#include "GraphScene.h"
#include "CodeEditor.h"

// MODEL Related Includes
#include "model_GraphDocument.h"

// Graph Related Includes
#include "graphDocument.h"
#include "graph.h"

#include "settings.h"

// Action Related Includes
#include "AbstractAction.h"
#include "AddNode.h"
#include "AddEdge.h"
#include "MoveNode.h"
#include "Select.h"
#include "DeleteAction.h"
#include "AlignAction.h"

// backends
#include "qtScriptBackend.h"
#include <kstandarddirs.h>
#include <ktexteditor/view.h>
#include <ktexteditor/editor.h>
#include <ktexteditor/document.h>
#include <qscriptenginedebugger.h>
#include <QActionGroup>
#include "threadScriptExecution.h"
#include "../Plugins/PluginManager.h"

MainWindow* mainWindow = 0;

MainWindow::MainWindow() :
        KXmlGuiWindow() {
    _activeGraphDocument = 0;
    _graph = 0;
    _tScriptExecution=0;
    setObjectName ( "Rocs" );

        
    kDebug() << "############ Load Plugins ###############";
    Rocs::PluginManager::New()->loadPlugins();
//     if (Rocs::ToolsPluginManager::New()->plugins().size() > 0){
// 	foreach (Rocs::ToolsPluginInterface * plugin, Rocs::ToolsPluginManager::New()->plugins()){
// 	      this->addClient(plugin);
// 	}
//     }
//     kDebug() << Rocs::ToolsPluginManager::New()->plugins();
    
    setupWidgets();
    setupActions();

    // this will create a new opened file by default
    setActiveGraphDocument( new GraphDocument(i18n("Untitled"), 800, 600));
    _activeGraphDocument->addGraph(i18n("Untitled0"));
    setupGUI();
    _moveNodeAction->setView( _graphVisualEditor->view() );

    mainWindow = this;
    statusBar()->hide();
    _GraphLayers->populate();



}

MainWindow::~MainWindow() {
    Settings::setVSplitterSizeTop( _vSplitter->sizes()[0] );
    Settings::setVSplitterSizeBottom( _vSplitter->sizes()[1] );
    Settings::setHSplitterSizeLeft( _hSplitter->sizes()[0] );
    Settings::setHSplitterSizeRight( _hSplitter->sizes()[1] );

    Settings::self()->writeConfig();
    delete _tScriptExecution;
}

GraphDocument *MainWindow::activeDocument() const {
    if (! _activeGraphDocument) {
        kDebug() << "Document is NULL";
    }
    return _activeGraphDocument;
}

void MainWindow::setupWidgets() {
    _hSplitter = new QSplitter(this);

    QWidget *rightPanel = setupRightPanel();
    QWidget *leftPanel	= setupLeftPanel();
     
    _tScriptExecution = new ThreadScriptExecution(_txtDebug);
    connect(_tScriptExecution, SIGNAL(finished()),_bottomTabs, SLOT(setPlayString()));
    connect(_tScriptExecution, SIGNAL(terminated()), _bottomTabs, SLOT(setPlayString()));
    connect(_tScriptExecution, SIGNAL(destroyed(QObject*)), _bottomTabs, SLOT(setPlayString()));
    
    _hSplitter->addWidget(leftPanel);
    _hSplitter->addWidget(rightPanel);
    _hSplitter->setSizes( QList<int>() << Settings::hSplitterSizeLeft() << Settings::hSplitterSizeRight());

    setCentralWidget(_hSplitter);
}

QWidget* MainWindow::setupRightPanel() {
    _graphVisualEditor = new GraphVisualEditor(this);
    _codeEditor = new CodeEditor(this);
    _txtDebug = new KTextBrowser(this);

    _bottomTabs = new TabWidget(TabWidget::TabOnBottom, this);
    _bottomTabs->addWidget(_codeEditor,  i18n("Editor"), KIcon("accessories-text-editor"));
    _bottomTabs->addWidget(_txtDebug, i18n("Debugger"), KIcon("tools-report-bug"));
    _runScript = new KAction(KIcon("system-run"), i18n("Run"), this);
    connect(_runScript, SIGNAL(triggered()), this, SLOT(executeScript()));
    _bottomTabs->addAction(_runScript);

    _vSplitter = new QSplitter(this);
    _vSplitter->setOrientation(Qt::Vertical);
    _vSplitter->addWidget(_graphVisualEditor);
    _vSplitter->addWidget(_bottomTabs);
    _vSplitter->setSizes( QList<int>() << Settings::vSplitterSizeTop() << Settings::vSplitterSizeBottom() );
   // _bottomTabs->actions().first()->setChecked(true);
    return _vSplitter;
}

QWidget* MainWindow::setupLeftPanel() {
    _GraphLayers = new GraphLayers(this);
    return _GraphLayers;
}

void MainWindow::setupActions() {
    kDebug() << "Entering in Setup Actions";
    KStandardAction::quit( kapp,SLOT(quit()),actionCollection());
    GraphScene *gc = _graphVisualEditor->scene();

    _moveNodeAction = new MoveNodeAction(gc, this);

    KActionCollection *ac = actionCollection();
    QActionGroup *g = new QActionGroup(this);

    g->addAction(ac->addAction("move_node", _moveNodeAction));
    g->addAction(ac->addAction("add_node", new AddNodeAction(gc, this)));
    g->addAction(ac->addAction("add_edge", new AddEdgeAction(gc, this)));
    g->addAction(ac->addAction("select", new SelectAction(gc, this)));
    g->addAction(ac->addAction("delete", new DeleteAction(gc, this)));
    actionCollection()->action("move_node")->toggle();
    gc->setAction(_moveNodeAction);

    ac->addAction("align-hbottom",new AlignAction( i18n("Align on the base"),  AlignAction::Bottom, _graphVisualEditor ));
    ac->addAction("align-hcenter",new AlignAction( i18n("Align on the center"),AlignAction::HCenter,_graphVisualEditor ));
    ac->addAction("align-htop",   new AlignAction( i18n("Align on the top"),   AlignAction::Top,    _graphVisualEditor ));
    ac->addAction("align-vleft",  new AlignAction( i18n("Align on the left"),  AlignAction::Left,   _graphVisualEditor ));
    ac->addAction("align-vcenter",new AlignAction( i18n("Align on the center"),AlignAction::VCenter,_graphVisualEditor ));
    ac->addAction("align-vright", new AlignAction( i18n("Align on the right"), AlignAction::Right,  _graphVisualEditor ));

    KAction* action = new KAction(KIcon("document-new"), i18n("New Graph"),  _graphVisualEditor->view());
    action->setShortcut(Qt::CTRL + Qt::Key_N);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("new-graph", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(newGraph()));

    action = new KAction(KIcon("document-open"),i18n("Open Graph..."), _graphVisualEditor->view());
    action->setShortcut(Qt::CTRL + Qt::Key_O);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("open-graph", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(openGraph()));

    action = new KAction(KIcon("document-save"), i18n("Save Graph"), _graphVisualEditor->view());
    action->setShortcut(Qt::CTRL + Qt::Key_S);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("save-graph", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(saveGraph()));

    action = new KAction(KIcon("document-save-as"),i18n("Save Graph As..."), _graphVisualEditor->view());
    action->setShortcut(Qt::CTRL + Qt::Key_W);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("save-graph-as", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(saveGraphAs()));

    action = new KAction(KIcon("document-new"),i18n("New Script"), this);
    action->setShortcut(Qt::CTRL + Qt::Key_N);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("new-script", action);
    connect(action, SIGNAL(triggered(bool)), _codeEditor, SLOT(newScript()));

    action = new KAction(KIcon("document-open"),i18n("Open Script..."), this);
    action->setShortcut(Qt::CTRL + Qt::Key_O);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("open-script", action);
    connect(action, SIGNAL(triggered(bool)), _codeEditor, SLOT(openScript()));

    action = new KAction(KIcon("document-save"),i18n("Save Script"), this);
    action->setShortcut(Qt::CTRL + Qt::Key_S);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("save-script", action);
    connect(action, SIGNAL(triggered(bool)), _codeEditor, SLOT(saveScript()));

    action = new KAction(KIcon("document-save-as"), i18n("Save Script As..."), this);
    action->setShortcut(Qt::CTRL + Qt::Key_W);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("save-script-as", action);
    connect(action, SIGNAL(triggered(bool)), _codeEditor, SLOT(saveScriptAs()));
    
    if (Rocs::PluginManager::New()->plugins().size() > 0){
	  QObject * plugin = Rocs::PluginManager::New()->plugins()[0];
	  action = new KAction(Rocs::PluginManager::New()->plugins()[0]->pluginId(), 
			      plugin);
      //     action->setShortcut(Qt::CTRL + Qt::Key_W);
      //     action->setShortcutContext(Qt::WidgetShortcut);
	  actionCollection()->addAction("make_complete", action);
	  connect(action, SIGNAL(triggered(bool)), this, SLOT(runToolPlugin()));
    }
    KStandardAction::quit(kapp, SLOT(quit()),  actionCollection());
}

void MainWindow::setActiveGraphDocument(GraphDocument* d)
{
    _activeGraphDocument = d;
    _graphVisualEditor->setActiveGraphDocument(d);
    foreach( QAction *action, actionCollection()->actions() ) {
        if (AbstractAction *absAction = qobject_cast<AbstractAction*>(action))
            absAction->setActiveGraphDocument(d);
    }

    if (_activeGraphDocument->size() == 0) return;
    setActiveGraph(_activeGraphDocument->at(0));
    _GraphLayers->populate();
}

void MainWindow::setActiveGraph( Graph *g)
{
    if ( _activeGraphDocument  == 0) {
        kDebug() << "ERROR : Theres no activeGraphDocument, but this graph should belong to one.";
        return;
    }
    if ( _activeGraphDocument->indexOf(g) == -1) {
        kDebug() << "ERROR: this graph does not belong to the active document";
        return;
    }
    foreach( QAction *action, actionCollection()->actions() ) {
        if (AbstractAction *absAction = qobject_cast<AbstractAction*>(action))
            absAction->setActiveGraph(g);
    }
    _graphVisualEditor->setActiveGraph(g);
    _graph = g;
    kDebug() << "New Active Graph Set: " << g->name();
}

Graph* MainWindow::graph() const {
    return _graph;
}

GraphScene* MainWindow::scene() const {
    return _graphVisualEditor->scene();
}

void MainWindow::newGraph() {
    if (saveIfChanged() == KMessageBox::Cancel) return;
    loadDocument();

}

void MainWindow::openGraph() {
    if (saveIfChanged() == KMessageBox::Cancel) return;
    QString fileName = KFileDialog::getOpenFileName(QString(), i18n("*.graph|Graph files\n*.*|All files"), this, i18n("Graph Files") );
    if (fileName == "") return;
    loadDocument(fileName);
}

void MainWindow::loadDocument(const QString& name){
  if (!name.isEmpty() && !name.endsWith(".graph")){
	KMessageBox::sorry(this, i18n("This does not seem to be a graph file."), i18n("Invalid file"));
	return;
  }

  _graphVisualEditor->releaseGraphDocument();
  delete _activeGraphDocument;
  
  setActiveGraphDocument(new GraphDocument(i18n("Untitled"), 800,600));
  if (name.isEmpty()){
    _activeGraphDocument->addGraph(i18n("Untitled0"));
  }
  else{
    _activeGraphDocument->loadFromInternalFormat(name);
  }
  _GraphLayers->populate();
  _graphVisualEditor->scene()->updateDocument();
}

void MainWindow::saveGraph() {
    if (_activeGraphDocument == 0) {
        kDebug() << "Graph Document is NULL";
        return;
    }

    if (_activeGraphDocument->documentPath().isEmpty() ) {
        saveGraphAs();
    }
    else {
        _activeGraphDocument->savedDocumentAt(_activeGraphDocument->documentPath());
    }
}

void MainWindow::saveGraphAs() {
    if (_activeGraphDocument == 0) {
        kDebug() << "Graph Document is NULL";
        return;
    }

    _activeGraphDocument->saveAsInternalFormat(KFileDialog::getSaveFileName());
}

void MainWindow::debug(const QString& s) {
    _txtDebug->insertPlainText(s);
}

int MainWindow::saveIfChanged(){
  if (_activeGraphDocument->isModified()){
     int btnCode;
     btnCode = KMessageBox::warningYesNoCancel(this, i18n("Do you want to save your unsaved document?"));
    if ( btnCode == KMessageBox::Yes){
      saveGraph();
    }
    return btnCode;
  }
  return KMessageBox::No;
}

#ifdef USING_QTSCRIPT
// move that to the thread thing.
// static QScriptValue debug_script(QScriptContext* context, QScriptEngine* /*engine*/) {
//     mainWindow->debug(QString("%1 \n").arg(context->argument(0).toString()));
//     return QScriptValue();
// }

void MainWindow::executeScript(QString text) {
    if (_activeGraphDocument == 0)  return;
    if (_txtDebug == 0)   return;
    if (scene() == 0)    return;

    _txtDebug->clear();
    _bottomTabs->setStopString();

    if ( !_tScriptExecution->isRunning() ){
	kDebug() << "Starting Script";
	if (text == ""){
	    _tScriptExecution->setData(_codeEditor->text(), _activeGraphDocument);
	}else{
	    _tScriptExecution->setData(text, _activeGraphDocument);
	}
        _tScriptExecution ->start();
        
    }else{
	kDebug() << "Aborting Script";
	_bottomTabs->setPlayString();
        _tScriptExecution->abort();
    }
}

#endif

void MainWindow::runToolPlugin(){
    QAction *action = qobject_cast<KAction *>(sender());
    Rocs::ToolsPluginInterface *plugin = qobject_cast<Rocs::ToolsPluginInterface *>(action->parent());
    QString run = plugin->run(0);
    executeScript(run);
     
}
