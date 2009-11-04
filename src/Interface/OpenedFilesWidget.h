
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
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with Step; if not, write to the Free Software
	 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
*/

#ifndef SUI_OPENED_FILES_WIDGET_H
#define SUI_OPENED_FILES_WIDGET_H

#include "ui_OpenedFilesWidget.h"

class GraphDocumentModel;

class GraphDocument;


/*!
	\brief The Opened Files widget.
	This Holds the list of opened Graph Files
*/
class OpenedFilesWidget : public QWidget, public Ui::OpenedFilesWidget {
    Q_OBJECT

public:
    /*! default constructor.
    \param model the model that will populate the view.*/
    explicit OpenedFilesWidget(GraphDocumentModel *model, QWidget* parent = 0);
    void selectDefaultFile();
signals:
    /*! when the active document changes, this signal is emmited.
    	\param document the new active document.
    */
    void activeDocumentChanged( GraphDocument *document);

private slots:
    /*! when you click on the name of a document inside of the model, this method is invocked.
    \param model the index of the model ( rename it to index? )
    */

    void setActiveDocument(const QModelIndex& model);

public	slots:
    /*! executed when btnNewFileis clicked. */
    void on__btnNewFile_clicked();

private:
    GraphDocumentModel *_documentModel;
};

#endif
