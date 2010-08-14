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
#include "DataStructureBase.h"
#include <QString>
#include <KSaveFile>
#include <QByteArray>
#include <KDebug>
#include "node.h"
#include "edge.h"
#include "graphGroups.h"

#include "DSPluginManager.h"

// Default Constructor
GraphDocument::GraphDocument(const QString name, int width,  int height)
        : QObject(0), QList<DataStructureBase*>()
{
    _name = name;
    _width = width;
    _height = height;
    _modified = false;
    _saved = false;
    _engineBackend = new QtScriptBackend(this);
    _DSType = Rocs::DSPluginManager::New()->actualPlugin();
}

GraphDocument::GraphDocument(const GraphDocument& gd)
        : QObject(0), QList<DataStructureBase*>()
{
    _name = gd.name();
    _width = gd.width();
    _height = gd.height();
    _DSType = Rocs::DSPluginManager::New()->actualPlugin();
    _engineBackend = new QtScriptBackend(this);
    for (int i = 0; i < gd.count(); ++i){
//     foreach (Graph *g, gd) {
        append(Rocs::DSPluginManager::New()->changeToDS(gd.at(i)));
    }
}

// Default Destructor
GraphDocument::~GraphDocument() {
    kDebug() << "Deleting Graph Document";
    kDebug() << this;
    kDebug() << size();

    for(int i = 0; i < size(); i ++){
	DataStructureBase *g = at(i);
	kDebug() << "Deleting graph" << g->name();
        delete g;
    }
}

QtScriptBackend * GraphDocument::engineBackend() const{
    return _engineBackend;
}

// Sets the current file name of the Graph Collection
void GraphDocument::setName(const QString& name) {
    _name = name;
    emit nameChanged(name);
}

// Gets the name of the Graph
QString GraphDocument::name() const {
    return _name;
}

// set the width of the drawable area
void GraphDocument::setWidth(qreal width) {
    _width = width;
}

//set the height of the drawable area
void GraphDocument::setHeight(qreal height) {
    _height = height;
}

// gets the wheight of the drawable area
qreal GraphDocument::height() const {
    return _height;
}

// sets the width of the drawable area
qreal GraphDocument::width() const {
    return _width;
}

bool GraphDocument::isModified(){
  return _modified;
}

void GraphDocument::setActiveGraph(DataStructureBase *g){
    if ( indexOf(g) != -1){
        _activeGraph = g;
        emit activeGraphChanged(g);
    }
}

DataStructureBase* GraphDocument::addGraph(QString name) {


  DataStructureBase *g = Rocs::DSPluginManager::New()->createNewDS(this, _DSType);
//     Graph *g = new Graph(this);
    g->setName(name);
    append(g);
    _activeGraph = g;
    emit graphCreated(g);
    kDebug() << "Graph Added" << g->name();
    return g;
}


void GraphDocument::savedDocumentAt(const QString& fileName) {
    _lastSavedDocumentPath = fileName;
}

const QString& GraphDocument::documentPath() const {
    return _lastSavedDocumentPath;
}


bool GraphDocument::saveAsInternalFormat(const QString& filename) {
    k_buf.clear();

    KSaveFile saveFile( !filename.endsWith(".graph") ? QString("%1.graph").arg(filename) : filename);

    if (!saveFile.open()) {
        kDebug() << "Error: File Not Open";
        return false;
    }

    QTextStream stream(&saveFile);
    stream.setCodec("UTF-8");

    int graphSize = count();

    for (int i = 0; i < graphSize; i++) {
        DataStructureBase *g = this->at(i);

        k_buf += QString("[Graph %1] \n").arg(i).toUtf8();

	    savePropertiesInternalFormat(g);

        foreach( ::Node *n, g->nodes()) {
	        k_buf += QString("[Node %1]\n").arg(g->nodes().indexOf(n)).toUtf8();
	        savePropertiesInternalFormat(n);
        }

        int from, to;
        foreach( Edge *e, g->edges()) {
            from = g->nodes().indexOf(e->from());
            to = g->nodes().indexOf(e->to());

	        k_buf += QString("[Edge %1->%2]\n").arg(from).arg(to).toUtf8();
	        savePropertiesInternalFormat(e);
        }

        /*     buf += " \n \n ############ GROUPS ########### \n \n";
             foreach( GraphGroup *gg, g->groups()) {
                 buf += QString("[Group %1] \n").arg((long) gg);

                 foreach( ::Node *n, gg->nodes() ) {
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

void GraphDocument::savePropertiesInternalFormat(QObject *o) {
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

void GraphDocument::loadFromInternalFormat(const QString& filename) {
    QFile f(filename);
    if ( !f.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        kDebug() << "File not open " << filename.toUtf8();
        return;
    }

    DataStructureBase* tmpGraph = 0;
   //GraphGroup *tmpGroup = 0;
    QObject *tmpObject = 0;


    QTextStream in(&f);
    in.setCodec("UTF-8");

    while (!in.atEnd()) {
        QString str = in.readLine().simplified();

        if (str.startsWith('#')) { //! Ignore it, commented line.
            continue;
        }

        else if (str.startsWith("[Graph")) {
            QString gName = str.section(' ',1,1);
            gName.remove(']');
            tmpGraph = Rocs::DSPluginManager::New()->createNewDS(this);
            tmpGraph->setName(gName.toAscii());
            tmpObject = tmpGraph;
            append(tmpGraph);
            kDebug() << "Graph Created";
        }

        else if (str.startsWith("[Node")) {
            QString nName = str.section(' ',1,1);
            nName.remove(']');
            tmpObject = tmpGraph->addNode(nName);
            kDebug() << "Node Created";
        }

        else if (str.startsWith("[Edge")) {
            QString eName = str.section(' ',1,1);
            eName.remove(']');

            QString nameFrom = eName.section("->", 0,0);
            QString nameTo = eName.section("->", 1,1);

            tmpObject = tmpGraph->addEdge(tmpGraph->nodes()[nameFrom.toInt()], tmpGraph->nodes()[nameTo.toInt()]);
            kDebug() << "Edge Created";
        }
        else if (str.startsWith("[Group")) {
            /*QString gName = str.section(" ",1,1);
            gName.remove(']');
            tmpGroup = tmpGraph->addGroup(gName); */
        }
        else if (str.contains(':')) {
            QString propertyName = str.section(':',0,0).trimmed();
            QString propertyValue = str.section(':',1,1).trimmed();
            tmpObject->setProperty( propertyName.toUtf8() , propertyValue );
        }
        else {
//            // tmpGroup->append( tmpGraph->node(str));
        }
    }
    kDebug() << "Graph Document Loaded.";
}

void GraphDocument::convertToDS(QString newDS){
    if (newDS != _DSType){
        kDebug() << "Need convert doc from " << _DSType << " to "<< newDS ;
//         GraphDocument * gDoc = new GraphDocument(*this);
        _DSType = newDS;
        int numGraphs = count();
        for (int i = 0 ; i < numGraphs; ++i){
            DataStructureBase * g = Rocs::DSPluginManager::New()->changeToDS(at(i));
            if (at(i) == _activeGraph)
              _activeGraph = g;
            append(g);
        }
        for (int i = 0 ; i < numGraphs; ++i){
            at(0)->deleteLater();
            removeAt(0);
        }
//         return gDoc;
    }
//     return 0;
}
