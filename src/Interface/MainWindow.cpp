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
#include <KTar>
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
#include <knewstuff3/uploaddialog.h>

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
#include "ConfigureDefaultProperties.h"
#include <IncludeManager.h>
#include "ImporterExporterManager.h"
#include <DataStructurePluginInterface.h>
#include <DataStructurePluginManager.h>
#include "DocumentManager.h"
#include <QCloseEvent>
#include <KMessageBox>
#include "zoom.h"
#include "../GraphicsItem/GraphicsLayout.h"
#include "PossibleIncludes.h"

MainWindow::MainWindow() :  KXmlGuiWindow()
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
    dialog.setDescription("Added your description here.");

    dialog.exec();

//Remove compressed file..
    QDir::temp().remove(local);
}


QWidget* MainWindow::setupRightPanel()
{
    _graphVisualEditor = GraphVisualEditor::self();

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

    // graph editor whiteboard
    _vSplitter->addWidget (_graphVisualEditor);

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
    g->addAction ( ac->addAction ( "zoom", new ZoomAction ( gc, this ) ) );
    actionCollection()->action ( "move_node" )->toggle();
    gc->setAction ( _moveDataAction );

    ac->addAction ( "align-hbottom",new AlignAction ( i18n ( "Align on the base" ),  AlignAction::Bottom, _graphVisualEditor ) );
    ac->addAction ( "align-hcenter",new AlignAction ( i18n ( "Align on the center" ),AlignAction::HCenter,_graphVisualEditor ) );
    ac->addAction ( "align-htop",   new AlignAction ( i18n ( "Align on the top" ),   AlignAction::Top,    _graphVisualEditor ) );
    ac->addAction ( "align-vleft",  new AlignAction ( i18n ( "Align on the left" ),  AlignAction::Left,   _graphVisualEditor ) );
    ac->addAction ( "align-vcenter",new AlignAction ( i18n ( "Align on the center" ),AlignAction::VCenter,_graphVisualEditor ) );
    ac->addAction ( "align-vright", new AlignAction ( i18n ( "Align on the right" ), AlignAction::Right,  _graphVisualEditor ) );

    createAction("document-new",     i18n("New Graph"),         "new-graph",         Qt::Key_N, SLOT(newGraph()),    this);
    createAction("document-open",    i18n("Open Graph"),        "open-graph",        Qt::Key_0, SLOT(openGraph()),   this);
    createAction("document-save",    i18n("Save Graph"),        "save-graph",        Qt::Key_S, SLOT(saveGraph()),   this);
    createAction("document-save-as", i18n("Save Graph as"),     "save-graph-as",     Qt::Key_W, SLOT(saveGraphAs()), this);
    createAction("",                 i18n("Download Examples"), "download", Qt::Key_D, SLOT(downloadNewExamples()),  this);
    createAction("",                 i18n("Upload script"),     "upload",   Qt::Key_U, SLOT(uploadScript()),  this);

    createAction("document-save-as", i18n("Possible Includes"), "possible_includes", Qt::Key_P, SLOT(showPossibleIncludes()), this);

    createAction("document-new",     i18n("New Script"),        "new-script",        Qt::Key_N, SLOT(newScript()),    _codeEditor);
    createAction("document-open",    i18n("Open Script"),       "open-script",       Qt::Key_O, SLOT(openScript()),   _codeEditor);
    createAction("document-save",    i18n("Save Script"),       "save-script",       Qt::Key_S, SLOT(saveScript()),   _codeEditor);
    createAction("document-save-as", i18n("Save Script as"),    "save-script-as",    Qt::Key_W, SLOT(saveScriptAs()), _codeEditor);

    kDebug() << "Creating Actions (import export)";
    if (PluginManager::instance()->filePlugins().count()) {
        createAction("document-save-as", i18n("Import"), "import", Qt::Key_I, SLOT(importFile()), this);
        createAction("document-save-as", i18n("Export"), "export", Qt::Key_E, SLOT(exportFile()), this);
    }

    KStandardAction::quit ( kapp, SLOT ( quit() ),  actionCollection() );
}

void MainWindow::createAction(const QByteArray& iconName, const QString& actionTitle, const QString& actionName,
                              const QKeySequence& shortcut, const char* slot, QObject *parent)
{
    KAction* action = new KAction ( KIcon ( iconName ), actionTitle, parent );
    action->setShortcut ( shortcut);
    action->setShortcutContext ( Qt::WidgetShortcut );
    actionCollection()->addAction ( actionName, action );
    connect ( action, SIGNAL ( triggered ( bool ) ), parent, slot );
}

void MainWindow::showSettings()
{
     KConfigDialog dialog(this,  "settings", Settings::self());

     IncludeManagerSettings * set = new IncludeManagerSettings(&dialog);
     ConfigureDefaultProperties * defaultProperties = new ConfigureDefaultProperties( &dialog );

     dialog.addPage(set,"Include Manager",QString(),"Include Manager",true);
     dialog.addPage(defaultProperties,"Default Settings",QString(),"Default Settings",true);


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
        if (p->name() == DataStructurePluginManager::self()->actualPluginName()){
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
    QtScriptBackend *engine = activeDocument->engineBackend();

    _graphVisualEditor->setActiveDocument();
    _GraphLayers->setActiveDocument();

    connect ( this, SIGNAL(runTool(  ToolsPluginInterface*,Document*)),
                activeDocument->engineBackend(), SLOT (runTool( ToolsPluginInterface*,Document*)));

//     connect(this, SIGNAL(startEvaluation()),   engine,  SLOT(start()));
    connect( engine, SIGNAL(sendDebug(QString)), this,  SLOT(debugString(QString)));
    connect( engine, SIGNAL(sendOutput(QString)), this, SLOT(outputString(QString)));
    connect( engine, SIGNAL(finished()),   _bottomTabs, SLOT(setPlayString()));
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

// void MainWindow::debug ( const QString& s ){
//     _txtDebug->insertPlainText ( s );
// }

int MainWindow::saveIfChanged(){
    if ( DocumentManager::self()->activeDocument()->isModified() ){
        const int btnCode = KMessageBox::warningYesNoCancel ( this, i18n ( "Do you want to save your unsaved document?" ) );
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

void MainWindow::executeScript(const QString& text) {
    kDebug() << "Going to execut the script";
    if (_txtDebug == 0)   return;
    if ( scene() == 0)    return;

    _txtDebug->clear();

    QString script = text.isEmpty() ? _codeEditor->text() : text;

    IncludeManager inc;

    script = inc.include(script, _codeEditor->document()->url().path(),
                                 _codeEditor->document()->documentName());

    QtScriptBackend *engine = DocumentManager::self()->activeDocument()->engineBackend();
    if ( !engine->isRunning() ){
        _bottomTabs->setStopString();
        engine->setScript(script, DocumentManager::self()->activeDocument());
        engine->start();
    }else{
       _bottomTabs->setPlayString();
       engine->stop();
    }
}

#endif

void MainWindow::outputString ( const QString& s ) { _txtOutput->append ( s ); }
void MainWindow::debugString ( const QString& s )  { _txtDebug->append ( s );  }
