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

#include "GmlFileFormatPlugin.h"

#include "Document.h"
#include "Pointer.h"
#include "DataStructure.h"
#include "DataStructurePluginManager.h"
#include "GmlGraphParsingHelper.h"
#include "GmlGrammar.h"

#include <KAboutData>
#include <KGenericFactory>

#include <QFile>

static const KAboutData aboutdata("rocs_gmlfileformat",
                                  0,
                                  ki18nc("@title Displayed plugin name", "GML File Backend"),
                                  "0.1",
                                  ki18n("Read and write Graph Markup Language (GML) files."),
                                  KAboutData::License_GPL_V2);

extern GmlParser::GmlGraphParsingHelper* phelper;

K_PLUGIN_FACTORY(FilePLuginFactory, registerPlugin<GmlFileFormatPlugin>();)
K_EXPORT_PLUGIN(FilePLuginFactory(aboutdata))


GmlFileFormatPlugin::~GmlFileFormatPlugin()
{
}


GmlFileFormatPlugin::GmlFileFormatPlugin(QObject* parent, const QList<QVariant>&) :
    GraphFilePluginInterface(FilePLuginFactory::componentData().aboutData(), parent)
{
}


const QStringList GmlFileFormatPlugin::extensions() const
{
    return QStringList()
           << i18n("*.gml|Graph Markup Language Format") + '\n';
}


void GmlFileFormatPlugin::readFile()
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
    if (!GmlParser::parse(content, graphDoc)) { //TODO change interface and pass graph structure
        setError(EncodingProblem, i18n("Could not parse file \"%1\".", file().toLocalFile()));
        delete graphDoc;
        return;
    }
    setGraphDocument(graphDoc);
    setError(None);
}


void GmlFileFormatPlugin::writeFile(Document& document)
{
    DataStructurePtr graph = document.activeDataStructure();
    //TODO make export graph selectable
    if (!graph) {
        setError(NoGraphFound, i18n("No active graph in this document."));
        return;
    }

    QFile fileHandle(file().toLocalFile());
    QVariantList subgraphs;
    if (!fileHandle.open(QFile::WriteOnly | QFile::Text)) {
        setError(FileIsReadOnly, i18n("Cannot open file %1 to write document. Error: %2", file().fileName(), fileHandle.errorString()));
        return;
    } else {
        QTextStream out(&fileHandle);
//FIXME uncommented following directed() check sind this is moved to subclass
//need to add toggle
//             out << QString("graph [\n directed %1 \n").arg(g->directed()?"1":"0");
        out << QString("id \"%1\" \n").arg(graph->name());

        foreach(DataPtr n, graph->dataListAll()) {
            out << QString("node [\n id \"%1\" \n").arg(n->name());
//                 foreach (QByteArray p, n->dynamicPropertyNames()){
//                    out << p << " " << n->property(p).toString() << "\n";
//                  }
            out << processNode(n);
            out << "]\n";

        }
        foreach(PointerPtr e, graph->pointerListAll()) {
            out << "edge [\n";
//                  foreach (QByteArray p, e->dynamicPropertyNames()){
//                    out << p << " " << e->property(p).toString() << "\n";
//                  }
            out << processEdge(e);

            out << "]\n";
        }
        out << "]\n";
    }
    setError(None);
    return;
}


QString const GmlFileFormatPlugin::processEdge(PointerPtr e) const
{
    QString edge;
    edge.append(QString("source \"%1\"\n target \"%2\"\n").arg(e->from()->name(), e->to()->name()));
    if (!e->name().isEmpty()) {
        edge.append(QString(" id \"%1\"\n").arg(e->name()));
    }
    edge.append(QString(" width \"%1\"\n").arg(e->width()));
//     edge.append (QString(" color \"%1\"\n").arg(e->color())); //Problem with comments (both starts by '#')
    edge.append(QString(" value \"%1\"\n").arg(e->value()));


    foreach(const QByteArray& p, e->dynamicPropertyNames()) {
        edge.append(QString("%1 %2\n").arg(QString(p)).arg(e->property(p).toString()));
    }

    return edge;
}


QString const GmlFileFormatPlugin::processNode(DataPtr n) const
{
    QString node;
    node.append(QString("  x %1 \n  y %2 \n").arg(n->x()).arg(n->y()));
    node.append(QString(" width %1\n").arg(n->width()));
//       node.append (QString(" color \"%1\"\n").arg(n->color())); //Problem with comments (both starts by '#')
    node.append(QString(" value \"%1\"\n").arg(n->value().toString()));
    node.append(QString(" iconPackage \"%1\"\n").arg(n->dataStructure()->document()->iconPackage()));
    node.append(QString(" icon \"%1\"\n").arg(n->icon()));

    foreach(const QByteArray& p, n->dynamicPropertyNames()) {
        QString s = p ;

        node.append(QString("%1 \"%2\"\n").arg(s.replace('.', '_')).arg(n->property(p).toString()));
    }

    return node;
}


#include "GmlFileFormatPlugin.moc"
