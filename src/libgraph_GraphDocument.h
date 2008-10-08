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

/*! \brief the graph document 
  long explanation needed.
*/
class GraphDocument : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName)
  Q_PROPERTY(qreal width READ width WRITE setWidth)
  Q_PROPERTY(qreal height READ height WRITE setHeight)

public:
  /*! Some Simple Enum 
    \enum simple defines a simple graph.
    \enum multi defines a multi graph
    \enum digraph defines a digraph.
  */
  enum{ simple, multi, digraph };

  /*! Default Constructor 
    \param name sets the name of the document.
  */
  GraphDocument(const QString name = QString());
  
  /*! Default Destructor */
  ~GraphDocument();

  /*! Sets the current file name of the Graph Collection 
    \param name changes the name of the document.
  */
  void setName(const QString& name);
  
  /*! \return the Name of the Collection of Graphs */
  QString name() const;

  /*! set the height of the working area 
      \param height the new height of the document.
  */
  void setHeight(qreal height);

  /*! \return the heigth of the working area */
  qreal height() const;

  /*! set the width of the working area 
    \param width the new width of the working area.
  */
  void setWidth(qreal width);

  /*! \return the width of the working area */
  qreal width() const;

  /*! \return the size of the graph Collection */
  int size(); 

  /*! \return the Graph at position 'i' on the _graphs list */
  Graph* operator[](int i); 

  /*! \return the graph at position i on the _graphs list */
  Graph* at(int i);

  /*! Append a grpah on the graph's list 
    \param g the graph to be appende.
  */
  void operator<<(Graph *g);

  /*! Append a graph on the Graph's list 
    \param g the graph to be appended
  */
  void append(Graph *g);

  /*! Prepend a graph on the Graph's list 
    \param g the graph to be prepended.
  */
  void prepend(Graph *g);

  /*! get the graph's list */
  QList<Graph*> graphs() const;
  
  /*! Removes the graph at position 'i' 
    \param i the position to remove the graph.
  */
  void removeAt(int i = 0); 

  /*! Remove the first instance of the graph g 
    \param g the instance of the graph to be removed.
  */
  void removeOne(Graph *g);

  /*! search for the index of a graph.
    \param g the graph that you're looking for the index
    \return the index of the graph g or -1 if not foind
  */
  int indexOf(Graph *g);

  /*! destroy all the graphs */
  void clear();

  /*! sets the current script on this file 
    \param script the new script of the file.
  */
  void setScript(const QString& script);

  /*! \return the current script on this file */
  QString script();

public slots:
  /*! Creates a new Graph 
    \param name the name of the graph
    \param type the the of the graph. 
  */
  void addGraph(QString name = "", int type = 0);

signals:
  /*! emited when a new graph is created 
    \param g the created graph */
  void graphCreated(Graph *g);

  /*! emitted when a graph is removed.
    \param i the index of the removed graph */
  void graphRemoved(int i);

  /*! emitted when the document changes it's name. 
    \param name the new name of the document */
  void nameChanged(QString name);

  /*! emitted when the document changes it's height 
    \param height the new height of the document */
  void heightChanged(qreal height);

  /*!  emitted when the document changes it's width
    \param width the new width of the document */
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
