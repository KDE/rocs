/***************************************************************************
 * main.cpp
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNGraphScene::ESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/

#ifndef SUI_GRAPHSCENE_H
#define SUI_GRAPHSCENE_H

#include <QGraphicsScene>
#include <QMultiHash>

class AbstractAction;
class QAction;
class Datum;
class Pointer;
class QGraphicsItem;
class DataTypeDocument;
class DataType;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class QKeyEvent;
class DatumPropertiesWidget;
class PointerPropertiesWidget;

class GraphScene : public QGraphicsScene {
    Q_OBJECT
public:
    GraphScene(QObject *parent);
    void setAction(QAction *action);
    void updateGraph(DataType *g);
    void updateDocument();
    void setActiveGraph(DataType *g);
    void setActiveDataTypeDocument(DataTypeDocument *gd);
    void clearGraph();

    void setHideEdges(bool h);
    bool hideEdges();
    void updateAfter(QGraphicsItem *item);
    bool fade() const{ return _fade; }
    void fade(bool b){ _fade = b; }
    void hideGraph( DataType *g, bool visibility);

public slots:
    QGraphicsItem* createDatum( Datum* n);
    QGraphicsItem* createEdge( Pointer *e);
    void connectGraphSignals(DataType *g);
    void createItems();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseDoubleClickEvent (QGraphicsSceneMouseEvent * mouseEvent);
    void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent);
    void keyPressEvent(QKeyEvent *keyEvent);

private:
    DataTypeDocument *_graphDocument;
    DataType *_graph;
    AbstractAction *_action;
    QMultiHash<DataType*, QGraphicsItem* > _hashGraphs;
    QList<QGraphicsItem*> _hidedEdges;
    bool _hideEdges;
    DatumPropertiesWidget *_datumPropertiesWidget;
    PointerPropertiesWidget *_pointerPropertiesWidget;
    bool _fade;
    void releaseDocument();
};

#endif
