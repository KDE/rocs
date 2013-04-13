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
#include <Group.h>

extern DotParser::DotGraphParsingHelper* phelper;

namespace DotParser
{

DotGraphParsingHelper::DotGraphParsingHelper():
    attributeId(),
    valid(),
    attributed(),
    unprocessedAttributes(),
    dataStructureAttributes(),
    dataAttributes(),
    pointerAttributes(),
    dataStructureAttributeStack(),
    dataAttributeStack(),
    pointerAttributeStack(),
    edgebounds(),
    currentDataPtr(),
    currentPointerPtr(),
    dataMap()
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
            graphElement->setProperty(iter.key().toAscii(), iter.value());
        }
    }
}

void DotGraphParsingHelper::setDataStructureAttributes()
{
    setObjectAttributes(dataStructure.get(), dataStructureAttributes);
}

void DotGraphParsingHelper::setSubDataStructureAttributes()
{
}

void DotGraphParsingHelper::setDataAttributes()
{
    if (!currentDataPtr) {
        return;
    }
    setObjectAttributes(currentDataPtr.get(), dataAttributes);
}

void DotGraphParsingHelper::setPointerAttributes()
{
    if (!currentPointerPtr) {
        return;
    }
    setObjectAttributes(currentPointerPtr.get(), pointerAttributes);
}

void DotGraphParsingHelper::applyAttributedList()
{
    if (attributed == "graph") {
        if (unprocessedAttributes.find("bb") != unprocessedAttributes.end()) {
            std::vector< int > v;
            parseIntegers(unprocessedAttributes["bb"].toStdString().c_str(), v);
//             if (v.size() >= 4) {
//                 kDebug() << "setting width and height to " << v[2] << v[3];
//             }
        }
        AttributesMap::const_iterator it, it_end;
        it = unprocessedAttributes.constBegin();
        it_end = unprocessedAttributes.constEnd();
        for (; it != it_end; it++) {
            dataStructureAttributes[it.key()] = it.value();
        }
    } else if (attributed == "node") {
        AttributesMap::const_iterator it, it_end;
        it = unprocessedAttributes.constBegin();
        it_end = unprocessedAttributes.constEnd();
        for (; it != it_end; it++) {
            dataAttributes[it.key()] = it.value();
        }
    } else if (attributed == "edge") {
        AttributesMap::const_iterator it, it_end;
        it = unprocessedAttributes.constBegin();
        it_end = unprocessedAttributes.constEnd();
        for (; it != it_end; it++) {
            pointerAttributes[it.key()] = it.value();
        }
    }
    unprocessedAttributes.clear();
}

void DotGraphParsingHelper::createData(QString identifier)
{
    edgebounds.clear(); //TODO explain meaning of this

    if (dataMap.contains(identifier)) {
        kWarning() << "Omitting data element, ID is already used: "<< identifier;
        return;
    }

//     kDebug() << "Creating new data element: " << identifier;
    currentDataPtr = dataStructure->createData(identifier, 0);
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
            DataPtr from = dataStructure->createData(fromId, 0);
            dataMap.insert(fromId, from);
            currentDataPtr = from;
            setDataAttributes();
        }
        DataPtr from = dataMap[fromId];

        // if necessary create to node
        if (!dataMap.contains(toId)) {
            DataPtr to = dataStructure->createData(toId, 0);
            dataMap.insert(toId, to);
            currentDataPtr = to;
            setDataAttributes();
        }
        DataPtr to = dataMap[toId];

        currentPointerPtr = dataStructure->createPointer(from, to, 0);
//         kDebug() << "Creating new pointer: " << from->identifier() << " -> " << to->identifier();
        setPointerAttributes();

        fromId = toId;
    }
    edgebounds.clear();
}

}
