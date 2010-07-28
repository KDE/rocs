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

Node* Rocs::ListStructure::addNode ( QString name ) {
    return Graph::addNode ( name );
}

QScriptValue Rocs::ListStructure::front() {
  return begin()->scriptValue();
}

void Rocs::ListStructure::pointTo ( Node* node) {
    setBegin(node);
}
