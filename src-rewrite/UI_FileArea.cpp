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
#include "GraphCollection.h"

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

   _graphFolder = new QTreeWidgetItem(_treeWidget);
   _graphFolder->setIcon(0, KIcon("folder"));
   _graphFolder->setText(0, i18n("Graphs"));

    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->addWidget(_treeWidget);

   _scripts.clear();
   _graphcollections.clear();

   createNewScript();
   createNewScript();
   createNewScript();

   createNewGraph();
   createNewGraph();
   createNewGraph();

   connect( _treeWidget, SIGNAL(itemClicked (QTreeWidgetItem*, int)),
            _mainWindow, SLOT(changeActive(QTreeWidgetItem*))); 
}

void FileArea::createNewScript()
{
   KTextEditor::Editor *e = _mainWindow->editor();
   KTextEditor::Document *d = e -> createDocument(0);
   _scripts.append(d);
   atualizeTreeWidget();
}

void FileArea::createNewGraph()
{
   GraphCollection *gc = new GraphCollection();
   _graphcollections.append(gc);
   atualizeTreeWidget();
}

void FileArea::atualizeTreeWidget()
{
  QList<QTreeWidgetItem *> children = _scriptFolder->takeChildren();
  foreach(QTreeWidgetItem* t, children)
  { 
    delete t;
  }
  children = _graphFolder->takeChildren();
  foreach(QTreeWidgetItem* t, children)
  { 
    delete t;
  }

  for(int i = 0; i < _scripts.size(); i++)
  {
     KTextEditor::Document *d = _scripts.at(i);
     QTreeWidgetItem *item = new QTreeWidgetItem(_scriptFolder, ScriptType);
     item->setText (0, d->documentName());
     item->setData (0, ScriptType, i);
  }

  for(int i = 0; i < _graphcollections.size(); i++)
  {
      GraphCollection *gc = _graphcollections.at(i);
      QTreeWidgetItem *item = new QTreeWidgetItem(_graphFolder, GraphType);
      item->setText(0, gc -> name());
      item->setData(0, GraphType, i);
  }

}

KTextEditor::Document* FileArea::scriptAt(int pos)
{
   return _scripts.at(pos);
}

GraphCollection* FileArea::graphCollectionAt(int pos)
{
   return _graphcollections.at(pos);
}