/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2006-2007 Gael de Chalendar <kleag@free.fr>
    SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
    SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef DOT_GRAMMARHELPER_H
#define DOT_GRAMMARHELPER_H

#include "typenames.h"
#include <QMap>
#include <QObject>
#include <QStringList>

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
    void addEdgeBound(QString bound)
    {
        edgebounds.append(bound);
    }
    void setObjectAttributes(QObject *graphElement, const DotParser::DotGraphParsingHelper::AttributesMap &attributes);

    QString attributeId;
    QString valid;
    std::string attributed; // FIXME change to enum

    AttributesMap unprocessedAttributes;
    AttributesMap graphAttributes;
    AttributesMap nodeAttributes;
    AttributesMap edgeAttributes;
    QList<AttributesMap> graphAttributeStack;
    QList<AttributesMap> nodeAttributeStack;
    QList<AttributesMap> edgeAttributeStack;

    QStringList edgebounds;

    GraphTheory::GraphDocumentPtr document;
    GraphTheory::NodePtr currentNode;
    GraphTheory::EdgePtr currentEdge;
    QMap<QString, GraphTheory::NodePtr> nodeMap; // for mapping node element ids
};
}

#endif
