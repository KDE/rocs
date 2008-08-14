#ifndef UI_FILEAREA_H
#define UI_FILEAREA_H

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
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */

#include <QWidget>
#include <QList>
#include <QVariant>

class MainWindow;
class QTreeWidget;
class QTreeWidgetItem;

namespace KTextEditor{
class Document;
}

class FileArea : public QWidget
{
Q_OBJECT
public:
  FileArea(MainWindow *parent);
  void createNewScript();
  enum {ScriptType = QVariant::UserType + 1};
  KTextEditor::Document* scriptAt(int pos);
public slots:


private:
  QTreeWidget* _treeWidget;
  QTreeWidgetItem* _scriptFolder;
  QList<KTextEditor::Document*> _scriptList;
  MainWindow *_mainWindow;

 void  atualizeTreeWidget();
};

#endif
