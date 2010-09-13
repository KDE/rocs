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

#ifndef POINTER_H
#define POINTER_H

#ifndef USING_QTSCRIPT
#define USING_QTSCRIPT 1
#endif

#include <QObject>
#include <QtScript>
#include <QString>

#ifdef USING_QTSCRIPT
#include <QScriptValue>
#include "qtScriptBackend.h"
#endif

#include "rocslib_export.h"

class Datum;
class PointerPrivate;

/**
* \class Pointer
*
* This class is an pointer on the graph. it holds the orientation, the from and the to datums,auto
* and has a name, value and color properties.
* you can change all properties by 'pointer.propertyName = new property' and access it's value
* by 'pointer.propertyName' .
* new properties can be added on the fly via pointer.addProp(propertyname) , and accessed by pointer.propertyName.
*/

class ROCSLIB_EXPORT Pointer : public QObject {
    Q_OBJECT

    /*! all properties are accessible from the scripting engine via .propertyName */

    /*! this property holds the color attribute. */
    Q_PROPERTY(QString color READ color WRITE setColor)

    /*! this property hold the value attribute */
    Q_PROPERTY(QString value READ value WRITE setValue)

    /*! this property holds the name of the pointer. */
    Q_PROPERTY(QString name READ name WRITE setName)

    /*! this property holds the width of the pointer */
    Q_PROPERTY(double width READ width WRITE setWidth)

    /*! this property holds the style of the pointer */
    Q_PROPERTY(QString style READ style WRITE setStyle)

public:
    /*! default constructor, an pointer connects two datums.
    \p parent a Graph
    \p from the first datum
    \p to the second datum */
    Pointer(DataType *parent, Datum *from, Datum *to);

    /*! default destructor */
    ~Pointer();

    /*! relative index is the index that this pointer has relative to the datums that it's bound to.
    eg. if the datums have 2 or more pointers connecteds between them, it will have a unique
    identifier relative to that datums

    \return the relativeIndex identifier.
    */
    int relativeIndex() const;

    /*! remove this datum from the graph */
    void remove(Datum* node = 0);

    /*! forces emitting a signal that will update this pointer on screen */
    void emitChangedSignal();

    /*! returns the datastructure that owns this pointer. */
    DataType *dataType() const;
    
#ifdef USING_QTSCRIPT
    /*! if the qtscript is enabled for this rocs,
      this method returns the self-referenced script value for this pointer.
      \return QScriptValue self reference for this datum.
      */
    QScriptValue scriptValue() const;

    /*! if qtscript is enabled for this rocs,
    this method will set the engine for this single object
    \p engine the QScriptEngine that will work on the object */
    void setEngine(	QScriptEngine *engine );
#endif

public  slots:
    /*! return the first datum of this pointer
      \return Datum* pointer for the first datum of this pointer.
    */
    Datum* from() const;

    /*! return the second datum of this pointer
      \return Datum* pointer for the second datum of this pointer.
    */
    Datum* to() const ;

    /*! return the value of this pointer
    \return the value of the pointer.
    */
    const QString& value() const;

    /*! sets the value attribute of this pointer
    \p s the new value of this pointer. */
    void setValue (const QString& s);

    /*! returns the name attribute of the pointer.
      \return the name of the pointer.
    */
    const QString& name() const;
    
    /*! sets the name attribute of the pointer
      \p s the new name of this pointer
    */
    void setName (const QString& s) ;

    /*! gets the color attribute of the pointer
      \return the string value of the pointer.
    */
    const QString& color() const ;

    /*! sets the color attribute of the pointer
      \p s the new color of the pointer in the format "#000000" or by it's english name ("red" for example)
    */
    void setColor(const QString& s);

    qreal width () const;
    void setWidth(double w);

    const QString& style() const;
    void setStyle(const QString& s);

    /** Add a property to this pointer
    * @param property Name of property
    * @param value Value of the property. value shoud be different of QVariant::Invalid.
    */
    void addDynamicProperty(QString Property, QVariant value);

    /** Remove property arg1 from this pointer. If property arg1 don't exist in this pointer, nothing is made.
    * @param arg1 name os property to remove
    */
    void removeDynamicProperty(QString property);

    bool showName();
    bool showValue();
    void hideName(bool b);
    void hideValue(bool b);

#ifdef USING_QTSCRIPT
    /*! this method can be used inside of the script interface.
    \return the first datum of this pointer.
    */
    QScriptValue start();

    /*! this method can be used inside of the script interface,
    \return the last datum of this pointer
    */
    QScriptValue end();

    /*! this method can be used inside of the script interface,
     it will remove this pointer from the graph.
     */
    void self_remove();

#endif
    
private:
  PointerPrivate * const d;

signals:
    /*! emmited when this pointer is removed. */
    void removed();
    /*! emmited when a datum connected to this pointer changes, or when this pointer changes. */
    void changed();
};

typedef QList<Pointer*> PointerList;
#endif
