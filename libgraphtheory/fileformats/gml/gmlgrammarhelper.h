/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2006-2007 Gael de Chalendar <kleag@free.fr>
    SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
    SPDX-License-Identifier: GPL-2.0-only
*/


#ifndef GMLGRAMMARHELPER_H
#define GMLGRAMMARHELPER_H

#include "typenames.h"
#include "node.h"
#include <map>
#include <list>
#include <string>
#include <QStringList>
#include <QMap>
#include <QHash>

namespace GmlParser
{
struct GmlGrammarHelper {
    enum State {begin, graph, node, edge};

    GmlGrammarHelper();

    void createNode();
    void createEdge();
    void createGraph();
    void endList();
    void startList(const QString& key);

    void setAttribute(const QString& key, const QString& value);
    const QString processKey(const QString& key);
    QString edgeSource;
    QString edgeTarget;

    State currentState;

    GraphTheory::GraphDocumentPtr document;
    GraphTheory::NodePtr currentNode;
    GraphTheory::EdgePtr currentEdge;
    QStringList attributeStack;
    QHash<QString, QString> edgeAttributes;
    QMap<QString, GraphTheory::NodePtr> nodeMap; // for mapping data element ids
};
}

#endif
