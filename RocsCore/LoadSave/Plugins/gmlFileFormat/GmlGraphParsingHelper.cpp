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

#include "GmlGraphParsingHelper.h"
#include "GmlGrammar.h"
#include "Document.h"
#include "Pointer.h"

#include <boost/throw_exception.hpp>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_distinct.hpp>
#include <boost/spirit/include/classic_loops.hpp>
#include <boost/spirit/include/classic_confix.hpp>

#include <KDebug>
#include <QFile>

extern GmlParser::GmlGraphParsingHelper* phelper;

namespace GmlParser
{

GmlGraphParsingHelper::GmlGraphParsingHelper():
    edgeSource(),
    edgeTarget(),
    _actualState(begin)
{
    actualGraph.reset();
    actualNode.reset();
    actualEdge.reset();
}

void GmlGraphParsingHelper::startList(const QString& key)
{
    kDebug() << "starting a list with key:" << key;
    if (_actualState == begin && key.compare("graph", Qt::CaseInsensitive) == 0) {
        createGraph();
        return;
    } else if (_actualState == graph) {
        if (key.compare("node", Qt::CaseInsensitive) == 0) {
            createNode();
            return;
        } else if (key.compare("edge", Qt::CaseInsensitive) == 0) {
            createEdge();
            return;
        }
    }
    _properties.append(key);
}


void GmlGraphParsingHelper::endList()
{
    if (!_properties.isEmpty()) {
        _properties.removeLast();
        return;
    }
    switch (_actualState) {
    case begin: kDebug() << "Ending a list without begin a item??"; break;
    case node: actualNode.reset();
        _actualState = graph;
        break;
    case edge: actualEdge.reset();
        _actualState = graph;
        break;
    case graph:
        actualGraph.reset();
        _actualState = begin;
        break;
    }
}


const QString GmlGraphParsingHelper::processKey(const QString& key)
{
    QString ret = key;
    if (key.compare("id", Qt::CaseInsensitive) == 0) {
        ret = "name";
    }

    return ret;
}


void GmlGraphParsingHelper::setAttribute(const QString& key, const QString& value)
{
    kDebug() << "Setting attibute " << key;
    switch (_actualState) {
    case begin: break;
    case graph:
        if (!_properties.isEmpty()) {
            QString joined = _properties.join(".");
            joined.append('.').append(key);
            actualGraph->setProperty(joined.toAscii(), value);
        } else {
            kDebug() << "seting property to graph" << key << value;
//           if (!actualGraph->setProperty(processKey(key).toAscii(),value)){
            actualGraph->addDynamicProperty(processKey(key), value); //is a dinamic property
//           }
        }
        break;
    case edge:
        if (!_properties.isEmpty()) { //is a list of properties of edge
            QString joined = _properties.join(".");
            joined.append('.').append(key);
            if (actualEdge) {
                actualEdge->setProperty(joined.toAscii(), value);
            } else {
                _edgeProperties.insert(joined, value);
            }
        } else if (key.compare("source", Qt::CaseInsensitive) == 0) {  // search for source....
            edgeSource = value;
            createEdge();
        } else if (key.compare("target", Qt::CaseInsensitive) == 0) {  // .... and target
            edgeTarget = value;
            createEdge();
        } else if (actualEdge) {      //if edge was created.
//               if(!actualEdge->setProperty(processKey(key).toAscii(),value)){
            kDebug() << "inserting edge key: " << key;
            actualEdge->addDynamicProperty(processKey(key), value);
// //               }
        } else {
            kDebug() << "Saving edge key: " << key;
            _edgeProperties.insert(processKey(key), value); //store to be inserted later
        }
        break;
    case node:
        if (!_properties.isEmpty()) {
            QString joined = _properties.join(".");
            joined.append('.').append(key);
            actualNode->setProperty(joined.toAscii(), value);
        } else {
            kDebug() << "seting property to node" << key << value;
//           if(!actualNode->setProperty(processKey(key).toAscii(),value)){
            actualNode->addDynamicProperty(processKey(key), value);
//           }
        }
        break;
    }
}


void GmlGraphParsingHelper::createGraph()
{
    if (_actualState == begin) {
        actualGraph = gd->addDataStructure();
        _actualState = graph;
    }
}


void GmlGraphParsingHelper::createNode()
{
    if (_actualState == graph) {
        kDebug() << "Creating a node";
        _actualState = node;
        actualNode = actualGraph->addData("NewNode");
    }
}


void GmlGraphParsingHelper::createEdge()
{
    if (!edgeSource.isEmpty() && !edgeTarget.isEmpty()) {
        kDebug() << "Creating a edge";
        _actualState = edge;
        if (!dataMap.contains(edgeSource) || !dataMap.contains(edgeTarget)) {
            kError() << "No edge created: end points were not created";
            return;
        }
        actualEdge = actualGraph->addPointer(dataMap[edgeSource], dataMap[edgeTarget]);
        edgeSource.clear();;
        edgeTarget.clear();
        while (! _edgeProperties.isEmpty()) {
            QString property = _edgeProperties.keys().at(0);
            actualEdge->addDynamicProperty(property, _edgeProperties.value(property));
            _edgeProperties.remove(property);
        }
    } else if (_actualState == graph) {
        kDebug() << "changing state Edge";
        _actualState = edge;
        actualEdge.reset();
    }
}

}
