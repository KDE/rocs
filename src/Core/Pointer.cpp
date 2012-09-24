/*
    This file is part of Rocs.
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "Pointer.h"
#include "PointerType.h"
#include "Data.h"
#include "DataStructure.h"
#include "DynamicPropertiesList.h"
#include <KDebug>
#include <QColor>
#include <boost/weak_ptr.hpp>

class PointerPrivate
{
public:
    PointerPrivate() {}
    boost::weak_ptr<Pointer> q; // self pointer

    DataPtr from;
    DataPtr to;
    int relativeIndex;

    QString value;
    QString name;
    QColor color;
    qreal width;
    PointerTypePtr pointerType;

    bool showName;
    bool showValue;
    bool visible;

    DataStructurePtr dataStructure;

    QScriptValue scriptvalue;
    QScriptEngine *engine;
};

PointerPtr Pointer::create(DataStructurePtr parent, DataPtr from, DataPtr to, int pointerType)
{
    PointerPtr pi(new Pointer(parent, from, to, pointerType));
    pi->d->q = pi;

    from->registerOutPointer(pi);
    to->registerInPointer(pi);
    pi->updateRelativeIndex();

    connect(to.get(), SIGNAL(posChanged(QPointF)), pi.get(), SIGNAL(posChanged()));
    connect(from.get(), SIGNAL(pointerListChanged()), pi.get(), SLOT(updateRelativeIndex()));
    connect(parent.get(), SIGNAL(complexityChanged(bool)), pi.get(), SIGNAL(changed()));
    connect(from.get(), SIGNAL(posChanged(QPointF)), pi.get(), SIGNAL(posChanged()));

    return pi;
}

PointerPtr Pointer::getPointer() const
{
    PointerPtr px(d->q);
    return px;
}

Pointer::Pointer(DataStructurePtr parent, DataPtr from, DataPtr to, int pointerType) :
    QObject(parent.get()), d(new PointerPrivate())
{
    d->from          = from;
    d->to            = to;
    d->visible       = true;
    d->dataStructure = parent;
    d->color         = d->dataStructure->document()->pointerType(pointerType)->defaultColor();
    d->showName      = true; //FIXME remove with next refactoring to properties
    d->showValue     = true; //FIXME remove with next refactoring to properties
    d->width         = 1;
    d->pointerType   = d->dataStructure->document()->pointerType(pointerType);

    connect(d->pointerType.get(), SIGNAL(directionChanged(PointerType::Direction)),
            this, SIGNAL(directionChanged(PointerType::Direction)));
    connect(d->pointerType.get(), SIGNAL(styleChanged()),
            this, SIGNAL(changed()));
}

Pointer::~Pointer()
{
    if (d->from == d->to) {
        if (d->from) {
            kDebug() << "Removing from a loop node";
            d->from->removePointer(getPointer());
        }
    } else {
        kDebug() << "Removing from not a loop node.";
        if (d->from) {
            d->from->removePointer(getPointer());
            kDebug() << "Removed from the from node";
        }
        if (d->to) {
            d->to->removePointer(getPointer());
            kDebug() << "Removed from the to node";
        }
    }
}

int Pointer::relativeIndex() const
{
    return d->relativeIndex;
}

void Pointer::updateRelativeIndex() {
    d->relativeIndex = d->to->pointerList(d->from).indexOf(getPointer());
}

DataStructurePtr Pointer::dataStructure() const
{
    return d->dataStructure;
}

DataPtr Pointer::from() const
{
    return d->from;
}

DataPtr Pointer::to() const
{
    return d->to;
}

const QString& Pointer::value() const
{
    return d->value;
}

const QString& Pointer::name() const
{
    return d->name;
}

int Pointer::pointerType() const
{
    return d->pointerType->identifier();
}


void Pointer::setPointerType(int typeIdentifier)
{
    Q_ASSERT(d->dataStructure->document()->pointerTypeList().contains(typeIdentifier));

    // disconnect from old type
    disconnect(d->pointerType.get());

    // change type
    d->pointerType = d->dataStructure->document()->pointerType(typeIdentifier);
    d->dataStructure->updatePointer(getPointer());

    // connect to new pointer type and emit information about
    connect(d->pointerType.get(), SIGNAL(directionChanged(PointerType::Direction)),
            this, SIGNAL(directionChanged(PointerType::Direction)));
    emit pointerTypeChanged(typeIdentifier);
}

void Pointer::remove()
{
    emit removed();
    if (d->from) {
        d->from->removePointer(getPointer());
        d->from.reset();
    }
    if (d->to) {
        d->to->removePointer(getPointer());
        d->to.reset();
    }
    d->dataStructure->remove(getPointer());
}

PointerType::Direction Pointer::direction() const
{
    return d->pointerType->direction();
}

bool Pointer::isNameVisible() const
{
    return d->showName;
}

bool Pointer::isValueVisible() const
{
    return d->showValue;
}

void Pointer::setNameVisible(bool visible)
{
    d->showName = visible;
    emit changed();
}

void Pointer::setValueVisible(bool visible)
{
    d->showValue = visible;
    emit changed();
}

void Pointer::setVisible(bool visible)
{
    d->visible = visible;
    emit changed();
}

bool Pointer::isVisible() const
{
    return d->visible;
}

void Pointer::setValue(const QString& value)
{
    d->value = value;
    emit changed();
}

void Pointer::setName(const QString& name)
{
    d->name = name;
    emit changed();
}

void Pointer::setColor(const QColor& color)
{
    if (d->color != color) {
        d->color = color;
        emit changed();
    }
}

const QColor& Pointer::color() const
{
    return d->color;
}

qreal Pointer::width() const
{
    return d->width;
}

void Pointer::setWidth(qreal width)
{
    d->width = width;
    emit changed();
}

Qt::PenStyle Pointer::style() const
{
    return d->pointerType->lineStyle();
}

void Pointer::addDynamicProperty(const QString & property, const QVariant& value)
{
        DynamicPropertiesList::New()->addProperty(this, property.toAscii(), value);
}

void Pointer::removeDynamicProperty(QString property)
{
    DynamicPropertiesList::New()->removeProperty(this, property.toAscii());
}

QScriptValue Pointer::set_type(int pointerType)
{
    if (!d->dataStructure->document()->pointerTypeList().contains(pointerType)) {
        kDebug() << "pointer type does not exist."; //TODO give script error
        return d->dataStructure->engine()->newVariant(false);
    }
    setPointerType(pointerType);
    return d->dataStructure->engine()->newVariant(true);
}

QScriptValue Pointer::type() const
{
    return d->dataStructure->engine()->newVariant(d->pointerType->identifier());
}

void Pointer::remove_property (const QString& name)
{
    removeDynamicProperty(name);
}


void Pointer::add_property(QString name, QString value)
{
    addDynamicProperty(name, value);
}

QScriptValue Pointer::start() const
{
    // from==0 possible if this pointer is deleted
    if (!d->from) {
        return QScriptValue();
    }
    return d->from->scriptValue();
}

QScriptValue Pointer::end() const
{
    // to==0 possible if this pointer is deleted
    if (!d->to) {
        return QScriptValue();
    }
    return d->to->scriptValue();
}

void Pointer::setEngine(QScriptEngine *engine)
{
    d->engine = engine;
    d->scriptvalue = d->engine->newQObject(this);
}

QScriptValue Pointer::scriptValue() const
{
    return  d->scriptvalue;
}
