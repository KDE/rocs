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

#include "node.h"
#include "edge.h"
#include "graph.h"
#include <QDebug>

Node::Node(QObject *parent) : QObject(parent)
{
}

Node::~Node(){
  emit removed();
  empty(&_in_edges);
  empty(&_out_edges);
  empty(&_self_edges);
}

void Node::empty(QList<Edge*> *list){
  Graph *g = qobject_cast<Graph*>( QObject::parent() );
  foreach(Edge *e, (*list) ){
    g->remove(e);
  }
}

QList<Node*> Node::adjacent_nodes() const
{
  QList<Node*> adjacent;
  
  
  foreach(Edge *e, _out_edges){
    adjacent.append( e->to()  );
  }
  Graph *g = qobject_cast<Graph*>( QObject::parent()  );
  if ( g -> directed() ){
    foreach(Edge *e, _self_edges){
      adjacent.append( e->to() );
    }  
    return adjacent;
 }

 foreach(Edge *e, _in_edges){
    adjacent.append( e->from() );
 }
 return adjacent;
}


QList<Edge*> Node::adjacent_edges() const
{
  QList<Edge*> adjacent;
  

  foreach(Edge* e, _out_edges){
    adjacent.append( e );
  }
  Graph *g = qobject_cast<Graph*>( QObject::parent()  );
  
  if ( g -> directed() ){
    foreach(Edge *e, _self_edges){
      adjacent.append( e );
    }  
    return adjacent;
  }
  
  foreach(Edge *e, _in_edges){
    adjacent.append( e );
  } 

  return adjacent;
}

void Node::addInEdge(Edge *e){
  _in_edges.append( e );
}

void Node::addOutEdge(Edge *e){
  _out_edges.append( e  );
}

void Node::addSelfEdge(Edge *e){
  _self_edges.append( e );
}

QList<Edge*> Node::in_edges() const{
  return _in_edges;
}

QList<Edge*> Node::out_edges() const {
  return _out_edges;
}

QList<Edge*> Node::self_edges() const {
  return _self_edges;
}

Edge* Node::addEdge(Node* to){
  Graph *g = qobject_cast<Graph*>( QObject::parent()  );
  return g->addEdge(this, to);
}

void Node::removeEdge(Edge *e, int edgeList){
  switch(edgeList){
    case In  : removeEdge(e, &_in_edges); 
    case Out : removeEdge(e, &_out_edges);
    case Self: removeEdge(e, &_self_edges);
  }
}

void Node::removeEdge(Edge *e, QList<Edge*> *list){
    foreach(Edge* tmp, (*list) ){
    if (tmp == e){
      (*list).removeOne( e );
    }
  }
}

QList<Edge*> Node::edges(Node *n){
  QList<Edge*> list;
  if (n == this){
    foreach(Edge *tmp, _self_edges){
	list.append(tmp);
    }  
    return list;
  }
  foreach (Edge *tmp, _out_edges){
    if (tmp->to() == n){
      list.append(tmp);
    }
  }
  foreach(Edge *tmp, _in_edges){
    if (tmp->from() == n){
      list.append(tmp);
    }
  }
  return list;
}

#ifdef USING_QTSCRIPT

QScriptValue Node::scriptValue() const{
	return _value;
}

void Node::setEngine(	QtScriptBackend *engine ){
	_engine = engine;
	_value = engine->newQObject(this);
}

QScriptValue Node::adj_nodes(){
	QList<Node*> list = adjacent_nodes();
	QScriptValue array = _engine->newArray();
	foreach(Node* n, list){
		array.property("push").call(array, QScriptValueList() << n->scriptValue());
	}
	return array;
}

QScriptValue Node::adj_edges(){
	QList<Edge*> list = adjacent_edges();
	return createScriptArray(list);
}

QScriptValue Node::input_edges(){
	QList<Edge*> list = in_edges();
	return createScriptArray(list);
}

QScriptValue Node::output_edges(){
	QList<Edge*> list = out_edges();
	return createScriptArray(list);
}

QScriptValue Node::loop_edges(){
	QList<Edge*> list = out_edges();
	return createScriptArray(list);
}


QScriptValue Node::connected_edges(Node *n){
	QList<Edge*> list = edges(n);
	return createScriptArray(list);
}

QScriptValue Node::createScriptArray(QList<Edge*> list){
	QScriptValue array = _engine->newArray();
	foreach(Edge* e, list){
		array.property("push").call(array, QScriptValueList() << e->scriptValue());
	}
	return array;
}

#endif