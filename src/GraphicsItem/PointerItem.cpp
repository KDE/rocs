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
#include "math_constants.h"
#include "GraphicsLayout.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QLine>
#include <QPolygonF>
#include <QtAlgorithms>
#include <KDebug>
#include <QtCore/qmath.h>
#include <QGraphicsSimpleTextItem>

PointerItem::PointerItem(PointerPtr pointer, QGraphicsItem *parent)
    : QObject(0), QGraphicsPathItem(parent)
    , _pointer(pointer)
    , _index(pointer->relativeIndex())
    , _font(QFont("Helvetica [Cronyx]", 12))
    , _item(new QGraphicsItemGroup())
{
    connect(_pointer.get(), SIGNAL(posChanged()), this, SLOT(updatePos()));
    connect(_pointer.get(), SIGNAL(removed()), this, SLOT(remove()));
    connect(_pointer.get(), SIGNAL(changed()), this, SLOT(updateAttributes()));

    connect(_pointer.get(), SIGNAL(propertyAdded(QString)), this, SLOT(registerProperty(QString)));
    connect(_pointer.get(), SIGNAL(propertyRemoved(QString)), this, SLOT(removeProperty(QString)));
    connect(_pointer.get(), SIGNAL(propertyChanged(QString)), this, SLOT(updateProperty(QString)));
    connect(GraphicsLayout::self(), SIGNAL(changed()), this, SLOT(updatePropertyList()));

    connect(_pointer.get(), SIGNAL(directionChanged(PointerType::Direction)), this, SLOT(updateAttributes()));
    connect(_pointer.get(), SIGNAL(pointerTypeChanged(int)), this, SLOT(updateAttributes()));

    connect(GraphicsLayout::self(), SIGNAL(changed()), this, SLOT(updateAttributes()));

    setZValue(-_index);
    setFlag(ItemIsSelectable, true);
    updateAttributes();
}

PointerItem::~PointerItem()
{
    foreach (QString name, _propertyValues.keys()) {
        delete _propertyValues[name];
    }
    _propertyValues.clear();
    delete _item;
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
    if (!_pointer || !_pointer->from() || !_pointer->to()) {
        return;
    }
    QLine q(_pointer->from()->x(), _pointer->from()->y(),    _pointer->to()->x(),  _pointer->to()->y());
    qreal size = qSqrt(qPow(q.dx(), 2) + qPow(q.dy(), 2));
    if (_pointer->from() != _pointer->to() && size < 20) {
        setPath(QPainterPath());
    } else {
        setPath(createCurves());
    }
    updateAttributes();
}

void PointerItem::updateAttributes()
{
    Qt::PenStyle style = pointer()->style();

    setPen(QPen(QBrush(QColor(_pointer->color())), _pointer->width(), style, Qt::RoundCap, Qt::RoundJoin));
    this->hide();
    QPointF middle = path().pointAtPercent(0.5);

    if (_pointer->from() == _pointer->to()) {
        qreal x1 = boundingRect().x() + boundingRect().width() + 5;
        qreal y1 = boundingRect().y() + (boundingRect().height() / 2) - 10;
        updatePropertyList(x1, y1);
    } else {
        updatePropertyList(middle.x() / 2, middle.y());
    }

    update();
}

void PointerItem::updatePropertyList(qreal x, qreal y) {
    qreal offset = 0;
    foreach (QString property, _pointer->properties()) {
        if (!_propertyValues.contains(property)) {
            kError() << "Cannot update unknown property : " << property;
            continue;
        }
        if (_propertyValues[property]->isVisible() == false) {
            continue;
        }
        _propertyValues[property]->setPos(x+20, y + offset);
        _propertyValues[property]->update();
        offset += 20;
    }
}


void PointerItem::registerProperty(QString name)
{
    if (_propertyValues.contains(name)) {
        return;
    }
    _propertyValues.insert(name, new QGraphicsSimpleTextItem(_pointer->property(name.toStdString().c_str()).toString()));
    _propertyValues[name]->setFlags(ItemIgnoresTransformations);
    _propertyValues[name]->setFont(_font);
    _propertyValues[name]->setZValue(zValue() + 1);
    _item->addToGroup(_propertyValues[name]);

    updateAttributes();
}

void PointerItem::updateProperty(QString name)
{
    if (!_propertyValues.contains(name)) {
        registerProperty(name);
        return;
    }
    PointerTypePtr pointerType = _pointer->dataStructure()->document()->pointerType(_pointer->pointerType());
    _propertyValues[name]->setText(_pointer->property(name.toStdString().c_str()).toString());
    _propertyValues[name]->setVisible(pointerType->isPropertyVisible(name));
    _propertyValues[name]->update();
    updateAttributes();
}

void PointerItem::removeProperty(QString name)
{
    if (_propertyValues.contains(name)) {
        kWarning() << "Property not removed: not registered at DataItem.";
        return;
    }
    _propertyValues[name]->setVisible(false);
    _item->removeFromGroup(_propertyValues[name]);
    delete _propertyValues[name];
    _propertyValues.remove(name);

    updateAttributes();
}

QGraphicsItem* PointerItem::propertyListItem() const
{
    return _item;
}


void PointerItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (isSelected()) {
        painter->setPen(QPen(Qt::black, _pointer->width(),  Qt::DotLine));
    }
    QGraphicsPathItem::paint(painter, option, widget);
}
