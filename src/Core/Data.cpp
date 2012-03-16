/*
    This file is part of Rocs.
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>

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

#include "Data.h"
#include "Pointer.h"
#include "DataStructure.h"
#include <KDebug>
#include <KGlobal>
#include <kstandarddirs.h>
#include <QColor>
#include <QMap>

#include "DynamicPropertiesList.h"
#include "DataType.h"

void DataPrivate::empty(PointerList &list)
{
    while (!list.isEmpty()) {
        list.first()->remove();
    }
}

DataPrivate::DataPrivate(DataStructurePtr parent, int uniqueIdentifer, int dataType)
    : _x(0)
    , _y(0)
    , _width(0.3)
    , _begin(true)
    , _end(true)
    , _showName(parent->dataType(dataType)->isNameVisible())
    , _showValue(parent->dataType(dataType)->isValueVisible())
    , _visible(parent->dataType(dataType)->isVisible())
    , _useColor(false)
    , _dataStructure(parent)
    , _uniqueIdentifier(uniqueIdentifer)
    , _dataType(dataType)
    , _color(parent->dataType(dataType)->defaultColor())
    , _iconpackage(KGlobal::dirs()->locate("appdata", "iconpacks/default.svg"))
    , _icon("rocs_default")
    , _value(0)
{
    _in_pointers = PointerList();
    _out_pointers = PointerList();
    _self_pointers = PointerList();
}


DataStructurePtr Data::dataStructure() const
{
    return d->_dataStructure;
}

DataPtr Data::create(DataStructurePtr parent, int uniqueIdentifier, int dataType)
{
    DataPtr pi(new Data(parent, uniqueIdentifier, dataType));
    pi->d->q = pi;
    return pi;
}

DataPtr Data::getData() const
{
    DataPtr px(d->q);
    return px;
}

Data::Data(DataStructurePtr parent, int uniqueIdentifier, int dataType)
    :  QObject(parent.get()),
       d(new DataPrivate(parent, uniqueIdentifier, dataType))
{
}

Data::~Data()
{
    emit removed();

    if (d) {
        d->empty(d->_in_pointers);
        d->empty(d->_out_pointers);
        d->empty(d->_self_pointers);
    }
}

bool Data::showName()
{
    return d->_showName;
}

bool Data::showValue()
{
    return d->_showValue;
}

void Data::setShowName(bool b)
{
    if (d->_showName != b) {
        d->_showName = b;
        emit nameVisibilityChanged(b);
    }
}

void Data::setShowValue(bool b)
{
    if (d->_showValue != b) {
        d->_showValue = b;
        emit valueVisibilityChanged(b);
    }
}

void Data::setIcon(const QString& s)
{
    if (d->_icon != s) {
        d->_icon = s;
        emit iconChanged(s);
    }
}


void Data::setUseColor(bool b)
{
    if (d->_useColor != b) {
        d->_useColor = b;
        emit useColorChanged(b);
    }
}

bool Data::isVisible() const
{
    return d->_visible;
}


void Data::setVisible(bool visible)
{
    d->_visible = visible;
    emit visibilityChanged(visible);
}


void Data::setIconPackage(const QString& s)
{
    d-> _iconpackage = s;
}

void Data::setDataItem(boost::shared_ptr<DataItem> item)
{
    d->_item = item;
}

DataList Data::adjacent_data() const
{
    // use QMap as the DataPtr elements are not hashable
    // and we can speed up process by using the uniqe IDs
    QMap<int, DataPtr> adjacent;

    foreach(PointerPtr e, d->_out_pointers) {
        adjacent[e->to()->identifier()] = e->to();
    }

    foreach(PointerPtr e, d->_self_pointers) {
        adjacent[e->to()->identifier()] = e->to();
    }

    foreach(PointerPtr e, d->_in_pointers) {
        adjacent[e->from()->identifier()] = e->from();
    }

    return adjacent.values();
}

PointerList Data::adjacent_pointers() const
{
    PointerList adjacent;

    adjacent << d->_out_pointers;
    adjacent << d->_self_pointers;
    adjacent << d->_in_pointers;

    return adjacent;
}

void Data::addInPointer(PointerPtr e)
{
    d->_in_pointers.append(e);
}

void Data::addOutPointer(PointerPtr e)
{
    d->_out_pointers.append(e);
}

void Data::addSelfPointer(PointerPtr e)
{
    d->_self_pointers.append(e);
}

PointerPtr Data::addPointer(DataPtr to)
{
    return d->_dataStructure->addPointer(this->getData(), to);
}

void Data::removePointer(PointerPtr e, int pointerList)
{
    switch (pointerList) {
    case -1  : removePointer(e, d->_in_pointers);
        removePointer(e, d->_out_pointers);
        removePointer(e, d->_self_pointers);
    case In  : removePointer(e, d->_in_pointers);    break;
    case Out : removePointer(e, d->_out_pointers);   break;
    case Self: removePointer(e, d->_self_pointers);  break;
    }
}

void Data::removePointer(PointerPtr e, PointerList &list)
{
    if (list.contains(e))
        list.removeOne(e);
}

PointerList Data::pointers(DataPtr n) const
{
    PointerList list;
    if (n == getData()) {
        return d->_self_pointers;
    }
    foreach(PointerPtr tmp, d->_out_pointers) {
        if (tmp->to() == n) {
            list.append(tmp);
        }
    }
    foreach(PointerPtr tmp, d->_in_pointers) {
        if (tmp->from() == n) {
            list.append(tmp);
        }
    }
    return list;
}

void Data::remove()
{
    if (d->_dataStructure) {
        d->_dataStructure->remove(getData());
        d->_dataStructure.reset();  // allow datastructure to be destroyed
    }

    d->empty(d->_in_pointers);
    d->empty(d->_out_pointers);
    d->empty(d->_self_pointers);

    emit removed();
}

void Data::setX(int x)
{
    if (d->_x != x) {
        d->_x = x;
        emit posChanged(QPointF(d->_x, d->_y));
    }
}

void Data::setY(int y)
{
    if (d->_y != y) {
        d->_y  = y;
        emit posChanged(QPointF(d->_x, d->_y));
    }
}

void Data::setWidth(double w)
{
    if (d->_width != w) {
        d->_width = (qreal)w;
        emit widthChanged(w);
    }
}

void Data::setPos(qreal x, qreal y)
{
    if (d->_x != x || d->_y != y) {
        d->_x = x;
        d->_y = y;
        emit posChanged(QPointF(d->_x, d->_y));
    }
}

void Data::setColor(const QVariant& s)
{
    QColor c = s.value<QColor>();
    if (d->_color != c) {
        d->_color = c;
        emit colorChanged(c);
    }
}

void Data::setName(const QString& s)
{
    if (d->_name != s) {
        d->_name = s;
        emit nameChanged(s);
    }
}

void  Data::setValue(const QVariant& s)
{
    if (d->_value != s) {
        d->_value = s;
        emit valueChanged(s);
    }
}

void  Data::setValue(const QString& s)
{
    QVariant v(s);
    if (d->_value != v) {
        d->_value = v;
        emit valueChanged(v);
    }
}

void Data::addDynamicProperty(QString property, QVariant value)
{
    if (! setProperty(property.toUtf8(), value)  && value.isValid()) {
        DynamicPropertiesList::New()->addProperty(this, property);
    }
}

void Data::removeDynamicProperty(QString property)
{
    addDynamicProperty(property.toUtf8(), QVariant::Invalid);
    DynamicPropertiesList::New()->removeProperty(this, property);
}

void Data::self_remove()
{
    remove();
}

QScriptValue Data::scriptValue() const
{
    return d->_scriptvalue;
}

void Data::setEngine(QScriptEngine *engine)
{
    d->_engine = engine;
    d->_scriptvalue = engine->newQObject(getData().get());
}


QScriptValue Data::adj_data()
{
    QList< DataPtr > list = adjacent_data();
    QScriptValue array = d->_engine->newArray();
    foreach(DataPtr n, list) {
        array.property("push").call(array, QScriptValueList() << n->scriptValue());
    }
    return array;
}

QScriptValue Data::adj_pointers()
{
    PointerList list = adjacent_pointers();
    return createScriptArray(list);
}

QScriptValue Data::input_pointers()
{
    PointerList list = in_pointers();
    return createScriptArray(list);
}

QScriptValue Data::output_pointers()
{
    PointerList list = out_pointers();
    return createScriptArray(list);
}

QScriptValue Data::loop_pointers()
{
    PointerList list = self_pointers();
    return createScriptArray(list);
}

QScriptValue Data::connected_pointers(DataPtr n)
{
    PointerList list = pointers(n);
    return createScriptArray(list);
}

QScriptValue Data::createScriptArray(PointerList list)
{
    QScriptValue array = d->_engine->newArray();
    foreach(PointerPtr e, list) {
        array.property("push").call(array, QScriptValueList() << e->scriptValue());
    }
    return array;
}
