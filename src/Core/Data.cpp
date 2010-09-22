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
class DatumPrivate{
public:
  DatumPrivate(Datum *classPtr) : q(classPtr){}
  
    PointerList _in_pointers;
    PointerList _out_pointers;
    PointerList _self_pointers;

    //! fixed properties
    qreal _x;
    qreal _y;
    qreal _width;

    bool _begin;
    bool _end;
    bool _changing;
    bool _showName;
    bool _showValue;

    DataType *_dataType;

    QString _name;
    QString _color;
    QString _iconpackage;
    QString _icon;

    QVariant _value;
    QScriptValue _scriptvalue;
    QScriptEngine *_engine;
    
    void empty(PointerList &list) {
        for(int i = 0; i < list.size(); ++i){
            list.at(i)->remove(q);
        }
    }
private:
  Datum *q;
};

DataType *Datum::dataType() const{ return d->_dataType; }

Datum::Datum(DataType *parent) : QObject(parent), d(new DatumPrivate(this)) {
    d->_dataType = parent;
    d->_x = 0;
    d->_y = 0;
    d->_width = 0.5;
    d->_showName = true;
    d->_showValue = true;
    d->_begin = false;
    d->_end = false;
    d->_color = d->_dataType->datumDefaultColor();
    d->_changing = false;
    d->_value = 0;
    d->_icon = "rocs_default";
    d->_iconpackage = KGlobal::dirs()->locate("appdata", "iconpacks/default.svg");
    kDebug() << "Node successfully created" << d->_iconpackage;
}

Datum::~Datum() {
    emit removed();
    d->empty(d->_in_pointers);
    d->empty(d->_out_pointers);
    d->empty(d->_self_pointers);
}

bool Datum::showName() {
    return d->_showName;
}

bool Datum::showValue() {
    return d->_showValue;
}

void Datum::hideName(bool b) {
    d->_showName = b;
    emit changed();
}

void Datum::hideValue(bool b) {
    d->_showValue = b;
    emit changed();
}

void Datum::setIcon(const QString& s){
    d->_icon = s;
    emit changed();
}

void Datum::setIconPackage(const QString& s){
   d-> _iconpackage = s;
}

const QString& Datum::icon() const {
    return d->_icon; 
}

const QString& Datum::iconPackage() const { 
    return d-> _iconpackage; 
}

DataList Datum::adjacent_data() const
{
    QList<Datum*> adjacent;

    foreach(Pointer *e, d->_out_pointers) {
        adjacent.append( e->to()  );
    }

    if ( d->_dataType -> directed() ) {
        foreach(Pointer *e, d->_self_pointers) {
            adjacent.append( e->to() );
        }
        return adjacent;
    }

    foreach(Pointer *e, d->_in_pointers) {
        adjacent.append( e->from() );
    }
    return adjacent;
}


PointerList Datum::adjacent_pointers() const
{
    PointerList adjacent;

    adjacent << d->_out_pointers;

    if ( d->_dataType -> directed() ) {
        adjacent << d-> _self_pointers;
    }else{
      adjacent << d->_in_pointers;
    }

    return adjacent;
}

void Datum::addInPointer(Pointer *e) {
    d-> _in_pointers.append( e );
}

void Datum::addOutPointer(Pointer *e) {
    d->_out_pointers.append( e  );
}

void Datum::addSelfPointer(Pointer *e) {
    d->_self_pointers.append( e );
}

PointerList Datum::in_pointers() const {
    return d->_in_pointers;
}

PointerList Datum::out_pointers() const {
    return d->_out_pointers;
}

PointerList Datum::self_pointers() const {
    return d->_self_pointers;
}

Pointer* Datum::addPointer(Datum* to) {
    return d->_dataType->addPointer(this, to);
}

void Datum::removePointer(Pointer *e, int pointerList) {
    switch (pointerList) {
    case In  : removePointer(e, d->_in_pointers);    break;
    case Out : removePointer(e, d->_out_pointers);   break;
    case Self: removePointer(e, d->_self_pointers);  break;
    }
}

void Datum::removePointer(Pointer* e, PointerList &list) {
    if (list.contains(e))
      list.removeOne(e);
}

PointerList Datum::pointers(Datum *n) {
    PointerList list;
    if (n == this) {
        return d->_self_pointers;
    }
    foreach (Pointer *tmp, d->_out_pointers) {
        if (tmp->to() == n) {
            list.append(tmp);
        }
    }
    foreach(Pointer *tmp, d->_in_pointers) {
        if (tmp->from() == n) {
            list.append(tmp);
        }
    }
    return list;
}

void Datum::remove() {
  d->_dataType->remove(this);
}

//! Properties:
void Datum::setX(int x) {
    d->_x = x;
    if (! d->_changing) {
	emit changed();
    }
}

qreal Datum::x() const {
    return d->_x;
}

void Datum::setY(int y) {
    d->_y  = y;
    if (! d->_changing) {
	emit changed();
    }
}

void Datum::setWidth(qreal w) {
    d->_width = w;
    if (! d->_changing) {
	emit changed();
        kDebug() << "Updating node drawing";
    }
}

void Datum::setPos(qreal x, qreal y) {
    d->_x = x;
    d->_y = y;
    if (! d->_changing) {
	emit changed();
    }

}

qreal Datum::y() const {
    return d->_y;
}

qreal Datum::width() const {
    return d->_width;
}

void Datum::setColor(const QString& s) {
    d->_color = s;
    if (! d->_changing) {
	emit changed();
    }
}

const QString& Datum::color() const {
    return d->_color;
}

void Datum::setName(const QString& s) {
    d->_name = s;
    if (! d->_changing) {
	emit changed();
    }
}

const QString& Datum::name() const {
    return d->_name;
}

void Datum::setBegin(bool begin) {
    if (!begin) {
        d->_begin = false;
	if (d->_dataType->begin() == this){
	    d->_dataType->setBegin(0);
	}
    }
    else if (d->_dataType->begin() == this) {
        return;
    }
    else if( d->_dataType->setBegin(this) ) {
        d->_begin = true;
    }else{
        return;
    }

    if (! d->_changing) {
	emit changed();
    }
}

void Datum::setEnd(bool end) {
    d->_end = end;

    if (end) {
        d->_dataType->addEnd(this);
    } else {
        d->_dataType->removeEnd(this);
    }
    if (! d->_changing) {
	emit changed();
    }
}

bool Datum::begin() const {
    return d->_begin;
}

bool Datum::end() const {
    return d->_end;
}

const QVariant Datum::value() const {
    return d->_value;
}

void  Datum::setValue(const QVariant s) {
    d->_value = s;
    if (! d->_changing) {
      emit changed();
    }
}

void Datum::setValue(const QString& c){
    d->_value = c;
    if (d->_changing){
      emit changed();
    }
}
void Datum::startChange() {
    d->_changing = true;
}

void Datum::endChange() {
    d->_changing = false;
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
    return d->_scriptvalue;
}

void Datum::setEngine(	QScriptEngine *engine ) {
    d->_engine = engine;
    d->_scriptvalue = engine->newQObject(this);
}

QScriptValue Datum::adj_data() {
    QList<Datum*> list = adjacent_data();
    QScriptValue array = d->_engine->newArray();
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
    QScriptValue array = d->_engine->newArray();
    foreach(Pointer* e, list) {
        array.property("push").call(array, QScriptValueList() << e->scriptValue());
    }
    return array;
}

#endif
