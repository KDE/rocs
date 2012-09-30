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

class VISUALEDITOR_EXPORT GraphVisualEditor : public QWidget
{
    Q_OBJECT
public:
    static GraphVisualEditor* self();

    /**
     * re-implementation of resize event to also setup the graph scene
     */
    void resizeEvent(QResizeEvent  *event);

    GraphScene *scene() const;
    QGraphicsView *view() const;
    MainWindow *mainWindow();

//     void resizeVisualEditor(qreal yTop, qreal xLeft, qreal yBottom, qreal xRight);

    /*! as the name says, it removes the current DataStructureDocument
    from the screen and releases it from the evil hands of GraphEditWidget.*/
    void releaseDocument();

public slots:
    /**
     * Reflect currently active data structure in graph scene and graph editor toolbar.
     *
     *\param g is the active data structure
     */
    void updateActiveDataStructure(DataStructurePtr g);

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
    /*! Default Constructor
    \param parent the owner of this widget. */
    GraphVisualEditor(QWidget* parent);

    void setupWidgets();

    /**
     * Creates toolbar for the graph scene.
     *
     * \return created toolbar
     */
    QWidget* sceneToolbar();

    static GraphVisualEditor* _self;

    GraphScene *_scene;

    KComboBox *_documentSelectorCombo;
    QToolButton *_documentPropertiesButton;
    KComboBox *_dataStructureSelectorCombo;
    QToolButton *_dataStructurePropertiesButton;

    QSlider *_zoomSlider;
    Document *_document;
    DataStructurePtr _dataStructure;

    QGraphicsView *_graphicsView;
};

#endif
