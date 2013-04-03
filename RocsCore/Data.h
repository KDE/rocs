/*
    This file is part of Rocs.
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
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

#ifndef DATA_H
#define DATA_H


#include "RocsCoreExport.h"
#include "CoreTypes.h"
#include "DataStructure.h"
#include "DataType.h"

#include <QObject>
#include <QString>
#include <QtScript>
#include <QColor>

class DataItem;
class Pointer;
class DataPrivate;

/**
 * \class Data
 */
class  ROCSLIB_EXPORT Data : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)
    Q_PROPERTY(qreal width READ width WRITE setWidth)
    Q_PROPERTY(int id READ identifier)
    Q_PROPERTY(QVariant color READ color WRITE setColor)

public:
    /**
     * Create data element objects.
     *
     * \param dataStructure is data structure to that the new data element belongs
     * \param uniqueIdentifier is unique identifier of the data element, it must be unique within the document
     * \param dataType is the type of the created data element, it must be existing and registered
     * \return the created data element
     */
    static DataPtr create(DataStructurePtr dataStructure, int uniqueIdentifier, int dataType);

    /**
     * \return shared pointer to this data element
     */
    virtual DataPtr getData() const;

    /**
     * Default destructor.
     * DO NOT CALL IT, let the shared pointer take care for deletion.
     */
    virtual ~Data();

    /**
     * Create new pointer to \p to. By using this method, the pointer gets registered at both of
     * its end-points. This method should be used to create new pointers.
     *
     * \param to is the target of the new pointer
     * \return the created pointer
     */
    PointerPtr addPointer(DataPtr to);

    /**
     * Add incoming pointer to in-pointer list.
     *
     * \param e is pointer to be added
     */
    void registerInPointer(PointerPtr e);

    /**
     * Add out pointer to out-pointer list.
     *
     * \param e is pointer to be added
     */
    void registerOutPointer(PointerPtr e);

    /**
     * Remove pointer from pointer list
     *
     * \param e is pointer to be removed
     */
    void remove(PointerPtr e);

    /**
     * \return data structure to that this data element belongs
     */
    DataStructurePtr dataStructure() const;

    /**
     * \return script value representation of this data element
     */
    QScriptValue scriptValue() const;

    /**
     * Set script engine. The engine must be set to access the script value.
     */
    virtual void setEngine(QScriptEngine *_engine);

    /**
     * \return script engine
     */
    QScriptEngine * engine() const;

    /**
     * \return x-position of data element
     */
    qreal x() const;

    /**
     * \return y-position of data element
     */
    qreal y() const;

    /**
     * \return size of data icon
     */
    qreal width() const;

    /**
     * \return color of data element
     */
    QVariant color() const;

    /**
     * \return icon of data element
     */
    QString icon() const;

    QList<QString> properties() const;

    /**
     * \return true if data element is visible, otherwise false
     */
    bool isVisible() const;

    /**
     * \return unique identifier of this data element
     */
    int identifier() const;

    /**
     * \return data type of this data element
     */
    int dataType() const;

    /**
     * \return list of adjacent data elements
     */
    DataList adjacentDataList() const;

    /**
     * \return list of all pointers connected to this data element (in and out pointers)
     */
    PointerList pointerList() const;

    /**
     * \return all pointers that connect this data element with \p to
     */
    PointerList pointerList(DataPtr to) const;

    /**
     * \return list of in-pointers of this data element
     */
    PointerList& inPointerList() const;

    /**
     * \return list of out-pointers of this data element
     */
    PointerList& outPointerList() const;

public Q_SLOTS:
    void remove();

    /**
     * \deprecated
     * Will be removed in Rocs 2.0
     */
    void self_remove();
    void setX(int x);
    void setY(int y);
    void setWidth(double w);
    void setPos(qreal x, qreal y);
    void setColor(const QVariant& s);
    void setVisible(bool visible);
    void setDataType(int dataType);

    /**
     * Add new dynamic property with identifier \p property to this data element and
     * sets it to \p value.
     *
     * \param property is the identifier for the new property
     * \param value is the value of this new property
     */
    void addDynamicProperty(const QString &property, const QVariant& value);

    /**
     * Remove dynamic property with identifier \p property from data element.
     *
     * \param property is identifier of the property
     */
    void removeDynamicProperty(const QString &property);

    void updateDynamicProperty(const QString &property);

    void renameDynamicProperty(const QString &oldName, const QString &newName);

    /**
     * FIXME proof of concept implementation: since each Pointer emits a changed direction signal,
     * there are unnecessary many updates.
     *
     * Update pointer \p e in inPointerList and outPointerList, depending on how the direction changed
     * This method is a condidate for optimizations, but it happens only seldom.
     */
    void updatePointerList();

    QScriptValue type();
    QScriptValue set_type(int type);
    void add_property(const QString &name, const QString& value);
    void remove_property(const QString &name);
    QScriptValue adj_data();
    QScriptValue adj_pointers();
    QScriptValue adj_pointers(int pointerType);
    QScriptValue input_pointers();
    QScriptValue input_pointers(int pointerType);
    QScriptValue output_pointers();
    QScriptValue output_pointers(int pointerType);
    QScriptValue connected_pointers(Data *n);

Q_SIGNALS:
    void removed();
    void posChanged(const QPointF &p);
    void widthChanged(double w);
    void colorChanged(const QColor &c);
    void visibilityChanged(bool visible);
    void useColorChanged(bool b);
    void dataTypeChanged(int dataType);
    void pointerListChanged();
    void propertyAdded(const QString &name);
    void propertyRemoved(const QString &name);
    void propertyChanged(const QString &name);

protected:
    /**
     * Protected constructor.
     */
    Data(DataStructurePtr dataStructure, int uniqueIdentifer, int dataType);

    /**
     * Factory method to create a data element of type \p T.
     */
    template<typename T>
    static DataPtr create(DataStructurePtr parent, int uniqueIdentifier, int dataType) {
        DataPtr pi(new T(parent, uniqueIdentifier, dataType));
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
    const QScopedPointer<DataPrivate> d;

    /**
     * \internal
     * Set q-pointer in private data object.
     */
    void setQpointer(DataPtr q);

    void initialize();
    Data(const Data&);
    Data& operator=(const Data&);
};

#endif
