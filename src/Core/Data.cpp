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
#include "DataStructure.h"
#include <KDebug>
#include <KGlobal>
#include <kstandarddirs.h>
#include <QColor>

#include "DynamicPropertiesList.h"

void DataPrivate::empty(PointerList &list) {
    for(int i = 0; i < list.size(); ++i){
        list.at(i)->remove(q);
    }
}

DataStructure *Data::dataStructure() const{ return d->_dataStructure; }

Data::Data(DataStructure *parent) 
: QObject(parent)
, d(new DataPrivate(this)) {
    d->_dataStructure = parent;
    d->_x = 0;
    d->_y = 0;
    d->_width = 0.5;
    d->_showName = true;
    d->_showValue = true;
    d->_begin = false;
    d->_end = false;
    d->_color = d->_dataStructure->dataDefaultColor();
    d->_changing = false;
    d->_value = 0;
    d->_icon = "rocs_default";
    d->_iconpackage = KGlobal::dirs()->locate("appdata", "iconpacks/default.svg");
    kDebug() << "Node successfully created" << d->_iconpackage;
}

Data::~Data() {

    d->empty(d->_in_pointers);
    d->empty(d->_out_pointers);
    d->empty(d->_self_pointers);
    emit removed();
    delete d;
}

bool Data::showName() {
    return d->_showName;
}

bool Data::showValue() {
    return d->_showValue;
}

void Data::setShowName(bool b) {
    d->_showName = b;
    emit changed();
}

void Data::setShowValue(bool b) {
    d->_showValue = b;
    emit changed();
}

void Data::setIcon(const QString& s){
    d->_icon = s;
    emit changed();
}

void Data::setIconPackage(const QString& s){
   d-> _iconpackage = s;
}

DataList Data::adjacent_data() const
{
    QList<Data*> adjacent;

   foreach(Pointer *e, d->_out_pointers) {
        adjacent.append( e->to()  );
   }

   foreach(Pointer *e, d->_self_pointers) {
        adjacent.append( e->to() );
   }

   foreach(Pointer *e, d->_in_pointers) {
        adjacent.append( e->from() );
   }

   return adjacent;
}

PointerList Data::adjacent_pointers() const
{
    PointerList adjacent;

    adjacent << d->_out_pointers;
    adjacent << d-> _self_pointers;
    adjacent << d->_in_pointers;

    return adjacent;
}

void Data::addInPointer(Pointer *e) {
    d-> _in_pointers.append( e );
}

void Data::addOutPointer(Pointer *e) {
    d->_out_pointers.append( e  );
}

void Data::addSelfPointer(Pointer *e) {
    d->_self_pointers.append( e );
}

Pointer* Data::addPointer(Data* to) {
    return d->_dataStructure->addPointer(this, to);
}

void Data::removePointer(Pointer *e, int pointerList) {
    switch (pointerList) {
    case In  : removePointer(e, d->_in_pointers);    break;
    case Out : removePointer(e, d->_out_pointers);   break;
    case Self: removePointer(e, d->_self_pointers);  break;
    }
}

void Data::removePointer(Pointer* e, PointerList &list) {
    if (list.contains(e))
      list.removeOne(e);
}

PointerList Data::pointers(Data *n) const {
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

void Data::remove() {
  d->_dataStructure->remove(this);
}

void Data::setX(int x) {
    d->_x = x;
    if (! d->_changing) {
    emit changed();
    }
}

void Data::setY(int y) {
    d->_y  = y;
    if (! d->_changing) {
    emit changed();
    }
}

void Data::setWidth(double w) {
    d->_width = (qreal)w;
    if (! d->_changing) {
        emit changed();
    }
}

void Data::setPos(qreal x, qreal y) {
    d->_x = x;
    d->_y = y;
    if (! d->_changing) {
    emit changed();
    }
}

void Data::setColor(const QColor& s) {
    d->_color = s;
    if (! d->_changing) {
    emit changed();
    }
}

void Data::setName(const QString& s) {
    d->_name = s;
    if (! d->_changing) {
    emit changed();
    }
}

void  Data::setValue(const QVariant& s) {
    d->_value = s;
    if (! d->_changing) {
      emit changed();
    }
}

void Data::startChange() {
    d->_changing = true;
}

void Data::endChange() {
    d->_changing = false;
    emit changed();
}

void Data::addDynamicProperty(QString property, QVariant value){
    if (! setProperty(property.toUtf8(), value)  && value.isValid()){
      DynamicPropertiesList::New()->addProperty(this, property);
    }
}

void Data::removeDynamicProperty(QString property){
    addDynamicProperty(property.toUtf8(), QVariant::Invalid);
    DynamicPropertiesList::New()->removeProperty(this, property);
}

void Data::self_remove() {
    remove();
}

QScriptValue Data::scriptValue() const {
    return d->_scriptvalue;
}

void Data::setEngine(    QScriptEngine *engine ) {
    d->_engine = engine;
    d->_scriptvalue = engine->newQObject(this);
}


QScriptValue Data::adj_data() {
    QList<Data*> list = adjacent_data();
    QScriptValue array = d->_engine->newArray();
    foreach(Data* n, list) {
        array.property("push").call(array, QScriptValueList() << n->scriptValue());
    }
    return array;
}

QScriptValue Data::adj_pointers() {
    PointerList list = adjacent_pointers();
    return createScriptArray(list);
}

QScriptValue Data::input_pointers() {
    PointerList list = in_pointers();
    return createScriptArray(list);
}

QScriptValue Data::output_pointers() {
    PointerList list = out_pointers();
    return createScriptArray(list);
}

QScriptValue Data::loop_pointers() {
    PointerList list = self_pointers();
    return createScriptArray(list);
}

QScriptValue Data::connected_pointers(Data *n) {
    PointerList list = pointers(n);
    return createScriptArray(list);
}

QScriptValue Data::createScriptArray(PointerList list) {
    QScriptValue array = d->_engine->newArray();
    foreach(Pointer* e, list) {
        array.property("push").call(array, QScriptValueList() << e->scriptValue());
    }
    return array;
}

