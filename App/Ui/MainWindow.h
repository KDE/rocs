/*
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
    Copyright 2009-2011  Wagner Reck <wagner.reck@gmail.com>
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

#ifndef SUI_MAINWINDOW_H
#define SUI_MAINWINDOW_H

#include "libgraphtheory/editor.h"
#include <KXmlGuiWindow>
#include <kmessagebox.h>
#include <QMutex>
#include <QUrl>

class DocumentTypesWidget;
class KRecentFilesAction;
class QAction;
class QPushButton;
class KComboBox;
class GraphLayers;
class GraphVisualEditor;
class Document;
class DataStructure;
class Project;
class QSplitter;
class QAction;
class SelectMoveHandAction;
class CodeEditor;
class QScriptEngine;
class QScriptEngineDebugger;
class ToolsPluginInterface;
class QCloseEvent;
class QComboBox;
class QToolButton;
class KActionMenu;
class ScriptOutputWidget;
class JournalEditorWidget;

#ifndef USING_QTSCRIPT
#define USING_QTSCRIPT 1
#endif

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT


public:
    /*! default constructor */
    MainWindow();
    ~MainWindow();

    enum ScriptMode {
        Execute,
        DebugMode,
        DebugOnlyInCaseOfError
    };

private: // Methods
    void setupWidgets(); // Setup all the widgets.
    void setupActions(); // Setup all the actions.
    void setupToolbars();

    /**
     * Setup the information panel at the right side.
     *
     * \return created widget
     */
    QWidget* setupSidePanel();
    QWidget* setupScriptPanel(); // setup the panel with the editors and stuff

    void finishLoadingUi();
    void createAction(const QByteArray& iconName, const QString& actionTitle, const QString& actionName,
                      const QKeySequence & shortcut , const char* slot, QObject *parent);
    void createAction(const QByteArray& iconName, const QString& actionTitle, const QString& actionName,
                      const char* slot, QObject *parent);

    /** setup menu with data structures plugins. */
    void setupDSPluginsAction();

    /**
     * Returns an absolute and unique filename: /path-to-current-project/basePrefix.suffix
     * A number will follow basePrefix in the filename if there were other files
     * with that same name in the current project's directory.
     *
     * \return unique file path
     */
    QString uniqueFilename(const QString &basePrefix, const QString &suffix);

protected:
    /**
     * Reimplemented method KMainWindow::queryClose().
     */
    virtual bool queryClose();
    void closeEvent(QCloseEvent *event);

private slots:
    void showSettings();
    //Testing to connect to KGHNS3
    void downloadNewExamples();
    void uploadScript();

    void showPossibleIncludes();

    void showLoadedPlugins();

    void showCodeEditorConfig();

    /**
     * Create an empty project configuration with one graph document and one code file and set as
     * current project. All files are temporary until saved.
     */
    void createNewProject();

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

    void newProjectAssistant();
    void saveProject(bool saveAs = false);
    void openProject(const QUrl& fileName = QUrl());

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
     * Save graph document \p document if document is specified.
     */
    void saveGraph(GraphTheory::GraphDocumentPtr document);

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
    void saveGraphAs(GraphTheory::GraphDocumentPtr document);
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
    void quit();

public slots:
    /**
     * Sets the current active document given by \see DocumentManager::activeDocument() as
     * active document. Use with case, since this method creates connections to the current Document.
     * If already one document is active, \see releaseDocument(d) must be called before this.
     */
    void setActiveDocument();
    void releaseDocument(Document *d);
    void runToolPlugin();

    /** setup documents list.*/
    void setupDocumentsList();
    void disableStopAction();
    void enableStopAction();

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

private:
    Project *m_currentProject;

    // Right Area:
    GraphTheory::Editor *m_graphEditor;
    CodeEditor *_codeEditor;
    ScriptOutputWidget* _outputWidget;
    JournalEditorWidget* _journalWidget;

    // Other Bunch of stuff.
    QAction *_runScript;
    QAction *_stepRunScript;
    QAction *_stopScript;
    KActionMenu *_debugMenu;
    QList<QAction*> _toolsPlugins;

    ///Store the recent files.
    KRecentFilesAction *_recentProjects;

    //! Needed to restore the size of the splitter after closing / opening the UI.
    QSplitter *_vSplitter;
    QSplitter *_hSplitter;
    QSplitter *_hScriptSplitter;

    QComboBox *_selectListing;

    QScriptEngineDebugger *_scriptDbg;
    QAction* _debugScript;
    QAction* _interruptScript;

    void createToolsPluginsAction();
};

#endif
