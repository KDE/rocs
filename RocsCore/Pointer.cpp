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
#include "QtScriptBackend.h"

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

    QColor color;
    qreal width;
    PointerTypePtr pointerType;
    bool visible;

    DataStructurePtr dataStructure;

    QScriptValue scriptvalue;
    QScriptEngine *engine;
};

PointerPtr Pointer::create(DataStructurePtr dataStructure, DataPtr from, DataPtr to, int pointerType)
{
    return create<Pointer>(dataStructure, from, to, pointerType);
}

void Pointer::setQpointer(PointerPtr q)
{
    d->q = q;
}

void Pointer::initialize()
{
    // register pointer at endpoints and connect to changes
    d->from->registerOutPointer(getPointer());
    d->to->registerInPointer(getPointer());
    updateRelativeIndex();
    connect(d->to.get(), SIGNAL(posChanged(QPointF)), this, SIGNAL(posChanged()));
    connect(d->from.get(), SIGNAL(pointerListChanged()), this, SLOT(updateRelativeIndex()));
    connect(d->dataStructure.get(), SIGNAL(complexityChanged(bool)), this, SIGNAL(changed()));
    connect(d->from.get(), SIGNAL(posChanged(QPointF)), this, SIGNAL(posChanged()));

    // register properties and connect to changes
    installEventFilter(this);
    foreach(const QString& property, d->pointerType->properties()) {
        addDynamicProperty(property, d->pointerType->propertyDefaultValue(property));
    }
    connect(d->pointerType.get(), SIGNAL(propertyAdded(QString,QVariant)),
            this, SLOT(addDynamicProperty(QString,QVariant)));
    connect(d->pointerType.get(), SIGNAL(propertyRemoved(QString)),
            this, SLOT(removeDynamicProperty(QString)));
    connect(d->pointerType.get(), SIGNAL(propertyRenamed(QString,QString)),
            this, SLOT(renameDynamicProperty(QString,QString)));
    connect(d->pointerType.get(), SIGNAL(propertyDefaultValueChanged(QString)),
            this, SLOT(updateDynamicProperty(QString)));
    connect(d->pointerType.get(), SIGNAL(propertyVisibilityChanged(QString)),
            this, SLOT(updateDynamicProperty(QString)));
}

PointerPtr Pointer::getPointer() const
{
    PointerPtr px(d->q);
    return px;
}

Pointer::Pointer(DataStructurePtr parent, DataPtr from, DataPtr to, int pointerType) :
    d(new PointerPrivate())
{
    d->from          = from;
    d->to            = to;
    d->visible       = true;
    d->dataStructure = parent;
    d->color         = d->dataStructure->document()->pointerType(pointerType)->defaultColor();
    d->width         = 1;
    d->pointerType   = d->dataStructure->document()->pointerType(pointerType);

    connect(d->pointerType.get(), SIGNAL(directionChanged(PointerType::Direction)),
            this, SIGNAL(directionChanged(PointerType::Direction)));
    connect(d->pointerType.get(), SIGNAL(styleChanged()),
            this, SIGNAL(changed()));
    connect(d->pointerType.get(), SIGNAL(removed()), this, SLOT(remove()));
}

Pointer::~Pointer()
{
}

bool Pointer::eventFilter(QObject *obj, QEvent *event){
    if (event->type() == QEvent::DynamicPropertyChange) {
        if (QDynamicPropertyChangeEvent* const dynEvent = dynamic_cast<QDynamicPropertyChangeEvent*>(event)) {
            event->accept();
            emit(propertyChanged(dynEvent->propertyName()));
        }
    }
    return QObject::eventFilter(obj, event);
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

int Pointer::pointerType() const
{
    return d->pointerType->identifier();
}


void Pointer::setPointerType(int typeIdentifier)
{
    Q_ASSERT(d->dataStructure->document()->pointerTypeList().contains(typeIdentifier));

    // disconnect from old type
    d->pointerType.get()->disconnect(this);

    // change type
    d->pointerType = d->dataStructure->document()->pointerType(typeIdentifier);
    d->dataStructure->updatePointer(getPointer());

    // connect to new pointer type and emit information about
    connect(d->pointerType.get(), SIGNAL(directionChanged(PointerType::Direction)),
            this, SIGNAL(directionChanged(PointerType::Direction)));
    connect(d->pointerType.get(), SIGNAL(removed()), this, SLOT(remove()));
    emit pointerTypeChanged(typeIdentifier);
}

QList<QString> Pointer::properties() const
{
    return d->pointerType->properties();
}

void Pointer::remove()
{
    d->pointerType->disconnect(this);
    d->from->disconnect(this);
    d->to->disconnect(this);

    // create self reference to ensure pointer to live up to the end of this method call
    // creating the reference increases the shared pointer count by 1
    PointerPtr pointer = getPointer();
    if (d->from) {
        d->from->remove(pointer);
    }
    if (d->to) {
        d->to->remove(pointer);
    }
    d->dataStructure->remove(pointer);
    emit removed();
}

void Pointer::self_remove()
{
    kWarning() << "self_remove() is a deprecated function, please use remove()";
    remove();
}


PointerType::Direction Pointer::direction() const
{
    return d->pointerType->direction();
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

void Pointer::setColor(const QColor& color)
{
    if (d->color != color) {
        d->color = color;
        emit changed();
    }
}

QColor Pointer::color() const
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

void Pointer::addDynamicProperty(const QString& property, const QVariant& value)
{
    if (!Document::isValidIdentifier(property)) {
        kWarning() << "Property identifier \"" << property << "\" is not valid: aborting";
        return;
    }
    setProperty(property.toAscii(), value);
    emit(propertyAdded(property));
}

void Pointer::removeDynamicProperty(const QString& property)
{
    setProperty(property.toAscii(), QVariant::Invalid);
    emit(propertyRemoved(property));
}

void Pointer::updateDynamicProperty(const QString& property)
{
    if (this->property(property.toStdString().c_str()) == QVariant::Invalid
        || this->property(property.toStdString().c_str()).toString().isEmpty()
    ) {
        this->setProperty(property.toStdString().c_str(), d->pointerType->propertyDefaultValue(property));
    }
    emit propertyChanged(property);
}

void Pointer::renameDynamicProperty(const QString& oldName, const QString& newName)
{
    if (!Document::isValidIdentifier(newName)) {
        kWarning() << "Property identifier \"" << newName << "\" is not valid: aborting";
        return;
    }
    setProperty(newName.toStdString().c_str(), property(oldName.toStdString().c_str()));
    setProperty(oldName.toStdString().c_str(), QVariant::Invalid);
}

QScriptValue Pointer::set_type(int pointerType)
{
    if (!d->dataStructure->document()->pointerTypeList().contains(pointerType)) {
        dataStructure()->document()->engineBackend()->debug(
            i18n("Could not set pointer type: pointer type does not exist."));
        return d->dataStructure->engine()->newVariant(false);
    }
    setPointerType(pointerType);
    return d->dataStructure->engine()->newVariant(true);
}

QScriptValue Pointer::type() const
{
    return d->dataStructure->engine()->newVariant(d->pointerType->identifier());
}

void Pointer::remove_property(const QString& name)
{
    removeDynamicProperty(name);
}

void Pointer::add_property(const QString& name, const QString& value)
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
