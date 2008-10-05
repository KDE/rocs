
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

#include "UI_OpenedFilesDockWidget.h"
#include "UI_MainWindow.h"

#include "model_GraphDocument.h"
#include <QDebug>
OpenedFilesDockWidget::OpenedFilesDockWidget(GraphDocumentModel *model, QWidget* parent, Qt::WindowFlags flags)
: QDockWidget(i18n("Opened Files"),parent, flags) 
{
  setupUi(this);  
  _documentModel = model;
  _openedFilesListView->setModel(model);
  connect(_openedFilesListView, SIGNAL(clicked(const QModelIndex&)),
  this, SLOT(setActiveDocument(const QModelIndex&)));
}

void OpenedFilesDockWidget::on__btnNewFile_clicked()
{
  _openedFilesListView->model()->insertRow(1);
}

void OpenedFilesDockWidget::setActiveDocument(const QModelIndex& modelindex){
  libgraph::GraphDocument *g = _documentModel -> at(modelindex);
  if (g == 0) qDebug() << "Returning a NULL pointer";
  else{
    emit activeDocumentChanged( g );
    qDebug() << "Send the Doc";
  }
}
