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

#include "dinamicpropertieslist.h"



DinamicPropertiesList * DinamicPropertiesList::self = 0;

DinamicPropertiesList::DinamicPropertiesList(QObject* parent): QObject(parent){
  
}


DinamicPropertiesList* DinamicPropertiesList::New()
{
  if (DinamicPropertiesList::self == 0){
      DinamicPropertiesList::self = new DinamicPropertiesList();
  }
  return DinamicPropertiesList::self;
}


void DinamicPropertiesList::addProperty(QObject* obj, QString name)
{
      Node * node = qobject_cast< Node* >(obj);
      if (node){
	  _NodesProperties.insert(name, node);
	  return;
      }
      
      Edge * edge = qobject_cast< Edge* >(obj);
      if (edge){
	  _EdgesProperties.insert(name, edge);
	  return;
      }
      
      Graph * graph = qobject_cast< Graph* >(obj);
      if (graph){
	  _GraphProperties.insert(name, graph);
	  return;
      }
}


void DinamicPropertiesList::removeProperty(QObject* obj, QString name)
{
      Node * node = qobject_cast< Node* >(obj);
      if (node){
	  _NodesProperties.remove(name, node);
	  return;
      }
      
      Edge * edge = qobject_cast< Edge* >(obj);
      if (edge){
	  _EdgesProperties.remove(name, edge);
	  return;
      }
      
      Graph * graph = qobject_cast< Graph* >(obj);
      if (graph){
	  _GraphProperties.remove(name, graph);
	  return;
      }
}

DinamicPropertyType DinamicPropertiesList::type(QObject* obj, QString name)
{
      Node * node = qobject_cast< Node* >(obj);
      if (node){
	  QList <Node*> list = this->_NodesProperties.values(name);
	  int count= 0;
	  foreach (Node * n, list ){
	      if (n->graph() == node->graph()){
		++count;
	      }
	  }
	  if (node->graph()->nodes().size() == count){
	      return Global;
	  }
	  switch(count){
	    case 0 :return None;
	    case 1 :return Unique;
	    default : return Multiple;
	  }
      }
      
      Edge * edge = qobject_cast< Edge* >(obj);
      if (edge){
	  QList <Edge*> list = this->_EdgesProperties.values(name);
	  int count= 0;
	  foreach (Edge * e, list ){
	      if (e->graph() == edge->graph()){
		++count;
	      }
	  }
	  if (edge->graph()->edges().size() == count){
	      return Global;
	  }
	  switch(count){
	    case 0 :return None;
	    case 1 :return Unique;
	    default : return Multiple;
	  }
      }
      
      Graph * graph = qobject_cast< Graph* >(obj);
      if (graph){
	    return None;
      }
      return None;
}

QString DinamicPropertiesList::typeInText(QObject* obj, QString name){
    switch (type(obj, name)){
      case None: 	return i18n("None");
      case Unique: 	return i18n("Unique");
      case Multiple: 	return i18n("Multiple");
      case Global: 	return i18n("Global");
    }
}

const QStringList DinamicPropertiesList::properties(QObject* obj)
{
      Node * node = qobject_cast< Node* >(obj);
      if (node){
	  return _NodesProperties.keys(node);
      }
      
      Edge * edge = qobject_cast< Edge* >(obj);
      if (edge){
	  return _EdgesProperties.keys(edge);
      }
      
      Graph * graph = qobject_cast< Graph* >(obj);
      if (graph){
	  return _GraphProperties.keys(graph);
      }
      return QStringList();
}


void DinamicPropertiesList::clear(Graph* graph)
{
    if (graph == 0){
	_EdgesProperties.clear();
	_GraphProperties.clear();
	_NodesProperties.clear();
    }
}

