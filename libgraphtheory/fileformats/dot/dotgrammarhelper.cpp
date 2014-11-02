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


#include "dotgrammarhelper.h"
#include "dotgrammar.h"
#include "graphdocument.h"
#include "node.h"
#include <edge.h>

#include <QDebug>
#include <QFile>

extern DotParser::DotGraphParsingHelper* phelper;

using namespace GraphTheory;

namespace DotParser
{

DotGraphParsingHelper::DotGraphParsingHelper():
    attributeId(),
    valid(),
    attributed(),
    unprocessedAttributes(),
    graphAttributes(),
    nodeAttributes(),
    edgeAttributes(),
    graphAttributeStack(),
    nodeAttributeStack(),
    edgeAttributeStack(),
    edgebounds(),
    currentNode(),
    currentEdge(),
    nodeMap()
{
}

void DotGraphParsingHelper::setObjectAttributes(QObject *graphElement, const AttributesMap &attributes)
{
    AttributesMap::const_iterator iter;
    iter = attributes.constBegin();
    for (; iter != attributes.constEnd(); ++iter) {
        if (iter.key() == "label" && strcmp(graphElement->metaObject()->className(), "Edge") == 0) {
            QString label = iter.value();
            label.replace("\\n", "\n");
            graphElement->setProperty("name", label);
        } else {
            graphElement->setProperty(iter.key().toUtf8(), iter.value());
        }
    }
}

void DotGraphParsingHelper::setDocumentAttributes()
{
    setObjectAttributes(document.data(), graphAttributes);
}

void DotGraphParsingHelper::setSubGraphAttributes()
{
    // not implemented
}

void DotGraphParsingHelper::setNodeAttributes()
{
    if (!currentNode) {
        return;
    }
    AttributesMap::ConstIterator iter;
    iter = nodeAttributes.constBegin();
    for (; iter != nodeAttributes.constEnd(); ++iter) {
        if (!currentNode->dynamicProperties().contains(iter.key())) {
            currentNode->type()->addDynamicProperty(iter.key());
        }
        QString key = iter.key(); // do not overwrite labels
        if (key == "name") {
            key = "dot_name";
        }
        currentNode->setDynamicProperty(key, iter.value());
    }
}

void DotGraphParsingHelper::setEdgeAttributes()
{
    if (!currentEdge) {
        return;
    }
    AttributesMap::ConstIterator iter;
    iter = edgeAttributes.constBegin();
    for (; iter != edgeAttributes.constEnd(); ++iter) {
        if (!currentEdge->dynamicProperties().contains(iter.key())) {
            currentEdge->type()->addDynamicProperty(iter.key());
        }
        currentEdge->setDynamicProperty(iter.key(), iter.value());
    }
}

void DotGraphParsingHelper::applyAttributedList()
{
    if (attributed == "graph") {
        if (unprocessedAttributes.find("bb") != unprocessedAttributes.end()) {
            std::vector< int > v;
            parseIntegers(unprocessedAttributes["bb"].toStdString().c_str(), v);
//             if (v.size() >= 4) {
//                 qDebug() << "setting width and height to " << v[2] << v[3];
//             }
        }
        AttributesMap::const_iterator it, it_end;
        it = unprocessedAttributes.constBegin();
        it_end = unprocessedAttributes.constEnd();
        for (; it != it_end; it++) {
            graphAttributes[it.key()] = it.value();
        }
    } else if (attributed == "node") {
        AttributesMap::const_iterator it, it_end;
        it = unprocessedAttributes.constBegin();
        it_end = unprocessedAttributes.constEnd();
        for (; it != it_end; it++) {
            nodeAttributes[it.key()] = it.value();
        }
    } else if (attributed == "edge") {
        AttributesMap::const_iterator it, it_end;
        it = unprocessedAttributes.constBegin();
        it_end = unprocessedAttributes.constEnd();
        for (; it != it_end; it++) {
            edgeAttributes[it.key()] = it.value();
        }
    }
    unprocessedAttributes.clear();
}

void DotGraphParsingHelper::createNode(const QString &name)
{
    edgebounds.clear(); //TODO explain meaning of this

    if (nodeMap.contains(name)) {
        qCritical() << "Omitting data element, identifying label is already used: "<< name;
        return;
    }
    currentNode = GraphTheory::Node::create(document);
    if (!currentNode->type()->dynamicProperties().contains("name")) {
        currentNode->type()->addDynamicProperty("name");
    }
    currentNode->setDynamicProperty("name", name);
    nodeMap.insert(name, currentNode);
}

void DotGraphParsingHelper::createSubGraph()
{
    // currently not implemented
}

void DotGraphParsingHelper::setSubGraphId(const QString &identifier)
{
    Q_UNUSED(identifier);
    // currently not implemented
}

void DotGraphParsingHelper::leaveSubGraph()
{
    // currently not implemented
}

void DotGraphParsingHelper::createEdge()
{
    QString fromId, toId;

    if (edgebounds.isEmpty()) {
        return;
    }
    fromId = edgebounds.first();
    edgebounds.removeFirst();
    while (!edgebounds.isEmpty()) {
        toId = edgebounds.first();
        edgebounds.removeFirst();

        // if necessary create from id
        if (!nodeMap.contains(fromId)) {
            NodePtr from = Node::create(document);
            from->setDynamicProperty("name", fromId);
            if (from->dynamicProperties().contains("name")) {
                from->type()->addDynamicProperty("name");
            }
            nodeMap.insert(fromId, from);
            currentNode = from;
            setNodeAttributes();
        }
        NodePtr from = nodeMap[fromId];

        // if necessary create to node
        if (!nodeMap.contains(toId)) {
            NodePtr to = Node::create(document);
            if (to->dynamicProperties().contains("name")) {
                to->type()->addDynamicProperty("name");
            }
            to->setDynamicProperty("name", toId);
            nodeMap.insert(toId, to);
            currentNode = to;
            setNodeAttributes();
        }
        NodePtr to = nodeMap[toId];

        currentEdge = Edge::create(from, to);
//         qDebug() << "Creating new edge: " << from->identifier() << " -> " << to->identifier();
        setEdgeAttributes();

        fromId = toId;
    }
    edgebounds.clear();
}

}
