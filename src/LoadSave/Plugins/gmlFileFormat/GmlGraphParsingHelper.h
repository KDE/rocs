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


#ifndef GML_GRAPHPARSINGHELPER_H
#define GML_GRAPHPARSINGHELPER_H

#include <map>
#include <list>
#include <string>
#include <QStringList>
#include "DataStructure.h"
#include "Data.h"
#include "Rocs_Typedefs.h"

#include <QObject>

namespace GmlParser
{
struct GmlGraphParsingHelper {
    enum State {begin, graph, node, edge};

    GmlGraphParsingHelper();

    void createNode();
    void createEdge();
    void createGraph();
    void endList();
    void startList(const QString& key);

    void setAttribute(const QString& key, const QString& value);
    const QString processKey(const QString& key);
    QString edgeSource;
    QString edgeTarget;

    State _actualState;

    DataStructurePtr actualGraph;
    DataPtr actualNode;
    PointerPtr actualEdge;
    Document* gd;
    QStringList _properties;
    QHash<QString, QString> _edgeProperties;
    QMap<QString, DataPtr> dataMap; // for mapping data element ids
};
}

#endif



