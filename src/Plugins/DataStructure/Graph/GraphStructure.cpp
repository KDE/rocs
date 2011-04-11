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

Rocs::GraphStructure::GraphStructure ( Document* parent ) :
    DataStructure ( parent ),
    _directed(false)
{

}

Rocs::GraphStructure::GraphStructure(DataStructure& other, Document* parent): DataStructure(other, parent)
{
    QHash <Data*, Data* > dataTodata;
    foreach(Data* n, other.dataList()){
        Data* newdata = addData(n->name());
        newdata->setColor(n->color());
        newdata->setValue(n->value());
        newdata->setX(n->x());
        newdata->setY(n->y());
        newdata->setWidth(n->width());
        dataTodata.insert(n, newdata);
    }
    foreach(Pointer *e, other.pointers()){
        Data* from =  dataTodata.value(e->from());
        Data* to =  dataTodata.value(e->to());

        Pointer* newPointer = addPointer(from, to);
        newPointer->setColor(e->color());
        newPointer->setValue(e->value());
    }
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
    _directed = directed;
    foreach(Pointer* pointer, pointers()) {
       pointer->emitChangedSignal();
    }
}

bool Rocs::GraphStructure::directed()
{
    return _directed;
}

Pointer* Rocs::GraphStructure::addPointer(Data *from, Data *to) {
    if (from->dataStructure()->readOnly()) return 0;

    if ( from == 0 || to == 0 ) {
        return 0;
    }

    if (!directed()) {
        // self-edges
        if (from == to) {
            return 0;
        }
        // back-edges
        if ( from->pointers(to).size() >= 1 ) {
            return 0;
        }
    }

    return DataStructure::addPointer(from, to);
}