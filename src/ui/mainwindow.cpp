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
#include <ktexteditor/configpage.h>
#include <ktexteditor/view.h>
#include <ktexteditor/editor.h>
#include <ktexteditor/document.h>

#include "ui/documenttypeswidget.h"
#include "ui/codeeditorwidget.h"
#include "ui/scriptoutputwidget.h"
#include "ui/sidedockwidget.h"
#include "ui/fileformatdialog.h"
#include "ui/journalwidget.h"
#include "grapheditorwidget.h"
#include "plugins/scriptapi/scriptapiwidget.h"
#include "project/project.h"

using namespace GraphTheory;

MainWindow::MainWindow()
    : KXmlGuiWindow()
    , m_currentProject(0)
    , m_kernel(new Kernel)
    , m_codeEditorWidget(new CodeEditorWidget(this))
    , m_graphEditorWidget(new GraphEditorWidget(this))
    , m_outputWidget(new ScriptOutputWidget(this))
{
    setObjectName("RocsMainWindow");
    m_graphEditor = new GraphTheory::Editor();

    setupWidgets();
    setupActions();
    setupGUI(Keys | Save | Create);

    setupToolsPluginsAction();

    // setup kernel
    connect(m_kernel, &Kernel::message, m_outputWidget, &ScriptOutputWidget::processMessage);

    // TODO: use welcome widget instead of creating default empty project
    createProject();
    updateCaption();

    // update rocs config version
    Settings::setVersion(ROCS_VERSION_STRING);

    // disable save action from kpart, since we take care for the editor by global save action
    // here "file_save" is the action identifier from katepartui.rc
    // note that we may not use that name for our own actions
    foreach(KActionCollection *ac, KActionCollection::allCollections()) {
        if (ac->action("file_save")) {
            ac->action("file_save")->setDisabled(true);
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
    } else {
        event->ignore();
    }
    return;
}

void MainWindow::setupWidgets()
{
    // setup main widgets
    QWidget *sidePanel = setupSidePanel();
    QWidget *scriptPanel = setupScriptPanel();

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
    m_hScriptSplitter->setSizes({
        Settings::hScriptSplitterSizeLeft(),
        Settings::hScriptSplitterSizeRight(),
        80
    });

    // set sizes for vertical splitter
    m_vSplitter->setSizes({
        Settings::vSplitterSizeTop(),
        Settings::vSplitterSizeBottom()
    });

    // set sizes for side panel
    // the following solves the setting of the panel width if it was closed at previous session
    int panelWidth = Settings::hSplitterSizeRight();
    if (panelWidth == 0) {
        //FIXME this is only a workaround
        // that fixes the wrong saving of hSplitterSizeRight
        panelWidth = 400;
    }

    m_hSplitter->setSizes({
        Settings::hSplitterSizeLeft(),
        panelWidth
    });

    setCentralWidget(m_hSplitter);
}

QWidget* MainWindow::setupScriptPanel()
{
    m_hScriptSplitter = new QSplitter(this);
    m_hScriptSplitter->setOrientation(Qt::Horizontal);

    KToolBar *executeCommands = new KToolBar(this);
    executeCommands->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    executeCommands->setOrientation(Qt::Vertical);
    m_runScript = new QAction(QIcon::fromTheme("media-playback-start"), i18nc("@action:intoolbar Script Execution", "Run"), this);
    m_runScript->setToolTip(i18nc("@info:tooltip", "Execute currently active script on active graph document."));
    m_stopScript = new QAction(QIcon::fromTheme("process-stop"), i18nc("@action:intoolbar Script Execution", "Stop"), this);
    m_stopScript->setToolTip(i18nc("@info:tooltip", "Stop script execution."));
    m_stopScript->setEnabled(false);
    m_openDebugger = new QAction(QIcon::fromTheme("system-run"), i18nc("@action:intoolbar Open Debugger", "Debugger"), this);
    m_openDebugger->setToolTip(i18nc("@info:tooltip", "Open the Javascript code debugger."));
    m_openDebugger->setCheckable(true);
    executeCommands->addAction(m_runScript);
    executeCommands->addAction(m_stopScript);
    executeCommands->addAction(m_openDebugger);
    // add actions to action collection to be able to set shortcuts on them in the ui
    actionCollection()->addAction("_runScript", m_runScript);
    actionCollection()->addAction("_stopScript", m_stopScript);
    actionCollection()->addAction("_openDebugger", m_openDebugger);

    connect(m_runScript, &QAction::triggered, this, &MainWindow::executeScript);
    connect(m_stopScript, &QAction::triggered, this, &MainWindow::stopScript);
    connect(m_openDebugger, &QAction::triggered, this, &MainWindow::checkDebugger);

    m_hScriptSplitter->addWidget(m_codeEditorWidget);
    m_hScriptSplitter->addWidget(m_outputWidget);

    QWidget *scriptInterface = new QWidget(this);
    scriptInterface->setLayout(new QHBoxLayout);
    scriptInterface->layout()->addWidget(m_hScriptSplitter);
    scriptInterface->layout()->addWidget(executeCommands);

    return scriptInterface;
}

QWidget* MainWindow::setupSidePanel()
{
    // add sidebar
    SidedockWidget* sideDock = new SidedockWidget(this);
    addToolBar(Qt::RightToolBarArea, sideDock->toolbar());

    // add widgets to dock
    // document property widgets
    DocumentTypesWidget *documentTypesWidget = new DocumentTypesWidget(this);
    connect(this, &MainWindow::graphDocumentChanged, documentTypesWidget, &DocumentTypesWidget::setDocument);
    sideDock->addDock(documentTypesWidget, i18n("Element Types"), QIcon::fromTheme("document-properties"));
    if (m_currentProject && m_currentProject->activeGraphDocument()) {
        documentTypesWidget->setDocument(m_currentProject->activeGraphDocument());
    }

    // Project Journal
    m_journalWidget = new JournalEditorWidget(this);
    sideDock->addDock(m_journalWidget, i18nc("@title", "Journal"), QIcon::fromTheme("story-editor"));

    // Rocs scripting API documentation
    ScriptApiWidget* apiDoc = new ScriptApiWidget(this);
    sideDock->addDock(apiDoc, i18nc("@title", "Scripting API"), QIcon::fromTheme("documentation"));

    return sideDock;
}

void MainWindow::setProject(Project *project)
{
    m_codeEditorWidget->setProject(project);
    m_graphEditorWidget->setProject(project);
    m_journalWidget->openJournal(project);
    updateCaption();

    if (m_currentProject) {
        m_currentProject->disconnect(this);
        m_currentProject->deleteLater();
    }

    connect(project, static_cast<void (Project::*)(GraphTheory::GraphDocumentPtr)>(&Project::activeGraphDocumentChanged),
        this, &MainWindow::graphDocumentChanged);
    connect(project, &Project::modifiedChanged,
        this, &MainWindow::updateCaption);
    m_currentProject = project;
    emit graphDocumentChanged(m_currentProject->activeGraphDocument());
}

void MainWindow::setupActions()
{
    KStandardAction::quit(this, SLOT(quit()), actionCollection());
    KStandardAction::preferences(this, SLOT(showConfigurationDialog()), actionCollection());

    // setup graph visual editor actions and add them to mainwindow action collection
//     m_graphEditor->setupActions(actionCollection()); //FIXME add editor actions to main action collection

    // Menu actions
    QAction *newProjectAction = new QAction(QIcon::fromTheme("document-new"), i18nc("@action:inmenu", "New Project"), this);
    newProjectAction->setShortcutContext(Qt::ApplicationShortcut);
    actionCollection()->addAction("new-project", newProjectAction);
    actionCollection()->setDefaultShortcut(newProjectAction, QKeySequence::New);
    connect(newProjectAction, &QAction::triggered, this, &MainWindow::createProject);

    QAction *projectSaveAction = new QAction(QIcon::fromTheme("document-save"), i18nc("@action:inmenu", "Save Project"), this);
    projectSaveAction->setShortcutContext(Qt::ApplicationShortcut);
    actionCollection()->addAction("save-project", projectSaveAction);
    actionCollection()->setDefaultShortcut(projectSaveAction, QKeySequence::Save);
    connect(projectSaveAction, &QAction::triggered, this, &MainWindow::saveProject);

    QAction *projectOpenAction = new QAction(QIcon::fromTheme("document-open"), i18nc("@action:inmenu", "Open Project"), this);
    projectOpenAction->setShortcutContext(Qt::ApplicationShortcut);
    actionCollection()->addAction("open-project", projectOpenAction);
    actionCollection()->setDefaultShortcut(projectOpenAction, QKeySequence::Open);
    connect(projectOpenAction, &QAction::triggered, this, [=] () { openProject(); });

    m_recentProjects = new KRecentFilesAction(QIcon ("document-open"), i18nc("@action:inmenu","Recent Projects"), this);
    connect(m_recentProjects, &KRecentFilesAction::urlSelected,
        this, &MainWindow::openProject);
    actionCollection()->addAction("recent-project", m_recentProjects);
    m_recentProjects->loadEntries(Settings::self()->config()->group("RecentFiles"));

    createAction("document-save-as",     i18nc("@action:inmenu", "Save Project as"),   "save-project-as",    SLOT(saveProjectAs()), this);
    createAction("document-new",        i18nc("@action:inmenu", "New Graph Document"), "new-graph",         SLOT(createGraphDocument()), this);
    createAction("document-new",        i18nc("@action:inmenu", "New Script File"),    "new-script",        SLOT(tryToCreateCodeDocument()),    this);
    createAction("document-import",     i18nc("@action:inmenu", "Import Graph"),       "import-graph",      SLOT(importGraphDocument()),   this);
    createAction("document-export",     i18nc("@action:inmenu", "Export Graph as"),    "export-graph-as",      SLOT(exportGraphDocument()), this);
    createAction("document-import",  i18nc("@action:inmenu", "Import Script"),       "add-script",          SLOT(importCodeDocument()),   this);
    createAction("document-export", i18nc("@action:inmenu", "Export Script"),      "export-script",      SLOT(exportCodeDocument()), this);
}

void MainWindow::createAction(const QByteArray& iconName, const QString& actionTitle, const QString& actionName,
                              const char* slot, QObject *parent)
{
    QAction* action = new QAction(QIcon::fromTheme(iconName), actionTitle, parent);
    actionCollection()->addAction(actionName, action);
    connect(action, SIGNAL(triggered(bool)), parent, slot);
}

void MainWindow::showConfigurationDialog()
{
    QPointer<KConfigDialog> dialog = new KConfigDialog(this, "settings", Settings::self());
    KTextEditor::Editor *editor = KTextEditor::Editor::instance();
    for (int index = 0; index < editor->configPages(); ++index) {
        KTextEditor::ConfigPage *page = editor->configPage(index, dialog);
        dialog->addPage(page,
            page->name(),
            page->icon().name(),
            page->fullName());
    }
    dialog->exec();
}

void MainWindow::setupToolsPluginsAction()
{
    QList<EditorPluginInterface*> availablePlugins =  m_graphEditorPluginManager.plugins();
    QList<QAction*> actions;
    int count = 0;
    for (auto plugin : availablePlugins) {
        QAction *action = new QAction(plugin->displayName(), this);
        action->setData(count++);
        connect(action, &QAction::triggered,
            this, &MainWindow::showEditorPluginDialog);
        actions << action;
    }
    unplugActionList("tools_plugins");
    plugActionList("tools_plugins", actions);
}

void MainWindow::importCodeDocument()
{
    QString startDirectory = Settings::lastOpenedDirectory();
    QUrl fileUrl = QUrl::fromLocalFile(QFileDialog::getOpenFileName(this,
        i18nc("@title:window", "Import Script into Project"),
        startDirectory));

    if (fileUrl.isEmpty()) {
        return;
    }

    m_currentProject->importCodeDocument(fileUrl);
    Settings::setLastOpenedDirectory(startDirectory);
}

void MainWindow::exportCodeDocument()
{
    QString startDirectory = Settings::lastOpenedDirectory();
    QUrl fileUrl = QUrl::fromLocalFile(QFileDialog::getSaveFileName(this,
        i18nc("@title:window", "Export Script"),
        startDirectory,
        i18n("JavaScript (*.js)")));
    m_codeEditorWidget->activeDocument()->saveAs(fileUrl);
}

void MainWindow::createProject()
{
    if (!queryClose()) {
        return;
    }

    Project *project = new Project(m_graphEditor);
    project->createCodeDocument(i18n("untitled"));
    project->addGraphDocument(m_graphEditor->createDocument());
    project->setModified(false);

    setProject(project);
}

void MainWindow::saveProject()
{
    if (m_currentProject->projectUrl().isEmpty()) {
        saveProjectAs();
        return;
    } else {
        m_currentProject->projectSave();
        m_recentProjects->addUrl(m_currentProject->projectUrl());
    }
    updateCaption();
}

void MainWindow::saveProjectAs()
{
    QString startDirectory = Settings::lastOpenedDirectory();
    QString file = QFileDialog::getSaveFileName(this,
                        i18nc("@title:window", "Save Project As"),
                        startDirectory,
                        i18n("Rocs Projects (*.rocs)"));

    if (file.isEmpty()) {
        qCritical() << "Filename is empty and no script file was created.";
        return;
    }
    QFileInfo fi(file);
    if (fi.exists()) {
        const int btnCode = KMessageBox::warningContinueCancel(
            this,
            i18nc("@info", "A file named \"%1\" already exists. Are you sure you want to overwrite it?", fi.fileName()),
            i18nc("@title:window", "Overwrite File?"),
            KStandardGuiItem::overwrite());
        if (btnCode == KMessageBox::Cancel) {
            return; // cancel saving
        }
    }
    Settings::setLastOpenedDirectory(m_currentProject->projectUrl().path());
    m_currentProject->projectSaveAs(QUrl::fromLocalFile(file));
    m_recentProjects->addUrl(QUrl::fromLocalFile(file));
    updateCaption();
}

void MainWindow::openProject(const QUrl &fileName)
{
    if (!queryClose()) {
        return;
    }

    QString startDirectory = Settings::lastOpenedDirectory();
    QUrl file = fileName;
    if (file.isEmpty()){
    // show open dialog
         file = QUrl::fromLocalFile(QFileDialog::getOpenFileName(this,
                    i18nc("@title:window", "Open Project Files"),
                    startDirectory,
                    i18n("Rocs projects (*.rocs)")));

        if (file.isEmpty()) {
            return;
        }
    }

    Project *project = new Project(file, m_graphEditor);
    setProject(project);
    m_recentProjects->addUrl(file);
    updateCaption();

    Settings::setLastOpenedDirectory(file.path());
}

void MainWindow::updateCaption()
{
    if (!m_currentProject) {
        return;
    }
    QString modified;
    if (m_currentProject->isModified()) {
        modified = '*';
    }

    if (m_currentProject->projectUrl().isEmpty()) {
        setCaption(i18nc("caption text for temporary project", "[ untitled ]%1", modified));
    } else {
        setCaption(QString("[ %1 ]%2").arg(m_currentProject->projectUrl().toLocalFile()).arg(modified));
    }
}

QString MainWindow::uniqueFilename(const QString &basePrefix, const QString &suffix) {
    QFile targetFile;
    QString basePath = m_currentProject->projectUrl().path();
    QString fullSuffix = '.' + suffix;
    QString fullPrefix = basePrefix;

    if (fullPrefix.isNull()) {
        fullPrefix = m_currentProject->projectUrl().fileName().remove(QRegExp(".rocs*$"));
    } else if (fullPrefix.endsWith(fullSuffix)) {
        fullPrefix.remove(QRegExp(fullSuffix + '$'));
    }

    targetFile.setFileName(basePath + fullPrefix + fullSuffix);
    for(int i = 1; targetFile.exists(); i++) {
        targetFile.setFileName(basePath + fullPrefix + QString::number(i) + fullSuffix);
    }

    return targetFile.fileName();
}

void MainWindow::tryToCreateCodeDocument()
{
    QString basePrefix = QInputDialog::getText(this,
                            i18n("ScriptName"),
                            i18n("Enter the name of your new script"));
    if (basePrefix.isNull()) {
        qDebug() << "Filename is empty and no script file was created.";
        return;
    }

    QString fullPath = m_currentProject->workingDir() + QLatin1Char('/') + basePrefix + QStringLiteral(".js");
    QFileInfo file(fullPath);
    if (file.exists()) {
        KMessageBox::error(this, i18n("File already exists."));
        return;
    }

    m_currentProject->createCodeDocument(basePrefix);
}

void MainWindow::createGraphDocument()
{
    GraphDocumentPtr document = m_graphEditor->createDocument();
    m_currentProject->addGraphDocument(document);
}

bool MainWindow::queryClose()
{
    if (!m_currentProject) {
        return true;
    }
    if (!m_currentProject->isModified()) {
        return true;
    }
    const int btnCode = KMessageBox::warningYesNoCancel(this, i18nc(
                            "@info",
                            "Changes on your project are unsaved. Do you want to save your changes?"));

    if (btnCode == KMessageBox::Cancel) {
        return false;
    }

    if (btnCode == KMessageBox::Yes) {
        saveProject();
    }

    return true;
}

void MainWindow::quit()
{
    if (queryClose()) {
        QApplication::quit();
    }
}

void MainWindow::importGraphDocument()
{
    FileFormatDialog importer(this);
    GraphDocumentPtr document = importer.importFile();
    if (!document) {
        qWarning() << "No graph document was imported.";
        return;
    }
    m_currentProject->addGraphDocument(document);
}

void MainWindow::exportGraphDocument()
{
    FileFormatDialog exporter(this);
    exporter.exportFile(m_currentProject->activeGraphDocument());
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
    QString script = m_codeEditorWidget->activeDocument()->text();
    enableStopAction();

    if (m_openDebugger->isChecked()) {
        m_kernel->triggerInterruptAction();
    }

    m_kernel->execute(m_currentProject->activeGraphDocument(), script);
}

void MainWindow::stopScript()
{
    m_kernel->stop();
    disableStopAction();
}

void MainWindow::checkDebugger()
{
    if (m_openDebugger->isChecked()) {
        m_kernel->attachDebugger();
    } else {
        m_kernel->detachDebugger();
    }
}

void MainWindow::enableStopAction()
{
    m_stopScript->setEnabled(true);
}

void MainWindow::disableStopAction()
{
    m_stopScript->setEnabled(false);
}
