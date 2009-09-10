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
#include <QToolBox>

// KDE Related Includes
#include <KActionCollection>
#include <KIcon>
#include <QSplitter>
#include <KDebug>
#include <KApplication>
#include <KTextBrowser>
#include <KMessageBox>
#include <KApplication>
#include <KToolBar>
#include <QToolButton>
#include <QGraphicsView>
#include <kfiledialog.h>
#include "TabWidget.h"

// UI RELATED INCLUDES

#include "OpenedFilesWidget.h"
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
#include <KStatusBar>
#include <qscriptenginedebugger.h>

MainWindow* mainWindow = 0;

MainWindow::MainWindow() :
        KXmlGuiWindow() {
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
    statusBar()->hide();
    _GraphLayers->populate();
}

MainWindow::~MainWindow() {
    Settings::setVSplitterSizeTop( _vSplitter->sizes()[0] );
    Settings::setVSplitterSizeBottom( _vSplitter->sizes()[1] );
    Settings::setHSplitterSizeLeft( _hSplitter->sizes()[0] );
    Settings::setHSplitterSizeRight( _hSplitter->sizes()[1] );

    Settings::self()->writeConfig();
}

void MainWindow::setupModels() {
    _documentModel = new GraphDocumentModel( &_documents );
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
    _OpenedFiles = new OpenedFilesWidget ( _documentModel, this );
    _GraphLayers = new GraphLayers(this);
    _leftTabs->addWidget( _OpenedFiles,  "Files", KIcon("document-open"));
    _leftTabs->addWidget( _GraphLayers, "Properties" , KIcon("applications-system"));
    return _leftTabs;
}

void MainWindow::setupActions() {
    kDebug() << "Entrou no Setup Actions";
    KStandardAction::quit( kapp,SLOT(quit()),actionCollection());


    GraphScene *gc = _graphVisualEditor->scene();
    if (!gc) {
        return;
    }
    _moveNodeAction = new MoveNodeAction(gc, this);

    KActionCollection *ac = actionCollection();
    ac->addAction("move_node", _moveNodeAction);
    ac->addAction("add_node", new AddNodeAction(gc, this));
    ac->addAction("add_edge", new AddEdgeAction(gc, this));
    ac->addAction("select", new SelectAction(gc, this));
    ac->addAction("delete", new DeleteAction(gc, this));
    gc -> setAction(actionCollection()->action("move_node"));

    ac->addAction("align-hbottom",new AlignAction( "Align on the base",  AlignAction::Bottom, _graphVisualEditor ));
    ac->addAction("align-hcenter",new AlignAction( "Align on the center",AlignAction::HCenter,_graphVisualEditor ));
    ac->addAction("align-htop",   new AlignAction( "Align on the top",   AlignAction::Top,    _graphVisualEditor ));
    ac->addAction("align-vleft",  new AlignAction( "Align on the left",  AlignAction::Left,   _graphVisualEditor ));
    ac->addAction("align-vcenter",new AlignAction( "Align on the center",AlignAction::VCenter,_graphVisualEditor ));
    ac->addAction("align-vright", new AlignAction( "Align on the right", AlignAction::Right,  _graphVisualEditor ));

    KAction* action = new KAction(KIcon("document-new"),i18n("New Graph"),  _graphVisualEditor->view());
    action->setShortcut(Qt::CTRL + Qt::Key_N);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("new-graph", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(newGraph()));

    action = new KAction(KIcon("document-open"),i18n("Open Graph"), _graphVisualEditor->view());
    action->setShortcut(Qt::CTRL + Qt::Key_O);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("open-graph", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(openGraph()));

    action = new KAction(KIcon("document-save"), i18n("Save Graph"), _graphVisualEditor->view());
    action->setShortcut(Qt::CTRL + Qt::Key_S);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("save-graph", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(saveGraph()));

    action = new KAction(KIcon("document-save-as"),i18n("Save Graph As"), _graphVisualEditor->view());
    action->setShortcut(Qt::CTRL + Qt::Key_W);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("save-graph-as", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(saveGraphAs()));

    action = new KAction(KIcon("document-new"),i18n("New Script"), _codeEditor->view());
    action->setShortcut(Qt::CTRL + Qt::Key_N);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("new-script", action);
    connect(action, SIGNAL(triggered(bool)), _codeEditor, SLOT(newScript()));

    action = new KAction(KIcon("document-open"),i18n("Open Script"), _codeEditor->view());
    action->setShortcut(Qt::CTRL + Qt::Key_O);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("open-script", action);
    connect(action, SIGNAL(triggered(bool)), _codeEditor, SLOT(openScript()));

    action = new KAction(KIcon("document-save"),i18n("Save Script"), _codeEditor->view());
    action->setShortcut(Qt::CTRL + Qt::Key_S);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("save-script", action);
    connect(action, SIGNAL(triggered(bool)), _codeEditor, SLOT(saveScript()));

    action = new KAction(KIcon("document-save-as"), i18n("Save Script As"), _codeEditor->view());
    action->setShortcut(Qt::CTRL + Qt::Key_W);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction("save-script-as", action);
    connect(action, SIGNAL(triggered(bool)), _codeEditor, SLOT(saveScriptAs()));

    KStandardAction::quit(kapp, SLOT(quit()),  actionCollection());
}

void MainWindow::setupSignals() {
    connect( _OpenedFiles, SIGNAL(activeDocumentChanged(GraphDocument*)),
             this,	 SLOT(setActiveGraphDocument(GraphDocument*)));

//connect( actionCollection()->action("select"), SIGNAL(ItemSelectedChanged(QGraphicsItem*)),
//            _GraphProperties, SLOT(setDataSource(QGraphicsItem*)));

}

void MainWindow::setActiveGraphDocument(GraphDocument* d) {
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

void MainWindow::setActiveGraph( Graph *g) {
    if ( _activeGraphDocument  == 0) {
        kDebug() << "ERROR : Theres no activeGraphDocument, but this graph should beong to one.";
        return;
    }
    if ( _activeGraphDocument->indexOf(g) == -1) {
        kDebug() << "ERROR: this graph doesn't belong to the active document";
        return;
    }
    foreach( QAction *action, actionCollection()->actions() ) {
        if (AbstractAction *absAction = qobject_cast<AbstractAction*>(action))
            absAction->setActiveGraph(g);
    }
    _graphVisualEditor->setActiveGraph(g);
    _graph = g;
    kDebug() << "New Active Graph Setted: " << g->name();
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

void MainWindow::debug(const QString& s) {
    _txtDebug->insertPlainText(s);
}

#ifdef USING_QTSCRIPT

static QScriptValue debug_script(QScriptContext* context, QScriptEngine* /*engine*/) {
    mainWindow->debug(QString("%1 \n").arg(context->argument(0).toString()));
    return QScriptValue();
}

void MainWindow::executeScript() {
    if (_activeGraphDocument == 0) {
        return;
    }
    if (_txtDebug == 0) {
        return;
    }

    _txtDebug->clear();
    QtScriptBackend *engine = new QtScriptBackend((*_activeGraphDocument),  _txtDebug);
    QScriptEngineDebugger *e = new QScriptEngineDebugger(this);

    e->attachTo(engine);

    engine->globalObject().setProperty("debug", engine->newFunction(debug_script));
    QScriptValue results = engine->evaluate(_codeEditor->text());
    _txtDebug->insertPlainText(results.toString());

    if (scene() == 0) {
        return;
    }

    _graphVisualEditor->scene()->updateDocument();
}

#endif
