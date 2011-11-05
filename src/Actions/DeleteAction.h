/*  
    This file is part of Rocs.
    Copyright 2008  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008  Ugo Sangiori <ugorox@gmail.com>
    Copyright 2011  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef DELETEACTION_H
#define DELETEACTION_H

#include <KAction>

#include "Rocs_Typedefs.h"

class GraphScene;

/*! this class defines the delete button on the pallete. */
class DeleteAction : public KAction
{
    Q_OBJECT
public:
    /** Constructor for delete action that deletes all selected data items.
     * \param name of this action
     * \param scene the graph scene
     * \param parent the mainwindow
     */
    DeleteAction(const QString& name, GraphScene *scene, QWidget *parent);

    /** Constructor for delete action that deletes specified data item.
     * \param name of this action
     * \param data item that shall be deleted
     * \param scene the graph scene
     * \param parent the mainwindow
     */
    DeleteAction(const QString& name, GraphScene *scene, DataPtr data, QWidget *parent);

    /** Constructor for delete action that deletes specified data structure.
     * \param name of this action
     * \param dataStructure that shall be deleted
     * \param scene the graph scene
     * \param parent the mainwindow
     */
    DeleteAction(const QString& name, GraphScene *scene, DataStructurePtr dataStructure, QWidget *parent);

public slots:
    void executeDelete();

private:
    GraphScene *_graphScene;
    DataPtr _data;
    DataStructurePtr _dataStructure;
};

#endif // DELETEACTION_H
