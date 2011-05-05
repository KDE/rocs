/***************************************************************************
 * This file is part of the KDE project
 * Copyright (C) by
 *  2004-2007   Tomaz Canabrava <tomaz.canabrava@gmail.com>
 *  2011        Andreas Cord-Landwehr <cola@uni-paderborn.de>
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

GraphScene::GraphScene( QObject *parent) :
    QGraphicsScene(parent)
{
    _graphDocument = 0;
    _hideEdges = false;
    _datumPropertiesWidget = new DataPropertiesWidget(qobject_cast<MainWindow*>(parent));
    _pointerPropertiesWidget = new PointerPropertiesWidget(qobject_cast<MainWindow*>(parent));
    _action = 0;
    _minHeight = 0;
    _minWidth = 0;

    // first scene resize will resize to actual whiteboard size
    setSceneRect(0, 0, 0, 0);
}

void GraphScene::updateMinSize(qreal minWidth, qreal minHeight)
{
    _minWidth = minWidth;
    _minHeight = minHeight;
    setSceneRect(-minWidth/2, -minHeight/2, minWidth/2, minHeight/2);

    Document *gd = DocumentManager::self()->activeDocument();
    if (gd->width()<_minWidth) {
        gd->setXLeft(gd->xLeft()-(_minWidth-gd->width())/2);
        gd->setXRight(gd->xRight()+(_minWidth-gd->width())/2);
    }
    if (gd->height()<_minHeight) {
        gd->setYTop(gd->yTop()-(_minHeight-gd->height())/2);
        gd->setYBottom(gd->yBottom()+(_minHeight-gd->height())/2);
    }
    gd->changeMinimalSize(minWidth, minHeight);
    resize();
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
    if(_action){
        removeEventFilter(_action);
    }
    _action = qobject_cast<AbstractAction*>( action );
    installEventFilter(action);
}

void GraphScene::setActiveDocument() {
    kDebug() << "Setting the document in the scene";
    Document *gd = DocumentManager::self()->activeDocument();
    if (_graphDocument == gd){
        return;
    }else if ( gd == 0) {
        releaseDocument();
        return;
    }

     // adapt document to scene if too small
    _graphDocument = gd;
    if (gd->width()<_minWidth) {
        gd->setXLeft(gd->xLeft()-(_minWidth-gd->width())/2);
        gd->setXRight(gd->xRight()+(_minWidth-gd->width())/2);
    }
    if (gd->height()<_minHeight) {
        gd->setYTop(gd->yTop()-(_minHeight-gd->height())/2);
        gd->setYBottom(gd->yBottom()+(_minHeight-gd->height())/2);
    }

    resize();

    int size = gd->dataStructures().size();
    for (int i = 0; i < size; i++) {
        updateGraph(gd->dataStructures().at(i));
        connectGraphSignals(gd->dataStructures().at(i));
    }

    connect( gd, SIGNAL(dataStructureCreated(DataStructure*)),
             this, SLOT(connectGraphSignals(DataStructure*)));

    connect( gd, SIGNAL(resized()), this, SLOT(resize()));

    createItems();
}

void GraphScene::createItems(){
    foreach(DataStructure *g, _graphDocument->dataStructures()){
        foreach( Data *d, g->dataList()) createData( d );
        foreach( Pointer *p, g->pointers()) createEdge( p );
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
    addItem(nItem->name());
    addItem(nItem->value());
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

    Data *movableData = nitem->data();
    int numDegrees = wheelEvent->delta();
    if (wheelEvent->orientation() == Qt::Vertical) {
        if (numDegrees > 0 && movableData->width() + 0.10 < 2.0){
            movableData->setWidth(movableData->width()+0.1);
            nitem->update();
        }
        else if (movableData->width() - 0.10 > 0.15){
            movableData->setWidth(movableData->width()-0.1);
            nitem->update();
        }
    }
    wheelEvent->accept();
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    Q_UNUSED(mouseEvent);
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (mouseEvent->button() == Qt::MidButton) {
        if (DataItem *nItem = qgraphicsitem_cast<DataItem*>(itemAt(mouseEvent->scenePos()))){
            nItem->data()->setWidth(1);
        }
    }
    else if( mouseEvent->button() == Qt::RightButton){
        QGraphicsItem *i = itemAt(mouseEvent->scenePos());
        if (DataItem *nItem = qgraphicsitem_cast<DataItem*>(i)){
            _datumPropertiesWidget->setData(nItem, mouseEvent->screenPos());
        }
        else if (PointerItem *eItem = qgraphicsitem_cast<PointerItem*>(i)){
            _pointerPropertiesWidget->setPointer(eItem->pointer(), mouseEvent->screenPos());
        }
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
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

void GraphScene::resize() {
    QRectF newSize(_graphDocument->xLeft(), // x
                   _graphDocument->yTop(),  // y
                   _graphDocument->xRight()-_graphDocument->xLeft(), // width
                   _graphDocument->yBottom()-_graphDocument->yTop()); // height

    setSceneRect( newSize );
    emit resized();
}