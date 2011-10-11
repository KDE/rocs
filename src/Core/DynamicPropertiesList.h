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

#ifndef DYNAMICPROPERTIESLIST_H
#define DYNAMICPROPERTIESLIST_H

#include "rocslib_export.h"
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QList>

class Data;
class DataStructure;
class Pointer;

enum DynamicPropertyType{
      None,
      Unique,
      Multiple,
      Global
};


class ROCSLIB_EXPORT DynamicPropertiesList : public QObject
{
  Q_OBJECT
  QMap< DataStructure*,  QMultiMap <QString, Data* > > _NodesProperties;
  QMap< DataStructure*,  QMultiMap <QString, Pointer* > > _EdgesProperties;
  QMultiMap <QString, DataStructure*> _GraphProperties;

  static DynamicPropertiesList * self;

  DynamicPropertiesList(QObject* parent = 0);

public:

  static DynamicPropertiesList * New();

  void addProperty(QObject *obj,const QString& name);
  void removeProperty(QObject *obj,const QString& name);

  DynamicPropertyType type(QObject *obj,const QString& name);
  const QString typeInText(QObject* obj,const QString& name);

//   const QStringList properties (QObject * obj);

  void clear(DataStructure * dataStructure = 0);

  /** Change property name in all objects from same graph of object.*/
  void changePropertyName(QString name, QString newName, QObject* object);


};

#endif // DYNAMICPROPERTIESLIST_H
