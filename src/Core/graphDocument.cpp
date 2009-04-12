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

#include "graph.h"
#include "graphDocument.h"
#include <QString>
#include <KSaveFile>
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
    foreach(Graph *g, (*this)) {
        delete g;
    }
    clear();
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

Graph* GraphDocument::addGraph(QString name) {
    Graph *g = new Graph();
    g->setProperty("name",name);
    append(g);
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

    foreach(Graph *g, (*this)) {
        buf += " \n \n ############ Graph  ########### \n \n";
        buf += "[Graph " + g->property("name").toString() + "] \n";

        savePropertiesInternalFormat(g);

        buf += " \n \n ############ NODES ########### \n \n";
        foreach( ::Node *n, g->nodes()) {
            buf += "[Node ";
            buf += n->property("name").toString();
            buf += "] \n";

            savePropertiesInternalFormat(n);
        }

        buf += " \n \n ############ EDGES ########### \n \n";
        foreach( Edge *e, g->edges()) {
            buf += QString("[Edge ");
            buf += e->from()->property("name").toString();
            buf += "->";
            buf += e->to()->property("name").toString();
            buf += QString("] \n");
            savePropertiesInternalFormat(e);
        }

        buf += " \n \n ############ GROUPS ########### \n \n";
        foreach( GraphGroup *gg, g->groups()) {
            buf += "[Group ";
            buf += gg->name();
            buf += "] \n";

            foreach( ::Node *n, g->nodes() ) {
                buf += n->property("name").toString();
                buf += "\n";
            }
        }
    }
    qDebug() << buf.toAscii();

    stream << buf.toAscii();
    if (!saveFile.finalize()) {
        qDebug() << "Error, file not saved.";
    }
    return true;
}

void GraphDocument::savePropertiesInternalFormat(QObject *o) {
    const QMetaObject *metaObject = o->metaObject();
    int propertyCount = metaObject->propertyCount();

    for ( int i = 0; i < propertyCount; ++i) {
        QMetaProperty metaProperty = metaObject->property(i);
        const char *name = metaProperty.name();

        if ( QString("objectName") == QString(name) ) {
            continue;
        }

        QVariant value = o->property(name);
        buf +=  name;
        buf += " : ";
        buf += value.toString();
        buf += "\n";
    }
}

void GraphDocument::loadFromInternalFormat(const QString& filename) {
    QFile f(filename);
    if ( !f.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        qDebug  () << "File not open " << filename.toAscii();
        return;
    }

    Graph *tmpGraph = 0;
    ::Node *tmpNode  = 0;
    Edge *tmpEdge = 0;
    GraphGroup *tmpGroup = 0;
    QObject *tmpObject = 0;


    while (!f.atEnd()) {
        QString str = f.readLine();
        str = str.simplified();

        if (str.startsWith("#")) { //! Ignore it, commented line.
            continue;
        }

        else if (str.startsWith("[Graph")) {
            QString gName = str.section(" ",1,1);
            gName.remove(']');

            tmpGraph = new Graph();
            tmpGraph->setProperty("name", gName.toAscii());
            tmpObject = tmpGraph;
            append(tmpGraph);
        }

        else if (str.startsWith("[Node")) {
            QString nName = str.section(" ",1,1);
            nName.remove(']');

            tmpNode = tmpGraph->addNode(nName);
            tmpNode->setProperty("name", nName);
            tmpObject = tmpNode;
        }

        else if (str.startsWith("[Edge")) {
            QString eName = str.section(" ",1,1);
            eName.remove(']');

            QString nameFrom = eName.section("->", 0,0);
            QString nameTo = eName.section("->", 1,1);

            tmpEdge = tmpGraph->addEdge(tmpGraph->node(nameFrom), tmpGraph->node(nameTo));
            tmpObject = tmpEdge;

        }
        else if (str.startsWith("[Group")) {
            QString gName = str.section(" ",1,1);
            gName.remove(']');
            tmpGroup = tmpGraph->addGroup(gName);
        }
        else if (str.contains(":")) {
            QString propertyName = str.section(":",0,0);
            QString propertyValue = str.section(":",1,1);
            tmpObject->setProperty( propertyName.toAscii() , propertyValue.toAscii() );
        }
        else {
            tmpGroup->append( tmpGraph->node(str));
        }
    }

}
