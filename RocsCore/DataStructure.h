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

#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <QObject>
#include <QList>
#include <QString>
#include <QtScript>
#include <QColor>

#include "RocsCoreExport.h"
#include "CoreTypes.h"
#include "Document.h"

class Group;
class Document;
class Data;
class Pointer;
class DataStructurePrivate;

/**
 * \class DataStructure
 */
class ROCSLIB_EXPORT DataStructure : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName)

public:
    static DataStructurePtr create(Document *parent = 0);
    static DataStructurePtr create(DataStructurePtr other, Document *parent = 0);

    virtual DataStructurePtr getDataStructure() const;

    void updateRelativeCenter();

    /**
    * Returns cached relative center of the data structure.
    * The relative center gets updated when data elements are added/deleted.
    *
    * \return center of datastructure
    */
    QPointF relativeCenter() const;
    Document *document() const;

    void setReadOnly(bool r);
    bool readOnly() const;

    QScriptValue scriptValue() const;
    virtual void setEngine(QScriptEngine *engine);
    QScriptEngine *engine() const;

    /**
     * Updates registration of data in internal reference list.
     * \param data that is set with new type
     */
    void updateData(DataPtr data);

    /**
     * Updates registration of pointer in internal reference list.
     * \param pointer that is set with new type
     */
    void updatePointer(PointerPtr pointer);

    /**
     * Returns true if data elements of specified data type are visible, otherwise false.
     * \param dataType for that visibility information are returned
     */
    bool isDataVisible(int dataType) const;

    /**
     * Returns true if pointers of specified pointer type are visible, otherwise false.
     * \param pointerType for that visibility information are returned
     */
    bool isPointerVisible(int pointerType) const;

    QString name() const;

    /**
     * Gives list of data elements of specified type if type exists.
     * If dataType does not exists, returns empty list.
     * \param dataType is type of data elements to be returned
     * \return DataList of data elements of specified type
     */
    const DataList dataList(int dataType = 0) const;

    /**
     * Gives list of all data elements of all existing types.
     * \return DataList of data elements
     */
    DataList dataListAll() const;

    /**
     * Gives list of pointers of specified type if type exists.
     * If pointerType does not exists, returns empty list.
     * \param pointerType is type of pointers to be returned
     * \return PointerList of pointers of specified type
     */
    const PointerList pointers(int pointerType = 0) const;

    /**
     * Gives list all pointers of all existing types.
     * \return PointerList of pointers
     */
    PointerList pointerListAll() const;

    const QList<GroupPtr> groups() const;

    /** @brief clear data that only is useful for a type of data structure and that cannot be converted to others
    */
    virtual void cleanUpBeforeConvert();

    /**
     * Gives a map with plugin specific properties of the data structure.
     * \return map keys are property names, values are property values.
     */
    virtual QMap<QString,QString> pluginProperties() const;

    /**
     * Set plugin specific properties of data structure.
     * \param identifier is the unique identifier for this property
     * \param value is the to be set value for the property
     */
    virtual void setPluginProperty(const QString& /*identifier*/, const QString& /*property*/);

public Q_SLOTS:
    virtual DataPtr addData(const QString& name, int dataType = 0);
    virtual DataList addDataList(DataList dataList, int dataType = 0);

    /**
     * Creates new pointer from data element "from" to data element "to" of
     * given type "pointerType". The pointer type must exist.
     *
     * \param from data element where the pointer starts
     * \param to data element where the pointer ends
     * \param pointerType is the type of this pointer
     */
    virtual PointerPtr addPointer(DataPtr from, DataPtr to, int pointerType = 0);

    /**
     * Access data element by its unique identifier.
     * Operation has access time O(n).
     *
     * \param uniqueIdentifier the unique identifier of the data element
     */
    DataPtr getData(int uniqueIdentifier);

    /**
     * Remove \p data from data structure and (if necessary) destroys the data object.
     * It is valid to call this method more than once for the same data object.
     *
     * \param data the data element that shall be removed
     */
    virtual void remove(DataPtr data);

    /**
     * Remove \p pointer from data structure and (if necessary) destroys the pointer object.
     * It is valid to call this method more than once for the same pointer object.
     *
     * \param pointer the pointer that shall be removed
     */
    virtual void remove(PointerPtr pointer);

    /**
     * Remove \p group from data structure and (if necessary) destroys the group object.
     * It is valid to call this method more than once for the same group object.
     *
     * \param group the group that shall be removed
     */
    virtual void remove(GroupPtr group);

    virtual GroupPtr addGroup(const QString& name);
    virtual DataList addDataList(QList< QPair<QString, QPointF> > dataList, int dataType = 0);
    virtual DataPtr addData(const QString& name, const QPointF& point, int dataType = 0);

    void addDynamicProperty(const QString& property, const QVariant& value = QVariant(0));
    void removeDynamicProperty(const QString& property);
    void renameDynamicProperty(const QString& oldName, const QString& newName);

    /** add the property named \p name to this Data structure.
     * \param name is an identifier to property.
     * \param value is the initial value of property.
     */
    void add_property(const QString& name, const QVariant& value);

    /** remove the string named \p name from this data structure.
     * \param name is the name of property to be removed.
     */
    void remove_property(const QString& name);

    /**
     * add dynamic property to all data elements of data structure
     */
    void addPointersDynamicProperty(const QString& property, const QVariant& value = QVariant(0));

    void removePointersDynamicProperty(const QString& property);

    /**
     * if this datastructure shall be deleted, call ONLY this function
     */
    void remove();

    // setters
    void setName(const QString& s);

    /**
     * Set all colors of all data elements of given \param dataType to the given value of \param color.
     * This is a fast implementation that starts several threads for updating all data elements in parallel.
     *
     * \param color is the color to be set
     * \param dataType is the identifier of the data type for that this function has affect.
     */
    void setDataColor(const QColor& color, int dataType);

    /**
     * Set all data items for all data elements of given \param dataType to the given value of \param visible.
     * This is a fast implementation that starts several threads for updating all data elements in parallel.
     *
     * \param visible must be true if data item shall be shown, false if hidden
     * \param dataType is the identifier of the data type for that this function has affect.
     */
    void setDataVisibility(bool visible, int dataType);

    void toggleDataVisibility(int dataType);

    /**
     * Set all colors of all data of given \param dataType to the given value of \param color.
     * This is a fast implementation that starts several threads for updating all data types in parallel.
     *
     * \param color is the color to be set
     * \param dataType is the identifier of the data type for that this function has affect.
     */
    void setPointerColor(const QColor& color, int dataType);

    /**
     * Set all pointer items for pointers of given \param pointerType to the given value of \param visible.
     * This is a fast implementation that starts several threads for updating all pointers in parallel.
     *
     * \param visible must be true if pointer shall be shown, false if hidden
     * \param pointerType is the identifier of the pointer type for that this function has affect.
     */
    void setPointerVisibility(bool visible, int pointerType);

    void togglePointerVisibility(int pointerType);

// #ifdef USING_QTSCRIPT
//     QScriptValue list_data();
//     QScriptValue list_pointers();
//     QScriptValue add_data(const QString& name);
//     QScriptValue add_pointer(Data* from, Data* to);
//     QScriptValue data_byname(const QString& name);
//     QScriptValue begin_data();
//     QScriptValue end_data();
// #endif

private Q_SLOTS:
    /**
     * Only for internal use of the data structure: Slot that is connected signal
     * Document::dataTypeCreated(...). this method registers data type for this data structure.
     * \param identifier is the data type identifier of data type that will be registered
     */
    void registerDataType(int identifier);

    /**
     * Only for internal use of the data structure: Slot that is connected signal
     * Document::pointerTypeCreated(...). this method registers pointer type for this data structure.
     * \param identifier is the pointer type identifier of pointer type that will be registered
     */
    void registerPointerType(int identifier);

    /**
     * Only for internal use of the data structure: Slot that is connected signal
     * Document::dataTypeRemoved(...). this method removes data type for this data structure.
     * \param identifier is the data type identifier of data type that will be removed
     */
    void removeDataType(int identifier);

    /**
     * Only for internal use of the data structure: Slot that is connected signal
     * Document::pointerTypeRemoved(...). this method removes pointer type for this data structure.
     * \param identifier is the pointer type identifier of pointer type that will be removed
     */
    void removePointerType(int identifier);

Q_SIGNALS:
    void dataCreated(DataPtr n);
    void pointerCreated(PointerPtr e);
    void complexityChanged(bool directed);
    void nameChanged(const QString& name);
    void changed();
    void resizeRequest(Document::Border border);

protected:
    DataPtr addData(DataPtr data, int dataType = 0);
    PointerPtr addPointer(PointerPtr pointer, int pointerType = 0);
    int generateUniqueIdentifier();

    /**
     * Default constructor.
     *
     * \param parent the parent document of the data structure
     */
    DataStructure(Document *parent = 0);

    /**
     * Default destructor.
     */
    virtual ~DataStructure();

    /**
     * overwrites the current DataStructure with all values (Data and Pointer)
     * from the given datastructure object.
     *
     * \param other the data structure that shall be imported
     * \return void
     */
    virtual void importStructure(DataStructurePtr other);

    void initialize();
    template<typename T> static DataStructurePtr create(Document *parent = 0) {
        DataStructurePtr pi(new T(parent), deleter());
        pi->setQpointer(pi);
        pi->initialize();
        return pi;
    }
    template<typename T> static DataStructurePtr create(DataStructurePtr other, Document *parent = 0) {
        DataStructurePtr pi(new T(parent), deleter());
        pi->setQpointer(pi);
        pi->initialize();
        pi->importStructure(other);
        return pi;
    }

private:
    /**
     * \internal
     * d-Pointer.
     */
    const QScopedPointer<DataStructurePrivate> d;

    DataStructure(const DataStructure&);
    DataStructure& operator=(const DataStructure&);

    /**
     * \internal
     * Set q-pointer in private data object.
     */
    void setQpointer(DataStructurePtr q);

    class deleter;
    friend class deleter;
    class deleter
    {
        public:
        void operator()(DataStructure * p) { delete p; }
    };
};

#endif
