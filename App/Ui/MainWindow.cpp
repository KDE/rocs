/*
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011-2013  Andreas Cord-Landwehr <cordlandwehr@kde.org>

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
#include "rocsversion.h"
#include "settings.h"

#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QGraphicsView>
#include <QLabel>
#include <QLayout>
#include <QSplitter>
#include <QStackedWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QToolButton>
#include <QPointer>
#include <QGridLayout>
#include <QFile>
#include <QDebug>
#include <QIcon>
#include <QPushButton>
#include <QInputDialog>
#include <QActionGroup>
#include <QMutexLocker>
#include <QFormLayout>
#include <QScriptEngineDebugger>
#include <QFileDialog>

#include <KActionCollection>
#include <KRecentFilesAction>
#include <KActionMenu>
#include <KTar>
#include <KMessageBox>
#include <KLocalizedString>
#include <KConfigDialog>
#include <KToolBar>
#include <KNS3/DownloadDialog>
#include <KNS3/UploadDialog>
#include <ktexteditor/view.h>
#include <ktexteditor/editor.h>
#include <ktexteditor/document.h>

#include "DocumentTypesWidget.h"
#include "GraphVisualEditor.h"
#include "Ui/CodeEditor.h"
#include "Ui/ScriptOutputWidget.h"
#include "Ui/PossibleIncludes.h"
// #include "Ui/LoadedPluginsDialog.h" //FIXME
#include "Ui/SideDockWidget.h"
#include "Ui/JournalEditorWidget.h"
#include "Ui/DocumentationWidget.h"
#include "InterfacePlugins/ApiDoc/ApiDocWidget.h"
#include "Scene/GraphicsLayout.h"
#include "Document.h"
#include "DataStructure.h"
#include "Project.h"
#include "QtScriptBackend.h"
#include "Actions/AbstractAction.h"
#include "Actions/AlignAction.h"
#include "Actions/ZoomAction.h"
#include "Actions/DeleteAction.h"
#include "Actions/PropertiesDialogAction.h"
#include "IncludeManagerSettings.h"
#include "ConfigureDefaultProperties.h"
#include "IncludeManager.h"
#include "ImporterExporterManager.h"
#include "DataStructureBackendInterface.h"
#include "DataStructureBackendManager.h"
#include "DocumentManager.h"
#include "Tools/ToolManager.h"


MainWindow::MainWindow()
    : KXmlGuiWindow()
    , _currentProject(0)
    , _scriptDbg(0)
{
    setObjectName("RocsMainWindow");

    setupWidgets();
    setupActions();
    setupGUI(ToolBar | Keys | Save | Create);

    setupToolbars();
    setupToolsPluginsAction();
    setupDSPluginsAction();

    connect(&DocumentManager::self(), SIGNAL(activateDocument()),
            this, SLOT(setActiveDocument()));
    connect(&DocumentManager::self(), SIGNAL(deactivateDocument(Document*)),
            this, SLOT(releaseDocument(Document*)));
    connect(&DocumentManager::self(), SIGNAL(documentRemoved(Document*)),
            this, SLOT(releaseDocument(Document*)));

    // TODO: use welcome widget instead of creating default empty project
    createNewProject();
    updateCaption();

    GraphicsLayout::self()->setViewStyleDataNode(Settings::dataNodeDisplay());
    GraphicsLayout::self()->setViewStyleDataEdge(Settings::dataEdgeDisplay());

    // update rocs config version
    Settings::setVersion(ROCS_VERSION_STRING);

    // disable save action from kpart, since we take care for the editor by global save action
    // here "file_save" is the action identifier from katepartui.rc
    // note that we may not use that name for our own actions
    foreach(KActionCollection* ac, KActionCollection::allCollections()) {
        if (ac->action("file_save")) {
            ac->removeAction(ac->action("file_save"));
            break; // we only expect that action once
        }
    }
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
    if (queryClose() == true) {
        event->accept();
        return;
    } else {
        event->ignore();
        return;
    }
}

void MainWindow::setupWidgets()
{
    // setup main widgets
    QWidget *sidePanel = setupSidePanel();
    _graphVisualEditor = GraphVisualEditor::self();
    QWidget *scriptPanel = setupScriptPanel();

    // splits the main window horizontal
    _vSplitter = new QSplitter(this);
    _vSplitter->setOrientation(Qt::Vertical);
    _vSplitter->addWidget(_graphVisualEditor);
    _vSplitter->addWidget(scriptPanel);

    // horizontal arrangement
    _hSplitter = new QSplitter(this);
    _hSplitter->setOrientation(Qt::Horizontal);
    _hSplitter->addWidget(_vSplitter);
    _hSplitter->addWidget(sidePanel);

    // set sizes for script panel
    _hScriptSplitter->setSizes(QList<int>() << Settings::hScriptSplitterSizeLeft() << Settings::hScriptSplitterSizeRight() << 80);

    // set sizes for vertical splitter
    _vSplitter->setSizes(QList<int>() << Settings::vSplitterSizeTop() << Settings::vSplitterSizeBottom());

    // set sizes for side panel
    // the following solves the setting of the panel width if it was closed at previous session
    int panelWidth = Settings::hSplitterSizeRight();
    if (panelWidth == 0) {
        //FIXME this is only a workaround
        // that fixes the wrong saving of hSplitterSizeRight
        panelWidth = 400;
    }
    _hSplitter->setSizes(QList<int>() << Settings::hSplitterSizeLeft() << panelWidth);

    setCentralWidget(_hSplitter);
}

void MainWindow::setupToolbars()
{
    // If current version in settings file is less than demanded version
    // perform operations.
    QString configVersion = Settings::version();
    if (configVersion.compare(QString("1.7.70")) < 0) {
        qDebug() << "Apply new default settings for toolbars";
        KToolBar* bar;

        bar = toolBar("main");
        bar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        bar->setOrientation(Qt::Vertical);
        addToolBar(Qt::LeftToolBarArea, bar);

        bar = toolBar("align");
        bar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        bar->setOrientation(Qt::Vertical);
        addToolBar(Qt::LeftToolBarArea, bar);
    }
}

void MainWindow::downloadNewExamples()
{
    QPointer<KNS3::DownloadDialog> dialog = new KNS3::DownloadDialog("rocs.knsrc", this);
    dialog->exec();
}

void MainWindow::uploadScript()
{

    QPointer<KNS3::UploadDialog> dialog = new KNS3::UploadDialog(this);

//First select the opened doc.
    QUrl str = _codeEditor->document()->url();
    if (str.isEmpty()) {
        //... then try to open
        str = QFileDialog::getOpenFileName(this, i18n("Rocs Script Files", QString(), i18n("*.js|Script files")));
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
    _outputWidget = new ScriptOutputWidget(this);
    _outputWidget->setConsoleInterface(new ConsoleModule(_outputWidget));

    KToolBar *executeCommands = new KToolBar(this);
    executeCommands->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    executeCommands->setOrientation(Qt::Vertical);
    _runScript = new QAction(QIcon::fromTheme("media-playback-start"), i18nc("@action:intoolbar Script Execution", "Run"), this);
    _stepRunScript = new QAction(QIcon::fromTheme("go-next"), i18nc("@action:intoolbar Script Execution", "One Step"), this);
    _stopScript = new QAction(QIcon::fromTheme("process-stop"), i18nc("@action:intoolbar Script Execution", "Stop"), this);
    _stopScript->setEnabled(false);
    executeCommands->addAction(_runScript);
    executeCommands->addAction(_stepRunScript);
    // add actions to action collection to be able to set shortcuts on them in the ui
    actionCollection()->addAction("_runScript", _runScript);
    actionCollection()->addAction("_stepRunScript", _stepRunScript);
    actionCollection()->addAction("_stopScript", _stopScript);

    // debug controls submenu
    _debugMenu = new KActionMenu(QIcon::fromTheme("debug-run"), i18nc("@title:menu Debug execution", "Debug"), this);
    _debugScript = new QAction(QIcon::fromTheme("debug-run"), i18nc("@action:inmenu Debug execution", "Debug run"), _debugMenu);
    _interruptScript = new QAction(QIcon::fromTheme("debug-run-cursor"), i18nc("@action:inmenu Debug execution", "Interrupt at first line"), _debugMenu);
    _debugMenu->addAction(_debugScript);
    _debugMenu->addAction(_interruptScript);
    executeCommands->addWidget(_debugMenu->createWidget(executeCommands));
    executeCommands->addAction(_stopScript);
    actionCollection()->addAction("_debugScript", _debugScript);
    actionCollection()->addAction("_interruptScript", _interruptScript);

    // set toolbar visibility defaults
    showExecutionButtonDebug(Settings::executionModeDebugVisible());
    showExecutionButtonOneStep(Settings::executionModeOneStepVisible());

    connect(_runScript, SIGNAL(triggered()), this, SLOT(executeScriptFull()));
    connect(_stepRunScript, SIGNAL(triggered()), this, SLOT(executeScriptOneStep()));
    connect(_debugScript, SIGNAL(triggered()), this, SLOT(debugScript()));
    connect(_interruptScript, SIGNAL(triggered()), this, SLOT(debugScript()));
    connect(_stopScript, SIGNAL(triggered()), this, SLOT(stopScript()));

    _hScriptSplitter->addWidget(_codeEditor);
    _hScriptSplitter->addWidget(_outputWidget);

    QWidget *scriptInterface = new QWidget(this);
    scriptInterface->setLayout(new QHBoxLayout);
    scriptInterface->layout()->addWidget(_hScriptSplitter);
    scriptInterface->layout()->addWidget(executeCommands);

    return scriptInterface;
}

QWidget* MainWindow::setupSidePanel()
{
    QWidget *panel = new QWidget(this);
    panel->setLayout(new QVBoxLayout);
    panel->setVisible(false);

    // add sidebar
    SideDockWidget* sideDock = new SideDockWidget(panel);
    addToolBar(Qt::RightToolBarArea, sideDock->toolbar());
    panel->layout()->addWidget(sideDock);

    // add widgets to dock
    // document property widgets
    DocumentTypesWidget* documentTypesWidget = new DocumentTypesWidget(this);
    connect(&DocumentManager::self(), SIGNAL(activateDocument()), documentTypesWidget, SLOT(updateDocument()));
    sideDock->addDock(documentTypesWidget, i18n("Element Types"), QIcon::fromTheme("document-properties"));

    // Project Journal
    _journalWidget = new JournalEditorWidget(panel);
    sideDock->addDock(_journalWidget, i18nc("@title", "Journal"), QIcon::fromTheme("story-editor"));

    // Rocs handbook
    DocumentationWidget* documentation = new DocumentationWidget(panel);
    sideDock->addDock(documentation, i18nc("@title", "Handbook"), QIcon::fromTheme("help-contents"));

    // Rocs scripting API documentation
//FIXME commented out until Grantlee is ported
//     ApiDocWidget* apiDoc = new ApiDocWidget(panel);
//     sideDock->addDock(apiDoc, i18nc("@title", "Scripting API"), QIcon::fromTheme("documentation"));

    return panel;
}

void MainWindow::setupActions()
{
    qDebug() << "create and connect actions";
    KStandardAction::quit(this, SLOT(quit()), actionCollection());
    KStandardAction::preferences(this, SLOT(showSettings()), actionCollection());

    // setup graph visual editor actions and add them to mainwindow action collection
    _graphVisualEditor->setupActions(actionCollection());

    // Menu actions
    createAction("document-new",        i18nc("@action:inmenu", "New Project"),        "new-project", QKeySequence::New, SLOT(createNewProject()), this);
    createAction("document-save",       i18nc("@action:inmenu", "Save Project"),       "save-project", QKeySequence::Save, SLOT(saveProject()), this);
    createAction("document-open",       i18nc("@action:inmenu", "Open Project"),       "open-project", QKeySequence::Open, SLOT(openProject()), this);

    _recentProjects = new KRecentFilesAction(QIcon ("document-open"), i18nc("@action:inmenu","Recent Projects"), this);
    connect(_recentProjects, SIGNAL(urlSelected(QUrl)), this, SLOT(openProject(QUrl)));
    actionCollection()->addAction("recent-project", _recentProjects);

    _recentProjects->loadEntries(Settings::self()->config()->group("RecentFiles"));
    createAction("document-save-as",     i18nc("@action:inmenu", "Save Project as"),   "save-project-as",    SLOT(saveProjectAs()), this);
    createAction("document-new",        i18nc("@action:inmenu", "New Graph Document"), "new-graph",         SLOT(newGraph()), this);
    createAction("document-new",        i18nc("@action:inmenu", "New Script File"),    "new-script",        SLOT(newScript()),    this);
    createAction("document-import",     i18nc("@action:inmenu", "Import Graph"),       "import-graph",      SLOT(importGraphFile()),   this);
//     createAction("document-save",       i18nc("@action:inmenu", "Save Graph"),         "save-graph",        SLOT(saveGraph()),   this);
    createAction("document-export",     i18nc("@action:inmenu", "Export Graph as"),    "export-graph-as",      SLOT(exportGraphFile()), this);
    createAction("get-hot-new-stuff",   i18nc("@action:inmenu", "Download Examples"),  "download",          SLOT(downloadNewExamples()),  this);
    createAction("get-hot-new-stuff",   i18nc("@action:inmenu", "Upload project"),     "upload",            SLOT(uploadScript()),  this);

    createAction("help-hint", i18nc("@action:inmenu", "Possible Includes"),          "possible_includes",   SLOT(showPossibleIncludes()), this);
    createAction("document-import",  i18nc("@action:inmenu", "Import Script"),       "add-script",          SLOT(importScript()),   this);
//     createAction("document-save",    i18nc("@action:inmenu", "Save Script"),         "save-script",         SLOT(saveActiveScript()),   _codeEditor);
    createAction("document-export", i18nc("@action:inmenu", "Export Script as"),      "export-script-as",      SLOT(saveActiveScriptAs()), _codeEditor);
    createAction("",  i18nc("@action:inmenu", "Loaded Plugins"),      "loaded-plugins",      SLOT(showLoadedPlugins()), this);
    createAction("",  i18nc("@action:inmenu", "Configure Code Editor..."),      "config-code-editor",      SLOT(showCodeEditorConfig()), this);
}

void MainWindow::createAction(const QByteArray& iconName, const QString& actionTitle, const QString& actionName,
                              const QKeySequence& shortcut, const char* slot, QObject *parent)
{
    QAction* action = new QAction(QIcon::fromTheme(iconName), actionTitle, parent);
    action->setShortcut(shortcut);
    action->setShortcutContext(Qt::ApplicationShortcut);
    actionCollection()->addAction(actionName, action);
    connect(action, SIGNAL(triggered(bool)), parent, slot);
}

void MainWindow::createAction(const QByteArray& iconName, const QString& actionTitle, const QString& actionName,
                              const char* slot, QObject *parent)
{
    QAction* action = new QAction(QIcon::fromTheme(iconName), actionTitle, parent);
    actionCollection()->addAction(actionName, action);
    connect(action, SIGNAL(triggered(bool)), parent, slot);
}

void MainWindow::showSettings()
{
    QPointer<KConfigDialog> dialog = new KConfigDialog(this, "settings", Settings::self());

    IncludeManagerSettings * set = new IncludeManagerSettings(dialog);
    ConfigureDefaultProperties * defaultProperties = new ConfigureDefaultProperties(dialog);

    dialog->addPage(set, i18nc("@title:tab", "Include Manager"), QString(), i18nc("@title:tab", "Include Manager"), true);
    dialog->addPage(defaultProperties, i18nc("@title:tab", "Default Settings"), QString(), i18nc("@title:tab", "Default Settings"), true);

    // TODO port include manager to kcfg_ elements and remove signals
    // since then everything is handled by KConfigDialog
    connect(set,               SIGNAL(changed(bool)), dialog, SLOT(enableButtonApply(bool)));
    connect(dialog, SIGNAL(applyClicked()),   set, SLOT(saveSettings()));
    connect(dialog, SIGNAL(okClicked()),      set, SLOT(saveSettings()));

    connect(defaultProperties, SIGNAL(showExecuteModeDebugChanged(bool)),
            this, SLOT(showExecutionButtonDebug(bool)));
    connect(defaultProperties, SIGNAL(showExecuteModeOneStepChanged(bool)),
            this, SLOT(showExecutionButtonOneStep(bool)));

    dialog->exec();
}

void MainWindow::setupToolsPluginsAction()
{
    if (_toolsPlugins.isEmpty()) {
        createToolsPluginsAction();
    }

    foreach(QAction* action, _toolsPlugins) {
        ToolsPluginInterface *plugin = ToolManager::self().plugins().at(action->data().toInt());
        action->setEnabled(
                DocumentManager::self().activeDocument() &&
                plugin->supportedDataStructures().contains(DocumentManager::self().activeDocument()->backend()->internalName())
                          );
    }
}

void MainWindow::createToolsPluginsAction(){
    QAction *action = 0;
    QList<ToolsPluginInterface*> availablePlugins =  ToolManager::self().plugins();
    int count = 0;
    foreach(ToolsPluginInterface * plugin, availablePlugins) {
        action = new QAction(plugin->displayName(), this);
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
    QActionGroup* group = new QActionGroup(this);
    QStringList backends = DataStructureBackendManager::self().backends();

    // reset values
    QAction* action = 0;
    unplugActionList("DS_plugins");
    // create actions and associate them to signal mapper
    QSignalMapper* mapper = new QSignalMapper(this);
    foreach(const QString& identifier, backends) {
        DataStructureBackendInterface *plugin = DataStructureBackendManager::self().backend(identifier);
        action = new QAction(plugin->name(), this);
        action->setCheckable(true);
        if (plugin->internalName() == DataStructureBackendManager::self().activeBackend()->internalName()) {
            action->setChecked(true);
        }
        action->setActionGroup(group);

        connect(action, SIGNAL(triggered(bool)), mapper, SLOT(map()));
        mapper->setMapping(action, identifier);
        pluginList.append(action);
    }
    connect(mapper, SIGNAL(mapped(QString)),
            &DataStructureBackendManager::self(), SLOT(setBackend(QString)));

    plugActionList("DS_plugins", pluginList);
}

void MainWindow::setupDocumentsList()
{
    QList<QAction*> pluginList;
    QAction* action = 0;
    unplugActionList("Doc_List");
    QActionGroup * group = new QActionGroup(this);
    int count = 0;
    foreach(Document * doc, DocumentManager::self().documentList()) {
        action = new QAction(doc->name(), this);
        action->setData(count++);
        action->setCheckable(true);
        if (doc == DocumentManager::self().activeDocument()) {
            action->setChecked(true);
        }
        action->setActionGroup(group);
        connect(action, SIGNAL(triggered(bool)), &DocumentManager::self(), SLOT(changeDocument()));
        pluginList.append(action);
    }

    plugActionList("Doc_List", pluginList);
}

void MainWindow::setActiveDocument()
{
    qDebug() << "Setting the document in the main window";
    Document *activeDocument = DocumentManager::self().activeDocument();

    // create engine and interface objects to UI plugins
    QtScriptBackend *engine = activeDocument->engineBackend();
    engine->engine();

    // finally set active
    _graphVisualEditor->setActiveDocument();

    // Update engine toolbar
    connect(engine, SIGNAL(finished()), this, SLOT(disableStopAction()));

    activeDocument->setModified(false);
    setupToolsPluginsAction();
}

void MainWindow::releaseDocument(Document* d)
{
    if (d == 0) {
        return;
    }
    d->disconnect(this);
    disconnect(d);

    d->engineBackend()->stop();
    d->engineBackend()->disconnect(this);
    _graphVisualEditor->releaseDocument();
}

void MainWindow::addEmptyGraphDocument()
{
    _currentProject->addGraphFileNew(
        DocumentManager::self().newDocument()
    );
}

void MainWindow::importScript()
{
    QUrl startDirectory = Settings::lastOpenedDirectory();
    if (!_currentProject->isTemporary()) {
        startDirectory = QUrl::fromLocalFile(_currentProject->projectDirectory());
    }

    QString fileUrl = QFileDialog::getOpenFileName(this,
                        i18nc("@title:window", "Add Existing Script File to Project"),
                        startDirectory.toLocalFile());
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

    DocumentManager::self().openDocument(QUrl::fromLocalFile(name));
}

void MainWindow::createNewProject()
{
    if (!queryClose()) {
        return;
    }

    _codeEditor->closeAllScripts();
    DocumentManager::self().closeAllDocuments();

    delete _currentProject;
    _currentProject = new Project();
    _currentProject->addCodeFileNew(_codeEditor->newScript());
    _currentProject->addGraphFileNew(DocumentManager::self().newDocument());
    _journalWidget->openJournal(_currentProject);
    _currentProject->setModified(false);

    updateCaption();
}

void MainWindow::newProjectAssistant()
{
    //TODO needs to be implemented
}

void MainWindow::saveProject(bool saveAs)
{
    // save graphs and scripts

    QUrl startDirectory = Settings::lastOpenedDirectory();
    if (_currentProject->isTemporary() || saveAs) {
        startDirectory = QUrl::fromLocalFile(_currentProject->projectDirectory());
        QString file = QFileDialog::getSaveFileName(this,
                            i18nc("@title:window", "Save Project"),
                            startDirectory.toLocalFile(),
                            i18n("*.rocs|Rocs project files\n*.rocsz|Compressed Rocs project files\n*|All files"));

        if (file.isEmpty()) {
            qDebug() << "Filename is empty and no script file was created.";
            return;
        }
        _currentProject->setProjectFile(QUrl::fromLocalFile(file));
        saveAllGraphs();
        saveScripts();
        _journalWidget->saveJournal();
        if (file.endsWith(QLatin1String("rocsz"))){
            _currentProject->exportProject(QUrl::fromLocalFile(file));
        }else {

            // we need to set project directory first to allow correcte relative paths
            // save files and finally write project to file
            _currentProject->writeProjectFile(file);
        }
    } else {
        saveAllGraphs();
        saveScripts();
        _journalWidget->saveJournal();
        if (_currentProject->projectFile().fileName().endsWith(QLatin1String("rocsz"))){
            _currentProject->exportProject(_currentProject->projectFile());
        }else{
            _currentProject->writeProjectFile();
        }
    }
    updateCaption();
    Settings::setLastOpenedDirectory(_currentProject->projectFile().path());
}

void MainWindow::saveProjectAs()
{
    saveProject(true);

    // add project to recently opened projects
    _recentProjects->addUrl(_currentProject->projectFile().toLocalFile());
}

void MainWindow::openProject(const QUrl& fileName)
{
    if (!queryClose()) {
        return;
    }

    QUrl startDirectory = Settings::lastOpenedDirectory();
    QUrl file = fileName;
    if (file.isEmpty()){
    // show open dialog
         file = QFileDialog::getOpenFileName(this,
                    i18nc("@title:window", "Open Project Files"),
                    startDirectory.toLocalFile(),
                    i18n("*rocs *.rocsz|All Rocs files\n*.rocs|Rocs project files\n*.rocsz|Compressed Rocs project files\n*|All files"));

        if (file.isEmpty()) {
            return;
        }
    }
    // import project specified: close everything and delete old project
    DocumentManager::self().activeDocument()->engineBackend()->stop();
    DocumentManager::self().closeAllDocuments();
    _codeEditor->closeAllScripts();
    delete _currentProject;

    // extract and open new project
    // at the end of this _currentProject must exist
    if (file.fileName().endsWith(QLatin1String("rocsz"), Qt::CaseInsensitive)){
        _currentProject = new Project(file); //FIXME correctly load unpacked project
        foreach(const QUrl& graphFile, _currentProject->graphFiles()) {
            DocumentManager::self().openDocument(graphFile);
        }
    } else {
        _currentProject = new Project(file);
        foreach(const QUrl& graphFile, _currentProject->graphFiles()) {
            DocumentManager::self().openDocument(graphFile);
        }
    }
    if (_currentProject->graphFiles().count() == 0) {
        _currentProject->addGraphFileNew(DocumentManager::self().newDocument());
    }
    foreach(const QUrl& codeFile, _currentProject->codeFiles()) {
        _codeEditor->openScript(codeFile);
        //TODO set curser line
    }
    if (_currentProject->codeFiles().count() == 0) {
        _currentProject->addCodeFileNew(_codeEditor->newScript());
    }
    _journalWidget->openJournal(_currentProject);

    updateCaption();
    _recentProjects->addUrl(file.path(QUrl::FullyDecoded));
    Settings::setLastOpenedDirectory(file.path());
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

QString MainWindow::uniqueFilename(const QString &basePrefix, const QString &suffix) {
    QFile targetFile;
    QString basePath = _currentProject->projectDirectory();
    QString fullSuffix = "." + suffix;
    QString fullPrefix = basePrefix;

    if (fullPrefix.isNull()) {
        fullPrefix = _currentProject->projectFile().fileName().remove(QRegExp(".rocsz*$"));
    } else if (fullPrefix.endsWith(fullSuffix)) {
        fullPrefix.remove(QRegExp(fullSuffix + "$"));
    }

    targetFile.setFileName(basePath + fullPrefix + fullSuffix);
    for(int i = 1; targetFile.exists(); i++) {
        targetFile.setFileName(basePath + fullPrefix + QString::number(i) + fullSuffix);
    }

    return targetFile.fileName();
}

void MainWindow::saveScripts()
{
    foreach (KTextEditor::Document * textDocument, _currentProject->codeFilesNew()) {
        QString basePrefix = QInputDialog::getText(this,
                                i18n("ScriptName"),
                                i18n("Enter the name of your new script"));
        QString fileName = uniqueFilename(basePrefix, "js");

        textDocument->saveAs(QUrl::fromLocalFile(fileName));
        _currentProject->saveCodeFileNew(textDocument, fileName);
    }

    _codeEditor->saveAllScripts();
}

void MainWindow::newScript()
{
    if (!_currentProject->isTemporary()) {
        QString basePrefix = QInputDialog::getText(this,
                                i18n("ScriptName"),
                                i18n("Enter the name of your new script"));
        if (basePrefix.isNull()) {
            qDebug() << "Filename is empty and no script file was created.";
        } else {
            QString fileName = uniqueFilename(basePrefix, "js");

            KTextEditor::Document *document = _codeEditor->newScript(QUrl::fromLocalFile(fileName));
            _currentProject->addCodeFileNew(document);
            _currentProject->saveCodeFileNew(document, QUrl::fromLocalFile(fileName));
        }
    } else {
        saveProject(true);
        if (!_currentProject->isTemporary()) {
            newScript();
        }
    }
}

void MainWindow::saveGraph(Document* document)
{
    if (document == 0) {
        document = DocumentManager::self().activeDocument();
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
    foreach(Document * document, DocumentManager::self().documentList()) {
        if (document->fileUrl().isEmpty()) {
            _currentProject->saveGraphFileAs(document,
                                             _currentProject->projectFile().toLocalFile().remove(QRegExp(".rocsz*$"))
                                                                                                              + ".graph");
        } else if (document->isModified()) {
            document->save();
        }
    }
}

void MainWindow::saveGraphAs()
{
    saveGraphAs(DocumentManager::self().activeDocument());
}


void MainWindow::saveGraphAs(Document* document)
{
    if (document == 0) {
        document = DocumentManager::self().activeDocument();
    }
    Q_ASSERT(document);
    QString file = QFileDialog::getSaveFileName(this,
                        i18nc("@title:window", "Save Graph Document"),
                        QString(),
                        i18n("*.graph|Rocs graph documents\n*|All files"));
    _currentProject->saveGraphFileAs(document, file);
}

void MainWindow::newGraph()
{
    QString file = QInputDialog::getText(this,
                        i18n("Graph name"),
                        i18n("Enter the name of the Graph"));
    if (file.isEmpty()) {
        qDebug() << "Filename is empty and no script file was created.";
        return;
    }
    if (!file.endsWith(QLatin1String(".graph"))){
        file.append(".graph");
    }
    DocumentManager::self().openDocument(QUrl::fromLocalFile(file));
    _currentProject->addGraphFile(file);
}

bool MainWindow::queryClose()
{
    if (!_currentProject) {
        return true;
    }

    //TODO move modification information to project
    bool anyGraphDocumentModified = false;
    foreach(Document * document, DocumentManager::self().documentList()) {
        if (document->isModified()) {
            anyGraphDocumentModified = true;
            break;
        }
    }

    if (_currentProject->isModified()
        || _journalWidget->isModified()
        || anyGraphDocumentModified
        || _codeEditor->isModified())
    {
        const int btnCode = KMessageBox::warningYesNoCancel(this, i18nc(
                                "@info",
                                "Changes on your project are unsaved. Do you want to save your changes?"));
        if (btnCode == KMessageBox::Yes) {
            saveProject();
            return true;
        }
        if (btnCode == KMessageBox::No) {
            return true;
        }
        // do not close
        return false;
    }
    // save to close project: no changes
    return true;
}

void MainWindow::quit()
{
    if (queryClose()) {
        QApplication::quit();
    }
}

void MainWindow::importGraphFile()
{
    ImporterExporterManager importer(this);
    Document * gd = importer.importFile();
    if (gd == 0) {
        return;
    }

    DocumentManager::self().addDocument(gd);

    if (importer.hasDialog()) {
        importer.dialogExec();
    }
}

void MainWindow::exportGraphFile()
{
    ImporterExporterManager exp(this);

    exp.exportFile(DocumentManager::self().activeDocument());
}

void MainWindow::showPossibleIncludes()
{
    QPointer<PossibleIncludes> dialog = new PossibleIncludes(this);
    dialog->exec();
}

void MainWindow::showLoadedPlugins()
{
    //FIXME commented out for porting
//     QPointer<LoadedPluginsDialog> dialog = new LoadedPluginsDialog(this);
//     dialog->exec();
}

void MainWindow::showCodeEditorConfig()
{
    KTextEditor::Editor *editor = _codeEditor->editor();
    editor->configDialog(this);
}

void MainWindow::runToolPlugin()
{
    QAction *action = qobject_cast<QAction *> (sender());

    if (! action) {
        return;
    }
    if (ToolsPluginInterface *plugin =  ToolManager::self().plugins().value(action->data().toInt())) {
        plugin->run(DocumentManager::self().activeDocument());
    }
}

void MainWindow::executeScriptFull(const QString& text)
{
    executeScript(MainWindow::Execute, text);
}

void MainWindow::executeScript(const MainWindow::ScriptMode mode, const QString& text)
{
    Q_ASSERT(_outputWidget);
    if (_outputWidget->isOutputClearEnabled()) {
        _outputWidget->clear();
    }

    // set script
    QString script = text.isEmpty() ? _codeEditor->text() : text;
    QString scriptPath = _codeEditor->document()->url().path();

    // prepare engine
    QtScriptBackend *engine = DocumentManager::self().activeDocument()->engineBackend();
    if (engine->isRunning()) {
        engine->stop();
    }

    // set console
    // TODO this should part of a plugin interface to for setting up all engine modules
    engine->registerGlobalObject(_outputWidget->consoleInterface(), "Console");

    connect(engine, SIGNAL(scriptError(QString)), _outputWidget->consoleInterface(), SLOT(error(QString)));
    connect(engine, SIGNAL(scriptInfo(QString)), _outputWidget->consoleInterface(), SLOT(log(QString)));
    connect(engine, SIGNAL(sendDebug(QString)), _outputWidget->consoleInterface(), SLOT(debug(QString)));
    connect(engine, SIGNAL(sendOutput(QString)), _outputWidget->consoleInterface(), SLOT(log(QString)));

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

    engine->setScript(script, DocumentManager::self().activeDocument());
    engine->execute();

    // disconnect console listener
    engine->disconnect(_outputWidget->consoleInterface());
}

void MainWindow::executeScriptOneStep(const QString& text)
{
    Q_ASSERT(_outputWidget);

    QtScriptBackend *engine = DocumentManager::self().activeDocument()->engineBackend();

    //TODO disable start action
    enableStopAction();
    if (!engine->isRunning()) {
        if (_outputWidget->isOutputClearEnabled()) {
            _outputWidget->clear();
        }
        QString script = text.isEmpty() ? _codeEditor->text() : text;
        QString scriptPath = _codeEditor->document()->url().path();
        IncludeManager inc;

        script = inc.include(script,
                             scriptPath.isEmpty() ? scriptPath : scriptPath.section('/', 0, -2),
                             _codeEditor->document()->documentName());

        engine->setScript(script, DocumentManager::self().activeDocument());
        engine->executeStep();
        return;
    }
    engine->continueExecutionStep();
}

void MainWindow::stopScript()
{
    QtScriptBackend *engine = DocumentManager::self().activeDocument()->engineBackend();
    disableStopAction();
    engine->stop();
}

void MainWindow::debugScript()
{
    QAction *action = qobject_cast<QAction *> (sender());
    if (action == _interruptScript) {
        executeScript(DebugMode);
    } else {
        executeScript(DebugOnlyInCaseOfError);
    }
}

void MainWindow::enableStopAction()
{
    _stopScript->setEnabled(true);
}

void MainWindow::disableStopAction()
{
    _stopScript->setEnabled(false);
}

void MainWindow::showExecutionButtonDebug(bool visible)
{
    _debugMenu->setVisible(visible);
}

void MainWindow::showExecutionButtonOneStep(bool visible)
{
    _stepRunScript->setVisible(visible);
}
