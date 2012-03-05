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

#ifndef SUI_OPENED_FILES_WIDGET_H
#define SUI_OPENED_FILES_WIDGET_H

#include "ui_OpenedFilesWidget.h"

class DocumentModel;

class Document;


/*!
    \brief The Opened Files widget.
    This Holds the list of opened Graph Files
*/
class OpenedFilesWidget : public QWidget, public Ui::OpenedFilesWidget
{
    Q_OBJECT

public:
    /*! default constructor.
    \param model the model that will populate the view.*/
    explicit OpenedFilesWidget(DocumentModel *model, QWidget* parent = 0);
    void selectDefaultFile();
signals:
    /*! when the active document changes, this signal is emmited.
        \param document the new active document.
    */
    void activeDocumentChanged(Document *document);

private slots:
    /*! when you click on the name of a document inside of the model, this method is invocked.
    \param model the index of the model ( rename it to index? )
    */

    void setActiveDocument(const QModelIndex& model);

public  slots:
    /*! executed when btnNewFileis clicked. */
    void on__btnNewFile_clicked();

private:
    DocumentModel *_documentModel;
};

#endif
