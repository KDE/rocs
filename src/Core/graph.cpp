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
#include "graphDocument.h"
#include "DynamicPropertiesList.h"
#include <KDebug>
#include <QColor>

Graph::Graph(GraphDocument *parent) : QObject(parent) {
    _directed = false;
    _automate = false;
    _readOnly = false;
    _document = parent;
    _begin = 0;
    calcRelativeCenter();
    _nodeDefaultColor = "blue";
    _edgeDefaultColor = "gray";
    _nodeNamesVisible = true;
    _nodeValuesVisible = true;
    _edgeNamesVisible = false;
    _edgeValuesVisible = true;
}

Graph::Graph(Graph& other): QObject(other.parent()){
    _directed = other._directed;
    _automate = other._automate;
    _readOnly = other._readOnly;
    _document = other._document;
    _begin = other._begin;
    calcRelativeCenter();
    _nodeDefaultColor = other._nodeDefaultColor;
    _edgeDefaultColor = other._edgeDefaultColor;
    _nodeNamesVisible = other._nodeNamesVisible;
    _nodeValuesVisible = other._nodeValuesVisible;
    _edgeNamesVisible = other._edgeNamesVisible;
    _edgeValuesVisible = other._edgeValuesVisible;
    QHash <Node*, Node* > nodeToNode;
    foreach(Node *n, other._nodes){
      Node* newNode = addNode(n->name());
      newNode->setColor(n->color());
      newNode->setValue(n->value());
      newNode->setX(n->x());
      newNode->setY(n->y());
      newNode->setWidth(n->width());
      nodeToNode.insert(n, newNode);

    }
    foreach(Edge *e, other._edges){
      Node* from =  nodeToNode.value(e->from());
      Node* to =  nodeToNode.value(e->to());

      Edge* newEdge = addEdge(from, to);
      newEdge->setColor(e->color());
      newEdge->setValue(e->value());
    }
}


Graph::~Graph() {
    foreach(Edge* e,  _edges) {
        remove(e);
    }

    foreach(Node* n, _nodes) {
        remove(n);
    }
}

GraphDocument *Graph::document() const {
    return _document;
}

void Graph::remove() {
  _document->removeOne(this);
  delete this;
}

QList<Node*> Graph::nodes() const {
    return _nodes;
}

QList<Edge*> Graph::edges() const {
    return _edges;
}

void Graph::setNodesColor(QString c){
  kDebug() << "Entrou no setNodesColor, com a cor " << c;
  foreach(Node *n, _nodes) {
        n->setColor(c);
    }
}

void Graph::setEdgesColor(QString c){
    kDebug() << "Entrou no setEdgesColor, com a cor " << c;
    foreach(Edge *e, _edges) {
        e->setColor(c);
    }
}

Node* Graph::addNode(QString name) {
    if (_readOnly) return 0;

    Node  *n = new Node(this);
    n->setName(name);
    _nodes.append( n );
    emit nodeCreated(n);
    connect (n, SIGNAL(changed()), this, SIGNAL(changed()));
    return n;
}

void Graph::addNode(QString name, QPointF pos){
    Node *node = addNode(name);
    node->setPos(pos.x(), pos.y());
}

Edge* Graph::addEdge(Node* from,Node* to) {
    if (_readOnly) return 0;

    if ( from == 0 || to == 0 ) {      return 0;   }


    if ( ( from == to) && ( !_directed ) ) {
        return 0;
    } else if ((from->edges(to).size() >= 1)&&(!_directed)) {
        return 0;
    } else if ((_nodes.indexOf(from) == -1) || (_nodes.indexOf(to) == -1)) {
        return 0;
    }

    Edge *e  = new Edge(this, from, to);
    _edges.append( e );
    emit edgeCreated(e);
    connect (e, SIGNAL(changed()), this, SIGNAL(changed()));
    return e;
}

Edge* Graph::addEdge(const QString& name_from, const QString& name_to) {
    if (_readOnly) return 0;
    Node *from = 0;
    Node *to   = 0;

    QString tmpName;

    foreach( Node* n,  _nodes) {
        tmpName = n->name();

        if (tmpName == name_from) {
            from = n;
        }
        if (tmpName == name_to) {
            to = n;
        }
        if ((to != 0) && (from != 0)) {
            break;
        }
    }

    return addEdge(from, to);
}

bool Graph::directed() const {
    return _directed;
}

Node* Graph::node(const QString& name) {
    QString tmpName;
    foreach( Node* n,  _nodes) {
        tmpName = n->name();
        if (tmpName == name) {
            return n;
        }
    }
    return 0;
}

void Graph::remove(Node *n) {
    _nodes.removeOne( n  );
    n->deleteLater();
}

void Graph::remove(Edge *e) {
    _edges.removeOne( e );
    e->deleteLater();
//     delete e;
}

void Graph::setDirected(bool directed) {

    foreach(Node *n1, _nodes) {
        foreach(Node *n2, n1->adjacent_nodes()) {
	    // do not permit loop nodes while changing graph's state.
            if ( (n1->edges(n2).size() == 1) && (n1 != n2) ) {
                continue;
            }

            QList<Edge*> listEdges = n1->edges(n2);
            if (n1 != n2) {
                listEdges.removeFirst();
            }

            foreach(Edge *e, listEdges) {
	      remove(e);
            }
        }
    }
    foreach(Edge *e, _edges){
      e->emitChangedSignal(); // dummy updater.
    }
    _directed = directed;
    emit complexityChanged(directed);
    return;
}

GraphGroup* Graph::addGroup(const QString& name) {
    GraphGroup *gg = new GraphGroup();
    gg->setName(name);
    return gg;
}

//QList<GraphGroup*> Graph::groups() const {
//    return _graphGroups;
//}

void Graph::calcRelativeCenter() {
    /*
      _top = _nodes[0]->y();   _bottom = _nodes[0]->y();
      _left = _nodes[0]->x(); _right = _nodes[0]->x();
      kDebug() << "INIT:";
      kDebug() << "TOP : " << _top << " BOTTOM " << _bottom << "LEFT" << _left << "RIGHT" << _right;
      for (int counter = 0; counter < _nodes.size(); counter++) {
       if(_nodes[counter]->x() > _right)  _right = _nodes[counter]->x();
       if(_nodes[counter]->y() > _top)  _top = _nodes[counter]->y();
       if(_nodes[counter]->x() < _left)  _left = _nodes[counter]->x();
       if(_nodes[counter]->y() < _bottom)  _bottom = _nodes[counter]->y();
           kDebug() << "TOP : " << _top << " BOTTOM " << _bottom << "LEFT" << _left << "RIGHT" << _right;
      }
      */
    /// this will be here till I find a better way to calculate a *relative* center of the graph, and not the center of the document.
    if (parent() != 0){
	GraphDocument *gd = qobject_cast<GraphDocument*>(parent());
	_relativeCenter.setY(gd->height()/2);
	_relativeCenter.setX(gd->width()/2);
    }else{
        _relativeCenter.setY(0);
	_relativeCenter.setX(0);
    }
}

QPointF Graph::relativeCenter() const {
    return _relativeCenter;
}

const QString& Graph::name() const {
    return _name;
}
void Graph::setName(const QString& s) {
    _name = s;
}

bool Graph::setBegin(Node* n) {
  if (!n){
    _begin = 0;
    return false;
  }

  if (!_begin){
      _begin = n;
      return true;
  }else if( _begin == n){
    return false;
  }

   _begin->setBegin(false);
   _begin = n;
    return true;
}

Node* Graph::begin() const {
    return _begin;
}

Node* Graph::addEnd(Node *n) {
    _ends.append(n);
    return n;
}

void Graph::removeEnd(Node *n) {
    _ends.removeAll(n);
}

void Graph::setNodeDefaultColor(const QString& color) {
    kDebug() << "Entrou com cor aqui painho." << color;
    _nodeDefaultColor = color;
}

const QString& Graph::nodeDefaultColor() const {
    return _nodeDefaultColor;
}

void Graph::setEdgeDefaultColor(const QString& color) {
    _edgeDefaultColor = color;
}
const QString& Graph::edgeDefaultColor() const {
    return _edgeDefaultColor;
}

void Graph::setAutomate(bool b) {
    _automate = b;
    emit automateChanged(b);
}
bool Graph::automate() {
    return _automate;
}

void Graph::addDynamicProperty(QString property, QVariant value){
    this->setProperty(property.toUtf8(), value);
    if (value.isValid()){
      DynamicPropertiesList::New()->addProperty(this, property);
    }
}

void Graph::removeDynamicProperty(QString property){
    this->addDynamicProperty(property, QVariant::Invalid);
    DynamicPropertiesList::New()->removeProperty(this, property);
}

void Graph::addNodesDynamicProperty(QString property, QVariant value){
    foreach(Node *n, _nodes){
      n->addDynamicProperty(property, value);
    }
}

void Graph::addEdgesDynamicProperty(QString property, QVariant value){
    foreach(Edge *e, _edges){
      e->addDynamicProperty(property, value);
    }
}

void Graph::removeNodesDynamicProperty(QString property){
  foreach(Node *n, _nodes){
    n->removeDynamicProperty(property);
  }
}
void Graph::removeEdgesDynamicProperty(QString property){
  foreach(Edge *e, _edges){
    e->removeDynamicProperty(property);
  }
}

void Graph::setNodeNameVisibility(bool b){
  _nodeNamesVisible = b;
  foreach(Node *n, _nodes){
    n->hideName(b);
  }
}

bool Graph::nodeNameVisibility(){
  return _nodeNamesVisible;
}

void Graph::setEdgeNameVisibility(bool b){
  _edgeNamesVisible = b;
  foreach(Edge *n, _edges){
    n->hideName(b);
  }
}

bool Graph::edgeNameVisibility(){
  return _edgeNamesVisible;
}

void Graph::setNodeValueVisibility(bool b){
  _nodeValuesVisible = b;
  foreach(Node *n, _nodes){
    n->hideValue(b);
  }
}
bool Graph::nodeValueVisibility(){
  return _nodeValuesVisible;
}

void Graph::setEdgeValueVisibility(bool b){
  _edgeValuesVisible = b;
  foreach(Edge *n, _edges){
    n->hideValue(b);
  }
}

bool Graph::edgeValueVisibility(){
  return _edgeValuesVisible;
}

#ifdef USING_QTSCRIPT




QScriptValue Graph::scriptValue() const {
    return _value;
}

// void Graph::setEngine(	QScriptEngine *engine ) {
//     _engine = engine;
//
//     _value = _engine->newQObject(this);
//
//     if (! _name.isEmpty() ) {
//         _engine->globalObject().setProperty(_name, _value);
//         kDebug() << _name << "Added as global object.";
//     }
//
//     foreach(Node *n, _nodes) {
//         n->setEngine(engine);
//     }
//     foreach(Edge *e, _edges) {
//         e->setEngine(engine);
//     }
// //    foreach(GraphGroup *g, _graphGroups) {
// //        QScriptValue array = _engine->newArray();
// //        foreach(Node* n, (*g) ) {
// //            array.property("push").call(array, QScriptValueList() << n->scriptValue());
// //        }
// //        _engine->globalObject().setProperty(g->name(), array);
// //    }
// }


#endif
