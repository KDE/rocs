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

#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"
#include "QtScriptBackend.h"
#include "Group.h"
#include "Document.h"
#include "DynamicPropertiesList.h"
#include "ConcurrentHelpClasses.h"

#include <KDebug>
#include <QColor>

DataStructure::DataStructure(Document *parent) : QObject(parent), d(new DataStructurePrivate){
    d->_directed = false;
    d->_automate = false;
    d->_readOnly = false;
    d->_document = parent;
    d->_begin = 0;
    calcRelativeCenter();
    d->_dataDefaultColor = QColor("blue");
    d->_pointerDefaultColor = QColor("gray");
    d->_dataNamesVisible = true;
    d->_dataValuesVisible = true;
    d->_pointerNamesVisible = false;
    d->_pointerValuesVisible = true;
}

DataStructure::DataStructure(DataStructure& other, Document * parent): QObject(parent), d(new DataStructurePrivate){
    d->_readOnly = other.readOnly();
    d->_document = other.document();
    calcRelativeCenter();

    d->_pointerDefaultColor     = other.pointerDefaultColor();
    d->_dataDefaultColor       =  other.dataDefaultColor();
    d->_dataNamesVisible       = other.d->_dataNamesVisible;
    d->_dataValuesVisible      = other.d->_dataValuesVisible;
    d->_pointerNamesVisible     = other.d->_pointerNamesVisible;
    d->_pointerValuesVisible    = other.d->_pointerValuesVisible;

    QHash <Data*, Data* > dataTodata;
    foreach(Data* n, other.d->_data){
      Data* newdata = addData(n->name());
      newdata->setColor(n->color());
      newdata->setValue(n->value());
      newdata->setX(n->x());
      newdata->setY(n->y());
      newdata->setWidth(n->width());
      dataTodata.insert(n, newdata);

    }
    foreach(Pointer *e, other.d->_pointers){
      Data* from =  dataTodata.value(e->from());
      Data* to =  dataTodata.value(e->to());

      Pointer* newPointer = addPointer(from, to);
      newPointer->setColor(e->color());
      newPointer->setValue(e->value());
    }
}


DataStructure::~DataStructure() {
    foreach(Pointer* e,  d->_pointers) {
        remove(e);
    }
    foreach(Data* n, d->_data) {
        remove(n);
    }
    delete d;
}

void DataStructure::setReadOnly(bool r){
    d->_readOnly = r;
}

void DataStructure::remove() {
  d->_document->remove(this);
  deleteLater();
}

Data* DataStructure::addData(QString name) {
    if (d->_readOnly) return 0;

    Data *n = new Data(this);
    n->setName(name);
    return addData(n);
}

Data* DataStructure::addData(Data *data){
    d->_data.append( data );
    emit dataCreated( data );
    connect ( data, SIGNAL(changed()), this, SIGNAL(changed()));
    return data;
}

Data* DataStructure::addData(QString name, QPointF pos){
    if (Data *data = addData(name)){
        data->setPos(pos.x(), pos.y());
        return data;
    }
    return 0;
}

Pointer* DataStructure::addPointer(Data *from,Data *to) {
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

Pointer* DataStructure::addPointer(const QString& name_from, const QString& name_to) {
    if (d->_readOnly) return 0;
    Data *from = 0;
    Data *to   = 0;

    QString tmpName;

    foreach( Data* n,  d->_data) {
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

Data *DataStructure::data(const QString& name) {
    QString tmpName;
    foreach( Data * n,  d->_data) {
        tmpName = n->name();
        if (tmpName == name) {
            return n;
        }
    }
    return 0;
}

void DataStructure::remove(Data *n) {
    d->_data.removeOne( n  );
    n->deleteLater();
}

void DataStructure::remove(Pointer *e) {
    d->_pointers.removeOne( e );
    if (e->to() || e->from())
      e->remove();
    e->deleteLater();
}

void DataStructure::remove(Group *g) {
    d->_groups.removeOne( g );
    //g->deleteLater();
}

Group* DataStructure::addGroup(const QString& name) {
     Group *gg = new Group();
     gg->setName(name);
     return gg;
}

void DataStructure::calcRelativeCenter() {
   if (parent() != 0){
        Document *gd = qobject_cast<Document*>(parent());
        d->_relativeCenter.setY(gd->height()/2);
        d->_relativeCenter.setX(gd->width()/2);
    }else{
        d->_relativeCenter.setY(0);
        d->_relativeCenter.setX(0);
    }
}

void DataStructure::setName(const QString& s) {
    d->_name = s;
}

void DataStructure::setDataDefaultColor(const QColor& color) {
    d->_dataDefaultColor = color;
}

void DataStructure::setPointerDefaultColor(const QColor& color) {
    d->_pointerDefaultColor = color;
}

void DataStructure::addDynamicProperty(const QString& property, QVariant value){
    if ( !setProperty(property.toUtf8(), value) && value.isValid()){
      DynamicPropertiesList::New()->addProperty(this, property);
    }
}

void DataStructure::removeDynamicProperty(const QString& property){
    this->addDynamicProperty(property, QVariant::Invalid);
    DynamicPropertiesList::New()->removeProperty(this, property);
}

void DataStructure::setDataColor(const QColor& c){
    QtConcurrent::blockingMap(d->_data, DataColorSetted(c));
}

void DataStructure::setPointersColor(const QColor& c){
    QtConcurrent::blockingMap(d->_pointers, PointerColorSetted(c));
}

void DataStructure::addDataDynamicProperty(const QString& property, QVariant value){
    QtConcurrent::blockingMap(d->_data, DataDynamicPropertySetted(property, value));
}

void DataStructure::addPointersDynamicProperty(const QString& property, QVariant value){
    QtConcurrent::blockingMap(d->_pointers, PointerDynamicPropertySetted(property, value));
}

void DataStructure::removeDataDynamicProperty(const QString& property){
    QtConcurrent::blockingMap(d->_data, DataDynamicPropertyUnSetted(property));
}
void DataStructure::removePointersDynamicProperty(const QString& property){
    QtConcurrent::blockingMap(d->_pointers, PointerDynamicPropertyUnSetted(property));
}

void DataStructure::setDataNameVisibility(bool b){
  d->_dataNamesVisible = b;
  QtConcurrent::blockingMap(d->_data, DataNameVisibilitySetted(b));
}

void DataStructure::setPointerNameVisibility(bool b){
  d->_pointerNamesVisible = b;
  QtConcurrent::blockingMap(d->_pointers, PointerNameVisibilitySetted(b));
}

void DataStructure::setDataValueVisibility(bool b){
  d-> _dataValuesVisible = b;
  QtConcurrent::blockingMap(d->_data, DataValueVisibilitySetted(b));
}

void DataStructure::setPointerValueVisibility(bool b){
  d->_pointerValuesVisible = b;
  QtConcurrent::blockingMap(d->_pointers, PointerValueVisibilitySetted(b));
}

void DataStructure::setEngine(	QScriptEngine *engine ) {
    d-> _engine = engine;
    d->_value = d->_engine->newQObject(this);

    if (! d->_name.isEmpty() ) {
        d->_engine->globalObject().setProperty(d->_name, d->_value);
    }

    for( int i = 0; i < d->_data.size(); ++i){
        d->_data.at(i)->setEngine(engine);
    }
    for( int i = 0; i < d->_pointers.size(); ++i){
        d->_pointers.at(i)->setEngine(engine);
    }

   foreach(Group *g, d->_groups) {
       QScriptValue array = d->_engine->newArray();
    //   foreach(Data * n, (*g) ) {
    //       array.property("push").call(array, QScriptValueList() << n->scriptValue());
    //   }
       d->_engine->globalObject().setProperty(g->name(), array);
   }
}
