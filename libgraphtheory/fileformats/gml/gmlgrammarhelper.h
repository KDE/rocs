/*
    This file is part of Rocs.
    Copyright 2006-2007  Gael de Chalendar <kleag@free.fr>
    Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>

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


#ifndef GMLGRAMMARHELPER_H
#define GMLGRAMMARHELPER_H

#include "typenames.h"
#include "node.h"
#include <map>
#include <list>
#include <string>
#include <QStringList>
#include <QMap>

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
