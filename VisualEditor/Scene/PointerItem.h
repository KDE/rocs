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

class PointerItemPrivate;
class QGraphicsSceneMouseEvent;
class Pointer;
class QGraphicsSimpleTextItem;

/**
 * \class PointerItem
 * \brief Graphics item for data connections.
 *
 * This is the base class for graphics item represenations of connections. Data structure plugins
 * may provide their own implementations to present the connections with a different look.
 */
class VISUALEDITOR_EXPORT PointerItem : public QObject, public QGraphicsPathItem
{
    Q_OBJECT

public:
    /**
     * Default constructor.
     *
     * \param pointer the pointer that this graphics item presents
     * \param parent the parent for the graphics item, if 0 item must be added to scene manually
    */
    explicit PointerItem(PointerPtr pointer, QGraphicsItem *parent = 0);

    /**
     * Default destructor.
     */
    virtual ~PointerItem();

    /**
     * \see QGraphicsItem::type
     */
    enum { Type = UserType + 3 };

    /**
     * \return the type of the item
     * \see QGraphicsItem::type()
     */
    int type() const;

    /**
     * \return the graphics item to be displayed at the scene
     */
    QGraphicsItem *propertyListItem() const;

    /**
     * \return the pointer that is represented by this graphics item
     */
    PointerPtr pointer() const;

protected:
    /**
     * when there's a mouse click on the element, this method is invoked
     * \param event the mouse object
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    /**
     * when there's a mouse release on the element, this method is invoked
     * \param event the mouse object
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual QPainterPath createCurves() = 0;

public Q_SLOTS:
    void remove();
    void updatePos();
    void updateAttributes();
    void updateProperty(const QString& name);
    void registerProperty(const QString& name);
    void removeProperty(const QString& name);

private:
    // d-pointer
    PointerItemPrivate * const d;
};

#endif
