/* This file is part of Rocs,
	 Copyright (C) 2008 by:
	 Tomaz Canabrava <tomaz.canabrava@gmail.com>
	 Ugo Sangiori <ugorox@gmail.com>

	 Rocs is free software; you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation; either version 2 of the License, or
	 (at your option) any later version.

	 Rocs is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with Step; if not, write to the Free Software
	 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
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
class GraphDocument;
class DataStructureBase;
class KTextBrowser;
class QSplitter;
class GraphScene;
class KAction;
class MoveNodeAction;
class TabWidget;
class CodeEditor;
class ThreadDocument;
class QScriptEngine;
class QScriptEngineDebugger;

#include <QWaitCondition>

namespace Rocs{
    class ToolsPluginInterface;
}

#ifndef USING_QTSCRIPT
#define USING_QTSCRIPT 1
#endif

class MainWindow : public KXmlGuiWindow {
    Q_OBJECT


public:
    /*! defalt constructor */
    MainWindow();
    ~MainWindow();
    DataStructureBase *graph() const;
    GraphScene *scene() const;
//    void debug(const QString& s);
    GraphDocument *activeDocument() const;
    QMutex& mutex();

private: // Methods
    void setupWidgets(); // Setup all the widgets.
    void setupActions(); // Setup all the actions.

    QWidget* setupLeftPanel(); // Setup the left actionbar panel & related widgets..
    QWidget* setupRightPanel(); // setup the Right area of the panel, the GraphicsView, the editors and stuff
    int saveIfChanged();
    void finishLoadingUi();

    void startThreadDocument();

    /** setup menu with data structures plugins. */
    void setupDSPluginsAction();


private slots:

     void showSettings();
    //Testing to connect to KGHNS3
    void downloadNewExamples();

    void showPossibleIncludes();

    void executeScript(const QString &text = QString());
    void newGraph();
    void openGraph();
    void saveGraph();
    void saveGraphAs();
    void loadDocument(const QString& name = QString());

    void importFile();
    void exportFile();
    void setupToolsPluginsAction();

    void dsChanged();

public slots:
    void setActiveGraphDocument(GraphDocument *d);
    void releaseDocument(GraphDocument *d);
    void setActiveGraph(DataStructureBase*g);
    void runToolPlugin();
    void outputString(const QString& s);
    void debugString(const QString& s);

    /** setup documents list.*/
    void setupDocumentsList();

  signals:
      void startDocument(const QString& s);
      void startEvaluation();
      void stopEvaluation();
      void endThreadDocument();

      void runTool(Rocs::ToolsPluginInterface * plugin, GraphDocument * graphs);
private: // Variables.

    // Left Area:
    GraphLayers* _GraphLayers; //! Area where the nodes and edges will be modified.
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

    //! Needed to restore the size of the splitter after closing / opening the UI.
    QSplitter *_vSplitter;
    QSplitter *_hSplitter;

    //! The Active Graph.

    //! all the actions that I need to reference later.
    MoveNodeAction *_moveNodeAction;

    QScriptEngineDebugger *_scriptDbg;

    //!threading and Document part
    ThreadDocument *_tDocument;
    QWaitCondition _waitForDocument;
    QMutex _mutex;
    bool _idle;
};

#endif
