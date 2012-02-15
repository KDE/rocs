/* 
    This file is part of Rocs.
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>

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
#include "Data.h"
#include "DataStructure.h"
#include "DynamicPropertiesList.h"
#include <KDebug>
#include <QColor>

class PointerPrivate{
public:
    PointerPrivate(){}
    boost::weak_ptr<Pointer> q; // self pointer
    
    DataPtr from;
    DataPtr to;
    int relativeIndex;

    QString value;
    QString name;
    QColor color;
    int pointerType;

    bool showName;
    bool showValue;

    QString style;
    qreal width;

    DataStructurePtr dataStructure;

    QScriptValue scriptvalue;
    QScriptEngine *engine;
};

PointerPtr Pointer::create(DataStructurePtr parent, DataPtr from, DataPtr to, int pointerType) {
    PointerPtr pi(new Pointer(parent, from, to, pointerType));
    pi->d->q = pi;
    
    if ( from == to ) {
        from->addSelfPointer(pi);
    }
    else {
        from->addOutPointer(pi);
        to->addInPointer(pi);
        connect(to.get(), SIGNAL(posChanged(QPointF)), pi.get(), SIGNAL(posChanged()));
    }   
    
    return pi;
}

PointerPtr Pointer::getPointer() const {
    PointerPtr px(d->q);
    return px;
}

Pointer::Pointer(DataStructurePtr parent, DataPtr from, DataPtr to, int pointerType) :
        QObject(parent.get()), d(new PointerPrivate())
{
    d->from          = from;
    d->to            = to;
    d->dataStructure = parent;
    d->color         = d->dataStructure->pointerDefaultColor();
    d->showName      = true;
    d->showValue     = true;
    d->style         = "solid";
    d->width         = 1;
    d->relativeIndex = d->to->pointers(d->from).size();
    d->pointerType   = pointerType;
    
    connect(parent.get(), SIGNAL(complexityChanged(bool)), this, SIGNAL(changed()));
    connect(from.get(), SIGNAL(posChanged(QPointF)), this, SIGNAL(posChanged()));
}

Pointer::~Pointer() {
    if (d->from == d->to) {
        if (d->from){
          kDebug() << "Removing from a loop node";
          d->from->removePointer(getPointer(), Data::Self);
        }
    }
    else {
        kDebug() << "Removing from not a loop node.";
        if (d->from){
          d->from->removePointer(getPointer(), Data::Out);
          kDebug() << "Removed from the from node";
        }
        if (d->to){
          d->to->removePointer(getPointer(), Data::In);
          kDebug() << "Removed from the to node";
        }
    }
}

int Pointer::relativeIndex() const{
    return d->relativeIndex;
}


DataStructurePtr Pointer::dataStructure() const{
    return d->dataStructure;
}

DataPtr Pointer::from() const{
    return d->from;
}

DataPtr Pointer::to() const{
    return d->to;
}

const QString& Pointer::value() const{
    return d->value;
}

const QString& Pointer::name() const{
    return d->name;
}

void Pointer::remove() {
    emit removed();
    if (d->from){
        d->from->removePointer(getPointer());
        d->from.reset();
    }
    if (d->to){
        d->to->removePointer(getPointer());
        d->to.reset();
    }
    d->dataStructure->remove(getPointer());
}


bool Pointer::showName() {
    return d->showName;
}

bool Pointer::showValue() {
    return d->showValue;
}

void Pointer::hideName(bool b) {
    d->showName = b;
    emit changed();
}

void Pointer::hideValue(bool b) {
    d->showValue = b;
    emit changed();
}

void Pointer::setValue(const QString& value){
    d->value = value;
    emit changed();
}

void Pointer::setName(const QString& name){
    d->name = name;
    emit changed();
}

void Pointer::setColor(const QColor& color){
  if (d->color != color){
    d->color = color;
    emit changed();
  }
}

const QColor& Pointer::color() const{
     return d->color;
}

qreal Pointer::width () const {
    return d->width;
}
void Pointer::setWidth(qreal w) {
    d->width = w;
    emit changed();
}

const QString& Pointer::style() const {
    return d->style;
}
void Pointer::setStyle(const QString& s) {
    d->style = s;
    emit changed();
}

void Pointer::addDynamicProperty(QString property, QVariant value){
  if (!setProperty(property.toUtf8(), value)  &&   value.isValid()){ //if is addeding and NOT is a Q_PROPERTY
      DynamicPropertiesList::New()->addProperty(this, property);
  }
}

void Pointer::removeDynamicProperty(QString property){
  addDynamicProperty(property.toUtf8(), QVariant::Invalid);
  DynamicPropertiesList::New()->removeProperty(this, property);
}

QScriptValue Pointer::start() {
    // from==0 possible if this pointer is deleted
    if (!d->from)
        return QScriptValue();
    return d->from->scriptValue();
}
QScriptValue  Pointer::end() {
  // to==0 possible if this pointer is deleted
    if (!d->to)
        return QScriptValue();
    return d->to->scriptValue();
}

void Pointer::setEngine(	QScriptEngine *engine ) {
    d->engine = engine;
    d->scriptvalue = d->engine->newQObject(this);
}

QScriptValue Pointer::scriptValue() const {
    return  d->scriptvalue;
}
void Pointer::self_remove() {
    remove();
}

