/*
    This file is part of Rocs.
    Copyright (C) 2011  Andreas Cord-Landwehr <phoenixx@uni-paderborn.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef ASSIGNVALUEACTION_H
#define ASSIGNVALUEACTION_H

#include <QAction>
#include "CoreTypes.h"

class GraphScene;

class AssignValueAction : public QAction
{
    Q_OBJECT
public:

    enum AssignMethod {Enumerate, RandomInteger, RandomReal};

    /** Creates a new assign value action that assigns values to selected data items.
     * \param name of the action
     * \param graphscene that contains the data items for the context menu
     * \param method how values shall be assigned
     * \param parent the parent widget
     */
    AssignValueAction(const QString& name, GraphScene *scene, AssignMethod method, QWidget *parent);

    /** Creates a new assign value action. If a DataStructure is set all data values of this DataStructure are
     * updated according to the specified. Else the currently selected set of nodes is aligned.
     * \param name of the action
     * \param graphscene that contains the data items for the context menu
     * \param method how values shall be assigned
     * \param ds data structure from which all data values shall be overwritten
     * \param parent the parent widget
     */
    AssignValueAction(const QString& name, GraphScene *scene, AssignMethod method, DataStructurePtr ds, QWidget *parent);

public slots:
    void enumerateDataStructure();
    void enumerateSelected();
    void assignRandomIntegersDataStructure();
    void assignRandomIntegersSelected();
    void assignRandomRealsDataStructure();
    void assignRandomRealsSelected();

private:
    GraphScene *_graphScene;
    DataStructurePtr _dataStructure;
};

#endif // ASSIGNVALUEACTION_H
