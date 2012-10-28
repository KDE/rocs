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
#include "Scene/GraphScene.h"
#include "Scene/DataItem.h"
#include "DataStructure.h"
#include "Pointer.h"
#include "Modifiers/ValueModifier.h"

#include <KIcon>


AssignValueAction::AssignValueAction(const QString& name, GraphScene *scene, AssignMethod method, QWidget *parent)
    : KAction(KIcon(), name, parent)
{
    _graphScene = scene;
    switch (method) {
    case Enumerate:
        connect(this, SIGNAL(triggered()), this, SLOT(enumerateSelected()));
        break;
    case RandomInteger:
        connect(this, SIGNAL(triggered()), this, SLOT(assignRandomIntegersSelected()));
        break;
    case RandomReal:
        connect(this, SIGNAL(triggered()), this, SLOT(assignRandomRealsSelected()));
        break;
    default:
        break;
    }
}


AssignValueAction::AssignValueAction(const QString& name, GraphScene *scene, AssignMethod method, DataStructurePtr ds, QWidget *parent)
    : KAction(KIcon(), name, parent)
{
    _graphScene = scene;
    _dataStructure = ds;
    switch (method) {
    case Enumerate:
        connect(this, SIGNAL(triggered()), this, SLOT(enumerateDataStructure()));
        break;
    case RandomInteger:
        connect(this, SIGNAL(triggered()), this, SLOT(assignRandomIntegersDataStructure()));
        break;
    case RandomReal:
        connect(this, SIGNAL(triggered()), this, SLOT(assignRandomRealsDataStructure()));
        break;
    default:
        break;
    }
}


void AssignValueAction::enumerateDataStructure()
{
    if (_dataStructure) {
        ValueModifier modifier;
        modifier.enumerate(_dataStructure->dataList(), QString("value"), 1, true);
    }
}


void AssignValueAction::enumerateSelected()
{
    DataList dataList;
    QList<QGraphicsItem*> itemList = _graphScene->selectedItems();
    foreach(QGraphicsItem * i, itemList) {
        if (DataItem *dataItem = qgraphicsitem_cast<DataItem*>(i)) {
            dataList.append(dataItem->data());
        }
    }
    ValueModifier modifier;
    modifier.enumerate(dataList, QString("value"), 1, true);
}

void AssignValueAction::assignRandomIntegersDataStructure()
{
    if (_dataStructure) {
        ValueModifier modifier;
        //FIXME use really random seed
        modifier.assignRandomIntegers(_dataStructure->dataList(), QString("value"), 1, 100, 1, true);
    }
}


void AssignValueAction::assignRandomIntegersSelected()
{
    DataList dataList;
    QList<QGraphicsItem*> itemList = _graphScene->selectedItems();
    foreach(QGraphicsItem * i, itemList) {
        if (DataItem *dataItem = qgraphicsitem_cast<DataItem*>(i)) {
            dataList.append(dataItem->data());
        }
    }
    ValueModifier modifier;
    modifier.assignRandomIntegers(dataList, QString("value"), 1, 100, 1, true);
}


void AssignValueAction::assignRandomRealsDataStructure()
{
    if (_dataStructure) {
        ValueModifier modifier;
        //FIXME use really random seed
        modifier.assignRandomReals(_dataStructure->dataList(), QString("value"), 1, 10, 1, true);
    }
}


void AssignValueAction::assignRandomRealsSelected()
{
    DataList dataList;
    QList<QGraphicsItem*> itemList = _graphScene->selectedItems();
    foreach(QGraphicsItem * i, itemList) {
        if (DataItem *dataItem = qgraphicsitem_cast<DataItem*>(i)) {
            dataList.append(dataItem->data());
        }
    }
    ValueModifier modifier;
    modifier.assignRandomReals(dataList, QString("value"), 1, 10, 1, true);
}
