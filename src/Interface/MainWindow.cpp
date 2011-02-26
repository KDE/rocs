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
#include <KConfigDialog>

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
#include "Document.h"
#include "DataStructure.h"

#include "settings.h"

// Action Related Includes
#include "AbstractAction.h"
#include "AddData.h"
#include "AddConnection.h"
#include "MoveData.h"
#include "Select.h"
#include "DeleteAction.h"
#include "AlignAction.h"
#include <KNS3/DownloadDialog>

// backends
#include "QtScriptBackend.h"
#include <kstandarddirs.h>
#include <ktexteditor/view.h>
#include <ktexteditor/editor.h>
#include <ktexteditor/document.h>
#include <qscriptenginedebugger.h>
#include <QActionGroup>

#include "../Plugins/PluginManager.h"
#include <KPushButton>
#include <QMutexLocker>
#include <QScriptEngineDebugger>
#include "IncludeManagerSettings.h"
#include <IncludeManager.h>
#include "ImporterExporterManager.h"
#include <DataStructurePluginInterface.h>
#include <DataStructurePluginManager.h>
#include "DocumentManager.h"

MainWindow::MainWindow() :  KXmlGuiWindow()
{
    setObjectName ( "RocsMainWindow" );

    setupWidgets();
    setupActions();
    setupGUI();

    statusBar()->hide();

    setupToolsPluginsAction();
    setupDSPluginsAction();

    connect(DocumentManager::self(), SIGNAL(activateDocument()),   
            this, SLOT(setActiveDocument()), Qt::UniqueConnection );
    connect(DocumentManager::self(), SIGNAL(deactivateDocument(Document*)), 
            this, SLOT(releaseDocument(Document*)),Qt::UniqueConnection   );
    connect(DocumentManager::self(), SIGNAL(documentRemoved(Document*)),   
            this, SLOT(releaseDocument(Document*)),Qt::UniqueConnection   );

     /* just for testing prurposes,
     * this should not be hardcoded here.
     * use KWelcomeWidget instead.
     */
    DocumentManager::self()->loadDocument();
}

MainWindow::~MainWindow()
{
    Settings::setVSplitterSizeTop ( _vSplitter->sizes() [0] );
    Settings::setVSplitterSizeBottom ( _vSplitter->sizes() [1] );
    Settings::setHSplitterSizeLeft ( _hSplitter->sizes() [0] );
    Settings::setHSplitterSizeRight ( _hSplitter->sizes() [1] );

    Settings::self()->writeConfig();
}

void MainWindow::setupWidgets()
{
    _hSplitter = new QSplitter ( this );

    QWidget *rightPanel = setupRightPanel();
    QWidget *leftPanel  = setupLeftPanel();


    _hSplitter->addWidget ( leftPanel );
    _hSplitter->addWidget ( rightPanel );
    _hSplitter->setSizes ( QList<int>() << Settings::hSplitterSizeLeft() << Settings::hSplitterSizeRight() );

    setCentralWidget ( _hSplitter );
}

void MainWindow::downloadNewExamples(){
    KNS3::DownloadDialog dialog("rocs.knsrc", this);
    dialog.exec();
}

QWidget* MainWindow::setupRightPanel()
{
    _graphVisualEditor = new GraphVisualEditor ( this );
    _codeEditor = new CodeEditor ( this );
    _txtDebug = new KTextBrowser ( this );
    _txtOutput = new KTextBrowser( this );

    _bottomTabs = new TabWidget ( TabWidget::TabOnBottom, this );
    _bottomTabs->addWidget ( _codeEditor,  i18n ( "Editor" ), KIcon ( "accessories-text-editor" ) );
    _bottomTabs->addWidget ( _txtDebug, i18n ( "Debugger" ), KIcon ( "tools-report-bug" ) );
    _bottomTabs->addWidget ( _txtOutput, i18n("Output"), KIcon ("tools-document"));

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
    KStandardAction::preferences(this, SLOT(showSettings()), actionCollection());

    GraphScene *gc = _graphVisualEditor->scene();

    _moveDataAction = new MoveDataAction ( gc, this );

    KActionCollection *ac = actionCollection();
    QActionGroup *g = new QActionGroup ( this );

    g->addAction ( ac->addAction ( "move_node", _moveDataAction ) );
    g->addAction ( ac->addAction ( "add_node", new AddNodeAction ( gc, this ) ) );
    g->addAction ( ac->addAction ( "add_edge", new AddConnectionAction ( gc, this ) ) );
    g->addAction ( ac->addAction ( "select", new SelectAction ( gc, this ) ) );
    g->addAction ( ac->addAction ( "delete", new DeleteAction ( gc, this ) ) );
    actionCollection()->action ( "move_node" )->toggle();
    gc->setAction ( _moveDataAction );

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

    action = new KAction (i18n("Download Examples"), this);
    actionCollection()->addAction ( "download", action );
    connect(action, SIGNAL(triggered(bool)), this, SLOT(downloadNewExamples()));

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


    if (  PluginManager::instance()->filePlugins().count() > 0 )
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
        kDebug() << "Not Creating Actions (import export).." <<  PluginManager::instance()->filePlugins().count();
    }
    action = new KAction ( KIcon ( "document-save-as" ), i18n ( "Possible Includes" ), this );
    action->setShortcut ( Qt::CTRL + Qt::Key_P );
    action->setShortcutContext ( Qt::WidgetShortcut );
    actionCollection()->addAction ( "possible_includes", action );
    connect ( action, SIGNAL ( triggered ( bool ) ), this, SLOT ( showPossibleIncludes()) );

    KStandardAction::quit ( kapp, SLOT ( quit() ),  actionCollection() );
}

void MainWindow::showSettings()
{
     KConfigDialog dialog(this,  "settings", Settings::self());

     IncludeManagerSettings * set = new IncludeManagerSettings(&dialog);

     dialog.addPage(set,"Include Manager",QString(),"Include Manager",true);
     connect(set, SIGNAL(changed(bool)), &dialog, SLOT(enableButtonApply(bool)));
//      connect(set, SIGNAL(changed(bool)), dialog, SLOT(enableButtonApply(bool)));

     connect(&dialog, SIGNAL(applyClicked()),   set, SLOT(saveSettings()));
     connect(&dialog, SIGNAL(okClicked()),      set, SLOT(saveSettings()));
     connect(&dialog, SIGNAL(defaultClicked()), set, SLOT(readConfig()));

     dialog.exec();
}

void MainWindow::setupToolsPluginsAction()
{
    QList <QAction*> pluginList;
    QAction* action = 0;
    unplugActionList ( "tools_plugins" );
    QList <  ToolsPluginInterface*> avaliablePlugins =  PluginManager::instance()->toolPlugins();
    int count = 0;
    foreach (  ToolsPluginInterface* p, avaliablePlugins ){
        action = new KAction ( p->displayName(), this );
        action->setData(count++);
        connect ( action, SIGNAL ( triggered ( bool ) ),this, SLOT ( runToolPlugin() ) );
        pluginList.append ( action );
    }
    plugActionList ( "tools_plugins", pluginList );
}

void MainWindow::setupDSPluginsAction()
{
    QList <QAction*> pluginList;
    QAction* action = 0;
    unplugActionList ( "DS_plugins" );
    QList < DataStructurePluginInterface*> avaliablePlugins = DataStructurePluginManager::self()->pluginsList();
    QActionGroup * group = new QActionGroup(this);
    int count = 0;
    foreach ( DataStructurePluginInterface* p, avaliablePlugins ) {
        action = new KAction ( p->name(), this );
        action->setData(count++);
        action->setCheckable(true);
        if (p->name() == DataStructurePluginManager::self()->actualPlugin()){
          action->setChecked(true);
        }
        action->setActionGroup(group);
        connect ( action, SIGNAL ( triggered ( bool ) ),DataStructurePluginManager::self(), SLOT ( changeActiveDataStructure()) );
        pluginList.append ( action );
    }
    plugActionList ( "DS_plugins", pluginList );
}

void MainWindow::setupDocumentsList(){
    QList <QAction*> pluginList;
    QAction* action = 0;
    unplugActionList ( "Doc_List" );
    QActionGroup * group = new QActionGroup(this);
    int count = 0;
    foreach(Document * doc, DocumentManager::self()->documentList()){
        action = new KAction ( doc->name(), this );
        action->setData(count++);
        action->setCheckable(true);
        if (doc == DocumentManager::self()->activeDocument()){
          action->setChecked(true);
        }
        action->setActionGroup(group);
        connect ( action, SIGNAL ( triggered ( bool ) ), DocumentManager::self(), SLOT ( changeDocument()) );
        pluginList.append ( action );
    }
    plugActionList ( "Doc_List", pluginList );
}

void MainWindow::setActiveDocument ( )
{
    kDebug() << "Setting the document in the main widnow";
    Document *activeDocument = DocumentManager::self()->activeDocument();
    
    _graphVisualEditor->setActiveDocument();
    _GraphLayers->setActiveDocument();
    
    connect ( this, SIGNAL(runTool(  ToolsPluginInterface*,Document*)),
                activeDocument->engineBackend(), SLOT (runTool( ToolsPluginInterface*,Document*)));
    
//     connect(this, SIGNAL(startEvaluation()),
//             engine(), SLOT(start()));

//     connect( engine(), SIGNAL(sendDebug(QString)), this, SLOT(debugString(QString)));
//     connect( engine(), SIGNAL(sendOutput(QString)), this, SLOT(outputString(QString)));
//     connect( engine(), SIGNAL(finished()),_bottomTabs, SLOT(setPlayString()));

    
}

void MainWindow::releaseDocument ( Document* d ){
    if (d == 0){
      return;
    }
    d->disconnect(this);
    disconnect(d);

    _graphVisualEditor->releaseDocument();
    _GraphLayers->disconnect(d);
    d->engineBackend()->disconnect(this);
    d->engineBackend()->disconnect(_bottomTabs);
}

GraphScene* MainWindow::scene() const
{
    return _graphVisualEditor->scene();
}

void MainWindow::newGraph()
{
  if (DocumentManager::self()->activeDocument() != 0){
    if ( saveIfChanged() == KMessageBox::Cancel ) return;
  }
  loadDocument();
}

void MainWindow::openGraph()
{
    if ( saveIfChanged() == KMessageBox::Cancel ) return;
    QString fileName = KFileDialog::getOpenFileName ( QString(), i18n ( "*.graph|Graph files\n*|All files" ), this, i18n ( "Graph Files" ) );
    if ( fileName == "" ) return;
//      ImporterExporterManager imp(this);
//     imp.openDocument();
    loadDocument ( fileName );
}

void MainWindow::loadDocument ( const QString& name )
{
    if ( !name.isEmpty() && !name.endsWith ( ".graph" ) )
    {
        KMessageBox::sorry ( this, i18n ( "This does not seem to be a graph file." ), i18n ( "Invalid file" ) );
        return;
    }

//      _graphVisualEditor->releaseDocument();

    emit startDocument( name );

//     setActiveDocument ( _tDocument->document() );


}

void MainWindow::saveGraph(){
    Document *d = DocumentManager::self()->activeDocument();
    if ( d->documentPath().isEmpty() ){
        saveGraphAs();
    }else{
        d->savedDocumentAt ( d->documentPath() );
    }
}

void MainWindow::saveGraphAs(){
    Document *d = DocumentManager::self()->activeDocument();
    if ( d == 0 ){
        kDebug() << "Graph Document is NULL";
        return;
    }

    d->saveAsInternalFormat ( KFileDialog::getSaveFileName() );
}

// void MainWindow::debug ( const QString& s ){
//     _txtDebug->insertPlainText ( s );
// }

int MainWindow::saveIfChanged(){
    if ( DocumentManager::self()->activeDocument()->isModified() ){
        int btnCode;
        btnCode = KMessageBox::warningYesNoCancel ( this, i18n ( "Do you want to save your unsaved document?" ) );
        if ( btnCode == KMessageBox::Yes ){
            saveGraph();
        }
        return btnCode;
    }
    return KMessageBox::No;
}

void MainWindow::importFile(){
    ImporterExporterManager importer(this);
    Document * gd = importer.importFile();
    if (gd == 0){
        return;
    }

    DocumentManager::self()->addDocument(gd);

    _graphVisualEditor->scene()->createItems();

    if (importer.hasDialog()){
      importer.dialogExec();
    }

    if (!importer.scriptToRun().isEmpty()){
      executeScript(importer.scriptToRun());
    }
}

void MainWindow::exportFile()
{
    ImporterExporterManager exp(this);

    exp.exportFile(DocumentManager::self()->activeDocument());
}

void MainWindow::showPossibleIncludes()
{
   QDialog dialog(this);
   dialog.exec();
}

void MainWindow::runToolPlugin()
{
    kDebug() << "seeking for a plugin";
    QAction *action = qobject_cast<QAction *> ( sender() );

    if (! action ){
      return;
    }

    if (ToolsPluginInterface *plugin =  PluginManager::instance()->toolPlugins().value(action->data().toInt()) ){
      emit runTool ( plugin, DocumentManager::self()->activeDocument() );
    }
}

void MainWindow::dsChanged(){
    kDebug() << "Data structure was changed, need to reload graphic part.";

//    setActiveDocument();
//     QAction *action = qobject_cast<QAction *> ( sender() );
//
//     if (! action ){
//       return;
//     }
//     DataStructurePluginInterface *plugin = DataStructurePluginManager::instance()->pluginsList().at(action->data().toInt() );
//
//     kDebug() << "Changed " << plugin->name();
}

#ifdef USING_QTSCRIPT

void MainWindow::executeScript(const QString& text) {
    kDebug() << "Going to execut the script";
    if (_txtDebug == 0)   return;
    if ( scene() == 0)    return;

    _txtDebug->clear();

    QString script = text.isEmpty() ? _codeEditor->text() : text;

    IncludeManager inc;

    script = inc.include(script, _codeEditor->document()->url().path(), _codeEditor->document()->documentName());

    //if ( !engine()->isRunning() ){
	//_bottomTabs->setStopString();
    //    engine()->setScript(script, DocumentManager::self()->activeDocument());
    //    emit startEvaluation();
    //}else{
    //    _bottomTabs->setPlayString();
    //    engine()->stop();
    //}
}

#endif

void MainWindow::outputString ( const QString& s ) { _txtOutput->append ( s ); }
void MainWindow::debugString ( const QString& s )  { _txtDebug->append ( s );  }
