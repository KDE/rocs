/*
    This file is part of Rocs.
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef POINTER_H
#define POINTER_H

#include "QtScriptBackend.h"
#include "PointerType.h"
#include <QObject>
#include <QtScript>
#include <QString>
#include <QColor>
#include <QScriptValue>

#include <boost/scoped_ptr.hpp>

#include "RocsCoreExport.h"
#include "CoreTypes.h"

class Data;
class PointerPrivate;
class DataStructure;


/**
* \class Pointer
*
* Pointers are connections between two nodes in the data structure. They are usually unidirectional
* and hold several properties.
* Properties can accessed directly by the getter methods or (also from the script engine) be created
* by 'add_property(name, value)' and access by 'pointer.propertyName'.
*/
class ROCSLIB_EXPORT Pointer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(qreal width READ width WRITE setWidth)

public:
    /**
     * Create pointer objects.
     *
     * \param dataStructure is data structure to that the new data element belongs
     * \param from is source of pointer
     * \param to is target of pointer
     * \param pointerType is the type of the created pointer, it must be existing and registered
     * \return the created pointer element
     */
    static PointerPtr create(DataStructurePtr parent, DataPtr from, DataPtr to, int pointerType);

    /**
     * \return shared pointer to this pointer element
     */
    PointerPtr getPointer() const;

    /**
     * Default destructor.
     * DO NOT CALL IT, let the shared pointer take care for deletion.
     */
    virtual ~Pointer();

    PointerType::Direction direction() const;

    /**
     * \return data structure to that this pointer belongs
     */
    DataStructurePtr dataStructure() const;

    /**
     * The relative index is the index that this pointer has relative to the pair of data elements
     * to which it is bound. I.e., if its end points have have two or more pointers
     * connecteds between them, the relative index gives a unique identifier for the pointer.
     * Note that the relative index can change whenever pointers between its endpoints are created
     * or removed.
     *
     * \return the relative index identifier
     */
    int relativeIndex() const;

    /*! if the qtscript is enabled for this rocs,
      this method returns the self-referenced script value for this pointer.
      \return QScriptValue self reference for this datum.
      */
    QScriptValue scriptValue() const;

    /*! if qtscript is enabled for this rocs,
    this method will set the engine for this single object
    \p engine the QScriptEngine that will work on the object */
    void setEngine(QScriptEngine *engine);

signals:
    /**
     * Emitted when this pointer is removed
     */
    void removed();

    /**
     * Emitted when a connected data element or the pointer is changed.
     */
    void changed();

    /**
     * Emitted when the position of a connected data element changes.
     */
    void posChanged();

    /**
     * Emitted when the pointer type of this pointer changes.
     */
    void pointerTypeChanged(int pointerType);

    /**
     * Emitted when direction was changed.
     */
    void directionChanged(PointerType::Direction direction);

    void propertyAdded(QString name);
    void propertyRemoved(QString name);
    void propertyChanged(QString name);

public slots:
    /**
     * \return source data element of this pointer
     */
    DataPtr from() const;

    /**
     * \return target data element of this pointer
     */
    DataPtr to() const;

    /**
     * Remove pointer.
     */
    void remove();

    /**
     * \deprecated
     * Will be removed in Rocs 2.0
     */
    void self_remove();

    /**
     * Change pointer type of the pointer. The specified pointer type must exist.
     * \param pointerType is the new pointer type
     */
    void setPointerType(int pointerType);

    /**
     * \return the color of this pointer
     */
    QColor color() const;

    /**
     * Set color attribute of this pointer. The new color must be set either in format "#000000" or by
     * it's english name ("red" for example).
     *
     * \param color is the new color of this pointer
     */
    void setColor(const QColor& color);

    /**
     * \return pointer type identifier, \see class PointerType
     */
    int pointerType() const;

    /**
     * \return width of the pointer
     */
    qreal width() const;

    /**
     * Set the width of this pointer.
     *
     * \param width of the pointer line
     */
    void setWidth(qreal width);

    /**
     * \return line style of the pointer line
     */
    Qt::PenStyle style() const;

    /**
     * Add new dynamic property with identifier \p property to this data element and
     * sets it to \p value.
     *
     * \param property is the identifier for the new property
     * \param value is the value of this new property
     */
    void addDynamicProperty(const QString& property, const QVariant& value);

    /**
     * Remove dynamic property with identifier \p property from data element.
     *
     * \param property is identifier of the property
     */
    void removeDynamicProperty(const QString& property);

    void updateDynamicProperty(const QString& property);

    void renameDynamicProperty(const QString& oldName, const QString& newName);

    /**
     * Update relative index. \see relativeIndex().
     */
    void updateRelativeIndex();

    QList<QString> properties() const;

    bool isVisible() const;
    void setVisible(bool visible);

    /**
     * \return pointer type identifier for script engine
     */
    QScriptValue type() const;

    /**
     * Set pointer type from script engine. If pointer type \p type does not exist, the method
     * returns false. Otherwise it returns true.
     *
     * \param type is pointer type identifier.
     * \return if change of type was successful
     */
    QScriptValue set_type(int type);

    /**
     * Add new property to pointer.
     *
     * \param name is identifier for new property
     * \param value is the initial value of the property
     */
    void add_property(const QString& name, const QString& value);

    /**
     * Remove a property named \p name from this pointer.
     * \param name identifier of the property to remove.
     */
    void remove_property(const QString& name);
    /**
     * \return the \see from() data element for script engine
     */
    QScriptValue start() const;

    /**
     * \return the \see to() data element for script engine
     */
    QScriptValue end() const;

protected:
    /**
     * Protected constructor. Use static \see create() method.
     *
     * \param parent the data structure the pointer belongs to
     * \param from the first data element
     * \param to the second data element
     * \param pointerType the type of this pointer (default is 0)
     */
    Pointer(DataStructurePtr parent, DataPtr from, DataPtr to, int pointerType = 0);

    /**
     * Factory method to create a pointer element of type \p T.
     */
    template<typename T>
    static PointerPtr create(DataStructurePtr parent, DataPtr from, DataPtr to, int pointerType) {
        PointerPtr pi(new T(parent, from, to, pointerType));
        pi->setQpointer(pi);
        pi->initialize();
        return pi;
    }

    bool eventFilter(QObject *obj, QEvent *event);

private:
    /**
     * \internal
     * d-Pointer.
     */
    boost::scoped_ptr<PointerPrivate> d;

    /**
     * \internal
     * Set q-pointer in private data object.
     */
    void setQpointer(PointerPtr q);

    void initialize();
    Pointer(const Pointer&);
    Pointer& operator=(const Pointer&);
};

#endif
