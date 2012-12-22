/*
    This file is part of Rocs.
    Copyright (C) 2011 Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef ASSIGNVALUESWIDGET_H
#define ASSIGNVALUESWIDGET_H

#include "ui_AssignValuesWidget.h"
#include <KDialog>


class QGridLayout;
class Document;
class DataStructure;

class AssignValuesWidget :
    public KDialog
{
    Q_OBJECT

    enum AssignMethod {
        ID,
        ALPHA,
        ID_ALPHA,
        UNIFORM_INTEGER,
        UNIFORM_FLOAT,
        CONSTANT
    };

public:
    explicit AssignValuesWidget(Document* graphDoc, QWidget *parent = 0);
    ~AssignValuesWidget();

    /**
     * Add data structures to QComboBox of UI starting at position 0. Take care that
     * the data structure IDs must be given in increasing ID order without gaps.
     * Only data structures of documents of data structure type "Graph" are used.
     *
     * \param   QStringList dsNames names of data structures
     * \return  void
     */
    void addDataStructures(const QStringList& dsNames);

public slots:
    /**
     * Assign values as specified at the UI.
     */
    void assignValues();

    /**
     * Only enable Apply/Ok buttons if the specified property is valid.
     */
    void updateApplyButtonStates();

private:
    Document* graphDoc_;
    Ui::AssignValuesWidget *ui;
};

#endif // ASSIGNVALUESWIDGET_H
