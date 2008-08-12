/* This file is part of Rocs, 
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>
   With Sponsorship from Faculdades Ruy Barbosa

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

#include "UI_FileArea.h"
#include "UI_MainWindow.h"
#include <QTreeWidgetItem>
#include <KPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <KLocale>
#include <KIcon>
#include <Graph.h>

FileArea::FileArea(MainWindow *parent) : QWidget(parent) 
{
   setObjectName("FileArea");
   
   createTreeWidget();
   createButtons();
   createDesignLayout();
   
   addScript(i18n("untitled"));
   addGraph(i18n("untitled"));
    
}

void FileArea::createButtons()
{
  _btnNewGraph  = new KPushButton(this);
  _btnNewGraph->setIcon(KIcon("file-new"));

  _btnNewScript = new KPushButton(this);
  _btnNewScript->setIcon(KIcon("file-new"));

  _btnSave      = new KPushButton(this); 
  _btnSave->setIcon(KIcon("file-save"));

  _btnSaveAll   = new KPushButton(this); 
  _btnSaveAll->setIcon(KIcon("file-save"));

  _btnClose     = new KPushButton(this);
  _btnClose->setIcon(KIcon("file-close"));

  _btnCloseAll  = new KPushButton(this);
  _btnCloseAll->setIcon(KIcon("file-close"));

  _btnShowHide  = new KPushButton(this);
  _btnShowHide->setIcon(KIcon(""));
}


void FileArea::createTreeWidget()
{
   _treeWidget = new QTreeWidget(this);
   _treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   //_treeWidget->setRootIsDecorated(false);
   _treeWidget->setAnimated(true);
   _treeWidget->setHeaderHidden(true);

   _scriptFolder = new QTreeWidgetItem(_treeWidget);
   _scriptFolder->setIcon(0, KIcon("folder"));
   _scriptFolder->setText(0, i18n("Scripts"));

   _graphFolder = new QTreeWidgetItem(_treeWidget);
   _graphFolder->setIcon(0, KIcon("folder"));
   _graphFolder->setText(0, i18n("Graphs"));
}

void FileArea::createDesignLayout()
{
  QVBoxLayout* verticalLayout = new QVBoxLayout(this);
  QHBoxLayout* horizontalLayout = new QHBoxLayout();  
  
  horizontalLayout->addWidget(_btnShowHide);
  horizontalLayout->addWidget(_btnNewGraph);
  horizontalLayout->addWidget(_btnNewScript);
  horizontalLayout->addWidget(_btnSave);
  horizontalLayout->addWidget(_btnSaveAll);
  horizontalLayout->addWidget(_btnClose);
  horizontalLayout->addWidget(_btnCloseAll);

  verticalLayout->addWidget(_treeWidget);
  verticalLayout->addLayout(horizontalLayout);
  setLayout(verticalLayout);
}

void FileArea::addScript(const QString& s)
{
   QTreeWidgetItem *scriptItem = new QTreeWidgetItem(_scriptFolder);
   scriptItem->setIcon(0, KIcon("file-new"));
   scriptItem->setText(0, s);
   scriptItem->setData(0, ScriptRole, "");
}

void FileArea::addGraph(const QString& s)
{
   QTreeWidgetItem *graphItem = new QTreeWidgetItem(_graphFolder);
   graphItem->setIcon(0, KIcon("file-new"));
   graphItem->setText(0, s);
   graphItem->setData(0, GraphRole, "");
}

void FileArea::addScript(const QString& name, const QString& data)
{

}

void FileArea::addGraph(const QString& name, Graph& g)
{

}
