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

#include "graph.h"
#include "edge.h"
#include "node.h"
#include "qtScriptBackend.h"
#include "graphGroups.h"
#include <KDebug>

Graph::Graph() : QObject(0){
  _directed = false;
}

Graph::~Graph(){
  
  foreach(Edge* e,  _edges){
    remove(e);
  }
  
 foreach(Node* n, _nodes){
   remove(n);
 }
}
  
QList<Node*> Graph::nodes() const{
  return _nodes;
}

QList<Edge*> Graph::edges() const{
  return _edges;
}

Node* Graph::addNode(QString name){
  Node  *n = new Node(this);
  n->setProperty("name", name);
  _nodes.append( n );
  emit nodeCreated(n);
  return n;
}

Edge* Graph::addEdge(Node* from,Node* to){
  if( ( from == to) && ( !_directed ) ){
    return 0;  
  }else if ((from->edges(to).size() >= 1)&&(!_directed)){
    return 0;
  }else if((_nodes.indexOf(from) == -1) || (_nodes.indexOf(to) == -1)){
    return 0;
  }

  Edge *e  = new Edge(this, from, to);
  
  if ( from == to ){
    from -> addSelfEdge(e);
  }
  else{
    from -> addOutEdge(e);
    to -> addInEdge(e);
  }
  _edges.append( e );
  return e;
}

Edge* Graph::addEdge(const QString& name_from, const QString& name_to){
  Node *from = 0;
  Node *to   = 0;
  
   QString tmpName;

  foreach( Node* n,  _nodes){
    tmpName = n->property("name").toString();
		
    if (tmpName == name_from){  from = n; }
    if (tmpName == name_to){    to = n;   }
    if ((to != 0) && (from != 0)){  break;  }
  }

  return addEdge(from, to);
}

bool Graph::directed() const{
  return _directed;
}

Node* Graph::node(const QString& name){
  QString tmpName;
  foreach( Node* n,  _nodes){
  tmpName = n->property("name").toString();
    if (tmpName == name){
      return n;
    }
  }
  return 0;
}

void Graph::remove(Node *n){
  _nodes.removeOne( n  );
   delete n;
}

void Graph::remove(Edge *e){
  _edges.removeOne( e );
  foreach(Node *n, _nodes){
      n->removeEdge(e, 0);
      n->removeEdge(e, 1);
      n->removeEdge(e, 2);
  }
  delete e;
}

void Graph::setDirected(bool directed){
	_directed = directed;
	if(!_directed){
		return;
	}
	
	foreach(Node *n1, _nodes){
		foreach(Node *n2, n1->adjacent_nodes()){
			if (n1->edges(n2).size() == 1){
				continue;
			}

			QList<Edge*> listEdges = n1->edges(n2);
			if (n1 != n2){
				listEdges.removeFirst();
				kDebug()  << "Chamou Update Pos";
			}
			foreach(Edge *e, listEdges){
				remove(e);
			}
		}
	}

	return;
}

GraphGroup* Graph::addGroup(const QString& name){
	GraphGroup *gg = new GraphGroup();
	gg->setName(name);
	_graphGroups.append(gg);
	return gg;
}

QList<GraphGroup*> Graph::groups() const{
	return _graphGroups;
}


#ifdef USING_QTSCRIPT

QScriptValue Graph::scriptValue() const{
	return _value;
}

void Graph::setEngine(	QtScriptBackend *engine ){
	_engine = engine;

	_value = _engine->newQObject(this);

	if ( property("name") != QVariant() ){
    _engine->globalObject().setProperty(property("name").toString(), _value);
  }

	foreach(Node *n, _nodes){
		n->setEngine(engine);
	}
	foreach(Edge *e, _edges){
		e->setEngine(engine);
	}
	foreach(GraphGroup *g, _graphGroups){
		QScriptValue array = _engine->newArray();
		foreach(Node* n, (*g) ){
			array.property("push").call(array, QScriptValueList() << n->scriptValue());
		}
		_engine->globalObject().setProperty(g->name(), array);
	}
}

QScriptValue Graph::list_nodes(){
	QList<Node*> list = nodes();
	QScriptValue array = _engine->newArray();
	foreach(Node* n, list){
		array.property("push").call(array, QScriptValueList() << n->scriptValue());
	}
	return array;
}

QScriptValue Graph::list_edges(){
	QList<Edge*> list = edges();
	QScriptValue array = _engine->newArray();
	foreach(Edge* n, list){
		array.property("push").call(array, QScriptValueList() << n->scriptValue());
	}
	return array;
}

QScriptValue Graph::add_node(const QString& name){
	Node* n = addNode(name);
	n->setEngine(_engine);
	return n->scriptValue();
}

QScriptValue Graph::add_edge(const QString& from, const QString& to){
	Edge *e = addEdge(from, to);
	e->setEngine(_engine);
	return e->scriptValue();
}

QScriptValue Graph::node_byname(const QString& name){
	Node *n = node(name);
	return n->scriptValue();
}
#endif