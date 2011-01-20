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

#include "Document.h"
#include "DataStructure.h"
#include <QString>
#include <KSaveFile>
#include <QByteArray>
#include <KDebug>
#include "Data.h"
#include "Pointer.h"

//#include "dataStructureGroups.h"

#include "DataStructurePluginManager.h"

class DocumentPrivate{
public:
    DocumentPrivate(){}
    
    QString _buf;
    QString _lastSavedDocumentPath;
    QString _name;
    qreal _width;
    qreal _height;
    bool _modified;
    bool _saved;
    DataStructure *_activeDataStructure;
    QString _dataStructureType;
    QtScriptBackend* _engineBackend;
    QList<DataStructure*> _dataStructures;
    
 
};

Document::Document(const QString& name, int width,  int height, QObject *parent)
        : QObject(parent), d( new DocumentPrivate())
{
    d->_name = name;
    d->_width = width;
    d->_height = height;
    d->_modified = false;
    d->_saved = false;
    d->_engineBackend = new QtScriptBackend(this);
    d->_dataStructureType = DataStructurePluginManager::self()->actualPlugin();
}

/*
Document::Document(const Document& gd)
        : QObject(0)
{
    QObject::setParent(gd.parent());
    d->_name = gd.name();
    d->_width = gd.width();
    d->_height = gd.height();
    d->_dataStructureType = DataStructurePluginManager::instance()->actualPlugin();
    d->_engineBackend = new QtScriptBackend(this);
    
    for (int i = 0; i < gd.dataStructures().count(); ++i){
        append(DataStructurePluginManager::instance()->changeToDataStructure(gd.d->_dataStructures.at(i)));
    }
} */

// Default Destructor
Document::~Document() {

    for(int i = 0; i < d->_dataStructures.size(); i ++){
	DataStructure *g = d->_dataStructures.at(i);
        delete g;
    }
    delete d;
}

QList<DataStructure*>& Document::dataStructures() const { return d->_dataStructures; }

QtScriptBackend * Document::engineBackend() const{
    return d->_engineBackend;
}

// Sets the current file name of the DataStructure Collection
void Document::setName(const QString& name) {
    d->_name = name;
    emit nameChanged(name);
}

// Gets the name of the DataStructure
QString Document::name() const {
    return d->_name;
}

// set the width of the drawable area
void Document::setWidth(qreal width) {
    d->_width = width;
}

//set the height of the drawable area
void Document::setHeight(qreal height) {
    d->_height = height;
}

// gets the wheight of the drawable area
qreal Document::height() const {
    return d->_height;
}

// sets the width of the drawable area
qreal Document::width() const {
    return d->_width;
}

bool Document::isModified(){
  return d->_modified;
}

void Document::setActiveDataStructure(DataStructure *g){
    if (d->_dataStructures.indexOf(g) != -1){
        d->_activeDataStructure = g;
        emit activeDataStructureChanged(g);
    }
}

DataStructure* Document::addDataStructure(QString name) {
    DataStructure *g = DataStructurePluginManager::self()->createNewDataStructure(this, d->_dataStructureType);
    g->setName(name);
    d->_dataStructures.append(g);
    d->_activeDataStructure = g;
    emit dataStructureCreated(g);
    return g;
}

void Document::savedDocumentAt(const QString& fileName) {
    d->_lastSavedDocumentPath = fileName;
}

const QString& Document::documentPath() const {
    return d->_lastSavedDocumentPath;
}

void Document::remove(DataStructure *dataStructure){
  d->_dataStructures.removeOne(dataStructure);
}

bool Document::saveAsInternalFormat(const QString& filename) {
    d->_buf.clear();

    KSaveFile saveFile( !filename.endsWith(".dataStructure") ? QString("%1.dataStructure").arg(filename) : filename);

    if (!saveFile.open()) {
        kDebug() << "Error: File Not Open";
        return false;
    }

    QTextStream stream(&saveFile);
    stream.setCodec("UTF-8");

    for (int i = 0; i < d->_dataStructures.count(); i++) {
        DataStructure *g = d->_dataStructures.at(i);

        d->_buf += QString("[DataStructure %1] \n").arg(i).toUtf8();

	savePropertiesInternalFormat(g);

        foreach( Data *n, g->dataList()) {
	     d->_buf += QString("[Data %1]\n").arg(g->dataList().indexOf(n)).toUtf8();
	     savePropertiesInternalFormat(n);
        }

        int from, to;
        foreach( Pointer *e, g->pointers()) {
            from = g->dataList().indexOf(e->from());
            to = g->dataList().indexOf(e->to());
	    d->_buf += QString("[Pointer %1->%2]\n").arg(from).arg(to).toUtf8();
	    savePropertiesInternalFormat(e);
        }

        /*     buf += " \n \n ############ GROUPS ########### \n \n";
             foreach( DataStructureGroup *gg, g->groups()) {
                 buf += QString("[Group %1] \n").arg((long) gg);

                 foreach( ::Data *n, gg->data() ) {
                     buf += QString("%1\n").arg((long)n);
                 }
             } */
    }
    kDebug() << d->_buf;

    stream << d->_buf;

    if (!saveFile.finalize()) {
        kDebug() << "Error, file not saved.";
        return false;
    }
    d->_lastSavedDocumentPath = filename;
    return true;
}

void Document::savePropertiesInternalFormat(QObject *o) {
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
        }

        d->_buf +=  QString("%1 : %2 \n" ).arg(name, value.toString());
    }

    QList<QByteArray> propertyNames = o->dynamicPropertyNames();
    foreach(const QByteArray& name, propertyNames) {
        QVariant value = o->property(name);
        d->_buf +=  QString("%1 : %2 \n" ).arg(name, value.toString()).toUtf8();
    }

    d->_buf += '\n';
}

void Document::loadFromInternalFormat(const QString& filename) {
    QFile f(filename);
    if ( !f.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        kDebug() << "File not open " << filename.toUtf8();
        return;
    }

    DataStructure* tmpDataStructure = 0;
   //DataStructureGroup *tmpGroup = 0;
    QObject *tmpObject = 0;


    QTextStream in(&f);
    in.setCodec("UTF-8");

    while (!in.atEnd()) {
        QString str = in.readLine().simplified();

        if (str.startsWith('#')) { //! Ignore it, commented line.
            continue;
        }

        else if (str.startsWith("[DataStructure")) {
            QString gName = str.section(' ',1,1);
            gName.remove(']');
            tmpDataStructure = DataStructurePluginManager::self()->createNewDataStructure(this);
            tmpDataStructure->setName(gName.toAscii());
            tmpObject = tmpDataStructure;
            d->_dataStructures.append(tmpDataStructure);
            kDebug() << "DataStructure Created";
        }

        else if (str.startsWith("[Data")) {
            QString nName = str.section(' ',1,1);
            nName.remove(']');
            tmpObject = tmpDataStructure->addData(nName);
            kDebug() << "Data Created";
        }

        else if (str.startsWith("[Pointer")) {
            QString eName = str.section(' ',1,1);
            eName.remove(']');

            QString nameFrom = eName.section("->", 0,0);
            QString nameTo = eName.section("->", 1,1);

            tmpObject = tmpDataStructure->addPointer(tmpDataStructure->dataList().at(nameFrom.toInt()), 
                                                     tmpDataStructure->dataList().at(nameTo.toInt()));
            kDebug() << "Pointer Created";
        }
        else if (str.startsWith("[Group")) {
            /*QString gName = str.section(" ",1,1);
            gName.remove(']');
            tmpGroup = tmpDataStructure->addGroup(gName); */
        }
        else if (str.contains(':')) {
            QString propertyName = str.section(':',0,0).trimmed();
            QString propertyValue = str.section(':',1,1).trimmed();
            tmpObject->setProperty( propertyName.toUtf8() , propertyValue );
        }
        else {
//            // tmpGroup->append( tmpDataStructure->data(str));
        }
    }
    kDebug() << "DataStructure Document Loaded.";
}

DataStructure *Document::activeDataStructure() const { return d->_activeDataStructure; }
void Document::convertToDataStructure(QString newDataStructure){
    if (newDataStructure == d->_dataStructureType) return;
//        kDebug() << "Need convert doc from " << d->_dataStructureType << " to "<< newDataStructure ;
        d->_dataStructureType = newDataStructure;
//         Document * gDoc = new Document(*this);
//        int numDataStructures = count();
//         for (int i = 0 ; i < numDataStructures; ++i){
//             DataStructure * g = DataStructurePluginManager::instance()->changeToDataStructure(at(i));
//             if (at(i) == d->_activeDataStructure)
//               d->_activeDataStructure = g;
//             append(g);
//         }
//         for (int i = 0 ; i < numDataStructures; ++i){
//             at(0)->deleteLater();
//             removeAt(0);
//         }
//         return gDoc;
//    }
//     return 0;
}
