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

#include "Data.h"

#include <KAction>

class GraphScene;

class AssignValueAction : public KAction
{
    Q_OBJECT
public:

    enum AssignMethod {Enumerate, RandomInteger, RandomReal};


    /*! Creates a new assign value action. If a DataStructure is set all data values of this DataStructure are
      updated according to the specified. Else the currently selected set of nodes is aligned.
      \param actionName the name of the button
      \param tooltip some helper text.
      \param o the orientation that this button will work on.
      \param parent the parent widget
      \param if true (default) trigger-signal is connected to align slot, else if false this need to be set by hand
    */
    AssignValueAction(const QString& name, GraphScene *scene, AssignMethod method, QWidget *parent);
    
    AssignValueAction(const QString& name, GraphScene *scene, AssignMethod method, DataStructurePtr ds, QWidget *parent);
    
public slots:
    void enumerateDataStructure();
    void enumerateSelected();


private:

    GraphScene *_graphScene;
    DataStructurePtr _dataStructure;
};

#endif // ASSIGNVALUEACTION_H
