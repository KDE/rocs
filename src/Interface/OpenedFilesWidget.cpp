/* 
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of 
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "OpenedFilesWidget.h"

#include "model_GraphDocument.h"
#include <KDebug>

OpenedFilesWidget::OpenedFilesWidget(DocumentModel *model, QWidget* parent)
        : QWidget(parent) {
    setupUi(this);
    _documentModel = model;
    _openedFilesListView->setModel(model);
    connect(_openedFilesListView,SIGNAL(clicked(QModelIndex)),this,SLOT(setActiveDocument(QModelIndex)));
}

void OpenedFilesWidget::on__btnNewFile_clicked() {
    _documentModel->insertRow(_documentModel->rowCount());
    Document *g = _documentModel -> at( _documentModel->index(_documentModel->rowCount()-1,0) );
    emit activeDocumentChanged( g );
}

void OpenedFilesWidget::setActiveDocument(const QModelIndex& modelindex) {
    Document *g = _documentModel -> at(modelindex);
    if (g == 0) return;
    emit activeDocumentChanged( g );
}

void OpenedFilesWidget::selectDefaultFile() {
    setActiveDocument(_documentModel->index(0,0));
}
