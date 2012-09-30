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
#include <Group.h>

extern DotParser::DotGraphParsingHelper* phelper;

namespace DotParser
{

DotGraphParsingHelper::DotGraphParsingHelper():
    attributeId(),
    valid(),
    attributed(),
    attributes(),
    dataStructureAttributes(),
    dataAttributes(),
    pointersAttributes(),
    dataStructureAttributesStack(),
    dataAttributesStack(),
    pointersAttributesStack(),
    edgebounds(),
    currentDataPtr(),
    currentPointerPtr(),
    dataMap()
{
}

void DotGraphParsingHelper::setDataStructureElementAttributes(QObject* graphElement, const AttributesMap& attributes)
{
    AttributesMap::const_iterator it, it_end;
    it = attributes.constBegin();
    it_end = attributes.constEnd();
    for (; it != it_end; it++) {
        kDebug() << "    " << it.key() << "\t=\t'" << it.value() << "'";
        kDebug() << graphElement->metaObject()->className();
        if (it.key() == "label" && strcmp(graphElement->metaObject()->className(), "Edge") == 0) {
            QString label = it.value();
            label.replace("\\n", "\n");
            graphElement->setProperty("name", label);
        } else {
            DynamicPropertiesList::New()->addProperty(graphElement, it.key().toAscii(), it.value());
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

void DotGraphParsingHelper::applyAttributedList()
{
    if (attributed == "graph") {
        if (attributes.find("bb") != attributes.end()) {
            std::vector< int > v;
            parseIntegers(attributes["bb"].toStdString().c_str(), v);
            if (v.size() >= 4) {
                kDebug() << "setting width and height to " << v[2] << v[3];
            }
        }
        AttributesMap::const_iterator it, it_end;
        it = attributes.constBegin();
        it_end = attributes.constEnd();
        for (; it != it_end; it++) {
            dataStructureAttributes[it.key()] = it.value();
        }
    } else if (attributed == "node") {
        AttributesMap::const_iterator it, it_end;
        it = attributes.constBegin();
        it_end = attributes.constEnd();
        for (; it != it_end; it++) {
            dataAttributes[it.key()] = it.value();
        }
    } else if (attributed == "edge") {
        AttributesMap::const_iterator it, it_end;
        it = attributes.constBegin();
        it_end = attributes.constEnd();
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
        kWarning() << "Omitting data element, ID is already used: "<< identifier;
        return;
    }

    kDebug() << "Creating new data element: " << identifier;
    currentDataPtr = dataStructure->addData(identifier);
    dataMap.insert(identifier, currentDataPtr);

    if (!groupStack.isEmpty()) {
        groupStack.last()->addData(currentDataPtr);
    }
}

void DotGraphParsingHelper::createSubDataStructure()
{
    GroupPtr newGroup = dataStructure->addGroup("Group");
    groupStack.append(newGroup);
    currentDataPtr = newGroup->getData();
}

void DotGraphParsingHelper::setSubDataStructureId(QString identifier)
{
    if(groupStack.isEmpty()) {
        kError() << "Cannot set sub data structure id: no group on stack";
        return;
    }
    // at this point the currentDataPtr is already the sub data structure
    dataMap.insert(identifier, currentDataPtr);
    groupStack.last()->setName(identifier);
}

void DotGraphParsingHelper::leaveSubDataStructure()
{
    if(groupStack.isEmpty()) {
        kWarning() << "Cannot leave group: currently not inside any group.";
        return;
    }
    groupStack.removeLast();
}

void DotGraphParsingHelper::createPointers()
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
        if (!dataMap.contains(fromId)) {
            DataPtr from = dataStructure->addData(fromId);
            dataMap.insert(fromId, from);
        }
        DataPtr from = dataMap[fromId];

        // if necessary create to node
        if (!dataMap.contains(toId)) {
            DataPtr to = dataStructure->addData(toId);
            dataMap.insert(toId, to);
        }
        DataPtr to = dataMap[toId];

        currentPointerPtr = dataStructure->addPointer(from, to);
        kDebug() << "Creating new pointer: " << from->identifier() << " -> " << to->identifier();
        setPointerAttributes();

        fromId = toId;
    }
    edgebounds.clear();
}

}
