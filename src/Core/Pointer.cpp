
/***************************************************************************
 * main.cpp
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/

#include "Pointer.h"
#include "Data.h"
#include "DataType.h"
#include "DynamicPropertiesList.h"
#include <KDebug>

class PointerPrivate{
public:
    PointerPrivate(){}
    Datum *from;
    Datum *to;
    int relativeIndex;

    QString value;
    QString name;
    QString color;

    bool showName;
    bool showValue;

    QString style;
    double width;

     DataType *dataType;

    QScriptValue scriptvalue;
    QScriptEngine *engine;
};

Pointer::Pointer(DataType *parent, Datum *from, Datum *to) :
        QObject(parent), d(new PointerPrivate())
{
    d->from     = from;
    d->to       = to;
    d->dataType = parent;
    d->color    = d->dataType->pointerDefaultColor();

    connect(parent, SIGNAL(complexityChanged(bool)), this, SIGNAL(changed()));
    connect(from, SIGNAL(changed()), this, SIGNAL(changed()));

    if ( from == to ) {
        from -> addSelfPointer(this);
    }
    else {
        from -> addOutPointer(this);
        to -> addInPointer(this);
        connect(to, SIGNAL(changed()), this, SIGNAL(changed()));
    }

    d->relativeIndex  = d->to-> pointers(d->from).size();
    d->showName       = true;
    d->showValue      = true;
    d->style          = "solid";
    d->width          = 1;
}

Pointer::~Pointer() {
    if (d->from == d->to) {
        if (d->from != 0){
          kDebug() << "Removing from a loop node";
          d->from->removePointer(this, Datum::Self);
        }
    }
    else {
        kDebug() << "Removing from not a loop node.";
        if (d->from != 0){
          d->from->removePointer(this, Datum::Out);
          kDebug() << "Removed from the from node";
        }
        if (d->to != 0){
          d->to->removePointer(this, Datum::In);
          kDebug() << "Removed from the to node";
        }
    }
    delete d;
}

int Pointer::relativeIndex() const{
    return d->relativeIndex;
}

void Pointer::emitChangedSignal(){
    emit changed();
}

DataType *Pointer::dataType() const{
    return d->dataType;
}

Datum *Pointer::from() const{
    return d->from;
}

Datum *Pointer::to() const{
    return d->to;
}

const QString& Pointer::value() const{
    return d->value;
}

const QString& Pointer::name() const{
    return d->name;
}

void Pointer::remove() {
      if (d->from || d->to){
        emit removed();
        if (d->from){
          d->from->removePointer(this);
          d->from = 0;
        }
        if (d->to){
          d->to->removePointer(this);
          d->to = 0;
        }
    }

 d->dataType->remove(this);

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

void Pointer::setColor(const QString& color){
    d->color = color;
    emit changed();
}

const QString& Pointer::color() const{
     return d->color;
}

qreal Pointer::width () const {
    return d->width;
}
void Pointer::setWidth(double w) {
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

#ifdef USING_QTSCRIPT

QScriptValue Pointer::start() {
    return d->from->scriptValue();
}
QScriptValue  Pointer::end() {
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
#endif
