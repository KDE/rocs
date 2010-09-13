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

DataType::DataType(DataTypeDocument *parent) : QObject(parent) {
    _directed = false;
    _automate = false;
    _readOnly = false;
    _document = parent;
    _begin = 0;
    calcRelativeCenter();
    _datumDefaultColor = "blue";
    _pointerDefaultColor = "gray";
    _datumNamesVisible = true;
    _datumValuesVisible = true;
    _pointerNamesVisible = false;
    _pointerValuesVisible = true;
}

DataType::DataType(DataType& other): QObject(other.parent()){
    _directed = other._directed;
    _automate = other._automate;
    _readOnly = other._readOnly;
    _document = other._document;
    _begin = other._begin;
    calcRelativeCenter();
    _datumDefaultColor = other._datumDefaultColor;
    _pointerDefaultColor = other._pointerDefaultColor;
    _datumNamesVisible = other._datumNamesVisible;
    _datumValuesVisible = other._datumValuesVisible;
    _pointerNamesVisible = other._pointerNamesVisible;
    _pointerValuesVisible = other._pointerValuesVisible;
    QHash <Datum*, Datum* > datumToDatum;
    foreach(Datum *n, other._data){
      Datum* newDatum = addDatum(n->name());
      newDatum->setColor(n->color());
      newDatum->setValue(n->value());
      newDatum->setX(n->x());
      newDatum->setY(n->y());
      newDatum->setWidth(n->width());
      datumToDatum.insert(n, newDatum);

    }
    foreach(Pointer *e, other._pointers){
      Datum* from =  datumToDatum.value(e->from());
      Datum* to =  datumToDatum.value(e->to());

      Pointer* newPointer = addPointer(from, to);
      newPointer->setColor(e->color());
      newPointer->setValue(e->value());
    }
}


DataType::~DataType() {
    foreach(Pointer* e,  _pointers) {
        remove(e);
    }

    foreach(Datum* n, _data) {
        remove(n);
    }
}

DataTypeDocument *DataType::document() const {
    return _document;
}

void DataType::remove() {
  _document->removeOne(this);
  delete this;
}

QList<Datum*> DataType::data() const {
    return _data;
}

QList<Pointer*> DataType::pointers() const {
    return _pointers;
}

void DataType::setDatumsColor(QString c){
  kDebug() << "Entrou no setDatumsColor, com a cor " << c;
  foreach(Datum *n, _data) {
        n->setColor(c);
    }
}

void DataType::setPointersColor(QString c){
    kDebug() << "Entrou no setPointersColor, com a cor " << c;
    foreach(Pointer *e, _pointers) {
        e->setColor(c);
    }
}

Datum* DataType::addDatum(QString name) {
    if (_readOnly) return 0;

    Datum  *n = new Datum(this);
    n->setName(name);
    return addDatum(n);
}

Datum* DataType::addDatum(Datum *datum){
    _data.append( datum );
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
    if (_readOnly) return 0;

    if ( from == 0 || to == 0 ) {      return 0;   }


    if ( ( from == to) && ( !_directed ) ) {
        return 0;
    } else if ((from->pointers(to).size() >= 1)&&(!_directed)) {
        return 0;
    } else if ((_data.indexOf(from) == -1) || (_data.indexOf(to) == -1)) {
        return 0;
    }

    Pointer *e  = new Pointer(this, from, to);
    _pointers.append( e );
    emit pointerCreated(e);
    connect (e, SIGNAL(changed()), this, SIGNAL(changed()));
    return e;
}

Pointer* DataType::addPointer(const QString& name_from, const QString& name_to) {
    if (_readOnly) return 0;
    Datum *from = 0;
    Datum *to   = 0;

    QString tmpName;

    foreach( Datum* n,  _data) {
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
    return _directed;
}

Datum* DataType::datum(const QString& name) {
    QString tmpName;
    foreach( Datum* n,  _data) {
        tmpName = n->name();
        if (tmpName == name) {
            return n;
        }
    }
    return 0;
}

void DataType::remove(Datum *n) {
    _data.removeOne( n  );
    n->deleteLater();
}

void DataType::remove(Pointer *e) {
    _pointers.removeOne( e );
    e->deleteLater();
//     delete e;
}

void DataType::setDirected(bool directed) {

    foreach(Datum *n1, _data) {
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
    foreach(Pointer *e, _pointers){
      e->emitChangedSignal(); // dummy updater.
    }
    _directed = directed;
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
	_relativeCenter.setY(gd->height()/2);
	_relativeCenter.setX(gd->width()/2);
    }else{
        _relativeCenter.setY(0);
	_relativeCenter.setX(0);
    }
}

QPointF DataType::relativeCenter() const {
    return _relativeCenter;
}

const QString& DataType::name() const {
    return _name;
}
void DataType::setName(const QString& s) {
    _name = s;
}

bool DataType::setBegin(Datum* n) {
  if (!n){
    _begin = 0;
    return false;
  }

  if (!_begin){
      _begin = n;
      return true;
  }else if( _begin == n){
    return false;
  }

   _begin->setBegin(false);
   _begin = n;
    return true;
}

Datum* DataType::begin() const {
    return _begin;
}

Datum* DataType::addEnd(Datum *n) {
    _ends.append(n);
    return n;
}

void DataType::removeEnd(Datum *n) {
    _ends.removeAll(n);
}

void DataType::setDatumDefaultColor(const QString& color) {
    kDebug() << "Entrou com cor aqui painho." << color;
    _datumDefaultColor = color;
}

const QString& DataType::datumDefaultColor() const {
    return _datumDefaultColor;
}

void DataType::setPointerDefaultColor(const QString& color) {
    _pointerDefaultColor = color;
}
const QString& DataType::pointerDefaultColor() const {
    return _pointerDefaultColor;
}

void DataType::setAutomate(bool b) {
    _automate = b;
    emit automateChanged(b);
}
bool DataType::automate() {
    return _automate;
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
    foreach(Datum *n, _data){
      n->addDynamicProperty(property, value);
    }
}

void DataType::addPointersDynamicProperty(QString property, QVariant value){
    foreach(Pointer *e, _pointers){
      e->addDynamicProperty(property, value);
    }
}

void DataType::removeDatumsDynamicProperty(QString property){
  foreach(Datum *n, _data){
    n->removeDynamicProperty(property);
  }
}
void DataType::removePointersDynamicProperty(QString property){
  foreach(Pointer *e, _pointers){
    e->removeDynamicProperty(property);
  }
}

void DataType::setDatumNameVisibility(bool b){
  _datumNamesVisible = b;
  foreach(Datum *n, _data){
    n->hideName(b);
  }
}

bool DataType::datumNameVisibility(){
  return _datumNamesVisible;
}

void DataType::setPointerNameVisibility(bool b){
  _pointerNamesVisible = b;
  foreach(Pointer *n, _pointers){
    n->hideName(b);
  }
}

bool DataType::pointerNameVisibility(){
  return _pointerNamesVisible;
}

void DataType::setDatumValueVisibility(bool b){
  _datumValuesVisible = b;
  foreach(Datum *n, _data){
    n->hideValue(b);
  }
}
bool DataType::datumValueVisibility(){
  return _datumValuesVisible;
}

void DataType::setPointerValueVisibility(bool b){
  _pointerValuesVisible = b;
  foreach(Pointer *n, _pointers){
    n->hideValue(b);
  }
}

bool DataType::pointerValueVisibility(){
  return _pointerValuesVisible;
}

#ifdef USING_QTSCRIPT




QScriptValue DataType::scriptValue() const {
    return _value;
}

void DataType::setEngine(	QScriptEngine *engine ) {
    _engine = engine;

    _value = _engine->newQObject(this);

    if (! _name.isEmpty() ) {
        _engine->globalObject().setProperty(_name, _value);
        kDebug() << _name << "Added as global object.";
    }

    foreach(Datum *n, _data) {
        n->setEngine(engine);
    }
    foreach(Pointer *e, _pointers) {
        e->setEngine(engine);
    }
//    foreach(GraphGroup *g, _graphGroups) {
//        QScriptValue array = _engine->newArray();
//        foreach(Datum* n, (*g) ) {
//            array.property("push").call(array, QScriptValueList() << n->scriptValue());
//        }
//        _engine->globalObject().setProperty(g->name(), array);
//    }
}


#endif
