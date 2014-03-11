/*
    This file is part of Rocs.
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "EditorToolbar.h"

#include "Scene/GraphScene.h"

#include "Actions/AddConnectionHandAction.h"
#include "Actions/AddDataHandAction.h"
#include "Actions/DeleteHandAction.h"
#include "Actions/ZoomAction.h"
#include "Actions/SelectMoveHandAction.h"

#include "Document.h"
#include "DataType.h"
#include "PointerType.h"
#include "CoreTypes.h"

#include <QIcon>
#include <KLocalizedString>
#include <KActionCollection>
#include <KActionMenu>
#include <KMenu>
#include <QDebug>

EditorToolbar::EditorToolbar(QWidget* parent)
    : QWidget(parent)
{
}

void EditorToolbar::setup(GraphScene* scene, KActionCollection* collection)
{
    _scene = scene;

    _selectMoveAction = new SelectMoveHandAction(scene, this);
    DeleteHandAction* deleteAction = new DeleteHandAction(scene, this);
    ZoomAction* zoomAction = new ZoomAction(scene, this);

    connect(_selectMoveAction, SIGNAL(triggered()), _selectMoveAction, SLOT(sendExecuteBit()));
    connect(deleteAction, SIGNAL(triggered()), deleteAction, SLOT(sendExecuteBit()));
    connect(zoomAction, SIGNAL(triggered()), zoomAction, SLOT(sendExecuteBit()));

    _addDataActionMenu = new KActionMenu(QIcon::fromTheme("rocsadddata"), i18nc("@title:menu", "Add Data"), this);
    _addDataActionMenu->setIconText(i18nc("@action:intoolbar", "Add Data"));
    _addDataActionMenu->setToolTip(i18nc("@info:tooltip", "Add new data element"));
    _addDataActionMenu->setDelayed(true);
    _addDataActionMenu->setCheckable(true);

    _addPointerActionMenu = new KActionMenu(QIcon::fromTheme("rocsaddedge"), i18nc("@title:menu", "Add Connection"), this);
    _addPointerActionMenu->setIconText(i18nc("@action:intoolbar", "Add Connection"));
    _addPointerActionMenu->setToolTip(i18nc("@info:tooltip", "Add a new connection between two data elements of selected type"));
    _addPointerActionMenu->setDelayed(true);
    _addPointerActionMenu->setCheckable(true);

    // add actions to collection and group
    QActionGroup *g = new QActionGroup(this);
    g->addAction(collection->addAction("selectmove", _selectMoveAction));
    g->addAction(collection->addAction("add_node", _addDataActionMenu));
    g->addAction(collection->addAction("add_edge", _addPointerActionMenu));
    g->addAction(collection->addAction("delete", deleteAction));
    g->addAction(collection->addAction("zoom", zoomAction));
    collection->action("selectmove")->toggle();
    _scene->setAction(_selectMoveAction);
}

void EditorToolbar::setActiveDocument(Document* activeDocument)
{
    if (_document != 0) {
        disconnect(_document);
    }
    _document = activeDocument;
    updateTypeActions();

    // connect to new document
    connect(_document, SIGNAL(dataTypeCreated(int)), this, SLOT(updateTypeActions()));
    connect(_document, SIGNAL(pointerTypeCreated(int)), this, SLOT(updateTypeActions()));
    connect(_document, SIGNAL(dataTypeRemoved(int)), this, SLOT(updateTypeActions()));
    connect(_document, SIGNAL(pointerTypeRemoved(int)), this, SLOT(updateTypeActions()));
}

void EditorToolbar::updateTypeActions()
{
    _addDataActionMenu->menu()->clear();
    foreach (int identifier, _document->dataTypeList()) {
        DataTypePtr type = _document->dataType(identifier);
        AddDataHandAction* addDataAction = new AddDataHandAction(_scene, type, _addDataActionMenu->menu());
        addDataAction->setCheckable(false);

        if (identifier == 0) { // set default action to menu
            connect(_addDataActionMenu, SIGNAL(triggered()), addDataAction, SLOT(sendExecuteBit()));
        }

        _addDataActionMenu->menu()->addAction(addDataAction);
        connect(type.get(), SIGNAL(iconChanged(QString)), addDataAction, SLOT(updateIcon()));
        connect(addDataAction, SIGNAL(triggered()), addDataAction, SLOT(sendExecuteBit()));
    }

    _addPointerActionMenu->menu()->clear();
    foreach (int identifier, _document->pointerTypeList()) {
        PointerTypePtr type = _document->pointerType(identifier);
        AddConnectionHandAction* addPointerAction = new AddConnectionHandAction(_scene, type, _addPointerActionMenu->menu());
        addPointerAction->setCheckable(false);

        if (identifier == 0) { // set default action to menu
            connect(_addPointerActionMenu, SIGNAL(triggered()), addPointerAction, SLOT(sendExecuteBit()));
        }

        _addPointerActionMenu->menu()->addAction(addPointerAction);
        connect(addPointerAction, SIGNAL(triggered()), addPointerAction, SLOT(sendExecuteBit()));
    }
}
