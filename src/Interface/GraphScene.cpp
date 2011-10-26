/* 
    This file is part of Rocs,
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2011       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either 
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public 
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "GraphScene.h"

#include "Data.h"
#include "Pointer.h"
#include "DataStructure.h"
#include "Document.h"
#include <DataStructurePluginManager.h>
#include "DocumentManager.h"

#include "MainWindow.h"
#include "DataItem.h"
#include "PointerItem.h"
#include "NodePropertiesWidget.h"
#include "edgepropertieswidget.h"

#include "AbstractAction.h"
#include "AlignAction.h"
#include "AddDataAction.h"
#include "DeleteAction.h"
#include "ZoomAction.h"

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>
#include <QMenu>

#include <KDebug>


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
        gd->setLeft(gd->left()-(_minWidth-gd->width())/2);
        gd->setRight(gd->right()+(_minWidth-gd->width())/2);
    }
    if (gd->height()<_minHeight) {
        gd->setTop(gd->top()-(_minHeight-gd->height())/2);
        gd->setBottom(gd->bottom()+(_minHeight-gd->height())/2);
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

void GraphScene::setActiveGraph(DataStructurePtr g) {
    kDebug() << "Active Graph Set";
    _graph = g;
}

void GraphScene::updateAfter(QGraphicsItem *item) {
    if (_hidedEdges.contains(item)) return;
    _hidedEdges << item;
}

void GraphScene::hideGraph(DataStructurePtr g, bool visibility)
{
    QList<QGraphicsItem*> list = _hashGraphs.values(g.get());
    foreach(QGraphicsItem *i, list){
        i->setVisible(visibility);
    }
}

void GraphScene::setAction(QAction *action) {
    if(_action){
        removeEventFilter(_action);
    }
    _action = qobject_cast<AbstractAction*>( action );
    action->setChecked(true);
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
        gd->setLeft(gd->left()-(_minWidth-gd->width())/2);
        gd->setRight(gd->right()+(_minWidth-gd->width())/2);
    }
    if (gd->height()<_minHeight) {
        gd->setTop(gd->top()-(_minHeight-gd->height())/2);
        gd->setBottom(gd->bottom()+(_minHeight-gd->height())/2);
    }

    resize();

    int size = gd->dataStructures().size();
    for (int i = 0; i < size; i++) {
        updateGraph(gd->dataStructures().at(i));
        connectGraphSignals(gd->dataStructures().at(i));
    }

    connect( gd, SIGNAL(dataStructureCreated(DataStructurePtr)),
             this, SLOT(connectGraphSignals(DataStructurePtr)));

    connect( gd, SIGNAL(resized()), this, SLOT(resize()));

    createItems();
}

void GraphScene::createItems(){
    foreach(DataStructurePtr g, _graphDocument->dataStructures()){
        foreach( DataPtr d, g->dataList()) createData( d );
        foreach( PointerPtr p, g->pointers()) createEdge( p );
    }
}

void GraphScene::connectGraphSignals(DataStructurePtr g){
    connect( g.get(), SIGNAL(dataCreated(DataPtr)), this, SLOT(createData(DataPtr)));
    connect( g.get(), SIGNAL(pointerCreated(PointerPtr)), this, SLOT(createEdge(PointerPtr)));
}

void GraphScene::releaseDocument(){
    _graphDocument->disconnect(this);
    disconnect(_graphDocument);
    foreach(DataStructurePtr ds, _graphDocument->dataStructures()){
        ds->disconnect(this);
        disconnect(ds.get());
    }
}

QGraphicsItem *GraphScene::createData(DataPtr n) {
    DataItem *nItem = (DataItem*)(DataStructurePluginManager::self()->dataItem(n));
    addItem(nItem);
    addItem(nItem->name());
    addItem(nItem->value());
    
    return nItem;
}

QGraphicsItem *GraphScene::createEdge(PointerPtr e) {
    QGraphicsItem *pointerItem = 0;
    pointerItem = DataStructurePluginManager::self()->pointerItem(e);
    addItem(pointerItem);
    return pointerItem;
}

void GraphScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) {
    DataItem *nitem = qgraphicsitem_cast<DataItem*>(itemAt(wheelEvent->scenePos()));
    if (!nitem) {
        wheelEvent->ignore();
        return;
    }

    DataPtr movableData = nitem->data();
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
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void  GraphScene::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent *mouseEvent ) {
   if( mouseEvent->button() == Qt::LeftButton){
        QGraphicsItem *i = itemAt(mouseEvent->scenePos());
        if (DataItem *nItem = qgraphicsitem_cast<DataItem*>(i)){
            _datumPropertiesWidget->setData(nItem, mouseEvent->screenPos());
        }
        else if (PointerItem *eItem = qgraphicsitem_cast<PointerItem*>(i)){
            _pointerPropertiesWidget->setPointer(eItem->pointer(), mouseEvent->screenPos());
        }
    }
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphScene::keyPressEvent(QKeyEvent *keyEvent) {
    keyEvent->accept();
    emit (keyPressed(keyEvent));
}


void GraphScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* event) {
    event->accept();
    QMenu menu; // the context menu
    QMenu *menuDataStructure = menu.addMenu( i18n("Data Structure") );
    QMenu *menuSelected = menu.addMenu( i18n("Selected") );
    
    
    // prepare some context information
    bool contextAtItem = false;
    QGraphicsItem *i = itemAt(event->scenePos());
    if ((qgraphicsitem_cast<DataItem*>(i)) || (qgraphicsitem_cast<PointerItem*>(i))){
        contextAtItem = true;
    }
    
    // zoom menu
    QMenu *menuZoom = new QMenu( i18n("Zoom") );
    ZoomAction *zoomAction = new ZoomAction(this, 0);
    QAction* zoomInAction = new QAction( i18n("In"), zoomAction);
    QAction *zoomOutAction = new QAction(i18n("Out"), zoomAction);
    QAction *zoomResetAction = new QAction(i18n("Reset"), zoomAction);
    menuZoom->addAction( zoomInAction );
    menuZoom->addAction( zoomOutAction );
    menuZoom->addAction( zoomResetAction );
        
    // alignment menu
    QMenu *menuDataStructureAlign = new QMenu( i18n("Align") );
    QAction *alignDataStructureBottom = new AlignAction ( i18n ( "Bottom" ),  AlignAction::Bottom, this,0, false );
    QAction *alignDataStructureCenter = new AlignAction ( i18n ( "Center" ),AlignAction::HCenter,this,0, false );
    QAction *alignDataStructureTop    = new AlignAction ( i18n ( "Top" ),   AlignAction::Top, this,0, false );
    QAction *alignDataStructureLeft   = new AlignAction ( i18n ( "Left" ),  AlignAction::Left, this,0, false );
    QAction *alignDataStructureRight  = new AlignAction ( i18n ( "Right" ), AlignAction::Right, this,0, false );
    QAction *alignDataStructureCircle = new AlignAction ( i18n ( "Circle" ),  AlignAction::Circle, this,0, false );
    QAction *alignDataStructureTree   = new AlignAction ( i18n ( "Tree" ),  AlignAction::MinCutTree, this,0, false );
    menuDataStructureAlign->addAction ( alignDataStructureBottom );
    menuDataStructureAlign->addAction ( alignDataStructureCenter );
    menuDataStructureAlign->addAction ( alignDataStructureTop );
    menuDataStructureAlign->addAction ( alignDataStructureLeft );
    menuDataStructureAlign->addAction ( alignDataStructureRight );
    menuDataStructureAlign->addAction ( alignDataStructureCircle );
    menuDataStructureAlign->addAction ( alignDataStructureTree );

    QMenu *menuSelectedAlign = new QMenu( i18n("Align") );
    menuSelectedAlign->addAction ( new AlignAction ( i18n ( "Bottom" ),  AlignAction::Bottom, this,0 ) );
    menuSelectedAlign->addAction ( new AlignAction ( i18n ( "Center" ),AlignAction::HCenter,this,0 ) );
    menuSelectedAlign->addAction ( new AlignAction ( i18n ( "Top" ),   AlignAction::Top, this,0 ) );
    menuSelectedAlign->addAction ( new AlignAction ( i18n ( "Left" ),  AlignAction::Left, this,0 ) );
    menuSelectedAlign->addAction ( new AlignAction ( i18n ( "Right" ), AlignAction::Right, this,0 ) );
    menuSelectedAlign->addAction ( new AlignAction ( i18n ( "Circle" ),  AlignAction::Circle, this,0 ) );
    menuSelectedAlign->addAction ( new AlignAction ( i18n ( "Tree" ),  AlignAction::MinCutTree, this,0 ) );

    
    // puzzling the menu together
    AddDataAction *addAction = new AddDataAction(this);
    DeleteAction *deleteDataStructureAction = new DeleteAction(this, 0);
    DeleteAction *deleteSelectedAction = new DeleteAction(this, 0);
    DeleteAction *deleteItemAction = new DeleteAction(this, 0);
    QAction *propertyAction = new QAction(i18n("Properties"), this); //FIXME remove hack
    menuSelected->addMenu(menuSelectedAlign);
    menuSelected->addAction(deleteSelectedAction);
    menuDataStructure->addMenu(menuDataStructureAlign);
    menuDataStructure->addAction(deleteDataStructureAction);
    
    // activate/deactivate context depending on where the user click
    if (selectedItems().count()==0) {
        menuSelected->setDisabled(true);
    }
    if (!contextAtItem) {
        menuDataStructure->setDisabled(true);
        menu.addAction(addAction);
        menu.addMenu(menuZoom);
    } else {
        menu.addAction(propertyAction);
        menu.addAction(deleteItemAction);
    }

    // TODO this is not a nice code
    // refactor this for SC 4.9
    QAction* selectedItem = menu.exec(event->screenPos());
    if (selectedItem == addAction) {
        addAction->executePress(event->scenePos());
    }
    if (selectedItem == deleteItemAction) {
        deleteItemAction->executePress(event->scenePos());
    }
    if (selectedItem == deleteDataStructureAction) {
        deleteItemAction->setDeleteTarget(DeleteAction::DATA_STRUCTURE);
        deleteItemAction->executePress(event->scenePos());
    }
    if (selectedItem == deleteSelectedAction) {
        deleteItemAction->setDeleteTarget(DeleteAction::SELECTED);
        deleteItemAction->executePress(event->scenePos());
    }
    if (selectedItem == zoomInAction) {
        zoomAction->zoomIn(event->scenePos());
    }
    if (selectedItem == zoomOutAction) {
        zoomAction->zoomOut(event->scenePos());
    }
    if (selectedItem == zoomResetAction) {
        zoomAction->zoomReset();
    }
    if ( selectedItem == alignDataStructureBottom ||
        selectedItem == alignDataStructureTop ||
        selectedItem == alignDataStructureCenter ||
        selectedItem == alignDataStructureLeft ||
        selectedItem == alignDataStructureRight ||
        selectedItem == alignDataStructureCircle ||
        selectedItem == alignDataStructureTree )
    {
        AlignAction* action = static_cast<AlignAction*>(selectedItem);
        QGraphicsItem *i = itemAt(event->scenePos());
        if (DataItem *nItem = qgraphicsitem_cast<DataItem*>(i)){
            action->setDataStructure(nItem->data()->dataStructure());
        }
        action->align();
        action->unsetDataStructure();
    }
    
    if (selectedItem == propertyAction) {
        QGraphicsItem *i = itemAt(event->scenePos());
        if (DataItem *nItem = qgraphicsitem_cast<DataItem*>(i)){
            _datumPropertiesWidget->setData(nItem, event->screenPos());
        }
        else if (PointerItem *eItem = qgraphicsitem_cast<PointerItem*>(i)){
            _pointerPropertiesWidget->setPointer(eItem->pointer(), event->screenPos());
        }
    }
    else
    {
        // nothing was chosen
    }
}


void GraphScene::updateGraph(DataStructurePtr g) {
    foreach(DataPtr n, g->dataList()) {
        n->setName(n->name());
    }

    foreach(PointerPtr e, g->pointers()) {
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
    QRectF newSize(_graphDocument->left(), // x
                   _graphDocument->top(),  // y
                   _graphDocument->right()-_graphDocument->left(), // width
                   _graphDocument->bottom()-_graphDocument->top()); // height

    setSceneRect( newSize );
    emit resized();
}
