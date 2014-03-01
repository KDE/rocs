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

#include "TgfFileFormatPlugin.h"
#include "Document.h"
#include <KAboutData>
#include <KGenericFactory>
#include <KUrl>
#include <QFile>
#include <DataStructure.h>
#include <Data.h>
#include <Pointer.h>
#include <Modifiers/Topology.h>
#include <DataStructureBackendManager.h>

//FIXME
// static const KAboutData aboutdata("rocs_tgffileformat",
//                                   0,
//                                   ki18nc("@title Displayed plugin name", "TGF File Backend"),
//                                   "0.1",
//                                   ki18n("Read and write Trivial Graph Format (TGF) files."),
//                                   KAboutData::License_GPL_V2);

K_PLUGIN_FACTORY(FilePluginFactory, registerPlugin<TgfFileFormatPlugin>();)


TgfFileFormatPlugin::TgfFileFormatPlugin(QObject* parent, const QList< QVariant >&) :
    GraphFilePluginInterface("rocs_tgffileformat", parent)
{
}

TgfFileFormatPlugin::~TgfFileFormatPlugin()
{
}

const QStringList TgfFileFormatPlugin::extensions() const
{
    return QStringList()
           << i18n("*.tgf|Trivial Graph Format") + '\n';
}


void TgfFileFormatPlugin::readFile()
{
    Document * graphDoc = new Document("Untitled");
    //TODO select graph data structure
    DataStructureBackendManager::self().setBackend("Graph");
    DataStructurePtr graph = graphDoc->addDataStructure();

    // map node identifier from file to created data elements
    QMap<int,DataPtr> nodeMap;

    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::ReadOnly)) {
        setError(CouldNotOpenFile, i18n("Could not open file \"%1\" in read mode: %2", file().toLocalFile(), fileHandle.errorString()));
        delete graphDoc;
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
            QString label = line.section(' ', 1);    // get label, everything after first space
            DataPtr data = graph->createData(label.simplified(), 0);
            if (nodeMap.contains(identifier)) {
                setError(EncodingProblem, i18n("Could not parse file. Identifier \"%1\" is used more than once.", identifier));
                return;
            }
            nodeMap[identifier] = data;
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
            PointerPtr pointer = graph->createPointer(nodeMap[from], nodeMap[to], 0);
            pointer->setProperty("value", value.simplified());
        }
    }
    Topology layouter;
    layouter.directedGraphDefaultTopology(graph);
    setGraphDocument(graphDoc);
    setError(None);
}

void TgfFileFormatPlugin::writeFile(Document &graph )
{
    // TODO allow selection which data structure shall be exported
    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::WriteOnly | QFile::Text)) {
        setError(FileIsReadOnly, i18n("Could not open file \"%1\" in write mode: %2", file().fileName(), fileHandle.errorString()));
        return;
    }

    QTextStream out(&fileHandle);
    DataStructurePtr g = graph.activeDataStructure();
    if (!g) {
        setError(NoGraphFound, i18n("No data structure specified for output in this document."));
        return;
    }

    // export data elements
    //FIXME only default data type considered
    foreach(DataPtr n, g->dataList(0)) {
        out << n->identifier();
        out << " ";
        out << n->property("name").toString(); //TODO change to selectable property
        out << '\n';
    }
    out << "#\n";
    // export pointers
    //FIXME only default pointer type considered
    foreach(PointerPtr e, g->pointers(0)) {
        out << e->from()->identifier() << " " << e->to()->identifier() << " " << e->property("value").toString() <<'\n';
    }
    setError(None);
}

#include "TgfFileFormatPlugin.moc"
