/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2010 Tomaz Canabrava <tomaz.canabrava@gmail.com>
    SPDX-FileCopyrightText: 2010 Wagner Reck <wagner.reck@gmail.com>
    SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "dotfileformat.h"
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

static QString processNode(const NodePtr &node);
static QString processEdge(const EdgePtr &edge);

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
    if (!fileHandle.open(QFile::WriteOnly | QFile::Text)) {
        setError(FileIsReadOnly, i18n("Cannot open file %1 to write document. Error: %2", file().fileName(), fileHandle.errorString()));
        return;
    }
    QTextStream out(&fileHandle);

    out << "digraph {\n";

    // process all data elements
    const auto nodes = document->nodes();
    for (const NodePtr &node : nodes) {
        out << processNode(node);
    }

    // process all edges
    const auto edges = document->edges();
    for (const auto &edge : edges) {
        out << processEdge(edge);
    }
    out << "}\n";
    setError(None);
    return;
}

static QString processEdge(const EdgePtr &edge)
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
    for(const QByteArray &property : edge->dynamicPropertyNames()) {
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

static QString processNode(const NodePtr &node)
{
    QString nodeStr;

    // use identifier for unique identification, store name as argument "label"
    nodeStr = QString("%1").arg(node->id());
    nodeStr.append(" [ ");
    if (!node->dynamicProperty("name").toString().isEmpty()) {
        nodeStr.append(QString("label=\"%1\" ").arg(node->dynamicProperty("name").toString()));
    }

    for(const QByteArray &property : node->dynamicPropertyNames()) {
        nodeStr.append(", ");
        nodeStr.append(QString(" %1 = \"%2\" ").arg(QString(property)).arg(node->property(property).toString()));
    }

    // at least one property was inserted
    nodeStr.append("]");
    return nodeStr.append(";\n");
}

#include "dotfileformat.moc"
