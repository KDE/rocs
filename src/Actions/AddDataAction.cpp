/*
    This file is part of Rocs.
    Copyright 2008  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008  Ugo Sangiori <ugorox@gmail.com>

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

#include "AddDataAction.h"
#include "GraphScene.h"
#include "DataStructure.h"
#include "DataItem.h"
#include "Data.h"
#include <KLocale>
#include "Document.h"
#include <KDebug>
#include <DocumentManager.h>

AddDataAction::AddDataAction(GraphScene *scene, QObject *parent)
    : AbstractAction(scene, parent)
{
    setText(i18n("Add Node"));
    setToolTip(i18n("Creates a new node at the click position."));
    _name = "add-node";
}

AddDataAction::~AddDataAction()
{
    kDebug() << "Destroyed";
}

bool AddDataAction::executePress(QPointF pos)
{
    if (!DocumentManager::self()->activeDocument()->activeDataStructure()
            ||  DocumentManager::self()->activeDocument()->activeDataStructure()->readOnly()
       ) {
        return false;
    }
    DataPtr tmp = DocumentManager::self()->activeDocument()->activeDataStructure()
                  ->addData(i18n("untitled"), QPointF(pos.x(), pos.y()));
    qDebug()  << " the data's actual position: " << tmp.get()->x() << ", " << tmp.get()->y() ;
    return true;
}


void AddDataAction::setAddPosition(QPointF position)
{
    qDebug() << "add node at position " << position;
    _position = position;
}


bool AddDataAction::executePress()
{
    return executePress(_position);
}
