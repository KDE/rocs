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

#include "AddConnectionAction.h"
#include "GraphScene.h"
#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"
#include "DataItem.h"
#include "PointerItem.h"
#include <KLocale>

#include <KDebug>
#include <DocumentManager.h>
#include <Document.h>

AddConnectionAction::AddConnectionAction(GraphScene *scene, QObject *parent)
        : AbstractAction(scene, parent) {
    setText(i18n ( "Add Edge" ));
    setToolTip ( i18n ( "Creates a new edge between 2 nodes" ) );
    setIcon ( KIcon ( "rocsaddedge" ) );

    _from = 0;
    _to   = 0;
    _tmpLine  = 0;
    _working = false;
    _name = "add-edge";
}

AddConnectionAction::~AddConnectionAction() {
    kDebug() << "Destroyed";
}

bool AddConnectionAction::executePress(QPointF pos) {
    if (_working){
        return false;
    }
    
    if ( !  DocumentManager::self()->activeDocument()->activeDataStructure()
         || DocumentManager::self()->activeDocument()->activeDataStructure()->readOnly()){
     return false;
    }
    
    if ( (_from = qgraphicsitem_cast<DataItem*>(_graphScene->itemAt(pos))) ) {
        _working = true;
        _startPos = QPointF(_from->data()->x(), _from->data()->y());
        return true;
    }

    return false;
}

bool AddConnectionAction::executeMove(QPointF pos) {
    if (   !DocumentManager::self()->activeDocument()->activeDataStructure() 
        || !_from){
        return false;
    }

    if ( !_tmpLine ) {
        _tmpLine = new QGraphicsLineItem();
        _graphScene->addItem(_tmpLine);
    }

    _tmpLine->setLine(_startPos.x(), _startPos.y(), pos.x(), pos.y());
    return true;
}

bool AddConnectionAction::executeRelease(QPointF pos) {
    if ( !_working 
     ||  !DocumentManager::self()->activeDocument()->activeDataStructure() ){
        return false;
    }

    delete _tmpLine;
    _tmpLine = 0;

    if ( (  _to = qgraphicsitem_cast<DataItem*>(_graphScene->itemAt(pos))) ) {
        DocumentManager::self()
            ->activeDocument()
            ->activeDataStructure()
            ->addPointer( _from->data(),  _to->data() );
    }

    _to = 0;
    _from = 0;
    _working = false;
    return true;
}
