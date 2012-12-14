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

#include "AddDataHandAction.h"
#include "Scene/GraphScene.h"
#include "Scene/DataItem.h"
#include "DataStructure.h"
#include "Data.h"
#include <KLocale>
#include "Document.h"
#include <KDebug>
#include <DocumentManager.h>

AddDataHandAction::AddDataHandAction(GraphScene *scene, QObject *parent)
    : AbstractAction(scene, parent),
      _dataType(DataTypePtr())
{
    setText(i18nc("@action:intoolbar", "Add Data"));
    setToolTip(i18nc("@info:tooltip", "Creates a new node at the click position on the drawing area."));
    setIcon(KIcon("rocsadddata"));
    _name = "rocs-hand-add-node";
}

AddDataHandAction::AddDataHandAction(GraphScene *scene, DataTypePtr dataType, QObject *parent)
    : AbstractAction(scene, parent),
      _dataType(dataType)
{
    setText(i18nc("@action:intoolbar", "Add %1", dataType->name()));
    setToolTip(i18nc("@info:tooltip", "Creates a new node at the click position on the drawing area."));
    setIcon(dataType->icon());
    _name = "rocs-hand-add-node";
}

AddDataHandAction::~AddDataHandAction()
{
}

void AddDataHandAction::setDataType(DataTypePtr dataType)
{
    _dataType = dataType;
    setText(i18n("Add %1", dataType->name()));
}

bool AddDataHandAction::executePress(QPointF pos)
{
    if (!DocumentManager::self().activeDocument()->activeDataStructure()
            ||  DocumentManager::self().activeDocument()->activeDataStructure()->readOnly()
       ) {
        return false;
    }

    // if no dataType set, we use default data type
    int dataTypeIdentifier = 0;
    if (_dataType) {
        dataTypeIdentifier = _dataType->identifier();
    }

    if (DocumentManager::self().activeDocument()->activeDataStructure()
        ->addData(QString(), QPointF(pos.x(), pos.y()), dataTypeIdentifier))
    {
        return true;
    }
    return true;
}

void AddDataHandAction::updateIcon()
{
    if (_dataType) {
        setIcon(_dataType->icon());
    }
}

