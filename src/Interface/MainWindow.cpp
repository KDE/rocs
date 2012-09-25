/*
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include <QPointer>
#include <QGridLayout>

// KDE Related Includes
#include <KActionCollection>
#include <KRecentFilesAction>
#include <KActionMenu>
#include <KApplication>
#include <KDebug>
#include <KIcon>
#include <KPushButton>
#include <KStatusBar>
#include <KTar>
#include <KTextBrowser>
#include <KMessageBox>
#include <KLocalizedString>
#include <KConfigDialog>
#include <KFileDialog>
#include <KInputDialog>
#include <KMenu>
#include <KToolBar>
#include <KComboBox>

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
#include "Project.h"

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
#include <QFormLayout>
#include <QScriptEngineDebugger>
#include "IncludeManagerSettings.h"
#include "ConfigureDefaultProperties.h"
#include <IncludeManager.h>
#include "ImporterExporterManager.h"
#include <DataStructurePluginInterface.h>
#include <DataStructurePluginManager.h>
#include "DocumentManager.h"
#include "PossibleIncludes.h"
#include "LoadedPluginsDialog.h"
#include <PropertiesDialogAction.h>


MainWindow::MainWindow() :  KXmlGuiWindow(), _scriptDbg(0)
{
    setObjectName("RocsMainWindow");

    setupWidgets();
    setupActions();
    setupGUI();

    //statusBar()->hide();

    setupToolsPluginsAction();
    setupDSPluginsAction();

    connect(DocumentManager::self(), SIGNAL(activateDocument()),
            this, SLOT(setActiveDocument()));
    connect(DocumentManager::self(), SIGNAL(deactivateDocument(Document*)),
            this, SLOT(releaseDocument(Document*)));
    connect(DocumentManager::self(), SIGNAL(documentRemoved(Document*)),
            this, SLOT(releaseDocument(Document*)));
    connect(DocumentManager::self(), SIGNAL(documentListChanged()),
            this, SLOT(updateGraphDocumentList()));

    // TODO: use welcome widget instead of creating default empty project
    _currentProject = createNewProject();
    updateCaption();

    GraphicsLayout::self()->setViewStyleDataNode(Settings::dataNodeDisplay());
    GraphicsLayout::self()->setViewStyleDataEdge(Settings::dataEdgeDisplay());
}

MainWindow::~MainWindow()
{
    Settings::setVSplitterSizeTop(_vSplitter->sizes() [0]);
    Settings::setVSplitterSizeBottom(_vSplitter->sizes() [1]);
    Settings::setHSplitterSizeLeft(_hSplitter->sizes() [0]);
    Settings::setHSplitterSizeRight(_hSplitter->sizes() [1]);
    Settings::setHScriptSplitterSizeLeft(_hScriptSplitter->sizes() [0]);
    Settings::setHScriptSplitterSizeRight(_hScriptSplitter->sizes() [1]);
    _recentProjects->saveEntries(Settings::self()->config()->group("RecentFiles"));

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
    _vSplitter = new QSplitter(this);
    _vSplitter->setOrientation(Qt::Vertical);

    // setup upper half
    QWidget *leftPanel  = setupWhiteboardPanel();         // graph properties
    _graphVisualEditor = GraphVisualEditor::self(); // graph editor whiteboard

    _hSplitter = new QSplitter(this);
    _hSplitter->setOrientation(Qt::Horizontal);
    _hSplitter->addWidget(leftPanel);
    _hSplitter->addWidget(_graphVisualEditor);

    // setup lower half
    QWidget *scriptPanel = setupScriptPanel();

    _vSplitter->addWidget(_hSplitter);
    _vSplitter->addWidget(scriptPanel);

    _hScriptSplitter->setSizes(QList<int>() << Settings::hScriptSplitterSizeLeft() << Settings::hScriptSplitterSizeRight() << 80);
    _vSplitter->setSizes(QList<int>() << Settings::vSplitterSizeTop() << Settings::vSplitterSizeBottom());
    _hSplitter->setSizes(QList<int>() << Settings::hSplitterSizeLeft() << Settings::hSplitterSizeRight());

    setCentralWidget(_vSplitter);
}

Project* MainWindow::createNewProject()
{
    Project* newProject = new Project();
    // create new document and add this to project new
    newProject->addGraphFileNew(DocumentManager::self()->newDocument());
    newProject->addCodeFileNew(_codeEditor->newScript());

    return newProject;
}

void MainWindow::downloadNewExamples()
{
    KNS3::DownloadDialog dialog("rocs.knsrc", this);
    dialog.exec();
}

void MainWindow::uploadScript()
{

    QPointer<KNS3::UploadDialog> dialog = new KNS3::UploadDialog(this);

//First select the opened doc.
    KUrl str = _codeEditor->document()->url();
    if (str.isEmpty()) {
        //... then try to open
        str = KFileDialog::getOpenFileName(QString(), i18n("*.js|Script files"),
                                           this, i18n("Rocs Script Files"));
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
    dialog->setUploadFile(local);

    dialog->setUploadName(_codeEditor->document()->documentName());
    dialog->setDescription(i18n("Add your description here."));

    dialog->exec();

//Remove compressed file..
    QDir::temp().remove(local);
}


QWidget* MainWindow::setupScriptPanel()
{
    _hScriptSplitter = new QSplitter(this);
    _hScriptSplitter->setOrientation(Qt::Horizontal);

    _codeEditor = new CodeEditor(this);
    _txtDebug = new KTextBrowser(this);
    _txtOutput = new KTextBrowser(this);

    QStackedWidget *stackedListing = new QStackedWidget;
    stackedListing->addWidget(_txtOutput);
    stackedListing->addWidget(_txtDebug);

    _selectListing = new QComboBox;
    _selectListing->addItem(KIcon("accessories-text-editor"), i18nc("@item:inlistbox Select output", "Program Messages"));
    _selectListing->addItem(KIcon("tools-report-bug"), i18nc("@item:inlistbox Select output", "Debug Messages"));

    QWidget *header = new QWidget(this);
    header->setLayout(new QHBoxLayout);
    header->layout()->addWidget(new QLabel(i18nc("@label:listbox", "Select output:")));
    header->layout()->addWidget(_selectListing);

    QWidget *listingWidget = new QWidget(this);
    listingWidget->setLayout(new QVBoxLayout);
    listingWidget->layout()->addWidget(header);
    listingWidget->layout()->addWidget(stackedListing);

    KToolBar *executeCommands = new KToolBar(this);
    executeCommands->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    executeCommands->setOrientation(Qt::Vertical);
    _runScript = new KAction(KIcon("media-playback-start"), i18nc("@action:intoolbar Script Execution", "Run"), this);
    _stepRunScript = new KAction(KIcon("go-next"), i18nc("@action:intoolbar Script Execution", "One Step"), this);
    _stopScript = new KAction(KIcon("process-stop"), i18nc("@action:intoolbar Script Execution", "Stop"), this);
    _stopScript->setEnabled(false);
    executeCommands->addAction(_runScript);
    executeCommands->addAction(_stepRunScript);
    // add actions to action collection to be able to set shortcuts on them in the ui
    actionCollection()->addAction("_runScript", _runScript);
    actionCollection()->addAction("_stepRunScript", _stepRunScript);
    actionCollection()->addAction("_stopScript", _stopScript);

    // debug controls submenu
    _debugMenu = new KActionMenu(KIcon("debug-run"), i18nc("@title:menu Debug execution", "Debug"), this);
    _debugScript = new KAction(KIcon("debug-run"), i18nc("@action:inmenu Debug execution", "Debug run"), _debugMenu);
    _interruptScript = new KAction(KIcon("debug-run-cursor"), i18nc("@action:inmenu Debug execution", "Interrupt at first line"), _debugMenu);
    _debugMenu->addAction(_debugScript);
    _debugMenu->addAction(_interruptScript);
    executeCommands->addWidget(_debugMenu->createWidget(executeCommands));
    executeCommands->addAction(_stopScript);
    actionCollection()->addAction("_debugScript", _debugScript);
    actionCollection()->addAction("_interruptScript", _interruptScript);

    // set toolbar visibility defaults
    showExecutionButtonDebug(Settings::excutionModeDebugVisible());
    showExecutionButtonOneStep(Settings::excutionModeOneStepVisible());

    connect(_runScript, SIGNAL(triggered()), this, SLOT(executeScriptFull()));
    connect(_stepRunScript, SIGNAL(triggered()), this, SLOT(executeScriptOneStep()));
    connect(_debugScript, SIGNAL(triggered()), this, SLOT(debugScript()));
    connect(_interruptScript, SIGNAL(triggered()), this, SLOT(debugScript()));
    connect(_stopScript, SIGNAL(triggered()), this, SLOT(stopScript()));
    connect(_selectListing, SIGNAL(currentIndexChanged(int)), stackedListing, SLOT(setCurrentIndex(int)));

    _hScriptSplitter->addWidget(_codeEditor);
    _hScriptSplitter->addWidget(listingWidget);

    QWidget *scriptInterface = new QWidget(this);
    scriptInterface->setLayout(new QHBoxLayout);
    scriptInterface->layout()->addWidget(_hScriptSplitter);
    scriptInterface->layout()->addWidget(executeCommands);

    return scriptInterface;
}

QWidget* MainWindow::setupWhiteboardPanel()
{
    QWidget *panel = new QWidget(this);

    _documentSelectorCombo = new KComboBox(this);
    _GraphLayers = new GraphLayers(this);

    // arrange widgets
    QWidget* selectorForm = new QWidget(panel);
    QGridLayout* documentLayout = new QGridLayout(selectorForm);
    selectorForm->setLayout(documentLayout);

    // at first setup document does not exists
    // this is later set when MainWindow::setActiveDocument() is called
    PropertiesDialogAction *propertiesAction = new PropertiesDialogAction(i18nc("@action:button", "Properties"), DocumentManager::self()->activeDocument(), this);
    _documentPropertiesButton = new QToolButton(panel);
    _documentPropertiesButton->setDefaultAction(propertiesAction);
    _documentPropertiesButton->setIcon(KIcon("document-properties"));

    documentLayout->addWidget(new QLabel(i18nc("@label:listbox", "Graph Document:")),1,1);
    documentLayout->addWidget(_documentSelectorCombo,1,2);
    documentLayout->addWidget(_documentPropertiesButton,1,3);

    connect(_documentSelectorCombo, SIGNAL(activated(int)),
            DocumentManager::self(), SLOT(changeDocument(int)));

    panel->setLayout(new QVBoxLayout);
    panel->layout()->addWidget(selectorForm);
    panel->layout()->addWidget(_GraphLayers);

    return panel;
}

void MainWindow::setupActions()
{
    kDebug() << "create and connect actions";
    KStandardAction::quit(kapp, SLOT(quit()), actionCollection());
    KStandardAction::preferences(this, SLOT(showSettings()), actionCollection());

    GraphScene *gc = _graphVisualEditor->scene();

    _addDataActionMenu = new KActionMenu(KIcon("rocsadddata"), i18nc("@title:menu", "Add Data"), this);
    _addDataActionMenu->setIconText(i18nc("@action:intoolbar", "Add Data"));
    _addDataActionMenu->setToolTip(i18nc("@info:tooltip", "Add new data element"));
    _addDataActionMenu->setDelayed(true);
    _addDataActionMenu->setCheckable(true);

    _addPointerActionMenu = new KActionMenu(KIcon("rocsaddedge"), i18nc("@title:menu", "Add Connection"), this);
    _addPointerActionMenu->setIconText(i18nc("@action:intoolbar", "Add Connection"));
    _addPointerActionMenu->setToolTip(i18nc("@info:tooltip", "Add a new connection between two data elements of selected type"));
    _addPointerActionMenu->setDelayed(true);
    _addPointerActionMenu->setCheckable(true);

    _selectMoveAction = new SelectMoveHandAction(gc, this);
    DeleteHandAction* deleteAction = new DeleteHandAction(gc, this);
    ZoomAction* zoomAction = new ZoomAction(gc, this);

    connect(_selectMoveAction, SIGNAL(triggered()), _selectMoveAction, SLOT(sendExecuteBit()));
    connect(deleteAction, SIGNAL(triggered()), deleteAction, SLOT(sendExecuteBit()));
    connect(zoomAction, SIGNAL(triggered()), zoomAction, SLOT(sendExecuteBit()));

    _paletteActions = actionCollection();
    QActionGroup *g = new QActionGroup(this);

    g->addAction(_paletteActions->addAction("selectmove", _selectMoveAction));
    g->addAction(_paletteActions->addAction("add_node", _addDataActionMenu));
    g->addAction(_paletteActions->addAction("add_edge", _addPointerActionMenu));
    g->addAction(_paletteActions->addAction("delete", deleteAction));
    g->addAction(_paletteActions->addAction("zoom", zoomAction));
    actionCollection()->action("selectmove")->toggle();
    gc->setAction(_selectMoveAction);

    _paletteActions->addAction("align-hbottom", new AlignAction(i18nc("@action:intoolbar Alignment", "Base"),  AlignAction::Bottom, gc));
    _paletteActions->addAction("align-hcenter", new AlignAction(i18nc("@action:intoolbar Alignment", "Center"), AlignAction::HCenter, gc));
    _paletteActions->addAction("align-htop",   new AlignAction(i18nc("@action:intoolbar Alignment", "Top"),   AlignAction::Top,    gc));
    _paletteActions->addAction("align-vleft",  new AlignAction(i18nc("@action:intoolbar Alignment", "Left"),  AlignAction::Left,   gc));
    _paletteActions->addAction("align-vcenter", new AlignAction(i18nc("@action:intoolbar Alignment", "Center"), AlignAction::VCenter, gc));
    _paletteActions->addAction("align-vright", new AlignAction(i18nc("@action:intoolbar Alignment", "Right"), AlignAction::Right,  gc));
    _paletteActions->addAction("align-circle", new AlignAction(i18nc("@action:intoolbar Alignment", "Circle"),  AlignAction::Circle, gc));
    _paletteActions->addAction("align-tree", new AlignAction(i18nc("@action:intoolbar Alignment", "Minimize Crossing Edges"), AlignAction::MinCutTree, gc));

    // Menu actions
    createAction("document-new",        i18nc("@action:inmenu", "New Project"),        "new-project", QKeySequence::New, SLOT(newProject()), this);
    createAction("document-save",       i18nc("@action:inmenu", "Save Project"),       "save-project", QKeySequence::Save, SLOT(saveProject()), this);
    createAction("document-open",       i18nc("@action:inmenu", "Open Project"),       "open-project", QKeySequence::Open, SLOT(openProject()), this);
    
    _recentProjects = new KRecentFilesAction(KIcon ("document-open"), i18nc("@action:inmenu","Recent Files"), this);
    connect(_recentProjects, SIGNAL(urlSelected(KUrl)), this, SLOT(openProject(KUrl)));
    actionCollection()->addAction("recent-project", _recentProjects);
    
    _recentProjects->loadEntries(Settings::self()->config()->group("RecentFiles"));
    
    
    createAction("document-export",     i18nc("@action:inmenu", "Export Project"),     "export-project",    SLOT(exportProject()), this);
    createAction("document-import",     i18nc("@action:inmenu", "Import Project"),     "import-project",    SLOT(importProject()), this);
    createAction("document-properties", i18nc("@action:inmenu", "Set Name"),           "set-project-name",  SLOT(setProjectName()), this);
    createAction("document-new",        i18nc("@action:inmenu", "New Graph Document"), "new-graph",         SLOT(newGraph()), this);
    createAction("document-new",        i18nc("@action:inmenu", "New Script File"),    "new-script",        SLOT(newScript()),    this);
    createAction("document-import",     i18nc("@action:inmenu", "Import Graph"),       "import-graph",      SLOT(importGraphFile()),   this);
    createAction("document-save",       i18nc("@action:inmenu", "Save Graph"),         "save-graph",        SLOT(saveGraph()),   this);
    createAction("document-export",     i18nc("@action:inmenu", "Export Graph"),       "export-graph",      SLOT(exportGraphFile()), this);
    createAction("get-hot-new-stuff",   i18nc("@action:inmenu", "Download Examples"),  "download",          SLOT(downloadNewExamples()),  this);
    createAction("get-hot-new-stuff",   i18nc("@action:inmenu", "Upload script"),      "upload",            SLOT(uploadScript()),  this);

    createAction("help-hint", i18nc("@action:inmenu", "Possible Includes"),          "possible_includes",   SLOT(showPossibleIncludes()), this);
    createAction("document-import",  i18nc("@action:inmenu", "Import Script"),       "add-script",          SLOT(importScript()),   this);
    createAction("document-save",    i18nc("@action:inmenu", "Save Script"),         "save-script",         SLOT(saveActiveScript()),   _codeEditor);
    createAction("document-save-as", i18nc("@action:inmenu", "Save Script as"),      "save-script-as",      SLOT(saveActiveScriptAs()), _codeEditor);
    createAction("",  i18nc("@action:inmenu", "Loaded Plugins"),      "loaded-plugins",      SLOT(showLoadedPlugins()), this);

    // EDIT actions
    actionCollection()->addAction("delete-selected", new DeleteAction(i18nc("@action:intoolbar", "Delete"), _graphVisualEditor->scene(), 0));

    KStandardAction::quit(kapp, SLOT(quit()),  actionCollection());
}

void MainWindow::createAction(const QByteArray& iconName, const QString& actionTitle, const QString& actionName,
                              const QKeySequence& shortcut, const char* slot, QObject *parent)
{
    KAction* action = new KAction(KIcon(iconName), actionTitle, parent);
    action->setShortcut(shortcut);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction(actionName, action);
    connect(action, SIGNAL(triggered(bool)), parent, slot);
}

void MainWindow::createAction(const QByteArray& iconName, const QString& actionTitle, const QString& actionName,
                              const char* slot, QObject *parent)
{
    KAction* action = new KAction(KIcon(iconName), actionTitle, parent);
    action->setShortcutContext(Qt::WidgetShortcut);
    actionCollection()->addAction(actionName, action);
    connect(action, SIGNAL(triggered(bool)), parent, slot);
}


void MainWindow::showSettings()
{
    KConfigDialog dialog(this,  "settings", Settings::self());

    IncludeManagerSettings * set = new IncludeManagerSettings(&dialog);
    ConfigureDefaultProperties * defaultProperties = new ConfigureDefaultProperties(&dialog);

    dialog.addPage(set, i18nc("@title:tab", "Include Manager"), QString(), i18nc("@title:tab", "Include Manager"), true);
    dialog.addPage(defaultProperties, i18nc("@title:tab", "Default Settings"), QString(), i18nc("@title:tab", "Default Settings"), true);


    connect(set,               SIGNAL(changed(bool)), &dialog, SLOT(enableButtonApply(bool)));
    connect(defaultProperties, SIGNAL(changed(bool)), &dialog, SLOT(enableButtonApply(bool)));

    connect(&dialog, SIGNAL(applyClicked()),   set, SLOT(saveSettings()));
    connect(&dialog, SIGNAL(okClicked()),      set, SLOT(saveSettings()));
    connect(&dialog, SIGNAL(defaultClicked()), set, SLOT(readConfig()));

    connect(&dialog, SIGNAL(applyClicked()),   defaultProperties, SLOT(saveConfig()));
    connect(&dialog, SIGNAL(okClicked()),      defaultProperties, SLOT(saveConfig()));
    connect(&dialog, SIGNAL(defaultClicked()), defaultProperties, SLOT(readConfig()));

    connect(defaultProperties, SIGNAL(showExecuteModeDebugChanged(bool)),
            this, SLOT(showExecutionButtonDebug(bool)));
    connect(defaultProperties, SIGNAL(showExecuteModeOneStepChanged(bool)),
            this, SLOT(showExecutionButtonOneStep(bool)));

    dialog.exec();
}

void MainWindow::setupToolsPluginsAction()
{
    if (_toolsPlugins.isEmpty()) {
        createToolsPluginsAction();
    }

    foreach(QAction* action, _toolsPlugins) {
        ToolsPluginInterface *plugin = PluginManager::instance()->toolPlugins().at(action->data().toInt());
        action->setEnabled(
                DocumentManager::self()->activeDocument() &&
                plugin->supportedDataStructures().contains(DocumentManager::self()->activeDocument()->dataStructureInternalName())
                          );
    }
}

void MainWindow::createToolsPluginsAction(){
    QAction* action = 0;
    QList<ToolsPluginInterface*> avaliablePlugins =  PluginManager::instance()->toolPlugins();
    int count = 0;
    foreach(ToolsPluginInterface * plugin, avaliablePlugins) {
        action = new KAction(plugin->displayName(), this);
        action->setData(count++);
        connect(action, SIGNAL(triggered(bool)), this, SLOT(runToolPlugin()));
        _toolsPlugins << action;
    }
    unplugActionList("tools_plugins");
    plugActionList("tools_plugins", _toolsPlugins);
}

void MainWindow::setupDSPluginsAction()
{
    QList <QAction*> pluginList;
    QAction* action = 0;
    unplugActionList("DS_plugins");

    QList < DataStructurePluginInterface*> avaliablePlugins = DataStructurePluginManager::self()->pluginsList();

    QActionGroup * group = new QActionGroup(this);

    int count = 0;
    foreach(DataStructurePluginInterface * p, avaliablePlugins) {
        action = new KAction(p->name(), this);
        action->setData(count++);
        action->setCheckable(true);

        if (p->internalName() == DataStructurePluginManager::self()->actualPlugin()->internalName()) {
            action->setChecked(true);
        }

        action->setActionGroup(group);

        connect(action, SIGNAL(triggered(bool)),
                DataStructurePluginManager::self(), SLOT(setDataStructurePlugin()));

        pluginList.append(action);
    }
    plugActionList("DS_plugins", pluginList);
}

void MainWindow::setupDocumentsList()
{
    QList <QAction*> pluginList;
    QAction* action = 0;
    unplugActionList("Doc_List");
    QActionGroup * group = new QActionGroup(this);
    int count = 0;
    foreach(Document * doc, DocumentManager::self()->documentList()) {
        action = new KAction(doc->name(), this);
        action->setData(count++);
        action->setCheckable(true);
        if (doc == DocumentManager::self()->activeDocument()) {
            action->setChecked(true);
        }
        action->setActionGroup(group);
        connect(action, SIGNAL(triggered(bool)), DocumentManager::self(), SLOT(changeDocument()));
        pluginList.append(action);
    }

    plugActionList("Doc_List", pluginList);
}

void MainWindow::setActiveDocument()
{
    kDebug() << "Setting the document in the main window";
    Document *activeDocument = DocumentManager::self()->activeDocument();
    QtScriptBackend *engine = activeDocument->engineBackend();

    _graphVisualEditor->setActiveDocument();
    _GraphLayers->setActiveDocument();

    // set button for document properties
    if (_documentPropertiesButton->defaultAction()) {
        _documentPropertiesButton->defaultAction()->deleteLater();
        PropertiesDialogAction *propertiesAction = new PropertiesDialogAction(i18nc("@action:button", "Properties"), activeDocument, this);
        _documentPropertiesButton->setDefaultAction(propertiesAction);
        _documentPropertiesButton->setIcon(KIcon("document-properties"));
    }

    // Graphical Data Structure Editor toolbar
    updateToolbarTypeActions();
    connect(activeDocument, SIGNAL(dataTypeCreated(int)), this, SLOT(updateToolbarTypeActions()));
    connect(activeDocument, SIGNAL(pointerTypeCreated(int)), this, SLOT(updateToolbarTypeActions()));
    connect(activeDocument, SIGNAL(dataTypeRemoved(int)), this, SLOT(updateToolbarTypeActions()));
    connect(activeDocument, SIGNAL(pointerTypeRemoved(int)), this, SLOT(updateToolbarTypeActions()));

    // Engine toolbar
    connect(this, SIGNAL(runTool(ToolsPluginInterface*, Document*)),
            activeDocument->engineBackend(), SLOT(runTool(ToolsPluginInterface*, Document*)));
//     connect(this, SIGNAL(startEvaluation()),   engine,  SLOT(start()));
    connect(engine, SIGNAL(sendDebug(QString)), this,  SLOT(debugString(QString)));
    connect(engine, SIGNAL(scriptError()), this, SLOT(showDebugOutput()));
    connect(engine, SIGNAL(sendOutput(QString)), this, SLOT(outputString(QString)));
    connect(engine, SIGNAL(finished()), this, SLOT(disableStopAction()));

    activeDocument->setModified(false);
    setupToolsPluginsAction();
}


void MainWindow::updateToolbarTypeActions()
{
    Document *activeDocument = DocumentManager::self()->activeDocument();

    _addDataActionMenu->menu()->clear();
    foreach (int identifier, activeDocument->dataTypeList()) {
        DataTypePtr type = activeDocument->dataType(identifier);
        AddDataHandAction* addDataAction = new AddDataHandAction(scene(), type, _addDataActionMenu->menu());
        addDataAction->setCheckable(false);

        if (identifier == 0) { // set default action to menu
            connect(_addDataActionMenu, SIGNAL(triggered()), addDataAction, SLOT(sendExecuteBit()));
        }

        _addDataActionMenu->menu()->addAction(addDataAction);
        connect(type.get(), SIGNAL(iconChanged(QString)), addDataAction, SLOT(updateIcon()));
        connect(addDataAction, SIGNAL(triggered()), addDataAction, SLOT(sendExecuteBit()));
    }

    _addPointerActionMenu->menu()->clear();
    foreach (int identifier, activeDocument->pointerTypeList()) {
        PointerTypePtr type = activeDocument->pointerType(identifier);
        AddConnectionHandAction* addPointerAction = new AddConnectionHandAction(scene(), type, _addPointerActionMenu->menu());
        addPointerAction->setCheckable(false);

        if (identifier == 0) { // set default action to menu
            connect(_addPointerActionMenu, SIGNAL(triggered()), addPointerAction, SLOT(sendExecuteBit()));
        }

        _addPointerActionMenu->menu()->addAction(addPointerAction);
        connect(addPointerAction, SIGNAL(triggered()), addPointerAction, SLOT(sendExecuteBit()));
    }
}


void MainWindow::releaseDocument(Document* d)
{
    if (d == 0) {
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

void MainWindow::addEmptyGraphDocument()
{
    _currentProject->addGraphFileNew(
        DocumentManager::self()->newDocument()
    );
}


void MainWindow::updateGraphDocumentList()
{
    _documentSelectorCombo->clear();
    foreach(Document * document, DocumentManager::self()->documentList()) {
        _documentSelectorCombo->addItem(document->name());
    }
}


void MainWindow::importScript()
{
    KUrl startDirectory = Settings::lastOpenedDirectory();
    if (!_currentProject->isTemporary()) {
        startDirectory = KUrl::fromPath(_currentProject->projectDirectory());
    }

    QString fileUrl = KFileDialog::getOpenFileName(  startDirectory,
                                                     QString(),
                                                     this,
                                                     i18nc("@title:window", "Add Existing Script File to Project"));
    if (fileUrl.isEmpty()) {
        return;
    }

    _currentProject->addCodeFile(fileUrl);
    _codeEditor->openScript(fileUrl);
    Settings::setLastOpenedDirectory(startDirectory.toLocalFile());
}


void MainWindow::loadDocument(const QString& name)
{
    if (!name.isEmpty() && !name.endsWith(QLatin1String(".graph"))) {
        KMessageBox::sorry(this, i18nc("@info", "This does not seem to be a graph file."), i18nc("@title:window", "Invalid File"));
        return;
    }

    DocumentManager::self()->openDocument(KUrl::fromLocalFile(name));
}


void MainWindow::newProject()
{
    saveIfChanged();
    _codeEditor->closeAllScripts();
    DocumentManager::self()->closeAllDocuments();

    delete _currentProject;
    _currentProject = new Project();
    _currentProject->addCodeFileNew(_codeEditor->newScript());
    _currentProject->addGraphFileNew(DocumentManager::self()->newDocument());
    updateCaption();
}


void MainWindow::newProjectAssistant()
{
    //TODO needs to be implemented
}


void MainWindow::saveProject()
{
    // save graphs and scripts
    if (_currentProject->isTemporary()) {
        QString file = KFileDialog::getSaveFileName(QString(),
                       i18n("*.rocs|Rocs project files\n*|All files"),
                       this,
                       i18nc("@title:window", "Save Project"));

        // we need to set project directory first to allow correcte relative paths
        _currentProject->setProjectFile(KUrl::fromLocalFile(file));

        // save files and finally write project to file
        saveAllGraphs();
        saveScripts();
        _currentProject->writeProjectFile(file);
        updateCaption();
    } else {
        saveAllGraphs();
        saveScripts();
        _currentProject->writeProjectFile();
    }
}


void MainWindow::openProject(const KUrl& fileName)
{
    saveIfChanged();
    KUrl file = fileName;
    if (file.isEmpty()){
    // show open dialog
         file = KFileDialog::getOpenUrl(KUrl(),
                   i18n("*.rocs|Rocs project files\n*|All files"),
                   this,
                   i18nc("@title:window", "Open Project Files"));

        if (file.isEmpty()) {
            return;
        }
    }
    // import project specified: close everything and delete old project
    _codeEditor->closeAllScripts();
    DocumentManager::self()->closeAllDocuments();
    delete _currentProject;

    // extract and open new project
    // at the end of this _currentProject must exist
    _currentProject = new Project(file);
    foreach(const KUrl& graphFile, _currentProject->graphFiles()) {
        DocumentManager::self()->openDocument(graphFile);
    }
    if (_currentProject->graphFiles().count() == 0) {
        _currentProject->addGraphFileNew(DocumentManager::self()->newDocument());
    }
    foreach(const KUrl& codeFile, _currentProject->codeFiles()) {
        _codeEditor->openScript(codeFile);
        //TODO set curser line
    }
    if (_currentProject->codeFiles().count() == 0) {
        _currentProject->addCodeFileNew(_codeEditor->newScript());
    }

    updateCaption();
    _recentProjects->addUrl(file);
}


void MainWindow::setProjectName()
{
    QString name = KInputDialog::getText(i18nc("@title:window", "Set Project Name"),
                                         i18nc("@label:textbox name of the project", "Name:"),
                                         _currentProject->name()
                                        );
    _currentProject->setName(name);
}

void MainWindow::updateCaption()
{
    if (!_currentProject) {
        return;
    }

    QString caption = "";
    if (!_currentProject->name().isEmpty()) {
        caption.append(_currentProject->name());
    }
    else if (_currentProject->isTemporary()) {
        caption.append(i18nc("caption text for temporary project", "[ untitled ]"));
    } else {
        caption.append(_currentProject->projectFile().toLocalFile());
    }
    setCaption(caption);
}



void MainWindow::saveScripts()
{
    foreach(KTextEditor::Document * textDocument, _currentProject->codeFilesNew()) {
        QString file = KFileDialog::getSaveFileName(QString(),
                       i18n("*.js|Rocs script documents\n*|All files"),
                       this,
                       i18nc("@title:window", "Save Script Document"));
        _currentProject->saveCodeFileNew(textDocument, file);
    }
    _codeEditor->saveAllScripts();
}


void MainWindow::newScript()
{
    QString file = KFileDialog::getSaveFileName(_currentProject->projectDirectory(),
                   i18n("*.js|Rocs script documents\n*|All files"),
                   this,
                   i18nc("@title:window", "Enter Filename for the New Script File"));
    if (file.isEmpty()) {
        kDebug() << "Filename is empty and no script file was created.";
        return;
    }
    _codeEditor->newScript(KUrl::fromLocalFile(file));
}


void MainWindow::saveGraph()
{
    saveGraph(DocumentManager::self()->activeDocument());
}


void MainWindow::saveGraph(Document* document)
{
    if (document == 0) {
        document = DocumentManager::self()->activeDocument();
    }
    Q_ASSERT(document);
    if (document->fileUrl().isEmpty()) {
        saveGraphAs(document);
    } else {
        document->save();
    }
}


void MainWindow::saveAllGraphs()
{
    foreach(Document * document, DocumentManager::self()->documentList()) {
        if (document->fileUrl().isEmpty()) {
            saveGraphAs(document);
        } else if (document->isModified()) {
            document->save();
        }
    }
}


void MainWindow::saveGraphAs()
{
    saveGraphAs(DocumentManager::self()->activeDocument());
}


void MainWindow::saveGraphAs(Document* document)
{
    if (document == 0) {
        document = DocumentManager::self()->activeDocument();
    }
    Q_ASSERT(document);
    QString file = KFileDialog::getSaveFileName(QString(),
                   i18n("*.graph|Rocs graph documents\n*|All files"),
                   this,
                   i18nc("@title:window", "Save Graph Document"));
    _currentProject->saveGraphFileAs(document, file);
}


void MainWindow::newGraph()
{
    QString file = KFileDialog::getSaveFileName(_currentProject->projectDirectory(),
                   i18n("*.graph|Rocs graph documents\n*|All files"),
                   this,
                   i18nc("@title:window", "Enter Filename for the New Graph Document"));
    if (file.isEmpty()) {
        kDebug() << "Filename is empty and no script file was created.";
        return;
    }
    DocumentManager::self()->openDocument(KUrl::fromLocalFile(file));
    _currentProject->addGraphFile(file);
}


int MainWindow::saveIfChanged()
{
    if (_currentProject->isModified()) {
        const int btnCode = KMessageBox::warningYesNoCancel(this, i18nc(
                                "@info",
                                "Changes on your project are unsaved. Do you want to save your changes?"));
        if (btnCode == KMessageBox::Yes) {
            _currentProject->writeProjectFile();
            saveAllGraphs();
            _codeEditor->saveAllScripts();
        }
        return btnCode;
    }

    bool anyGraphDocumentModified = false;
    foreach(Document * document, DocumentManager::self()->documentList()) {
        if (document->isModified()) {
            anyGraphDocumentModified = true;
            break;
        }
    }
    if (anyGraphDocumentModified && !_codeEditor->isModified()) {
        const int btnCode = KMessageBox::warningYesNoCancel(this, i18nc(
                                "@info",
                                "Changes on your graph document are unsaved. Do you want to save your changes?"));
        if (btnCode == KMessageBox::Yes) {
            saveAllGraphs();
        }
        return btnCode;
    }
    if (!DocumentManager::self()->activeDocument()->isModified() && _codeEditor->isModified()) {
        const int btnCode = KMessageBox::warningYesNoCancel(this, i18nc(
                                "@info",
                                "Changes on your script files are unsaved. Do you want to save all unsaved scripts?"));
        if (btnCode == KMessageBox::Yes) {
            _codeEditor->saveAllScripts();
        }
        return btnCode;
    }
    if (anyGraphDocumentModified && _codeEditor->isModified()) {
        const int btnCode = KMessageBox::warningYesNoCancel(this, i18nc(
                                "@info",
                                "Changes on your script files and on your graph document are unsaved. Do you want to save your graph document and all unsaved scripts?"));
        if (btnCode == KMessageBox::Yes) {
            _codeEditor->saveAllScripts();
            saveAllGraphs();
        }
        return btnCode;
    }
    return KMessageBox::No;
}


void MainWindow::importGraphFile()
{
    ImporterExporterManager importer(this);
    Document * gd = importer.importFile();
    if (gd == 0) {
        return;
    }

    DocumentManager::self()->addDocument(gd);

    _graphVisualEditor->scene()->createItems();

    if (importer.hasDialog()) {
        importer.dialogExec();
    }
}


void MainWindow::exportGraphFile()
{
    ImporterExporterManager exp(this);

    exp.exportFile(DocumentManager::self()->activeDocument());
}


void MainWindow::importProject()
{
    // save current project
    saveIfChanged();

    KUrl startDirectory = Settings::lastOpenedDirectory();
    if (!_currentProject->isTemporary()) {
        startDirectory = KUrl::fromPath(_currentProject->projectDirectory());
    }
    // get import information
    QString importFile = KFileDialog::getOpenFileName(startDirectory,
                   i18n("*.tar.gz|Import from tar.gz Archive"),
                   this,
                   i18nc("@title:window", "Project Archive to Import"));

    if (importFile.isEmpty()) {
        kDebug() << "No file specified for import: aborting.";
        return;
    }

    QString newLocation = KFileDialog::getExistingDirectory(startDirectory,
                   this,
                   i18nc("@title:window", "Select Project Directory for Import"));

    if (newLocation.isEmpty()) {
        kDebug() << "No storage directory specified for import: aborting.";
        return;
    }

    // import project specified: close everything and delete old project
    _codeEditor->closeAllScripts();
    DocumentManager::self()->closeAllDocuments();
    delete _currentProject;

    // extract and open new project
    // at the end of this _currentProject must exist
    _currentProject = new Project(KUrl::fromLocalFile(importFile),KUrl::fromPath(newLocation.append('/')));
    foreach(const KUrl& graphFile, _currentProject->graphFiles()) {
        DocumentManager::self()->openDocument(graphFile);
    }
    if (_currentProject->graphFiles().count() == 0) {
        _currentProject->addGraphFileNew(DocumentManager::self()->newDocument());
    }
    foreach(const KUrl& codeFile, _currentProject->codeFiles()) {
        _codeEditor->openScript(codeFile);
        //TODO set curser line
    }
    if (_currentProject->codeFiles().count() == 0) {
        _currentProject->addCodeFileNew(_codeEditor->newScript());
    }
    updateCaption();
    Settings::setLastOpenedDirectory(newLocation);
}


void MainWindow::exportProject()
{
    KUrl startDirectory = Settings::lastOpenedDirectory();
    if (!_currentProject->isTemporary()) {
        startDirectory = KUrl::fromPath(_currentProject->projectDirectory());
    }
    QString file = KFileDialog::getSaveFileName(startDirectory,
                   i18n("*.tar.gz|Export as tar.gz Archive"),
                   this,
                   i18nc("@title:window", "Export Project"));
    if (file.isEmpty()) {
        kDebug() << "Filename is empty and no script file was created.";
        return;
    }
    _currentProject->exportProject(KUrl::fromLocalFile(file));
    
    Settings::setLastOpenedDirectory(_currentProject->projectDirectory());
}


void MainWindow::showPossibleIncludes()
{
    PossibleIncludes dialog(this);
    dialog.exec();
}


void MainWindow::showLoadedPlugins()
{
    LoadedPluginsDialog dialog(this);
    dialog.exec();
}


void MainWindow::runToolPlugin()
{
    kDebug() << "seeking for a plugin";
    QAction *action = qobject_cast<QAction *> (sender());

    if (! action) {
        return;
    }
    if (ToolsPluginInterface *plugin =  PluginManager::instance()->toolPlugins().value(action->data().toInt())) {
        emit runTool(plugin, DocumentManager::self()->activeDocument());
    }
}


#ifdef USING_QTSCRIPT

void MainWindow::executeScriptFull(const QString& text)
{
    executeScript(MainWindow::Execute, text);
}


void MainWindow::executeScript(const MainWindow::ScriptMode mode, const QString& text)
{
    kDebug() << "Going to execute the script";
    if (_txtDebug == 0)   return;
    if (scene() == 0)    return;


    QString script = text.isEmpty() ? _codeEditor->text() : text;
    QString scriptPath = _codeEditor->document()->url().path();
    QtScriptBackend *engine = DocumentManager::self()->activeDocument()->engineBackend();
    if (engine->isRunning()) {
        engine->stop();
    }

    _txtDebug->clear();
    if (_scriptDbg) {
        _scriptDbg->detach();
        _scriptDbg->deleteLater();
        _scriptDbg = 0;
    }
    if (mode != Execute) {
        _scriptDbg = new QScriptEngineDebugger(this);
        _scriptDbg->setAutoShowStandardWindow(true);
        _scriptDbg->attachTo(engine->engine());
        if (mode == MainWindow::DebugMode)
            _scriptDbg->action(QScriptEngineDebugger::InterruptAction)->trigger();
    }
    engine->includeManager().initialize(Settings::includePath());
    script = engine->includeManager().include(script,
             scriptPath.isEmpty() ? scriptPath : scriptPath.section('/', 0, -2),
             _codeEditor->document()->documentName());

    enableStopAction();

    engine->setScript(script, DocumentManager::self()->activeDocument());
    engine->execute();
}


void MainWindow::executeScriptOneStep(const QString& text)
{
    if (_txtDebug == 0) {
        return;
    }
    if (scene() == 0) {
        return;
    }

    _txtDebug->clear();

    QtScriptBackend *engine = DocumentManager::self()->activeDocument()->engineBackend();

    //TODO disable start action
    enableStopAction();
    if (!engine->isRunning()) {
        QString script = text.isEmpty() ? _codeEditor->text() : text;
        QString scriptPath = _codeEditor->document()->url().path();
        IncludeManager inc;

        script = inc.include(script,
                             scriptPath.isEmpty() ? scriptPath : scriptPath.section('/', 0, -2),
                             _codeEditor->document()->documentName());

        engine->setScript(script, DocumentManager::self()->activeDocument());
        engine->executeStep();
        return;
    }
    engine->continueExecutionStep();
}



void MainWindow::stopScript()
{
    kDebug() << "Going to stop the script";
    if (_txtDebug == 0)   return;
    if (scene() == 0)    return;

    QtScriptBackend *engine = DocumentManager::self()->activeDocument()->engineBackend();

    disableStopAction();

    engine->stop();
}

void MainWindow::debugScript()
{
    QAction *action = qobject_cast<QAction *> (sender());
    if (action == _interruptScript)
        executeScript(DebugMode);
    else
        executeScript(DebugOnlyInCaseOfError);
}


#endif


void MainWindow::enableStopAction()
{
    _stopScript->setEnabled(true);
}

void MainWindow::disableStopAction()
{
    _stopScript->setEnabled(false);
}

void MainWindow::showDebugOutput()
{
    _selectListing->setCurrentIndex(1);
}

void MainWindow::outputString(const QString& s)
{
    _txtOutput->append(s);
}
void MainWindow::debugString(const QString& s)
{
    _txtDebug->append(s);
}

void MainWindow::showExecutionButtonDebug(bool visible)
{
    _debugMenu->setVisible(visible);
}

void MainWindow::showExecutionButtonOneStep(bool visible)
{
    _stepRunScript->setVisible(visible);
}

