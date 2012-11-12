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

#ifndef EDITORTOOLBAR_H
#define EDITORTOOLBAR_H

#include <QWidget>

class KActionCollection;
class KActionMenu;
class SelectMoveHandAction;
class GraphScene;
class Document;

/**
 * \class EditorToolbar
 *
 * This class provides hand actions to be performed at the graphics scene. These include adding/
 * deleting of elements, moving and selecting. To create the actions, execute \see setup(...).
 * Whenever the active document at the graph scene is changed, \see setActiveDocument(...) must be
 * called.
 */
class EditorToolbar : public QWidget
{
    Q_OBJECT

public:
    /**
     * Default constructor.
     */
    explicit EditorToolbar(QWidget *parent);

    /**
     * Setup toolbar actions.
     *
     * \param scene the graphics scene where actions shall be performed
     * \param collection the action collection to that the actions shall be added
     */
    void setup(GraphScene *scene, KActionCollection *collection);

    /**
     * Set current active graph document. By changing the active document also all connections
     * are updated accordingly.
     *
     * \param activeDocument the new active document
     */
    void setActiveDocument(Document *activeDocument);

public slots:
    /**
     * Update action menus for data types and pointer types. Update must be performed whenever
     * types are added/removed.
     */
    void updateTypeActions();

private:
    Document* _document;
    GraphScene* _scene;
    KActionMenu *_addDataActionMenu;
    KActionMenu *_addPointerActionMenu;
    SelectMoveHandAction *_selectMoveAction;
};

#endif
