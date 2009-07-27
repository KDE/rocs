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
#include <KActionCollection>
#include <KIcon>
#include <QSplitter>
#include <KDebug>
#include <KApplication>
#include <KTextBrowser>
#include <KMessageBox>
#include <kfiledialog.h>
#include "TabWidget.h"

// UI RELATED INCLUDES

#include "SUI_PaletteBarWidget.h"
#include "SUI_OpenedFilesWidget.h"
#include "SUI_GraphPropertiesWidget.h"
#include "SUI_GraphVisualEditor.h"
#include "SUI_GraphScene.h"
#include "CodeEditor.h"

// MODEL Related Includes
#include "model_GraphDocument.h"

// Graph Related Includes
#include "graphDocument.h"
#include "graph.h"

#include "settings.h"

// Action Related Includes
#include "AbstractAction.h"
#include "Pointer.h"
#include "AddNode.h"
#include "AddEdge.h"
#include "MoveNode.h"
#include "Select.h"
#include "SmartAction.h"
#include "DeleteAction.h"

// backends
#include "qtScriptBackend.h"
#include <kstandarddirs.h>

MainWindow* mainWindow = 0;

MainWindow::MainWindow() :
        KXmlGuiWindow(){
    _documentModel = 0;
    _activeGraphDocument = 0;
    _graph = 0;
    setObjectName ( "Rocs" );

    setupModels();
    setupWidgets();
    setupActions();
    setupSignals();

    // this will create a new opened file by default.
    _OpenedFiles->on__btnNewFile_clicked();
    _activeGraphDocument->addGraph("Untitled0");
    setupGUI();
    _moveNodeAction->setView( _graphVisualEditor->view() );
    mainWindow = this;
  _OpenedFiles->selectDefaultFile();
}

MainWindow::~MainWindow() {
    Settings::setVSplitterSizeTop( _vSplitter->sizes()[0] );
    Settings::setVSplitterSizeBottom( _vSplitter->sizes()[1] );
    Settings::setHSplitterSizeLeft( _hSplitter->sizes()[0] );
    Settings::setHSplitterSizeRight( _hSplitter->sizes()[1] );

    Settings::self()->writeConfig();
    kDebug() << "Config	File Written";

}
void MainWindow::setupModels() {
    _documentModel = new GraphDocumentModel( &_documents );
}

GraphDocument *MainWindow::activeDocument() const{ return _activeGraphDocument; }

void MainWindow::setupWidgets() {
    _hSplitter = new QSplitter(this);

    QWidget *rightPanel = setupRightPanel();
    QWidget *leftPanel	= setupLeftPanel();
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
  _bottomTabs->addWidget(_codeEditor,  "Editor", KIcon("accessories-text-editor"));
  _bottomTabs->addWidget(_txtDebug, "Debugger", KIcon("debugger"));
   _runScript = new KAction(KIcon("system-run"), "Run", this);
    connect(_runScript, SIGNAL(triggered()), this, SLOT(executeScript()));
  _bottomTabs->addAction(_runScript);

    _vSplitter = new QSplitter(this);
    _vSplitter->setOrientation(Qt::Vertical);
    _vSplitter->addWidget(_graphVisualEditor);
    _vSplitter->addWidget(_bottomTabs);
    _vSplitter->setSizes( QList<int>() << Settings::vSplitterSizeTop() << Settings::vSplitterSizeBottom() );
    return _vSplitter;
}

QWidget* MainWindow::setupLeftPanel() {
    _leftTabs = new TabWidget(TabWidget::TabOnLeft, this);
 
    QSplitter *palletePropertiesHolder = new QSplitter(this);
    palletePropertiesHolder->setOrientation(Qt::Vertical);
    
    _OpenedFiles = new OpenedFilesWidget ( _documentModel, this );
    _PaletteBar	= new PaletteBarWidget	( this );
    _GraphProperties = new GraphPropertiesWidget( this );

    palletePropertiesHolder -> setLayout(new QVBoxLayout());
    palletePropertiesHolder -> layout() -> addWidget( _PaletteBar );
    palletePropertiesHolder -> layout() -> addWidget( _GraphProperties );
  
   _leftTabs->addWidget( _OpenedFiles,  "Files", KIcon("document-open"));
   _leftTabs->addWidget( palletePropertiesHolder, "Tools" , KIcon("applications-system"));

    return _leftTabs;
}

void MainWindow::setupActions() {
    KStandardAction::quit ( this,		SLOT ( quit() ),				actionCollection() );
    GraphScene *gc = _graphVisualEditor->scene();
    if (!gc) {
        kDebug() << "There is no graph scene at this point.";
        return;
    }
    _paletteActions = new KActionCollection(qobject_cast<QObject*>(this));
    _paletteActions->addAction("pointer_action", new PointerAction(gc, this));
    _paletteActions->addAction("add_node_action", new AddNodeAction(gc, this));
    _paletteActions->addAction("add_edge_action", new AddEdgeAction(gc, this));
    _moveNodeAction = new MoveNodeAction(gc, this);
    _paletteActions->addAction("move_node_action", _moveNodeAction);
    _paletteActions->addAction("select_action", new SelectAction(gc, this));
    _paletteActions->addAction("delete_action", new DeleteAction(gc, this));
    _PaletteBar->setActionCollection(_paletteActions);

    // Pointer Action is the first.
    gc -> setAction(qobject_cast<AbstractAction*>(_paletteActions->actions()[0]));

    KAction* action = new KAction(KIcon("document-new"),i18n("New Graph"),  this);
    //clearAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("new-graph", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(newGraph()));

    action = new KAction(KIcon("document-open"),i18n("Open Graph"), this);
    //clearAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("open-graph", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(openGraph()));

    action = new KAction(KIcon("document-save"), i18n("Save Graph"), this);
    //clearAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("save-graph", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(saveGraph()));

    action = new KAction(KIcon("document-save-as"),i18n("Save Graph As"), this);
    //clearAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("save-graph-as", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(saveGraphAs()));

    action = new KAction(KIcon("document-new"),i18n("New Script"), this);
    //clearAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("new-script", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(newScript()));

    action = new KAction(KIcon("document-open"),i18n("Open Script"), this);
    //clearAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("open-script", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(openScript()));

    action = new KAction(KIcon("document-save"),i18n("Save Script"), this);
    //clearAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("save-script", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(saveScript()));

    action = new KAction(KIcon("document-save-as"), i18n("Save Script As"), this);
    //clearAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("save-script-as", action);
   // connect(action, SIGNAL(triggered(bool)), this, SLOT(saveScriptAs()));

    KStandardAction::quit(kapp, SLOT(quit()),  actionCollection());
}

void MainWindow::setupSignals() {
    connect( _OpenedFiles, SIGNAL(activeDocumentChanged(GraphDocument*)),
             this,	 SLOT(setActiveGraphDocument(GraphDocument*)));

    //! action here is "selection action". please, change that to a more intuitive approach.
    connect( _paletteActions->action(4), SIGNAL(ItemSelectedChanged(QGraphicsItem*)),
             _GraphProperties, SLOT(setDataSource(QGraphicsItem*)));

}

void MainWindow::setActiveGraphDocument(GraphDocument* d) {
    if (_paletteActions == 0) {
        kDebug() << " ERROR: There isn't pallete Actions.";
        return;
    }
    _activeGraphDocument = d;
    _graphVisualEditor->setActiveGraphDocument(d);
    foreach( QAction *action, _paletteActions->actions() ) {
        AbstractAction *absAction = qobject_cast<AbstractAction*>(action);
        if (!absAction) {
            kDebug() << "ERROR: Invalid conversion, it should be AbstractAction but it isn't. ";
            continue;
        }
        absAction->setActiveGraphDocument(d);
    }

    if (_activeGraphDocument->size() == 0) return;
    setActiveGraph(_activeGraphDocument->at(0));
}

void MainWindow::setActiveGraph( Graph *g) {
    if (_paletteActions == 0) {
        kDebug() << "ERROR : There isn't pallete actions";
        return;
    }
    if ( _activeGraphDocument  == 0) {
        kDebug() << "ERROR : Theres no activeGraphDocument, but this graph should beong to one.";
        return;
    }
    if ( _activeGraphDocument->indexOf(g) == -1) {
        kDebug() << "ERROR: this graph doesn't belong to the active document";
        return;
    }
    foreach( QAction *action, _paletteActions->actions() ) {
        AbstractAction *absAction = qobject_cast<AbstractAction*>(action);
        if (!absAction) {
            kDebug() << "ERROR: Invalid conversion, it should be AbstractAction but it isn't. ";
            continue;
        }
        absAction->setActiveGraph(g);
    }

    _graphVisualEditor->setActiveGraph(g);
    _graph = g;
}

Graph* MainWindow::graph() const {
    return _graph;
}

GraphScene* MainWindow::scene() const {
    return _graphVisualEditor->scene();
}

void MainWindow::newGraph() {
    _OpenedFiles->on__btnNewFile_clicked();
}

void MainWindow::openGraph() {
    _OpenedFiles->on__btnNewFile_clicked();
    _activeGraphDocument->loadFromInternalFormat(KFileDialog::getOpenFileName());
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

void MainWindow::newScript() {}
void MainWindow::openScript() {}

void MainWindow::debug(const QString& s) {
    _txtDebug->insertPlainText(s);
}

#ifdef USING_QTSCRIPT

static QScriptValue debug_script(QScriptContext* context, QScriptEngine* /*engine*/) {
    mainWindow->debug(QString("%1 \n").arg(context->argument(0).toString()));
    return QScriptValue();
}

void MainWindow::executeScript() {
    if (_activeGraphDocument == 0) {     return;     }
    if (_txtDebug == 0) {    return;    }

    _txtDebug->clear();
    QtScriptBackend *engine = new QtScriptBackend((*_activeGraphDocument),  _txtDebug);
    engine->globalObject().setProperty("debug", engine->newFunction(debug_script));
    QScriptValue results = engine->evaluate(_codeEditor->text());
    _txtDebug->insertPlainText(results.toString());

    if (scene() == 0) {   return;    }

    _graphVisualEditor->scene()->updateDocument();
}

#endif
