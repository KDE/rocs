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

class KPushButton;
class GraphLayers;
class GraphVisualEditor;
class KActionCollection;
class GraphDocument;
class Graph;
class KTextBrowser;
class QSplitter;
class GraphScene;
class KAction;
class MoveNodeAction;
class TabWidget;
class CodeEditor;

#ifndef USING_QTSCRIPT
#define USING_QTSCRIPT 1
#endif

class MainWindow : public KXmlGuiWindow {
    Q_OBJECT


public:
    /*! defalt constructor */
    MainWindow();
    ~MainWindow();
    Graph *graph() const;
    GraphScene *scene() const;
    void debug(const QString& s);
    GraphDocument *activeDocument() const;
private: // Methods
    void setupWidgets(); // Setup all the widgets.
    void setupActions(); // Setup all the actions.
    
    QWidget* setupLeftPanel(); // Setup the left actionbar panel & related widgets..
    QWidget* setupRightPanel(); // setup the Right area of the	panel, the GraphicsView, the editors and stuff
    void saveIfChanged();
private slots:
    void executeScript();

    void newGraph();
    void openGraph();
    void saveGraph();
    void saveGraphAs();
    
public slots:
    void setActiveGraphDocument(GraphDocument *d);
    void setActiveGraph(Graph *g);

private: // Variables.

    // Left Area:
    GraphLayers* _GraphLayers; //! Area where the nodes and edges will be modified.
    KPushButton *_btnAddGraph;

    // Right Area:
    GraphVisualEditor* _graphVisualEditor; //! Area where the graph will be editted.
    TabWidget *_bottomTabs;
    CodeEditor *_codeEditor;
    KTextBrowser *_txtDebug; //! this is the debug view.

    // Other Bunch of stuff.
    KActionCollection *_paletteActions; //! the pallete actions, move, add, etc.
    KAction *_runScript;

    //! Needed to reestore the size of the splitter after closing / opening the UI.
    QSplitter *_vSplitter;
    QSplitter *_hSplitter;

    //! The Active Graph.
    GraphDocument *_activeGraphDocument;
    Graph *_graph;

    //! all the actions that I need to reference later.
    MoveNodeAction *_moveNodeAction;
};

#endif
