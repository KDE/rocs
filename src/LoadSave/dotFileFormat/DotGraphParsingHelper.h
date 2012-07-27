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
#include "../Plugins/DataStructure/Graph/GraphStructure.h"
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
    void createSubDataStructure();
    void setDataStructureAttributes();
    void setSubDataStructureAttributes();
    void setDataAttributes();
    void setPointerAttributes();
    void setAttributedList();

    void createPointers();
    void addEdgeBound(QString bound) {
        edgebounds.append(bound);
    }
    void finalactions();
    void setDataStructureElementAttributes(QObject* graphElement, const DotParser::DotGraphParsingHelper::AttributesMap& attributes);

    QString attributeId;
    QString valid;
    std::string attributed;
    QStringList subdataTypeid;
    unsigned int unique;

    AttributesMap attributes;
    AttributesMap dataStructureAttributes;
    AttributesMap dataAttributes;
    AttributesMap pointersAttributes;
    QList< AttributesMap > dataStructureAttributesStack;
    QList< AttributesMap > dataAttributesStack;
    QList< AttributesMap > pointersAttributesStack;

    QStringList edgebounds;

    unsigned int z;
    unsigned int maxZ;

    boost::shared_ptr<Rocs::GraphStructure> dataStructure;

    DataPtr currentDataPtr;
    PointerPtr currentPointerPtr;
    Document* gd;                   // graph document of type "Graph", ensured when document is created
    QMap<QString, DataPtr> dataMap; // for mapping data element ids
};

}

#endif



