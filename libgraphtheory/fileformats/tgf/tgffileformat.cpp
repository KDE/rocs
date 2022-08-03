/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2010-2011 Tomaz Canabrava <tomaz.canabrava@gmail.com>
    SPDX-FileCopyrightText: 2010 Wagner Reck <wagner.reck@gmail.com>
    SPDX-FileCopyrightText: 2012 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "tgffileformat.h"
#include "modifiers/topology.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QVector>

using namespace GraphTheory;

K_PLUGIN_CLASS_WITH_JSON(TgfFileFormat, "tgffileformat.json")

TgfFileFormat::TgfFileFormat(QObject* parent, const QList< QVariant >&)
    : FileFormatInterface("rocs_tgffileformat", parent)
{
}

TgfFileFormat::~TgfFileFormat()
{
}

const QStringList TgfFileFormat::extensions() const
{
    return QStringList()
           << i18n("Trivial Graph Format (%1)", QString("*.tgf"));
}


void TgfFileFormat::readFile()
{
    GraphDocumentPtr document = GraphDocument::create();
    document->nodeTypes().first()->addDynamicProperty("label");
    document->edgeTypes().first()->addDynamicProperty("label");

    // map node identifier from file to created data elements
    QMap<int, NodePtr> nodeMap;

    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::ReadOnly)) {
        setError(CouldNotOpenFile, i18n("Could not open file \"%1\" in read mode: %2", file().toLocalFile(), fileHandle.errorString()));
        return;
    }

    ReadMode mode = Nodes;
    while (!fileHandle.atEnd()) {
        QString line = QString(fileHandle.readLine()).trimmed();

        if (line.startsWith('#')) { // recognize separator before edge list
            mode = Edges;
            continue;
        }

        if (mode == Nodes) { // read node
            int identifier = line.section(' ', 0, 0).toInt();
            QString label = line.section(' ', 1);  // get label, this is everything after first space
            NodePtr node = Node::create(document);
            node->setDynamicProperty("label", label.simplified());
            node->setId(identifier);

            if (nodeMap.contains(identifier)) {
                setError(EncodingProblem, i18n("Could not parse file. Identifier \"%1\" is used more than once.", identifier));
                return;
            }
            nodeMap[identifier] = node;
            continue;
        }

        if (mode == Edges) { // node edge
            int from = line.section(' ', 0, 0).toInt();
            int to = line.section(' ', 1, 1).toInt();
            QString value = line.section(' ', 2);
            if (!nodeMap.contains(from) || !nodeMap.contains(to)) {
                setError(EncodingProblem, i18n("Could not parse file. Edge from \"%1\" to \"%2\" uses undefined nodes.", from, to));
                return;
            }
            EdgePtr edge = Edge::create(nodeMap[from], nodeMap[to]);
            edge->setDynamicProperty("label", value.simplified());
        }
    }

    Topology::directedGraphDefaultTopology(document);
    setGraphDocument(document);
    setError(None);
}

void TgfFileFormat::writeFile(GraphDocumentPtr document)
{
    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::WriteOnly | QFile::Text)) {
        setError(FileIsReadOnly, i18n("Could not open file \"%1\" in write mode: %2", file().fileName(), fileHandle.errorString()));
        return;
    }

    QTextStream out(&fileHandle);
    // export data elements
    //FIXME only default data type considered
    const auto nodes = document->nodes();
    for (const NodePtr &node : nodes) {
        out << node->id();
        out << " ";
        out << node->dynamicProperty("label").toString(); //TODO change to selectable property
        out << '\n';
    }
    out << "#\n";
    // export pointers
    const auto edges = document->edges();
    for (const auto &edge : edges) {
        out << edge->from()->id() << " " << edge->to()->id() << " " << edge->dynamicProperty("label").toString() <<'\n';
    }
    setError(None);
}

#include "tgffileformat.moc"
