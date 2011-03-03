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

#include "GraphStructure.h"
#include "KDebug"
#include "Data.h"
#include "Pointer.h"
#include "Document.h"
#include "DataStructure.h"

Rocs::GraphStructure::GraphStructure ( Document* parent ) : DataStructure ( parent ) {

}

Rocs::GraphStructure::GraphStructure(DataStructure& other, Document* parent): DataStructure(other, parent)
{
    setDirected(false);
}


Rocs::GraphStructure::~GraphStructure() {

}


QScriptValue Rocs::GraphStructure::list_nodes() {
    QScriptValue array = engine()->newArray();
    foreach(Data* n, dataList()) {
        array.property("push").call(array, QScriptValueList() << n->scriptValue());
    }
    return array;
}

QScriptValue Rocs::GraphStructure::list_edges() {
    QScriptValue array = engine()->newArray();
    foreach(Pointer* n, pointers()) {
        array.property("push").call(array, QScriptValueList() << n->scriptValue());
    }
    return array;
}

QScriptValue Rocs::GraphStructure::add_node(const QString& name) {
    Data* n = addData(name);
    n->setEngine(engine());
    return n->scriptValue();
}

QScriptValue Rocs::GraphStructure::add_edge(Data* from, Data* to) {
    Pointer *e = addPointer(from, to);
    if (e){
      e->setEngine(engine());
      return e->scriptValue();
    }

    return QScriptValue();
}

QScriptValue Rocs::GraphStructure::node_byname(const QString& name) {
    Data *n = data(name);
    return n->scriptValue();
}

void Rocs::GraphStructure::setDirected(bool directed)
{
    DataStructure::setDirected(directed);
    qDebug() << "Implement the rest...";
    //TODO redraw, set layout
}

bool Rocs::GraphStructure::directed()
{
    return DataStructure::directed();
}
