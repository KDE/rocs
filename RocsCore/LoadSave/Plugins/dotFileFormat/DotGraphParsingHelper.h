/*
    This file is part of Rocs.
    Copyright 2006-2007  Gael de Chalendar <kleag@free.fr>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    KGraphViewer is free software; you can redistribute it and/or
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

#ifndef DOT_GRAPHPARSINGHELPER_H
#define DOT_GRAPHPARSINGHELPER_H

#include <QStringList>
#include <DataStructure.h>
#include "DataStructures/Graph/GraphStructure.h"
#include <Data.h>
#include <Document.h>
#include <Pointer.h>

#include <QObject>
namespace DotParser
{

struct DotGraphParsingHelper {
    typedef QMap<QString,QString> AttributesMap;

    DotGraphParsingHelper();

    /**
     * Creates new data element and registers the identifier in data map.
     */
    void createData(QString identifier);

    /**
     * Creates new sub data structure and enters it. All future created data elements are  add to
     * this sub data structure, until \see leaveSubDataStructure() is called.
     */
    void createSubDataStructure();

    /**
     * Leaves current group, i.e., leave current sub data structure and switches focus to ancestor
     * group or directly to parent data structures if left from all groups.
     */
    void leaveSubDataStructure();
    void setDataStructureAttributes();
    void setSubDataStructureAttributes();
    void setSubDataStructureId(QString identifier);
    void setDataAttributes();
    void setPointerAttributes();

    /**
     * Applies all attributes from the current attributed list to the currently parsed element.
     * Use this method before removing the attributed list.
     */
    void applyAttributedList();

    void createPointers();
    void addEdgeBound(QString bound) {
        edgebounds.append(bound);
    }
    void setDataStructureElementAttributes(QObject* graphElement, const DotParser::DotGraphParsingHelper::AttributesMap& attributes);

    QString attributeId;
    QString valid;
    std::string attributed; //FIXME change to enum

    AttributesMap attributes;
    AttributesMap dataStructureAttributes;
    AttributesMap dataAttributes;
    AttributesMap pointersAttributes;
    QList< AttributesMap > dataStructureAttributesStack;
    QList< AttributesMap > dataAttributesStack;
    QList< AttributesMap > pointersAttributesStack;

    QStringList edgebounds;

    boost::shared_ptr<Rocs::GraphStructure> dataStructure;

    QList<GroupPtr> groupStack; // stack of groups, increased each time a new group is entered

    DataPtr currentDataPtr;
    PointerPtr currentPointerPtr;
    Document* gd;                   // graph document of type "Graph", ensured when document is created
    QMap<QString, DataPtr> dataMap; // for mapping data element ids
};

}

#endif



