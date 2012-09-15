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


#include "rocslib_export.h"
#include "Rocs_Typedefs.h"
#include "DataStructure.h"
#include "DataType.h"

#include <QObject>
#include <QString>
#include <QtScript>
#include <QColor>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

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
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(int id READ identifier)
    Q_PROPERTY(QVariant color READ color WRITE setColor)
    Q_PROPERTY(QVariant value READ value WRITE setValue)
    Q_PROPERTY(bool useColor READ useColor WRITE setUseColor)

public:
    virtual ~Data();
    enum ListType {In, Out, Self};

    static DataPtr create(DataStructurePtr parent, int uniqueIdentifier, int dataType);

    template<typename T>
    static DataPtr create(DataStructurePtr parent, int uniqueIdentifier, int dataType) {
        DataPtr pi(new T(parent, uniqueIdentifier, dataType));
        pi->setQpointer(pi);
        return pi;
    }

    virtual DataPtr getData() const;

    void addInPointer(PointerPtr e);
    void addOutPointer(PointerPtr e);
    void addSelfPointer(PointerPtr e);
    void removePointer(PointerPtr e, int pointerList = -1);
    void removePointer(PointerPtr e, PointerList &list);
    void remove();

    bool showName();
    bool showValue();

    DataStructurePtr dataStructure() const;

    QScriptValue scriptValue() const;
    virtual void setEngine(QScriptEngine *_engine);
    QScriptValue createScriptArray(PointerList list);

    //getters
    qreal x() const;
    qreal y() const;
    qreal width() const;
    const QVariant color() const;
    const QString& name() const;
    const QVariant value() const;
    QString icon() const;
    const QString& iconPackage() const;
    bool showName() const;
    bool showValue() const;
    bool isVisible() const;
    bool useColor() const;
    boost::shared_ptr<DataItem> item() const;   //TODO really needed?
    int identifier() const;
    int dataType() const;

    /**
     * Gives a list of adjacent data elements. Each adjacent
     * data element is contained exactly once in this list.
     * \return DataList with adjacent data
     */
    DataList adjacent_data() const;
    PointerList adjacent_pointers() const;
    PointerList& in_pointers() const;
    PointerList& out_pointers() const;
    PointerList& self_pointers() const;
    PointerList pointers(DataPtr n) const;

    PointerPtr addPointer(DataPtr to);

    void addDynamicProperty(QString property, QVariant value);
    void removeDynamicProperty(QString property);

public  slots:
    void self_remove();

    void setX(int x);
    void setY(int y);
    void setWidth(double w);
    void setPos(qreal x, qreal y);
    void setColor(const QVariant& s);
    void setName(const QString& s);
    void setValue(const QVariant& v);
    void setShowName(bool b);
    void setShowValue(bool b);
    void setUseColor(bool b = true);
    void setVisible(bool visible);
    void setValue(const QString& v);
    void setDataItem(boost::shared_ptr<DataItem> item);
    void setDataType(int dataType);

    QScriptValue type();
    QScriptValue set_type(int type);
    void add_property(QString name, QString value);
    QScriptValue adj_data();
    QScriptValue adj_pointers();
    QScriptValue input_pointers();
    QScriptValue output_pointers();
    QScriptValue loop_pointers();
    QScriptValue connected_pointers(DataPtr n);

protected:
    Data(DataStructurePtr parent, int uniqueIdentifer, int dataType);

private:
    boost::shared_ptr<DataPrivate> d;
    void setQpointer(DataPtr q);
    Data(Data const &, int uniqueIdentifer, int dataType);
    Data & operator=(Data const &);


signals:
    void removed();
    void posChanged(const QPointF p);
    void widthChanged(double w);
    void colorChanged(const QColor& c);
    void nameChanged(const QString& name);
    void valueChanged(const QVariant& v);
    void nameVisibilityChanged(bool b);
    void valueVisibilityChanged(bool b);
    void visibilityChanged(bool visible);
    void useColorChanged(bool b);
    void dataTypeChanged(int dataType);
};

#endif
