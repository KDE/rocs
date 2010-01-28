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

#ifndef DINAMICPROPERTIESLIST_H
#define DINAMICPROPERTIESLIST_H
#include <QObject>
#include <QList>
#include "graph.h"
#include "edge.h"

enum DinamicPropertyType{
      None,
      Unique,
      Multiple,
      Global
};


class DinamicPropertiesList : public QObject
{
  Q_OBJECT
  QMultiMap <QString, Node*> _NodesProperties;
  QMultiMap <QString, Edge*> _EdgesProperties;
  QMultiMap <QString, Graph*> _GraphProperties;
  
  static DinamicPropertiesList * self;
  
  DinamicPropertiesList(QObject* parent = 0);
  
public:
  
  static DinamicPropertiesList * New();
  
  void addProperty(QObject *obj, QString name);
  void removeProperty(QObject *obj, QString name);
    
  DinamicPropertyType type(QObject *obj, QString name);
  
  const QList <QString> properties (QObject * obj);
  
  void clear(Graph * graph = 0);
};

#endif // DINAMICPROPERTIESLIST_H
