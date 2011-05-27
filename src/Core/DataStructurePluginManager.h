/*
    This file is part of Rocs.
    Copyright (C) 2010  Wagner Reck <wagner.reck@gmail.com>

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

#ifndef DATASTRUCTUREPLUGINMANAGER_H
#define DATASTRUCTUREPLUGINMANAGER_H

#include <QObject>

#include "rocslib_export.h"

class QLayout;
class QGraphicsItem;
class KPluginInfo;
class DataStructure;
class Document;
class Data;
class Pointer;

class DataStructurePluginInterface;


class ROCSLIB_EXPORT DataStructurePluginManager : public QObject {
  Q_OBJECT

  public:
    /** Accessor to the manager's instance.*/
    static DataStructurePluginManager * self();

    ~DataStructurePluginManager();

    /** build a new dataType using actual data structure. */
    DataStructure * createNewDataStructure(Document* parent, const QString& pluginName = QString());

    /** When the DataStructure pluin  is changed, all the existent dataTypes must be 'rebuild' with the new DataStructure. If is not possible to change to a new data structure without losing data, then user should be asked to continue or nor. If user say to not continue, all ready done job should be undo.*/
    DataStructure * changeToDataStructure(DataStructure* dataStructure, Document* parent);

    /** return the list of avaliable data structure plugins.*/
    const QStringList listOfDataStructures();

    /** return informations about plugin.*/
    KPluginInfo pluginInfo ( DataStructurePluginInterface* plugin ) const;


    /** return the active plugin that will be used in convertions and to build news data structures.*/
    DataStructurePluginInterface* actualPlugin();

    QList < DataStructurePluginInterface*> pluginsList();

    /** return the data dataTypeic item that will be drawed by canvas . */
    QGraphicsItem * dataItem(Data* data);
    /** return the pointer dataTypeic item that will be drawed by canvas . */
    QGraphicsItem * pointerItem(Pointer* pointer);


    /** Create extra widgets to data properties window. If no extra properties is needed the is returned 0*/
    QLayout * dataExtraProperties(Data* data, QWidget * parent);
    /** Create extra widgets to pointer properties window. If no extra properties is needed the is returned 0*/
    QLayout * pointerExtraProperties(Pointer* pointer, QWidget * parent);
    /** Create extra widgets to dataType properties window. If no extra properties is needed the is returned 0*/
    QLayout * dataStructureExtraProperties(DataStructure* dataType, QWidget * parent);

  signals:
    /** signal emited when is changing the data structure to \newDataStructure */
    void changingDataStructure(const QString &newDataStructure);

    /** this signal is emited at end of change of DataStructure. This is used to say that the change was a success.*/
    void DataStructureChangedTo(const QString &newDataStructure);



  public slots:
      /** used to set change the data structure. */
    void changeActiveDataStructure();
    void changeActiveDataStructure( const QString& newDataStructure);
    QString actualPluginName() const;

  private:
    DataStructurePluginManager();

    static DataStructurePluginManager * _self;
    class DataStructurePluginManagerPrivate * const _d;
};

#endif // DataStructurePLUGINMANAGER_H
