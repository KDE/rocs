/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "ListStructure.h"
#include "KDebug"

Rocs::ListStructure::ListStructure ( GraphDocument* parent ) : Graph ( parent ) {
  setDirected(true);
}

Rocs::ListStructure::~ListStructure() {

}


Edge* Rocs::ListStructure::addEdge ( Node* from, Node* to ) {
    foreach(Edge *e, from->adjacent_edges()){
      remove(e);
    }
    return Graph::addEdge ( from, to );
}

void Rocs::ListStructure::setEngine ( QScriptEngine* engine )
{
    _engine = engine;

    _value = _engine->newQObject(this);

    if (! name().isEmpty() ) {
        _engine->globalObject().setProperty(name(), _value);
        kDebug() << name() << "It's a Linked List!.";
    }

    foreach(Node *n, nodes()) {
        n->setEngine(engine);
    }
    foreach(Edge *e, edges()) {
        e->setEngine(engine);
    }
}


Node* Rocs::ListStructure::addNode ( QString name ) {
    return Graph::addNode ( name );
}

QScriptValue Rocs::ListStructure::front() {
  return begin()->scriptValue();
}

void Rocs::ListStructure::pointTo ( Node* node) {
    setBegin(node);
}
