/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef TYPENAMES_H
#define TYPENAMES_H

#include <QSharedPointer>

namespace GraphTheory
{
class GraphDocument;
class NodeType;
class EdgeType;
class Node;
class Edge;

typedef QSharedPointer<GraphDocument> GraphDocumentPtr;
typedef QSharedPointer<NodeType> NodeTypePtr;
typedef QSharedPointer<EdgeType> EdgeTypePtr;
typedef QSharedPointer<Node> NodePtr;
typedef QVector<QSharedPointer<Node>> NodeList;
typedef QSharedPointer<Edge> EdgePtr;
typedef QVector<QSharedPointer<Edge>> EdgeList;
}

#endif
