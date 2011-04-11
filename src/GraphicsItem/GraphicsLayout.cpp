/* This file is part of Rocs,
     Copyright (C) 2011 by:
     Andreas Cord-Landwehr <cola@uni-paderborn.de>

     Rocs is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.

     Rocs is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with Rocs; if not, write to the Free Software
     Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "GraphicsLayout.h"

#include "Document.h"
#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"
#include "DocumentManager.h"

GraphicsLayout::GraphicsLayout(QObject* parent) : QObject(parent)
{ }

GraphicsLayout::~GraphicsLayout() {

}

GraphicsLayout* GraphicsLayout::_self = 0;

GraphicsLayout* GraphicsLayout::self() {
    if (!_self){
        _self = new GraphicsLayout();
    }
    return _self;
}


void GraphicsLayout::setViewStyleDataNode(int style)
{
    _viewStyleDataNode = style;
    QList<DataStructure*> dsList = DocumentManager::self()->activeDocument()->dataStructures();
    foreach (DataStructure* ds, dsList) {
        ds->setDataNameVisibility( ds->dataNameVisibility() ); // triggers redraw of all edges
    }
//     _scene->update(); //TODO
}


void GraphicsLayout::setViewStyleDataEdge(int style)
{
    _viewStyleDataEdge = style;
    QList<DataStructure*> dsList = DocumentManager::self()->activeDocument()->dataStructures();
    foreach (DataStructure* ds, dsList) {
        ds->setPointerNameVisibility( ds->pointerNameVisibility() ); // triggers redraw of all edges
    }
//     _scene->update(); //TODO
}

int GraphicsLayout::viewStyleDataNode() const
{
    return _viewStyleDataNode;
}

int GraphicsLayout::viewStyleDataEdge() const
{
    return _viewStyleDataEdge;
}
