/*
    This file is part of Rocs.
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef DOT_GRAMMAR_H
#define DOT_GRAMMAR_H

#include <string>
#include <vector>

class Document;

namespace DotParser {
    /**
    * Parse the given string \p str that represents the textual respresentation of a
    * graph in DOT/Graphviz format. The given document \p graphDoc must
    * be of plugin type "Graph.
    */
    bool parse(const std::string& str, Document* graphDoc);

    bool parseIntegers(const std::string& str, std::vector<int>& v);

    void setStrict();
    void undirectedDataStructure();
    void directedDataStructure();
    void dataStructureId(const std::string& str);
    void attributeId(const std::string& str);
    void subDataStructureId(const std::string& str);
    void valid(const std::string& str);
    void insertAttributeIntoAttributeList();
    void createAttributeList();
    void removeAttributeList();
    void createSubDataStructure();
    void createData(const std::string& str);
    void setDataStructureAttributes();
    void setSubDataStructureAttributes(char const* first, char const* last);
    void setDataAttributes();
    void applyAttributeList();
    void checkEdgeOperator(const std::string& str);
    void edgebound(const std::string& str);
    void createPointers();
    void leaveSubDataStructure();
}
#endif



