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
#include <iostream>
#include <QSplitter>
#include <QToolBox>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KLocale>
#include <QGraphicsView>
#include <QTreeWidgetItem>
#include <KXMLGUIFactory>
#include <KMessageBox>
#include <ktexteditor/view.h>
#include <ktexteditor/editor.h>
#include <ktexteditor/editorchooser.h>

#include "UI_MainWindow.h"

#include "UI_PaletteBar.h"       // Left Side Toolbox Pallete.
#include "UI_GraphScene.h"       // Plot-Area
#include "UI_ScriptingArea.h"    // Script-Area
#include "UI_FileArea.h"         // handling of opened files
#include "UI_PropertiesArea.h"   // handling the properties of the graphs and stuff.
#include "UI_GraphLayers.h"      // Control of the Graph Layers.
#include "UI_ConfigureDialog.h"  // Configuration of the app

#include "UI_MainWindow.moc"     //! I *really* dislike this... 

MainWindow::MainWindow() : KXmlGuiWindow(),
  _graphView      (0),
  _paletteBar     (0),
  _scriptingArea  (0),
  _propertiesArea (0),
  _graphLayers    (0),
  _configureDialog(0),
  _centralWidget  (0),
  _rightToolBox   (0),
  _fileArea       (0)
{
  setObjectName("MainWindow");


  _editor = KTextEditor::EditorChooser::editor();
  if (!_editor) KMessageBox::error(this, i18n("A KDE text-editor component could not be found"));

   _editor -> setSimpleMode(true);

  _graphView       = new QGraphicsView(this);
  _paletteBar      = new PaletteBar(this);
  _scriptingArea   = new ScriptingArea(this);
  _propertiesArea  = new PropertiesArea(this);
  _graphLayers     = new GraphLayers(this);
  _configureDialog = new ConfigureDialog(this);
  _centralWidget   = new QSplitter(this);
  _rightToolBox    = new QToolBox(this);
  _fileArea        = new FileArea(this);

  setCorner(Qt::TopLeftCorner,     Qt::LeftDockWidgetArea);
  setCorner(Qt::BottomLeftCorner,  Qt::LeftDockWidgetArea);
  setCorner(Qt::TopRightCorner,    Qt::RightDockWidgetArea);
  setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

  setupActions();
  setupWidgets();
  setupGUI();

  _fileArea->createNewScript();
}

void MainWindow::setupWidgets()
{
   _paletteBar->setObjectName("PaletteBar");
   addDockWidget(Qt::LeftDockWidgetArea, _paletteBar);

   _rightToolBox->addItem(_fileArea, i18n("Opened Files"));
   _rightToolBox->addItem(_propertiesArea, i18n("Properties"));
   _rightToolBox->addItem(_graphLayers, i18n("Graphs"));
   QDockWidget *dockWidget = new QDockWidget(this);
   dockWidget->setObjectName("rightToolBox");
   dockWidget->setWidget(_rightToolBox);
   dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
   addDockWidget(Qt::RightDockWidgetArea, dockWidget);

   _centralWidget->setOrientation(Qt::Vertical);
   _centralWidget->addWidget(_graphView);
   _centralWidget->addWidget(_scriptingArea);
   setCentralWidget(_centralWidget);
}

void MainWindow::setupActions()
{
 // KStandardAction::quit(this,    SLOT(quit()),        actionCollection());

//! Only Activate those commented below when the graph view is active.
//   KStandardAction::undo(this,    SLOT(undo()),        actionCollection());
//   KStandardAction::redo(this,    SLOT(redo()),        actionCollection());
//   KStandardAction::cut(this,     SLOT(cut()),         actionCollection());
//   KStandardAction::copy(this,    SLOT(copy()),        actionCollection());
//   KStandardAction::paste(this,   SLOT(paste()),       actionCollection());
//   KStandardAction::save(this,    SLOT(save()),        actionCollection());
//   KStandardAction::saveAs(this,  SLOT(saveFileAs()),  actionCollection());
//   KStandardAction::openNew(this, SLOT(documentNew()), actionCollection());
//   KStandardAction::open(this,    SLOT(openFile()),    actionCollection());
//   KStandardAction::close(this,   SLOT(close()),       actionCollection());

  //! void closeAll();
//   KAction *tmpAction = new KAction(this);
//   tmpAction->setText(i18n("Close All"));
//   tmpAction->setIcon(KIcon("document-close-all"));
//   actionCollection()->addAction("CloseAll", tmpAction);
//   connect( tmpAction, SIGNAL(triggered()), this, SLOT(closeAll()) ); 

  //! void allignRight();
//   tmpAction = new KAction(this);
//   tmpAction->setText(i18n("Right"));
//   //tmpAction->setIcon(KIcon(""));
//   //tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
//   actionCollection()->addAction("AllignRight", tmpAction);
//   connect( tmpAction, SIGNAL(triggered()), this, SLOT(allignRight()) ); 

  //! void allignLeft();
//   tmpAction = new KAction(this);
//   tmpAction->setText(i18n("Left"));
//   //tmpAction->setIcon(KIcon("document-open"));
//   //tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
//   actionCollection()->addAction("AllignLeft", tmpAction);
//   connect( tmpAction, SIGNAL(triggered()), this, SLOT(allignLeft()) ); 

  //! void allignBottom();
//   tmpAction = new KAction(this);
//   tmpAction->setText(i18n("Bottom"));
//   //tmpAction->setIcon(KIcon("document-open"));
//   //tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
//   actionCollection()->addAction("AllignBottom", tmpAction);
//   connect( tmpAction, SIGNAL(triggered()), this, SLOT(allignBottom()) ); 

  //! void allignTop();
//   tmpAction = new KAction(this);
//   tmpAction->setText(i18n("Top"));
//   //tmpAction->setIcon(KIcon("document-open"));
//   //tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
//   actionCollection()->addAction("AllignTop", tmpAction);
//   connect( tmpAction, SIGNAL(triggered()), this, SLOT(allignTop()) ); 

  //! void allignVCenter();
//   tmpAction = new KAction(this);
//   tmpAction->setText(i18n("Vertical Center"));
//   //tmpAction->setIcon(KIcon("document-open"));
//   //tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
//   actionCollection()->addAction("AllignVCenter", tmpAction);
//   connect( tmpAction, SIGNAL(triggered()), this, SLOT(allignVCenter()) ); 

  //! void allignHCenter();
//   tmpAction = new KAction(this);
//   tmpAction->setText(i18n("Horizontal Center"));
//   // tmpAction->setIcon(KIcon("document-open"));
//   // tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
//   actionCollection()->addAction("AllignHCenter", tmpAction);
//   connect( tmpAction, SIGNAL(triggered()), this, SLOT(allignHCenter()) ); 

  //! void showManual();
//   tmpAction = new KAction(this);
//   tmpAction->setText(i18n("Show Manual"));
//   // tmpAction->setIcon(KIcon("document-open"));
//   tmpAction->setShortcut(QKeySequence::HelpContents);
//   actionCollection()->addAction("ShowManual", tmpAction);
//   connect( tmpAction, SIGNAL(triggered()), this, SLOT(showManual()) ); 

  //! void execute();
//   tmpAction = new KAction(this);
//   tmpAction->setText(i18n("Execute"));
//   // tmpAction->setIcon(KIcon("document-open"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_F9);
//   actionCollection()->addAction("Open", tmpAction);
//   connect( tmpAction, SIGNAL(triggered()), this, SLOT(openFile()) ); 

}

void MainWindow::debug(const QString& s)
{
}

void MainWindow::changeActive(QTreeWidgetItem * item)
{
  switch(item->type())
  {
//     case FileArea::ScriptRole : changeActiveScript(item); break;
     case FileArea::GraphRole  : changeActiveGraph(item);  break;
     default: KMessageBox::error(this, i18n("AAAAAAAAAAHHHHHHHH" )); break;
  }
}

void MainWindow::changeActiveScript(KTextEditor::Document *item)
{
//   KTextEditor::Document *d = 0;
//   kDebug() << item->data(0,FileArea::ScriptRole).type();
//   kDebug() << FileArea::ScriptRole;
//   if (item->data(0,FileArea::ScriptRole).canConvert<KTextEditor::Document*>() ) {
//     d = item->data(0, FileArea::ScriptRole).value<KTextEditor::Document*>();
    _scriptingArea -> setDocument( item );
    guiFactory()->addClient(_scriptingArea->view());
//   }

}

void MainWindow::changeActiveGraph(QTreeWidgetItem *item)
{
}

KTextEditor::Editor* MainWindow::editor()
{
   return _editor; 
}
