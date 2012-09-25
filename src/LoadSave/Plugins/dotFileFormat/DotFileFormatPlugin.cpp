/*
    This file is part of Rocs.
    Copyright 2010  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "DotFileFormatPlugin.h"

#include <KAboutData>
#include <KGenericFactory>
#include <KUrl>
#include <QFile>
#include <QHash>

#include <DataStructure.h>
#include <Document.h>
#include <DataStructurePluginManager.h>
#include <Modifiers/Topology.h>
#include "../Plugins/DataStructure/Graph/GraphStructure.h"
#include "DotGraphParsingHelper.h"
#include "DotGrammar.h"
#include "Rocs_Typedefs.h"
#include <Group.h>

static const KAboutData aboutdata("rocs_dotfileformat",
                                  0,
                                  ki18nc("@title Displayed plugin name", "Graphviz Graph File Backend"),
                                  "0.1",
                                  ki18n("Read and write Graphviz graph files."),
                                  KAboutData::License_GPL_V2);

extern DotParser::DotGraphParsingHelper* phelper;

K_PLUGIN_FACTORY(FilePluginFactory, registerPlugin<DotFileFormatPlugin>();)
K_EXPORT_PLUGIN(FilePluginFactory(aboutdata))


DotFileFormatPlugin::~DotFileFormatPlugin()
{
}

DotFileFormatPlugin::DotFileFormatPlugin(QObject* parent, const QList< QVariant >&) :
    GraphFilePluginInterface(FilePluginFactory::componentData().aboutData(), parent)
{
}


const QStringList DotFileFormatPlugin::extensions() const
{
    return QStringList()
           << i18n("*.dot *.gv|Graphviz Format") + '\n';
}


void DotFileFormatPlugin::readFile()
{
    Document * graphDoc = new Document(i18n("Import"));
    DataStructurePluginManager::self()->setDataStructurePlugin("Graph");

    QList < QPair<QString, QString> > edges;
    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::ReadOnly)) {
        setError(CouldNotOpenFile, i18n("Could not open file \"%1\" in read mode: %2", file().toLocalFile(), fileHandle.errorString()));
        delete graphDoc;
        return;
    }
    QString content = fileHandle.readAll();
    if (!DotParser::parse(content.toStdString(), graphDoc)) {
        setError(EncodingProblem, i18n("Could not parse file \"%1\".", file().toLocalFile()));
        delete graphDoc;
        return;
    }
    Topology layouter;
    layouter.directedGraphDefaultTopology(graphDoc->activeDataStructure());
    setGraphDocument(graphDoc);
    setError(None);
}


void DotFileFormatPlugin::writeFile(Document &document)
{
    DataStructurePtr graph = document.activeDataStructure();
    //TODO make export graph selectable
    if (!graph) {
        setError(NoGraphFound, i18n("No active graph in this document."));
        return;
    }

    // prepare file handle for output
    QFile fileHandle(file().toLocalFile());
    QVariantList subgraphs;
    if (!fileHandle.open(QFile::WriteOnly | QFile::Text)) {
        setError(FileIsReadOnly, i18n("Cannot open file %1 to write document. Error: %2", file().fileName(), fileHandle.errorString()));
        return;
    }
    QTextStream out(&fileHandle);

    // create fast access list of already processed nodes: serialize each node only once
    QHash<int, bool> processedData;

    // process groups
    foreach(GroupPtr group, graph->groups()) {
        out << QString("subgraph %1 {\n").arg(group->name());
        foreach(DataPtr data, group->dataList()) {
            out << processNode(data);
            processedData.insert(data->identifier(), true);
        }
        out << "}\n";
    }

    // process all data elements
    foreach(int type, document.dataTypeList()) {
        if (type == document.groupType()) { // do not process groups
            continue;
        }
        foreach(DataPtr n, graph->dataList(type)) {
            if (processedData.contains(n->identifier())) {
                continue;
            }
            out << processNode(n);
        }
    }

    // process all edges
    foreach(int type, document.pointerTypeList()) {
        foreach(PointerPtr edge, graph->pointers(type)) {
            out << processEdge(edge);
        }
    }
    out << "}\n";
    setError(None);
    return;
}

QString const DotFileFormatPlugin::processEdge(PointerPtr edge) const
{
    QString edgeStr;
    edgeStr.append(QString(" %1 -> %2 ")
                .arg(edge->from()->identifier())
                .arg(edge->to()->identifier()));

    // process properties if present
    bool firstProperty = true;
    if (!edge->property("name").toString().isEmpty()) {
        firstProperty = false;
        edgeStr.append("[");
        edgeStr.append(QString(" label = \"%2\" ").arg(edge->property("name").toString()));
    }
    foreach(const QByteArray& property, edge->dynamicPropertyNames()) {
        if (firstProperty == true) {
                firstProperty = false;
                edgeStr.append("[");
            } else {
                edgeStr.append(", ");
        }
        edgeStr.append(QString(" %1 = \"%2\" ").arg(QString(property)).arg(edge->property(property).toString()));
    }
    if (!firstProperty) { // at least one property was inserted
        edgeStr.append("]");
    }
    return edgeStr.append(";\n");
}

QString const DotFileFormatPlugin::processNode(DataPtr node) const
{
    QString nodeStr;

    // use identifier for unique identification, store name as argument "label"
    nodeStr = QString("%1").arg(node->identifier());
    nodeStr.append(QString(" [label=%1 ").arg(node->property("name").toString()));

    foreach(const QByteArray& property, node->dynamicPropertyNames()) {
        nodeStr.append(", ");
        nodeStr.append(QString(" %1 = \"%2\" ").arg(QString(property)).arg(node->property(property).toString()));
    }

    // at least one property was inserted
    nodeStr.append("]");
    return nodeStr.append(";\n");
}

#include "DotFileFormatPlugin.moc"
