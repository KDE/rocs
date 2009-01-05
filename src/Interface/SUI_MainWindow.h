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
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef SUI_MAINWINDOW_H
#define SUI_MAINWINDOW_H

#include <KXmlGuiWindow>
#include <QList>
#include <QStringList>

/* WORKING */
class PaletteBarWidget;
class OpenedFilesWidget;

class GraphPropertiesWidget;
class GraphVisualEditor;
class GraphDocumentModel;
class KActionCollection;
class GraphDocument;
class Graph;
class KMultiTabBar;
class KTextBrowser;
class QSplitter;

namespace KTextEditor{
	class Document;
	class View;
}

class MainWindow : public KXmlGuiWindow{
Q_OBJECT

public:
  /*! defalt constructor */
  MainWindow();
  ~MainWindow();
private:
  void setupModels();  // setup all the models to be used on the views.
  void setupWidgets(); // Setup all the widgets.
  void setupActions(); // Setup all the actions.
  void setupSignals();

	QWidget* setupLeftPanel(); // Setup the left actionbar panel & related widgets..
	QWidget* setupRightPanel(); // setup the Right area of the  panel, the GraphicsView, the editors and stuff.
	
	// Left Area:
  PaletteBarWidget*  _PaletteBar;   // area where the icons of actions will be.
  OpenedFilesWidget* _OpenedFiles;  // area where the opened files will be
  GraphPropertiesWidget* _GraphProperties; // Area where the nodes and edges will be modified.
  KMultiTabBar *_leftTabBar; // Tab that will hold the pallete, opened files, graphLayers and Properties.
	int _leftTabId;

	// Right Area:
  GraphVisualEditor* _graphVisualEditor; // Area where the graph will be editted.
	KMultiTabBar *_bottomTabBar; // This will hold the Editor and the Debugger.
	KTextEditor::View *_docView;
	KTextBrowser *_txtDebug;
  int _rightTabId;

	// Other Bunch of stuff.
	QList<GraphDocument*> _documents;
  GraphDocumentModel *_documentModel;
  KActionCollection  *_paletteActions;

	KTextEditor::Document *_scriptDoc;

	// Needed to reestore the size of the splitter after closing / opening the UI.
	QSplitter *_vSplitter;
	QSplitter *_hSplitter;

private slots:
	void releaseLeftTabbarButton(int index); // control the flux of the left tabbar.
	void executeScript();

public slots:
  void setGraph(Graph *g);
};

#endif
