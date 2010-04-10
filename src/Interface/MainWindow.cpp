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
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with Step; if not, write to the Free Software
     Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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
#include "threadDocument.h"
#include "../Plugins/PluginManager.h"
#include <KPushButton>
#include <QMutexLocker>

MainWindow::MainWindow() :  KXmlGuiWindow(), _mutex()
{
    setObjectName ( "Rocs" );

    kDebug() << "############ Load Plugins ###############";
    Rocs::PluginManager::New()->loadPlugins();


    kDebug() << "Creating Thread";
    _tDocument = new ThreadDocument ( _waitForDocument ,_mutex, this );

    // let's start the thread and wait for the graph document. to be created.
    _mutex.lock();
    kDebug() << "Starting Thread"; _tDocument->start();
    _waitForDocument.wait(&_mutex, 500);
    _mutex.unlock();

    // setting up the rest of stuff
    setupWidgets();
    setupActions();
    setupGUI();

    connect(activeDocument(), SIGNAL(activeGraphChanged(Graph*)), this, SLOT(setActiveGraph(Graph*)));
    connect(this, SIGNAL(startEvaluation()),    _tDocument->engine(), SLOT(start()));
    connect(this, SIGNAL(stopEvaluation()),     _tDocument->engine(), SLOT(stop()));
    
    //_moveNodeAction->setView( _graphVisualEditor->view() );

    statusBar()->hide();


    kDebug() << "################################";
    kDebug() << "Setting Graph Document";
    kDebug() << "#################################";

    setActiveGraphDocument ( _tDocument->document() );

    kDebug() << "#################################";
    kDebug() << "Graph Document Setted";
    kDebug() << "#################################";


    _GraphLayers->populate();

    setupToolsPluginsAction();
}

MainWindow::~MainWindow()
{
    Settings::setVSplitterSizeTop ( _vSplitter->sizes() [0] );
    Settings::setVSplitterSizeBottom ( _vSplitter->sizes() [1] );
    Settings::setHSplitterSizeLeft ( _hSplitter->sizes() [0] );
    Settings::setHSplitterSizeRight ( _hSplitter->sizes() [1] );

    Settings::self()->writeConfig();
    delete _tDocument;
}

void MainWindow::outputString ( const QString& s )
{
    _txtDebug->append ( s );
}

void MainWindow::debugString ( const QString& s )
{
    _txtDebug->append ( s );
}

GraphDocument *MainWindow::activeDocument() const
{
    if ( ! _tDocument->document() )
    {
        kDebug() << "Document is NULL";
    }
    return _tDocument->document();
}

void MainWindow::setupWidgets()
{
    _hSplitter = new QSplitter ( this );

    QWidget *rightPanel = setupRightPanel();
    QWidget *leftPanel  = setupLeftPanel();

    /*    connect(_tScriptExecution, SIGNAL(finished()),_bottomTabs, SLOT(setPlayString()));
        connect(_tScriptExecution, SIGNAL(terminated()), _bottomTabs, SLOT(setPlayString()));
        connect(_tScriptExecution, SIGNAL(destroyed(QObject*)), _bottomTabs, SLOT(setPlayString()));*/

    _hSplitter->addWidget ( leftPanel );
    _hSplitter->addWidget ( rightPanel );
    _hSplitter->setSizes ( QList<int>() << Settings::hSplitterSizeLeft() << Settings::hSplitterSizeRight() );

    setCentralWidget ( _hSplitter );
}

QWidget* MainWindow::setupRightPanel()
{
    _graphVisualEditor = new GraphVisualEditor ( this );
    _codeEditor = new CodeEditor ( this );
    _txtDebug = new KTextBrowser ( this );

    _bottomTabs = new TabWidget ( TabWidget::TabOnBottom, this );
    _bottomTabs->addWidget ( _codeEditor,  i18n ( "Editor" ), KIcon ( "accessories-text-editor" ) );
    _bottomTabs->addWidget ( _txtDebug, i18n ( "Debugger" ), KIcon ( "tools-report-bug" ) );
    _runScript = new KAction ( KIcon ( "system-run" ), i18n ( "Run" ), this );
    connect ( _runScript, SIGNAL ( triggered() ), this, SLOT ( executeScript() ) );
    _bottomTabs->addAction ( _runScript );

    _vSplitter = new QSplitter ( this );
    _vSplitter->setOrientation ( Qt::Vertical );
    _vSplitter->addWidget ( _graphVisualEditor );
    _vSplitter->addWidget ( _bottomTabs );
    _vSplitter->setSizes ( QList<int>() << Settings::vSplitterSizeTop() << Settings::vSplitterSizeBottom() );
    return _vSplitter;
}

QWidget* MainWindow::setupLeftPanel()
{
    _GraphLayers = new GraphLayers ( this );
    return _GraphLayers;
}

void MainWindow::setupActions()
{
    kDebug() << "Entering in Setup Actions";
    KStandardAction::quit ( kapp,SLOT ( quit() ),actionCollection() );
    GraphScene *gc = _graphVisualEditor->scene();

    _moveNodeAction = new MoveNodeAction ( gc, this );

    KActionCollection *ac = actionCollection();
    QActionGroup *g = new QActionGroup ( this );

    g->addAction ( ac->addAction ( "move_node", _moveNodeAction ) );
    g->addAction ( ac->addAction ( "add_node", new AddNodeAction ( gc, this ) ) );
    g->addAction ( ac->addAction ( "add_edge", new AddEdgeAction ( gc, this ) ) );
    g->addAction ( ac->addAction ( "select", new SelectAction ( gc, this ) ) );
    g->addAction ( ac->addAction ( "delete", new DeleteAction ( gc, this ) ) );
    actionCollection()->action ( "move_node" )->toggle();
    gc->setAction ( _moveNodeAction );

    ac->addAction ( "align-hbottom",new AlignAction ( i18n ( "Align on the base" ),  AlignAction::Bottom, _graphVisualEditor ) );
    ac->addAction ( "align-hcenter",new AlignAction ( i18n ( "Align on the center" ),AlignAction::HCenter,_graphVisualEditor ) );
    ac->addAction ( "align-htop",   new AlignAction ( i18n ( "Align on the top" ),   AlignAction::Top,    _graphVisualEditor ) );
    ac->addAction ( "align-vleft",  new AlignAction ( i18n ( "Align on the left" ),  AlignAction::Left,   _graphVisualEditor ) );
    ac->addAction ( "align-vcenter",new AlignAction ( i18n ( "Align on the center" ),AlignAction::VCenter,_graphVisualEditor ) );
    ac->addAction ( "align-vright", new AlignAction ( i18n ( "Align on the right" ), AlignAction::Right,  _graphVisualEditor ) );

    KAction* action = new KAction ( KIcon ( "document-new" ), i18n ( "New Graph" ),  _graphVisualEditor->view() );
    action->setShortcut ( Qt::CTRL + Qt::Key_N );
    action->setShortcutContext ( Qt::WidgetShortcut );
    actionCollection()->addAction ( "new-graph", action );
    connect ( action, SIGNAL ( triggered ( bool ) ), this, SLOT ( newGraph() ) );

    action = new KAction ( KIcon ( "document-open" ),i18n ( "Open Graph..." ), _graphVisualEditor->view() );
    action->setShortcut ( Qt::CTRL + Qt::Key_O );
    action->setShortcutContext ( Qt::WidgetShortcut );
    actionCollection()->addAction ( "open-graph", action );
    connect ( action, SIGNAL ( triggered ( bool ) ), this, SLOT ( openGraph() ) );

    action = new KAction ( KIcon ( "document-save" ), i18n ( "Save Graph" ), _graphVisualEditor->view() );
    action->setShortcut ( Qt::CTRL + Qt::Key_S );
    action->setShortcutContext ( Qt::WidgetShortcut );
    actionCollection()->addAction ( "save-graph", action );
    connect ( action, SIGNAL ( triggered ( bool ) ), this, SLOT ( saveGraph() ) );

    action = new KAction ( KIcon ( "document-save-as" ),i18n ( "Save Graph As..." ), _graphVisualEditor->view() );
    action->setShortcut ( Qt::CTRL + Qt::Key_W );
    action->setShortcutContext ( Qt::WidgetShortcut );
    actionCollection()->addAction ( "save-graph-as", action );
    connect ( action, SIGNAL ( triggered ( bool ) ), this, SLOT ( saveGraphAs() ) );

    action = new KAction ( KIcon ( "document-new" ),i18n ( "New Script" ), this );
    action->setShortcut ( Qt::CTRL + Qt::Key_N );
    action->setShortcutContext ( Qt::WidgetShortcut );
    actionCollection()->addAction ( "new-script", action );
    connect ( action, SIGNAL ( triggered ( bool ) ), _codeEditor, SLOT ( newScript() ) );

    action = new KAction ( KIcon ( "document-open" ),i18n ( "Open Script..." ), this );
    action->setShortcut ( Qt::CTRL + Qt::Key_O );
    action->setShortcutContext ( Qt::WidgetShortcut );
    actionCollection()->addAction ( "open-script", action );
    connect ( action, SIGNAL ( triggered ( bool ) ), _codeEditor, SLOT ( openScript() ) );

    action = new KAction ( KIcon ( "document-save" ),i18n ( "Save Script" ), this );
    action->setShortcut ( Qt::CTRL + Qt::Key_S );
    action->setShortcutContext ( Qt::WidgetShortcut );
    actionCollection()->addAction ( "save-script", action );
    connect ( action, SIGNAL ( triggered ( bool ) ), _codeEditor, SLOT ( saveScript() ) );

    action = new KAction ( KIcon ( "document-save-as" ), i18n ( "Save Script As..." ), this );
    action->setShortcut ( Qt::CTRL + Qt::Key_W );
    action->setShortcutContext ( Qt::WidgetShortcut );
    actionCollection()->addAction ( "save-script-as", action );
    connect ( action, SIGNAL ( triggered ( bool ) ), _codeEditor, SLOT ( saveScriptAs() ) );


    if ( Rocs::PluginManager::New()->filePlugins().count() > 0 )
    {
        kDebug() << "Creating Actions (import export)..";
        action = new KAction ( KIcon ( "document-save-as" ), i18n ( "Import" ), this );
        action->setShortcut ( Qt::CTRL + Qt::Key_I );
        action->setShortcutContext ( Qt::WidgetShortcut );
        actionCollection()->addAction ( "import", action );
        connect ( action, SIGNAL ( triggered ( bool ) ), this, SLOT ( importFile() ) );

        action = new KAction ( KIcon ( "document-save-as" ), i18n ( "Export" ), this );
        action->setShortcut ( Qt::CTRL + Qt::Key_E );
        action->setShortcutContext ( Qt::WidgetShortcut );
        actionCollection()->addAction ( "export", action );
        connect ( action, SIGNAL ( triggered ( bool ) ), this, SLOT ( exportFile() ) );
    }
    else
    {
        kDebug() << "Not Creating Actions (import export).." << Rocs::PluginManager::New()->filePlugins().count();
    }

    KStandardAction::quit ( kapp, SLOT ( quit() ),  actionCollection() );
}

void MainWindow::setupToolsPluginsAction()
{
    QList <QAction*> pluginList;
    QAction* action = 0;
    unplugActionList ( "tools_plugins" );
    QList < Rocs::ToolsPluginInterface*> avaliablePlugins = Rocs::PluginManager::New()->toolPlugins();
    foreach ( Rocs::ToolsPluginInterface* p, avaliablePlugins )
    {
        action = new KAction ( p->displayName(), p );
        connect ( action, SIGNAL ( triggered ( bool ) ),this, SLOT ( runToolPlugin() ) );
        pluginList.append ( action );
    }
    plugActionList ( "tools_plugins", pluginList );
}

void MainWindow::setActiveGraphDocument ( GraphDocument* d )
{
    foreach ( QAction *action, actionCollection()->actions() )
    {
        if ( AbstractAction *absAction = qobject_cast<AbstractAction*> ( action ) )
            absAction->setActiveGraphDocument ( d );
    }

    _graphVisualEditor->setActiveGraphDocument ( d );

    connect ( activeDocument(), SIGNAL ( activeGraphChanged ( Graph* ) ), this, SLOT ( setActiveGraph ( Graph* ) ) );

    connect ( this, SIGNAL ( runTool ( Rocs::ToolsPluginInterface* ) ), _tDocument->document(), SLOT ( runnTool ( Rocs::ToolsPluginInterface* ) ) );


    if ( _tDocument->document()->size() == 0 ) return;
    setActiveGraph ( _tDocument->document()->at ( 0 ) );

    _GraphLayers->populate();


}

void MainWindow::setActiveGraph ( Graph *g )
{
    if ( _tDocument->document() == 0 )
    {
        kDebug() << "ERROR : Theres no activeGraphDocument, but this graph should belong to one.";
        return;
    }
    if ( _tDocument->document()->indexOf ( g ) == -1 )
    {
        kDebug() << "ERROR: this graph does not belong to the active document";
        return;
    }
    foreach ( QAction *action, actionCollection()->actions() )
    {
        if ( AbstractAction *absAction = qobject_cast<AbstractAction*> ( action ) )
            absAction->setActiveGraph ( g );
    }

    _graphVisualEditor->setActiveGraph ( g );

    kDebug() << "New Active Graph Set: " << g->name();

}

Graph* MainWindow::graph() const
{
    return _tDocument->document()->activeGraph();
}

GraphScene* MainWindow::scene() const
{
    return _graphVisualEditor->scene();
}

void MainWindow::newGraph()
{
    if ( saveIfChanged() == KMessageBox::Cancel ) return;
    loadDocument();
}

void MainWindow::openGraph()
{
    if ( saveIfChanged() == KMessageBox::Cancel ) return;
    QString fileName = KFileDialog::getOpenFileName ( QString(), i18n ( "*.graph|Graph files\n*|All files" ), this, i18n ( "Graph Files" ) );
    if ( fileName == "" ) return;
    loadDocument ( fileName );
}

void MainWindow::loadDocument ( const QString& name )
{
    if ( !name.isEmpty() && !name.endsWith ( ".graph" ) )
    {
        KMessageBox::sorry ( this, i18n ( "This does not seem to be a graph file." ), i18n ( "Invalid file" ) );
        return;
    }

//   _graphVisualEditor->releaseGraphDocument();
    //delete _activeGraphDocument;

    //setActiveGraphDocument();
    GraphDocument *gd = new GraphDocument ( i18n ( "Untitled" ), 800,600 );
    if ( name.isEmpty() )
    {
        gd->addGraph ( i18n ( "Untitled0" ) );
    }
    else
    {
        gd->loadFromInternalFormat ( name );
    }

    _mutex.lock();
    _graphVisualEditor->releaseGraphDocument();
    kDebug() << "Starting Thread";
    _tDocument->setGraphDocument ( gd );
    //_tDocument->start();
    //_waitForDocument.wait(&_mutex);
    _mutex.unlock();
    setActiveGraphDocument ( _tDocument->document() );

    if ( !name.isEmpty() )
    {
        _graphVisualEditor->scene()->createItems();
    }
}

void MainWindow::saveGraph()
{
    if ( _tDocument->document()->documentPath().isEmpty() )
    {
        saveGraphAs();
    }
    else
    {
        _tDocument->document()->savedDocumentAt ( _tDocument->document()->documentPath() );
    }

}

void MainWindow::saveGraphAs()
{
    if ( _tDocument->document() == 0 )
    {
        kDebug() << "Graph Document is NULL";
        return;
    }

    _tDocument->document()->saveAsInternalFormat ( KFileDialog::getSaveFileName() );
}

void MainWindow::debug ( const QString& s )
{
    _txtDebug->insertPlainText ( s );
}

int MainWindow::saveIfChanged()
{
    if ( _tDocument->document()->isModified() )
    {
        int btnCode;
        btnCode = KMessageBox::warningYesNoCancel ( this, i18n ( "Do you want to save your unsaved document?" ) );
        if ( btnCode == KMessageBox::Yes )
        {
            saveGraph();
        }
        return btnCode;
    }
    return KMessageBox::No;
}

void MainWindow::importFile()
{
    if ( saveIfChanged() == KMessageBox::Cancel ) return;
    QString ext;

    foreach ( Rocs::FilePluginInterface *f, Rocs::PluginManager::New()->filePlugins() )
    {
        ext.append ( f->extensions().join ( "" ) );
    }

    ext.append ( i18n ( "*|All files" ) );



    KFileDialog dialog ( QString(),ext, this );
    dialog.setCaption ( i18n ( "Graph Files" ) );
    if ( !dialog.exec() )
    {
        return;
    }

    kDebug() << "Extensions:"<< ext;

    QString fileName = dialog.selectedFile();

    if ( fileName == "" ) return;

    int index = fileName.lastIndexOf ( '.' );
    Rocs::FilePluginInterface * f = 0;
    if ( index == -1 )
    {
        //Cant open file without extension.
        //kDebug () << "###########Filter: " << dialog.currentFilter();
        //f = Rocs::PluginManager::New()->filePluginsByExtension(dialog.currentFilter());
        return;
    }
    else
    {
        kDebug() << fileName.right ( fileName.count() - index );
        f = Rocs::PluginManager::New()->filePluginsByExtension ( fileName.right ( fileName.count() - index ) );
    }
    if ( !f )
    {
//       KMessageBox error();
//       error.exec();
        kDebug() <<  "Cannot handle extension "<<  fileName.right ( 3 );
        return;
    }

    GraphDocument * gd = f->readFile ( fileName );
    if ( !gd )
    {
        kDebug() << "Error loading file" << fileName << f->lastError();
        return;
    }



    _mutex.lock();
    _graphVisualEditor->releaseGraphDocument();
    kDebug() << "Starting Thread";
    _tDocument->setGraphDocument ( gd );
    //_tDocument->start();
    //_waitForDocument.wait(&_mutex);
    _mutex.unlock();

    setActiveGraphDocument ( _tDocument->document() );
    _graphVisualEditor->scene()->createItems();
    kDebug() << "Importing File.." << fileName;
}

void MainWindow::exportFile()
{
    QString ext;
    foreach ( Rocs::FilePluginInterface *f, Rocs::PluginManager::New()->filePlugins() )
    {
        ext.append ( f->extensions().join ( "" ) );
    }
    ext.append ( i18n ( "*|All files" ) );


    KFileDialog exportDialog ( QString(),ext,this );
    exportDialog.okButton()->setText ( i18n ( "Export" ) );
    exportDialog.okButton()->setToolTip ( i18n ( "Export graphs to file" ) );
    if ( exportDialog.exec() != KDialog::Accepted )
    {
        return;
    }

    kDebug() << "Exporting File..";
    if ( exportDialog.selectedFile() == "" )
    {
        return;
    }

    ext = exportDialog.currentFilter().remove ( '*' );
    kDebug () <<" Selected to export: " << ext;
    QString file = exportDialog.selectedFile();
    if ( !file.endsWith ( ext) )
    {
        file.append ( ext );
    }

    Rocs::FilePluginInterface * p = Rocs::PluginManager::New()->filePluginsByExtension ( ext );
    if ( !p )
    {
        kDebug() << "Cannot export file: " << file;
        return;
    }

    _mutex.lock();
    if ( !p->writeFile ( *_tDocument->document(), file ) )
    {
        kDebug() << "Error writing file: " << p->lastError();
    }
    else
    {
        kDebug() << "File Exported!" << file;
    }
    _mutex.unlock();
}
#ifdef USING_QTSCRIPT

void MainWindow::executeScript(const QString& text) {
    if (_txtDebug == 0)   return;
    if (scene() == 0)    return;
    
    _txtDebug->clear();
    _bottomTabs->setStopString();
    QString script = text.isEmpty()?_codeEditor->text():text;

    kDebug() << script;
    if ( !_tDocument->isRunning() ){
        kDebug() << "Starting Script";
        _tDocument->engine()->setScript(script, _tDocument->document());
        emit startEvaluation();
    }else{
        kDebug() << "Aborting Script";
        _bottomTabs->setPlayString();
        _tDocument->engine()->stop();
        //emit stopEvaluation();
    }

}

#endif

void MainWindow::runToolPlugin()
{
    QAction *action = qobject_cast<QAction *> ( sender() );
    if ( action )
    {
        Rocs::ToolsPluginInterface *plugin = qobject_cast<Rocs::ToolsPluginInterface *> ( action->parent() );
        if ( plugin )
        {
            emit runTool ( plugin );
        }
    }
}
