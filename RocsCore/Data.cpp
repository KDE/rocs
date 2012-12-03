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
#include "QtScriptBackend.h"
#include <KDebug>
#include <KGlobal>
#include <QColor>
#include <QMap>
#include <KDebug>

#include <boost/weak_ptr.hpp>

#include "DataType.h"

class DataPrivate
{
public:
    DataPrivate(DataStructurePtr parent, int uniqueIdentifier, int dataType);

    /**
     * self pointer to Data object
     */
    boost::weak_ptr<Data> q;

    PointerList _inPointers;
    PointerList _outPointers;

    qreal _x;
    qreal _y;
    qreal _width;
    bool _visible;

    DataStructurePtr _dataStructure;
    int _uniqueIdentifier;
    DataTypePtr _dataType;
    QColor _color;

    QScriptValue _scriptvalue;
    QScriptEngine *_engine;

    /**
     * Create a script value array out of a pointer list.
     *
     * \param list is a list of pointers
     * \return array of script value representation of the pointers
     */
    QScriptValue createScriptArray(PointerList list);
};

DataPrivate::DataPrivate(DataStructurePtr parent, int uniqueIdentifier, int dataType)
    : _x(0)
    , _y(0)
    , _width(0.3)
    , _visible(parent->isDataVisible(dataType))
    , _dataStructure(parent)
    , _uniqueIdentifier(uniqueIdentifier)
    , _dataType(_dataStructure->document()->dataType(dataType))
    , _color(parent->document()->dataType(dataType)->defaultColor())
{

    _inPointers = PointerList();
    _outPointers = PointerList();
}

QScriptValue DataPrivate::createScriptArray(PointerList list)
{
    QScriptValue array = _engine->newArray();
    foreach(PointerPtr e, list) {
        array.property("push").call(array, QScriptValueList() << e->scriptValue());
    }
    return array;
}

DataPtr Data::create(DataStructurePtr dataStructure, int uniqueIdentifier, int dataType)
{
    return create<Data>(dataStructure, uniqueIdentifier, dataType);
}

void Data::setQpointer(DataPtr q)
{
    d->q = q;
}

void Data::initialize()
{
    installEventFilter(this);
    foreach(const QString& property, d->_dataType->properties()) {
        addDynamicProperty(property, d->_dataType->propertyDefaultValue(property));
    }

    connect(d->_dataType.get(), SIGNAL(propertyAdded(QString,QVariant)),
            this, SLOT(addDynamicProperty(QString,QVariant)));
    connect(d->_dataType.get(), SIGNAL(propertyRemoved(QString)),
            this, SLOT(removeDynamicProperty(QString)));
    connect(d->_dataType.get(), SIGNAL(propertyRenamed(QString,QString)),
            this, SLOT(renameDynamicProperty(QString,QString)));
    connect(d->_dataType.get(), SIGNAL(propertyDefaultValueChanged(QString)),
            this, SLOT(updateDynamicProperty(QString)));
    connect(d->_dataType.get(), SIGNAL(propertyVisibilityChanged(QString)),
            this, SLOT(updateDynamicProperty(QString)));
    connect(d->_dataType.get(), SIGNAL(removed()), this, SLOT(remove()));
}

DataPtr Data::getData() const
{
    DataPtr px(d->q);
    return px;
}

// Do NOT set a parent, since shared pointer will take care for deletion!
Data::Data(DataStructurePtr dataStructure, int uniqueIdentifier, int dataType)
    :  QObject(0),
       d(new DataPrivate(dataStructure, uniqueIdentifier, dataType))
{
}

Data::~Data()
{
}

bool Data::eventFilter(QObject *obj, QEvent *event){
    if (event->type() == QEvent::DynamicPropertyChange) {
        if (QDynamicPropertyChangeEvent* const dynEvent = dynamic_cast<QDynamicPropertyChangeEvent*>(event)) {
            event->accept();
            emit(propertyChanged(dynEvent->propertyName()));
        }
    }
    return QObject::eventFilter(obj, event);
}

DataStructurePtr Data::dataStructure() const
{
    return d->_dataStructure;
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

void Data::setDataType(int dataType)
{
    Q_ASSERT(d->_dataStructure->document()->dataTypeList().contains(dataType));

    // disconnect from old data type
    if (d->_dataType) {
        d->_dataType.get()->disconnect(this);
    }

    // make changes
    d->_dataType = d->_dataStructure->document()->dataType(dataType);
    d->_dataStructure->updateData(getData());
    foreach(const QString& property, d->_dataType->properties()) {
        if (this->property(property.toStdString().c_str()) == QVariant::Invalid) {
            addDynamicProperty(property, d->_dataType->propertyDefaultValue(property));
        }
    }
    emit dataTypeChanged(dataType);

    // connect to new type
    connect(d->_dataType.get(), SIGNAL(propertyAdded(QString,QVariant)),
            this, SLOT(addDynamicProperty(QString,QVariant)));
    connect(d->_dataType.get(), SIGNAL(propertyDefaultValueChanged(QString)),
            this, SLOT(updateDynamicProperty(QString)));
    connect(d->_dataType.get(), SIGNAL(propertyVisibilityChanged(QString)),
            this, SLOT(updateDynamicProperty(QString)));
    connect(d->_dataType.get(), SIGNAL(propertyRenamed(QString,QString)),
            this, SLOT(renameDynamicProperty(QString,QString)));
    connect(d->_dataType.get(), SIGNAL(removed()), this, SLOT(remove()));
}

DataList Data::adjacentDataList() const
{
    // use QMap as the DataPtr elements are not hashable
    // and we can speed up process by using the uniqe IDs
    QMap<int, DataPtr> adjacent;
    foreach(PointerPtr e, d->_outPointers) {
        if (e->to() != e->from() && e->to() == getData()) {
            continue;
        }
        adjacent[e->to()->identifier()] = e->to();
    }

    foreach(PointerPtr e, d->_inPointers) {
        if (e->to() != e->from() && e->from() == getData()) {
            continue;
        }
        adjacent[e->from()->identifier()] = e->from();
    }
    return adjacent.values();
}

PointerList Data::pointerList() const
{
    PointerList adjacent;
    foreach (const PointerPtr& p, d->_outPointers) {
        adjacent.append(p);
    }
    foreach (const PointerPtr& p, d->_inPointers) {
        adjacent.append(p);
    }

    return adjacent;
}

void Data::registerInPointer(PointerPtr e)
{
    Q_ASSERT(e->to()->identifier() == identifier() || e->from()->identifier() == identifier());
    d->_inPointers.append(e);
    connect(e.get(), SIGNAL(directionChanged(PointerType::Direction)), this, SLOT(updatePointerList()));
    emit pointerListChanged();
}

void Data::registerOutPointer(PointerPtr e)
{
    Q_ASSERT(e->to()->identifier() == identifier() || e->from()->identifier() == identifier());
    d->_outPointers.append(e);
    connect(e.get(), SIGNAL(directionChanged(PointerType::Direction)), this, SLOT(updatePointerList()));
    emit pointerListChanged();
}

PointerPtr Data::addPointer(DataPtr to)
{
    return d->_dataStructure->addPointer(this->getData(), to);
}

void Data::remove(PointerPtr e)
{
    // removes pointer from any list that could contain it
    if (d->_inPointers.removeOne(e)) {
        emit pointerListChanged();
        d->_dataStructure->remove(e);
        // not necessary to call e->remove() here, since data structure already calls that
    }
    if (d->_outPointers.removeOne(e)) {
        emit pointerListChanged();
        d->_dataStructure->remove(e);
        // not necessary to call e->remove() here, since data structure already calls that
    }
}

void Data::updatePointerList()
{
    bool changed = false;
    PointerList tmpList = pointerList();
    foreach(PointerPtr e, tmpList) {
        if (e->direction() == PointerType::Bidirectional) {
            if (!d->_inPointers.contains(e)) {
                d->_inPointers.append(e);
                changed = true;
            }
            if (!d->_outPointers.contains(e)) {
                d->_outPointers.append(e);
                changed = true;
            }
        }
        if (e->direction() == PointerType::Unidirectional) {
            if (getData() == e->from() && d->_inPointers.contains(e)) {
                d->_inPointers.removeOne(e);
                changed = true;
            }
            if (getData() == e->to() && d->_outPointers.contains(e)) {
                d->_outPointers.removeOne(e);
                changed = true;
            }
        }
    }
    if (changed) {
        emit pointerListChanged();
    }
}

PointerList Data::pointerList(DataPtr to) const
{
    PointerList list;
    foreach(PointerPtr tmp, d->_outPointers) {
        if (tmp->to() == to) {
            list.append(tmp);
        }
    }
    return list;
}

void Data::remove()
{
    emit removed();
    // disconnect from DataType signals
    disconnect(d->_dataType.get(), 0, this, 0);

    while (!d->_inPointers.isEmpty()) {
        remove(d->_inPointers.first());
    }
    while (!d->_outPointers.isEmpty()) {
        remove(d->_outPointers.first());
    }

    d->_dataStructure->remove(getData());
}


void Data::self_remove()
{
    kWarning() << "self_remove() is a deprecated function, please use remove()";
    remove();
}

QVariant Data::color() const
{
    return d->_color;
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
    return d->_dataType->iconName();
}

QList< QString > Data::properties() const
{
    return d->_dataType->properties();
}

PointerList& Data::inPointerList() const
{
    return d->_inPointers;
}

PointerList& Data::outPointerList() const
{
    return d->_outPointers;
}

int Data::identifier() const
{
    return d->_uniqueIdentifier;
}

int Data::dataType() const
{
    return d->_dataType->identifier();
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

void Data::addDynamicProperty(const QString& property, const QVariant& value)
{
    if (!Document::isValidIdentifier(property)) {
        kWarning() << "Property identifier \"" << property << "\" is not valid: aborting";
        return;
    }
    setProperty(property.toAscii(), value);
    emit propertyAdded(property);
}

void Data::removeDynamicProperty(const QString& property)
{
    // setting property to invalid is equals to deleting it
    setProperty(property.toUtf8(), QVariant::Invalid);
    emit propertyRemoved(property);
}

void Data::updateDynamicProperty(const QString& property)
{
    if (this->property(property.toStdString().c_str()) == QVariant::Invalid
        || this->property(property.toStdString().c_str()).toString().isEmpty()
    ) {
        this->setProperty(property.toStdString().c_str(), d->_dataType->propertyDefaultValue(property));
    }
    emit propertyChanged(property);
}

void Data::renameDynamicProperty(const QString& oldName, const QString& newName)
{
    if (!Document::isValidIdentifier(newName)) {
        kWarning() << "Property identifier \"" << newName << "\" is not valid: aborting";
        return;
    }
    setProperty(newName.toStdString().c_str(), property(oldName.toStdString().c_str()));
    setProperty(oldName.toStdString().c_str(), QVariant::Invalid);
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
        dataStructure()->document()->engineBackend()->debug(
            i18n("Could not set data type for node %1: data type does not exist.", identifier()));
        return d->_dataStructure->engine()->newVariant(false);
    }
    setDataType(dataType);
    return d->_dataStructure->engine()->newVariant(true);
}

QScriptValue Data::type()
{
    return d->_dataStructure->engine()->newVariant(d->_dataType->identifier());
}

void Data::add_property(const QString& name, const QString& value)
{
    addDynamicProperty(name, value);
}

void Data::remove_property (const QString& name)
{
    removeDynamicProperty(name);
}


QScriptValue Data::adj_data()
{
    QList< DataPtr > list = adjacentDataList();
    QScriptValue array = d->_engine->newArray();
    foreach(DataPtr n, list) {
        array.property("push").call(array, QScriptValueList() << n->scriptValue());
    }
    return array;
}

QScriptValue Data::adj_pointers()
{
    PointerList list = pointerList();
    return d->createScriptArray(list);
}

QScriptValue Data::adj_pointers(int pointerType)
{
    PointerList list;
    foreach(PointerPtr n, pointerList()) {
        if (n->pointerType() != pointerType) {
            continue;
        }
        list.append(n);
    }
    return d->createScriptArray(list);
}

QScriptValue Data::input_pointers()
{
    PointerList list = inPointerList();
    return d->createScriptArray(list);
}

QScriptValue Data::output_pointers()
{
    PointerList list = outPointerList();
    return d->createScriptArray(list);
}

QScriptValue Data::output_pointers(int pointerType)
{
    PointerList list;
    foreach(PointerPtr n, outPointerList()) {
        if (n->pointerType() != pointerType) {
            continue;
        }
        list.append(n);
    }
    return d->createScriptArray(list);
}

QScriptValue Data::connected_pointers(DataPtr n)
{
    PointerList list = pointerList(n);
    return d->createScriptArray(list);
}
