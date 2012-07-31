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

#include <KDebug>

#include <QFile>
#include "DynamicPropertiesList.h"

extern DotParser::DotGraphParsingHelper* phelper;

namespace DotParser
{

DotGraphParsingHelper::DotGraphParsingHelper():
    attributeId(),
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

void DotGraphParsingHelper::setDataStructureElementAttributes(QObject* graphElement, const AttributesMap& attributes)
{
    AttributesMap::const_iterator it, it_end;
    it = attributes.begin();
    it_end = attributes.end();
    for (; it != it_end; it++) {
        kDebug() << "    " << it.key() << "\t=\t'" << it.value() << "'";
        kDebug() << graphElement->metaObject()->className();
        if (it.key() == "label" && strcmp(graphElement->metaObject()->className(), "Edge") == 0) {
            QString label = it.value();
            label.replace("\\n", "\n");
            graphElement->setProperty("name", label);
        } else {
            DynamicPropertiesList::New()->addProperty(graphElement, it.key());
            graphElement->setProperty(it.key().toStdString().c_str(), it.value());
        }
    }
}

void DotGraphParsingHelper::setDataStructureAttributes()
{
    setDataStructureElementAttributes(dataStructure.get(), dataStructureAttributes);
}

void DotGraphParsingHelper::setSubDataStructureAttributes()
{
}

void DotGraphParsingHelper::setDataAttributes()
{
    if (!currentDataPtr) {
        return;
    }
    setDataStructureElementAttributes(currentDataPtr.get(), dataAttributes);
}

void DotGraphParsingHelper::setPointerAttributes()
{
    if (!currentPointerPtr) {
        return;
    }
    setDataStructureElementAttributes(currentPointerPtr.get(), pointersAttributes);
}

void DotGraphParsingHelper::setAttributedList()
{
    if (attributed == "graph") {
        if (attributes.find("bb") != attributes.end()) {
            std::vector< int > v;
            parse_integers(attributes["bb"].toStdString().c_str(), v);
            if (v.size() >= 4) {
                kDebug() << "setting width and height to " << v[2] << v[3];
            }
        }
        AttributesMap::const_iterator it, it_end;
        it = attributes.begin();
        it_end = attributes.end();
        for (; it != it_end; it++) {
            dataStructureAttributes[it.key()] = it.value();
        }
    } else if (attributed == "datum") {
        AttributesMap::const_iterator it, it_end;
        it = attributes.begin();
        it_end = attributes.end();
        for (; it != it_end; it++) {
            dataAttributes[it.key()] = it.value();
        }
    } else if (attributed == "edge") {
        AttributesMap::const_iterator it, it_end;
        it = attributes.begin();
        it_end = attributes.end();
        for (; it != it_end; it++) {
            pointersAttributes[it.key()] = it.value();
        }
    }
    attributes.clear();
}

void DotGraphParsingHelper::createData(QString identifier)
{
    edgebounds.clear(); //TODO explain meaning of this

    if (dataMap.contains(identifier)) {
        kDebug() << "Omitting data element, ID is already used: "<< identifier;
        return;
    }

    kDebug() << "Creating new data element: " << identifier;
    currentDataPtr = dataStructure->addData(identifier);
    dataMap.insert(identifier, currentDataPtr);

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
    kDebug() << "createPointers() entered";
    QString fromId, toId;
    //TODO explain meaning of edge bounds
    if (edgebounds.isEmpty()) {
        return;
    }
    fromId = edgebounds.first();
    edgebounds.removeFirst();
    while (!edgebounds.isEmpty()) {
        toId = edgebounds.first();
        edgebounds.removeFirst();

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
        kDebug() << "Creating new pointer: " << from->name() << " -> " << to->name();
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
