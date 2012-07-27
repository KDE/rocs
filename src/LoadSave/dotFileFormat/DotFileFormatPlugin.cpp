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

#include <DataStructure.h>
#include <Document.h>
#include <DataStructurePluginManager.h>
#include "../Plugins/DataStructure/Graph/GraphStructure.h"
#include "DotGraphParsingHelper.h"
#include "DotGrammar.h"

static const KAboutData aboutdata("rocs_dotfileformat", 0, ki18n("Read and write Graphviz graph documents.") , "0.1");


extern DotParser::DotGraphParsingHelper* phelper;

K_PLUGIN_FACTORY(FilePluginFactory, registerPlugin<DotFileFormatPlugin>();)
K_EXPORT_PLUGIN(FilePluginFactory(aboutdata))


DotFileFormatPlugin::~DotFileFormatPlugin()
{

}

DotFileFormatPlugin::DotFileFormatPlugin(QObject* parent, const QList< QVariant >&) :
    GraphFilePluginInterface(FilePluginFactory::componentData(), parent)
{

}

const QStringList DotFileFormatPlugin::extensions() const
{
    return QStringList()
           << i18n("*.dot|Graphviz Files") + '\n';
}


void DotFileFormatPlugin::readFile()
{
    Document * graphDoc = new Document(i18n("Import"));
    DataStructurePluginManager::self()->setDataStructurePlugin("Graph");
    DataStructurePtr graph = graphDoc->addDataStructure();
//     Rocs::GraphStructure graph;  //TODO
//     Graph * graph = graphDoc->addGraph();
    QList < QPair<QString, QString> > edges;
    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::ReadOnly)) {
        setError(CouldNotOpenFile, i18n("Could not open file \"%1\" in read mode: %2", file().toLocalFile(), fileHandle.errorString()));
        delete graphDoc;
        return;
    }
    QString content = fileHandle.readAll();
    if (!parse(content.toStdString(), graphDoc)) { //TODO change interface and pass graph structure
        setError(EncodingProblem, i18n("Could not parse file \"%1\".", file().toLocalFile()));
        delete graphDoc;
        return;
    }
    setGraphDocument(graphDoc);
    setError(None);
}



void DotFileFormatPlugin::writeFile(Document &graph)
{
    //FIXME switch to use of shared pointers
    QFile fileHandle(file().toLocalFile());
    QVariantList subgraphs;
    if (fileHandle.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&fileHandle);
        DataStructurePtr g = graph.activeDataStructure(); //FIXME should be selectable
        if (g) {
//             out << QString("%1 %2 {\n").arg(g->directed() ? "digraph" : "graph").arg(g->name()); //FIXME
            foreach(DataPtr n, g->dataList()) {
                if (n->dynamicPropertyNames().contains("SubGraph")) {
                    if (!subgraphs.contains(n->property("SubGraph"))) {
                        subgraphs << n->property("SubGraph");
                        out << QString("subgraph %1 {\n").arg(n->property("SubGraph").toString());
                        foreach(DataPtr nTmp, g->dataList()) {
                            if (nTmp->property("SubGraph").isValid() && nTmp->property("SubGraph") == subgraphs.last()) {
                                out << processNode(nTmp.get());
                            }
                        }
                        foreach(PointerPtr e, g->pointers()) {
                            if (e->property("SubGraph").isValid() && e->property("SubGraph") == subgraphs.last()) { //Only edges from outer graph
                                out << processEdge(e.get());
                            }
                        }
                        out << "}\n";
                    }
                } else {
                    out << processNode(n.get());
                }
            }
            foreach(PointerPtr e, g->pointers()) {
                if (!e->dynamicPropertyNames().contains("SubGraph")) { //Only edges from outer graph
                    out << processEdge(e.get());
                }
            }
            out << "}\n";
            setError(None);
            return;
        }
        setError(NoGraphFound, i18n("No active graph in this document."));
    }
    setError(FileIsReadOnly, i18n("Cannot open file %1 to write document. Error: %2", file().fileName(), fileHandle.errorString()));
    return;
}

QString const DotFileFormatPlugin::processEdge(Pointer*e) const
{
    QString edge;
    edge.append(QString(" %1 -> %2 ").arg(e->from()->property("NodeName").isValid() ?
                                          e->from()->property("NodeName").toString() :
                                          e->from()->name())
                .arg(e->to()->property("NodeName").isValid() ?
                     e->to()->property("NodeName").toString() :
                     e->to()->name()));
    bool firstProperty = true;
    if (!e->name().isEmpty()) {
        firstProperty = false;
        edge.append("[");
        edge.append(QString(" label = \"%2\" ").arg(e->name()));
    }
    foreach(const QByteArray& property, e->dynamicPropertyNames()) {
        if (property != "SubGraph") {
            if (firstProperty == true) {
                firstProperty = false;
                edge.append("[");
            } else {
                edge.append(", ");
            }
            edge.append(QString(" %1 = \"%2\" ").arg(QString(property)).arg(e->property(property).toString()));
        }
    }
    if (!firstProperty) //At least one property was inserted
        edge.append("]");
    edge.append(";\n");
    return edge;
}

QString const DotFileFormatPlugin::processNode(Data* n) const
{
    QString node;
    if (n->property("NodeName").isValid())
        node = QString("%1").arg(n->property("NodeName").toString());
    else
        node = QString("%1").arg(n->name());

    bool firstProperty = true;
    foreach(const QByteArray& property, n->dynamicPropertyNames()) {
        if (property != "NodeName" && property != "SubGraph") {
            if (firstProperty == true) {
                firstProperty = false;
                node.append("[");
            } else {
                node.append(", ");
            }
            node.append(QString(" %1 = \"%2\" ").arg(QString(property)).arg(n->property(property).toString()));
        }
    }
    //Need save X and Y??
    if (!firstProperty) { //At least one property was inserted
        node.append("]");
    } else { //No needs os nodes definition, it doesn't have any property.
        return QString();
    }
    node.append(";\n");
    return node;
}

#include "DotFileFormatPlugin.moc"
