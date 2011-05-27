/* 
    This file is part of Rocs.
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"
#include "QtScriptBackend.h"
#include "Group.h"
#include "Document.h"
#include "DocumentManager.h"
#include "GraphScene.h"
#include "DynamicPropertiesList.h"
#include "ConcurrentHelpClasses.h"

#include <KDebug>
#include <QColor>

DataStructure::DataStructure(Document *parent) : QObject(parent), d(new DataStructurePrivate){
    d->_automate = false;
    d->_readOnly = false;
    d->_document = parent;
    d->_begin = 0;
    updateRelativeCenter();
    d->_dataDefaultColor = QColor("blue");
    d->_pointerDefaultColor = QColor("gray");
    d->_dataNamesVisible = true;
    d->_dataValuesVisible = true;
    d->_pointerNamesVisible = false;
    d->_pointerValuesVisible = true;

    connect (this, SIGNAL(changed()), parent, SLOT(resizeDocumentIncrease()));
    connect (this, SIGNAL(resizeRequest(Document::Border)), parent, SLOT(resizeDocumentBorder(Document::Border)));
    emit changed();
}

DataStructure::DataStructure(DataStructure& other, Document * parent): QObject(parent), d(new DataStructurePrivate){
    d->_readOnly = other.readOnly();
    d->_document = parent;
    updateRelativeCenter();

    d->_pointerDefaultColor     = other.pointerDefaultColor();
    d->_dataDefaultColor        = other.dataDefaultColor();
    d->_dataNamesVisible        = other.d->_dataNamesVisible;
    d->_dataValuesVisible       = other.d->_dataValuesVisible;
    d->_pointerNamesVisible     = other.d->_pointerNamesVisible;
    d->_pointerValuesVisible    = other.d->_pointerValuesVisible;

//     QHash <Data*, Data* > dataTodata;
//     foreach(Data* n, other.d->_data){
//         Data* newdata = addData(n->name());
//         newdata->setColor(n->color());
//         newdata->setValue(n->value());
//         newdata->setX(n->x());
//         newdata->setY(n->y());
//         newdata->setWidth(n->width());
//         dataTodata.insert(n, newdata);
//     }
//     foreach(Pointer *e, other.pointers()){
//         Data* from =  dataTodata.value(e->from());
//         Data* to =  dataTodata.value(e->to());
//
//         Pointer* newPointer = addPointer(from, to);
//         newPointer->setColor(e->color());
//         newPointer->setValue(e->value());
//     }

    connect (this, SIGNAL(changed()), parent, SLOT(resizeDocumentIncrease()));
    connect (this, SIGNAL(resizeRequest(Document::Border)), parent, SLOT(resizeDocumentBorder(Document::Border)));
    emit changed();
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
    d->_document->setModified();
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
    QMap<QString, QVariant>::const_iterator i = d->m_globalPropertiesData.constBegin();
    while (i != d->m_globalPropertiesData.constEnd()) {
       data->addDynamicProperty(i.key(), i.value());
       ++i;
    }
    emit dataCreated( data );
    emit changed();
    
    Data *n = data;
    connect(n, SIGNAL(removed()),                    this, SIGNAL(changed()));
    connect(n, SIGNAL(iconChanged(QString)),         this, SIGNAL(changed()));
    connect(n, SIGNAL(nameChanged(QString)),         this, SIGNAL(changed()));
    connect(n, SIGNAL(valueChanged(QVariant)),       this, SIGNAL(changed()));
    connect(n, SIGNAL(colorChanged(QColor)),         this, SIGNAL(changed()));
    connect(n, SIGNAL(posChanged(QPointF)),          this, SIGNAL(changed()));
    connect(n, SIGNAL(nameVisibilityChanged(bool)),  this, SIGNAL(changed()));
    connect(n, SIGNAL(valueVisibilityChanged(bool)), this, SIGNAL(changed()));
    connect(n, SIGNAL(useColorChanged(bool)),        this, SIGNAL(changed()));
    return data;
}

QList<Data*> DataStructure::addDataList(QList<Data*> dataList){
    Data* n;
    foreach (n, dataList) {
        d->_data.append( n );
        emit dataCreated( n );
        connect(n, SIGNAL(removed()),                    this, SIGNAL(changed()));
        connect(n, SIGNAL(iconChanged(QString)),         this, SIGNAL(changed()));
        connect(n, SIGNAL(nameChanged(QString)),         this, SIGNAL(changed()));
        connect(n, SIGNAL(valueChanged(QVariant)),       this, SIGNAL(changed()));
        connect(n, SIGNAL(colorChanged(QColor)),         this, SIGNAL(changed()));
        connect(n, SIGNAL(posChanged(QPointF)),          this, SIGNAL(changed()));
        connect(n, SIGNAL(nameVisibilityChanged(bool)),  this, SIGNAL(changed()));
        connect(n, SIGNAL(valueVisibilityChanged(bool)), this, SIGNAL(changed()));
        connect(n, SIGNAL(useColorChanged(bool)),        this, SIGNAL(changed()));
    }
    emit changed();
    return dataList;
}

QList<Data*> DataStructure::addDataList(QList< QPair<QString,QPointF> > dataList) {
    QList<Data*> dataCreateList;
    QPair<QString, QPointF> dataDefinition;
    foreach (dataDefinition, dataList) {
        if (Data *data = addData(dataDefinition.first)){
            data->setPos(dataDefinition.second.x(), dataDefinition.second.y());
            dataCreateList << data;
        }
    }
    return addDataList(dataCreateList);
}


Pointer* DataStructure::addPointer(Pointer *pointer){
    d->_pointers.append( pointer );
    QMap<QString, QVariant>::const_iterator i = d->m_globalPropertiesPointer.constBegin();
    while (i != d->m_globalPropertiesPointer.constEnd()) {
       pointer->addDynamicProperty(i.key(), i.value());
       ++i;
    }
    emit pointerCreated(pointer);
    emit changed();
    connect (pointer, SIGNAL(changed()), this, SIGNAL(changed()));
    return pointer;
}

Data* DataStructure::addData(QString name, QPointF pos){
    if (Data *data = addData(name)){
        data->setPos(pos.x(), pos.y());
        return data;
    }
    return 0;
}

Pointer* DataStructure::addPointer(Data *from, Data *to) {
    if (d->_readOnly)                   // If the data structure is in read only mode, no new stuff should be added.
        return 0;

    if ( !from || !to ) {               // one of the two required datas are null. do not add a pointer between a valid and a null datas.
        return 0;
    }
    
    if ( (d->_data.indexOf(from) == -1)  // the user is trying to connect datas from different graphs.
      || (d->_data.indexOf(to) == -1)) {
        return 0;
    }

    Pointer *pointer  = new Pointer(this, from, to);

    return addPointer(pointer);
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
    //Note: important for resize: remove node before emit resizeRequest
    Document *doc = DocumentManager::self()->activeDocument();
    bool left = false;
    bool top = false;
    bool bottom = false;
    bool right = false;

    if (doc!=0) {
        if (n->x()<doc->xLeft()+2*GraphScene::kBORDER)      left = true;
        if (n->x()>doc->xRight()-2*GraphScene::kBORDER)     right = true;
        if (n->y()<doc->yTop()+2*GraphScene::kBORDER)       top = true;
        if (n->y()>doc->yBottom()-2*GraphScene::kBORDER)    bottom = true;
    }

    // proceed delete
    d->_data.removeOne( n );
    n->deleteLater();

    // emit changes
    if (left)   emit resizeRequest( Document::BorderLeft );
    if (right)  emit resizeRequest( Document::BorderRight );
    if (top)    emit resizeRequest( Document::BorderTop );
    if (bottom) emit resizeRequest( Document::BorderBottom );

    updateRelativeCenter();
}

void DataStructure::remove(Pointer *e) {
    d->_pointers.removeOne( e );
    if (e->to() || e->from()) {
        e->remove();
        emit changed();
    }
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

void DataStructure::updateRelativeCenter() {
   if (parent() != 0){
        Document *gd = qobject_cast<Document*>(parent());
        d->_relativeCenter.setY((gd->yBottom()+gd->yTop())/2);
        d->_relativeCenter.setX((gd->xRight()+gd->xLeft())/2);
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
    d->m_globalPropertiesData.insert(property, value);
}

void DataStructure::addPointersDynamicProperty(const QString& property, QVariant value){
    QtConcurrent::blockingMap(d->_pointers, PointerDynamicPropertySetted(property, value));
    d->m_globalPropertiesData.insert(property, value);
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