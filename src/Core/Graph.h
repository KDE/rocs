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

#ifndef GRAPH_H
#define GRAPH_H

#include <QList>
#include <QObject>
#include <QColor>
#include <QString>
#include <QVariant>
#include <QPointF>

class Edge;
class Node;
class GraphDocument;

class Graph : public QObject
{
  Q_OBJECT

public:
  
  Graph(GraphDocument *parent = 0); // Default Constructor
  Graph( Graph& g); // Copy constructor
  ~Graph(); // destructor
  void append(Node *n){ _nodes.append(n); } // add`s a new Node on the li
  void append(Edge *e){ _edges.append(e); }

  void removeEdge(Edge *e);
  void nodeCreatedEdge(Edge *e);//{ emit createdEdge(e); }

  bool checkExistence(Node* n); // check if a node ie from this graph.
  bool checkExistence(Edge *e); // check if a edge is from this graph.
public slots:
  
  virtual Node* addNode(qreal x, qreal y); // Creates a new node
  QList<Node*> addMultipleNodesCircular(int Number, qreal Radius, QPointF& pos); // creates Number nodes in a circular way
  QList<Node*> addMultipleNodesSquared(int Number, qreal spacing, QPointF& pos); // creates Number nodes in a Square

  QList<Node*> addKGraph(int Number, qreal Radius, QPointF& pos); // Creates a complete Graph
  QList<Node*> addWGraph(int Number, qreal Radius, QPointF& pos); // Creates a Wheel graph
  QList<Node*> addCGraph(int Number, qreal Radius, QPointF& pos); // Creates a circular graph

  void toKGraph(); // Transform this graph to a complete graph
  void toCGraph(); // Transform this graph to a Circular graph
  void toWGraph(); // Transform this graph to a Wheel graph

  bool isComplete(); // Check if this graph is complete
  bool isCircular(); // Check if this graph is circular
  
  virtual QList<QList<int > > ajdMatrix(); // Gets the adjacency Matrix
  QList< QList <int> > incMatrix(); // gets the incidency matrix

  Graph* clone(); // Creates a clone of this graph

   QList<Node*> nodes(); // get the nodes array
   QList<Edge*> edges(); // get the edges array
  
  Node* nodeAt(int index); // return the node at position index
  Edge* edgeAt(int index); // return the edge at position index

  bool simple()         { return _simple;   }  // check if this a simple or multiple graph
  const QString& name() { return _name;     }  // returh the graph`s name
  const QColor& color() { return _color;    } // return the graph`s color

  void setName(const QString& n){ _name = n; emit nameChanged(n);   } // sets the name of the graph
  void setSimple(bool s){ _simple = s;       emit simpleChanged(s); } // sets the graph to simple or multiple
  void setColor(QColor c) { _color = c;      emit colorChanged(c);  }// sets the color of the graph
  
signals:
  void nameChanged(const QString& n);  
  void simpleChanged(bool s);
  void colorChanged(QColor c);
  void nodeCreated(Node* node); 
  void edgeCreated(Edge* edge); 

protected:
  QString _name; // The graph`s name
  QColor _color; // The graph`s color
  bool _simple;
  QList<Node*> _nodes; // The graph`s nodes
  QList<Edge*> _edges; // The graph`s edges

};

Q_DECLARE_METATYPE(Graph*)

#endif
