/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2008-2011 Tomaz Canabrava <tomaz.canabrava@gmail.com>
    SPDX-FileCopyrightText: 2008 Ugo Sangiori <ugorox@gmail.com>
    SPDX-FileCopyrightText: 2009-2011 Wagner Reck <wagner.reck@gmail.com>
    SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
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
class CodeEditorWidget;
class GraphEditorWidget;
class QCloseEvent;
class ScriptOutputWidget;
class JournalEditorWidget;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    /*! default constructor */
    MainWindow();
    ~MainWindow() override;

private:
    void setupWidgets();
    void setupActions();

    /**
     * Set @p project as active project. This method updates all responsible widgets.
     */
    void setProject(Project *project);

    /**
     * Setup the information panel at the right side.
     *
     * \return created widget
     */
    QWidget* setupSidePanel();
    QWidget* setupScriptPanel(); // setup the panel with the editors and stuff

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
    bool queryClose() override;
    void closeEvent(QCloseEvent *event) override;

private Q_SLOTS:
    void showConfigurationDialog();

    /**
     * Create an empty project configuration with one graph document and one code file and set as
     * current project. All files are temporary until saved.
     */
    void createProject();
    void saveProject();
    void saveProjectAs();

    // script file handling
    void tryToCreateCodeDocument();
    void importCodeDocument();
    void exportCodeDocument();

    // graph file handling
    void createGraphDocument();
    /**
     * Import dialog to add graph document to project.
     * Imported graph document is directly set as active document.
     */
    void importGraphDocument();
    void exportGraphDocument();

    /**
     * Update window caption according to current project.
     */
    void updateCaption();
    void setupToolsPluginsAction();
    void quit();

public Q_SLOTS:
    void openProject(const QUrl& fileName = QUrl());

private Q_SLOTS: // script execution
    /**
     * starts simulation kernel with currently active script and graph document.
     */
    void executeScript();
    /**
     * stop script execution
     */
    void stopScript();
    /**
     * enable/disable the debugger
     **/
    void checkDebugger();

public Q_SLOTS:
    void showEditorPluginDialog();
    void disableStopAction();
    void enableStopAction();

Q_SIGNALS:
    /** emitted when the currently active document (of the active project) changes */
    void graphDocumentChanged(GraphTheory::GraphDocumentPtr document);

private:
    Project *m_currentProject;

    // Right Area:
    GraphTheory::Editor *m_graphEditor;
    GraphTheory::Kernel *m_kernel; //!< simulation kernel
    GraphTheory::EditorPluginManager m_graphEditorPluginManager;
    CodeEditorWidget *m_codeEditorWidget;
    GraphEditorWidget *m_graphEditorWidget;
    ScriptOutputWidget *m_outputWidget;
    JournalEditorWidget *m_journalWidget;

    // Other Bunch of stuff.
    QAction *m_runScript;
    QAction *m_stopScript;
    QAction *m_openDebugger;

    ///Store the recent files.
    KRecentFilesAction *m_recentProjects;

    //! Needed to restore the size of the splitter after closing / opening the UI.
    QSplitter *m_vSplitter;
    QSplitter *m_hSplitter;
    QSplitter *m_hScriptSplitter;

    void createToolsPluginsAction();
};

#endif
