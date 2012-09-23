/*
    This file is part of Rocs.
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DynamicPropertiesList.h"

#include "Data.h"
#include "Pointer.h"
#include "DataStructure.h"
#include <KDebug>

DynamicPropertiesList * DynamicPropertiesList::self = 0;

//The regular expression to validate the property name
const QString propertyRX("(^([a-z]|[A-Z])+([0-9]|[a-z]|[A-Z]|_)*$)");

DynamicPropertiesList::DynamicPropertiesList(QObject* parent): QObject(parent)
{

}

DynamicPropertiesList::~DynamicPropertiesList()
{
    DynamicPropertiesList::self = 0;
}


DynamicPropertiesList* DynamicPropertiesList::New()
{
    if (DynamicPropertiesList::self == 0) {
        DynamicPropertiesList::self = new DynamicPropertiesList();
    }
    return DynamicPropertiesList::self;
}


void DynamicPropertiesList::addProperty(QObject* obj, const QByteArray& name, const QVariant & value)
{
    QRegExp validator(propertyRX);
    if (validator.indexIn(name) == -1){
            kWarning() << i18n("The name (%1) passed to property is invalid.",QString(name));
            return;
    }
    
    obj->setProperty(name, value);
    
    if (Data * node = qobject_cast< Data* >(obj)) {
        QMap< DataStructure*,  QMultiMap <QString, Data* > >::iterator multimap = _NodesProperties.find(node->dataStructure().get());
        if (multimap == _NodesProperties.end()) { //Not exist a dataStructure yet
            QMultiMap <QString, Data* > newMap;
            multimap = _NodesProperties.insert(node->dataStructure().get(), newMap);
        }

        multimap.value().insert(name, node);

        return;
    }

    if (Pointer * edge = qobject_cast< Pointer* >(obj)) {
        QMap< DataStructure*,  QMultiMap <QString, Pointer* > >::iterator multimap = _EdgesProperties.find(edge->dataStructure().get());
        if (multimap == _EdgesProperties.end()) { //Not exist a dataStructure yet
            QMultiMap <QString, Pointer* > newMap;
            multimap = _EdgesProperties.insert(edge->dataStructure().get(), newMap);
        }
        multimap.value().insert(name, edge);
        return;
    }

    DataStructure * dataStructure = qobject_cast< DataStructure* >(obj);

    if (dataStructure) {
        _GraphProperties.insert(name, dataStructure);
        return;
    }
}


void DynamicPropertiesList::removeProperty(QObject* obj, const QByteArray& name)
{
    Data * node = qobject_cast< Data* >(obj);
    if (node) {
        QMap< DataStructure*,  QMultiMap <QString, Data* > >::iterator multimap = _NodesProperties.find(node->dataStructure().get());
        if (multimap == _NodesProperties.end()) { //Not exist a dataStructure yet
            return;
        }
        multimap.value().remove(name, node);
        node->setProperty(name, QVariant::Invalid);
        return;
    }

    Pointer * edge = qobject_cast< Pointer* >(obj);
    if (edge) {
        QMap< DataStructure*,  QMultiMap <QString, Pointer* > >::iterator multimap = _EdgesProperties.find(edge->dataStructure().get());
        if (multimap == _EdgesProperties.end()) { //Not exist a dataStructure yet
            return;
        }
        multimap.value().remove(name, edge);
        edge->setProperty(name, QVariant::Invalid);
        return;
    }

    DataStructure * dataStructure = qobject_cast< DataStructure* >(obj);
    if (dataStructure) {
        _GraphProperties.remove(name, dataStructure);
        dataStructure->setProperty(name, QVariant::Invalid);
        return;
    }
}

DynamicPropertyType DynamicPropertiesList::type(QObject* obj, const QString& name)
{
    // case obj is Data*
    Data * node = qobject_cast< Data* >(obj);
    if (node) {
        QMap< DataStructure*,  QMultiMap <QString, Data* > >::iterator multimap = _NodesProperties.find(node->dataStructure().get());
        if (multimap == _NodesProperties.end()) { //Not exist a dataStructure yet
            return None;
        }

        QList <Data*> list = multimap.value().values(name);

        //FIXME the following is a hack that eleminates duplicates
        //happens eg with TestDynamicProperties::removeToAllNodes()
        QSet <Data*> tmpSet = list.toSet();
        list = tmpSet.toList();

        for (int i = 0; i < list.size(); i++)
            if (node->dataStructure()->dataList().size() == list.size()) {
                return Global;
            }
        switch (list.size()) {
        case 0  :   return None;
        case 1  :   return Unique;
        default :   return Multiple;
        }
    }

    // case obj is Pointer*
    Pointer * edge = qobject_cast< Pointer* >(obj);
    if (edge) {
        QMap< DataStructure*,  QMultiMap <QString, Pointer* > >::iterator multimap = _EdgesProperties.find(edge->dataStructure().get());
        if (multimap == _EdgesProperties.end()) { //Not exist a dataStructure yet
            return None;
        }
        QList <Pointer*> list = multimap.value().values(name);

        //FIXME the following is a hack that eleminates duplicates
        //happens eg with TestDynamicProperties::removeToAllEdges()
        QSet <Pointer*> tmpSet = list.toSet();
        list = tmpSet.toList();

        if (edge->dataStructure()->pointers().size() == list.size()) {
            return Global;
        }
        switch (list.size()) {
        case 0  :   return None;
        case 1  :   return Unique;
        default :   return Multiple;
        }
    }

    DataStructure * dataStructure = qobject_cast< DataStructure* >(obj);
    if (dataStructure) {
        if (_GraphProperties.values(name).size() == 0) {
            return None;
        } else {
            return Unique;
        }
    }
    return None;
}

const QString DynamicPropertiesList::typeInText(QObject* obj, const QString& name)
{
    switch (type(obj, name)) {
    case Unique:      return i18nc("dynamic property that is present exactly once", "Unique");
    case Multiple:    return i18nc("dynamic property that is present multiple times", "Multiple");
    case Global:      return i18nc("dynamic property that is present at all elements", "Global");
    case None:
    default:          return i18nc("type of dynamic property", "None");
    }
}



void DynamicPropertiesList::clear(DataStructure* dataStructure)
{
    if (dataStructure != 0) {
        _EdgesProperties.values(dataStructure).clear();
        foreach(const QString & name,  _GraphProperties.keys(dataStructure)) {
            _GraphProperties.remove(name, dataStructure);
        }

        _NodesProperties.values(dataStructure).clear();
    } else {
        _EdgesProperties.clear();
        _GraphProperties.clear();
        _NodesProperties.clear();
    }
}


void DynamicPropertiesList::changePropertyName(const QByteArray& name, const QByteArray & newName, QObject* object)
{
    QRegExp validator(propertyRX);
    if (validator.indexIn(newName) == -1){
        kWarning() << i18n("The new name (%1) passed to property is invalid.",QString(newName));
        return;
    }
    
    
    Data * node = qobject_cast< Data* >(object);
    if (node) {
        QMap< DataStructure*,  QMultiMap <QString, Data* > >::iterator multimap = _NodesProperties.find(node->dataStructure().get());
        if (multimap == _NodesProperties.end()) { //Not exist a dataStructure yet
            return;
        }
        foreach(node, multimap.value().values(name)) {
            node->addDynamicProperty(newName, node->property(name));
            node->removeDynamicProperty(name);
        }
    }
    Pointer * edge = qobject_cast< Pointer* >(object);
    if (edge) {
        QMap< DataStructure*,  QMultiMap <QString, Pointer* > >::iterator multimap = _EdgesProperties.find(edge->dataStructure().get());
        if (multimap == _EdgesProperties.end()) { //Not exist a dataStructure yet
            return;
        }

        foreach(edge, multimap.value().values(name)) {
            edge->addDynamicProperty(newName, edge->property(name));
            edge->removeDynamicProperty(name);
        }
    }
    DataStructure * dataStructure = qobject_cast<DataStructure*>(object);
    if (dataStructure) {
        dataStructure->addDynamicProperty(newName, dataStructure->property(name));
        dataStructure->removeDynamicProperty(name);
    }
}


