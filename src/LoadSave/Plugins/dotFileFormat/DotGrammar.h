/*
    This file is part of Rocs.
    Copyright 2006-2007  Gael de Chalendar <kleag@free.fr>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    Rocs is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation, version 2.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA
*/

/*
 * DataTypeViz dot DataType parsing grammar implemented with boost Spirit
 */

#ifndef DOT_GRAMMAR_H
#define DOT_GRAMMAR_H

#include <boost/throw_exception.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_int.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <QPoint>
#include <QColor>
#include <QPair>
#include <QVector>

#include <map>
#include <list>
#include <string>
#include <sstream>

class Document;

using namespace boost::spirit;

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
void addattr();
void pushAttrListC();
void popAttrListC();
void pushAttrList();
void popAttrList();
void createSubDataStructure();
void createData(const std::string& str);
void setDataStructureAttributes();
void setSubDataStructureAttributes(char const* first, char const* last);
void setDataAttributes();
void setAttributedList();
void checkedgeop(const std::string& str);
void edgebound(const std::string& str);
void createPointers();
void enterSubDataStructure();
void leaveSubDataStructure();

#endif



