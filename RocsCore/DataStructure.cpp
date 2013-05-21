/*
    This file is part of Rocs.
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"
#include "DataType.h"
#include "PointerType.h"
#include "QtScriptBackend.h"
#include "Group.h"
#include "Document.h"
#include "DocumentManager.h"
#include "ConcurrentHelpClasses.h"

#include <boost/weak_ptr.hpp>

#include <QColor>
#include <KDebug>

class DataStructurePrivate
{
public:
    DataStructurePrivate() {}

    /**
     * self pointer to DataStructure
     */
    boost::weak_ptr<DataStructure> q;

    int _identifierCount;   // represents the next identifier that will be assigend to data/pointer

    QMap<int, DataList> _dataTypeLists;         // list if data elements associated to specific type
    QMap<int, PointerList> _pointerTypeLists;   // list of pointers associated to specific type
    QHash<int, DataPtr> _dataIdentifierMap;

    QList<GroupPtr> _groups;

    QPointF _relativeCenter;
    QString _name;
    Document *_document;
    bool _readOnly;

    QScriptValue _value;
    QScriptEngine *_engine;
};


DataStructurePtr DataStructure::create(Document *parent)
{
    return create<DataStructure>(parent);
}

DataStructurePtr DataStructure::create(DataStructurePtr other, Document *parent)
{
    return create<DataStructure>(other, parent);
}

void DataStructure::setQpointer(DataStructurePtr q)
{
    d->q = q;
}

void DataStructure::initialize()
{
    d->_readOnly = false;
    d->_identifierCount = 1;

    // create type lists
    foreach (int identifier, d->_document->dataTypeList()) {
        registerDataType(identifier);
    }
    foreach (int identifier, d->_document->pointerTypeList()) {
        registerPointerType(identifier);
    }
    connect(document(), SIGNAL(dataTypeCreated(int)), this, SLOT(registerDataType(int)));
    connect(document(), SIGNAL(pointerTypeCreated(int)), this, SLOT(registerPointerType(int)));
    connect(document(), SIGNAL(dataTypeRemoved(int)), this, SLOT(removeDataType(int)));
    connect(document(), SIGNAL(pointerTypeRemoved(int)), this, SLOT(removePointerType(int)));

    emit changed();
}

DataStructurePtr DataStructure::getDataStructure() const
{
    DataStructurePtr px(d->q);
    return px;
}

DataStructure::DataStructure(Document *parent)
    : d(new DataStructurePrivate)
{
    d->_document = parent;
    // further initialization is done by separate call to initialize()
}

void DataStructure::importStructure(DataStructurePtr other)
{
    d->_readOnly = other->readOnly();
    // FIXME set dynamic properties

    QHash <Data*, DataPtr> dataTodata;
    //FIXME only reading default of elements
    foreach(DataPtr n, other->dataList(0)) {
        DataPtr newdata = createData(n->property("name").toString(), 0);
        newdata->setColor(n->color());
        //FIXME all dynamic properties must be set
        newdata->setX(n->x());
        newdata->setY(n->y());
        newdata->setWidth(n->width());
        dataTodata.insert(n.get(), newdata);
    }
    //FIXME only import of default elements
    foreach(PointerPtr e, other->pointers(0)) {
        DataPtr from =  dataTodata.value(e->from().get());
        DataPtr to =  dataTodata.value(e->to().get());

        PointerPtr newPointer = createPointer(from, to, 0);
        newPointer->setColor(e->color());
        //FIXME all dynamic properties must be set
    }
}


DataStructure::~DataStructure()
{
}


const DataList DataStructure::dataList(int dataType) const
{
    if (d->_dataTypeLists.contains(dataType)) {
        return d->_dataTypeLists[dataType];
    }
    kWarning() << "returning empty data list: data type not registered";
    return DataList();
}


DataList DataStructure::dataListAll() const
{
    DataList allData;
    foreach(int type, document()->dataTypeList()) {
        allData.append(dataList(type));
    }
    return allData;
}


const PointerList DataStructure::pointers(int pointerType) const
{
    if (d->_pointerTypeLists.contains(pointerType)) {
        return d->_pointerTypeLists[pointerType];
    }
    kWarning() << "returning empty pointer list: pointer type not registered";
    return PointerList();
}


PointerList DataStructure::pointerListAll() const
{
    PointerList allPointers;
    foreach(int type, document()->pointerTypeList()) {
        allPointers.append(pointers(type));
    }
    return allPointers;
}


void DataStructure::registerDataType(int identifier)
{
    if (d->_dataTypeLists.contains(identifier)) {
        kWarning() << "DataType already registered: aborting";
        return;
    }
    if (!d->_document->dataType(identifier)) {
        kError() << "DataType not registered at document: aborting";
        return;
    }
    d->_dataTypeLists.insert(identifier, DataList());
}


void DataStructure::registerPointerType(int identifier)
{
    if (d->_pointerTypeLists.contains(identifier)) {
        kWarning() << "PointerType already registered: aborting";
        return;
    }
    if (!d->_document->pointerType(identifier)) {
        kError() << "PointerType not registered at document: aborting";
        return;
    }
    d->_pointerTypeLists.insert(identifier, PointerList());
}


void DataStructure::removeDataType(int identifier)
{
    if (identifier == 0) {
        kWarning() << "Could not remove non-existing DataType";
        return;
    }

    foreach(DataPtr data, d->_dataTypeLists[identifier]) {
        data->remove();
    }
    d->_dataTypeLists[identifier].clear();
    d->_dataTypeLists.remove(identifier);
}


void DataStructure::removePointerType(int pointerType)
{
    if (pointerType == 0 || !d->_pointerTypeLists.contains(pointerType)) {
        kWarning() << "Could not remove non-existing PointerType";
        return;
    }

    foreach(PointerPtr pointer, d->_pointerTypeLists[pointerType]) {
        pointer->remove();
    }
    d->_pointerTypeLists[pointerType].clear();
    d->_pointerTypeLists.remove(pointerType);
}


void DataStructure::updateData(DataPtr data)
{
    foreach(int dataType, d->_document->dataTypeList()) {
        d->_dataTypeLists[dataType].removeAll(data);
    }
    d->_dataTypeLists[data->dataType()].append(data);
}


void DataStructure::updatePointer(PointerPtr pointer)
{
    foreach(int pointerType, d->_document->pointerTypeList()) {
        d->_pointerTypeLists[pointerType].removeAll(pointer);
    }
    d->_pointerTypeLists[pointer->pointerType()].append(pointer);
}


void DataStructure::setReadOnly(bool r)
{
    d->_readOnly = r;
    d->_document->setModified();
}

void DataStructure::remove()
{
    disconnect();
    //remove pointers
    foreach(const PointerList &pointerType, d->_pointerTypeLists) {
        foreach(const PointerPtr &pointer,  pointerType) {
            pointer->remove();
        }
    }
    d->_pointerTypeLists.clear();


    //remove data elements
    foreach(const DataList &dataType, d->_dataTypeLists) {
        foreach(const DataPtr &data, dataType) {
            data->remove();
        }
    }
    d->_dataTypeLists.clear();
    d->_dataIdentifierMap.clear();

    d->_document->remove(getDataStructure());
}

int DataStructure::generateUniqueIdentifier()
{
    return d->_identifierCount++;
}

DataPtr DataStructure::createData(const QString& name, int dataType)
{
    if (d->_readOnly) {
        return DataPtr();
    }
    DataPtr n = Data::create(this->getDataStructure(), generateUniqueIdentifier(), dataType);
    n->setProperty("name", name);

    return addData(n);
}

DataPtr DataStructure::addData(DataPtr data)
{
    Q_ASSERT(data->dataType() >= 0 && data->dataType() <= d->_dataTypeLists.size());

    // set data type properties
    d->_dataTypeLists[data->dataType()].append(data);
    DataTypePtr type = document()->dataType(data->dataType());
    foreach(const QString &property, type->properties()) {
        if (!data->property(property.toLatin1()).isValid()
            || data->property(property.toLatin1()).isNull())
        {
            data->addDynamicProperty(property, type->propertyDefaultValue(property));
        }
    }

    // register data at fast access identifier hash
    d->_dataIdentifierMap.insert(data->identifier(), data);

    emit dataCreated(data);
    emit changed();

    connect(data.get(), SIGNAL(propertyChanged(QString)),     this, SIGNAL(changed()));
    connect(data.get(), SIGNAL(colorChanged(QColor)),         this, SIGNAL(changed()));
    connect(data.get(), SIGNAL(posChanged(QPointF)),          this, SIGNAL(dataPositionChanged(QPointF)));
    connect(data.get(), SIGNAL(useColorChanged(bool)),        this, SIGNAL(changed()));
    return data;
}

DataPtr DataStructure::createData(const QString& name, const QPointF& pos, int dataType)
{
    if (DataPtr data = createData(name, dataType)) {
        data->setPos(pos.x(), pos.y());
        return data;
    }
    return DataPtr();
}

DataList DataStructure::addDataList(DataList dataList)
{
    foreach(DataPtr n, dataList) {
        addData(n);
    }

    return dataList;
}


DataList DataStructure::addDataList(QList< QPair<QString, QPointF> > dataList, int dataType)
{
    QList< DataPtr > dataCreateList;
    QList< QPair<QString, QPointF> >::const_iterator dataDefinition = dataList.constBegin();
    while (dataDefinition != dataList.constEnd()) {
        if (DataPtr data = createData(dataDefinition->first, dataType)) {
            data->setPos(dataDefinition->second.x(), dataDefinition->second.y());
            dataCreateList << data;
        }
        ++dataDefinition;
    }
    return dataCreateList;
}

PointerPtr DataStructure::addPointer(PointerPtr pointer)
{
    Q_ASSERT(d->_pointerTypeLists.contains(pointer->pointerType()));

    // set pointer type properties
    d->_pointerTypeLists[pointer->pointerType()].append(pointer);
    PointerTypePtr type = document()->pointerType(pointer->pointerType());
    foreach(const QString &property, type->properties()) {
        if (!pointer->property(property.toLatin1()).isValid()
            || pointer->property(property.toLatin1()).isNull())
        {
            pointer->addDynamicProperty(property, type->propertyDefaultValue(property));
        }
    }

    emit pointerCreated(pointer);
    emit changed();
    connect(pointer.get(), SIGNAL(changed()), this, SIGNAL(changed()));
    return pointer;
}

PointerPtr DataStructure::createPointer(DataPtr from, DataPtr to, int pointerType)
{
    Q_ASSERT(d->_document->pointerTypeList().contains(pointerType));

    if (d->_readOnly) {                  // If the data structure is in read only mode, no new stuff should be added.
        return PointerPtr();
    }
    if (!from || !to) {                 // If one of the two required data elements is 0, then do not add a pointer.
        return PointerPtr();
    }

    if (from->dataStructure() != to->dataStructure()) { // the user is trying to connect data elements from different graphs.
        return PointerPtr();
    }
    PointerPtr pointer = Pointer::create(getDataStructure(), from, to, pointerType);
    return addPointer(pointer);
}

DataPtr DataStructure::getData(int uniqueIdentifier)
{
    if (d->_dataIdentifierMap.contains(uniqueIdentifier)) {
        return d->_dataIdentifierMap[uniqueIdentifier];
    }

    foreach(const DataList &dataType, d->_dataTypeLists) {
        foreach(DataPtr data, dataType) {
            if (data->identifier() == uniqueIdentifier) {
                kWarning() << "Access do data element that is not registered at data-identifier mapper.";
                return data;
            }
        }
    }
    return DataPtr();
}

void DataStructure::remove(DataPtr data)
{
    if (!d->_dataTypeLists[data->dataType()].contains(data)) {
        kWarning() << "Data element not registered, aborting removal.";
        return;
    }

    // remove from internal lists
    if (d->_dataIdentifierMap.remove(data->identifier()) != 1) {
        kWarning() << "Data identifier hash is dirty.";
    }
    if (d->_dataTypeLists[data->dataType()].removeOne(data)) {
        // only remove data element if it is registered
        emit dataPositionChanged(QPointF(data->x(), data->y()));
        data->remove();
    }
    emit changed();
}

void DataStructure::remove(PointerPtr pointer)
{
    if (!d->_pointerTypeLists[pointer->pointerType()].contains(pointer)) {
        kWarning() << "Pointer not registered, aborting removal.";
        return;
    }
    // remove from internal list
    if (d->_pointerTypeLists[pointer->pointerType()].removeOne(pointer)) {
        // only remove pointer if it is registered
        pointer->remove();
    }
    emit changed();
}

void DataStructure::remove(GroupPtr group)
{
    if (d->_groups.removeOne(group)) {
        group->remove();
    }
}

GroupPtr DataStructure::addGroup(const QString& name)
{
    GroupPtr group = Group::create(getDataStructure(), generateUniqueIdentifier(), document()->groupType());
    group->setName(name);
    return group;
}

void DataStructure::setName(const QString& s)
{
    d->_name = s;
    emit nameChanged(d->_name);
}

void DataStructure::addDynamicProperty(const QString& property, const QVariant& value)
{
    if (!Document::isValidIdentifier(property)) {
        kWarning() << "Property identifier \"" << property << "\" is not valid: aborting";
        return;
    }
    setProperty(property.toAscii(), value);
}

void DataStructure::removeDynamicProperty(const QString& property)
{
    setProperty(property.toAscii(), QVariant::Invalid);
}

void DataStructure::renameDynamicProperty(const QString& oldName, const QString& newName)
{
    if (!Document::isValidIdentifier(newName)) {
        kWarning() << "Property identifier \"" << newName << "\" is not valid: aborting";
        return;
    }
    setProperty(newName.toLatin1(), property(oldName.toLatin1()));
    setProperty(oldName.toLatin1(), QVariant::Invalid);
}

void DataStructure::add_property(const QString& name, const QVariant& value)
{
    addDynamicProperty(name, value);
}

void DataStructure::remove_property (const QString& name)
{
    removeDynamicProperty(name);
}

void DataStructure::setEngine(QScriptEngine *engine)
{
    d-> _engine = engine;
    d->_value = d->_engine->newQObject(this);

    if (! d->_name.isEmpty()) {
        d->_engine->globalObject().setProperty(d->_name, d->_value);
    }

    foreach(const DataList& dataType, d->_dataTypeLists) {
        for (int i = 0; i < dataType.size(); ++i) {
            dataType.at(i)->setEngine(engine);
        }
    }

    foreach(const PointerList& pointerType, d->_pointerTypeLists) {
        for (int i = 0; i < pointerType.size(); ++i) {
            pointerType.at(i)->setEngine(engine);
        }
    }

    //FIXME groups are not available in engine
//     foreach(Group * g, d->_groups) {
//         QScriptValue array = d->_engine->newArray();
//         //   foreach(Data * n, (*g) ) {
//         //       array.property("push").call(array, QScriptValueList() << n->scriptValue());
//         //   }
//         d->_engine->globalObject().setProperty(g->name(), array);
//     }
}

QMap<QString, QString> DataStructure::pluginProperties() const
{
    return QMap<QString,QString>();
}

bool DataStructure::readOnly() const
{
    return d->_readOnly;
}

QString DataStructure::name() const
{
    return d->_name;
}

QScriptValue DataStructure::scriptValue() const
{
    return d->_value;
}
QScriptEngine *DataStructure::engine() const
{
    return d->_engine;
}

Document *DataStructure::document() const
{
    return d->_document;
}

const QList<GroupPtr> DataStructure::groups() const
{
    return d->_groups;
}

void DataStructure::cleanUpBeforeConvert()
{

}

void DataStructure::setPluginProperty(const QString&, const QString&)
{

}
