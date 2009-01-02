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
  
  foreach (Graph *g, gd){
    append(g);
  }
}

// Default Destructor
GraphDocument::~GraphDocument(){
  foreach(Graph *g, (*this)){
    delete g;
  }
  clear();
}


// Sets the current file name of the Graph Collection 
void GraphDocument::setName(const QString& name){
  _name = name;
  emit nameChanged(name);
}

// Gets the name of the Graph
QString GraphDocument::name() const{
  return _name;
}

// set the width of the drawable area
void GraphDocument::setWidth(qreal width){
  _width = width;
}

//set the height of the drawable area
void GraphDocument::setHeight(qreal height){
  _height = height;
}

// gets the wheight of the drawable area
qreal GraphDocument::height() const{
  return _height;
}

// sets the width of the drawable area
qreal GraphDocument::width() const{
  return _width;
}

Graph* GraphDocument::addGraph(QString name){
  Graph *g = new Graph();
  g->setProperty("name",name);
  append(g);
  return g;
}
