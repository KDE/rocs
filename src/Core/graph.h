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

#ifdef USING_QTSCRIPT
#include <QtScript>
#endif

// #include "graphGroups.h"

class Node;
class Edge;
class QtScriptBackend;
class GraphGroup;

class Graph : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool directed READ directed WRITE setDirected);

public:
    Graph();
    ~Graph();
    
#ifdef USING_QTSCRIPT
    QScriptValue scriptValue() const;
    void setEngine( QtScriptBackend *engine );
#endif

public  slots:
    void setDirected(bool directed = true);
    bool directed() const;
    QList<Node*> nodes() const;
    QList<Edge*> edges() const;
    QList<GraphGroup*> groups() const;

#ifdef USING_QTSCRIPT
    QScriptValue list_nodes();
    QScriptValue list_edges();
    QScriptValue add_node(const QString& name);
    QScriptValue add_edge(const QString& from, const QString& to);
    QScriptValue node_byname(const QString& name);
#endif

    Node* addNode(QString name);
    Edge* addEdge(Node* from,Node* to);
    Edge* addEdge(const QString& name_from, const QString& name_to);
    Node* node(const QString& name);

    void remove(Node *n);
    void remove(Edge *e);
    GraphGroup *addGroup(const QString& name);

signals:
    void nodeCreated(Node *n);
    void edgeCreated(Edge *e);
    void directionChanged(bool directed);

private:
    bool _directed;
    QList<Node*> _nodes;
    QList<Edge*> _edges;
    QList<GraphGroup*> _graphGroups;

#ifdef USING_QTSCRIPT
    QScriptValue _value;
    QtScriptBackend *_engine;
#endif

};

#endif
