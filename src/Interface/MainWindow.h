/*
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
    Copyright 2009-2011  Wagner Reck <wagner.reck@gmail.com>
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

#ifndef SUI_MAINWINDOW_H
#define SUI_MAINWINDOW_H

#include <KXmlGuiWindow>
#include <kmessagebox.h>
#include <QMutex>
#include <KUrl>

class KRecentFilesAction;
class QAction;
class KPushButton;
class KComboBox;
class GraphLayers;
class GraphVisualEditor;
class KActionCollection;
class Document;
class DataStructure;
class Project;
class KTextBrowser;
class QSplitter;
class GraphScene;
class KAction;
class SelectMoveHandAction;
class TabWidget;
class CodeEditor;
class QScriptEngine;
class QScriptEngineDebugger;
class ToolsPluginInterface;
class QCloseEvent;
class QComboBox;
class QToolButton;
class KActionMenu;

#ifndef USING_QTSCRIPT
#define USING_QTSCRIPT 1
#endif

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT


public:
    /*! defalt constructor */
    MainWindow();
    ~MainWindow();

    GraphScene *scene() const;

    enum ScriptMode {
        Execute,
        DebugMode,
        DebugOnlyInCaseOfError
    };

private: // Methods
    void setupWidgets(); // Setup all the widgets.
    void setupActions(); // Setup all the actions.

    QWidget* setupWhiteboardPanel(); // Setup the left actionbar panel & related widgets..
    QWidget* setupScriptPanel(); // setup the panel with the editors and stuff
    int saveIfChanged();
    void finishLoadingUi();
    void createAction(const QByteArray& iconName, const QString& actionTitle, const QString& actionName,
                      const QKeySequence & shortcut , const char* slot, QObject *parent);
    void createAction(const QByteArray& iconName, const QString& actionTitle, const QString& actionName,
                      const char* slot, QObject *parent);

    /**
     * Create an empty project configuration with one graph document and one code file.
     * All files are temporary until saved.
     * \return project pointer
     */
    Project* createNewProject();

    /** setup menu with data structures plugins. */
    void setupDSPluginsAction();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void showSettings();
    //Testing to connect to KGHNS3
    void downloadNewExamples();
    void uploadScript();

    void showPossibleIncludes();

    void showLoadedPlugins();

    /**
     * execute the given script and print results
     * \param text string that will be printed at result shell
     */
    void executeScriptFull(const QString &text = QString());
    void executeScript(const ScriptMode mode, const QString &text = QString());

    void debugScript();

    /**
     * execute script until next "step" command is found in script
     * \param text string that will be printed at result shell
     */
    void executeScriptOneStep(const QString &text = QString());

    /**
     * stop script execution
     */
    void stopScript();

    /**
     * Create new empty graph document for this project.
     * This can be a new document or an existing one.
     * Graph document is added to project.
     */
    void addEmptyGraphDocument();

    /**
     * Updates selector combobox for graph documents.
     */
    void updateGraphDocumentList();
    void newProject(); //TODO maybe not needed anymore, check after finish of new project assistant
    void newProjectAssistant();
    void saveProject(const bool& saveAs = false);
    void openProject(const KUrl& fileName = KUrl());

    /**
     * Import dialog to add graph document to project.
     * Imported graph document is directly set as active document.
     */
    void importScript();

    /**
     * Update window caption according to current project.
     */
    void updateCaption();

    /**
     * Save currently active graph document.
     */
    void saveGraph();

    /**
     * Save graph document \p document if document is specified.
     * Otherwise if \p document is 0 save current active document.
     * \param docoument is reference to the to be saved graph document or 0
     */
    void saveGraph(Document* document);

    /**
     * Save all graphs currently registered by DocumentMananger
     */
    void saveAllGraphs();

    /**
     * Save graph currently active document under new file name.
     */
    void saveGraphAs();

    /**
     * Save graph document \p document under new file name if document is specified.
     * Otherwise if \p document is 0 save current active document under new file name.
     * \param docoument is reference to the to be saved graph document or 0
     */
    void saveGraphAs(Document* document);
    void newGraph();
    void saveScripts();
    void newScript();
    void loadDocument(const QString& name = QString());

    /**
     * Import dialog to add graph document to project.
     * Imported graph document is directly set as active document.
     */
    void importGraphFile();
    void exportGraphFile();

    /**
     * Import a new project from internal Rocs project file. This method guarantees that
     * when finishing a project, a graph document, and a script file exist.
     */
//     void importProject();

    void saveProjectAs();
    void setupToolsPluginsAction();

    /**
     * Update add data and add pointer actions at graphical editor toolbar
     */
    void updateToolbarTypeActions();

public slots:
    /**
     * Sets the current active document given by \see DocumentManager::activeDocument() as
     * active document. Use with case, since this method creates connections to the current Document.
     * If already one document is active, \see releaseDocument(d) must be called before this.
     */
    void setActiveDocument();
    void releaseDocument(Document *d);
    void runToolPlugin();
    void outputString(const QString& s);
    void debugString(const QString& s);

    /** setup documents list.*/
    void setupDocumentsList();
    void disableStopAction();
    void enableStopAction();
    /** opens debug output listing **/
    void showDebugOutput();

    /**
     * Show button to execute script in debug mode.
     * \param visible specifies whether button shall be shown or not
     */
    void showExecutionButtonDebug(bool visible);

    /**
     * Show button to exectue script for one command.
     * \param visible specifies whether button shall be shown or not
     */
    void showExecutionButtonOneStep(bool visible);

signals:
    void startEvaluation();
    void stopEvaluation();
    void runTool(ToolsPluginInterface * plugin, Document * graphs);

private: // Variables.
    Project* _currentProject;

    // Left Area:
    GraphLayers* _GraphLayers; //! Area where the data and edges will be modified.
    KComboBox* _documentSelectorCombo;
    KPushButton *_btnAddGraph;
    QToolButton* _documentPropertiesButton;

    // Right Area:
    GraphVisualEditor* _graphVisualEditor; //! Area where the graph will be editted.
    TabWidget *_bottomTabs;
    CodeEditor *_codeEditor;
    KTextBrowser *_txtDebug; //! this is the debug view.
    KTextBrowser *_txtOutput;

    // Other Bunch of stuff.
    KActionCollection *_paletteActions; //! the pallete actions, move, add, etc.
    KActionMenu *_addDataActionMenu;
    KActionMenu *_addPointerActionMenu;
    KAction *_runScript;
    KAction *_stepRunScript;
    KAction *_stopScript;
    KActionMenu *_debugMenu;
    QList<QAction*> _toolsPlugins;
    
    ///Store the recent files.
    KRecentFilesAction *_recentProjects;

    //! Needed to restore the size of the splitter after closing / opening the UI.
    QSplitter *_vSplitter;
    QSplitter *_hSplitter;
    QSplitter *_hScriptSplitter;

    QComboBox *_selectListing;

    //! all the actions that I need to reference later.
    SelectMoveHandAction *_selectMoveAction;

    QScriptEngineDebugger *_scriptDbg;
    KAction* _debugScript;
    KAction* _interruptScript;

    void createToolsPluginsAction();
};

#endif
