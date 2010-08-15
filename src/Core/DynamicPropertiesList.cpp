/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

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
#include "DataType.h"

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


void DynamicPropertiesList::addProperty(QObject* obj, QString name)
{
      if (Datum * node = qobject_cast< Datum* >(obj)){
	  QMap< DataType*,  QMultiMap <QString, Datum* > >::iterator multimap = _NodesProperties.find(node->dataType());
	  if( multimap == _NodesProperties.end()){ //Not exist a dataType yet
	      QMultiMap <QString, Datum* > newMap;
	      multimap = _NodesProperties.insert(node->dataType(),newMap);
	  }

	  multimap.value().insert(name, node);

	  return;
      }

      if (Pointer * edge = qobject_cast< Pointer* >(obj)){
	  QMap< DataType*,  QMultiMap <QString, Pointer* > >::iterator multimap = _EdgesProperties.find(edge->dataType());
	  if( multimap == _EdgesProperties.end()){ //Not exist a dataType yet
	      QMultiMap <QString, Pointer* > newMap;
	      multimap = _EdgesProperties.insert(edge->dataType(),newMap);
	  }
	  multimap.value().insert(name, edge);
	  return;
      }

      DataType * dataType = qobject_cast< DataType* >(obj);

      if (dataType){
	  _GraphProperties.insert(name, dataType);
	  return;
      }
}


void DynamicPropertiesList::removeProperty(QObject* obj, QString name)
{
      Datum * node = qobject_cast< Datum* >(obj);
      if (node){
	  QMap< DataType*,  QMultiMap <QString, Datum* > >::iterator multimap = _NodesProperties.find(node->dataType());
	  if( multimap == _NodesProperties.end()){ //Not exist a dataType yet
	    return;
	  }
	  multimap.value().remove(name, node);
// 	  _NodesProperties.remove(name, node);
	  return;
      }

      Pointer * edge = qobject_cast< Pointer* >(obj);
      if (edge){
	  QMap< DataType*,  QMultiMap <QString, Pointer* > >::iterator multimap = _EdgesProperties.find(edge->dataType());
	  if( multimap == _EdgesProperties.end()){ //Not exist a dataType yet
		return;
	  }
	  multimap.value().remove(name, edge);
	  return;
      }

      DataType * dataType = qobject_cast< DataType* >(obj);
      if (dataType){
	  _GraphProperties.remove(name, dataType);
	  return;
      }
}

DynamicPropertyType DynamicPropertiesList::type(QObject* obj, QString name)
{
      Datum * node = qobject_cast< Datum* >(obj);
      if (node){
	  QMap< DataType*,  QMultiMap <QString, Datum* > >::iterator multimap = _NodesProperties.find(node->dataType());
	  if( multimap == _NodesProperties.end()){ //Not exist a dataType yet
	    return None;
	  }
	  QList <Datum*> list = multimap.value().values(name);

	  if (node->dataType()->data().size() == list.size()){
	      return Global;
	  }
	  switch(list.size()){
	    case 0 :return None;
	    case 1 :return Unique;
	    default : return Multiple;
	  }
      }

      Pointer * edge = qobject_cast< Pointer* >(obj);
      if (edge){
	  QMap< DataType*,  QMultiMap <QString, Pointer* > >::iterator multimap = _EdgesProperties.find(edge->dataType());
	  if( multimap == _EdgesProperties.end()){ //Not exist a dataType yet
		return None;
	  }
	  QList <Pointer*> list = multimap.value().values(name);
	  if (edge->dataType()->pointers().size() == list.size()){
	      return Global;
	  }
	  switch(list.size()){
	    case 0 :return None;
	    case 1 :return Unique;
	    default : return Multiple;
	  }
      }

      DataType * dataType = qobject_cast< DataType* >(obj);
      if (dataType){
	    if (_GraphProperties.values(name).size() == 0){
	      return None;
	    }else {
		return Unique;
	    }
      }
      return None;
}

QString DynamicPropertiesList::typeInText(QObject* obj, QString name)
{
    switch (type(obj, name)){
      case Unique: 	return i18n("Unique");
      case Multiple: 	return i18n("Multiple");
      case Global: 	return i18n("Global");
      case None:
      default:
        return i18n("None");
    }
}



void DynamicPropertiesList::clear(DataType* dataType)
{
    if (dataType != 0){
	_EdgesProperties.values(dataType).clear();
	foreach (QString name,  _GraphProperties.keys(dataType) ){
	    _GraphProperties.remove(name, dataType);
	}

	_NodesProperties.values(dataType).clear();
    }else{
	_EdgesProperties.clear();
	_GraphProperties.clear();
	_NodesProperties.clear();
    }
}


void DynamicPropertiesList::changePropertyName(QString name, QString newName, QObject* object){
      Datum * node = qobject_cast< Datum* >(object);
      if (node){
	  QMap< DataType*,  QMultiMap <QString, Datum* > >::iterator multimap = _NodesProperties.find(node->dataType());
	  if( multimap == _NodesProperties.end()){ //Not exist a dataType yet
		return;
	  }	
	  foreach(node, multimap.value().values(name)){
	      node->addDynamicProperty(newName, node->property(name.toUtf8()));
	      node->removeDynamicProperty(name);
	  }
      }		
      Pointer * edge = qobject_cast< Pointer* >(object);
      if (edge){
	  QMap< DataType*,  QMultiMap <QString, Pointer* > >::iterator multimap = _EdgesProperties.find(edge->dataType());
	  if( multimap == _EdgesProperties.end()){ //Not exist a dataType yet
		return;
	  }

	 foreach (edge, multimap.value().values(name)){
	      edge->addDynamicProperty(newName, edge->property(name.toUtf8()));
	      edge->removeDynamicProperty(name);
	 }
      }
    DataType * dataType = qobject_cast<DataType*>(object);
    if (dataType){
	dataType->addDynamicProperty(newName, dataType->property(name.toUtf8()));
	dataType->removeDynamicProperty(name);
    }
}


