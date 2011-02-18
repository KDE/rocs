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

#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QList>
#include <QString>

#include <QtScript>
#include "QtScriptBackend.h"
#include <QColor>

#include "DataItem.h"
#include "rocslib_export.h"
#include "Rocs_Typedefs.h"

class DataItem;
class Pointer;
class DataPrivate;
class DataStructure;

class  ROCSLIB_EXPORT Data : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)
    Q_PROPERTY(qreal width READ width WRITE setWidth)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(QVariant value READ value WRITE setValue)
    Q_PROPERTY(QString iconPackage READ iconPackage WRITE setIconPackage)
    Q_PROPERTY(QString icon READ icon WRITE setIcon)

public:
    enum ListType{In, Out, Self};
    
    Data(DataStructure *parent);
    ~Data();
    
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

    DataStructure *dataStructure() const;

    QScriptValue scriptValue() const;
    virtual void setEngine(	QScriptEngine *_engine );
    QScriptValue createScriptArray(PointerList list);
    
        //setters
    void setX(int x);
    void setY(int y);
    void setWidth(qreal w);
    void setPos(qreal x, qreal y);
    void setColor(const QColor& s);
    void setName(const QString& s);
    void setValue(const QVariant& v);
    void setIcon(const QString& s);
    void setIconPackage(const QString& s);
    void setShowName(bool b);
    void setShowValue(bool b);
    
    //getters
    qreal x() const;
    qreal y() const;
    qreal width() const;
    const QColor& color() const;
    const QString& name() const;
    const QVariant value() const;
    const QString& icon() const;
    const QString& iconPackage() const;
    bool showName() const;
    bool showValue() const;
    
    DataItem *item() const;
    
    DataList adjacent_data() const;
    PointerList adjacent_pointers() const;
    PointerList& in_pointers() const;
    PointerList& out_pointers() const;
    PointerList& self_pointers() const;
    PointerList pointers(Data *n) const;


    void addDynamicProperty(QString property, QVariant value);
    void removeDynamicProperty(QString property);

public  slots:
    QScriptValue adj_data();
    QScriptValue adj_pointers();
    QScriptValue input_pointers();
    QScriptValue output_pointers();
    QScriptValue loop_pointers();
    QScriptValue connected_pointers(Data *n);
    void self_remove();
    Pointer* addPointer(Data* to);

private:
    DataPrivate *d;

signals:
    void removed();
    void changed();
};

class DataPrivate{
public:
  DataPrivate(Data *classPtr) : q(classPtr){}
  
    PointerList _in_pointers;
    PointerList _out_pointers;
    PointerList _self_pointers;

    qreal _x;
    qreal _y;
    qreal _width;

    bool _begin;
    bool _end;
    bool _changing;
    bool _showName;
    bool _showValue;

    DataStructure *_dataStructure;
    DataItem *_item;
    
    QString _name;
    QColor _color;
    QString _iconpackage;
    QString _icon;

    QVariant _value;  
    QScriptValue _scriptvalue;
    QScriptEngine *_engine; 
    
    void empty(PointerList &list) ;
private:
  Data *q;
};

inline const QVariant Data::value() const { return d->_value; }
inline const QString& Data::name()  const { return d->_name;  }
inline const QColor&  Data::color() const { return d->_color; }

inline DataItem* Data::item() const { return d->_item; }

inline qreal Data::x() const { return d->_x; }
inline qreal Data::y() const { return d->_y; }

inline qreal Data::width() const { return d->_width; }

inline const QString& Data::icon() const { return d->_icon; }
inline const QString& Data::iconPackage() const {  return d-> _iconpackage;  }

inline PointerList& Data::in_pointers()   const { return d->_in_pointers;   }
inline PointerList& Data::out_pointers()  const { return d->_out_pointers;  }
inline PointerList& Data::self_pointers() const { return d->_self_pointers; }

inline bool Data::showName() const { return d->_showName; }
inline bool Data::showValue() const { return d->_showValue; }
#endif
