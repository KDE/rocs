/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef ORIENTEDEDGEITEM_H
#define ORIENTEDEDGEITEM_H

#include <QGraphicsLineItem>

class QGraphicsSceneMouseEvent;
class Edge;
class Node;
class QGraphicsSimpleTextItem;

/*! \brief the Edge drawing on screen.
  long explanation here...
*/
class OrientedEdgeItem : public QObject, public QGraphicsPathItem {
    Q_OBJECT
public:
    /*! default constructor
    \param node the libgraph::Node that this item will interact to.
    \param parent the QGraphicsITem that this Item belongs to. */
    explicit OrientedEdgeItem(Edge *edge, QGraphicsItem *parent = 0);
    virtual ~OrientedEdgeItem();
    /*! The type of the item */
    enum { Type = UserType + 3 };

    /*! gets the type of the item
      \return the type of the item
    */
    int type() const {
        return Type;
    }

    /*! Gets the edge of the node */
    Edge* edge() {
        return _edge;
    }

protected:
    /*! when there's a mouse click on the node, this method is invocked
      \param event the mouse object
    */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    /*! when the mouse is released, this method is invocked
      \param event the mouse object
    */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    
public slots:
    void remove();
    void updatePos();
    void updateAttributes();
private:
    Edge *_edge;
    int _index;
    bool _loop;
    QPainterPath createCurves() const;
    QPolygonF createArrow(const QPointF& Pos1, const QPointF& Pos2) const;
    QPainterPath createLoop(QPointF pos) const;
    void connectSignals();

    QGraphicsSimpleTextItem *_name;
    QGraphicsSimpleTextItem *_value;
};

#endif
