/*
    This file is part of Rocs.
    Copyright 2010       Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "dotfileformat.h"
#include "fileformats/fileformatinterface.h"
#include "modifiers/topology.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include <KLocalizedString>
#include <KPluginFactory>
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QHash>
#include <QVector>
#include "dotgrammarhelper.h"
#include "dotgrammar.h"

extern DotParser::DotGraphParsingHelper* phelper;

using namespace GraphTheory;

K_PLUGIN_FACTORY_WITH_JSON( FilePluginFactory,
                            "dotfileformat.json",
                            registerPlugin<DotFileFormat>();)

DotFileFormat::~DotFileFormat()
{
}

DotFileFormat::DotFileFormat(QObject* parent, const QList< QVariant >&) :
    FileFormatInterface("rocs_dotfileformat", parent)
{
}

const QStringList DotFileFormat::extensions() const
{
    return QStringList()
           << i18n("Graphviz Format (%1)", QString("*.dot"));
}

void DotFileFormat::readFile()
{
    GraphDocumentPtr document = GraphDocument::create();
    setGraphDocument(document);

    QList < QPair<QString, QString> > edges;
    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::ReadOnly)) {
        setError(CouldNotOpenFile, i18n("Could not open file \"%1\" in read mode: %2", file().toLocalFile(), fileHandle.errorString()));
        return;
    }
    QString content = fileHandle.readAll();
    if (!DotParser::parse(content.toStdString(), document)) {
        setError(EncodingProblem, i18n("Could not parse file \"%1\".", file().toLocalFile()));
        return;
    }

    Topology::directedGraphDefaultTopology(document);
     setError(None);
}

void DotFileFormat::writeFile(GraphDocumentPtr document)
{
    // prepare file handle for output
    QFile fileHandle(file().toLocalFile());
    QVariantList subgraphs;
    if (!fileHandle.open(QFile::WriteOnly | QFile::Text)) {
        setError(FileIsReadOnly, i18n("Cannot open file %1 to write document. Error: %2", file().fileName(), fileHandle.errorString()));
        return;
    }
    QTextStream out(&fileHandle);

    out << "digraph {\n";

    // create fast access list of already processed nodes: serialize each node only once
    QHash<int, bool> processedData;

    // process all data elements
    foreach(NodePtr node, document->nodes()) {
        out << processNode(node);
    }

    // process all edges
    for (auto const edge : document->edges()) {
        out << processEdge(edge);
    }
    out << "}\n";
    setError(None);
    return;
}

QString DotFileFormat::processEdge(EdgePtr edge) const
{
    QString edgeStr;
    edgeStr.append(QString(" %1 -> %2 ")
                .arg(edge->from()->id())
                .arg(edge->to()->id()));

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

QString DotFileFormat::processNode(NodePtr node) const
{
    QString nodeStr;

    // use identifier for unique identification, store name as argument "label"
    nodeStr = QString("%1").arg(node->id());
    nodeStr.append(" [ ");
    if (!node->dynamicProperty("name").toString().isEmpty()) {
        nodeStr.append(QString("label=\"%1\" ").arg(node->dynamicProperty("name").toString()));
    }

    foreach(const QByteArray& property, node->dynamicPropertyNames()) {
        nodeStr.append(", ");
        nodeStr.append(QString(" %1 = \"%2\" ").arg(QString(property)).arg(node->property(property).toString()));
    }

    // at least one property was inserted
    nodeStr.append("]");
    return nodeStr.append(";\n");
}

#include "dotfileformat.moc"
