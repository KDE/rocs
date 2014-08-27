/*
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>

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

#include "mainwindow.h"
#include "rocsversion.h"
#include "settings.h"

#include "libgraphtheory/editor.h"
#include "libgraphtheory/editorplugins/editorpluginmanager.h"
#include "libgraphtheory/kernel/kernel.h"
#include "libgraphtheory/view.h"

#include <QApplication>
#include <QCloseEvent>
#include <QGraphicsView>
#include <QLabel>
#include <QLayout>
#include <QSplitter>
#include <QToolButton>
#include <QGridLayout>
#include <QDebug>
#include <QIcon>
#include <QPushButton>
#include <QInputDialog>
#include <QActionGroup>
#include <QFileDialog>
#include <QQuickWidget>
#include <QPointer>

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

#include "ui/documenttypeswidget.h"
#include "ui/configuredefaultproperties.h"
#include "ui/codeeditor.h"
#include "ui/scriptoutputwidget.h"
#include "ui/sidedockwidget.h"
#include "ui/documentationwidget.h"
#include "ui/fileformatdialog.h"
#include "ui/journalwidget.h"
#include "plugins/ApiDoc/ApiDocWidget.h"
#include "project/project.h"

using namespace GraphTheory;

MainWindow::MainWindow()
    : KXmlGuiWindow()
    , m_currentProject(0)
    , m_kernel(new Kernel)
    , m_graphEditorWidget(new QWidget)
    , m_scriptDbg(0)
{
    setObjectName("RocsMainWindow");
    m_graphEditor = new GraphTheory::Editor();

    setupWidgets();
    setupActions();
    setupGUI(ToolBar | Keys | Save | Create);

    setupToolbars();
    setupToolsPluginsAction();

    // setup kernel
    connect(m_kernel, SIGNAL(message(QString, GraphTheory::Kernel::MessageType)), m_outputWidget,
        SLOT(processMessage(QString,GraphTheory::Kernel::MessageType)));

    // TODO: use welcome widget instead of creating default empty project
    createProject();
    updateCaption();

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
    Settings::setVSplitterSizeTop(m_vSplitter->sizes() [0]);
    Settings::setVSplitterSizeBottom(m_vSplitter->sizes() [1]);
    Settings::setHSplitterSizeLeft(m_hSplitter->sizes() [0]);
    Settings::setHSplitterSizeRight(m_hSplitter->sizes() [1]);
    Settings::setHScriptSplitterSizeLeft(m_hScriptSplitter->sizes() [0]);
    Settings::setHScriptSplitterSizeRight(m_hScriptSplitter->sizes() [1]);
    m_recentProjects->saveEntries(Settings::self()->config()->group("RecentFiles"));

    Settings::self()->save();

    m_graphEditor->deleteLater();
    m_kernel->deleteLater();
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
    QWidget *scriptPanel = setupScriptPanel();
    m_graphEditorWidget->setLayout(new QGridLayout);

    // splits the main window horizontal
    m_vSplitter = new QSplitter(this);
    m_vSplitter->setOrientation(Qt::Vertical);
    m_vSplitter->addWidget(m_graphEditorWidget);
    m_vSplitter->addWidget(scriptPanel);

    // horizontal arrangement
    m_hSplitter = new QSplitter(this);
    m_hSplitter->setOrientation(Qt::Horizontal);
    m_hSplitter->addWidget(m_vSplitter);
    m_hSplitter->addWidget(sidePanel);

    // set sizes for script panel
    m_hScriptSplitter->setSizes(QList<int>() << Settings::hScriptSplitterSizeLeft() << Settings::hScriptSplitterSizeRight() << 80);

    // set sizes for vertical splitter
    m_vSplitter->setSizes(QList<int>() << Settings::vSplitterSizeTop() << Settings::vSplitterSizeBottom());

    // set sizes for side panel
    // the following solves the setting of the panel width if it was closed at previous session
    int panelWidth = Settings::hSplitterSizeRight();
    if (panelWidth == 0) {
        //FIXME this is only a workaround
        // that fixes the wrong saving of hSplitterSizeRight
        panelWidth = 400;
    }
    m_hSplitter->setSizes(QList<int>() << Settings::hSplitterSizeLeft() << panelWidth);

    setCentralWidget(m_hSplitter);
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
    QUrl str = m_codeEditor->activeDocument()->url();
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

    dialog->setUploadName(m_codeEditor->activeDocument()->documentName());
    dialog->setDescription(i18n("Add your description here."));

    dialog->exec();

//Remove compressed file..
    QDir::temp().remove(local);
}


QWidget* MainWindow::setupScriptPanel()
{
    m_hScriptSplitter = new QSplitter(this);
    m_hScriptSplitter->setOrientation(Qt::Horizontal);

    m_codeEditor = new CodeEditor(this);
    m_outputWidget = new ScriptOutputWidget(this);

    KToolBar *executeCommands = new KToolBar(this);
    executeCommands->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    executeCommands->setOrientation(Qt::Vertical);
    m_runScript = new QAction(QIcon::fromTheme("media-playback-start"), i18nc("@action:intoolbar Script Execution", "Run"), this);
    m_stepRunScript = new QAction(QIcon::fromTheme("go-next"), i18nc("@action:intoolbar Script Execution", "One Step"), this);
    m_stopScript = new QAction(QIcon::fromTheme("process-stop"), i18nc("@action:intoolbar Script Execution", "Stop"), this);
    m_stopScript->setEnabled(false);
    executeCommands->addAction(m_runScript);
    executeCommands->addAction(m_stepRunScript);
    // add actions to action collection to be able to set shortcuts on them in the ui
    actionCollection()->addAction("_runScript", m_runScript);
    actionCollection()->addAction("_stepRunScript", m_stepRunScript);
    actionCollection()->addAction("_stopScript", m_stopScript);

    // debug controls submenu
    m_debugMenu = new KActionMenu(QIcon::fromTheme("debug-run"), i18nc("@title:menu Debug execution", "Debug"), this);
    m_debugScript = new QAction(QIcon::fromTheme("debug-run"), i18nc("@action:inmenu Debug execution", "Debug run"), m_debugMenu);
    m_interruptScript = new QAction(QIcon::fromTheme("debug-run-cursor"), i18nc("@action:inmenu Debug execution", "Interrupt at first line"), m_debugMenu);
    m_debugMenu->addAction(m_debugScript);
    m_debugMenu->addAction(m_interruptScript);
    executeCommands->addWidget(m_debugMenu->createWidget(executeCommands));
    executeCommands->addAction(m_stopScript);
    actionCollection()->addAction("_debugScript", m_debugScript);
    actionCollection()->addAction("_interruptScript", m_interruptScript);

    // set toolbar visibility defaults
    showExecutionButtonDebug(Settings::executionModeDebugVisible());
    showExecutionButtonOneStep(Settings::executionModeOneStepVisible());

    connect(m_runScript, SIGNAL(triggered()), this, SLOT(executeScript()));
    connect(m_stepRunScript, SIGNAL(triggered()), this, SLOT(executeScriptOneStep()));
    connect(m_stopScript, SIGNAL(triggered()), this, SLOT(stopScript()));

    m_hScriptSplitter->addWidget(m_codeEditor);
    m_hScriptSplitter->addWidget(m_outputWidget);

    QWidget *scriptInterface = new QWidget(this);
    scriptInterface->setLayout(new QHBoxLayout);
    scriptInterface->layout()->addWidget(m_hScriptSplitter);
    scriptInterface->layout()->addWidget(executeCommands);

    return scriptInterface;
}

QWidget* MainWindow::setupSidePanel()
{
    QWidget *panel = new QWidget(this);
    panel->setLayout(new QVBoxLayout);
    panel->setVisible(false);

    // add sidebar
    SidedockWidget* sideDock = new SidedockWidget(panel);
    addToolBar(Qt::RightToolBarArea, sideDock->toolbar());
    panel->layout()->addWidget(sideDock);

    // add widgets to dock
    // document property widgets
    DocumentTypesWidget *documentTypesWidget = new DocumentTypesWidget(panel);
    connect(this, SIGNAL(graphDocumentChanged(GraphTheory::GraphDocumentPtr)), documentTypesWidget, SLOT(setDocument(GraphTheory::GraphDocumentPtr)));
    sideDock->addDock(documentTypesWidget, i18n("Element Types"), QIcon::fromTheme("document-properties"));
    if (m_currentProject && m_currentProject->activeGraphDocument()) {
        documentTypesWidget->setDocument(m_currentProject->activeGraphDocument());
    }

    // Project Journal
    m_journalWidget = new JournalEditorWidget(panel);
    sideDock->addDock(m_journalWidget, i18nc("@title", "Journal"), QIcon::fromTheme("story-editor"));

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
//     m_graphEditor->setupActions(actionCollection()); //FIXME add editor actions to main action collection

    // Menu actions
    createAction("document-new",        i18nc("@action:inmenu", "New Project"),        "new-project", QKeySequence::New, SLOT(createProject()), this);
    createAction("document-save",       i18nc("@action:inmenu", "Save Project"),       "save-project", QKeySequence::Save, SLOT(saveProject()), this);
    createAction("document-open",       i18nc("@action:inmenu", "Open Project"),       "open-project", QKeySequence::Open, SLOT(openProject()), this);

    m_recentProjects = new KRecentFilesAction(QIcon ("document-open"), i18nc("@action:inmenu","Recent Projects"), this);
    connect(m_recentProjects, SIGNAL(urlSelected(QUrl)), this, SLOT(openProject(QUrl)));
    actionCollection()->addAction("recent-project", m_recentProjects);

    m_recentProjects->loadEntries(Settings::self()->config()->group("RecentFiles"));
    createAction("document-save-as",     i18nc("@action:inmenu", "Save Project as"),   "save-project-as",    SLOT(saveProjectAs()), this);
    createAction("document-new",        i18nc("@action:inmenu", "New Graph Document"), "new-graph",         SLOT(newGraph()), this);
    createAction("document-new",        i18nc("@action:inmenu", "New Script File"),    "new-script",        SLOT(newScript()),    this);
    createAction("document-import",     i18nc("@action:inmenu", "Import Graph"),       "import-graph",      SLOT(importGraphFile()),   this);
//     createAction("document-save",       i18nc("@action:inmenu", "Save Graph"),         "save-graph",        SLOT(saveGraph()),   this);
    createAction("document-export",     i18nc("@action:inmenu", "Export Graph as"),    "export-graph-as",      SLOT(exportGraphFile()), this);
    createAction("get-hot-new-stuff",   i18nc("@action:inmenu", "Download Examples"),  "download",          SLOT(downloadNewExamples()),  this);
    createAction("get-hot-new-stuff",   i18nc("@action:inmenu", "Upload project"),     "upload",            SLOT(uploadScript()),  this);

    createAction("document-import",  i18nc("@action:inmenu", "Import Script"),       "add-script",          SLOT(importCodeDocument()),   this);
    createAction("document-export", i18nc("@action:inmenu", "Export Script"),      "export-script",      SLOT(exportCodeDocument()), this);
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
    ConfigureDefaultProperties * defaultProperties = new ConfigureDefaultProperties(dialog);

    dialog->addPage(defaultProperties, i18nc("@title:tab", "Default Settings"), QString(), i18nc("@title:tab", "Default Settings"), true);

    connect(defaultProperties, SIGNAL(showExecuteModeDebugChanged(bool)),
            this, SLOT(showExecutionButtonDebug(bool)));
    connect(defaultProperties, SIGNAL(showExecuteModeOneStepChanged(bool)),
            this, SLOT(showExecutionButtonOneStep(bool)));
    dialog->exec();
}

void MainWindow::setupToolsPluginsAction()
{
    QAction *action = 0;
    QList<EditorPluginInterface*> availablePlugins =  m_graphEditorPluginManager.plugins();
    QList<QAction*> actions;
    int count = 0;
    foreach(EditorPluginInterface * plugin, availablePlugins) {
        action = new QAction(plugin->displayName(), this);
        action->setData(count++);
        connect(action, SIGNAL(triggered(bool)), this, SLOT(showEditorPluginDialog()));

        actions << action;
    }
    unplugActionList("tools_plugins");
    plugActionList("tools_plugins", actions);
}

void MainWindow::setActiveGraphDocument()
{
    Q_ASSERT(m_currentProject);
    if (!m_currentProject) {
        return;
    }
    GraphDocumentPtr activeDocument = m_currentProject->activeGraphDocument();
    m_graphEditorWidget->layout()->addWidget(activeDocument->createView(this));
    emit graphDocumentChanged(activeDocument);

    //TODO reenable after porting script engine
    // Update engine toolbar
//     connect(engine, SIGNAL(finished()), this, SLOT(disableStopAction()));
}

void MainWindow::importCodeDocument()
{
    QUrl startDirectory = Settings::lastOpenedDirectory();

    QString fileUrl = QFileDialog::getOpenFileName(this,
                        i18nc("@title:window", "Import Script into Project"),
                        startDirectory.toLocalFile());

    if (fileUrl.isEmpty()) {
        return;
    }

    m_currentProject->importCodeDocument(fileUrl);
    Settings::setLastOpenedDirectory(startDirectory.toLocalFile());
}

void MainWindow::exportCodeDocument()
{
    QUrl startDirectory = Settings::lastOpenedDirectory();

    QString fileUrl = QFileDialog::getSaveFileName(this,
                        i18nc("@title:window", "Export Script"),
                        startDirectory.toLocalFile(),
                       i18n("JavaScript (*.js)"));
    m_codeEditor->activeDocument()->saveAs(QUrl::fromLocalFile(fileUrl));
}

void MainWindow::createProject()
{
    if (!queryClose()) {
        return;
    }

    if (m_currentProject) {
        m_currentProject->disconnect(this);
        m_currentProject->deleteLater();
    }

    m_currentProject = new Project(m_graphEditor);
    m_currentProject->addCodeDocument(KTextEditor::Editor::instance()->createDocument(0));
    m_currentProject->addGraphDocument(m_graphEditor->createDocument());
    m_codeEditor->setProject(m_currentProject);
    m_journalWidget->openJournal(m_currentProject);
    setActiveGraphDocument();
    connect(m_currentProject, SIGNAL(activeGraphDocumentChanged()), this, SLOT(setActiveGraphDocument()));

    m_currentProject->setModified(false);

    updateCaption();
}

void MainWindow::saveProject(bool saveAs)
{
    // save graphs and scripts

    QUrl startDirectory = Settings::lastOpenedDirectory();
    if (m_currentProject->projectUrl().isEmpty() || saveAs) {
        QUrl startDirectory = Settings::lastOpenedDirectory();
        QString file = QFileDialog::getSaveFileName(this,
                            i18nc("@title:window", "Save Project"),
                            startDirectory.toLocalFile(),
                            i18n("Rocs Projects (*.rocs)"));

        if (file.isEmpty()) {
            qCritical() << "Filename is empty and no script file was created.";
            return;
        }
        Settings::setLastOpenedDirectory(m_currentProject->projectUrl().path());
        m_currentProject->setProjectUrl(QUrl::fromLocalFile(file));
    }
    m_currentProject->projectSave();
    updateCaption();
}

// void MainWindow::saveProjectAs()
// {
//     saveProject(true);
//
//     // add project to recently opened projects
//     m_recentProjects->addUrl(m_currentProject->projectFile().toLocalFile());
// }

void MainWindow::openProject(const QUrl &fileName)
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
                    i18n("Rocs projects (*.rocs)"));

        if (file.isEmpty()) {
            return;
        }
    }
    // import project specified: close everything and delete old project
    m_currentProject->disconnect(this);
    delete m_currentProject;

    // extract and open new project
    // at the end of this m_currentProject must exist
    m_currentProject = new Project(file, m_graphEditor);
    if (m_currentProject->graphDocuments().count() == 0) {
        m_currentProject->addGraphDocument(m_graphEditor->createDocument());
    }
    m_codeEditor->setProject(m_currentProject);
    m_journalWidget->openJournal(m_currentProject);

    updateCaption();
    setActiveGraphDocument();
    m_recentProjects->addUrl(file.path(QUrl::FullyDecoded));
    Settings::setLastOpenedDirectory(file.path());

    connect(m_currentProject, SIGNAL(activeGraphDocumentChanged()), this, SLOT(setActiveGraphDocument()));
}

void MainWindow::updateCaption()
{
    if (!m_currentProject) {
        return;
    }

    QString caption = "";
    if (!m_currentProject->name().isEmpty()) {
        caption.append(m_currentProject->name());
    }
    else if (m_currentProject->projectUrl().isEmpty()) {
        caption.append(i18nc("caption text for temporary project", "[ untitled ]"));
    } else {
        caption.append(m_currentProject->projectUrl().toLocalFile());
    }
    setCaption(caption);
}

QString MainWindow::uniqueFilename(const QString &basePrefix, const QString &suffix) {
    QFile targetFile;
    QString basePath = m_currentProject->projectUrl().path();
    QString fullSuffix = "." + suffix;
    QString fullPrefix = basePrefix;

    if (fullPrefix.isNull()) {
        fullPrefix = m_currentProject->projectUrl().fileName().remove(QRegExp(".rocs*$"));
    } else if (fullPrefix.endsWith(fullSuffix)) {
        fullPrefix.remove(QRegExp(fullSuffix + "$"));
    }

    targetFile.setFileName(basePath + fullPrefix + fullSuffix);
    for(int i = 1; targetFile.exists(); i++) {
        targetFile.setFileName(basePath + fullPrefix + QString::number(i) + fullSuffix);
    }

    return targetFile.fileName();
}

void MainWindow::newScript()
{
    QString basePrefix = QInputDialog::getText(this,
                            i18n("ScriptName"),
                            i18n("Enter the name of your new script"));
    if (basePrefix.isNull()) {
        qDebug() << "Filename is empty and no script file was created.";
    } else {
        QString fileName = uniqueFilename(basePrefix, "js"); //TODO this does nothing
        KTextEditor::Document *document = KTextEditor::Editor::instance()->createDocument(0);
        m_currentProject->addCodeDocument(document);
    }
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

    GraphDocumentPtr document = m_graphEditor->createDocument();
    m_currentProject->addGraphDocument(document);
    document->setDocumentName(file);
}

bool MainWindow::queryClose()
{
    if (!m_currentProject) {
        return true;
    }
    if (m_currentProject->isModified()) {
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
        return false; // do not close
    }
    return true; // save to close project: no changes
}

void MainWindow::quit()
{
    if (queryClose()) {
        QApplication::quit();
    }
}

void MainWindow::importGraphFile()
{
    //FIXME port to GraphTheory
#if 0
    //TODO port to GraphTheory library
    ImporterExporterManager importer(this);
    Document * gd = importer.importFile();
    if (gd == 0) {
        return;
    }

    DocumentManager::self().addDocument(gd);

    if (importer.hasDialog()) {
        importer.dialogExec();
    }
#endif
}

void MainWindow::exportGraphFile()
{
    FileFormatDialog exporter(this);
    exporter.exportFile(m_currentProject->activeGraphDocument());
}

void MainWindow::showCodeEditorConfig()
{
    KTextEditor::Editor *editor = KTextEditor::Editor::instance();
    editor->configDialog(this);
}

void MainWindow::showEditorPluginDialog()
{
    QAction *action = qobject_cast<QAction *> (sender());

    if (!action) {
        return;
    }
    if (EditorPluginInterface *plugin =  m_graphEditorPluginManager.plugins().value(action->data().toInt())) {
        plugin->showDialog(m_currentProject->activeGraphDocument());
    }
}

void MainWindow::executeScript()
{
    if (m_outputWidget->isOutputClearEnabled()) {
        m_outputWidget->clear();
    }
    QString script = m_codeEditor->activeDocument()->text();
    enableStopAction();
    m_kernel->execute(m_currentProject->activeGraphDocument(), script);
}

void MainWindow::executeScriptOneStep()
{
    //FIXME implement scripting interfaces for GraphTheory
    qCritical() << "Scripting engine currently disalbed";
#if 0
    Q_ASSERT(m_outputWidget);

    //FIXME implement scripting interfaces for GraphTheory
    qCritical() << "Scripting engine currently disalbed";
//     QtScriptBackend *engine = DocumentManager::self().activeDocument()->engineBackend();

    //TODO disable start action
    enableStopAction();
    if (!engine->isRunning()) {
        if (m_outputWidget->isOutputClearEnabled()) {
            m_outputWidget->clear();
        }
        QString script = text.isEmpty() ? m_codeEditor->text() : text;
        QString scriptPath = m_codeEditor->document()->url().path();
        IncludeManager inc;

        script = inc.include(script,
                             scriptPath.isEmpty() ? scriptPath : scriptPath.section('/', 0, -2),
                             m_codeEditor->document()->documentName());

        //FIXME implement scripting interfaces for GraphTheory
        qCritical() << "Scripting engine currently disalbed";
//         engine->setScript(script, DocumentManager::self().activeDocument());
        engine->executeStep();
        return;
    }
    engine->continueExecutionStep();
#endif
}

void MainWindow::stopScript()
{
    m_kernel->stop();
    disableStopAction();
}

void MainWindow::enableStopAction()
{
    m_stopScript->setEnabled(true);
}

void MainWindow::disableStopAction()
{
    m_stopScript->setEnabled(false);
}

void MainWindow::showExecutionButtonDebug(bool visible)
{
    m_debugMenu->setVisible(visible);
}

void MainWindow::showExecutionButtonOneStep(bool visible)
{
    m_stepRunScript->setVisible(visible);
}
