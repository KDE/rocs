/*
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
    Copyright 2011       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "SelectMoveHandAction.h"

#include "Scene/GraphScene.h"
#include "Scene/DataItem.h"
#include "DataStructure.h"
#include "DocumentManager.h"
#include "Data.h"
#include "Document.h"

#include <QGraphicsView>
#include <QKeyEvent>

#include <KLocale>
#include <KDebug>

SelectMoveHandAction::SelectMoveHandAction(GraphScene *scene, QObject *parent)
    : AbstractAction(scene, parent),
    _currentItem(0),
    _selectionMode(false)
{
    setText(i18nc("@action:intoolbar", "Move"));
    setToolTip(i18nc("@info:tooltip", "Select and move items."));
    setIcon(KIcon("rocsselectmove"));
    _name = "rocs-hand-select-move";

    connect(_graphScene, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(executeKeyPress(QKeyEvent*)));
    connect(_graphScene, SIGNAL(keyReleased(QKeyEvent*)), this, SLOT(executeKeyRelease(QKeyEvent*)));
}

SelectMoveHandAction::~SelectMoveHandAction()
{
}

bool SelectMoveHandAction::executePress(QPointF pos)
{
    if (!DocumentManager::self().activeDocument()->activeDataStructure()) {
        return false;
    }

    // check if there is item at click-position,
    // if no item is selected, then enable rectangle selection mode and return
    if (!(_currentItem = qgraphicsitem_cast<DataItem*>(_graphScene->itemAt(pos)))) {
        _graphScene->views().at(0)->setInteractive(true);
        _graphScene->views().at(0)->setDragMode(QGraphicsView::RubberBandDrag);
        return true;
    }

    // if item is not selected, deselect everything else and select it
    if (!_selectionMode && _currentItem->isSelected() == false) {
        foreach(QGraphicsItem * item, _graphScene->selectedItems()) {
            item->setSelected(false);
        }
    }
    _currentItem->setSelected(true);

    // switch to move mode for items that are currently selected
    _deltas.clear();
    foreach(QGraphicsItem * item, _graphScene->selectedItems()) {
        if (DataItem *dItem = qgraphicsitem_cast<DataItem*>(item)) {
            QPointF delta = pos - QPointF(dItem->data()->x(), dItem->data()->y());
            _deltas.insert(dItem, delta);
        }
    }

    if (_graphScene->items().count() > 600) { // 5! + 25 items on screen
        _graphScene->views().at(0)->setRenderHints(QPainter::Antialiasing
                & QPainter::TextAntialiasing);
    }
    return true;
}

bool SelectMoveHandAction::executeMove(QPointF pos)
{
    // only move if we are in move mode: i.e., a data item is selected
    if (!_currentItem || !_graphScene->selectedItems().contains(_currentItem)) {
        return false;
    }

    if (!DocumentManager::self().activeDocument()->isPointAtDocument(pos)) {
        Document *doc = DocumentManager::self().activeDocument();
        QRectF sceneRect = doc->sceneRect();

        if (pos.x() < sceneRect.left()) {
            pos.setX(sceneRect.left());
        }
        if (pos.x() > sceneRect.right()) {
            pos.setX(sceneRect.right());
        }
        if (pos.y() < sceneRect.top()) {
            pos.setY(sceneRect.top());
        }
        if (pos.y() > sceneRect.bottom()) {
            pos.setY(sceneRect.bottom());
        }
    }

    foreach(QGraphicsItem * item, _graphScene->selectedItems()) {
        if (DataItem *dItem = qgraphicsitem_cast<DataItem*>(item)) {
            dItem->data() -> setPos(pos.x() - _deltas.value(dItem).x(),
                                    pos.y() - _deltas.value(dItem).y());
        }
    }

    return true;
}

bool SelectMoveHandAction::executeRelease(QPointF pos)
{
    _graphScene->views().at(0)->setDragMode(QGraphicsView::NoDrag);

    if (!_currentItem) {
        return false;
    }

    foreach(QGraphicsItem * item, _graphScene->selectedItems()) {
        if (DataItem *dItem = qgraphicsitem_cast<DataItem*>(item)) {
            dItem->data() -> setPos(pos.x() - _deltas.value(dItem).x(),
                                    pos.y() - _deltas.value(dItem).y());
        }
    }

    _currentItem = 0;
    if (_graphScene->items().count() > 600) { // 5! + 25 itens on screen
        _graphScene->views()[0]->setRenderHint(QPainter::Antialiasing);
    }
    return true;
}

bool SelectMoveHandAction::executeKeyPress(QKeyEvent* keyEvent)
{
    Q_ASSERT(keyEvent->type() == QEvent::KeyPress);

    // consider key sequences
    if (keyEvent->matches(QKeySequence::SelectAll)) {
        foreach(QGraphicsItem * item, _graphScene->items()) {
            if (DataItem *dItem = qgraphicsitem_cast<DataItem*>(item)) {
                dItem->setSelected(true);
            }
        }
        return true;
    }

    // consider single key press events
    QPointF move;
    switch (keyEvent->key()) {
    case Qt::Key_Escape: {
        _graphScene->setAction(this);
        break;
    }
    case Qt::Key_Up: {
        moveSelectedItems(QPointF(0, -10));
        break;
    }
    case Qt::Key_Down: {
        moveSelectedItems(QPointF(0, 10));
        break;
    }
    case Qt::Key_Left: {
        moveSelectedItems(QPointF(-10, 0));
        break;
    }
    case Qt::Key_Right: {
        moveSelectedItems(QPointF(10, 0));
        break;
    }
    case Qt::Key_Control: {
        _selectionMode = true;
        break;
    }
    default:
        return false;
    }
    return true;
}

void SelectMoveHandAction::moveSelectedItems(QPointF movement)
{
    foreach(QGraphicsItem * item, _graphScene->selectedItems()) {
        if (DataItem *dItem = qgraphicsitem_cast<DataItem*>(item)) {
            dItem->data()-> setPos(dItem->data()->x() + movement.x(),
                                   dItem->data()->y() + movement.y());
        }
    }
}

bool SelectMoveHandAction::executeKeyRelease(QKeyEvent* keyEvent)
{
    Q_ASSERT(keyEvent->type() == QEvent::KeyRelease);

    if (keyEvent->key() == Qt::Key_Control) {
        _selectionMode = false;
        return true;
    }
    return false;
}
