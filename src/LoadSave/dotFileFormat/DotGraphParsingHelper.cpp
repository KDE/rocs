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


#include "DotGraphParsingHelper.h"
#include "DotGrammar.h"

#include <boost/throw_exception.hpp>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_distinct.hpp>
#include <boost/spirit/include/classic_loops.hpp>
#include <boost/spirit/include/classic_confix.hpp>


#include <iostream>

#include <kdebug.h>

#include <QFile>
#include <QUuid>
#include "DynamicPropertiesList.h"

using namespace std;

extern KGraphViewer::DotGraphParsingHelper* phelper;

namespace KGraphViewer
{
#define KGV_MAX_ITEMS_TO_LOAD std::numeric_limits<int>::max()

DotGraphParsingHelper::DotGraphParsingHelper():
    attrid(),
    valid(),
    attributed(),
    subdataTypeid(),
    unique(0),
    attributes(),
    dataStructureAttributes(),
    dataAttributes(),
    pointersAttributes(),
    dataStructureAttributesStack(),
    dataAttributesStack(),
    pointersAttributesStack(),
    edgebounds(),
    z(0),
    maxZ(0),
    currentDataPtr(),
    currentPointerPtr(),
    dataMap()
{
}

void DotGraphParsingHelper::setdataTypeelementattributes(QObject* graphElement, const AttributesMap& attributes)
{
    AttributesMap::const_iterator it, it_end;
    it = attributes.begin();
    it_end = attributes.end();
    for (; it != it_end; it++) {
        kDebug() << "    " << QString::fromStdString((*it).first) << "\t=\t'" << QString::fromStdString((*it).second) << "'";
        kDebug() << graphElement->metaObject()->className();
        if ((*it).first == "label" && strcmp(graphElement->metaObject()->className(), "Edge") == 0) {

            QString label = QString::fromUtf8((*it).second.c_str());
            label.replace("\\n", "\n");
            graphElement->setProperty("name", label);
        } else {
//       (*ge).attributes()[QString::fromStdString((*it).first)] =
            DynamicPropertiesList::New()->addProperty(graphElement, QString::fromStdString((*it).first));
            graphElement->setProperty((*it).first.c_str(), QString::fromStdString((*it).second));
        }
    }


}

void DotGraphParsingHelper::setDataStructureAttributes()
{
    setdataTypeelementattributes(dataStructure.get(), dataStructureAttributes);
}

void DotGraphParsingHelper::setSubDataStructureAttributes()
{
}

void DotGraphParsingHelper::setDataAttributes()
{
    if (!currentDataPtr) {
        return;
    }
    setdataTypeelementattributes(currentDataPtr.get(), dataAttributes);
}

void DotGraphParsingHelper::setPointerAttributes()
{
    if (!currentPointerPtr) {
        return;
    }
    setdataTypeelementattributes(currentPointerPtr.get(), pointersAttributes);
}

void DotGraphParsingHelper::setAttributedList()
{
    if (attributed == "graph") {
        if (attributes.find("bb") != attributes.end()) {
            std::vector< int > v;
            parse_integers(attributes["bb"].c_str(), v);
            if (v.size() >= 4) {
                kDebug() << "setting width and height to " << v[2] << v[3];
            }
        }
        AttributesMap::const_iterator it, it_end;
        it = attributes.begin();
        it_end = attributes.end();
        for (; it != it_end; it++) {
            dataStructureAttributes[(*it).first] = (*it).second;
        }
    } else if (attributed == "datum") {
        AttributesMap::const_iterator it, it_end;
        it = attributes.begin();
        it_end = attributes.end();
        for (; it != it_end; it++) {
            dataAttributes[(*it).first] = (*it).second;
        }
    } else if (attributed == "edge") {
        AttributesMap::const_iterator it, it_end;
        it = attributes.begin();
        it_end = attributes.end();
        for (; it != it_end; it++) {
//       kDebug() << "    " << QString::fromStdString((*it).first) << " = " <<  QString::fromStdString((*it).second);
            pointersAttributes[(*it).first] = (*it).second;
        }
    }
    attributes.clear();
}

void DotGraphParsingHelper::createData(const std::string& identifier)
{
    QString id = QString::fromStdString(identifier);
    edgebounds.clear(); //TODO explain meaning of this

    if (dataMap.contains(id)) {
        kDebug() << "Omitting data element, ID is already used: "<< id;
        return;
    }

    kDebug() << "Creating new data element: " << subdataTypeid;
    currentDataPtr = dataStructure->addData(id);
    dataMap.insert(id, currentDataPtr);

    if (!subdataTypeid.isEmpty()) { //TODO no proper subgraph implementation exists
        currentDataPtr->addDynamicProperty("SubGraph", subdataTypeid.last());
    }
}

void DotGraphParsingHelper::createSubDataStructure()
{
    kDebug() << "NOT IMPLEMENTED!";
}

void DotGraphParsingHelper::createPointers()
{
    //TODO explain meaning of edge bounds

    QString fromId, toId;
    fromId = QString::fromStdString(edgebounds.front());
    edgebounds.pop_front();
    while (!edgebounds.empty()) {
        toId = QString::fromStdString(edgebounds.front());
        edgebounds.pop_front();

        // if necessary create from id
        if (!dataMap.contains(fromId)) {
            DataPtr from = dataStructure->addData(fromId);
            dataMap.insert(fromId, from);
            if (!subdataTypeid.isEmpty()) { //TODO no proper subgraph implementation exists
                from->addDynamicProperty("SubGraph", subdataTypeid.last());
            }
        }
        DataPtr from = dataMap[fromId];

        // if necessary create to node
        if (!dataMap.contains(toId)) {
            DataPtr to = dataStructure->addData(toId);
            dataMap.insert(toId, to);
            if (!subdataTypeid.isEmpty()) { //TODO no proper subgraph implementation exists
                to->addDynamicProperty("SubGraph", subdataTypeid.last());
            }
        }
        DataPtr to = dataMap[toId];

        currentPointerPtr = dataStructure->addPointer(from, to);
        if (!subdataTypeid.isEmpty()) {
            currentPointerPtr->addDynamicProperty("SubGraph", subdataTypeid.last());
        }
        setPointerAttributes();

        fromId = toId;
    }
    edgebounds.clear();
}

void DotGraphParsingHelper::finalactions()
{
}

}
