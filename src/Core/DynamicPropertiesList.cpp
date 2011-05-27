/*
    This file is part of Rocs.
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

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

DynamicPropertiesList * DynamicPropertiesList::self = 0;

DynamicPropertiesList::DynamicPropertiesList(QObject* parent): QObject(parent){

}


DynamicPropertiesList* DynamicPropertiesList::New()
{
    if (DynamicPropertiesList::self == 0){
        DynamicPropertiesList::self = new DynamicPropertiesList();
    }
    return DynamicPropertiesList::self;
}


void DynamicPropertiesList::addProperty(QObject* obj,const QString& name)
{
      if (Data * node = qobject_cast< Data* >(obj)){
	  QMap< DataStructure*,  QMultiMap <QString, Data* > >::iterator multimap = _NodesProperties.find(node->dataStructure());
	  if( multimap == _NodesProperties.end()){ //Not exist a dataStructure yet
	      QMultiMap <QString, Data* > newMap;
	      multimap = _NodesProperties.insert(node->dataStructure(),newMap);
	  }

	  multimap.value().insert(name, node);

	  return;
      }

      if (Pointer * edge = qobject_cast< Pointer* >(obj)){
	  QMap< DataStructure*,  QMultiMap <QString, Pointer* > >::iterator multimap = _EdgesProperties.find(edge->dataStructure());
	  if( multimap == _EdgesProperties.end()){ //Not exist a dataStructure yet
	      QMultiMap <QString, Pointer* > newMap;
	      multimap = _EdgesProperties.insert(edge->dataStructure(),newMap);
	  }
	  multimap.value().insert(name, edge);
	  return;
      }

      DataStructure * dataStructure = qobject_cast< DataStructure* >(obj);

      if (dataStructure){
	  _GraphProperties.insert(name, dataStructure);
	  return;
      }
}


void DynamicPropertiesList::removeProperty(QObject* obj,const QString& name)
{
      Data * node = qobject_cast< Data* >(obj);
      if (node){
	  QMap< DataStructure*,  QMultiMap <QString, Data* > >::iterator multimap = _NodesProperties.find(node->dataStructure());
	  if( multimap == _NodesProperties.end()){ //Not exist a dataStructure yet
	    return;
	  }
	  multimap.value().remove(name, node);
// 	  _NodesProperties.remove(name, node);
	  return;
      }

      Pointer * edge = qobject_cast< Pointer* >(obj);
      if (edge){
	  QMap< DataStructure*,  QMultiMap <QString, Pointer* > >::iterator multimap = _EdgesProperties.find(edge->dataStructure());
	  if( multimap == _EdgesProperties.end()){ //Not exist a dataStructure yet
		return;
	  }
	  multimap.value().remove(name, edge);
	  return;
      }

      DataStructure * dataStructure = qobject_cast< DataStructure* >(obj);
      if (dataStructure){
	  _GraphProperties.remove(name, dataStructure);
	  return;
      }
}

DynamicPropertyType DynamicPropertiesList::type(QObject* obj, const QString& name)
{
    // case obj is Data*
    Data * node = qobject_cast< Data* >(obj);
    if (node){
    QMap< DataStructure*,  QMultiMap <QString, Data* > >::iterator multimap = _NodesProperties.find(node->dataStructure());
    if( multimap == _NodesProperties.end()){ //Not exist a dataStructure yet
        return None;
    }
    
    QList <Data*> list = multimap.value().values(name);

    //FIXME the following is a hack that eleminates duplicates
    //happens eg with TestDynamicProperties::removeToAllNodes()
    QSet <Data*> tmpSet = list.toSet();
    list = tmpSet.toList();
    
    for(int i=0; i<list.size(); i++)
    if (node->dataStructure()->dataList().size() == list.size()){
        return Global;
    }
    switch(list.size()){
        case 0  :   return None;
        case 1  :   return Unique;
        default :   return Multiple;
    }
    }

    // case obj is Pointer*
    Pointer * edge = qobject_cast< Pointer* >(obj);
    if (edge){
    QMap< DataStructure*,  QMultiMap <QString, Pointer* > >::iterator multimap = _EdgesProperties.find(edge->dataStructure());
    if( multimap == _EdgesProperties.end()){ //Not exist a dataStructure yet
        return None;
    }
    QList <Pointer*> list = multimap.value().values(name);
    
    //FIXME the following is a hack that eleminates duplicates
    //happens eg with TestDynamicProperties::removeToAllEdges()
    QSet <Pointer*> tmpSet = list.toSet();
    list = tmpSet.toList();
    
    if (edge->dataStructure()->pointers().size() == list.size()){
        return Global;
    }
    switch(list.size()){
        case 0  :   return None;
        case 1  :   return Unique;
        default :   return Multiple;
    }
    }

    DataStructure * dataStructure = qobject_cast< DataStructure* >(obj);
    if (dataStructure){
        if (_GraphProperties.values(name).size() == 0){
        return None;
        }else {
        return Unique;
        }
    }
    return None;
}

const QString DynamicPropertiesList::typeInText(QObject* obj, const QString& name)
{
    switch (type(obj, name)){
      case Unique:      return i18n("Unique");
      case Multiple:    return i18n("Multiple");
      case Global:      return i18n("Global");
      case None:
      default:          return i18n("None");
    }
}



void DynamicPropertiesList::clear(DataStructure* dataStructure)
{
    if (dataStructure != 0){
	_EdgesProperties.values(dataStructure).clear();
	foreach (QString name,  _GraphProperties.keys(dataStructure) ){
	    _GraphProperties.remove(name, dataStructure);
	}

	_NodesProperties.values(dataStructure).clear();
    }else{
	_EdgesProperties.clear();
	_GraphProperties.clear();
	_NodesProperties.clear();
    }
}


void DynamicPropertiesList::changePropertyName(QString name, QString newName, QObject* object){
      Data * node = qobject_cast< Data* >(object);
      if (node){
	  QMap< DataStructure*,  QMultiMap <QString, Data* > >::iterator multimap = _NodesProperties.find(node->dataStructure());
	  if( multimap == _NodesProperties.end()){ //Not exist a dataStructure yet
		return;
	  }	
	  foreach(node, multimap.value().values(name)){
	      node->addDynamicProperty(newName, node->property(name.toUtf8()));
	      node->removeDynamicProperty(name);
	  }
      }		
      Pointer * edge = qobject_cast< Pointer* >(object);
      if (edge){
	  QMap< DataStructure*,  QMultiMap <QString, Pointer* > >::iterator multimap = _EdgesProperties.find(edge->dataStructure());
	  if( multimap == _EdgesProperties.end()){ //Not exist a dataStructure yet
		return;
	  }

	 foreach (edge, multimap.value().values(name)){
	      edge->addDynamicProperty(newName, edge->property(name.toUtf8()));
	      edge->removeDynamicProperty(name);
	 }
      }
    DataStructure * dataStructure = qobject_cast<DataStructure*>(object);
    if (dataStructure){
	dataStructure->addDynamicProperty(newName, dataStructure->property(name.toUtf8()));
	dataStructure->removeDynamicProperty(name);
    }
}


