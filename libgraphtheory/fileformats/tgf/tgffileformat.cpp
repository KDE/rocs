/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "tgffileformat.h"
#include "fileformats/fileformatinterface.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>
#include <QFile>
#include <QTextStream>
#include <QUrl>

using namespace GraphTheory;

//FIXME
// static const KAboutData aboutdata("rocs_tgffileformat",
//                                   0,
//                                   ki18nc("@title Displayed plugin name", "TGF File Backend"),
//                                   "0.1",
//                                   ki18n("Read and write Trivial Graph Format (TGF) files."),
//                                   KAboutData::License_GPL_V2);

K_PLUGIN_FACTORY(FilePluginFactory, registerPlugin<TgfFileFormat>();)


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
           << i18n("*.tgf|Trivial Graph Format") + '\n';
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
    //FIXME re-implement layouter
//     Topology layouter;
//     layouter.directedGraphDefaultTopology(graph);
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
    foreach(NodePtr node, document->nodes()) {
        out << node->id();
        out << " ";
        out << node->dynamicProperty("label").toString(); //TODO change to selectable property
        out << '\n';
    }
    out << "#\n";
    // export pointers
    foreach(EdgePtr edge, document->edges()) {
        out << edge->from()->id() << " " << edge->to()->id() << " " << edge->dynamicProperty("label").toString() <<'\n';
    }
    setError(None);
}

#include "tgffileformat.moc"
