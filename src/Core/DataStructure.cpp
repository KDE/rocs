/*
    This file is part of Rocs.
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include "MainWindow.h"

#include <boost/shared_ptr.hpp>

#include <KDebug>
#include <QColor>
#include <DataItem.h>

DataStructurePtr DataStructure::create(Document *parent) {
    return create<DataStructure>(parent);
}

DataStructurePtr DataStructure::create(DataStructurePtr other, Document *parent) {
    return create<DataStructure>(other, parent);
}

DataStructurePtr DataStructure::getDataStructure() const {
    DataStructurePtr px(d->q);
    return px;
}

DataStructure::DataStructure(Document *parent) : QObject(parent), d(new DataStructurePrivate){
    d->_automate = false;
    d->_readOnly = false;
    d->_document = parent;
    updateRelativeCenter();
    d->_dataDefaultColor = QColor("blue");
    d->_pointerDefaultColor = QColor("gray");
    d->_dataNamesVisible = true;
    d->_dataValuesVisible = true;
    d->_pointerNamesVisible = false;
    d->_pointerValuesVisible = true;
    d->_identifierCount = 1;

    // create type lists
    d->_dataTypeLists.insert(0,DataList());
    d->_pointerTypeLists.insert(0,PointerList());

    connect (this, SIGNAL(changed()), parent, SLOT(resizeDocumentIncrease()));
    connect (this, SIGNAL(resizeRequest(Document::Border)), parent, SLOT(resizeDocumentBorder(Document::Border)));
    emit changed();
}

void DataStructure::importStructure(DataStructurePtr other){
    d->_readOnly = other->readOnly();
    updateRelativeCenter();

    d->_pointerDefaultColor     = other->pointerDefaultColor();
    d->_dataDefaultColor        = other->dataDefaultColor();
    d->_dataNamesVisible        = other->d->_dataNamesVisible;
    d->_dataValuesVisible       = other->d->_dataValuesVisible;
    d->_pointerNamesVisible     = other->d->_pointerNamesVisible;
    d->_pointerValuesVisible    = other->d->_pointerValuesVisible;

    QHash <Data*, DataPtr > dataTodata;
    foreach(DataPtr n, other->dataList()){
        DataPtr newdata = addData(n->name());
        newdata->setColor(n->color());
        newdata->setValue(n->value());
        newdata->setX(n->x());
        newdata->setY(n->y());
        newdata->setWidth(n->width());
        dataTodata.insert(n.get(), newdata);
    }
    foreach(PointerPtr e, other->pointers()){
        DataPtr from =  dataTodata.value(e->from().get());
        DataPtr to =  dataTodata.value(e->to().get());

        PointerPtr newPointer = addPointer(from, to);
        newPointer->setColor(e->color());
        newPointer->setValue(e->value());
    }

    connect (this, SIGNAL(changed()), d->_document, SLOT(resizeDocumentIncrease()));
    connect (this, SIGNAL(resizeRequest(Document::Border)), d->_document, SLOT(resizeDocumentBorder(Document::Border)));
    emit changed();
}


DataStructure::~DataStructure() {
    foreach(PointerList pointerType, d->_pointerTypeLists) {
    foreach(PointerPtr pointer,  pointerType) {
        pointer->remove();
    }
    }
    foreach(DataList dataType, d->_dataTypeLists) {
    foreach(DataPtr data, dataType) {
        data->remove();
    }
    }
}

const DataList DataStructure::dataList(int dataType) const {
    if( d->_dataTypeLists.contains( dataType ) ) {
        return d->_dataTypeLists[dataType];
    }
    return DataList();
}

const PointerList DataStructure::pointers(int pointerType) const {
    if( d->_pointerTypeLists.contains( pointerType ) ) {
        return d->_pointerTypeLists[pointerType];
    }
    return PointerList();
}

int DataStructure::registerDataType(QString name) {
    int identifier = d->_dataTypeLists.size()+1;
    d->_dataTypeLists.insert(identifier,DataList());
    d->_dataTypes.insert(identifier,name);
    return identifier;
}

int DataStructure::registerPointerType(QString name) {
    int identifier = d->_pointerTypeLists.size()+1;
    d->_pointerTypeLists.insert(identifier,PointerList());
    d->_pointerTypes.insert(identifier,name);
    return identifier;
}

QString DataStructure::getDataTypeName(int dataType) const {
    return d->_dataTypes[dataType];
}

QString DataStructure::getPointerTypeName(int pointerType) const {
    return d->_pointerTypes[pointerType];
}

bool DataStructure::removeDataType(int dataType) {
    if(dataType==0) return false;

    foreach(DataPtr data, d->_dataTypeLists[dataType]) {
        data->remove();
    }
    d->_dataTypeLists[dataType].clear();
    d->_dataTypeLists.remove(dataType);
    return d->_dataTypes.remove(dataType)>0;
}

bool DataStructure::removePointerType(int pointerType) {
    if( pointerType==0 || !d->_pointerTypeLists.contains(pointerType) ) {
        return false;
    }

    foreach(PointerPtr pointer, d->_pointerTypeLists[pointerType]) {
        pointer->remove();
    }
    d->_pointerTypeLists[pointerType].clear();
    d->_pointerTypeLists.remove(pointerType);
    return d->_pointerTypes.remove(pointerType)>0;
}

void DataStructure::setReadOnly(bool r){
    d->_readOnly = r;
    d->_document->setModified();
}

void DataStructure::remove() {
    foreach(PointerList pointerType, d->_pointerTypeLists) {
    foreach(PointerPtr pointer,  pointerType) {
        pointer->remove();
    }
    }
    foreach(DataList dataType, d->_dataTypeLists) {
    foreach(DataPtr data, dataType) {
        data->remove();
    }
    }
    d->_document->remove(getDataStructure());
}

int DataStructure::generateUniqueIdentifier() {
    return d->_identifierCount++;
}

DataPtr DataStructure::addData(QString name, int dataType) {
    if (d->_readOnly) return DataPtr();

    DataPtr n = Data::create( this->getDataStructure(), generateUniqueIdentifier(), dataType );
    n->setName(name);
    return addData(n);
}

DataPtr DataStructure::addData(DataPtr data, int dataType){
    Q_ASSERT(dataType>=0 && dataType<=d->_dataTypeLists.size());

    d->_dataTypeLists[dataType].append( data );
    QMap<QString, QVariant>::const_iterator i = d->m_globalPropertiesData.constBegin();
    while (i != d->m_globalPropertiesData.constEnd()) {
       data->addDynamicProperty(i.key(), i.value());
       ++i;
    }
    emit dataCreated( data );
    emit changed();

//     connect(data.get(), SIGNAL(removed()),                    this, SIGNAL(changed())); //FIXME removed for now
    connect(data.get(), SIGNAL(iconChanged(QString)),         this, SIGNAL(changed()));
    connect(data.get(), SIGNAL(nameChanged(QString)),         this, SIGNAL(changed()));
    connect(data.get(), SIGNAL(valueChanged(QVariant)),       this, SIGNAL(changed()));
    connect(data.get(), SIGNAL(colorChanged(QColor)),         this, SIGNAL(changed()));
    connect(data.get(), SIGNAL(posChanged(QPointF)),          this, SIGNAL(changed()));
    connect(data.get(), SIGNAL(nameVisibilityChanged(bool)),  this, SIGNAL(changed()));
    connect(data.get(), SIGNAL(valueVisibilityChanged(bool)), this, SIGNAL(changed()));
    connect(data.get(), SIGNAL(useColorChanged(bool)),        this, SIGNAL(changed()));
    return data;
}

QList< DataPtr > DataStructure::addDataList(QList< DataPtr > dataList, int dataType){
    Q_ASSERT(dataType>=0 && dataType<d->_dataTypeLists.size());

    foreach (DataPtr n, dataList) {
        d->_dataTypeLists[dataType].append( n );
        emit dataCreated( n );
//         connect(n.get(), SIGNAL(removed()),                    this, SIGNAL(changed())); //FIXME removed for now
        connect(n.get(), SIGNAL(iconChanged(QString)),         this, SIGNAL(changed()));
        connect(n.get(), SIGNAL(nameChanged(QString)),         this, SIGNAL(changed()));
        connect(n.get(), SIGNAL(valueChanged(QVariant)),       this, SIGNAL(changed()));
        connect(n.get(), SIGNAL(colorChanged(QColor)),         this, SIGNAL(changed()));
        connect(n.get(), SIGNAL(posChanged(QPointF)),          this, SIGNAL(changed()));
        connect(n.get(), SIGNAL(nameVisibilityChanged(bool)),  this, SIGNAL(changed()));
        connect(n.get(), SIGNAL(valueVisibilityChanged(bool)), this, SIGNAL(changed()));
        connect(n.get(), SIGNAL(useColorChanged(bool)),        this, SIGNAL(changed()));
    }
    emit changed();
    return dataList;
}

QList< DataPtr > DataStructure::addDataList(QList< QPair<QString,QPointF> > dataList, int dataType) {
    QList< DataPtr > dataCreateList;
    QPair<QString, QPointF> dataDefinition;
    foreach (dataDefinition, dataList) {
        if (DataPtr data = addData(dataDefinition.first, dataType)){
            data->setPos(dataDefinition.second.x(), dataDefinition.second.y());
            dataCreateList << data;
        }
    }
    return addDataList(dataCreateList);
}


PointerPtr DataStructure::addPointer(PointerPtr pointer, int pointerType){
    Q_ASSERT(pointerType>=0 && pointerType<d->_pointerTypeLists.size());

    d->_pointerTypeLists[pointerType].append( pointer );
    QMap<QString, QVariant>::const_iterator i = d->m_globalPropertiesPointer.constBegin();
    while (i != d->m_globalPropertiesPointer.constEnd()) {
       pointer->addDynamicProperty(i.key(), i.value());
       ++i;
    }
    emit pointerCreated(pointer);
    emit changed();
    connect (pointer.get(), SIGNAL(changed()), this, SIGNAL(changed()));
    return pointer;
}

DataPtr DataStructure::addData(QString name, QPointF pos, int dataType){
    if (DataPtr data = addData(name, dataType)){
        data->setPos(pos.x(), pos.y());
        return data;
    }
    return DataPtr();
}

PointerPtr DataStructure::addPointer(DataPtr from, DataPtr to, int pointerType) {
    if (d->_readOnly)                   // If the data structure is in read only mode, no new stuff should be added.
        return PointerPtr();

    if ( !from || !to ) {               // one of the two required datas are null. do not add a pointer between a valid and a null datas.
        return PointerPtr();
    }

    if ( from->dataStructure() != to->dataStructure()) { // the user is trying to connect datas from different graphs.
        return PointerPtr();
    }
    PointerPtr pointer = Pointer::create(getDataStructure(), from, to, pointerType);
    return addPointer(pointer);
}

PointerPtr DataStructure::addPointer(const QString& name_from, const QString& name_to, int pointerType) {
//FIXME reimplement by ids
// using of strings allows uncontrollable behavior
    if (d->_readOnly) return PointerPtr();

    DataPtr from, to;

    QString tmpName;

    foreach(DataList dataType, d->_dataTypeLists) {
    foreach( DataPtr n, dataType) {
        tmpName = n->name();

        if (tmpName == name_from) {
            from = n;
        }
        if (tmpName == name_to) {
            to = n;
        }
        if (to && from) {
            break;
        }
    }
    }

    return addPointer(from, to, pointerType);
}

DataPtr DataStructure::getData(int uniqueIdentifier) {
    foreach(DataList dataType, d->_dataTypeLists) {
    foreach(DataPtr data, dataType) {
        if (data->identifier() == uniqueIdentifier) {
            return data;
        }
    }
    }
    return DataPtr();
}

void DataStructure::remove(DataPtr n) {
    //Note: important for resize: remove node before emit resizeRequest
    Document *doc = DocumentManager::self()->activeDocument();
    bool left = false;
    bool top = false;
    bool bottom = false;
    bool right = false;

    if (doc!=0) {
        if (n->x()<doc->left()+2*GraphScene::kBORDER)      left = true;
        if (n->x()>doc->right()-2*GraphScene::kBORDER)     right = true;
        if (n->y()<doc->top()+2*GraphScene::kBORDER)       top = true;
        if (n->y()>doc->bottom()-2*GraphScene::kBORDER)    bottom = true;
    }

    // find data among all types and delete it
    //TODO improved performance: use type information to access list
    QMap<int, DataList>::iterator i = d->_dataTypeLists.begin();
    while (i != d->_dataTypeLists.end()) {
        i->removeOne(n);
        ++i;
    }

    // emit changes
    if (left)   emit resizeRequest( Document::BorderLeft );
    if (right)  emit resizeRequest( Document::BorderRight );
    if (top)    emit resizeRequest( Document::BorderTop );
    if (bottom) emit resizeRequest( Document::BorderBottom );

    updateRelativeCenter();
}

void DataStructure::remove(PointerPtr e) {
    //TODO improved performance: use type information to access list
    QMap<int, PointerList>::iterator i = d->_pointerTypeLists.begin();
    while (i != d->_pointerTypeLists.end()) {
        i->removeOne(e);
        ++i;
    }
    emit changed();
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
        d->_relativeCenter.setY((gd->bottom()+gd->top())/2);
        d->_relativeCenter.setX((gd->right()+gd->left())/2);
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
    foreach(DataList dataType, d->_dataTypeLists) {
        QtConcurrent::blockingMap(dataType, DataColorSetted(c));
    }
}

void DataStructure::setPointersColor(const QColor& c){
    foreach(PointerList pointerType, d->_pointerTypeLists) {
        QtConcurrent::blockingMap(pointerType, PointerColorSetted(c));
    }
}

void DataStructure::addDataDynamicProperty(const QString& property, QVariant value){
    foreach(DataList dataType, d->_dataTypeLists) {
        QtConcurrent::blockingMap(dataType, DataDynamicPropertySetted(property, value));
    }
    d->m_globalPropertiesData.insert(property, value);
}

void DataStructure::addPointersDynamicProperty(const QString& property, QVariant value){
    foreach(PointerList pointerType, d->_pointerTypeLists) {
        QtConcurrent::blockingMap(pointerType, PointerDynamicPropertySetted(property, value));
    }
    d->m_globalPropertiesData.insert(property, value);
}

void DataStructure::removeDataDynamicProperty(const QString& property){
    foreach(DataList dataType, d->_dataTypeLists) {
        QtConcurrent::blockingMap(dataType, DataDynamicPropertyUnSetted(property));
    }
}
void DataStructure::removePointersDynamicProperty(const QString& property){
    foreach(PointerList pointerType, d->_pointerTypeLists) {
        QtConcurrent::blockingMap(pointerType, PointerDynamicPropertyUnSetted(property));
    }
}

void DataStructure::setDataNameVisibility(bool b){
    d->_dataNamesVisible = b;
    foreach(DataList dataType, d->_dataTypeLists) {
        QtConcurrent::blockingMap(dataType, DataNameVisibilitySetted(b));
    }
}

void DataStructure::setPointerNameVisibility(bool b){
    d->_pointerNamesVisible = b;
    foreach(PointerList pointerType, d->_pointerTypeLists) {
        QtConcurrent::blockingMap(pointerType, PointerNameVisibilitySetted(b));
    }
}

void DataStructure::setDataValueVisibility(bool b){
    d-> _dataValuesVisible = b;
    foreach(DataList dataType, d->_dataTypeLists) {
        QtConcurrent::blockingMap(dataType, DataValueVisibilitySetted(b));
    }
}

void DataStructure::setPointerValueVisibility(bool b){
    d->_pointerValuesVisible = b;
    foreach(PointerList pointerType, d->_pointerTypeLists) {
        QtConcurrent::blockingMap(pointerType, PointerValueVisibilitySetted(b));
    }
}

void DataStructure::setEngine(	QScriptEngine *engine ) {
    d-> _engine = engine;
    d->_value = d->_engine->newQObject(this);

    if (! d->_name.isEmpty() ) {
        d->_engine->globalObject().setProperty(d->_name, d->_value);
    }

    foreach(DataList dataType, d->_dataTypeLists) {
    for( int i = 0; i < dataType.size(); ++i){
        dataType.at(i)->setEngine(engine);
    }
    }

    foreach(PointerList pointerType, d->_pointerTypeLists) {
    for( int i = 0; i < pointerType.size(); ++i){
        pointerType.at(i)->setEngine(engine);
    }
    }

    foreach(Group *g, d->_groups) {
       QScriptValue array = d->_engine->newArray();
    //   foreach(Data * n, (*g) ) {
    //       array.property("push").call(array, QScriptValueList() << n->scriptValue());
    //   }
       d->_engine->globalObject().setProperty(g->name(), array);
   }
}
