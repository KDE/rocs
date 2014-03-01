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

#include "PointerItem.h"

#include "DataItem.h"
#include "GraphScene.h"
#include "Data.h"
#include "Pointer.h"
#include "PointerType.h"
#include "DataStructure.h"
#include "GraphicsLayout.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QLine>
#include <QPolygonF>
#include <QtAlgorithms>
#include <QDebug>
#include <QtCore/qmath.h>
#include <QGraphicsSimpleTextItem>

// class PointerItemPrivate

class PointerItemPrivate
{
public:
    PointerItemPrivate(PointerPtr pointer)
    : _pointer(pointer)
    , _index(pointer->relativeIndex())
    , _font(QFont("Helvetica [Cronyx]", 12))
    , _item(new QGraphicsItemGroup())
    {
    }

    ~PointerItemPrivate()
    {
        qDeleteAll(_propertyValues);
        _propertyValues.clear();
        delete _item;
    }

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

void PointerItemPrivate::updatePropertyList(qreal x, qreal y) {
    qreal offset = 0;
    foreach (const QString& property, _pointer->properties()) {
        if (!_propertyValues.contains(property)) {
            qCritical() << "Cannot update unknown property : " << property;
            continue;
        }
        if (_propertyValues[property]->isVisible() == false) {
            continue;
        }
        _propertyValues[property]->setPos(x + 20, y + offset);
        _propertyValues[property]->update();
        offset += 20;
    }
}


// class PointerItem

PointerItem::PointerItem(PointerPtr pointer, QGraphicsItem *parent)
    : QObject(0)
    , QGraphicsPathItem(parent)
    , d(new PointerItemPrivate(pointer))
{
    connect(pointer.get(), SIGNAL(posChanged()), this, SLOT(updatePos()));
    connect(pointer.get(), SIGNAL(removed()), this, SLOT(remove()));
    connect(pointer.get(), SIGNAL(changed()), this, SLOT(updateAttributes()));

    connect(pointer.get(), SIGNAL(propertyAdded(QString)), this, SLOT(registerProperty(QString)));
    connect(pointer.get(), SIGNAL(propertyRemoved(QString)), this, SLOT(removeProperty(QString)));
    connect(pointer.get(), SIGNAL(propertyChanged(QString)), this, SLOT(updateProperty(QString)));
    connect(GraphicsLayout::self(), SIGNAL(changed()), this, SLOT(updateAttributes()));

    connect(pointer.get(), SIGNAL(directionChanged(PointerType::Direction)), this, SLOT(updateAttributes()));
    connect(pointer.get(), SIGNAL(pointerTypeChanged(int)), this, SLOT(updateAttributes()));

    setZValue(-1 - d->_index);
    setFlag(ItemIsSelectable, true);

    // register properties
    foreach (const QString &property, d->_pointer->properties()) {
        registerProperty(property);
    }

    updateAttributes();
    this->show();
}

PointerItem::~PointerItem()
{
    delete d;
}

void PointerItem::mousePressEvent(QGraphicsSceneMouseEvent */*event*/)
{
}

void PointerItem::mouseReleaseEvent(QGraphicsSceneMouseEvent */*event*/)
{
}

void PointerItem::remove()
{
    if (scene()) {
        scene()->removeItem(this);
    }
    deleteLater();
}

void PointerItem::updatePos()
{
    if (!d->_pointer || !d->_pointer->from() || !d->_pointer->to()) {
        return;
    }
    QLine q(d->_pointer->from()->x(), d->_pointer->from()->y(), d->_pointer->to()->x(), d->_pointer->to()->y());
    qreal size = qSqrt(qPow(q.dx(), 2) + qPow(q.dy(), 2));
    if (d->_pointer->from() != d->_pointer->to() && size < 20) {
        setPath(QPainterPath());
    } else {
        setPath(createCurves());
    }
    updateAttributes();
}

void PointerItem::updateAttributes()
{
    Qt::PenStyle style = pointer()->style();

    setPen(QPen(QBrush(QColor(d->_pointer->color())), d->_pointer->width(), style, Qt::RoundCap, Qt::RoundJoin));
    this->hide();
    QPointF middle = path().pointAtPercent(0.5);

    if (d->_pointer->from() == d->_pointer->to()) {
        qreal x1 = boundingRect().x() + boundingRect().width() + 5;
        qreal y1 = boundingRect().y() + (boundingRect().height() / 2) - 10;
        d->updatePropertyList(x1, y1);
    } else {
        d->updatePropertyList(middle.x(), middle.y());
    } if (d->_pointer->isVisible() == true) {
        this->show();
        PointerTypePtr pointerType = d->_pointer->dataStructure()->document()->pointerType(d->_pointer->pointerType());
        foreach (const QString& property, d->_pointer->properties()) {
            if (!d->_propertyValues.contains(property)) {
                qWarning() << "No graphics item registered for property : " << property;
                continue;
            }
            d->_propertyValues[property]->setVisible(pointerType->isPropertyVisible(property));
        }
    } else {
        this->hide();
        foreach (const QString& property, d->_pointer->properties()) {
            if (!d->_propertyValues.contains(property)) {
                qWarning() << "No graphics item registered for property : " << property;
                continue;
            }
            d->_propertyValues[property]->setVisible(false);
        }
    }
    update();
}

void PointerItem::registerProperty(const QString& name)
{
    if (d->_propertyValues.contains(name)) {
        return;
    }
    PointerTypePtr pointerType = d->_pointer->dataStructure()->document()->pointerType(d->_pointer->pointerType());
    d->_propertyValues.insert(name, new QGraphicsSimpleTextItem(d->_pointer->property(name.toLatin1()).toString()));
    d->_propertyValues[name]->setFlags(ItemIgnoresTransformations);
    d->_propertyValues[name]->setVisible(pointerType->isPropertyVisible(name));
    d->_propertyValues[name]->setFont(d->_font);
    d->_propertyValues[name]->setZValue(zValue() + 1);
    d->_item->addToGroup(d->_propertyValues[name]);

    updateAttributes();
}

void PointerItem::updateProperty(const QString& name)
{
    if (!d->_propertyValues.contains(name)) {
        registerProperty(name);
        return;
    }
    PointerTypePtr pointerType = d->_pointer->dataStructure()->document()->pointerType(d->_pointer->pointerType());
    d->_propertyValues[name]->setText(d->_pointer->property(name.toLatin1()).toString());
    d->_propertyValues[name]->setVisible(pointerType->isPropertyVisible(name));
    d->_propertyValues[name]->update();
    updateAttributes();
}

void PointerItem::removeProperty(const QString& name)
{
    if (d->_propertyValues.contains(name)) {
        qWarning() << "Property not removed: not registered at DataItem.";
        return;
    }
    d->_propertyValues[name]->setVisible(false);
    d->_item->removeFromGroup(d->_propertyValues[name]);
    delete d->_propertyValues[name];
    d->_propertyValues.remove(name);

    updateAttributes();
}

int PointerItem::type() const
{
    return Type;
}

QGraphicsItem* PointerItem::propertyListItem() const
{
    return d->_item;
}

PointerPtr PointerItem::pointer() const {
    return d->_pointer;
}

void PointerItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (isSelected()) {
        painter->setPen(QPen(Qt::black, d->_pointer->width(),  Qt::DotLine));
    }
    QGraphicsPathItem::paint(painter, option, widget);
}
