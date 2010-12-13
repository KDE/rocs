/***************************************************************************
 * main.cpp
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/

#ifndef NODE_H
#define NODE_H

#ifndef USING_QTSCRIPT
#define USING_QTSCRIPT 1
#endif

#include <QObject>
#include <QList>
#include <QString>

#include <QtScript>
#include "qtScriptBackend.h"

#include "rocslib_export.h"
#include "rocs_typedefs.h"

class Pointer;
class Datum;
class DatumPrivate;

class  ROCSLIB_EXPORT Datum : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)
    Q_PROPERTY(qreal width READ width WRITE setWidth)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString color READ color WRITE setColor)
    Q_PROPERTY(bool begin READ begin WRITE setBegin)
    Q_PROPERTY(bool end READ end WRITE setEnd)
    Q_PROPERTY(QVariant value READ value WRITE setValue)
    Q_PROPERTY(QString iconPackage READ iconPackage WRITE setIconPackage)
    Q_PROPERTY(QString icon READ icon WRITE setIcon)

public:
    enum PointerLists {In, Out, Self};
    
    Datum(DataType *parent);
    ~Datum();
    
    void addInPointer(Pointer *e);
    void addOutPointer(Pointer *e);
    void addSelfPointer(Pointer *e);
    void removePointer(Pointer *e, int pointerList);
    void removePointer(Pointer *e, PointerList &list);
    void remove();
    
    void startChange();
    void endChange();
    bool showName();
    bool showValue();

    DataType *dataType() const;

    QScriptValue scriptValue() const;
    virtual void setEngine(	QScriptEngine *_engine );
    QScriptValue createScriptArray(PointerList list);

public  slots:
    DataList adjacent_data() const;
    PointerList adjacent_pointers() const;
    PointerList pointers(Datum *n);
    PointerList in_pointers() const;
    PointerList out_pointers() const;
    PointerList self_pointers() const;

    void setX(int x);
    void setY(int y);
    void setWidth(qreal w);
    void setPos(qreal x, qreal y);
    qreal x() const;
    qreal y() const;
    qreal width() const;
    void setColor(const QString& s);
    const QString& color() const;
    void setName(const QString& s);
    const QString& name() const;
    bool begin() const;
    bool end() const;
    void setBegin(bool begin = true);
    void setEnd(bool end = true);
    void setValue(const QVariant v);
    void setValue(const QString& s);
    const QVariant value() const;
    void setIcon(const QString& s);
    const QString& icon() const;
    const QString& iconPackage() const;
    void setIconPackage(const QString& s);
    void hideName(bool b);
    void hideValue(bool b);
    /** Add a property to this node
    * @param property Name of property
    * @param value Value of the property. arg2 shoud be different of QVariant::Invalid.
    */
    void addDynamicProperty(QString property, QVariant value);

    /** Remove property arg1 from this node. If property arg1 don't exist in this node, nothing is made.
    * @param property name os property to remove
    */
    void removeDynamicProperty(QString property);

    QScriptValue adj_data();
    QScriptValue adj_pointers();
    QScriptValue input_pointers();
    QScriptValue output_pointers();
    QScriptValue loop_pointers();
    QScriptValue connected_pointers(Datum *n);
    void self_remove();

    Pointer* addPointer(Datum* to);

private:
    DatumPrivate *d;

signals:
    void removed();
    void changed();
};


#endif
