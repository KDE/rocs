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
#ifndef GRAPH_H
#define GRAPH_H

#ifndef USING_QTSCRIPT
#define USING_QTSCRIPT 1
#endif

#include <QObject>
#include <QList>
#include <QString>

#ifdef USING_QTSCRIPT
#include <QtScript>
#endif

#include "rocs_typedefs.h"
#include <klocalizedstring.h>
#include "rocslib_export.h"

class QtScriptBackend;
//class GraphGroup;
class DataTypeDocument;
class Datum;
class Pointer;


/*! \brief this class acts as a container for datums and pointers.
  this class has all the pointers and datums, plus a few extra funcionalities
  to help programming with it on the programming backend. */
class ROCSLIB_EXPORT DataType : public QObject {
    Q_OBJECT

    /*! this property defines if the graph is directed or simple */
    Q_PROPERTY(bool directed READ directed WRITE setDirected)

    /*! this property holds the name of the graph */
    Q_PROPERTY(QString name READ name WRITE setName)

    /*! this property holds the default color for new datums */
    Q_PROPERTY(QString datumDefaultColor READ datumDefaultColor WRITE setDatumDefaultColor)

    /*! this property holds the default color for new datums */
    Q_PROPERTY(QString pointerDefaultColor READ pointerDefaultColor WRITE setPointerDefaultColor)

    /*! this property tells the system if this graph is to be treated as an automate */
    Q_PROPERTY(bool automate READ automate WRITE setAutomate)

public:
    /*! constructo
      \p parent the parent QObject
    */
    DataType(DataTypeDocument *parent = 0);

    DataType(DataType& other);

    /*! destructor */
    ~DataType();

    /*! calculates the relative center of the graph,
      taking into account the top, bottom, left and right pointers. */
    void calcRelativeCenter();

    /*! gets the precalculated relative center.
      \return the center of the graph.
      */
    QPointF relativeCenter() const;

    /*! gets the document that holds this graph.
      \return the GraphDocument
    */
    DataTypeDocument *document() const;

    void readOnly( bool r) { _readOnly = r; }
    bool readOnly() const { return _readOnly; }

#ifdef USING_QTSCRIPT
    /*! if QtScript is enabled for this rocs,
    \return  the self-referenced Script Value for this graph. */
    QScriptValue scriptValue() const;

    /*! if QtScript is enabled for this rocs,
    \p engine is the QtScript engine that will be set for this graph. */
    virtual void setEngine( QScriptEngine *engine );
#endif

public  slots:
    /*! sets the name of the graph.
    \p s the new name of the graph. */
    void setName(const QString& s);

    /*! gets the name of the graph.
    \return the name of the graph */
    const QString& name() const;

    /*! sets if the graph is directed or simple datum
    \param directed true directed, false simple. FIXME: change that to enum.
    */
    void setDirected(bool directed = true);

    /*! gets if the datum is diirected of simple.
    \return true if directed, false if simple. FIXME: change that to enum.
    */
    bool directed() const;

    /*! all data are accessible from that method. 
      \return the QList containing all the  datums.
    */
    DataList data() const;


    /*! all pointers are accessible from that method.
      \return the QList containing all the pointers.
    */
    PointerList pointers() const;


    /*! all groups are accessible from that method.
      \return the QList containing all the  groups.
    */
    //QList<GraphGroup*> groups() const;

    /*! get the datum marked as 'begin'
    \return the 'begin' datum. (datum.begin == true), NULL if there isn't a begin datum.
    */
    Datum* begin() const;

    /*! sets a datum as begin.
      \param b the new begin of the graph.
    */
    bool setBegin(Datum* b);

    /*! creates a new datum.
    \p name the name of the datum.
    \return the newly created datum.
    */
    virtual Datum* addDatum(QString name);

    /*! creates a new pointer
    \p from the 'from' datum.
    \p to the 'to' datum.
    \return the newly created pointer.
    */
    virtual Pointer* addPointer(Datum* from, Datum* to);

    /*! creates a new pointer
    \p name_from the name of the datum that will be used as 'from'
    \p name_to the name of the datum that will be used as 'to'
    \return the newly created pointer.
    */
    Pointer* addPointer(const QString& name_from, const QString& name_to);

    /*! returns a datum
    \p name the name of the datum to be returned.
    \return a datum, or null if an invalid name is provided.
    */
    Datum* datum(const QString& name = i18n("Untitled"));

    /*! removes a datum.
    \p n the datum to be removed.
    */
    void remove(Datum *n);

    /*! removes an pointer
    \p e the pointer to be removed.
    */
    void remove(Pointer *e);

    /*! adds a graph - group to the graph. FIXME: not implemented yet.
    \p name the name of the group.
    \return the newly created graph group.
    */
  //  GraphGroup *addGroup(const QString& name);

    /*! \brief add a new datum to the 'end' list.
    end list is an QList that will hold every datum as a final value.
    \p n the datum to be added.
    \return the recently added datum.
    */
    Datum* addEnd(Datum* n);

    /*! remove a datum from the end's list.
    \p n the datum to be removed from the list.
    */
    void  removeEnd(Datum* n);

    /*! sets the default color for new added datums.
    \p color the new color.
    */
    void setDatumDefaultColor(const QString& color);

    /*! gets the default datum color for this graph.
    \return a QColor.
    */
    const QString& datumDefaultColor() const;

    /*! sets the default color for new added pointers.
    \p color the new color.http://imagebin.ca/view/VoxkFg.html
    */
    void setPointerDefaultColor(const QString& color);

    /*! gets the default pointer color for this graph
    \return a QColor
    */
    const QString& pointerDefaultColor() const;

    /*! sets this graph an automate
      \param b true = automate, false = graph.
    */
    void setAutomate(bool b);

    /*! boolean that represents if the graph is an automate or not
      \return true = automate, false = graph.
    */
    bool automate();

       /** Add a property to this graph.
    * @param property Name of property
    * @param value Value of the property. value shoud be different of QVariant::Invalid.
    */
    void addDynamicProperty(QString property, QVariant value = QVariant(0));
    /** remove a property from this graph.
    * @param property Name of property
    */
    void removeDynamicProperty(QString property);

    /** Add a property to all datums in this graph. If a new datum is add later, the datum will not containg this property.
    * @param property Name of property
    * @param value Value of the property. value shoud be different of QVariant::Invalid.
    */
    void addDatumsDynamicProperty(QString property, QVariant value = QVariant(0));

    /** remove a property to all datums in this graph.
    * @param property Name of property
    */
    void addPointersDynamicProperty(QString property, QVariant value = QVariant(0));

     /** Add a property to all Pointers in this graph. If a new pointer is add later, the pointer will not containg this property.
    * @param property Name of property
    * @param value Value of the property. value shoud be different of QVariant::Invalid.
    */
    void removeDatumsDynamicProperty(QString property);

    /** remove a property to all pointers in this graph.
    * @param property Name of property
    */
    void removePointersDynamicProperty(QString property);

    /** adds a datum in this graph.
    * @param name the name of the datum,
    * @param point the point in the euclidian space where this datum is.
    */
    Datum* addDatum(QString name, QPointF point);

    /** sets all datums for this color.
    * @param color the color that each datum will have.
    */
    void setDatumsColor(QString c);

    /** sets all pointers for this color
    * @param color the color that each pointer will have.
    */
    void setPointersColor(QString c);

    /** puts sets the visitbility of the names in the datums.
    * @param b true if visible false invisible */
    void setDatumNameVisibility(bool b);
    bool datumNameVisibility();

    void setPointerNameVisibility(bool b);
    bool pointerNameVisibility();

    void setDatumValueVisibility(bool b);
    bool datumValueVisibility();

    void setPointerValueVisibility(bool b);
    bool pointerValueVisibility();

    /*! remove this graph from the document. */
    void remove();

#ifdef USING_QTSCRIPT
//     QScriptValue list_data();
//     QScriptValue list_pointers();
//     QScriptValue add_datum(const QString& name);
//     QScriptValue add_pointer(Datum* from, Datum* to);
//     QScriptValue datum_byname(const QString& name);
//     QScriptValue begin_datum();
//     QScriptValue end_data();
#endif

signals:
    void datumCreated(Datum *n);
    void pointerCreated(Pointer *e);
    void complexityChanged(bool directed);
    void changed();
    void orientedChanged(bool b);
    void automateChanged(bool b);

protected:
    Datum* addDatum(Datum *datum);
    
    bool _directed;
    
    DataList _data;
    PointerList _pointers;
    
    //QList<GraphGroup*> _graphGroups;
    qreal _top;
    qreal _bottom;
    qreal _left;
    qreal _right;

    QPointF _relativeCenter;
    QString _name;
    Datum* _begin;
    DataList _ends;
    QString _datumDefaultColor;
    QString _pointerDefaultColor;
    bool _automate;
    DataTypeDocument *_document;
    bool _readOnly;

    bool _datumNamesVisible;
    bool _pointerNamesVisible;
    bool _datumValuesVisible;
    bool _pointerValuesVisible;

#ifdef USING_QTSCRIPT
  protected:
    QScriptValue _value;
    QScriptEngine *_engine;
#endif

};

#endif
