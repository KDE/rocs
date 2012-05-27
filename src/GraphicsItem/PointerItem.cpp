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

#include "PointerItem.h"

#include "DataItem.h"
#include "GraphScene.h"
#include "Data.h"
#include "Pointer.h"
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
{
    _pointer = pointer;
    _index = _pointer->relativeIndex();
    _name = new QGraphicsSimpleTextItem(this);
    _value = new QGraphicsSimpleTextItem(this);
    setZValue(- _index);
    setFlag(ItemIsSelectable, true);
    connectSignals();
    updateAttributes();
}

PointerItem::~PointerItem()
{
    //dynamic_cast<GraphScene*>(scene())->removeGItem(this);
}

void PointerItem::connectSignals()
{
    connect(_pointer.get(), SIGNAL(posChanged()), this, SLOT(updatePos()));
    connect(_pointer.get(), SIGNAL(removed()), this, SLOT(remove()));
    connect(_pointer.get(), SIGNAL(changed()), this, SLOT(updateAttributes()));
    connect(_pointer.get(), SIGNAL(pointerTypeChanged(int)), this, SLOT(updateAttributes()));

    connect(GraphicsLayout::self(), SIGNAL(changed()), this, SLOT(updateAttributes()));
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
    Qt::PenStyle s;
    if (_pointer->style() == "dash") {
        s = Qt::DashLine;
    } else if (_pointer->style() == "dot") {
        s = Qt::DotLine;
    } else if (_pointer->style() == "dash dot") {
        s = Qt::DashDotLine;
    } else if (_pointer->style() == "solid") {
        s = Qt::SolidLine;
    } else                                 {
        s = Qt::SolidLine;
    }

    setPen(QPen(QBrush(QColor(_pointer->color())), _pointer->width(), s, Qt::RoundCap, Qt::RoundJoin));
    _value->hide();
    _name->hide();
    this->hide();
    QPointF middle = path().pointAtPercent(0.5);
    _name->setText(_pointer->name());
    _value->setText(_pointer->value());

    if (_pointer->from() == _pointer->to()) {
        qreal x1 = boundingRect().x() + boundingRect().width() + 5;
        qreal y1 = boundingRect().y() + (boundingRect().height() / 2) - 10;
        _name->setPos(x1, y1);
        _value->setPos(x1, y1 + 14);
    } else {
        _name->setPos(middle.x() - _name->boundingRect().width() / 2, middle.y());
        _value->setPos(middle.x() - _name->boundingRect().width() / 2, middle.y() - 14);
    }

    // overall visibility for pointer
    if (_pointer->isVisible()) {
        if (_pointer->showValue()) {
            _value->show();
        }
        if (_pointer->showName()) {
            _name->show();
        }
        this->show();
    }

    update();
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
