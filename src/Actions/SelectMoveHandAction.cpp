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
#include "GraphScene.h"
#include "DataStructure.h"
#include "DocumentManager.h"
#include "Data.h"
#include "Document.h"
#include "DataItem.h"
#include <KLocale>
#include <QGraphicsView>
#include <QKeyEvent>

#include <KDebug>
//#include "settings.h"

SelectMoveHandAction::SelectMoveHandAction(GraphScene *scene, QObject *parent)
    : AbstractAction(scene, parent)
{
    setText(i18n("Move"));
    setToolTip(i18n("Select and move items."));
    setIcon(KIcon("rocsselectmove"));
    _movableNode = 0;
    _name = "rocs-hand-select-move";

    connect(_graphScene, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(executeArrowKeyMove(QKeyEvent*)));
}

SelectMoveHandAction::~SelectMoveHandAction()
{
    kDebug() << "Destroyed";
}

bool SelectMoveHandAction::executePress(QPointF pos)
{
    if (!DocumentManager::self()->activeDocument()->activeDataStructure()) {
        return false;
    }

    // if now data item at clicked point: switch to select mode
    if (!(_movableNode = qgraphicsitem_cast<DataItem*>(_graphScene->itemAt(pos)))) {
        _graphScene->views().at(0)->setInteractive(true);
        _graphScene->views().at(0)->setDragMode(QGraphicsView::RubberBandDrag);
        return true;
    } else {
        _movableNode->setSelected(true);
    }

    // otherwise we have a node to move
    _deltas.clear();
    if (! _graphScene->selectedItems().contains(_movableNode)) {
        _data = _movableNode->data();
        _delta = pos - QPointF(_data->x(), _data->y());
    } else foreach(QGraphicsItem * item, _graphScene->selectedItems()) {

        if (DataItem *dItem = qgraphicsitem_cast<DataItem*>(item)) {
            QPointF delta = pos - QPointF(dItem->data()->x(), dItem->data()->y());
            _deltas.insert(dItem, delta);
        }
    }

    if (_graphScene->items().count() > 600) { // 5! + 25 itens on screen
        _graphScene->views().at(0)->setRenderHints(QPainter::Antialiasing
                & QPainter::TextAntialiasing);
    }
    return true;
}

bool SelectMoveHandAction::executeMove(QPointF pos)
{
    if (! _movableNode) {
        return false;
    }

    if (!DocumentManager::self()->activeDocument()->isPointAtDocument(pos)) {
        Document *d = DocumentManager::self()->activeDocument();
        if (pos.x() < d->left()) {
            pos.setX(d->left());
        }
        if (pos.x() > d->right()) {
            pos.setX(d->right());
        }
        if (pos.y() < d->top()) {
            pos.setY(d->top());
        }
        if (pos.y() > d->bottom()) {
            pos.setY(d->bottom());
        }
    }

    if (! _graphScene->selectedItems().contains(_movableNode)) {
        _data -> setPos(pos.x() - _delta.x(), pos.y() - _delta.y());
    } else foreach(QGraphicsItem * item, _graphScene->selectedItems()) {
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

    if (!_movableNode) {
        return false;
    }

    if (! _graphScene->selectedItems().contains(_movableNode)) {
        _data -> setPos(pos.x() - _delta.x(), pos.y() - _delta.y());
    } else foreach(QGraphicsItem * item, _graphScene->selectedItems()) {
        if (DataItem *dItem = qgraphicsitem_cast<DataItem*>(item)) {
            dItem->data() -> setPos(pos.x() - _deltas.value(dItem).x(),
                                    pos.y() - _deltas.value(dItem).y());
        }
    }

    _movableNode = 0;
    if (_graphScene->items().count() > 600) { // 5! + 25 itens on screen
        _graphScene->views()[0]->setRenderHint(QPainter::Antialiasing);
    }
    return true;
}

bool SelectMoveHandAction::executeArrowKeyMove(QKeyEvent* keyEvent)
{

    // consider key sequences
    if (keyEvent->matches(QKeySequence::SelectAll)) {
        foreach(QGraphicsItem * item, _graphScene->items()) {
            if (DataItem *dItem = qgraphicsitem_cast<DataItem*>(item)) {
                dItem->setSelected(true);
            }
        }
        return true;
    }

    // consider single keys
    QPointF move;
    switch (keyEvent->key()) {
    case Qt::Key_Escape: {
        _graphScene->setAction(this);
        break;
    }
    case Qt::Key_Up: {
        move = QPointF(0, -10);
        break;
    }
    case Qt::Key_Down: {
        move = QPointF(0, 10);
        break;
    }
    case Qt::Key_Left: {
        move = QPointF(-10, 0);
        break;
    }
    case Qt::Key_Right: {
        move = QPointF(10, 0);
        break;
    }
    default:
        return false;
    }

    // move all selcted items
    foreach(QGraphicsItem * item, _graphScene->selectedItems()) {
        if (DataItem *dItem = qgraphicsitem_cast<DataItem*>(item)) {
            dItem->data()-> setPos(dItem->data()->x() + move.x(),
                                   dItem->data()->y() + move.y());
        }
    }
    return true;
}
