/* This file is part of Rocs,
	 Copyright (C) 2008 by:
	 Tomaz Canabrava <tomaz.canabrava@gmail.com>

	 Rocs is free software; you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation; either version 2 of the License, or
	 (at your option) any later version.

	 Rocs is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with Step; if not, write to the Free Software
	 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef SUI_GRAPHVISUALEDITOR_H
#define SUI_GRAPHVISUALEDITOR_H

#include <QWidget>
#include <QString>
#include "AlignAction.h"

class Document;
class DataStructure;
class Graph;
class Data;
class GraphScene;
class QToolButton;
class MainWindow;
class QGraphicsView;

class GraphVisualEditor : public QWidget {
    Q_OBJECT
public:
    static GraphVisualEditor* self();

    /**
     * re-implementation of resize event to also setup the graph scene
     */
    void resizeEvent (QResizeEvent  *event );

    GraphScene *scene() const;
    QGraphicsView *view() const;
    MainWindow *mainWindow();

//     void resizeVisualEditor(qreal yTop, qreal xLeft, qreal yBottom, qreal xRight);

    /*! as the name says, it removes the current DataStructureDocument
    from the screen and releases it from the evil hands of GraphEditWidget.*/
    void releaseDocument();
    
    int viewStyleDataNode() const;
    int viewStyleDataEdge() const;

public slots:    
    /*! set the current selected Graph.
    	\param graph the new active graph.
    	*/
    void setActiveGraph( DataStructure *g);

    void setActiveDocument();
    QList<DataItem*> selectedNodes() const;

    void setViewStyleDataNode(int style);
    void setViewStyleDataEdge(int style);

private:
    /*! Default Constructor
    \param parent the owner of this widget. */
    GraphVisualEditor(MainWindow *parent = 0);

    void setupWidgets();

    /*! as the name says, draw a graph on scene.
    \param g the graph to be drawn. */
    void drawGraphOnScene( DataStructure *g );

    QToolButton *setupToolButton(const QString& actionName, const QString& tooltip, AlignAction::Orientation o, QWidget *parent);

    static GraphVisualEditor* _self;

    int _viewStyleDataNode;
    int _viewStyleDataEdge;

    GraphScene *_scene;
    Document *_document;
    DataStructure *_dataStructure;

    MainWindow *_mainWindow;

    QGraphicsView *_graphicsView;
};

#endif
