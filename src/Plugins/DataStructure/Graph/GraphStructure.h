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

#ifndef GRAPHSTRUCTURE_H
#define GRAPHSTRUCTURE_H

#include "DataStructure.h"

namespace Rocs{
class ROCSLIB_EXPORT GraphStructure : public DataStructure {
  Q_OBJECT
  public:
    typedef enum {
        UNDIRECTED,
        DIRECTED,
        MULTIGRAPH
    } GRAPH_TYPE;

    //to avoid hide some methods
    using DataStructure::remove;
    using DataStructure::addPointer;
    using DataStructure::addData;

    GraphStructure ( Document* parent = 0 );

    GraphStructure(DataStructure& other, Document *parent);

    virtual ~GraphStructure();

    Pointer* addPointer(Data *from, Data *to);
    Data* addData(QString name);

  public slots:
    QScriptValue list_nodes();
    QScriptValue list_edges();
    QScriptValue add_node(const QString& name);
    QScriptValue add_edge(Data* from, Data* to);
    QScriptValue node_byname(const QString& name);
    bool directed();
    GRAPH_TYPE graphType();
    void setGraphType(int type);

  private:
    GraphStructure::GRAPH_TYPE _type;
};
}
#endif // GRAPHSTRUCTURE_H
