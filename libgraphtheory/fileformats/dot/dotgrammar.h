/*
    This file is part of Rocs.
    Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DOTGRAMMAR_H
#define DOTGRAMMAR_H

#include "typenames.h"
#include <string>
#include <vector>

class Document;

namespace DotParser {
    /**
    * Parse the given string \p str that represents the textual representation of a
    * graph in DOT/Graphviz format. The given document \p graphDoc must
    * be of plugin type "Graph.
    */
    bool parse(const std::string& str, GraphTheory::GraphDocumentPtr document);

    bool parseIntegers(const std::string& str, std::vector<int>& v);

    void setStrict();
    void setUndirected();
    void setDirected();
    void setGraphId(const std::string &str);
    void attributeId(const std::string &str);
    void subGraphId(const std::string &str);
    void valid(const std::string &str);
    void insertAttributeIntoAttributeList();
    void createAttributeList();
    void removeAttributeList();
    void createSubGraph();
    void leaveSubGraph();
    void createNode(const std::string &str);
    void setGraphAttributes();
    void setSubGraphAttributes(char const* first, char const* last);
    void setNodeAttributes();
    void applyAttributeList();
    void checkEdgeOperator(const std::string &str);
    void edgebound(const std::string &str);
    void createEdge();
}

#endif
