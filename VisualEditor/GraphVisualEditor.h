/*
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef SUI_GRAPHVISUALEDITOR_H
#define SUI_GRAPHVISUALEDITOR_H

#include "VisualEditorExport.h"
#include <CoreTypes.h>
#include <QWidget>
#include <QSlider>
#include <QString>
#include <kaction.h>
#include <boost/scoped_ptr.hpp>

class GraphVisualEditorPrivate;
class EditorToolbar;
class KActionCollection;
class Document;
class DataStructure;
class DataItem;
class Graph;
class Data;
class GraphScene;
class QToolButton;
class MainWindow;
class QGraphicsView;
class KComboBox;
class KPushButton;

/**
 * \class GraphVisualEditor
 * \brief Singelton class that provides a whiteboard for interactions with data structures.
 */
class VISUALEDITOR_EXPORT GraphVisualEditor : public QWidget
{
    Q_OBJECT
public:
    static GraphVisualEditor* self();

    /**
     * re-implementation of resize event to also setup the graph scene
     */
    void resizeEvent(QResizeEvent *event);

    /**
     * Setup actions of visual graph editor and adds them to contributed action collection
     * \p collection.
     *
     * \param collection the action collection at which actions are registered
     */
    void setupActions(KActionCollection* collection);

public slots:
    /**
     * Reflect currently active data structure in graph scene and graph editor toolbar.
     *
     *\param g is the active data structure
     */
    void updateActiveDataStructure(DataStructurePtr g);

    /*! as the name says, it removes the current DataStructureDocument
    from the screen and releases it from the evil hands of GraphEditWidget.*/
    void releaseDocument();

    void setActiveDocument();
    void addDataStructure();
    void removeDataStructure();
    QList<DataItem*> selectedNodes() const;
    void updateZoomSlider(qreal zoomFactor);
    void zoomTo(int sliderValue);

    /**
     * Updates selector combobox for graph documents.
     */
    void updateGraphDocumentList();
    void updateDataStructureList();

private:
    /**
     * Default constructor.
     *
     *\param parent the owner of this widget.
     */
    GraphVisualEditor(QWidget* parent);

    /**
     * Default destructor.
     */
    virtual ~GraphVisualEditor();

    /**
     * Creates toolbar for the graph scene.
     *
     * \return created toolbar
     */
    QWidget* sceneToolbar();

    void setupWidgets();

    static GraphVisualEditor* _self;
    boost::scoped_ptr<GraphVisualEditorPrivate> d;
};

#endif
