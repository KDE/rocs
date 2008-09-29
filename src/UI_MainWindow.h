#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

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

#include <KXmlGuiWindow>
#include <QList>
#include <QStringList>

class PaletteBar;
class GraphScene;
class ScriptingArea;
class FileArea;
class PropertiesArea;
class GraphLayers;
class ConfigureDialog;
class QSplitter;
class Graph;
class QToolBox;
class QGraphicsView;
class QTreeWidgetItem;
class GraphCollection;

namespace KTextEditor
{
  class View;
  class Editor;
  class Document;
};

namespace libgraph
{
  class GraphDocument;
};

/*! 
\brief the main window of the program
This class is responsable for connecting all signals and stuff to all the other parts of the Gui
*/
class MainWindow : public KXmlGuiWindow
{
Q_OBJECT

public:
	/*! defalt constructor */
	MainWindow();
	

public slots:

signals:
	/*! send a message to the debug() method 
	\param s the message to be send
	*/
	void sendDebugMsg ( const QString& s );

private:
	//! Widgets being displayed
	PaletteBar*      _paletteBar; //! area where the icons of actions will be.
	QGraphicsView*   _graphView;  //! area where you can draw graphs.
	ScriptingArea*   _scriptingArea; //! area where you can create your scripts.
	FileArea*        _fileArea; 	//! area where the opened files will be
	PropertiesArea*  _propertiesArea;  //! *NOT IMPLEMENTED* edit the properties of the graphs, nodes and edges.
	GraphLayers*     _graphLayers;	//! *NOT IMPLEMENTED* area where the layers of the active graph will appear.
	ConfigureDialog* _configureDialog; //! *NOT IMPLEMENTED* the configure dialog.
	QSplitter*       _centralWidget; //! The container for the Script Area and GraphcsView
	QToolBox*        _rightToolBox;	//! The toolbox that will hold the the FileArea, Properties and stuff.
	KTextEditor::Editor* _editor;	//! The active editor.

	void setupWidgets(); //! Setup all the widgets
	void setupActions(); //! Setup all the actions.

};

#endif
