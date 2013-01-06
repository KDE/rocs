/*
    This file is part of Rocs,
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2013       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef SUI_GRAPHSCENE_H
#define SUI_GRAPHSCENE_H

#include "VisualEditorExport.h"
#include <QGraphicsScene>
#include <QMultiHash>
#include "CoreTypes.h"

class AbstractAction;
class Document;
class QAction;
class QGraphicsItem;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class QGraphicsSceneContextMenuEvent;
class QKeyEvent;
class QMenu;

class VISUALEDITOR_EXPORT GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphScene(QObject *parent);
    void setAction(QAction *action);
    void updateGraph(DataStructurePtr g);
    void updateDocument();
    void setActiveGraph(DataStructurePtr g);
    void clearGraph();
    qreal zoomFactor() const;
    void zoomBy(qreal scaleFactor);
    void zoomTo(qreal scaleFactor);
    void zoomToRect(const QRectF &rect);
    void resetZoom();
    void centerOn(const QPointF &pos);

public slots:
    QGraphicsItem * createData(DataPtr n);
    QGraphicsItem * createEdge(PointerPtr e);
    void connectGraphSignals(DataStructurePtr g);

    /**
     * Create graphic items for all elements of \p dataStructure.
     */
    void createItems(DataStructurePtr dataStructure);

    /**
     * Create graphic items for all elements on this document
     */
    void createItems();
    void setActiveDocument();
    void resize();

signals:
    void resized();
    void keyPressed(QKeyEvent *key);
    void keyReleased(QKeyEvent *key);
    void addData(const QPointF &pos);
    void removeSelected();
    void zoomFactorChanged(qreal zoomFactor);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
    void wheelEvent(QGraphicsSceneWheelEvent* wheelEvent);
    void keyPressEvent(QKeyEvent *keyEvent);
    void keyReleaseEvent(QKeyEvent *keyEvent);

private:
    QMenu * createContextMenu(const QPointF &scenePosition, const QPointF &screenPosition);
    QMenu *_contextMenu;
    Document *_graphDocument;
    DataStructurePtr _graph;
    AbstractAction *_action;
    QMultiHash<DataStructure*, QGraphicsItem* > _hashGraphs;
    QGraphicsRectItem *_whiteboard;
    void releaseDocument();
    qreal _zoomFactor;
};

#endif
