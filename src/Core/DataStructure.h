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
    
    QMap<int,DataList> _dataTypeLists;         // list if data elements associated to specific type
    QMap<int,PointerList> _pointerTypeLists;   // list of pointers associated to specific type
    QMap<int,QString> _dataTypes;           // list of data types
    QMap<int,QString> _pointerTypes;        // list of pointer types
    
    int _identifierCount;   // represents the next identifier that will be assigend to data/pointer

    QList<Group*> _groups;
    qreal _top;
    qreal _bottom;
    qreal _left;
    qreal _right;

    QPointF _relativeCenter;
    QString _name;
    QColor _dataDefaultColor;
    QColor _pointerDefaultColor;
    bool _automate;
    Document *_document;
    bool _readOnly;

    bool _dataNamesVisible;
    bool _pointerNamesVisible;
    bool _dataValuesVisible;
    bool _pointerValuesVisible;

    QScriptValue _value;
    QScriptEngine *_engine;
    QMap<QString, QVariant> m_globalPropertiesData;
    QMap<QString, QVariant> m_globalPropertiesPointer;
};

class ROCSLIB_EXPORT DataStructure : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QColor dataDefaultColor READ dataDefaultColor WRITE setDataDefaultColor)
    Q_PROPERTY(QColor pointerDefaultColor READ pointerDefaultColor WRITE setPointerDefaultColor)

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
    void setEngine( QScriptEngine *engine );
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

    /** removes this pointer type and all data elements of this type
     * \param pointerType is positive id>0
     * \return true if a dataType was removed
     */
    bool removePointerType(int pointerType);
    
    // getters
    QString getDataTypeName(int dataType) const;
    QString getPointerTypeName(int pointerType) const;
    
    bool dataNameVisibility() const;
    bool pointerNameVisibility() const;
    bool dataValueVisibility() const;
    bool pointerValueVisibility() const;
    const QColor& pointerDefaultColor() const;
    const QColor& dataDefaultColor() const;
    const QString& name() const;
    const DataList dataList(int dataType=0) const;
    const PointerList pointers(int pointerType=0) const;
    const QList<Group*> groups() const;

    /** @brief clear data that only is usefull for a type of data structure and that cannot be converted to others
    */
    virtual void cleanUpBeforeConvert(){ }

public slots:
    virtual DataPtr addData(QString name, int dataType=0);
    virtual DataList addDataList(DataList dataList, int dataType=0);
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
    void setDataColor(const QColor& c);
    void setPointersColor(const QColor& c);
    void setDataNameVisibility(bool b);
    void setPointerValueVisibility(bool b);
    void setDataValueVisibility(bool b);
    void setPointerNameVisibility(bool b);
    void setDataDefaultColor(const QColor& color);
    void setPointerDefaultColor(const QColor& color);

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
    template<typename T> static DataStructurePtr create(Document *parent = 0) {
        DataStructurePtr pi(new T(parent));
        pi->d->q=pi;
        return pi;
    }
    template<typename T> static DataStructurePtr create(DataStructurePtr other, Document *parent = 0) {
        DataStructurePtr pi(new T(parent));
        pi->d->q=pi;
        pi->importStructure(other);
        return pi;
    }
    
private:
    boost::shared_ptr<DataStructurePrivate> d;
};



inline bool DataStructure::readOnly()                      const { return d->_readOnly;              }
inline bool DataStructure::dataValueVisibility()           const { return d->_dataValuesVisible;     }
inline bool DataStructure::dataNameVisibility()            const { return d->_dataNamesVisible;     }
inline bool DataStructure::pointerNameVisibility()         const { return d->_pointerNamesVisible;   }
inline bool DataStructure::pointerValueVisibility()        const { return d->_pointerValuesVisible;  }

inline const QColor& DataStructure::pointerDefaultColor()  const { return d->_pointerDefaultColor; }
inline const QColor& DataStructure::dataDefaultColor()     const { return d->_dataDefaultColor;    }
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
