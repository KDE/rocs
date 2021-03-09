/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2006-2007 Gael de Chalendar <kleag@free.fr>
    SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    KGraphViewer is free software; you can redistribute it and/or
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

#ifndef DOT_GRAMMARHELPER_H
#define DOT_GRAMMARHELPER_H

#include "typenames.h"
#include <QStringList>
#include <QObject>
#include <QMap>

namespace DotParser
{

struct DotGraphParsingHelper {
    typedef QMap<QString, QString> AttributesMap;

    DotGraphParsingHelper();

    /**
     * Creates new data element and registers the identifier in data map.
     */
    void createNode(const QString &name);

    /**
     * Creates new sub data structure and enters it. All future created data elements are  add to
     * this sub data structure, until \see leaveSubDataStructure() is called.
     */
    void createSubGraph();

    /**
     * Leaves current group, i.e., leave current sub data structure and switches focus to ancestor
     * group or directly to parent data structures if left from all groups.
     */
    void leaveSubGraph();
    void setDocumentAttributes();
    void setSubGraphAttributes();
    void setSubGraphId(const QString &identifier);
    void setNodeAttributes();
    void setEdgeAttributes();

    /**
     * Generates a new attribute list from all unprocessed attributes and set the corresponding
     * attribute list for data-structure, data, or pointer.
     */
    void applyAttributedList();

    void createEdge();
    void addEdgeBound(QString bound) {
        edgebounds.append(bound);
    }
    void setObjectAttributes(QObject *graphElement, const DotParser::DotGraphParsingHelper::AttributesMap &attributes);

    QString attributeId;
    QString valid;
    std::string attributed; //FIXME change to enum

    AttributesMap unprocessedAttributes;
    AttributesMap graphAttributes;
    AttributesMap nodeAttributes;
    AttributesMap edgeAttributes;
    QList< AttributesMap > graphAttributeStack;
    QList< AttributesMap > nodeAttributeStack;
    QList< AttributesMap > edgeAttributeStack;

    QStringList edgebounds;

    GraphTheory::GraphDocumentPtr document;
    GraphTheory::NodePtr currentNode;
    GraphTheory::EdgePtr currentEdge;
    QMap<QString, GraphTheory::NodePtr> nodeMap; // for mapping node element ids
};
}

#endif
