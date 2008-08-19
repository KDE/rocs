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

namespace KTextEditor{
class View;
class Editor;
class Document;
}

class MainWindow : public KXmlGuiWindow
{
Q_OBJECT

public:
  MainWindow();
  KTextEditor::Editor* editor();
  void changeActiveScript(KTextEditor::Document *item);
  void changeActiveGraph(GraphCollection *item);

public slots:
  //! #####################
  //! ##      MENUS      ##
  //! #####################

  //! File Menu

  //! Edit Menu

  //! Help Menu

  //! Script Menu

  void debug(const QString& s);

  void changeActive(QTreeWidgetItem * item);

protected slots:

signals:
  void sendDebugMsg(const QString& s);

private:
  //! Widgets being displayed
  PaletteBar*      _paletteBar;
  QGraphicsView*   _graphView;
  ScriptingArea*   _scriptingArea;
  FileArea*        _fileArea;
  PropertiesArea*  _propertiesArea;
  GraphLayers*     _graphLayers;
  ConfigureDialog* _configureDialog;
  QSplitter*       _centralWidget;
  QToolBox*        _rightToolBox;
  KTextEditor::Editor* _editor;
  QTreeWidgetItem *_activeGraphItem;
  QTreeWidgetItem *_activeScriptItem;
  
  void setupWidgets();
  void setupActions();

};

#endif
