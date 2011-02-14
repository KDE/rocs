/***************************************************************************
 * main.cpp
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/

#include "GraphScene.h"
#include "AbstractAction.h"

#include "Pointer.h"
#include "Document.h"
#include "DataItem.h"
#include "PointerItem.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>
#include <KDebug>
#include "Data.h"
#include "Pointer.h"
#include "Document.h"
#include "DataStructure.h"
#include "DocumentManager.h"
#include "NodePropertiesWidget.h"
#include "MainWindow.h"
#include "edgepropertieswidget.h"
#include <DataStructurePluginManager.h>

GraphScene::GraphScene(QObject *parent) : QGraphicsScene(parent) {
    _graphDocument = 0;
    _hideEdges = false;
    _datumPropertiesWidget = new DataPropertiesWidget(qobject_cast<MainWindow*>(parent));
    _pointerPropertiesWidget = new PointerPropertiesWidget(qobject_cast<MainWindow*>(parent));
    _action = 0;
}

bool GraphScene::hideEdges() {
    return _hideEdges;
}

void GraphScene::setHideEdges(bool h) {
    _hideEdges = h;
    if ( ! _hideEdges ) {
        foreach(QGraphicsItem *i, _hidedEdges) {
            i->update();
        }
    }
}

void GraphScene::setActiveGraph(DataStructure *g) {
    kDebug() << "Active Graph Set";
    _graph = g;
}

void GraphScene::updateAfter(QGraphicsItem *item) {
    if (_hidedEdges.contains(item)) return;
    _hidedEdges << item;
}

void GraphScene::hideGraph(DataStructure* g, bool visibility)
{
  QList<QGraphicsItem*> list = _hashGraphs.values(g);
  foreach(QGraphicsItem *i, list){
    i->setVisible(visibility);
  }
}


void GraphScene::setAction(QAction *action) {
    _action = qobject_cast<AbstractAction*>(action);
}

void GraphScene::setActiveDocument(Document *gd) {

    if (gd == 0) {
        releaseDocument();
        return;
    }
    _graphDocument = gd;
    
    setSceneRect(QRectF(0,0, gd->width(), gd->height() ));
    QGraphicsRectItem *n = new QGraphicsRectItem(0,0, gd->width(), gd->height());
    n->setFlag(QGraphicsItem::ItemIsSelectable, false);
    n->setZValue(-1000);
    addItem(n);

    int size = gd->dataStructures().size();
    for (int i = 0; i < size; i++) {
        updateGraph(gd->dataStructures().at(i));
        connectGraphSignals(gd->dataStructures().at(i));
    }
    
    connect( gd, SIGNAL(dataTypeCreated(DataStructure*)),
             this, SLOT(connectGraphSignals(DataStructure*)));
    
    createItems();
    
}
void GraphScene::createItems(){
    foreach(DataStructure *g, DocumentManager::self()->activeDocument()->dataStructures()){
        foreach( Data *d, g->dataList()) createData( d );
        foreach( Pointer *p, g->pointers()) createEdge( p);
    }
}

void GraphScene::connectGraphSignals(DataStructure *g){
    connect( g, SIGNAL(dataCreated(Data*)), this, SLOT(createData(Data*)));
    connect( g, SIGNAL(pointerCreated(Pointer*)), this, SLOT(createEdge(Pointer*)));
}

void GraphScene::releaseDocument(){
  _graphDocument->disconnect(this);
  disconnect(_graphDocument);
  foreach(DataStructure *ds, _graphDocument->dataStructures()){
    ds->disconnect(this);
    disconnect(ds);
  }
}


QGraphicsItem *GraphScene::createData(Data *n) {
    DataItem *nItem = (DataItem*)(DataStructurePluginManager::self()->dataItem(n));
    addItem(nItem);
    return nItem;
}

QGraphicsItem *GraphScene::createEdge(Pointer *e) {
    QGraphicsItem *pointerItem = 0;
    pointerItem = DataStructurePluginManager::self()->pointerItem(e);
    addItem(pointerItem);
    return pointerItem;
}

void GraphScene::mouseDoubleClickEvent (QGraphicsSceneMouseEvent * mouseEvent){
  mouseEvent->accept();
}

void GraphScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) {
    DataItem *nitem = qgraphicsitem_cast<DataItem*>(itemAt(wheelEvent->scenePos()));
    if (!nitem) {
        wheelEvent->ignore();
        return;
    }

    Data *movableData = nitem->datum();
    int numDegrees = wheelEvent->delta();
    if (wheelEvent->orientation() == Qt::Vertical) {
        if (numDegrees > 0){
            movableData->setWidth(movableData->width()+0.25);
            nitem->update();
        }
        else if (movableData->width() > 0.5){
            movableData->setWidth(movableData->width()-0.25);
            nitem->update();
        }
    }
    wheelEvent->accept();
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    _action->executeMove(mouseEvent->scenePos());
    mouseEvent->accept();
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (mouseEvent->button() == Qt::MidButton) {
        DataItem *nitem = qgraphicsitem_cast<DataItem*>(itemAt(mouseEvent->scenePos()));
        if (!nitem) return;
        Data *movableData = nitem->datum();
        movableData->setWidth(1);
    }
    else if( mouseEvent->button() == Qt::RightButton){
        QGraphicsItem *i = itemAt(mouseEvent->scenePos());
        if (DataItem *nItem = qgraphicsitem_cast<DataItem*>(i)){
            _datumPropertiesWidget->setData(nItem, mouseEvent->screenPos());
        }
        else if (OrientedEdgeItem *eItem = qgraphicsitem_cast<OrientedEdgeItem*>(i)){
            _pointerPropertiesWidget->setPointer(eItem->pointer(), mouseEvent->screenPos());
        }
    }else if( mouseEvent -> button() == Qt::LeftButton){
        _action->executePress(mouseEvent->scenePos());
    }
    mouseEvent->accept();
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
     if (mouseEvent->button() == Qt::LeftButton){
	_action->executeRelease(mouseEvent->scenePos());
     }
}

void GraphScene::keyPressEvent(QKeyEvent *keyEvent) {
    keyEvent->accept();
}

void GraphScene::updateGraph(DataStructure *g) {
    foreach(Data *n, g->dataList()) {
        n->setName(n->name());
    }
    
    foreach(Pointer *e, g->pointers()) {
       e->setName(e->name()); 
    }
}

void GraphScene::updateDocument() {
    if (_graphDocument == 0) {
        return;
    }

    clear();
    int size = _graphDocument->dataStructures().size();

    for (int i = 0; i < size; i++) {
        updateGraph( _graphDocument->dataStructures().at(i) );
    }
}
