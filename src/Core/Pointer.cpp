
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

Pointer::Pointer(DataType *parent, Datum *from, Datum *to) :
        QObject(parent),
        _from(from),
        _to(to)
{
  _graph = parent;
    _color =_graph->pointerDefaultColor();

    if ( from == to ) {
        connect(from, SIGNAL(changed()), this, SIGNAL(changed()));
        from -> addSelfPointer(this);
    }
    else {
        connect(from, SIGNAL(changed()), this, SIGNAL(changed()));
        from -> addOutPointer(this);
        connect(to, SIGNAL(changed()), this, SIGNAL(changed()));
        to -> addInPointer(this);
    }
    connect(parent, SIGNAL(complexityChanged(bool)), this, SIGNAL(changed()));

    _relativeIndex = _to -> pointers(_from).size();
    _showName = true;
    _showValue = true;
    _style = "solid";
    _width = 1;
}

Pointer::~Pointer() {

    if (_from == _to) {
        if (_from != 0){
          kDebug() << "Removing from a loop node";
          _from->removePointer(this, Datum::Self);
        }
    }
    else {
      kDebug() << "Removing from not a loop node.";
      if (_from != 0){
        _from->removePointer(this, Datum::Out);
        kDebug() << "Removed from the from node";
      }
      if (_to != 0){
        _to->removePointer(this, Datum::In);
        kDebug() << "Removed from the to node";
      }

    }
}

void Pointer::remove(Datum * node) {
    if (node){
        if (node == _from){
          _from = 0;
        }
        if (node == _to){
          _to = 0;
        }
    }
      emit removed();
    _graph->remove(this);
}


bool Pointer::showName() {
    return _showName;
}

bool Pointer::showValue() {
    return _showValue;
}

void Pointer::hideName(bool b) {
    _showName = b;
    emit changed();
    kDebug() << "Hide Name: " << b;
}

void Pointer::hideValue(bool b) {
    _showValue = b;
    emit changed();
    kDebug() << "Hide Value: " << b;
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
    return _from->scriptValue();
}
QScriptValue  Pointer::end() {
    return _to->scriptValue();
}

void Pointer::setEngine(	QScriptEngine *engine ) {
    _engine = engine;
    _scriptvalue = _engine->newQObject(this);
}

QScriptValue Pointer::scriptValue() const {
    return  _scriptvalue;
}
void Pointer::self_remove() {
    remove();
}
#endif
