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
#include "libgraphtheory/editorplugins/editorpluginmanager.h"
#include <KXmlGuiWindow>
#include <QUrl>

namespace GraphTheory {
    class Kernel;
}

class KRecentFilesAction;
class QAction;
class Project;
class QSplitter;
class QAction;
class CodeEditor;
class QScriptEngineDebugger;
class QCloseEvent;
class KActionMenu;
class ScriptOutputWidget;
class JournalEditorWidget;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    /*! default constructor */
    MainWindow();
    ~MainWindow();

private:
    void setupWidgets();
    void setupActions();
    void setupToolbars();

    /**
     * Setup the information panel at the right side.
     *
     * \return created widget
     */
    QWidget* setupSidePanel();
    QWidget* setupScriptPanel(); // setup the panel with the editors and stuff

    void createAction(const QByteArray& iconName, const QString& actionTitle, const QString& actionName,
                      const QKeySequence & shortcut , const char* slot, QObject *parent);
    void createAction(const QByteArray& iconName, const QString& actionTitle, const QString& actionName,
                      const char* slot, QObject *parent);

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

private Q_SLOTS:
    void showSettings();
    //Testing to connect to KGHNS3
    void downloadNewExamples();
    void uploadScript();
    void showCodeEditorConfig();

    /**
     * Create an empty project configuration with one graph document and one code file and set as
     * current project. All files are temporary until saved.
     */
    void createProject();
    void saveProject(bool saveAs = false);
    void openProject(const QUrl& fileName = QUrl());
//     void saveProjectAs(); //TODO update

    /**
     * Import dialog to add graph document to project.
     * Imported graph document is directly set as active document.
     */
    void importScript();

    /**
     * Update window caption according to current project.
     */
    void updateCaption();

    void newGraph();
    void newScript();

    /**
     * Import dialog to add graph document to project.
     * Imported graph document is directly set as active document.
     */
    void importGraphFile();
    void exportGraphFile();

    void setupToolsPluginsAction();
    void quit();

private Q_SLOTS: // script execution
    /**
     * starts simulation kernel with currently active script and graph document.
     */
    void executeScript();
    /**
     * execute script until next "step" command is found in script
     */
    void executeScriptOneStep();
    /**
     * stop script execution
     */
    void stopScript();

public Q_SLOTS:
    /**
     * Update UI for changed active graph document
     */
    void setActiveGraphDocument();
    void showEditorPluginDialog();
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

Q_SIGNALS:
    /** emitted when the currently active document (of the active project) changes */
    void graphDocumentChanged(GraphTheory::GraphDocumentPtr document);

private:
    Project *m_currentProject;

    // Right Area:
    GraphTheory::Editor *m_graphEditor;
    GraphTheory::Kernel *m_kernel; //!< simulation kernel
    GraphTheory::EditorPluginManager m_graphEditorPluginManager;
    CodeEditor *m_codeEditor;
    ScriptOutputWidget *m_outputWidget;
    JournalEditorWidget *m_journalWidget;

    // Other Bunch of stuff.
    QAction *m_runScript;
    QAction *m_stepRunScript;
    QAction *m_stopScript;
    KActionMenu *m_debugMenu;

    ///Store the recent files.
    KRecentFilesAction *m_recentProjects;

    //! Needed to restore the size of the splitter after closing / opening the UI.
    QSplitter *m_vSplitter;
    QSplitter *m_hSplitter;
    QSplitter *m_hScriptSplitter;
    QWidget *m_graphEditorWidget; // contains graph editor scene

    QScriptEngineDebugger *m_scriptDbg;
    QAction* m_debugScript;
    QAction* m_interruptScript;

    void createToolsPluginsAction();
};

#endif
