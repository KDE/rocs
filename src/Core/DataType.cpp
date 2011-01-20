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

#include "DataType.h"
#include "Data.h"
#include "Pointer.h"
#include "qtScriptBackend.h"
//#include "graphGroups.h"
#include "graphDocument.h"
#include "DynamicPropertiesList.h"
#include <KDebug>
#include <QColor>

class DataTypePrivate{
public:
    DataTypePrivate(){}
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

    QScriptValue _value;
    QScriptEngine *_engine;
};

DataType::DataType(DataTypeDocument *parent) : QObject(parent), d(new DataTypePrivate){
    d->_directed = false;
    d->_automate = false;
    d->_readOnly = false;
    d->_document = parent;
    d->_begin = 0;
    calcRelativeCenter();
    d->_datumDefaultColor = "blue";
    d->_pointerDefaultColor = "gray";
    d->_datumNamesVisible = true;
    d->_datumValuesVisible = true;
    d->_pointerNamesVisible = false;
    d->_pointerValuesVisible = true;
}

DataType::DataType(DataType& other): QObject(other.parent()),d(new DataTypePrivate){
    d->_directed = other.directed();
    d->_automate = other.automate();
    d->_readOnly = other.readOnly();
    d->_document = other.document();
    d->_begin = other.begin();
    calcRelativeCenter();

    d->_pointerDefaultColor     = other.pointerDefaultColor();
    d->_datumDefaultColor       = other.datumDefaultColor();
    d->_datumNamesVisible       = other.d->_datumNamesVisible;
    d->_datumValuesVisible      = other.d->_datumValuesVisible;
    d->_pointerNamesVisible     = other.d->_pointerNamesVisible;
    d->_pointerValuesVisible    = other.d->_pointerValuesVisible;

    QHash <Datum*, Datum* > datumToDatum;
    foreach(Datum *n, other.data()){
      Datum* newDatum = addDatum(n->name());
      newDatum->setColor(n->color());
      newDatum->setValue(n->value());
      newDatum->setX(n->x());
      newDatum->setY(n->y());
      newDatum->setWidth(n->width());
      datumToDatum.insert(n, newDatum);

    }
    foreach(Pointer *e, other.d->_pointers){
      Datum* from =  datumToDatum.value(e->from());
      Datum* to =  datumToDatum.value(e->to());

      Pointer* newPointer = addPointer(from, to);
      newPointer->setColor(e->color());
      newPointer->setValue(e->value());
    }
}


DataType::~DataType() {
    foreach(Pointer* e,  d->_pointers) {
        remove(e);
    }

    foreach(Datum* n, d->_data) {
        remove(n);
    }
    delete d;
}

void DataType::setReadOnly(bool r){
    d->_readOnly = r;
}

bool DataType::readOnly() const{
  return d->_readOnly;
}

DataTypeDocument *DataType::document() const {
    return d->_document;
}

void DataType::remove() {
  d->_document->removeOne(this);
  delete this;
}

QList<Datum*> DataType::data() const {
    return d->_data;
}

QList<Pointer*> DataType::pointers() const {
    return d->_pointers;
}

void DataType::setDatumsColor(QString c){
  kDebug() << "Entrou no setDatumsColor, com a cor " << c;
  foreach(Datum *n, d->_data) {
        n->setColor(c);
    }
}

void DataType::setPointersColor(QString c){
    kDebug() << "Entrou no setPointersColor, com a cor " << c;
    foreach(Pointer *e, d->_pointers) {
        e->setColor(c);
    }
}

Datum* DataType::addDatum(QString name) {
    if (d->_readOnly) return 0;

    Datum  *n = new Datum(this);
    n->setName(name);
    return addDatum(n);
}

Datum* DataType::addDatum(Datum *datum){
    d->_data.append( datum );
    emit datumCreated( datum );
    connect ( datum, SIGNAL(changed()), this, SIGNAL(changed()));
    return datum;
}

Datum* DataType::addDatum(QString name, QPointF pos){
    if (Datum *datum = addDatum(name)){
        datum->setPos(pos.x(), pos.y());
        return datum;
    }
    return 0;
}

Pointer* DataType::addPointer(Datum* from,Datum* to) {
    if (d->_readOnly) return 0;

    if ( from == 0 || to == 0 ) {      return 0;   }


    if ( ( from == to) && ( !d->_directed ) ) {
        return 0;
    } else if ((from->pointers(to).size() >= 1)&&(!d->_directed)) {
        return 0;
    } else if ((d->_data.indexOf(from) == -1) || (d->_data.indexOf(to) == -1)) {
        return 0;
    }

    Pointer *e  = new Pointer(this, from, to);
    d->_pointers.append( e );
    emit pointerCreated(e);
    connect (e, SIGNAL(changed()), this, SIGNAL(changed()));
    return e;
}

Pointer* DataType::addPointer(const QString& name_from, const QString& name_to) {
    if (d->_readOnly) return 0;
    Datum *from = 0;
    Datum *to   = 0;

    QString tmpName;

    foreach( Datum* n,  d->_data) {
        tmpName = n->name();

        if (tmpName == name_from) {
            from = n;
        }
        if (tmpName == name_to) {
            to = n;
        }
        if ((to != 0) && (from != 0)) {
            break;
        }
    }

    return addPointer(from, to);
}

bool DataType::directed() const {
    return d->_directed;
}

Datum* DataType::datum(const QString& name) {
    QString tmpName;
    foreach( Datum* n,  d->_data) {
        tmpName = n->name();
        if (tmpName == name) {
            return n;
        }
    }
    return 0;
}

void DataType::remove(Datum *n) {
    if (begin() == n){
      setBegin(0);
    }
    foreach (Pointer *p, n->adjacent_pointers()){
        remove(p);
    }
    d->_data.removeOne( n  );
    n->deleteLater();
}

void DataType::remove(Pointer *e) {
    d->_pointers.removeOne( e );
    e->deleteLater();
//     delete e;
}

void DataType::setDirected(bool directed) {

    foreach(Datum *n1, d->_data) {
        foreach(Datum *n2, n1->adjacent_data()) {
	    // do not permit loop datums while changing graph's state.
            if ( (n1->pointers(n2).size() == 1) && (n1 != n2) ) {
                continue;
            }

            QList<Pointer*> listPointers = n1->pointers(n2);
            if (n1 != n2) {
                listPointers.removeFirst();
            }

            foreach(Pointer *e, listPointers) {
	      remove(e);
            }
        }
    }
    foreach(Pointer *e, d->_pointers){
      e->emitChangedSignal(); // dummy updater.
    }
    d->_directed = directed;
    emit complexityChanged(directed);
    return;
}

// GraphGroup* DataType::addGroup(const QString& name) {
//     GraphGroup *gg = new GraphGroup();
//     gg->setName(name);
//     return gg;
// }

//QList<GraphGroup*> Graph::groups() const {
//    return _graphGroups;
//}

void DataType::calcRelativeCenter() {
    /*
      _top = _data[0]->y();   _bottom = _data[0]->y();
      _left = _data[0]->x(); _right = _data[0]->x();
      kDebug() << "INIT:";
      kDebug() << "TOP : " << _top << " BOTTOM " << _bottom << "LEFT" << _left << "RIGHT" << _right;
      for (int counter = 0; counter < _data.size(); counter++) {
       if(_data[counter]->x() > _right)  _right = _data[counter]->x();
       if(_data[counter]->y() > _top)  _top = _data[counter]->y();
       if(_data[counter]->x() < _left)  _left = _data[counter]->x();
       if(_data[counter]->y() < _bottom)  _bottom = _data[counter]->y();
           kDebug() << "TOP : " << _top << " BOTTOM " << _bottom << "LEFT" << _left << "RIGHT" << _right;
      }
      */
    /// this will be here till I find a better way to calculate a *relative* center of the graph, and not the center of the document.
    if (parent() != 0){
        DataTypeDocument *gd = qobject_cast<DataTypeDocument*>(parent());
        d->_relativeCenter.setY(gd->height()/2);
        d->_relativeCenter.setX(gd->width()/2);
    }else{
        d->_relativeCenter.setY(0);
        d->_relativeCenter.setX(0);
    }
}

QPointF DataType::relativeCenter() const {
    return d->_relativeCenter;
}

const QString& DataType::name() const {
    return d->_name;
}
void DataType::setName(const QString& s) {
    d->_name = s;
}

bool DataType::setBegin(Datum* n) {
  if (!n){
    d->_begin = 0;
    return false;
  }

  if (!d->_begin){
      d->_begin = n;
      return true;
  }else if( d->_begin == n){
    return false;
  }

   d->_begin->setBegin(false);
   d->_begin = n;
    return true;
}

Datum* DataType::begin() const {
    return d->_begin;
}

Datum* DataType::addEnd(Datum *n) {
   d-> _ends.append(n);
    return n;
}

void DataType::removeEnd(Datum *n) {
    d->_ends.removeAll(n);
}

void DataType::setDatumDefaultColor(const QString& color) {
    kDebug() << "Entrou com cor aqui painho." << color;
    d->_datumDefaultColor = color;
}

const QString& DataType::datumDefaultColor() const {
    return d->_datumDefaultColor;
}

void DataType::setPointerDefaultColor(const QString& color) {
    d->_pointerDefaultColor = color;
}
const QString& DataType::pointerDefaultColor() const {
    return d->_pointerDefaultColor;
}

void DataType::setAutomate(bool b) {
    d->_automate = b;
    emit automateChanged(b);
}
bool DataType::automate() {
    return d->_automate;
}

void DataType::addDynamicProperty(QString property, QVariant value){
    if ( !setProperty(property.toUtf8(), value) && value.isValid()){
      DynamicPropertiesList::New()->addProperty(this, property);
    }
}

void DataType::removeDynamicProperty(QString property){
    this->addDynamicProperty(property, QVariant::Invalid);
    DynamicPropertiesList::New()->removeProperty(this, property);
}

void DataType::addDatumsDynamicProperty(QString property, QVariant value){
    foreach(Datum *n, d->_data){
      n->addDynamicProperty(property, value);
    }
}

void DataType::addPointersDynamicProperty(QString property, QVariant value){
    foreach(Pointer *e, d->_pointers){
      e->addDynamicProperty(property, value);
    }
}

void DataType::removeDatumsDynamicProperty(QString property){
  foreach(Datum *n, d->_data){
    n->removeDynamicProperty(property);
  }
}
void DataType::removePointersDynamicProperty(QString property){
  foreach(Pointer *e, d->_pointers){
    e->removeDynamicProperty(property);
  }
}

void DataType::setDatumNameVisibility(bool b){
  d->_datumNamesVisible = b;
  foreach(Datum *n, d->_data){
    n->hideName(b);
  }
}

bool DataType::datumNameVisibility(){
  return d-> _datumNamesVisible;
}

void DataType::setPointerNameVisibility(bool b){
  d->_pointerNamesVisible = b;
  foreach(Pointer *n, d->_pointers){
    n->hideName(b);
  }
}

bool DataType::pointerNameVisibility(){
  return d->_pointerNamesVisible;
}

void DataType::setDatumValueVisibility(bool b){
 d-> _datumValuesVisible = b;
  foreach(Datum *n, d->_data){
    n->hideValue(b);
  }
}
bool DataType::datumValueVisibility(){
  return d->_datumValuesVisible;
}

void DataType::setPointerValueVisibility(bool b){
  d->_pointerValuesVisible = b;
  foreach(Pointer *n, d->_pointers){
    n->hideValue(b);
  }
}

bool DataType::pointerValueVisibility(){
  return d->_pointerValuesVisible;
}

QScriptValue DataType::scriptValue() const {
    return d->_value;
}

QScriptEngine *DataType::engine() const{
  return d->_engine;
}

void DataType::setEngine(	QScriptEngine *engine ) {
   d-> _engine = engine;

    d->_value = d->_engine->newQObject(this);

    if (! d->_name.isEmpty() ) {
        d->_engine->globalObject().setProperty(d->_name, d->_value);
        kDebug() << d->_name << "Added as global object.";
    }

    for( int i = 0; i < d->_data.size(); ++i){
        d->_data.at(i)->setEngine(engine);
    }
    for( int i = 0; i < d->_pointers.size(); ++i){
        d->_pointers.at(i)->setEngine(engine);
    }

//    foreach(GraphGroup *g, _graphGroups) {
//        QScriptValue array = _engine->newArray();
//        foreach(Datum* n, (*g) ) {
//            array.property("push").call(array, QScriptValueList() << n->scriptValue());
//        }
//        _engine->globalObject().setProperty(g->name(), array);
//    }
}
