/*
    This file is part of Rocs.
    Copyright 2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include <KMessageBox>
#include "GraphNode.h"

Rocs::GraphStructure::GraphStructure ( Document* parent ) :
    DataStructure ( parent )
{
    setGraphType(UNDIRECTED);
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
    setGraphType(UNDIRECTED);
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

void Rocs::GraphStructure::setGraphType(int type)
{
    if (_type == type)
        return;

    if ((_type  == MULTIGRAPH && type != MULTIGRAPH )
    || (_type == DIRECTED && type == UNDIRECTED)){
        if (KMessageBox::warningContinueCancel(0,i18n("This action will probably remove some edges. Do you want to continue?")) != KMessageBox::Continue){
            return;
        }
    }
    
    _type = static_cast<GRAPH_TYPE>(type);
    switch(_type){
    case UNDIRECTED :
        foreach(Data *data, dataList()){
            // Clear the 'self pointers', undirecetd graphs doesn't have self nodes.
            foreach(Pointer* p, data->self_pointers()){
               p->remove();
            }
            data->self_pointers().clear();
            
            // Clear the rest. there should be only one edge between two nodes.
            foreach(Data *data2, dataList()){
                if (data == data2){
                    continue;
                }
                
                bool foundOne = false;
                foreach (Pointer *tmp, data->out_pointers()) {
                    if (tmp->to() == data2) {
                        if (!foundOne){
                            foundOne = true;
                        }else{
                            data->out_pointers().removeOne(tmp);
                            data2->in_pointers().removeOne(tmp);
                            tmp->remove();
                        }
                    }
                }
                
                foreach(Pointer *tmp, data->in_pointers()) {
                    if (tmp->from() == data2) {
                        if (!foundOne){
                            foundOne = true;
                        }else{
                            data->in_pointers().removeOne(tmp);
                            data2->out_pointers().removeOne(tmp);
                            tmp->remove();
                        }
                    }
                }
            }
        } break;
    case DIRECTED :
       foreach(Data *data, dataList()){
            // Just One self pointer allowed.
            bool foundSelfEdge = false;
            foreach(Pointer* p, data->self_pointers()){
               if (!foundSelfEdge){
                   foundSelfEdge = true;
               }else{
                   data->self_pointers().removeOne(p);
                   p->remove();
               }
            }
            
            // Just one going in, and one going out.
            foreach(Data *data2, dataList()){
                if (data == data2){
                    continue;
                }
                
                bool foundOneOut = false;
                foreach (Pointer *tmp, data->out_pointers()) {
                    if (tmp->to() == data2) {
                        if (!foundOneOut){
                            foundOneOut = true;
                        }else{
                            data->out_pointers().removeOne(tmp);
                            data2->in_pointers().removeOne(tmp);
                            tmp->remove();
                        }
                    }
                }
                
                bool foundOneIn = false;
                foreach(Pointer *tmp, data->in_pointers()) {
                    if (tmp->from() == data2) {
                        if (!foundOneIn){
                            foundOneIn = true;
                        }else{
                            data->in_pointers().removeOne(tmp);
                            data2->out_pointers().removeOne(tmp);
                            tmp->remove();
                        }
                    }
                }
            }
        } break;
    default: break;
        
    }
    
    foreach(Pointer* pointer, pointers()) {
       QMetaObject::invokeMethod(pointer, "changed");
    }
}

Rocs::GraphStructure::GRAPH_TYPE  Rocs::GraphStructure::graphType()
{
    return _type;
}

bool Rocs::GraphStructure::directed()
{
    return (_type==DIRECTED||_type==MULTIGRAPH);
}

Pointer* Rocs::GraphStructure::addPointer(Data *from, Data *to) {
    if ( _type==UNDIRECTED ) {
        if (from == to) {  // self-edges
            return 0;
        }

        if ( from->pointers(to).size() >= 1 ) { // back-edges
            return 0;
        }
    }
    
    if ( _type==DIRECTED ) {   // do not add double edges
        PointerList list = from->out_pointers();
        foreach (Pointer *tmp, list) {
            if (tmp->to() == to) {
                return 0;
            }
        }
        if (from->self_pointers().size() >= 1){
            return 0;
        }
    }

    return DataStructure::addPointer(from, to);
}

Data* Rocs::GraphStructure::addData(QString name)
{
    if (readOnly()) return 0;

    GraphNode *n = new GraphNode(this);
    n->setName(name);
    return addData(n);
}
