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

#include <klocalizedstring.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "Rocs_Typedefs.h"
#include "rocslib_export.h"
#include "Document.h"

class Group;
class Document;
class Data;
class Pointer;
class DataStructurePrivate;

class DataStructurePrivate{
public:
    DataStructurePrivate(){}

    /**
     * self pointer to DataStructure
     */
    boost::weak_ptr<DataStructure> q;

    QMap<int, DataList> _dataTypeLists;         // list if data elements associated to specific type
    QMap<int, PointerList> _pointerTypeLists;   // list of pointers associated to specific type
    QMap<int, DataTypePtr> _dataTypes;           // list of data types
    QMap<int, PointerTypePtr> _pointerTypes;        // list of pointer types

    int _identifierCount;   // represents the next identifier that will be assigend to data/pointer

    QList<Group*> _groups;
    qreal _top;
    qreal _bottom;
    qreal _left;
    qreal _right;

    QPointF _relativeCenter;
    QString _name;
    QColor _dataDefaultColor;
    bool _automate;
    Document *_document;
    bool _readOnly;

    bool _dataNamesVisible;
    bool _dataValuesVisible;

    QScriptValue _value;
    QScriptEngine *_engine;
    QMap<QString, QVariant> m_globalPropertiesData;
    QMap<QString, QVariant> m_globalPropertiesPointer;
};

class ROCSLIB_EXPORT DataStructure : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName)
//     Q_PROPERTY(QColor dataDefaultColor READ dataDefaultColor WRITE setDataDefaultColor) //FIXME disabled for now: incompatible to current layout
//     Q_PROPERTY(QColor pointerDefaultColor READ pointerDefaultColor WRITE setPointerDefaultColor) //FIXME disabled for now: incompatible to current layout

public:
    static DataStructurePtr create(Document *parent = 0);
    static DataStructurePtr create(DataStructurePtr other, Document *parent = 0);

    virtual DataStructurePtr getDataStructure() const;
    virtual ~DataStructure();

    void updateRelativeCenter();
    QPointF relativeCenter() const;
    Document *document() const;

    void setReadOnly( bool r) ;
    bool readOnly() const ;

    QScriptValue scriptValue() const;
    virtual void setEngine( QScriptEngine *engine );
    QScriptEngine *engine() const;

    /** register new type for data elements
     * \param name of the dataType
     * \return positive integer >0 if successfully registered, else <=0
     */
    int registerDataType(QString name);

    /** register new type for pointers
     * \param name of the pointerType
     * \return positive integer >0 if successfully registered, else <=0
     */
    int registerPointerType(QString name);

    /** removes this data type and all data elements of this type
     * \param dataType is positive id>0
     * \return true if a dataType was removed
     */
    bool removeDataType(int dataType);

    /** Removes this pointer type and all data elements of this type.
     * Aborts and returns "false" if pointer type is "0" or if the pointertype does not exists.
     * \param pointerType is positive id>0
     * \return true if a dataType was removed
     */
    bool removePointerType(int pointerType);

    PointerTypePtr pointerType(int pointerType) const;
    DataTypePtr dataType(int dataType) const;

    // getters
    QString getDataTypeName(int dataType) const;
    QString getPointerTypeName(int pointerType) const;

    /**
     * Getter for all registered data types.
     * \return list of all data type ids
     */
    QList<int> dataTypeList() const;

    /**
     * Getter for all registered pointer types.
     * \return list of all pointer type ids
     */
    QList<int> pointerTypeList() const;

    bool dataNameVisibility() const;
    bool dataValueVisibility() const;
    const QColor& dataDefaultColor() const;
    const QString& name() const;

    /**
     * Gives list of data elements of specified type if type exists.
     * If dataType does not exists, returns empty list.
     * \param dataType is type of data elements to be returned
     * \return DataList of data elements of specified type
     */
    const DataList dataList(int dataType=0) const;

    /**
     * Gives list of pointers of specified type if type exists.
     * If pointerType does not exists, returns empty list.
     * \param pointerType is type of pointers to be returned
     * \return PointerList of pointers of specified type
     */
    const PointerList pointers(int pointerType=0) const;
    const QList<Group*> groups() const;

    /** @brief clear data that only is usefull for a type of data structure and that cannot be converted to others
    */
    virtual void cleanUpBeforeConvert(){ }

public slots:
    virtual DataPtr addData(QString name, int dataType=0);
    virtual DataList addDataList(DataList dataList, int dataType=0);

    /**
     * Creates new pointer from data element "from" to data element "to" of
     * given type "pointerType". The pointer type must exist.
     * \param from data element where the pointer starts
     * \param to data element where the pointer ends
     * \param pointerType is the type of this pointer
     */
    virtual PointerPtr addPointer(DataPtr from, DataPtr to, int pointerType=0);

    /**
     * get data by unique identifier
     * iterates over all data elements, running time O(n)
     * \param identifier
     */
    DataPtr getData(int uniqueIdentifier);

    virtual void remove(DataPtr n);

    /**
     * Unregister pointer from data structure. Do not use this for actual removal of pointer.
     * To remove a pointer \see Pointer::remove().
     */
    virtual void remove(PointerPtr e);
    virtual void remove(Group *g);

    virtual Group *addGroup(const QString& name);
    virtual QList< DataPtr > addDataList(QList< QPair<QString,QPointF> > dataList, int dataType=0);
    virtual DataPtr addData(QString name, QPointF point, int dataType=0);
    virtual PointerPtr addPointer(const QString& name_from, const QString& name_to, int pointerType=0);

    void addDynamicProperty(const QString& property, QVariant value = QVariant(0));
    void removeDynamicProperty(const QString& property);

    /**
     * add dynamic property to all data elements of data structure
     */
    void addDataDynamicProperty(const QString& property, QVariant value = QVariant(0));
    void addPointersDynamicProperty(const QString& property, QVariant value = QVariant(0));

    void removeDataDynamicProperty(const QString& property);
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
    void setDataColor(QColor color, int dataType);

    /**
     * Set all names of all data elements of given \param dataType to the given value of \param visible.
     * This is a fast implementation that starts several threads for updating all data elements in parallel.
     *
     * \param visible must be true if data names shall be shown, false if hidden
     * \param dataType is the identifier of the data type for that this function has affect.
     */
    void setDataNameVisibility(bool visible, int dataType);

    /**
     * Set all values of all data of given \param dataType to the given value of \param visible.
     * This is a fast implementation that starts several threads for updating all data types in parallel.
     *
     * \param visible must be true if data values shall be shown, false if hidden
     * \param dataType is the identifier of the data type for that this function has affect.
     */
    void setDataValueVisibility(bool visible, int dataType);

    /**
     * Set all colors of all data of given \param dataType to the given value of \param color.
     * This is a fast implementation that starts several threads for updating all data types in parallel.
     *
     * \param color is the color to be set
     * \param dataType is the identifier of the data type for that this function has affect.
     */
    void setPointerColor(QColor color, int dataType);

    /**
     * Set all names of all pointers of given \param pointerType to the given value of \param visible.
     * This is a fast implementation that starts several threads for updating all pointers in parallel.
     *
     * \param visible must be true if pointer names shall be shown, false if hidden
     * \param pointerType is the identifier of the pointer type for that this function has affect.
     */
    void setPointerNameVisibility(bool visible, int pointerType);

    /**
     * Set all values of all pointers of given \param pointerType to the given value of \param visible.
     * This is a fast implementation that starts several threads for updating all pointers in parallel.
     *
     * \param visible must be true if pointer values shall be shown, false if hidden
     * \param pointerType is the identifier of the pointer type for that this function has affect.
     */
    void setPointerValueVisibility(bool visible, int pointerType);

// #ifdef USING_QTSCRIPT
//     QScriptValue list_data();
//     QScriptValue list_pointers();
//     QScriptValue add_data(const QString& name);
//     QScriptValue add_pointer(Data* from, Data* to);
//     QScriptValue data_byname(const QString& name);
//     QScriptValue begin_data();
//     QScriptValue end_data();
// #endif

signals:
    void dataCreated(DataPtr n);
    void pointerCreated(PointerPtr e);
    void dataTypeCreated(int identifier);
    void pointerTypeCreated(int identifier);
    void dataTypeRemoved(int identifier);
    void pointerTypeRemoved(int identifier);
    void complexityChanged(bool directed);
    void changed();
    void resizeRequest(Document::Border border);

protected:
    DataPtr addData(DataPtr data, int dataType=0);
    PointerPtr addPointer(PointerPtr pointer, int pointerType=0);
    int generateUniqueIdentifier();

protected:
    DataStructure(Document *parent = 0);

    /**
     * overwrites the current DataStructure with all values (Data and Pointer)
     * from the given datastructure object.
     * \param boost::shared_ptr<DataStructure> other
     * \return void
     */
    virtual void importStructure(DataStructurePtr other);

    void initialize();
    template<typename T> static DataStructurePtr create(Document *parent = 0) {
        DataStructurePtr pi(new T(parent));
        pi->d->q=pi;
        pi->initialize();
        return pi;
    }
    template<typename T> static DataStructurePtr create(DataStructurePtr other, Document *parent = 0) {
        DataStructurePtr pi(new T(parent));
        pi->d->q=pi;
        pi->initialize();
        pi->importStructure(other);
        return pi;
    }

private:
    boost::shared_ptr<DataStructurePrivate> d;
};



inline bool DataStructure::readOnly()                      const { return d->_readOnly;              }
inline const QString& DataStructure::name()                const { return d->_name;                }

/**
 * returns cached relative center of datastructure
 * center needs to be updated at resizes by using \see updateRelativeCenter()
 * \return QPointF center of datastructure
 */
inline QPointF DataStructure::relativeCenter()             const { return d->_relativeCenter;      }

inline QScriptValue DataStructure::scriptValue()           const { return d->_value;    }
inline QScriptEngine *DataStructure::engine()              const { return d->_engine;   }
inline Document *DataStructure::document()                 const { return d->_document; }

inline const QList<Group*>   DataStructure::groups()       const { return d->_groups;   }

#endif
