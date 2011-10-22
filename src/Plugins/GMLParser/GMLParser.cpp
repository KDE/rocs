/*
    This file is part of Rocs.
    Copyright 2010  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010  Wagner Reck <wagner.reck@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "GMLParser.h"

#include "Core/Document.h"
#include <Pointer.h>
#include <KAboutData>
#include <KGenericFactory>
#include <QFile>
#include "Core/DataStructure.h"
#include "GMLGraphParsingHelper.h"
#include "GMLGrammar.h"

static const KAboutData aboutdata ( "rocs_gmlplugin", 0, ki18n ( "Open and Save GML files" ) , "0.1" );


extern Rocs::GMLPlugin::GMLGraphParsingHelper* phelper;

K_PLUGIN_FACTORY ( FilePLuginFactory, registerPlugin< GMLParser>(); )
K_EXPORT_PLUGIN ( FilePLuginFactory ( aboutdata ) )


GMLParser::~GMLParser() {

}

GMLParser::GMLParser ( QObject* parent, const QList< QVariant >& ) :
        FilePluginInterface ( FilePLuginFactory::componentData(), parent ) {

}

const QStringList GMLParser::extensions() const {
    return QStringList()
           << i18n ( "*.gml|Graph Markup Language Files" ) + '\n';
}


Document* GMLParser::readFile ( const QString& fileName ) {
    Document * graphDoc = new Document ( "Untitled" );
//     Graph * graph = graphDoc->addGraph();
    QList < QPair<QString, QString> > edges;
    QFile f ( fileName );
    if ( !f.open ( QIODevice::ReadOnly | QIODevice::Text ) ) {
        setError ( i18n ( "Cannot open the file: %1. Error %2" ).arg ( fileName ).arg ( f.errorString() ) );
        delete graphDoc;
        return 0;
    }
    QString content = f.readAll();
     if (!Rocs::GMLPlugin::parse(content, graphDoc)) {
         setError(i18n("cannot parse the file %1.").arg(fileName));
         delete graphDoc;
         return 0;
    }
    return graphDoc;
}



bool GMLParser::writeFile ( Document& graph, const QString& filename ) {
    QFile file ( filename );
    QVariantList subgraphs;
    if ( file.open ( QFile::WriteOnly | QFile::Text) ) {
        QTextStream out (&file);
        out << "Version 1\n";
        out << "Vendor \"Rocs\"\n";
        for (int i = 0 ; i < graph.dataStructures().count(); ++i){
          DataStructurePtr g = graph.dataStructures().at(i);
//         Graph *g = graph.activeGraph();
//FIXME uncommented following directed() check sind this is moved to subclass
//need to add toggle
//             out << QString("graph [\n directed %1 \n").arg(g->directed()?"1":"0");
            out << QString("id \"%1\" \n").arg(g->name());

            foreach( DataPtr n, g->dataList() ) {
                out << QString("node [\n id \"%1\" \n").arg(n->name());
//                 foreach (QByteArray p, n->dynamicPropertyNames()){
//                    out << p << " " << n->property(p).toString() << "\n";
//                  }
                out << processNode(n);
                out << "]\n";

            }
            foreach( PointerPtr e, g->pointers() ) {
                out << "edge [\n";
//                  foreach (QByteArray p, e->dynamicPropertyNames()){
//                    out << p << " " << e->property(p).toString() << "\n";
//                  }
                out << processEdge(e);

                out << "]\n";
            }
            out << "]\n";
        }
        return true;
    }
    setError(i18n("Cannot open file %1 to write document. Error: %2").arg(filename).arg(file.errorString()));
    return false;
}

QString const GMLParser::processEdge(PointerPtr e) const
{
    QString edge;
    edge.append(QString("source \"%1\"\n target \"%2\"\n").arg(e->from()->name(), e->to()->name()));
    if (!e->name().isEmpty()){
      edge.append (QString(" id \"%1\"\n").arg(e->name()));
    }
    edge.append (QString(" width \"%1\"\n").arg(e->width()));
//     edge.append (QString(" color \"%1\"\n").arg(e->color())); //Problem with comments (both starts by '#')
    edge.append (QString(" value \"%1\"\n").arg(e->value()));


    foreach (QByteArray p, e->dynamicPropertyNames()){
      edge.append( QString("%1 %2\n").arg(QString(p)).arg(e->property(p).toString()));
    }

    return edge;
}

QString const GMLParser::processNode(DataPtr n) const
{
      QString node;
      node.append(QString("  x %1 \n  y %2 \n").arg(n->x()).arg(n->y()));
      node.append (QString(" width %1\n").arg(n->width()));
//       node.append (QString(" color \"%1\"\n").arg(n->color())); //Problem with comments (both starts by '#')
      node.append (QString(" value \"%1\"\n").arg(n->value().toString()));
      node.append (QString(" iconPackage \"%1\"\n").arg(n->iconPackage()));
      node.append (QString(" icon \"%1\"\n").arg(n->icon()));

      foreach (QByteArray p, n->dynamicPropertyNames()){
        QString s = p ;

        node.append(QString("%1 \"%2\"\n").arg(s.replace('.', '_')).arg(n->property(p).toString()));
      }

        return node;
}

const QString GMLParser::lastError() {
    return _lastError;
}


void GMLParser::setError ( QString arg1 ) {
    _lastError = arg1;
}

const QString GMLParser::scriptToRun()
{
    return QString (/*"include ( arrangeNodes.js )\n"
                     "for (g = 0; g < graphs.length; ++g){\n"
                     "  nodes = graphs[g].list_nodes();\n"
                     "  for (var i = 0; i < nodes.length; i++){\n"
                     "    nodes[i].addDynamicProperty(\"NodeName\", nodes[i].name);\n"
                     "    if (nodes[i].label != undefined)\n"
                     "       nodes[i].name = nodes[i].label;\n"
                     "  }\n"
                     "}"*/
                   );
}


#include "GMLParser.moc"
