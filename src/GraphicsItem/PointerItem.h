/* 
    This file is part of Rocs.
    Copyright 2008-2011 Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008      Ugo Sangiori <ugorox@gmail.com>

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

#ifndef ROCS_POINTERITEM_H
#define ROCS_POINTERITEM_H

#include <QGraphicsLineItem>
#include "rocslib_export.h"

class QGraphicsSceneMouseEvent;
class Pointer;
class QGraphicsSimpleTextItem;

/*! \brief the Edge drawing on screen.
  long explanation here...
*/
class ROCSLIB_EXPORT PointerItem : public QObject, public QGraphicsPathItem {
    Q_OBJECT
public:
    /*! default constructor
    \param node the libgraph::Node that this item will interact to.
    \param parent the QGraphicsITem that this Item belongs to. */
    explicit PointerItem(Pointer *pointer, QGraphicsItem *parent = 0);
    virtual ~PointerItem();
    /*! The type of the item */
    enum { Type = UserType + 3 };

    /*! gets the type of the item
      \return the type of the item
    */
    int type() const {
        return Type;
    }

    /*! Gets the pointer of the node */
    Pointer* pointer() const{ return _pointer; }
    int index() const { return _index; }
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
    virtual QPainterPath createCurves() = 0;
    
public slots:
    void remove();
    void updatePos();
    void updateAttributes();
    
private:
    Pointer *_pointer;
    int _index;

  
    QPolygonF createArrow(const QPointF& Pos1, const QPointF& Pos2) const;
    QPainterPath createLoop(QPointF pos) const;
    void connectSignals();

    QGraphicsSimpleTextItem *_name;
    QGraphicsSimpleTextItem *_value;
};

#endif
