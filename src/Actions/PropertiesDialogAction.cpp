/*
    This file is part of Rocs.
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "PropertiesDialogAction.h"

#include <QPointer>
#include <DataStructurePropertiesFullWidget.h>
#include <DataPropertiesWidget.h>
#include <PointerPropertiesWidget.h>

PropertiesDialogAction::PropertiesDialogAction(QString text, DataStructurePtr dataStructure, QObject* parent)
    : KAction(text, parent)
{
    _dataStructure = dataStructure;
    _dialogType = DATASTRUCTURE;
    connect(this, SIGNAL(triggered()), this, SLOT(showDialog()));
}


PropertiesDialogAction::PropertiesDialogAction(QString text, DataPtr data, QObject* parent)
    : KAction(text, parent)
{
    _data = data;
    _dialogType = DATA;
    connect(this, SIGNAL(triggered()), this, SLOT(showDialog()));
}


PropertiesDialogAction::PropertiesDialogAction(QString text, PointerPtr pointer, QObject* parent)
    : KAction(text, parent)
{
    _pointer = pointer;
    _dialogType = POINTER;
    connect(this, SIGNAL(triggered()), this, SLOT(showDialog()));
}


void PropertiesDialogAction::showDialog()
{
    switch (_dialogType) {
    case DATASTRUCTURE: {
        if (!_dataStructure) {
            return;
        }
        QPointer<DataStructurePropertiesFullWidget> dialog = new DataStructurePropertiesFullWidget;
        dialog->setDataStructure(_dataStructure);
        dialog->setPosition(_screenPosition);
        dialog->exec();
        break;
    }
    case DATA: {
        if (!_data) {
            return;
        }
        QPointer<DataPropertiesWidget> dialog = new DataPropertiesWidget(_data);
        dialog->setPosition(_screenPosition);
        dialog->exec();
        break;
    }
    case POINTER: {
        if (!_pointer) {
            return;
        }
        QPointer<PointerPropertiesWidget> dialog = new PointerPropertiesWidget(_pointer);
        dialog->setPosition(_screenPosition);
        dialog->exec();
        break;
    }
    default:
        break;
    }
}


void PropertiesDialogAction::setPostion(QPointF screenPosition)
{
    _screenPosition = screenPosition;
}
