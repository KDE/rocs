/*
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "MainWindow.h"

//Qt related includes
#include <QtGui/QCloseEvent>
#include <QtGui/QComboBox>
#include <QtGui/QGraphicsView>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QSplitter>
#include <QtGui/QStackedWidget>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QToolButton>

// KDE Related Includes
#include <KActionCollection>
#include <KApplication>
#include <KDebug>
#include <KIcon>
#include <KPushButton>
#include <KStatusBar>
#include <KTar>
#include <KTextBrowser>
#include <KMessageBox>
#include <klocalizedstring.h>
#include <KConfigDialog>
#include <kfiledialog.h>
#include <KMenu>

// UI RELATED INCLUDES
#include "GraphLayers.h"
#include "GraphVisualEditor.h"
#include "GraphScene.h"
#include "CodeEditor.h"
#include "TabWidget.h"
#include "GraphicsLayout.h"

// MODEL Related Includes
#include "model_GraphDocument.h"

// Graph Related Includes
#include "Document.h"
#include "DataStructure.h"

#include "settings.h"

// Action Related Includes
#include "AbstractAction.h"
#include "AddDataHandAction.h"
#include "AddConnectionHandAction.h"
#include "SelectMoveHandAction.h"
#include "DeleteHandAction.h"
#include "AlignAction.h"
#include "ZoomAction.h"
#include "DeleteAction.h"

#include <KNS3/DownloadDialog>
#include <knewstuff3/uploaddialog.h>

// backends
#include "QtScriptBackend.h"
#include <kstandarddirs.h>
#include <ktexteditor/view.h>
#include <ktexteditor/editor.h>
#include <ktexteditor/document.h>
#include <QActionGroup>

#include "../Plugins/PluginManager.h"
#include <QMutexLocker>
#include <QScriptEngineDebugger>
#include "IncludeManagerSettings.h"
#include "ConfigureDefaultProperties.h"
#include <IncludeManager.h>
#include "ImporterExporterManager.h"
#include <DataStructurePluginInterface.h>
#include <DataStructurePluginManager.h>
#include "DocumentManager.h"



#include "PossibleIncludes.h"

MainWindow::MainWindow() :  KXmlGuiWindow(), _scriptDbg(0)
{
    setObjectName ( "RocsMainWindow" );

    setupWidgets();
    setupActions();
    setupGUI();

    //statusBar()->hide();

    setupToolsPluginsAction();
    setupDSPluginsAction();

    DocumentManager *dm = DocumentManager::self();
    connect(dm, SIGNAL(activateDocument()),
            this, SLOT(setActiveDocument()));
    connect(dm, SIGNAL(deactivateDocument(Document*)),
            this, SLOT(releaseDocument(Document*)));
    connect(dm, SIGNAL(documentRemoved(Document*)),
            this, SLOT(releaseDocument(Document*)));

     /* just for testing prurposes,
     * this should not be hardcoded here.
     * use KWelcomeWidget instead.
     */
    DocumentManager::self()->loadDocument();

    GraphicsLayout::self()->setViewStyleDataNode(Settings::dataNodeDisplay());
    GraphicsLayout::self()->setViewStyleDataEdge(Settings::dataEdgeDisplay());
}

MainWindow::~MainWindow()
{
    Settings::setVSplitterSizeTop ( _vSplitter->sizes() [0] );
    Settings::setVSplitterSizeBottom ( _vSplitter->sizes() [1] );
    Settings::setHSplitterSizeLeft ( _hSplitter->sizes() [0] );
    Settings::setHSplitterSizeRight ( _hSplitter->sizes() [1] );
    Settings::setHScriptSplitterSizeLeft ( _hScriptSplitter->sizes() [0] );
    Settings::setHScriptSplitterSizeRight ( _hScriptSplitter->sizes() [1] );

    Settings::self()->writeConfig();
}

void MainWindow::closeEvent(QCloseEvent *event)
 {
    switch (saveIfChanged()) {
      case KMessageBox::Cancel :
          event->ignore();
          return;
      default:
          event->accept();
          return;
    }
 }

void MainWindow::setupWidgets()
{
    // splits the main window horizontal
    _vSplitter = new QSplitter ( this );
    _vSplitter->setOrientation ( Qt::Vertical );

    // setup upper half
    QWidget *leftPanel  = setupWhiteboardPanel();         // graph properties
    _graphVisualEditor = GraphVisualEditor::self(); // graph editor whiteboard

    _hSplitter = new QSplitter ( this );
    _hSplitter->setOrientation( Qt::Horizontal );
    _hSplitter->addWidget (leftPanel);
    _hSplitter->addWidget (_graphVisualEditor);

    // setup lower half
    QWidget *scriptPanel = setupScriptPanel();

    _vSplitter->addWidget ( _hSplitter );
    _vSplitter->addWidget ( scriptPanel );

    _hScriptSplitter->setSizes ( QList<int>() << Settings::hScriptSplitterSizeLeft() << Settings::hScriptSplitterSizeRight() << 80 );
    _vSplitter->setSizes ( QList<int>() << Settings::vSplitterSizeTop() << Settings::vSplitterSizeBottom() );
    _hSplitter->setSizes ( QList<int>() << Settings::hSplitterSizeLeft() << Settings::hSplitterSizeRight() );

    setCentralWidget ( _vSplitter );
}

void MainWindow::downloadNewExamples(){
    KNS3::DownloadDialog dialog("rocs.knsrc", this);
    dialog.exec();
}

void MainWindow::uploadScript()
{

    KNS3::UploadDialog dialog(this);

//First select the opened doc.
    KUrl str = _codeEditor->document()->url();
    if (str.isEmpty()) {
        //... then try to open
        str = KFileDialog::getOpenFileName ( QString(), i18n ( "*.js|Script files" ),
                                             this, i18n ( "Rocs Script Files" ) );
        if (str.isEmpty())
            return;
    }
//Compress the file to a temp file (How it can be made in KDE way ? )
    QString local = QDir::temp().absoluteFilePath(str.fileName());
    local.chop(3);
    local.append(".tar.gz");

//create compressed file and set to dialog.
    KTar tar = KTar(local);
    tar.open(QIODevice::WriteOnly);
    tar.addLocalFile(str.toLocalFile(), str.fileName());
    tar.close();
    dialog.setUploadFile(local);

    dialog.setUploadName(_codeEditor->document()->documentName());
    dialog.setDescription(i18n("Added your description here."));

    dialog.exec();

//Remove compressed file..
    QDir::temp().remove(local);
}


QWidget* MainWindow::setupScriptPanel()
{
    _hScriptSplitter = new QSplitter( this );
    _hScriptSplitter->setOrientation( Qt::Horizontal );

    _codeEditor = new CodeEditor ( this );
    _txtDebug = new KTextBrowser ( this );
    _txtOutput = new KTextBrowser( this );

    QStackedWidget *stackedListing = new QStackedWidget;
    stackedListing->addWidget(_txtOutput);
    stackedListing->addWidget(_txtDebug);

    _selectListing = new QComboBox;
    _selectListing->addItem( KIcon ( "accessories-text-editor" ), i18n("Program Messages"));
    _selectListing->addItem( KIcon ( "tools-report-bug" ), i18n("Debug Messages"));

    QWidget *header = new QWidget( this );
    header->setLayout( new QHBoxLayout );
    header->layout()->addWidget(new QLabel(i18n("Select output:")));
    header->layout()->addWidget(_selectListing);

    QWidget *listingWidget = new QWidget(this);
    listingWidget->setLayout( new QVBoxLayout );
    listingWidget->layout()->addWidget( header );
    listingWidget->layout()->addWidget( stackedListing );

    QToolBar *executeCommands = new QToolBar;
    executeCommands->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    executeCommands->setOrientation(Qt::Vertical);
    _runScript = new KAction( KIcon( "system-run" ), i18nc( "Script Execution", "Run" ), this );
    _stepRunScript = new KAction( KIcon( "go-next" ), i18nc( "Script Execution", "One Step" ), this );
    _debugScript = new KAction ( KIcon ("debug-run"), i18n ( "Debug run" ), this );
    _interruptScript = new KAction ( KIcon ("debug-run-cursor"), i18n ( "Interrupt at first line" ), this );
    _stopScript = new KAction( KIcon( "process-stop" ), i18nc( "Script Execution", "Stop" ), this );
    _stopScript->setEnabled(false);
    executeCommands->addAction( _runScript );
    executeCommands->addAction( _stepRunScript );
    QToolButton *dbgBtn = new QToolButton(executeCommands);
    KMenu * menu = new KMenu(dbgBtn);
    menu->addAction(_debugScript);
    menu->addAction(_interruptScript);
    menu->setActiveAction(_debugScript);
    menu->setDefaultAction(_debugScript);
    menu->setIcon(_debugScript->icon());
    dbgBtn->setMenu(menu);
    dbgBtn->setDefaultAction(_debugScript);
    executeCommands->addWidget(dbgBtn);
    executeCommands->addAction( _stopScript );

    connect(_runScript, SIGNAL(triggered()), this, SLOT(executeScriptFull()));
    connect(_stepRunScript, SIGNAL(triggered()), this, SLOT(executeScriptOneStep()));
    connect(_debugScript, SIGNAL(triggered()), this, SLOT(debugScript()));
    connect(_interruptScript, SIGNAL(triggered()), this, SLOT(debugScript()));
    connect(_stopScript, SIGNAL(triggered()), this, SLOT(stopScript()));
    connect(_selectListing, SIGNAL(currentIndexChanged(int)), stackedListing, SLOT(setCurrentIndex(int)));

    _hScriptSplitter->addWidget( _codeEditor );
    _hScriptSplitter->addWidget( listingWidget );

    QWidget *scriptInterface = new QWidget( this );
    scriptInterface->setLayout( new QHBoxLayout );
    scriptInterface->layout()->addWidget( _hScriptSplitter );
    scriptInterface->layout()->addWidget( executeCommands );

    return scriptInterface;
}

QWidget* MainWindow::setupWhiteboardPanel()
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

    _selectMoveAction = new SelectMoveHandAction (gc, this);
    AddDataHandAction* addDataAction = new AddDataHandAction(gc, this );
    AddConnectionHandAction* addConnectionAction = new AddConnectionHandAction(gc, this);
    DeleteHandAction* deleteAction = new DeleteHandAction(gc, this);
    ZoomAction* zoomAction = new ZoomAction(gc, this);

    connect(_selectMoveAction, SIGNAL(triggered()), _selectMoveAction, SLOT( sendExecuteBit() ));
    connect(addDataAction, SIGNAL(triggered()), addDataAction, SLOT( sendExecuteBit() ));
    connect(addConnectionAction, SIGNAL(triggered()), addConnectionAction, SLOT( sendExecuteBit() ));
    connect(deleteAction, SIGNAL(triggered()), deleteAction, SLOT( sendExecuteBit() ));
    connect(zoomAction, SIGNAL(triggered()), zoomAction, SLOT( sendExecuteBit() ));

    _paletteActions = actionCollection();
    QActionGroup *g = new QActionGroup ( this );

    g->addAction ( _paletteActions->addAction ( "selectmove", _selectMoveAction ) );
    g->addAction ( _paletteActions->addAction ( "add_node", addDataAction ) );
    g->addAction ( _paletteActions->addAction ( "add_edge", addConnectionAction ) );
    g->addAction ( _paletteActions->addAction ( "delete", deleteAction ) );
    g->addAction ( _paletteActions->addAction ( "zoom", zoomAction ) );
    actionCollection()->action ( "selectmove" )->toggle();
    gc->setAction ( _selectMoveAction );

    _paletteActions->addAction ( "align-hbottom",new AlignAction ( i18nc( "Alignment", "Base" ),  AlignAction::Bottom, gc ) );
    _paletteActions->addAction ( "align-hcenter",new AlignAction ( i18nc( "Alignment", "Center" ),AlignAction::HCenter,gc ) );
    _paletteActions->addAction ( "align-htop",   new AlignAction ( i18nc( "Alignment", "Top" ),   AlignAction::Top,    gc ) );
    _paletteActions->addAction ( "align-vleft",  new AlignAction ( i18nc( "Alignment", "Left" ),  AlignAction::Left,   gc ) );
    _paletteActions->addAction ( "align-vcenter",new AlignAction ( i18nc( "Alignment", "Center" ),AlignAction::VCenter,gc ) );
    _paletteActions->addAction ( "align-vright", new AlignAction ( i18nc( "Alignment", "Right" ), AlignAction::Right,  gc ) );
    _paletteActions->addAction ( "align-circle", new AlignAction ( i18nc( "Alignment", "Circle" ),  AlignAction::Circle, gc ) );
    _paletteActions->addAction ( "align-tree", new AlignAction ( i18nc( "Alignment", "Minimize Crossing Edges" ), AlignAction::MinCutTree, gc ) );

    // Menu actions
    createAction("document-new",     i18n("New Graph"),         "new-graph",         SLOT(newGraph()),    this);
    createAction("document-open",    i18n("Open Graph"),        "open-graph",        SLOT(openGraph()),   this);
    createAction("document-save",    i18n("Save Graph"),        "save-graph",        SLOT(saveGraph()),   this);
    createAction("document-save-as", i18n("Save Graph as"),     "save-graph-as",     SLOT(saveGraphAs()), this);
    createAction("",                 i18n("Download Examples"), "download",          SLOT(downloadNewExamples()),  this);
    createAction("",                 i18n("Upload script"),     "upload",            SLOT(uploadScript()),  this);
    createAction("document-save",    i18n("Save All"),          "save-all",        Qt::Key_S, SLOT(saveAll()),   this);

    createAction("document-save-as", i18n("Possible Includes"), "possible_includes", SLOT(showPossibleIncludes()), this);

    createAction("document-new",     i18n("New Script"),        "new-script",        SLOT(newScript()),    _codeEditor);
    createAction("document-open",    i18n("Open Script"),       "open-script",       SLOT(openScript()),   _codeEditor);
    createAction("document-save",    i18n("Save Script"),       "save-script",       SLOT(saveActiveScript()),   _codeEditor);
    createAction("document-save-as", i18n("Save Script as"),    "save-script-as",    SLOT(saveActiveScriptAs()), _codeEditor);

    // eventually create hooks for file plugins
    PluginManager::instance()->loadFilePlugins();
    if (PluginManager::instance()->filePlugins().count()>0) {
        createAction("document-open", i18n("Import Graph"), "import-graph", SLOT(importFile()), this);
        createAction("document-save", i18n("Export Graph"), "export-graph", SLOT(exportFile()), this);
    }

    // EDIT actions
    actionCollection()->addAction("delete-selected", new DeleteAction( i18n("Delete"), _graphVisualEditor->scene(), 0) );

    KStandardAction::quit ( kapp, SLOT ( quit() ),  actionCollection() );
}

void MainWindow::createAction(const QByteArray& iconName, const QString& actionTitle, const QString& actionName,
                              const QKeySequence& shortcut, const char* slot, QObject *parent)
{
    KAction* action = new KAction ( KIcon ( iconName ), actionTitle, parent );
    action->setShortcut ( shortcut);
    action->setShortcutContext ( Qt::WidgetShortcut );
    actionCollection()->addAction ( actionName, action );
    connect(action, SIGNAL(triggered(bool)), parent, slot );
}

void MainWindow::createAction(const QByteArray& iconName, const QString& actionTitle, const QString& actionName,
                              const char* slot, QObject *parent)
{
    KAction* action = new KAction ( KIcon ( iconName ), actionTitle, parent );
    action->setShortcutContext ( Qt::WidgetShortcut );
    actionCollection()->addAction ( actionName, action );
    connect(action, SIGNAL(triggered(bool)), parent, slot);
}


void MainWindow::showSettings()
{
     KConfigDialog dialog(this,  "settings", Settings::self());

     IncludeManagerSettings * set = new IncludeManagerSettings(&dialog);
     ConfigureDefaultProperties * defaultProperties = new ConfigureDefaultProperties( &dialog );

     dialog.addPage(set,i18n("Include Manager"),QString(),i18n("Include Manager"),true);
     dialog.addPage(defaultProperties,i18n("Default Settings"),QString(),i18n("Default Settings"),true);


     connect(set,               SIGNAL(changed(bool)), &dialog, SLOT(enableButtonApply(bool)));
     connect(defaultProperties, SIGNAL(changed(bool)), &dialog, SLOT(enableButtonApply(bool)));

     connect(&dialog, SIGNAL(applyClicked()),   set, SLOT(saveSettings()));
     connect(&dialog, SIGNAL(okClicked()),      set, SLOT(saveSettings()));
     connect(&dialog, SIGNAL(defaultClicked()), set, SLOT(readConfig()));

     connect(&dialog, SIGNAL(applyClicked()),   defaultProperties, SLOT(saveConfig()));
     connect(&dialog, SIGNAL(okClicked()),      defaultProperties, SLOT(saveConfig()));
     connect(&dialog, SIGNAL(defaultClicked()), defaultProperties, SLOT(readConfig()));


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
        connect(action, SIGNAL(triggered(bool)), this, SLOT(runToolPlugin()));
        pluginList.append( action );
    }
    plugActionList( "tools_plugins", pluginList );
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

        if (p->name() == DataStructurePluginManager::self()->pluginName()){
          action->setChecked(true);
        }

        action->setActionGroup(group);

        connect ( action, SIGNAL (triggered(bool)),
                  DataStructurePluginManager::self(), SLOT(setDataStructurePlugin()));

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
        connect(action, SIGNAL(triggered(bool)), DocumentManager::self(), SLOT(changeDocument()));
        pluginList.append ( action );
    }

    plugActionList ( "Doc_List", pluginList );
}

void MainWindow::setActiveDocument ( )
{
    kDebug() << "Setting the document in the main widnow";
    Document *activeDocument = DocumentManager::self()->activeDocument();
    QtScriptBackend *engine = activeDocument->engineBackend();

    _graphVisualEditor->setActiveDocument();
    _GraphLayers->setActiveDocument();

    connect ( this, SIGNAL(runTool(ToolsPluginInterface*,Document*)),
                activeDocument->engineBackend(), SLOT(runTool(ToolsPluginInterface*,Document*)));

//     connect(this, SIGNAL(startEvaluation()),   engine,  SLOT(start()));
    connect( engine, SIGNAL(sendDebug(QString)), this,  SLOT(debugString(QString)));
    connect( engine, SIGNAL(scriptError()), this, SLOT(showDebugOutput()));
    connect( engine, SIGNAL(sendOutput(QString)), this, SLOT(outputString(QString)));
    connect( engine, SIGNAL(finished()), this, SLOT(disableStopAction()));

    activeDocument->setModified(false);
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
    if ( !name.isEmpty() && !name.endsWith ( ".graph" ) ){
        KMessageBox::sorry ( this, i18n ( "This does not seem to be a graph file." ), i18n ( "Invalid file" ) );
        return;
    }

    DocumentManager::self()->loadDocument(name);
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

void MainWindow::saveAll() {
    saveGraph();
    _codeEditor->saveAllScripts();
}


int MainWindow::saveIfChanged(){
    if ( DocumentManager::self()->activeDocument()->isModified() && !_codeEditor->isModified() ){
        const int btnCode = KMessageBox::warningYesNoCancel ( this, i18n (
            "Changes on your graph document are unsaved. Do you want to save your changes?" ) );
        if ( btnCode == KMessageBox::Yes ){
            saveGraph();
        }
        return btnCode;
    }
    if ( !DocumentManager::self()->activeDocument()->isModified() && _codeEditor->isModified() ){
        const int btnCode = KMessageBox::warningYesNoCancel ( this, i18n (
            "Changes on your script files are unsaved. Do you want to save all unsaved scripts?" ) );
        if ( btnCode == KMessageBox::Yes ){
            _codeEditor->saveAllScripts();
        }
        return btnCode;
    }
    if ( DocumentManager::self()->activeDocument()->isModified() && _codeEditor->isModified() ){
        const int btnCode = KMessageBox::warningYesNoCancel ( this, i18n (
            "Changes on your script files and on your graph document are unsaved. Do you want to save your graph document and all unsaved scripts?" ) );
        if ( btnCode == KMessageBox::Yes ){
            _codeEditor->saveAllScripts();
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
        executeScriptFull(importer.scriptToRun());
    }
}

void MainWindow::exportFile()
{
    ImporterExporterManager exp(this);

    exp.exportFile(DocumentManager::self()->activeDocument());
}

void MainWindow::showPossibleIncludes()
{
   PossibleIncludes dialog(this);

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
//    kDebug() << "Data structure was changed, need to reload graphic part.";

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

void MainWindow::executeScriptFull(const QString& text) {
    executeScript(MainWindow::Execute, text);
}

void MainWindow::executeScript(const MainWindow::ScriptMode mode, const QString& text)
{
        kDebug() << "Going to execute the script";
    if (_txtDebug == 0)   return;
    if ( scene() == 0)    return;


    QString script = text.isEmpty() ? _codeEditor->text() : text;
    QString scriptPath = _codeEditor->document()->url().path();
    QtScriptBackend *engine = DocumentManager::self()->activeDocument()->engineBackend();
    if (engine->isRunning() ) {
        engine->stop();
    }

    _txtDebug->clear();
    if (_scriptDbg){
        _scriptDbg->detach();
        _scriptDbg->deleteLater();
        _scriptDbg = 0;
    }
    if ( mode != Execute){
        _scriptDbg = new QScriptEngineDebugger(this);
        _scriptDbg->setAutoShowStandardWindow(true);
        _scriptDbg->attachTo(engine->engine());
        if (mode == MainWindow::DebugMode)
            _scriptDbg->action(QScriptEngineDebugger::InterruptAction)->trigger();
    }
    engine->includeManager().initialize(Settings::includePath());
    script = engine->includeManager().include(script,
                         scriptPath.isEmpty()? scriptPath : scriptPath.section('/', 0, -2),
                         _codeEditor->document()->documentName());

    enableStopAction();

    engine->setScript(script, DocumentManager::self()->activeDocument());
    engine->execute();
}

void MainWindow::executeScriptOneStep(const QString& text) {
    qDebug() << "execution next step";
    if (_txtDebug == 0)   return;
    if ( scene() == 0)    return;

    _txtDebug->clear();

    QtScriptBackend *engine = DocumentManager::self()->activeDocument()->engineBackend();

    //TODO disable start action
    enableStopAction();
    if (!engine->isRunning()){
        QString script = text.isEmpty() ? _codeEditor->text() : text;
        QString scriptPath = _codeEditor->document()->url().path();
        IncludeManager inc;

        script = inc.include(script,
                            scriptPath.isEmpty()? scriptPath : scriptPath.section('/', 0, -2),
                            _codeEditor->document()->documentName());

        engine->setScript(script, DocumentManager::self()->activeDocument());
        engine->executeStep();
        return;
    }
    engine->continueExecutionStep();
}



void MainWindow::stopScript() {
    kDebug() << "Going to stop the script";
    if (_txtDebug == 0)   return;
    if ( scene() == 0)    return;

    QtScriptBackend *engine = DocumentManager::self()->activeDocument()->engineBackend();

    disableStopAction();

    engine->stop();
}

void MainWindow::debugScript()
{
    QAction *action = qobject_cast<QAction *> ( sender() );
    if (action == _interruptScript)
        executeScript(DebugMode);
    else
        executeScript(DebugOnlyInCaseOfError);
}


#endif


void MainWindow::enableStopAction(){
    _stopScript->setEnabled(true);
}

void MainWindow::disableStopAction(){
    _stopScript->setEnabled(false);
}

void MainWindow::showDebugOutput()
{
    _selectListing->setCurrentIndex(1);
}

void MainWindow::outputString ( const QString& s ) { _txtOutput->append ( s ); }
void MainWindow::debugString ( const QString& s )  { _txtDebug->append ( s );  }

