/***************************************************************************
 *   Copyright (C) 2005-2006 by Anne-Marie Mahfouf   *
 *   tomaz.canabrava@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include <QMainWindow>
#include <QtScript>
#include <QColorDialog>
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  //! cria a janela usando o arquivo gerado pelo Qt Designer.
  setupUi(this);

  spinEdgeLength->setVisible(false);
  pNodeAllign->setVisible(false);
  pNodeCtrl->setVisible(false);
  pEdgeCtrl->setVisible(false);
  
  scene = new DiagramScene(this);
  view->setScene(scene);
  view->setRenderHint(QPainter::Antialiasing);
  createActions();
}

MainWindow::~MainWindow()
{
  delete pNodeCtrl;   pNodeCtrl   = 0;
  delete pNodeAllign; pNodeAllign = 0;
  delete pEdgeCtrl;   pEdgeCtrl   = 0;
  delete scene;       scene       = 0;
}

void MainWindow::createActions()
{ /*
  connect(actionOpen_File,         SIGNAL(triggered()), this, SLOT(openFile())     );
  connect(actionSave_Graph,        SIGNAL(triggered()), this, SLOT(saveGraph())    );
  connect(actionSave_Graph_As,     SIGNAL(triggered()), this, SLOT(saveGraphAs()   );
  connect(actionNew_Script,        SIGNAL(triggered()), this, SLOT(newScript())    );
  connect(actionSave_Script,       SIGNAL(triggered()), this, SLOT(saveScript())   );
  connect(actionClose,             SIGNAL(triggered()), this, SLOT(quit())         );
  connect(actionNew_Graph,         SIGNAL(triggered()), this, SLOT(newGraph()      );
  connect(actionSave_Script_As,    SIGNAL(triggered()), this, SLOT(saveScriptAs()) );

  connect(actionUndo,              SIGNAL(triggered()), scene, SLOT(undo())            );
  connect(actionRedo,              SIGNAL(triggered()), scene, SLOT( redo())           );
  connect(actionCopy,              SIGNAL(triggered()), scene, SLOT(copy()             );
  connect(actionCut,               SIGNAL(triggered()), scene, SLOT( cut())            ); 
  connect(actionPaste,             SIGNAL(triggered()), scene, SLOT(paste())           );
  connect(actionRemove,            SIGNAL(triggered()), scene, SLOT(removeSelection()) );
  connect(actionSet_Start,         SIGNAL(triggered()), scene, SLOT(setStartNode())    );
  connect(actionSet_End,           SIGNAL(triggered()), scene, SLOT(setEndNode())      );
  connect(actionLeft,              SIGNAL(triggered()), scene, SLOT(allignVLeft())     );
  connect(actionRight,             SIGNAL(triggered()), scene, SLOT(allignVRight())    );
  connect(actionBottom,            SIGNAL(triggered()), scene, SLOT(allignHBottom())   );
  connect(actionTop,               SIGNAL(triggered()), scene, SLOT(allignHTop())      );
  connect(actionCenter_Horizontal, SIGNAL(triggered()), scene, SLOT(allignHCenter())   );
  connect(actionCenter_Vertical,   SIGNAL(triggered()), scene, SLOT(allignVCenter())   );
  connect(actionCreate_Group,      SIGNAL(triggered()), scene, SLOT(group())           );
  connect(actionRemove_From_Group, SIGNAL(triggered()), scene, SLOT(removeFromGroup()) );
  connect(actionUngroup,           SIGNAL(triggered()), scene, SLOT(ungroup())         );
  // connect(actionColor,             SIGNAL(triggered()), scene, SLOT(setColor()));
*/
}

void MainWindow::connectEdgeSignals(Edge *e)
{
  connect(spinEdgeLength, SIGNAL(valueChanged(double)), e, SLOT(setLength(double)));
  connect(chkEdgeShowLength, SIGNAL(stateChanged(int)), e, SLOT(showLength(int)));
}

void MainWindow::disconnectEdgeSignals(Edge *e)
{
  disconnect(spinEdgeLength, SIGNAL(valueChanged(double)), e, SLOT(setLength(double)));
  disconnect(chkEdgeShowLength, SIGNAL(stateChanged(int)), e, SLOT(showLength(int)));
}

void MainWindow::connectNodeSignals(Node *d)
{
  connect(txtNodeName, SIGNAL(textChanged(QString)), d, SLOT(setName(QString)));
}

void MainWindow::disconnectNodeSignals(Node *d)
{
  disconnect(txtNodeName, SIGNAL(textChanged(QString)), d, SLOT(setName(QString)));
}

void   MainWindow::resetSpinEdgeLength(bool showSpin, qreal value)
{
  spinEdgeLength->setValue(value);
  spinEdgeLength->setVisible(showSpin);
  chkEdgeShowLength->setChecked(showSpin);
}

void MainWindow::debug(const QString& s)
{
  txtDebug->append(s);
}

void MainWindow::runScript()
{
  txtDebug->clear();
  scene->createScriptFunctions(txtProgrammingArea->toPlainText());
}

void MainWindow::on_btnNodeColor_clicked(){
  QColor c = QColorDialog::getColor();
  scene->changeSelectedNodeColor(c);
}

void MainWindow::showNodeControls(bool show)
{
  if (pNodeCtrl)
  pNodeCtrl   -> setVisible(show); 
}

void MainWindow::showNodeAllign(bool show)  
{ 
  if (pNodeAllign)
  pNodeAllign -> setVisible(show); 
}

void MainWindow::showEdgeControls(bool show)
{ 
  if(pEdgeCtrl)
  pEdgeCtrl   -> setVisible(show); 
}

#include "MainWindow.moc"





