/* 
    This file is part of Rocs,
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>

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

#include <QGraphicsScene>
#include <QMultiHash>
#include <boost/shared_ptr.hpp>
#include "Rocs_Typedefs.h"

class AbstractAction;
class QAction;
class Data;
class Pointer;
class QGraphicsItem;
class Document;
class DataStructure;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class QGraphicsSceneContextMenuEvent;
class QKeyEvent;
class DataPropertiesWidget;
class PointerPropertiesWidget;

class GraphScene : public QGraphicsScene {
    Q_OBJECT
public:
    GraphScene( QObject *parent=0 );
    enum borderSize{ kBORDER = 50 };
    void updateMinSize(qreal minWidth, qreal minHeight);
    void setAction(QAction *action);
    void updateGraph(DataStructurePtr g);
    void updateDocument();
    void setActiveGraph(DataStructurePtr g);
    void clearGraph();

    void setHideEdges(bool h);
    bool hideEdges();
    void updateAfter(QGraphicsItem *item);
    bool fade() const{ return _fade; }
    void fade(bool b){ _fade = b; }
    void hideGraph(DataStructurePtr g, bool visibility);

public slots:
    QGraphicsItem* createData( DataPtr n);
    QGraphicsItem* createEdge( PointerPtr e);
    void connectGraphSignals(DataStructurePtr g);
    void createItems();
    void setActiveDocument();
    void resize();

signals:
    void resized();
    void keyPressed(QKeyEvent* key);
    void addData(QPointF pos);
    void removeSelected();
    void zoom(qreal amount);
    

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseDoubleClickEvent (QGraphicsSceneMouseEvent * mouseEvent);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
    void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent);
    void keyPressEvent(QKeyEvent *keyEvent);

private:
    Document *_graphDocument;
    DataStructurePtr _graph;
    AbstractAction *_action;
    QMultiHash<DataStructure*, QGraphicsItem* > _hashGraphs;
    QList<QGraphicsItem*> _hidedEdges;
    QGraphicsRectItem *_whiteboard;
    bool _hideEdges;
    DataPropertiesWidget *_datumPropertiesWidget;
    PointerPropertiesWidget *_pointerPropertiesWidget;
    bool _fade;
    void releaseDocument();
    qreal _minWidth;
    qreal _minHeight;
};

#endif
