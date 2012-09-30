/*
    This file is part of Rocs.
    Copyright (C) 2010  Wagner Reck <wagner.reck@gmail.com>

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

#ifndef DATASTRUCTUREPLUGINMANAGER_H
#define DATASTRUCTUREPLUGINMANAGER_H

#include <QObject>
#include <boost/shared_ptr.hpp>
#include "RocsCoreExport.h"
#include "CoreTypes.h"

class QLayout;
class QGraphicsItem;
class KPluginInfo;
class DataStructure;
class Document;
class Data;
class Pointer;

class DataStructurePluginInterface;


class ROCSLIB_EXPORT DataStructurePluginManager : public QObject
{
    Q_OBJECT

public:
    /** Accessor to the manager's instance.*/
    static DataStructurePluginManager * self();

    /** default destructor */
    ~DataStructurePluginManager();

    /** build a new DataStructure using the active plugin. */
    DataStructurePtr createNewDataStructure(Document* parent, const QString& pluginName = QString());

    /** When the DataStructure pluin  is changed,
     * all the existent dataTypes must be 'rebuild' with the new DataStructure.
     * If is not possible to change to a new data structure without losing data,
     * then user should be asked to continue or not.
     * If user say to not continue, do not change it.
     */
    DataStructurePtr changeToDataStructure(DataStructurePtr dataStructure, Document* parent);

    /** return the list of available data structure plugins.*/
    const QStringList listOfDataStructures();

    /** return information about plugin.*/
    KPluginInfo pluginInfo(DataStructurePluginInterface* plugin) const;


    /**
     * \return the active plugin that will be used in convertions
     * and to build news data structures.
     */
    DataStructurePluginInterface* actualPlugin();

    QList < DataStructurePluginInterface*> pluginsList();

    DataStructurePluginInterface* plugin(const QString & internalName ) const;

    /** \return the data Visible item for the data that will be drawed by canvas . */
    QGraphicsItem * dataItem(DataPtr data);

    /** \return the visible pointer item that will be drawed by canvas . */
    QGraphicsItem * pointerItem(PointerPtr pointer);


    /** Create extra widgets to data properties window.
     *If no extra properties are needed, a null pointer is returned.
     */
    QLayout * dataExtraProperties(DataPtr data, QWidget * parent);

    /** Create extra widgets to pointer properties window.
     *If no extra properties are needed, a null pointer is returned.
     */
    QLayout * pointerExtraProperties(PointerPtr pointer, QWidget * parent);

    /** Create extra widgets to the DataStructure properties window.
     * If no extra properties are needed, a null pointer is returned.
     * */
    QLayout * dataStructureExtraProperties(DataStructurePtr dataType, QWidget * parent);

signals:

    /** signal emitted when is changing the plugin data structure to newPlugin */
    void changingDataStructurePlugin(const QString &pluginName);

    /** this signal is emitted at end of change of DataStructure.
    *This is used to say that the change was successful.
     */
    void DataStructurePluginChanged(const QString &newDataStructure);

public slots:
    /** used to set change the data structure. */
    void setDataStructurePlugin();

    /**
     * \param unique identifier for the plugin
     */
    void setDataStructurePlugin(const QString &pluginIdentifier);
    QString pluginName() const;

private:
    DataStructurePluginManager();

    static DataStructurePluginManager * _self;
    class DataStructurePluginManagerPrivate * const _d;
};

#endif // DataStructurePLUGINMANAGER_H
