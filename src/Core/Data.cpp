/*
    This file is part of Rocs.
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
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

#include "Data.h"
#include "Pointer.h"
#include "DataStructure.h"
#include <KDebug>
#include <KGlobal>
#include <QColor>
#include <QMap>

#include "DynamicPropertiesList.h"
#include "DataType.h"

class DataPrivate
{
public:
    DataPrivate(DataStructurePtr parent, int uniqueIdentifer, int dataType);

    /**
     * self pointer to Data object
     */
    boost::weak_ptr<Data> q;

    PointerList _in_pointers;
    PointerList _out_pointers;
    PointerList _self_pointers;

    qreal _x;
    qreal _y;
    qreal _width;

    bool _begin;
    bool _end;
    bool _showName;
    bool _showValue;
    bool _visible;
    bool _useColor;

    DataStructurePtr _dataStructure;
    boost::shared_ptr<DataItem> _item;

    int _uniqueIdentifier;
    int _dataType;
    QString _name;
    QColor _color;

    QVariant _value;
    QScriptValue _scriptvalue;
    QScriptEngine *_engine;

    void empty(PointerList &list);
};

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
    , _showName(parent->isDataNameVisible(dataType))
    , _showValue(parent->isDataValueVisible(dataType))
    , _visible(parent->isDataVisible(dataType))
    , _useColor(false)
    , _dataStructure(parent)
    , _uniqueIdentifier(uniqueIdentifer)
    , _dataType(dataType)
    , _color(parent->document()->dataType(dataType)->defaultColor())
    , _value(0)
{
    _in_pointers = PointerList();
    _out_pointers = PointerList();
    _self_pointers = PointerList();
}

DataPtr Data::create(DataStructurePtr parent, int uniqueIdentifier, int dataType)
{
    return create<Data>(parent, uniqueIdentifier, dataType);
}

void Data::setQpointer(DataPtr q)
{
    d->q = q;
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

DataStructurePtr Data::dataStructure() const
{
    return d->_dataStructure;
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


void Data::setDataItem(boost::shared_ptr<DataItem> item)
{
    d->_item = item;
}


void Data::setDataType(int dataType)
{
    Q_ASSERT(d->_dataStructure->document()->dataTypeList().contains(dataType));
    d->_dataType = dataType;
    d->_dataStructure->updateData(getData());
    emit dataTypeChanged(dataType);
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

const QVariant Data::value() const
{
    return d->_value;
}

const QString& Data::name()  const
{
    return d->_name;
}

const QVariant  Data::color() const
{
    return d->_color;
}

boost::shared_ptr<DataItem> Data::item() const
{
    return d->_item;
}

const QString& Data::iconPackage() const
{
    return d->_dataStructure->document()->iconPackage();
}

qreal Data::x() const
{
    return d->_x;
}

qreal Data::y() const
{
    return d->_y;
}

qreal Data::width() const
{
    return d->_width;
}

QString Data::icon() const
{
    return d->_dataStructure->document()->dataType(d->_dataType)->iconName();
}

PointerList& Data::in_pointers()   const
{
    return d->_in_pointers;
}

PointerList& Data::out_pointers()  const
{
    return d->_out_pointers;
}

PointerList& Data::self_pointers() const
{
    return d->_self_pointers;
}

bool Data::showName() const
{
    return d->_showName;
}

bool Data::showValue() const
{
    return d->_showValue;
}

bool Data::useColor() const
{
    return d->_useColor;
}

int Data::identifier() const
{
    return d->_uniqueIdentifier;
}

int Data::dataType() const
{
    return d->_dataType;
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

QScriptValue Data::set_type(int dataType)
{
    if (!d->_dataStructure->document()->dataTypeList().contains(dataType)) {
        kDebug() << "data type does not exist."; //TODO give script error
        return d->_dataStructure->engine()->newVariant(false);
    }
    setDataType(dataType);
    return d->_dataStructure->engine()->newVariant(true);
}

QScriptValue Data::type()
{
    return d->_dataStructure->engine()->newVariant(d->_dataType);
}

void Data::add_property(QString name, QString value)
{
    addDynamicProperty(name, value);
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
