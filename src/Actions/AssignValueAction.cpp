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


#include "AssignValueAction.h"

#include "GraphVisualEditor.h"
#include "GraphScene.h"
#include "DataStructure.h"
#include "Pointer.h"
#include "DataItem.h"
#include "Modifiers/ValueModifier.h"

#include <KIcon>


AssignValueAction::AssignValueAction(const QString& name, GraphScene *scene, AssignMethod method, QWidget *parent)
        : KAction(KIcon(), name, parent) 
{
    _graphScene = scene;

    connect(this, SIGNAL(triggered()), this, SLOT(enumerateSelected()));
    
}


AssignValueAction::AssignValueAction(const QString& name, GraphScene *scene, AssignMethod method, DataStructurePtr ds, QWidget *parent)
        : KAction(KIcon(), name, parent) 
{
    _graphScene = scene;
    _dataStructure = ds;
    connect(this, SIGNAL(triggered()), this, SLOT(enumerateDataStructure()));
    
}

void AssignValueAction::enumerateDataStructure() {
    if (_dataStructure) {
        ValueModifier modifier;
        modifier.enumerate(_dataStructure->dataList(), 1, true);
    }
}


void AssignValueAction::enumerateSelected()
{
    DataList dataList;
    QList<QGraphicsItem*> itemList = _graphScene->selectedItems();
    foreach(QGraphicsItem *i, itemList) {
        if ( DataItem *dataItem = qgraphicsitem_cast<DataItem*>(i) ) {
            dataList.append( dataItem->data() );
        }
    }
    ValueModifier modifier;
    modifier.enumerate(dataList, 1, true);
}
