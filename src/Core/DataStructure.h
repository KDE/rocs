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
#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <QObject>
#include <QList>
#include <QString>

#include <QtScript>
#include <QColor>

#include "Rocs_Typedefs.h"
#include <klocalizedstring.h>
#include "rocslib_export.h"

class Group;
class Document;
class Data;
class Pointer;
class DataStructurePrivate;

class ROCSLIB_EXPORT DataStructure : public QObject {
    Q_OBJECT


    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QColor dataDefaultColor READ dataDefaultColor WRITE setDataDefaultColor)
    Q_PROPERTY(QColor pointerDefaultColor READ pointerDefaultColor WRITE setPointerDefaultColor)

public:
    DataStructure(Document *parent = 0);
    DataStructure(DataStructure& other, Document* parent);
    ~DataStructure();

    void calcRelativeCenter();
    QPointF relativeCenter() const;
    Document *document() const;

    void setReadOnly( bool r) ;
    bool readOnly() const ;

    QScriptValue scriptValue() const;
    void setEngine( QScriptEngine *engine );
    QScriptEngine *engine() const;

    // getters
    bool dataNameVisibility() const;
    bool pointerNameVisibility() const;
    bool dataValueVisibility() const;
    bool pointerValueVisibility() const;
    const QColor& pointerDefaultColor() const;
    const QColor& dataDefaultColor() const;
    const QString& name() const;
    const QList<Data*> dataList() const;
    const QList<Pointer*> pointers() const;
    const QList<Group*> groups() const;

protected:

public  slots:

    virtual Data* addData(QString name);
    virtual Pointer* addPointer(Data* from, Data* to);
    Data* data(const QString& name = i18n("Untitled"));

    void remove(Data *n);
    void remove(Pointer *e);
    void remove(Group *g);

    Group *addGroup(const QString& name);
    Data* addData(QString name, QPointF point);
    Pointer* addPointer(const QString& name_from, const QString& name_to);

    void addDynamicProperty(const QString& property, QVariant value = QVariant(0));
    void removeDynamicProperty(const QString& property);

    void addDataDynamicProperty(const QString& property, QVariant value = QVariant(0));
    void addPointersDynamicProperty(const QString& property, QVariant value = QVariant(0));

    void removeDataDynamicProperty(const QString& property);
    void removePointersDynamicProperty(const QString& property);

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

#ifdef USING_QTSCRIPT
//     QScriptValue list_data();
//     QScriptValue list_pointers();
//     QScriptValue add_data(const QString& name);
//     QScriptValue add_pointer(Data* from, Data* to);
//     QScriptValue data_byname(const QString& name);
//     QScriptValue begin_data();
//     QScriptValue end_data();
#endif

signals:
    void dataCreated(Data *n);
    void pointerCreated(Pointer *e);
    void complexityChanged(bool directed);
    void changed();

protected:
    Data* addData(Data *data);
    Pointer* addPointer(Pointer *pointer);

private:
    DataStructurePrivate *d;
};

class DataStructurePrivate{
public:
    DataStructurePrivate(){}

    QList<Data*> _data;
    QList<Pointer*> _pointers;

    QList<Group*> _groups;
    qreal _top;
    qreal _bottom;
    qreal _left;
    qreal _right;

    QPointF _relativeCenter;
    QString _name;
    Data* _begin;
    DataList _ends;
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
};

inline bool DataStructure::readOnly()                      const { return d->_readOnly;              }
inline bool DataStructure::dataValueVisibility()           const { return d->_dataValuesVisible;     }
inline bool DataStructure::dataNameVisibility()            const { return d-> _dataNamesVisible;     }
inline bool DataStructure::pointerNameVisibility()         const { return d->_pointerNamesVisible;   }
inline bool DataStructure::pointerValueVisibility()        const { return d->_pointerValuesVisible;  }

inline const QColor& DataStructure::pointerDefaultColor()  const { return d->_pointerDefaultColor; }
inline const QColor& DataStructure::dataDefaultColor()     const { return d->_dataDefaultColor;    }
inline const QString& DataStructure::name()                const { return d->_name;                }
inline QPointF DataStructure::relativeCenter()             const { return d->_relativeCenter;      }

inline QScriptValue DataStructure::scriptValue()           const { return d->_value;    }
inline QScriptEngine *DataStructure::engine()              const { return d->_engine;   }
inline Document *DataStructure::document()                 const { return d->_document; }

inline const QList<Data*>    DataStructure::dataList()     const { return d->_data;     }
inline const QList<Pointer*> DataStructure::pointers()     const { return d->_pointers; }
inline const QList<Group*>   DataStructure::groups()       const { return d->_groups;   }

#endif
