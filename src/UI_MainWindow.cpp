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

#include <QSplitter>
#include <QToolBox>
#include <KLocale>
#include <QGraphicsView>
#include <QTreeWidgetItem>
#include <KAction>
#include <KStandardAction>
#include <KActionCollection>

#include <ktexteditor/editor.h>
#include <ktexteditor/editorchooser.h>

#include "UI_MainWindow.h"   // The Main Window
#include "UI_PaletteBar.h"       // Left Side Toolbox Pallete.
#include "UI_GraphScene.h"       // Plot-Area
#include "UI_ScriptingArea.h"    // Script-Area
#include "UI_FileArea.h"         // handling of opened files
#include "UI_PropertiesArea.h"   // handling the properties of the graphs and stuff.
#include "UI_GraphLayers.h"      // Control of the Graph Layers.
#include "UI_ConfigureDialog.h"  // Configuration of the app

#include "libgraph_GraphDocument.h"

MainWindow::MainWindow() : KXmlGuiWindow()
{
  setObjectName ( "Rocs" );
  _editor = KTextEditor::EditorChooser::editor();
  _editor -> setSimpleMode ( true );

  _graphView       = new QGraphicsView 	 ( this );
  _paletteBar      = new PaletteBar 	 ( this );
  _scriptingArea   = new ScriptingArea 	 ( this );
  _propertiesArea  = new PropertiesArea  ( this );
  _graphLayers     = new GraphLayers 	 ( this );
  _configureDialog = new ConfigureDialog ( this );
  _centralWidget   = new QSplitter 	 ( this );
  _rightToolBox    = new QToolBox 	 ( this );
  _fileArea        = new FileArea 	 ( this );

  setCorner ( Qt::TopLeftCorner,     Qt::LeftDockWidgetArea );
  setCorner ( Qt::BottomLeftCorner,  Qt::LeftDockWidgetArea );
  setCorner ( Qt::TopRightCorner,    Qt::RightDockWidgetArea );
  setCorner ( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

  setupActions();
  setupWidgets();
  setupGUI();
}

void MainWindow::setupWidgets()
{
  addDockWidget ( Qt::LeftDockWidgetArea, _paletteBar );

  _rightToolBox->addItem ( _fileArea, 		i18n ( "Opened Files" 	) );
  _rightToolBox->addItem ( _propertiesArea, 	i18n ( "Properties" 	) );
  _rightToolBox->addItem ( _graphLayers, 	i18n ( "Graphs" 	) );

  QDockWidget *dockWidget = new QDockWidget ( 	i18n ( "Tool Box" ), this );
  dockWidget -> setObjectName ( "RightToolBox" );
  dockWidget->setWidget ( _rightToolBox );
  dockWidget->setFeatures ( QDockWidget::NoDockWidgetFeatures );
  addDockWidget ( Qt::RightDockWidgetArea, dockWidget );

  _centralWidget->setOrientation ( Qt::Vertical );
  _centralWidget->addWidget ( _graphView );
  _centralWidget->addWidget ( _scriptingArea );

  setCentralWidget ( _centralWidget );
}

void MainWindow::setupActions()
{
  KStandardAction::quit ( this,    SLOT ( quit() ),        actionCollection() );
}
