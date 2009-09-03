/***************************************************************************
 * main.cpp
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/
#ifndef GRAPH_H
#define GRAPH_H

#ifndef USING_QTSCRIPT
#define USING_QTSCRIPT 1
#endif

#include <QObject>
#include <QList>
#include <QVariant>
#include <QString>

#ifdef USING_QTSCRIPT
#include <QtScript>
#endif

#include "node.h"
// #include "graphGroups.h"


class Edge;
class QtScriptBackend;
class GraphGroup;
class GraphDocument;


/*! \brief this class acts as a container for nodes and edges.
  this class has all the edges and nodes, plus a few extra funcionalities
  to help programming with it on the programming backend. */
class Graph : public QObject {
    Q_OBJECT

    /*! this property defines if the graph is directed or simple */
    Q_PROPERTY(bool directed READ directed WRITE setDirected);

    /*! this property holds the name of the graph */
    Q_PROPERTY(QString name READ name WRITE setName);

    /*! this property holds the default color for new nodes */
    Q_PROPERTY(QString nodeDefaultColor READ nodeDefaultColor WRITE setNodeDefaultColor)
    
    /*! this property holds the default color for new nodes */
    Q_PROPERTY(QString edgeDefaultColor READ edgeDefaultColor WRITE setEdgeDefaultColor)
    
    /*! this property tells the system if this graph is to be treated as an automate */
    Q_PROPERTY(bool automate READ automate WRITE setAutomate)
public:
    /*! constructo
      \p parent the parent QObject
    */
    Graph(GraphDocument *parent = 0);

    /*! destructor */
    ~Graph();

    /*! calculates the relative center of the graph,
      taking into account the top, bottom, left and right edges. */
    void calcRelativeCenter();

    /*! gets the precalculated relative center. 
      \return the center of the graph.
      */
    QPointF relativeCenter() const;
    
    /*! gets the document that holds this graph. 
      \return the GraphDocument
    */
    GraphDocument *document() const;
    
    /*! remove this graph from the document. */
    void remove();
    
#ifdef USING_QTSCRIPT
    /*! if QtScript is enabled for this rocs,
    \return  the self-referenced Script Value for this graph. */
    QScriptValue scriptValue() const;

    /*! if QtScript is enabled for this rocs, 
    \p engine is the QtScript engine that will be set for this graph. */
    void setEngine( QtScriptBackend *engine );
#endif

public  slots:
    /*! sets the name of the graph.
    \p s the new name of the graph. */
    void setName(const QString& s);

    /*! gets the name of the graph. 
    \return the name of the graph */
    const QString& name() const;

    /*! sets if the graph is directed or simple node 
    \param directed true directed, false simple. FIXME: change that to enum.
    */
    void setDirected(bool directed = true);

    /*! gets if the node is diirected of simple.
    \return true if directed, false if simple. FIXME: change that to enum. 
    */
    bool directed() const;

    /*! all nodes are accessible from that method. 
      \return the QList containing all the  nodes.
    */    
    QList<Node*> nodes() const;


    /*! all edges are accessible from that method. 
      \return the QList containing all the edges.
    */    
    QList<Edge*> edges() const;

    
    /*! all groups are accessible from that method. 
      \return the QList containing all the  groups.
    */    
    QList<GraphGroup*> groups() const;

    /*! get the node marked as 'begin' 
    \return the 'begin' node. (node.begin == true), NULL if there isn't a begin node.
    */
    Node* begin() const;

    /*! sets a node as begin. 
      \param b the new begin of the graph.
    */
    void setBegin(Node* b);

    /*! creates a new node. 
    \p name the name of the node.
    \return the newly created node.
    */
    Node* addNode(QString name);
    
    /*! creates a new edge 
    \p from the 'from' node.
    \p to the 'to' node.
    \return the newly created edge.
    */
    Edge* addEdge(Node* from,Node* to);
    
    /*! creates a new edge 
    \p name_from the name of the node that will be used as 'from' 
    \p name_to the name of the node that will be used as 'to'
    \return the newly created edge.
    */
    Edge* addEdge(const QString& name_from, const QString& name_to);
    
    /*! returns a node
    \p name the name of the node to be returned.
    \return a node, or null if an invalid name is provided. 
    */
    Node* node(const QString& name = "Untitled");

    /*! removes a node.
    \p n the node to be removed. 
    */
    void remove(Node *n);
    
    /*! removes an edge 
    \p e the edge to be removed. 
    */
    void remove(Edge *e);
    
    /*! adds a graph - group to the graph. FIXME: not implemented yet. 
    \p name the name of the group.
    \return the newly created graph group.
    */
    GraphGroup *addGroup(const QString& name);

    /*! \brief add a new node to the 'end' list.
	end list is an QList that will hold every node as a final value.
	\p n the node to be added.
	\return the recently added node. 
    */
    Node* addEnd(Node* n);
    
    /*! remove a node from the end's list.
	\p n the node to be removed from the list.
    */
    void  removeEnd(Node* n);
    
    void setNodeDefaultColor(const QString& color);
    const QString& nodeDefaultColor() const;
    
    void setEdgeDefaultColor(const QString& color);
    const QString& edgeDefaultColor() const;
    
    void setAutomate(bool b);
    bool automate();
    
#ifdef USING_QTSCRIPT
    QScriptValue list_nodes();
    QScriptValue list_edges();
    QScriptValue add_node(const QString& name);
    QScriptValue add_edge(Node* from, Node* to);
    QScriptValue node_byname(const QString& name);
    QScriptValue begin_node();
    QScriptValue end_nodes();
#endif

signals:
    void nodeCreated(Node *n);
    void edgeCreated(Edge *e);
    void complexityChanged(bool directed);
    
private:
    bool _directed;
    QList<Node*> _nodes;
    QList<Edge*> _edges;
    QList<GraphGroup*> _graphGroups;
    qreal _top, _bottom, _left, _right;
    QPointF _relativeCenter;
    QString _name;
    Node* _begin;
    QList<Node*> _ends;
    QString _nodeDefaultColor;
    QString _edgeDefaultColor;
    bool _automate;
    GraphDocument *_document;
    
#ifdef USING_QTSCRIPT
    QScriptValue _value;
    QtScriptBackend *_engine;
#endif

};

#endif
