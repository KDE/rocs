/*
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
    Copyright 2009-2011  Wagner Reck <wagner.reck@gmail.com>
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

#ifndef SUI_MAINWINDOW_H
#define SUI_MAINWINDOW_H

#include <KXmlGuiWindow>
#include <kmessagebox.h>
#include <QMutex>

class KPushButton;
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
    void openGraph();
    void saveGraph();
    void saveGraphAs();
    void saveAll();
    void loadDocument(const QString& name = QString());

    void importFile();
    void exportFile();
    void setupToolsPluginsAction();

    void dsChanged();

public slots:
    void setActiveDocument();
    void releaseDocument(Document *d);
    //void setActiveGraph(DataStructure *g);
    void runToolPlugin();
    void outputString(const QString& s);
    void debugString(const QString& s);

    /** setup documents list.*/
    void setupDocumentsList();
    void disableStopAction();
    void enableStopAction();
    /** opens debug output listing **/
    void showDebugOutput();
signals:
    void startEvaluation();
    void stopEvaluation();
    void runTool(ToolsPluginInterface * plugin, Document * graphs);

private: // Variables.
    Project* _currentProject;

    // Left Area:
    GraphLayers* _GraphLayers; //! Area where the data and edges will be modified.
    KPushButton *_btnAddGraph;

    // Right Area:
    GraphVisualEditor* _graphVisualEditor; //! Area where the graph will be editted.
    TabWidget *_bottomTabs;
    CodeEditor *_codeEditor;
    KTextBrowser *_txtDebug; //! this is the debug view.
    KTextBrowser *_txtOutput;

    // Other Bunch of stuff.
    KActionCollection *_paletteActions; //! the pallete actions, move, add, etc.
    KAction *_runScript;
    KAction *_stepRunScript;
    KAction *_stopScript;

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
};

#endif
