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
#include "graph.h"
#include <QString>
#include <KSaveFile>
#include <QByteArray>
#include <KDebug>
#include "node.h"
#include "edge.h"
#include "graphGroups.h"

// Default Constructor
GraphDocument::GraphDocument(const QString name, int width,  int height)
        : QObject(0), QList<Graph*>()
{
    _name = name;
    _width = width;
    _height = height;
    _modified = false;
}

GraphDocument::GraphDocument(const GraphDocument& gd)
        : QObject(0), QList<Graph*>()
{
    _name = gd.name();
    _width = gd.width();
    _height = gd.height();

    foreach (Graph *g, gd) {
        append(g);
    }
}

// Default Destructor
GraphDocument::~GraphDocument() {
    kDebug() << "Deleting Graph Document";
    kDebug() << this;
    kDebug() << size();
    
    for(int i = 0; i < size(); i ++){
	Graph *g = at(i);
	qDebug() << "Deleting graph" << g->name();
        delete g;
    }
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
Graph* GraphDocument::addGraph(QString name) {
    Graph *g = new Graph(this);
    g->setName(name);
    append(g);
    emit graphCreated(g);
    return g;
}


void GraphDocument::savedDocumentAt(const QString& fileName) {
    _lastSavedDocumentPath = fileName;
}

const QString& GraphDocument::documentPath() const {
    return _lastSavedDocumentPath;
}


bool GraphDocument::saveAsInternalFormat(const QString& filename) {
    buf.clear();
    KSaveFile saveFile;
    saveFile.setFileName( filename );
    qDebug() << filename;

    if (!saveFile.open()) {
        qDebug() << "Error: File Not Open";
        return false;
    }

    QTextStream stream(&saveFile);
    int graphSize = count();

    for (int i = 0; i < graphSize; i++) {
        Graph *g = this->at(i);
        buf += " \n \n ############ Graph  ########### \n \n";
        buf += QString("[Graph %1] \n").arg(i);

        savePropertiesInternalFormat(g);

        buf += " \n \n ############ NODES ########### \n \n";
        foreach( ::Node *n, g->nodes()) {
            buf += QString("[Node %1]\n").arg(g->nodes().indexOf(n));
            savePropertiesInternalFormat(n);
        }

        buf += " \n \n ############ EDGES ########### \n \n";
        foreach( Edge *e, g->edges()) {
            buf += QString("[Edge %1->%2]\n").arg(g->nodes().indexOf(e->from())).arg(g->nodes().indexOf(e->to()));
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
    qDebug() << buf.toAscii();

    stream << buf.toAscii();
    if (!saveFile.finalize()) {
        qDebug() << "Error, file not saved.";
        return false;
    }
    _lastSavedDocumentPath = filename;
    return true;
}

void GraphDocument::savePropertiesInternalFormat(QObject *o) {
    const QMetaObject *metaObject = o->metaObject();
    int propertyCount = metaObject->propertyCount();
    kDebug() << "Property Count: " << propertyCount;

    for ( int i = 0; i < propertyCount; ++i) {
        QMetaProperty metaProperty = metaObject->property(i);
        const char *name = metaProperty.name();
        if ( QString("objectName") == QString(name) ) {
            continue;
        }
        QVariant value = o->property(name);
        buf +=  QString("%1 : %2 \n" ).arg(name, value.toString());
    }

    QList<QByteArray> propertyNames = o->dynamicPropertyNames();
    foreach(const QByteArray& name, propertyNames) {
        QVariant value = o->property(name);
        buf +=  QString("%1 : %2 \n" ).arg(name, value.toString());
    }

    buf += '\n';
}

void GraphDocument::loadFromInternalFormat(const QString& filename) {
    QFile f(filename);
    if ( !f.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        qDebug  () << "File not open " << filename.toAscii();
        return;
    }

    Graph* tmpGraph = 0;
// //    GraphGroup *tmpGroup = 0;
    QObject *tmpObject = 0;

    while (!f.atEnd()) {
        QString str = f.readLine();
        str = str.simplified();

        if (str.startsWith('#')) { //! Ignore it, commented line.
            continue;
        }

        else if (str.startsWith("[Graph")) {
            QString gName = str.section(' ',1,1);
            gName.remove(']');
            tmpGraph = new Graph(this);
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
            tmpObject->setProperty( propertyName.toAscii() , propertyValue.toAscii() );
            kDebug() << "Property" << propertyName.toAscii() << "value" << propertyValue.toAscii();
        }
        else {
//            // tmpGroup->append( tmpGraph->node(str));
	}
    }
    kDebug() << "Graph Document Loaded.";
}
