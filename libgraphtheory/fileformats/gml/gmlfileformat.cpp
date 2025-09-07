/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2010 Tomaz Canabrava <tomaz.canabrava@gmail.com>
    SPDX-FileCopyrightText: 2010 Wagner Reck <wagner.reck@gmail.com>
    SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "gmlfileformat.h"
#include "edge.h"
#include "gmlgrammar.h"
#include "gmlgrammarhelper.h"
#include "graphdocument.h"
#include "modifiers/topology.h"
#include "node.h"
#include <KLocalizedString>
#include <KPluginFactory>
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QVector>

using namespace GraphTheory;

static QString processEdge(const EdgePtr &e);
static QString processNode(const NodePtr &n);

extern GmlParser::GmlGrammarHelper *phelper;

GmlFileFormat::GmlFileFormat()
    : FileFormatInterface()
{
}

GmlFileFormat::~GmlFileFormat()
{
}

const QStringList GmlFileFormat::extensions() const
{
    return QStringList{i18n("Graph Markup Language Format (%1)", QString("*.gml"))};
}

void GmlFileFormat::readFile()
{
    GraphDocumentPtr document = GraphDocument::create();
    setGraphDocument(document);

    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::ReadOnly)) {
        setError(CouldNotOpenFile, i18n("Could not open file \"%1\" in read mode: %2", file().toLocalFile(), fileHandle.errorString()));
        document->destroy();
        return;
    }
    QString content = fileHandle.readAll();
    if (!GmlParser::parse(content, document)) { // TODO change interface and pass graph structure
        setError(EncodingProblem, i18n("Could not parse file \"%1\".", file().toLocalFile()));
        document->destroy();
        return;
    }

    Topology::directedGraphDefaultTopology(document);
    setGraphDocument(document);
    setError(None);
}

void GmlFileFormat::writeFile(GraphDocumentPtr document)
{
    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::WriteOnly | QFile::Text)) {
        setError(FileIsReadOnly, i18n("Cannot open file %1 to write document. Error: %2", file().fileName(), fileHandle.errorString()));
        return;
    } else {
        QTextStream out(&fileHandle);
        // FIXME uncommented following directed() check since this is moved to subclass
        // need to add toggle
        //              out << QString("graph [\n directed %1 \n").arg(g->directed()?"1":"0");
        out << QString("id \"%1\" \n").arg("graph"); // TODO support export of name

        for (const NodePtr &n : document->nodes()) {
            out << QString("node [\n id \"%1\" \n").arg(n->dynamicProperty("name").toString());
            out << processNode(n);
            out << "]\n";
        }
        for (auto const &edge : document->edges()) {
            out << "edge [\n";
            out << processEdge(edge);
            out << "]\n";
        }
        out << "]\n";
    }
    setError(None);
    return;
}

static QString processEdge(const EdgePtr &e)
{
    QString edge;
    edge.append(QString("source \"%1\"\n target \"%2\"\n").arg(e->from()->dynamicProperty("name").toString(), e->to()->dynamicProperty("name").toString()));
    //     edge.append (QString(" color \"%1\"\n").arg(e->color())); //Problem with comments (both starts by '#')

    for (const QString &property : e->dynamicProperties()) {
        edge.append(QString("%1 %2\n").arg(property).arg(e->dynamicProperty(property).toString()));
    }

    return edge;
}

static QString processNode(const NodePtr &n)
{
    QString node;
    node.append(QString("  x %1 \n  y %2 \n").arg(n->x()).arg(n->y()));
    //       node.append (QString(" color \"%1\"\n").arg(n->color())); //Problem with comments (both starts by '#')

    for (const QString &property : n->dynamicProperties()) {
        node.append(QString("%1 %2\n").arg(property).arg(n->dynamicProperty(property).toString()));
    }

    return node;
}
