/*
    This file is part of Rocs,
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2011-2013  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include <DataStructureBackendManager.h>
#include "DocumentManager.h"

#include "DataItem.h"
#include "PointerItem.h"
#include "Interface/DataPropertiesWidget.h"
#include "Interface/PointerPropertiesWidget.h"

#include "Actions/AbstractAction.h"
#include "Actions/AlignAction.h"
#include "Actions/AssignValueAction.h"
#include "Actions/AddDataAction.h"
#include "Actions/DeleteAction.h"
#include "Actions/PropertiesDialogAction.h"
#include "Actions/ZoomAction.h"
#include <GraphVisualEditor.h>

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QGraphicsView>
#include <QPointer>

#include <KDebug>


GraphScene::GraphScene(QObject *parent)
    : QGraphicsScene(parent)
    , _graphDocument(0)
    , _action(0)
    , _zoomFactor(1)
{
}

void GraphScene::setActiveGraph(DataStructurePtr g)
{
    kDebug() << "Active Graph Set";
    _graph = g;
    connect( _graph.get(), SIGNAL(changed()), this, SLOT(update()), Qt::UniqueConnection);
}

void GraphScene::setAction(QAction *action)
{
    if (_action) {
        removeEventFilter(_action);
    }
    _action = qobject_cast<AbstractAction*>(action);
    action->setChecked(true);
    installEventFilter(action);
}

void GraphScene::setActiveDocument()
{
    kDebug() << "Setting the document in the scene";
    Document *document = DocumentManager::self().activeDocument();
    if (_graphDocument == document) {
        return;
    } else if (document == 0) {
        releaseDocument();
        return;
    }

    // set document
    _graphDocument = document;
    resize();

    int size = document->dataStructures().size();
    for (int i = 0; i < size; i++) {
        updateGraph(document->dataStructures().at(i));
        connectGraphSignals(document->dataStructures().at(i));
    }

    connect(document, SIGNAL(dataStructureCreated(DataStructurePtr)),
            this, SLOT(connectGraphSignals(DataStructurePtr)));
    connect(document, SIGNAL(sceneRectChanged(QRectF)),
            this, SLOT(resize()));

    createItems();
}

void GraphScene::createItems()
{
    foreach(DataStructurePtr g, _graphDocument->dataStructures()) {
        createItems(g);
    }
}

void GraphScene::createItems(DataStructurePtr dataStructure)
{
    foreach(int type, _graphDocument->dataTypeList()) {
        foreach(DataPtr data, dataStructure->dataList(type)) createData(data);
    }
    foreach(int type, _graphDocument->pointerTypeList()) {
        foreach(PointerPtr pointer, dataStructure->pointers(type)) createEdge(pointer);
    }
}

void GraphScene::connectGraphSignals(DataStructurePtr g)
{
    connect(g.get(), SIGNAL(dataCreated(DataPtr)), this, SLOT(createData(DataPtr)));
    connect(g.get(), SIGNAL(pointerCreated(PointerPtr)), this, SLOT(createEdge(PointerPtr)));
}

void GraphScene::releaseDocument()
{
    _graphDocument->disconnect(this);
    disconnect(_graphDocument);
    foreach(DataStructurePtr ds, _graphDocument->dataStructures()) {
        ds->disconnect(this);
        disconnect(ds.get());
    }
}

QGraphicsItem * GraphScene::createData(DataPtr n)
{
    DataItem *nItem = (DataItem*)(DataStructureBackendManager::self().dataItem(n));
    addItem(nItem);
    addItem(nItem->propertyListItem());
    return nItem;
}

QGraphicsItem * GraphScene::createEdge(PointerPtr e)
{
    PointerItem *pointerItem = (PointerItem*)DataStructureBackendManager::self().pointerItem(e);
    addItem(pointerItem);
    addItem(pointerItem->propertyListItem());
    return pointerItem;
}

void GraphScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent)
{
    DataItem *nitem = qgraphicsitem_cast<DataItem*>(itemAt(wheelEvent->scenePos()));
    if (!nitem) {
        wheelEvent->ignore();
        return;
    }

    DataPtr movableData = nitem->data();
    int numDegrees = wheelEvent->delta();
    if (wheelEvent->orientation() == Qt::Vertical) {
        if (numDegrees > 0 && movableData->width() + 0.10 < 2.0) {
            movableData->setWidth(movableData->width() + 0.1);
            nitem->update();
        } else if (movableData->width() - 0.10 > 0.15) {
            movableData->setWidth(movableData->width() - 0.1);
            nitem->update();
        }
    }
    wheelEvent->accept();
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent);
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::MidButton) {
        if (DataItem *nItem = qgraphicsitem_cast<DataItem*>(itemAt(mouseEvent->scenePos()))) {
            nItem->data()->setWidth(1);
        }
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton) {
        QGraphicsItem *i = itemAt(mouseEvent->scenePos());
        if (DataItem *nItem = qgraphicsitem_cast<DataItem*>(i)) {
            QPointer<DataPropertiesWidget> dialog = new DataPropertiesWidget(nItem->data());
            dialog->setPosition(mouseEvent->screenPos());
            dialog->exec();
        } else if (PointerItem *eItem = qgraphicsitem_cast<PointerItem*>(i)) {
            QPointer<PointerPropertiesWidget> dialog = new PointerPropertiesWidget(eItem->pointer());
            dialog->setPosition(mouseEvent->screenPos());
            dialog->exec();
        }
    }
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphScene::keyPressEvent(QKeyEvent *keyEvent)
{
    keyEvent->accept();
    emit keyPressed(keyEvent);
}

void GraphScene::keyReleaseEvent(QKeyEvent *keyEvent)
{
    keyEvent->accept();
    emit keyReleased(keyEvent);
}

void GraphScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->accept();
    _contextMenu = createContextMenu(event->scenePos(), event->screenPos());
    _contextMenu->exec(event->screenPos());
}


void GraphScene::updateGraph(DataStructurePtr g)
{
    //TODO this is a workaround to trigger updates
    DataList dataList = g->dataListAll();
    foreach(DataPtr n, dataList) {
        n->setColor(n->color());
    }
    PointerList pointerList = g->pointerListAll();
    foreach(PointerPtr e, pointerList) {
        e->setWidth(e->width());
    }
}

void GraphScene::updateDocument()
{
    if (_graphDocument == 0) {
        return;
    }

    clear();
    int size = _graphDocument->dataStructures().size();

    for (int i = 0; i < size; i++) {
        updateGraph(_graphDocument->dataStructures().at(i));
    }
}

void GraphScene::resize()
{
    if (!_graphDocument) {
        return;
    }

    // set scene rect according to visual editor rect
    QRectF graphRect = _graphDocument->sceneRect();
    QRectF editorRect = GraphVisualEditor::self()->rect();

    QRectF sceneRect = graphRect;
    sceneRect.adjust(-200, -200, 200, 200); // increase whiteboard in each direction around data elements
    if (editorRect.left() < sceneRect.left()) {
        sceneRect.setLeft(editorRect.left());
    }
    if (editorRect.right() > sceneRect.right()) {
        sceneRect.setRight(editorRect.right());
    }
    if (editorRect.top() < sceneRect.top()) {
        sceneRect.setTop(editorRect.top());
    }
    if (editorRect.bottom() > sceneRect.bottom()) {
        sceneRect.setBottom(editorRect.bottom());
    }

    setSceneRect(sceneRect);
    emit resized();
}

qreal GraphScene::zoomFactor() const
{
    return _zoomFactor;
}

void GraphScene::zoomBy(qreal scaleFactor)
{
    if (scaleFactor == 0 || _zoomFactor*scaleFactor < 0.001) {
        return;
    }
    views().at(0)->scale(scaleFactor, scaleFactor);
    _zoomFactor *= scaleFactor;
    emit zoomFactorChanged(_zoomFactor);
}

void GraphScene::zoomTo(qreal scaleFactor)
{
    if (scaleFactor == 0) {
        return;
    }
    views().at(0)->resetMatrix();
    views().at(0)->scale(scaleFactor, scaleFactor);
    _zoomFactor = scaleFactor;
    emit zoomFactorChanged(_zoomFactor);
}

void GraphScene::zoomToRect(const QRectF &rect)
{
    views().at(0)->fitInView(rect, Qt::KeepAspectRatioByExpanding);
    //FIXME add computation for new zoomfactor!
    emit zoomFactorChanged(_zoomFactor);
}


void GraphScene::resetZoom()
{
    views().at(0)->resetMatrix();
}

void GraphScene::centerOn(const QPointF &pos)
{
    views().at(0)->centerOn(pos);
}


QMenu * GraphScene::createContextMenu(const QPointF &scenePosition, const QPointF &screenPosition)
{
    QMenu *menu = new QMenu; // the context menu
    QMenu *menuDataStructure = menu->addMenu(i18nc("@title:menu", "Data Structure"));
    QMenu *menuSelected = menu->addMenu(i18nc("@title:menu", "Selected"));

    // prepare some context information
    bool contextAtItem = false;
    DataStructurePtr contextDataStructure;
    DataPtr contextData;
    PointerPtr contextPointer;
    QGraphicsItem *item = itemAt(scenePosition);
    DataItem *dataItem;
    PointerItem *pointerItem;
    if ((dataItem = (qgraphicsitem_cast<DataItem*>(item)))) {
        contextAtItem = true;
        contextDataStructure = dataItem->data()->dataStructure();
        contextData = dataItem->data();
    }
    if ((pointerItem = (qgraphicsitem_cast<PointerItem*>(item)))) {
        contextAtItem = true;
        contextDataStructure = pointerItem->pointer()->dataStructure();
        contextPointer = pointerItem->pointer();
    }

    // zoom menu
    QMenu *menuZoom = new QMenu(i18nc("@title:menu", "Zoom"));
    ZoomAction *zoomAction = new ZoomAction(this, 0);
    QAction* zoomInAction = new QAction(i18nc("@action:inmenu Zoom", "In"), zoomAction);
    QAction *zoomOutAction = new QAction(i18nc("@action:inmenu Zoom", "Out"), zoomAction);
    QAction *zoomResetAction = new QAction(i18nc("@action:inmenu Zoom", "Reset"), zoomAction);
    menuZoom->addAction(zoomInAction);
    menuZoom->addAction(zoomOutAction);
    menuZoom->addAction(zoomResetAction);
    connect(zoomInAction, SIGNAL(triggered(bool)), zoomAction, SLOT(zoomInCenter()));
    connect(zoomOutAction, SIGNAL(triggered(bool)), zoomAction, SLOT(zoomOutCenter()));
    connect(zoomResetAction, SIGNAL(triggered(bool)), zoomAction, SLOT(zoomReset()));

    // alignment menu
    QMenu *menuDataStructureAlign = new QMenu(i18n("Align"));
    AlignAction *alignDataStructureBottom = new AlignAction(i18nc("@action:inmenu align elements at bottom", "Bottom"),  AlignAction::Bottom, this);
    AlignAction *alignDataStructureCenter = new AlignAction(i18nc("@action:inmenu align elements at center", "Center"), AlignAction::HCenter, this);
    AlignAction *alignDataStructureTop    = new AlignAction(i18nc("@action:inmenu align elements at top", "Top"),   AlignAction::Top, this);
    AlignAction *alignDataStructureLeft   = new AlignAction(i18nc("@action:inmenu align elements left", "Left"),  AlignAction::Left, this);
    AlignAction *alignDataStructureRight  = new AlignAction(i18nc("@action:inmenu align elements right", "Right"), AlignAction::Right, this);
    AlignAction *alignDataStructureCircle = new AlignAction(i18nc("@action:inmenu align elements as a circle", "Circle"),  AlignAction::Circle, this);
    AlignAction *alignDataStructureTree   = new AlignAction(i18nc("@action:inmenu align elements such that crossing edges are minimized", "Minimize Crossing Edges"), AlignAction::MinCutTree, this);

    if (contextDataStructure) {
        //FIXME only default data type considered
        alignDataStructureBottom->registerData(contextDataStructure->dataList(0));
        alignDataStructureCenter->registerData(contextDataStructure->dataList(0));
        alignDataStructureTop->registerData(contextDataStructure->dataList(0));
        alignDataStructureLeft->registerData(contextDataStructure->dataList(0));
        alignDataStructureRight->registerData(contextDataStructure->dataList(0));
        alignDataStructureCircle->registerData(contextDataStructure->dataList(0));
        alignDataStructureTree->registerData(contextDataStructure->dataList(0));
    }
    if (contextAtItem) {
        PropertiesDialogAction *dataStructurePropertiesAction = new PropertiesDialogAction(i18nc("@action:inmenu", "Properties"), contextDataStructure, this);
        dataStructurePropertiesAction->setPosition(screenPosition);
        menuDataStructure->addAction(dataStructurePropertiesAction);
    }

    menuDataStructureAlign->addAction(alignDataStructureBottom);
    menuDataStructureAlign->addAction(alignDataStructureCenter);
    menuDataStructureAlign->addAction(alignDataStructureTop);
    menuDataStructureAlign->addAction(alignDataStructureLeft);
    menuDataStructureAlign->addAction(alignDataStructureRight);
    menuDataStructureAlign->addAction(alignDataStructureCircle);
    menuDataStructureAlign->addAction(alignDataStructureTree);

    QMenu *menuSelectedAlign = new QMenu(i18n("Align"));
    menuSelectedAlign->addAction(new AlignAction(i18nc("@action:inmenu align elements at bottom", "Bottom"),  AlignAction::Bottom, this));
    menuSelectedAlign->addAction(new AlignAction(i18nc("@action:inmenu align elements at center", "Center"), AlignAction::HCenter, this));
    menuSelectedAlign->addAction(new AlignAction(i18nc("@action:inmenu align elements at top", "Top"),   AlignAction::Top, this));
    menuSelectedAlign->addAction(new AlignAction(i18nc("@action:inmenu align elements left", "Left"),  AlignAction::Left, this));
    menuSelectedAlign->addAction(new AlignAction(i18nc("@action:inmenu align elements right", "Right"), AlignAction::Right, this));
    menuSelectedAlign->addAction(new AlignAction(i18nc("@action:inmenu align elements as a circle", "Circle"),  AlignAction::Circle, this));
    menuSelectedAlign->addAction(new AlignAction(i18nc("@action:inmenu align elements such that crossing edges are minimized", "Minimize Crossing Edges"),  AlignAction::MinCutTree, this));

    QMenu *menuDataStructureAssignValues = new QMenu(i18nc("@title:menu Assign values", "Values"));
    menuDataStructureAssignValues->addAction(new AssignValueAction(i18nc("@action:inmenu Value", "Enumerate"), this, AssignValueAction::Enumerate, contextDataStructure, 0));
    menuDataStructureAssignValues->addAction(new AssignValueAction(i18nc("@action:inmenu Value", "Random Integers"), this, AssignValueAction::RandomInteger, contextDataStructure, 0));
    menuDataStructureAssignValues->addAction(new AssignValueAction(i18nc("@action:inmenu Value", "Random Reals"), this, AssignValueAction::RandomReal, contextDataStructure, 0));

    QMenu *menuSelectedAssignValues = new QMenu(i18nc("@title:menu Assign values","Values"));
    menuSelectedAssignValues->addAction(new AssignValueAction(i18nc("@action:inmenu Value", "Enumerate"), this, AssignValueAction::Enumerate, 0));
    menuSelectedAssignValues->addAction(new AssignValueAction(i18nc("@action:inmenu Value", "Random Integers"), this, AssignValueAction::RandomInteger, 0));
    menuSelectedAssignValues->addAction(new AssignValueAction(i18nc("@action:inmenu Value", "Random Reals"), this, AssignValueAction::RandomReal, 0));

    // puzzling the menu together
    AddDataAction *addDataAction = new AddDataAction(this);
    addDataAction->setAddPosition(scenePosition);
    addDataAction->setCheckable(false); // it doesn't make sense to have a checkbox in the right click menu
    connect(addDataAction, SIGNAL(triggered(bool)), addDataAction, SLOT(executePress()));
    DeleteAction *deleteDataStructureAction = new DeleteAction(i18nc("@action:inmenu", "Delete"), this, contextDataStructure, 0);
    DeleteAction *deleteSelectedAction = new DeleteAction(i18nc("@action:inmenu", "Delete"), this, 0);
    DeleteAction *deleteItemAction = new DeleteAction(i18nc("@action:inmenu", "Delete"), this, contextData, 0);

    menuSelected->addMenu(menuSelectedAlign);
    menuSelected->addMenu(menuSelectedAssignValues);
    menuSelected->addAction(deleteSelectedAction);
    menuDataStructure->addMenu(menuDataStructureAlign);
    menuDataStructure->addMenu(menuDataStructureAssignValues);
    menuDataStructure->addAction(deleteDataStructureAction);

    // activate/deactivate context depending on where the user click
    if (selectedItems().count() == 0) {
        menuSelected->setDisabled(true);
    }
    if (!contextAtItem) {
        menuDataStructure->setDisabled(true);
        menu->addAction(addDataAction);
        menu->addMenu(menuZoom);
    }
    if (contextData) {
        PropertiesDialogAction *dataPropertiesAction = new PropertiesDialogAction(i18nc("@action:inmenu", "Properties"), contextData, this);
        dataPropertiesAction->setPosition(screenPosition);
        menu->addAction(dataPropertiesAction);
    }
    if (contextPointer) {
        PropertiesDialogAction *pointerPropertiesAction = new PropertiesDialogAction(i18nc("@action:inmenu", "Properties"), contextPointer, this);
        pointerPropertiesAction->setPosition(screenPosition);
        menu->addAction(pointerPropertiesAction);
    }
    if (contextAtItem) {
        menu->addAction(deleteItemAction);
    }

    return menu;
}
