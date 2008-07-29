/***************************************************************************
 *   Copyright (C) 2005-2006 by Tomaz Canabrava and Ugo Sangiori           *
 *   tomaz.canabrava@gmail.com                                             *
 *   ugorox@gmail.com                                                      *
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

#include <QtScript>
#include <QColorDialog>
#include <KActionCollection>
#include <KAction>
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : KXmlGuiWindow(parent)
{
  pNodeCtrl = 0;
  pNodeAllign = 0;
  pEdgeCtrl = 0;
  scene = 0;

  //! cria a janela usando o arquivo gerado pelo Qt Designer.
  setupUi(this);
  //setupActions();

  spinEdgeLength->setVisible(false);
  pNodeAllign->setVisible(false);
  pNodeCtrl->setVisible(false);
  pEdgeCtrl->setVisible(false);
  
  scene = new DiagramScene(this);
  view->setScene(scene);
  view->setRenderHint(QPainter::Antialiasing);
}

MainWindow::~MainWindow()
{
  if(pNodeCtrl){ 
    delete pNodeCtrl;   
    pNodeCtrl   = 0; 
  }

  if (pNodeAllign){
    delete pNodeAllign; 
    pNodeAllign = 0;
  }
 
  if (pEdgeCtrl){
    delete pEdgeCtrl;  
    pEdgeCtrl   = 0;
  }

  if (scene){
    delete scene;       
    scene       = 0;
  }
}

void MainWindow::setupActions()
{ 
  /*! Open File */
  KAction* tmpAction = new KAction(this);
  tmpAction->setText(i18n("Open File"));
  tmpAction->setIcon(KIcon("document-open"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("Open", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(openFile()) );

 /*! newGraph(), saveGraph(), saveGraphAs() */
tmpAction = new KAction(this);
  tmpAction->setText(i18n("New Graph"));
  tmpAction->setIcon(KIcon("document-new"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("newGraph", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(newGraph()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Save Graph"));
  tmpAction->setIcon(KIcon("document-save"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("saveGraph", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(saveGraph()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Save Graph as"));
  tmpAction->setIcon(KIcon("document-save-as"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("saveGraphAs", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(saveGraphAs()) );

/*! newScript(), saveScript(), saveScriptAs(),  */
tmpAction = new KAction(this);
  tmpAction->setText(i18n("New Script"));
  tmpAction->setIcon(KIcon("document-new"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("newScript", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(newScript()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Save Script"));
  tmpAction->setIcon(KIcon("document-save"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("saveScript", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(saveScript()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Save Script as"));
  tmpAction->setIcon(KIcon("document-save-as"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("saveScriptAs", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(saveScriptAs()) );

/*! closeThisGraph(), closeAllGraphs(), closeAllButThisGraph(), */
tmpAction = new KAction(this);
  tmpAction->setText(i18n("Close this Graph"));
  tmpAction->setIcon(KIcon("close"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("closeThisGraph", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(closeThisGraph()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Close all Graphs"));
  tmpAction->setIcon(KIcon("document-close"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("closeAllGraphs", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(closeAllGraphs()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Close all Graphs but this"));
  tmpAction->setIcon(KIcon("document-new"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("clear", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(closeAllButThisGraph()) );

/*! closeThisScript(), closeAllScripts(), closeAllButThisScript() */

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Close this Script"));
  tmpAction->setIcon(KIcon("document-close"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("closeThisScript", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(closeThisScript()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Close all Scripts"));
  tmpAction->setIcon(KIcon("document-close"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("closeAllScripts", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(closeAllScripts()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Close all Scripts but this"));
  tmpAction->setIcon(KIcon("document-close"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("closeAllButThisScript", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(closeAllButThisScript()) );

/*! undo(), redo(), copy(), cut(), paste()  */

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Undo"));
  tmpAction->setIcon(KIcon("edit-undo"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("undo", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(undo()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Redo"));
  tmpAction->setIcon(KIcon("edit-redo"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("redo", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(redo()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Copy"));
  tmpAction->setIcon(KIcon("edit-copy"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("copy", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(copy()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Cut"));
  tmpAction->setIcon(KIcon("edit-cut"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("cut", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(cut()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Paste"));
  tmpAction->setIcon(KIcon("edit-paste"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("paste", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(paste()) );

/*! delete(), setStart(), setEnd() */

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Delete"));
  tmpAction->setIcon(KIcon("edit-delete"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("delete", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(delete()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Set start node"));
//   tmpAction->setIcon(KIcon("document-new"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("setStart", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(setStart()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Set end node"));
//   tmpAction->setIcon(KIcon("document-new"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("setEnd", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(setEnd()) );

/*! ALLIGN ICONS MISSING FROM SPEC.
allignLeft(), allignRight(), allignTop(), allignBottom(), allignHCenter(), allignVCenter(),
*/

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Allign Left"));
  tmpAction->setIcon(KIcon("format-justify-left"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("allignLeft", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(allignLeft()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Allign Right"));
  tmpAction->setIcon(KIcon("format-justify-right"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("allignRight", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(allignRight()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Allign Top"));
  tmpAction->setIcon(KIcon("document-new")); 
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("allignTop", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(allignTop()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Allign Bottom"));
  tmpAction->setIcon(KIcon("document-new"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("allignBottom", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(allignBottom()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Allign Center Vertically"));
  tmpAction->setIcon(KIcon("document-new")); 
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("allignVCenter", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(allignVCenter()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Allign Center Horizontally"));
  tmpAction->setIcon(KIcon("document-new"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("allignHCenter", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(allignHCenter()) );

/*! createGroup(), removeFromGroup(), ungroup(), */
tmpAction = new KAction(this);
  tmpAction->setText(i18n("Group"));
  tmpAction->setIcon(KIcon("document-new"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("createGroup", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(createGroup()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Remove Node from Group"));
  tmpAction->setIcon(KIcon("document-new")); 
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("removeFromGroup", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(removeFromGroup()) );

tmpAction = new KAction(this);
  tmpAction->setText(i18n("Ungroup"));
  tmpAction->setIcon(KIcon("document-new"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("ungroup", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(ungroup()) );

/*! setColor() */
tmpAction = new KAction(this);
  tmpAction->setText(i18n("Color"));
  tmpAction->setIcon(KIcon("document-new"));
//   tmpAction->setShortcut(Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction("setColor", tmpAction);
  connect( tmpAction, SIGNAL(triggered()), this, SLOT(setColor()) );
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

void MainWindow::setColor(){
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

void MainWindow::openFile(){
//TODO: Create this.
}
void MainWindow::newGraph(){
//TODO: Create this.
}
void MainWindow::saveGraph(){
//TODO: Create this.
}
void MainWindow::saveGraphAs(){
//TODO: Create this.
}
void MainWindow::newScript(){
//TODO: Create this.
}
void MainWindow::saveScript(){
//TODO: Create this.
}
void MainWindow::saveScriptAs(){
//TODO: Create this.
}
void MainWindow::closeThisGraph(){
//TODO: Create this.
}
void MainWindow::closeAllGraphs(){
//TODO: Create this.
}
void MainWindow::closeAllButThisGraph(){
//TODO: Create this.
}
void MainWindow::closeThisScript(){
//TODO: Create this.
}
void MainWindow::closeAllScripts(){
//TODO: Create this.
}
void MainWindow::closeAllButThisScript(){
//TODO: Create this.
}
void MainWindow::undo(){
//TODO: Create this.
}
void MainWindow::redo(){
//TODO: Create this.
}
void MainWindow::copy(){
//TODO: Create this.
}
void MainWindow::cut(){
//TODO: Create this.
}
void MainWindow::paste(){
//TODO: Create this.
}

void MainWindow::remove(){
  scene -> removeSelection();
}

void MainWindow::setStart(){
  scene -> setStartNode(); 
}

void MainWindow::setEnd(){
  scene -> setEndNode();
}

void MainWindow::allignLeft(){
  scene -> allignVLeft();  
}

void MainWindow::allignRight(){
  scene -> allignVRight();  
}

void MainWindow::allignTop(){
  scene -> allignHTop();  
}

void MainWindow::allignBottom(){
  scene -> allignHBottom();  
}

void MainWindow::allignHCenter(){
  scene -> allignHCenter();  
}

void MainWindow::allignVCenter(){
  scene -> allignVCenter();  
}

void MainWindow::createGroup(){
  scene -> group();
}
void MainWindow::removeFromGroup(){
  scene -> removeFromGroup();
}

void MainWindow::ungroup(){
  scene -> ungroup();
}

#include "MainWindow.moc"





