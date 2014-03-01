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

#include "DeleteAction.h"

#include <KLocale>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include "Scene/GraphScene.h"
#include "Scene/DataItem.h"
#include "DataStructure.h"
#include "Data.h"


DeleteAction::DeleteAction(const QString& name, GraphScene *scene, QWidget *parent)
    : KAction(QIcon(), name, parent)
{
    _graphScene = scene;
    connect(this, SIGNAL(triggered()), this, SLOT(executeDelete()));
}

DeleteAction::DeleteAction(const QString& name, GraphScene *scene, DataPtr data, QWidget *parent)
    : KAction(QIcon(), name, parent)
{
    _graphScene = scene;
    _data = data;
    connect(this, SIGNAL(triggered()), this, SLOT(executeDelete()));
}

DeleteAction::DeleteAction(const QString& name, GraphScene *scene, DataStructurePtr dataStructure, QWidget *parent)
    : KAction(QIcon(), name, parent)
{
    _graphScene = scene;
    _dataStructure = dataStructure;
    connect(this, SIGNAL(triggered()), this, SLOT(executeDelete()));
}


void DeleteAction::executeDelete()
{
    if (_data) {
        _data->remove();
        return;
    }
    if (_dataStructure) {
        _dataStructure->remove();
        return;
    }
    foreach(QGraphicsItem * selectedItem, _graphScene->selectedItems()) {
        if (DataItem *dItem = qgraphicsitem_cast<DataItem*>(selectedItem)) {
            dItem->data()->remove();
        }
    }
}
