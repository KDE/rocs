/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "graphDocument.h"
#include "DataType.h"
#include <QString>
#include <KSaveFile>
#include <QByteArray>
#include <KDebug>
#include "Data.h"
#include "Pointer.h"

//#include "dataTypeGroups.h"

#include "DSPluginManager.h"

// Default Constructor
DataTypeDocument::DataTypeDocument(const QString name, int width,  int height)
        : QObject(0), QList<DataType*>()
{
    _name = name;
    _width = width;
    _height = height;
    _modified = false;
    _saved = false;
    _engineBackend = new QtScriptBackend(this);
    _DSType = Rocs::DSPluginManager::instance()->actualPlugin();
}

DataTypeDocument::DataTypeDocument(const DataTypeDocument& gd)
        : QObject(0), QList<DataType*>()
{
    _name = gd.name();
    _width = gd.width();
    _height = gd.height();
    _DSType = Rocs::DSPluginManager::instance()->actualPlugin();
    _engineBackend = new QtScriptBackend(this);
    for (int i = 0; i < gd.count(); ++i){
//     foreach (DataType *g, gd) {
        append(Rocs::DSPluginManager::instance()->changeToDS(gd.at(i)));
    }
}

// Default Destructor
DataTypeDocument::~DataTypeDocument() {
    kDebug() << "Deleting DataType Document";
    kDebug() << this;
    kDebug() << size();

    for(int i = 0; i < size(); i ++){
	DataType *g = at(i);
	kDebug() << "Deleting dataType" << g->name();
        delete g;
    }
}

QtScriptBackend * DataTypeDocument::engineBackend() const{
    return _engineBackend;
}

// Sets the current file name of the DataType Collection
void DataTypeDocument::setName(const QString& name) {
    _name = name;
    emit nameChanged(name);
}

// Gets the name of the DataType
QString DataTypeDocument::name() const {
    return _name;
}

// set the width of the drawable area
void DataTypeDocument::setWidth(qreal width) {
    _width = width;
}

//set the height of the drawable area
void DataTypeDocument::setHeight(qreal height) {
    _height = height;
}

// gets the wheight of the drawable area
qreal DataTypeDocument::height() const {
    return _height;
}

// sets the width of the drawable area
qreal DataTypeDocument::width() const {
    return _width;
}

bool DataTypeDocument::isModified(){
  return _modified;
}

void DataTypeDocument::setActiveDataType(DataType *g){
    if ( indexOf(g) != -1){
        _activeDataType = g;
        emit activeDataTypeChanged(g);
    }
}

DataType* DataTypeDocument::addDataType(QString name) {


  DataType *g = Rocs::DSPluginManager::instance()->createNewDS(this, _DSType);
//     DataType *g = new DataType(this);
    g->setName(name);
    append(g);
    _activeDataType = g;
    emit dataTypeCreated(g);
    kDebug() << "DataType Added" << g->name();
    return g;
}


void DataTypeDocument::savedDocumentAt(const QString& fileName) {
    _lastSavedDocumentPath = fileName;
}

const QString& DataTypeDocument::documentPath() const {
    return _lastSavedDocumentPath;
}


bool DataTypeDocument::saveAsInternalFormat(const QString& filename) {
    k_buf.clear();

    KSaveFile saveFile( !filename.endsWith(".dataType") ? QString("%1.dataType").arg(filename) : filename);

    if (!saveFile.open()) {
        kDebug() << "Error: File Not Open";
        return false;
    }

    QTextStream stream(&saveFile);
    stream.setCodec("UTF-8");

    int dataTypeSize = count();

    for (int i = 0; i < dataTypeSize; i++) {
        DataType *g = this->at(i);

        k_buf += QString("[DataType %1] \n").arg(i).toUtf8();

	    savePropertiesInternalFormat(g);

        foreach( Datum *n, g->data()) {
	        k_buf += QString("[Datum %1]\n").arg(g->data().indexOf(n)).toUtf8();
	        savePropertiesInternalFormat(n);
        }

        int from, to;
        foreach( Pointer *e, g->pointers()) {
            from = g->data().indexOf(e->from());
            to = g->data().indexOf(e->to());

	        k_buf += QString("[Pointer %1->%2]\n").arg(from).arg(to).toUtf8();
	        savePropertiesInternalFormat(e);
        }

        /*     buf += " \n \n ############ GROUPS ########### \n \n";
             foreach( DataTypeGroup *gg, g->groups()) {
                 buf += QString("[Group %1] \n").arg((long) gg);

                 foreach( ::Datum *n, gg->data() ) {
                     buf += QString("%1\n").arg((long)n);
                 }
             } */
    }
    kDebug() << k_buf;

    stream << k_buf;

    if (!saveFile.finalize()) {
        kDebug() << "Error, file not saved.";
        return false;
    }
    _lastSavedDocumentPath = filename;
    return true;
}

void DataTypeDocument::savePropertiesInternalFormat(QObject *o) {
    const QMetaObject *metaObject = o->metaObject();
    int propertyCount = metaObject->propertyCount();

    for ( int i = 0; i < propertyCount; ++i) {
        QMetaProperty metaProperty = metaObject->property(i);
        const char *name = metaProperty.name();
        QVariant value = o->property(name);

        if ( QString("objectName").compare(metaProperty.name()) == 0) {
            continue;
        }
        else if( QString("name").compare(metaProperty.name()) == 0 ) {
          QString namevalue = QString("%1 : %2 \n" ).arg(name).arg(value.toString());
          kDebug() << "Normal"    << namevalue;
          kDebug() << "Ascii "    << namevalue.toAscii();
          kDebug() << "Latin1"    << namevalue.toLatin1();
          kDebug() << "UTF-8"     << namevalue.toUtf8();
          kDebug() << "Local8bit" << namevalue.toLocal8Bit();
        }

        k_buf +=  QString("%1 : %2 \n" ).arg(name, value.toString());
    }

    QList<QByteArray> propertyNames = o->dynamicPropertyNames();
    foreach(const QByteArray& name, propertyNames) {
        QVariant value = o->property(name);
        k_buf +=  QString("%1 : %2 \n" ).arg(name, value.toString()).toUtf8();
    }

    k_buf += '\n';
}

void DataTypeDocument::loadFromInternalFormat(const QString& filename) {
    QFile f(filename);
    if ( !f.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        kDebug() << "File not open " << filename.toUtf8();
        return;
    }

    DataType* tmpDataType = 0;
   //DataTypeGroup *tmpGroup = 0;
    QObject *tmpObject = 0;


    QTextStream in(&f);
    in.setCodec("UTF-8");

    while (!in.atEnd()) {
        QString str = in.readLine().simplified();

        if (str.startsWith('#')) { //! Ignore it, commented line.
            continue;
        }

        else if (str.startsWith("[DataType")) {
            QString gName = str.section(' ',1,1);
            gName.remove(']');
            tmpDataType = Rocs::DSPluginManager::instance()->createNewDS(this);
            tmpDataType->setName(gName.toAscii());
            tmpObject = tmpDataType;
            append(tmpDataType);
            kDebug() << "DataType Created";
        }

        else if (str.startsWith("[Datum")) {
            QString nName = str.section(' ',1,1);
            nName.remove(']');
            tmpObject = tmpDataType->addDatum(nName);
            kDebug() << "Datum Created";
        }

        else if (str.startsWith("[Pointer")) {
            QString eName = str.section(' ',1,1);
            eName.remove(']');

            QString nameFrom = eName.section("->", 0,0);
            QString nameTo = eName.section("->", 1,1);

            tmpObject = tmpDataType->addPointer(tmpDataType->data()[nameFrom.toInt()], tmpDataType->data()[nameTo.toInt()]);
            kDebug() << "Pointer Created";
        }
        else if (str.startsWith("[Group")) {
            /*QString gName = str.section(" ",1,1);
            gName.remove(']');
            tmpGroup = tmpDataType->addGroup(gName); */
        }
        else if (str.contains(':')) {
            QString propertyName = str.section(':',0,0).trimmed();
            QString propertyValue = str.section(':',1,1).trimmed();
            tmpObject->setProperty( propertyName.toUtf8() , propertyValue );
        }
        else {
//            // tmpGroup->append( tmpDataType->datum(str));
        }
    }
    kDebug() << "DataType Document Loaded.";
}

void DataTypeDocument::convertToDS(QString newDS){
    if (newDS != _DSType){
        kDebug() << "Need convert doc from " << _DSType << " to "<< newDS ;
//         DataTypeDocument * gDoc = new DataTypeDocument(*this);
        _DSType = newDS;
        int numDataTypes = count();
        for (int i = 0 ; i < numDataTypes; ++i){
            DataType * g = Rocs::DSPluginManager::instance()->changeToDS(at(i));
            if (at(i) == _activeDataType)
              _activeDataType = g;
            append(g);
        }
        for (int i = 0 ; i < numDataTypes; ++i){
            at(0)->deleteLater();
            removeAt(0);
        }
//         return gDoc;
    }
//     return 0;
}
