/*
    This file is part of Rocs.
    Copyright 2008-2011 Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008      Ugo Sangiori <ugorox@gmail.com>
    Copyright 2012      Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include <QFont>
#include <boost/shared_ptr.hpp>
#include "VisualEditorExport.h"
#include "CoreTypes.h"

class QGraphicsSceneMouseEvent;
class Pointer;
class QGraphicsSimpleTextItem;

/*! \brief the Edge drawing on screen.
  long explanation here...
*/
class VISUALEDITOR_EXPORT PointerItem : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    /*! default constructor
    \param node the libgraph::Node that this item will interact to.
    \param parent the QGraphicsITem that this Item belongs to. */
    explicit PointerItem(PointerPtr pointer, QGraphicsItem *parent = 0);
    virtual ~PointerItem();
    /*! The type of the item */
    enum { Type = UserType + 3 };

    /*! gets the type of the item
      \return the type of the item
    */
    int type() const {
        return Type;
    }

    /**
     * \return the graphics item to be displayed at the scene
     */
    QGraphicsItem *propertyListItem() const;

    /*! Gets the pointer of the node */
    PointerPtr pointer() const {
        return _pointer;
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

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    virtual QPainterPath createCurves() = 0;

public slots:
    void remove();
    void updatePos();
    void updateAttributes();
    void updateProperty(const QString& name);
    void registerProperty(const QString& name);
    void removeProperty(const QString& name);

private:
    /**
     * Update positions, values and visibility of properties.
     *
     * \param x is the x position for starting the list
     * \param y is the x position for starting the list
     */
    void updatePropertyList(qreal x, qreal y);

    PointerPtr _pointer;
    int _index;
    QFont _font;
    QGraphicsItemGroup* _item;
    QMap<QString, QGraphicsSimpleTextItem*> _propertyValues;
};

#endif
