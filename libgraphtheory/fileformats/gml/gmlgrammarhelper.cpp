/*
    This file is part of Rocs.
    Copyright 2006-2007  Gael de Chalendar <kleag@free.fr>
    Copyright 2012       Andreas Cord-Landwehr <cordlandwehr@kde.org>

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

#include "gmlgrammarhelper.h"
#include "gmlgrammar.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include "logging_p.h"
#include <QFile>

extern GmlParser::GmlGrammarHelper *phelper;

using namespace GraphTheory;

namespace GmlParser
{

GmlGrammarHelper::GmlGrammarHelper():
    edgeSource(),
    edgeTarget(),
    currentState(begin)
{
    document.reset();
    currentNode.reset();
    currentEdge.reset();
}

void GmlGrammarHelper::startList(const QString& key)
{
    qCDebug(GRAPHTHEORY_FILEFORMAT) << "starting a list with key:" << key;
    if (currentState == begin && key.compare("graph", Qt::CaseInsensitive) == 0) {
        createGraph();
        return;
    } else if (currentState == graph) {
        if (key.compare("node", Qt::CaseInsensitive) == 0) {
            createNode();
            return;
        } else if (key.compare("edge", Qt::CaseInsensitive) == 0) {
            createEdge();
            return;
        }
    }
    attributeStack.append(key);
}

void GmlGrammarHelper::endList()
{
    if (!attributeStack.isEmpty()) {
        attributeStack.removeLast();
        return;
    }
    switch (currentState) {
    case begin: qCDebug(GRAPHTHEORY_FILEFORMAT) << "Ending a list without begin a item??"; break;
    case node: currentNode.reset();
        currentState = graph;
        break;
    case edge: currentEdge.reset();
        currentState = graph;
        break;
    case graph:
        document.reset();
        currentState = begin;
        break;
    }
}

const QString GmlGrammarHelper::processKey(const QString& key)
{
    QString ret = key;
    if (key.compare("id", Qt::CaseInsensitive) == 0) {
        ret = "name";
    }

    return ret;
}

void GmlGrammarHelper::setAttribute(const QString& key, const QString& value)
{
    qCDebug(GRAPHTHEORY_FILEFORMAT) << "Setting attribute " << key;
    switch (currentState) {
    case begin: break;
    case graph:
        if (!attributeStack.isEmpty()) {
            QString joined = attributeStack.join(".");
            joined.append('.').append(key);
//             document->setDynamicProperty(joined, value);
        } else {
            qCDebug(GRAPHTHEORY_FILEFORMAT) << "setting property to graph" << key << value;
//           if (!currentGraph->setProperty(processKey(key).toAscii(),value)){
//             document->addDynamicProperty(processKey(key), value); //is a dinamic property
//           }
        }
        break;
    case edge:
        if (!attributeStack.isEmpty()) { //is a list of properties of edge
            QString joined = attributeStack.join(".");
            joined.append('.').append(key);
            if (currentEdge) {
                currentEdge->setDynamicProperty(joined, value);
            } else {
                edgeAttributes.insert(joined, value);
            }
        } else if (key.compare("source", Qt::CaseInsensitive) == 0) {  // search for source....
            edgeSource = value;
            createEdge();
        } else if (key.compare("target", Qt::CaseInsensitive) == 0) {  // .... and target
            edgeTarget = value;
            createEdge();
        } else if (currentEdge) {      //if edge was created.
//               if(!currentEdge->setProperty(processKey(key).toAscii(),value)){
            qCDebug(GRAPHTHEORY_FILEFORMAT) << "inserting edge key: " << key;
            currentEdge->setDynamicProperty(processKey(key), value);
// //               }
        } else {
            qCDebug(GRAPHTHEORY_FILEFORMAT) << "Saving edge key: " << key;
            edgeAttributes.insert(processKey(key), value); //store to be inserted later
        }
        break;
    case node:
        if (!attributeStack.isEmpty()) {
            QString joined = attributeStack.join(".");
            joined.append('.').append(key);
            currentNode->setProperty(joined.toUtf8(), value);
        } else {
            qCDebug(GRAPHTHEORY_FILEFORMAT) << "setting property to node" << key << value;
//           if(!currentNode->setProperty(processKey(key).toAscii(),value)){
            currentNode->setDynamicProperty(processKey(key), value);
//           }
        }
        break;
    }
}

void GmlGrammarHelper::createGraph()
{
    if (currentState == begin) {
        document = GraphDocument::create();
        currentState = graph;
    }
}

void GmlGrammarHelper::createNode()
{
    if (currentState == graph) {
        qCDebug(GRAPHTHEORY_FILEFORMAT) << "Creating a node";
        currentState = node;
        currentNode = Node::create(document);
    }
}

void GmlGrammarHelper::createEdge()
{
    if (!edgeSource.isEmpty() && !edgeTarget.isEmpty()) {
        qCDebug(GRAPHTHEORY_FILEFORMAT) << "Creating a edge";
        currentState = edge;
        if (!nodeMap.contains(edgeSource) || !nodeMap.contains(edgeTarget)) {
            qCCritical(GRAPHTHEORY_FILEFORMAT) << "No edge created: end points were not created";
            return;
        }
        currentEdge = Edge::create(nodeMap[edgeSource], nodeMap[edgeTarget]);
        edgeSource.clear();
        edgeTarget.clear();
        while (!edgeAttributes.isEmpty()) {
            QString property = edgeAttributes.keys().at(0);
            currentEdge->setDynamicProperty(property, edgeAttributes.value(property));
            edgeAttributes.remove(property);
        }
    } else if (currentState == graph) {
        qCDebug(GRAPHTHEORY_FILEFORMAT) << "changing state Edge";
        currentState = edge;
        currentEdge.reset();
    }
}

}
