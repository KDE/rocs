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
#ifndef GRAPHCOLLECTION_H
#define GRAPHCOLLECTION_H

#include <QList>
#include <QObject>

class QString;

namespace libgraph
{

class Graph;

class GraphDocument : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName)
  Q_PROPERTY(qreal width READ width WRITE setWidth)
  Q_PROPERTY(qreal height READ height WRITE setHeight)

public:
  /*! Some Simple Enum */
  enum{ simple, multi, digraph };

  /*! Default Constructor */
  GraphDocument(const QString name = QString());
  
  /*! Default Destructor */
  ~GraphDocument();

  /*! Sets the current file name of the Graph Collection */
  void setName(const QString& name);
  
  /*! Gets the Name of the Collection of Graphs */
  QString name() const;

  /*! set the height of the working area */
  void setHeight(qreal height);

  /*! gets the heigth of the working area */
  qreal height() const;

  /*! set the width of the working area */
  void setWidth(qreal width);

  /*! get the width of the working area */
  qreal width() const;

  /*! Gets the size of the graph Collection */
  int size(); 

  /*! Gets the Graph at position 'i' on the _graphs list */
  Graph* operator[](int i); 

  /*! Gets the graph at position i on the _graphs list */
  Graph* at(int i);

  /*! Append a grpah on the graph's list */
  void operator<<(Graph *g);

  /*! Append a graph on the Graph's list */
  void append(Graph *g);

  /*! Prepend a graph on the Graph's list */
  void prepend(Graph *g);

  /*! Creates a new Graph */
  Graph *addGraph(QString name = "", int type = 1);
  
  /*! Removes the graph at position 'i' */
  void removeAt(int i = 0); 

  /*! Remove the first instance of the graph g */
  void removeOne(Graph *g);

  /*! returns the index of the graph or -1 if not found */
  int indexOf(Graph *g);

  /*! destroy all the graphs */
  void clear();

  /*! sets the current script on this file */
  void setScript(const QString& script);

  /*! gets the current script on this file */
  QString script();

signals:
  void graphCreated(Graph *g);
  void graphRemoved(int i);
  void nameChanged(QString name);
  void heightChanged(qreal height);
  void widthChanged(qreal width);

private:
  QList<Graph*> _graphs;
  QString _name;
  QString _script;
  qreal _width;
  qreal _height;
};

}
#endif
