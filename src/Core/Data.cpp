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

#include "Data.h"
#include "Pointer.h"
#include "DataType.h"
#include <KDebug>
#include <KGlobal>
#include <kstandarddirs.h>

#include "DynamicPropertiesList.h"


Datum::Datum(DataType *parent) : QObject(parent) {
    _graph = parent;
    _x = 0;
    _y = 0;
    _width = 0.5;
    _showName = true;
    _showValue = true;
    _begin = false;
    _end = false;
    _color = _graph->datumDefaultColor();
    _changing = false;
    _value = 0;
    _icon = "rocs_default";
    _iconpackage = KGlobal::dirs()->locate("appdata", "iconpacks/default.svg");
    kDebug() << "Node successfully created" << _iconpackage;
}

Datum::~Datum() {
    emit removed();
    empty(_in_pointers);
    empty(_out_pointers);
    empty(_self_pointers);

}

void Datum::empty(PointerList &list) {
    foreach(Pointer *e, list ) {
        e->remove();
    }
}

bool Datum::showName() {
    return _showName;
}

bool Datum::showValue() {
    return _showValue;
}

void Datum::hideName(bool b) {
    _showName = b;
    emit changed();
}

void Datum::hideValue(bool b) {
    _showValue = b;
    emit changed();
}

void Datum::setIcon(const QString& s){
    _icon = s;
    emit changed();
}

void Datum::setIconPackage(const QString& s){
    _iconpackage = s;
}

const QString& Datum::icon() const { return _icon; }

const QString& Datum::iconPackage() const { return _iconpackage; }

DataList Datum::adjacent_data() const
{
    QList<Datum*> adjacent;

    foreach(Pointer *e, _out_pointers) {
        adjacent.append( e->to()  );
    }

    if ( _graph -> directed() ) {
        foreach(Pointer *e, _self_pointers) {
            adjacent.append( e->to() );
        }
        return adjacent;
    }

    foreach(Pointer *e, _in_pointers) {
        adjacent.append( e->from() );
    }
    return adjacent;
}


PointerList Datum::adjacent_pointers() const
{
    PointerList adjacent;

    adjacent << _out_pointers;

    if ( _graph -> directed() ) {
        adjacent << _self_pointers;
    }else{
      adjacent << _in_pointers;
    }

    return adjacent;
}

void Datum::addInPointer(Pointer *e) {
    _in_pointers.append( e );
}

void Datum::addOutPointer(Pointer *e) {
    _out_pointers.append( e  );
}

void Datum::addSelfPointer(Pointer *e) {
    _self_pointers.append( e );
}

PointerList Datum::in_pointers() const {
    return _in_pointers;
}

PointerList Datum::out_pointers() const {
    return _out_pointers;
}

PointerList Datum::self_pointers() const {
    return _self_pointers;
}

Pointer* Datum::addPointer(Datum* to) {
    return _graph->addPointer(this, to);
}

void Datum::removePointer(Pointer *e, int pointerList) {
    switch (pointerList) {
    case In  : removePointer(e, _in_pointers);    break;
    case Out : removePointer(e, _out_pointers);   break;
    case Self: removePointer(e, _self_pointers);  break;
    }
}

void Datum::removePointer(Pointer* e, PointerList &list) {
      if (list.contains(e)) 
	list.removeOne(e);
}

PointerList Datum::pointers(Datum *n) {
    PointerList list;
    if (n == this) {
        return _self_pointers;
    }
    foreach (Pointer *tmp, _out_pointers) {
        if (tmp->to() == n) {
            list.append(tmp);
        }
    }
    foreach(Pointer *tmp, _in_pointers) {
        if (tmp->from() == n) {
            list.append(tmp);
        }
    }
    return list;
}

void Datum::remove() {
  _graph->remove(this);
}

//! Properties:
void Datum::setX(int x) {
    _x = x;
    if (! _changing) {
	emit changed();
    }
}

qreal Datum::x() const {
    return _x;
}

void Datum::setY(int y) {
    _y  = y;
    if (! _changing) {
	emit changed();
    }
}

void Datum::setWidth(qreal w) {
    _width = w;
    if (! _changing) {
	emit changed();
        kDebug() << "Updating node drawing";
    }
}

void Datum::setPos(qreal x, qreal y) {
    _x = x;
    _y = y;
    if (! _changing) {
	emit changed();
    }

}

qreal Datum::y() const {
    return _y;
}

qreal Datum::width() const {
    return _width;
}

void Datum::setColor(const QString& s) {
    _color = s;
    if (! _changing) {
	emit changed();
    }
}

const QString& Datum::color() const {
    return _color;
}

void Datum::setName(const QString& s) {
    _name = s;
    if (! _changing) {
	emit changed();
    }
}

const QString& Datum::name() const {
    return _name;
}

void Datum::setBegin(bool begin) {
    if (!begin) {
        _begin = false;
	if (_graph->begin() == this){
	    _graph->setBegin(0);
	}
    }
    else if (_graph->begin() == this) {
        return;
    }
    else if( _graph->setBegin(this) ) {
        _begin = true;
    }else{
	return;
    }

    if (! _changing) {
	emit changed();
    }
}

void Datum::setEnd(bool end) {
    _end = end;

    if (end) {
        _graph->addEnd(this);
    } else {
        _graph->removeEnd(this);
    }
    if (! _changing) {
	emit changed();
    }
}

bool Datum::begin() const {
    return _begin;
}

bool Datum::end() const {
    return _end;
}

const QVariant Datum::value() const {
    return _value;
}
void  Datum::setValue(QVariant s) {
    _value = s;
    if (! _changing) {
      emit changed();
    }
}

void Datum::setValue(const QString& c){
    _value = c;
    if (_changing){
      emit changed();
    }
}
void Datum::startChange() {
    _changing = true;
}

void Datum::endChange() {
    _changing = false;
    emit changed();
}

void Datum::addDynamicProperty(QString property, QVariant value){
    if (! setProperty(property.toUtf8(), value)  && value.isValid()){
      DynamicPropertiesList::New()->addProperty(this, property);
    }
}

void Datum::removeDynamicProperty(QString property){
    addDynamicProperty(property.toUtf8(), QVariant::Invalid);
    DynamicPropertiesList::New()->removeProperty(this, property);
}

#ifdef USING_QTSCRIPT
void Datum::self_remove() {
    remove();
}

QScriptValue Datum::scriptValue() const {
    return _scriptvalue;
}

void Datum::setEngine(	QScriptEngine *engine ) {
    _engine = engine;
    _scriptvalue = engine->newQObject(this);
}

QScriptValue Datum::adj_data() {
    QList<Datum*> list = adjacent_data();
    QScriptValue array = _engine->newArray();
    foreach(Datum* n, list) {
        array.property("push").call(array, QScriptValueList() << n->scriptValue());
    }
    return array;
}

QScriptValue Datum::adj_pointers() {
    PointerList list = adjacent_pointers();
    return createScriptArray(list);
}

QScriptValue Datum::input_pointers() {
    PointerList list = in_pointers();
    return createScriptArray(list);
}

QScriptValue Datum::output_pointers() {
    PointerList list = out_pointers();
    return createScriptArray(list);
}

QScriptValue Datum::loop_pointers() {
    PointerList list = self_pointers();
    return createScriptArray(list);
}

QScriptValue Datum::connected_pointers(Datum *n) {
    PointerList list = pointers(n);
    return createScriptArray(list);
}

QScriptValue Datum::createScriptArray(PointerList list) {
    QScriptValue array = _engine->newArray();
    foreach(Pointer* e, list) {
        array.property("push").call(array, QScriptValueList() << e->scriptValue());
    }
    return array;
}

#endif
