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
#include "graphDocument.h"
#include "NodeItem.h"
#include "OrientedEdgeItem.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>
#include <KDebug>
#include "Data.h"
#include "DataType.h"
#include "NodePropertiesWidget.h"
#include "MainWindow.h"
#include "edgepropertieswidget.h"
#include <DSPluginManager.h>

GraphScene::GraphScene(QObject *parent) : QGraphicsScene(parent) {
    _graphDocument = 0;
    _hideEdges = false;
    _datumPropertiesWidget = new DatumPropertiesWidget(qobject_cast<MainWindow*>(parent));
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

void GraphScene::setActiveGraph(DataType *g) {
    kDebug() << "Active Graph Set";
    _graph = g;
}

void GraphScene::updateAfter(QGraphicsItem *item) {
    if (_hidedEdges.contains(item)) return;
    _hidedEdges << item;
}

void GraphScene::hideGraph(DataType* g, bool visibility)
{
  QList<QGraphicsItem*> list = _hashGraphs.values(g);
  foreach(QGraphicsItem *i, list){
    i->setVisible(visibility);
  }
}


void GraphScene::setAction(QAction *action) {
    _action = qobject_cast<AbstractAction*>(action);
}

void GraphScene::setActiveDataTypeDocument(DataTypeDocument *gd) {

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

    int size = _graphDocument->size();
    if (size == 0) kDebug() << "############## SIZE = 0 ################";
    for (int i = 0; i < size; i++) {
        kDebug() << "Updating Graph at position: " << i;
        updateGraph(_graphDocument->at(i));
        connectGraphSignals(_graphDocument->at(i));
        kDebug() << "Graph Updated.";
    }
    
    connect( _graphDocument, SIGNAL(dataTypeCreated(DataType*)), this, SLOT(connectGraphSignals(DataType*)),Qt::UniqueConnection);
    kDebug() << "Graph Document Set" << _graphDocument -> name();
    createItems();
}
void GraphScene::createItems(){
    kDebug() << "Creating the graph items.";
    int size = _graphDocument->size();
    for (int i = 0; i < size; i++) {
        DataType *g = _graphDocument->at(i);
        kDebug() << "Creating " << g->data().size() << "data";
        for(int n = 0; n < g->data().size(); n++){
            createDatum( g->data()[n] );
        }
        kDebug() << "Creating" << g->pointers().size() << "pointers";
        for( int v = 0; v < g->pointers().size(); v++){
            createEdge( g->pointers()[v]);
        }
    }
}

void GraphScene::connectGraphSignals(DataType *g){
    connect( g, SIGNAL(datumCreated(Datum*)), this, SLOT(createDatum(Datum*)), Qt::UniqueConnection);
    connect( g, SIGNAL(pointerCreated(Pointer*)), this, SLOT(createEdge(Pointer*)), Qt::UniqueConnection);
}

void GraphScene::releaseDocument()
{
  _graphDocument->disconnect(this);
  disconnect(_graphDocument);
  for(int i = 0; i < _graphDocument->count(); ++i){
    _graphDocument->at(i)->disconnect(this);
    disconnect(_graphDocument->at(i));
  }
}


QGraphicsItem *GraphScene::createDatum(Datum *n) {
    DatumItem *nItem = (DatumItem*)(Rocs::DSPluginManager::instance()->datumItem(n));// new DatumItem(n);
    addItem(nItem);
    kDebug() << "Datum Item Created";
    return nItem;
}

QGraphicsItem *GraphScene::createEdge(Pointer *e) {
    QGraphicsItem *pointerItem = 0;
    pointerItem = Rocs::DSPluginManager::instance()->pointerItem(e);
//     pointerItem = new OrientedEdgeItem(e);

    addItem(pointerItem);
    kDebug() << "Edge Created";
    return pointerItem;
}

void GraphScene::mouseDoubleClickEvent (QGraphicsSceneMouseEvent * mouseEvent){
  mouseEvent->accept();
}

void GraphScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) {
    kDebug() << "Entering on Whell Event";
    DatumItem *nitem = qgraphicsitem_cast<DatumItem*>(itemAt(wheelEvent->scenePos()));
    if (!nitem) {
	kDebug() << "No Datum Item to Spand";
        wheelEvent->ignore();
        return;
    }

    Datum *movableDatum = nitem->datum();
    int numDegrees = wheelEvent->delta();
    if (wheelEvent->orientation() == Qt::Vertical) {
	kDebug() << "Upgrading datum size";
        if (numDegrees > 0)
        {
            movableDatum->setWidth(movableDatum->width()+0.25);
            nitem->update();
        }
        else if (movableDatum->width() > 0.5)
        {
            movableDatum->setWidth(movableDatum->width()-0.25);
            nitem->update();
        }
    }
    wheelEvent->accept();
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    _action->executeMove(mouseEvent->scenePos());
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (mouseEvent->button() == Qt::MidButton) {
        DatumItem *nitem = qgraphicsitem_cast<DatumItem*>(itemAt(mouseEvent->scenePos()));
        if (!nitem) return;
        Datum *movableDatum = nitem->datum();
        movableDatum->setWidth(1);
    }
    else if( mouseEvent->button() == Qt::RightButton){
        QGraphicsItem *i = itemAt(mouseEvent->scenePos());
        if (DatumItem *nItem = qgraphicsitem_cast<DatumItem*>(i)){
            _datumPropertiesWidget->setDatum(nItem, mouseEvent->screenPos());
        }
        else if (OrientedEdgeItem *eItem = qgraphicsitem_cast<OrientedEdgeItem*>(i)){
            _pointerPropertiesWidget->setPointer(eItem->pointer(), mouseEvent->screenPos());
        }
    }else if( mouseEvent -> button() == Qt::LeftButton){
        _action->executePress(mouseEvent->scenePos());
    }
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
     if (mouseEvent->button() == Qt::LeftButton){
	_action->executeRelease(mouseEvent->scenePos());
     }
}

void GraphScene::keyPressEvent(QKeyEvent *) {

}

void GraphScene::updateGraph(DataType *g) {
    kDebug() << "Removed Graph from the hash";

    kDebug() << "Creating" << g->data().size() << "data";
    QList<Datum*> data = g->data();
    foreach(Datum *n, data) {
        n->setName(n->name()); // just a dummy update trigger;
    }

    kDebug() << "Creating" << g->data().size() << "pointers";
    QList<Pointer*> pointers = g->pointers();
    foreach(Pointer *e, pointers) {
       e->setName(e->name()); // just a dummy update trigger.
    }
}

void GraphScene::updateDocument() {
    if (_graphDocument == 0) {
        kDebug() << "Graph Document is null. Please hit the developer.";
        return;
    }

    clear();
    kDebug() << "Graph Document Size: " << _graphDocument->size();
    int size = _graphDocument->size();

    for (int i = 0; i < size; i++) {
        updateGraph( _graphDocument->at(i) );
	kDebug() << "updated Graph at " << i;
    }
}
