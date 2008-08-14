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
#include "UI_GraphScene.h"

#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QVariant>
#include <KPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <KLocale>
#include <KIcon>
#include <Graph.h>
#include <ktexteditor/document.h>
#include <ktexteditor/editor.h>
#include <ktexteditor/editorchooser.h>

#include "UI_FileArea.moc" //! why, oh, why? each .moc files takes me almost 20 min to remember and include them.

FileArea::FileArea(MainWindow *parent) : QWidget(parent) 
{
   _scriptFolder = 0;
   _treeWidget = 0;

   _mainWindow = parent;
   _treeWidget = new QTreeWidget(this);
   _treeWidget->setHeaderHidden(true);
   _scriptFolder = new QTreeWidgetItem(_treeWidget);
   _scriptFolder->setIcon(0, KIcon("folder"));
   _scriptFolder->setText(0, i18n("Scripts"));
    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->addWidget(_treeWidget);

   _scriptList.clear();
   createNewScript();
   createNewScript();
   createNewScript();

   connect( _treeWidget, SIGNAL(itemClicked (QTreeWidgetItem*, int)),
            _mainWindow, SLOT(changeActive(QTreeWidgetItem*))); 
}

void FileArea::createNewScript(){
   KTextEditor::Editor *e = _mainWindow->editor();
   KTextEditor::Document *d = e -> createDocument(0);
   _scriptList.append(d);
   atualizeTreeWidget();
}

void FileArea::atualizeTreeWidget(){
  QList<QTreeWidgetItem *> children = _scriptFolder->takeChildren();
  foreach(QTreeWidgetItem* t, children)
  { 
    delete t; 
  }

  for(int i = 0; i < _scriptList.size(); i++){
     KTextEditor::Document *d = _scriptList.at(i);
     QTreeWidgetItem *item = new QTreeWidgetItem(_scriptFolder, ScriptType);
     item->setText (0, d->documentName());
     item->setData (0, ScriptType, i);
  }
}

KTextEditor::Document* FileArea::scriptAt(int pos)
{
   return _scriptList.at(pos);
}