/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
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
