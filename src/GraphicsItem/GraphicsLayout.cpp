/*
    This file is part of Rocs.
    Copyright (C) 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "GraphicsLayout.h"

#include "Document.h"
#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"
#include "DocumentManager.h"
#include <DataType.h>
#include <PointerType.h>

GraphicsLayout::GraphicsLayout(QObject* parent) : QObject(parent)
{ }

GraphicsLayout::~GraphicsLayout()
{

}

GraphicsLayout* GraphicsLayout::_self = 0;

GraphicsLayout* GraphicsLayout::self()
{
    if (!_self) {
        _self = new GraphicsLayout();
    }
    return _self;
}


void GraphicsLayout::setViewStyleDataNode(int style)
{
    _viewStyleDataNode = style;
    QList< DataStructurePtr > dsList = DocumentManager::self()->activeDocument()->dataStructures();
    foreach(DataStructurePtr ds, dsList) {
        foreach(int identifier, ds->dataTypeList()) {
            // update all data elements
            ds->setDataNameVisibility(ds->dataType(identifier)->isNameVisible(), identifier);
        }
    }
    emit changed();
}


void GraphicsLayout::setViewStyleDataEdge(int style)
{
    _viewStyleDataEdge = style;
    QList< DataStructurePtr > dsList = DocumentManager::self()->activeDocument()->dataStructures();
    foreach(DataStructurePtr ds, dsList) {
        foreach(int identifier, ds->pointerTypeList()) {
            // update all pointers
            ds->setPointerNameVisibility(ds->pointerType(identifier)->isNameVisible(), identifier);
        }
    }
    emit changed();
}

int GraphicsLayout::viewStyleDataNode() const
{
    return _viewStyleDataNode;
}

int GraphicsLayout::viewStyleDataEdge() const
{
    return _viewStyleDataEdge;
}
