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
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "gmlfileformat.h"
#include "gmlgrammarhelper.h"
#include "gmlgrammar.h"
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
#include <QVector>

using namespace GraphTheory;

extern GmlParser::GmlGrammarHelper* phelper;

K_PLUGIN_FACTORY_WITH_JSON( FilePluginFactory,
                            "gmlfileformat.json",
                            registerPlugin<GmlFileFormat>();)

GmlFileFormat::GmlFileFormat(QObject *parent, const QList<QVariant>&) :
    FileFormatInterface("rocs_gmlfileformat", parent)
{
}

GmlFileFormat::~GmlFileFormat()
{
}

const QStringList GmlFileFormat::extensions() const
{
    return QStringList()
        << i18n("Graph Markup Language Format (%1)", QString("*.gml"));
}

void GmlFileFormat::readFile()
{
    GraphDocumentPtr document = GraphDocument::create();
    setGraphDocument(document);

    QList < QPair<QString, QString> > edges;
    QFile fileHandle(file().toLocalFile());
    if (!fileHandle.open(QFile::ReadOnly)) {
        setError(CouldNotOpenFile, i18n("Could not open file \"%1\" in read mode: %2", file().toLocalFile(), fileHandle.errorString()));
        document->destroy();
        return;
    }
    QString content = fileHandle.readAll();
    if (!GmlParser::parse(content, document)) { //TODO change interface and pass graph structure
        setError(EncodingProblem, i18n("Could not parse file \"%1\".", file().toLocalFile()));
        document->destroy();
        return;
    }
    Topology layouter;
    layouter.directedGraphDefaultTopology(document);
    setGraphDocument(document);
    setError(None);
}

void GmlFileFormat::writeFile(GraphDocumentPtr document)
{
    QFile fileHandle(file().toLocalFile());
    QVariantList subgraphs;
    if (!fileHandle.open(QFile::WriteOnly | QFile::Text)) {
        setError(FileIsReadOnly, i18n("Cannot open file %1 to write document. Error: %2", file().fileName(), fileHandle.errorString()));
        return;
    } else {
        QTextStream out(&fileHandle);
//FIXME uncommented following directed() check since this is moved to subclass
//need to add toggle
//             out << QString("graph [\n directed %1 \n").arg(g->directed()?"1":"0");
        out << QString("id \"%1\" \n").arg("graph"); //TODO support export of name

        foreach(NodePtr n, document->nodes()) {
            out << QString("node [\n id \"%1\" \n").arg(n->dynamicProperty("name").toString());
//                 foreach (QByteArray p, n->dynamicPropertyNames()){
//                    out << p << " " << n->property(p).toString() << "\n";
//                  }
            out << processNode(n);
            out << "]\n";

        }
        for (auto const edge : document->edges()) {
            out << "edge [\n";
//                  foreach (QByteArray p, e->dynamicPropertyNames()){
//                    out << p << " " << e->property(p).toString() << "\n";
//                  }
            out << processEdge(edge);

            out << "]\n";
        }
        out << "]\n";
    }
    setError(None);
    return;
}


QString GmlFileFormat::processEdge(EdgePtr e) const
{
    QString edge;
    edge.append(QString("source \"%1\"\n target \"%2\"\n").arg(e->from()->dynamicProperty("name").toString(), e->to()->dynamicProperty("name").toString()));
//     edge.append (QString(" color \"%1\"\n").arg(e->color())); //Problem with comments (both starts by '#')

    foreach(const QString &property, e->dynamicProperties()) {
        edge.append(QString("%1 %2\n").arg(property).arg(e->dynamicProperty(property).toString()));
    }

    return edge;
}

QString GmlFileFormat::processNode(NodePtr n) const
{
    QString node;
    node.append(QString("  x %1 \n  y %2 \n").arg(n->x()).arg(n->y()));
//       node.append (QString(" color \"%1\"\n").arg(n->color())); //Problem with comments (both starts by '#')

    foreach(const QString &property, n->dynamicProperties()) {
        node.append(QString("%1 %2\n").arg(property).arg(n->dynamicProperty(property).toString()));
    }

    return node;
}

#include "gmlfileformat.moc"
