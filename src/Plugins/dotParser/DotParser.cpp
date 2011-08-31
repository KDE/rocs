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

#include "DotParser.h"

#include "Core/graphDocument.h"
#include <KAboutData>
#include <KGenericFactory>
#include <QFile>
#include <Core/DataType.h>
#include "DotGraphParsingHelper.h"
#include "dotgrammar.h"

static const KAboutData aboutdata ( "rocs_dotplugin", 0, ki18n ( "Open and Save Graphviz files" ) , "0.1" );


extern KGraphViewer::DotGraphParsingHelper* phelper;

K_PLUGIN_FACTORY ( FilePLuginFactory, registerPlugin< DotParser>(); )
K_EXPORT_PLUGIN ( FilePLuginFactory ( aboutdata ) )


DotParser::~DotParser() {

}

DotParser::DotParser ( QObject* parent, const QList< QVariant >& ) :
        FilePluginInterface ( FilePLuginFactory::componentData(), parent ) {

}

const QStringList DotParser::extensions() const {
    return QStringList()
           << i18n ( "*.dot|Graphviz Files" ) + '\n';
}


DataTypeDocument* DotParser::readFile ( const QString& fileName ) {
    DataTypeDocument * graphDoc = new DataTypeDocument ( "Untitled" );
//     Graph * graph = graphDoc->addGraph();
    QList < QPair<QString, QString> > edges;
    QFile f ( fileName );
    if ( !f.open ( QFile::ReadOnly ) ) {
        setError ( i18n ( "Cannot open the file: %1. Error %2" ).arg ( fileName ).arg ( f.errorString() ) );
        delete graphDoc;
        return 0;
    }
    QString content = f.readAll();
    if (!parse(content.toStdString(), graphDoc)) {
        setError(i18n("cannot parse the file %1.").arg(fileName));
        delete graphDoc;
        return 0;
    }
    return graphDoc;
}



bool DotParser::writeFile ( DataTypeDocument& graph, const QString& filename ) {
    QFile file ( filename );
    QVariantList subgraphs;
    if ( file.open ( QFile::WriteOnly | QFile::Text) ) {
        QTextStream out (&file);
        DataType *g = graph.activeDataType();
        if (g) {
            out << QString("%1 %2 {\n").arg(g->directed()?"digraph":"graph").arg(g->name());
            foreach ( Datum *n, g->data() ) {
                if (n->dynamicPropertyNames().contains("SubGraph")){
                  if (!subgraphs.contains(n->property("SubGraph"))){
                    subgraphs << n->property("SubGraph");
                    out << QString("subgraph %1 {\n").arg(n->property("SubGraph").toString());
                    foreach ( Datum *nTmp, g->data() ) {
                        if (nTmp->property("SubGraph").isValid() && nTmp->property("SubGraph") == subgraphs.last()){
                          out << processNode(nTmp);
                        }
                    }
                    foreach ( Pointer *e, g->pointers() ) {
                        if (e->property("SubGraph").isValid() && e->property("SubGraph") == subgraphs.last()){ //Only edges from outer graph
                           out << processEdge(e);
                        }
                    }
                    out << "}\n";
                  }
                }else{
                  out << processNode(n);
                }
            }
            foreach ( Pointer *e, g->pointers() ) {
                if (!e->dynamicPropertyNames().contains("SubGraph")){ //Only edges from outer graph
                    out << processEdge(e);
                }
            }
            out << "}\n";
            return true;
        }
        setError(i18n("No active graph in this document."));
    }
    setError(i18n("Cannot open file %1 to write document. Error: %2").arg(filename).arg(file.errorString()));
    return false;
}

QString const DotParser::processEdge(Pointer*e ) const
{
    QString edge;
    edge.append(QString(" %1 -> %2 ").arg(e->from()->property("NodeName").isValid()?
                                                  e->from()->property("NodeName").toString():
                                                  e->from()->name())
                                      .arg( e->to()->property("NodeName").isValid()?
                                                  e->to()->property("NodeName").toString():
                                                  e->to()->name()));
    bool firstProperty = true;
    if(!e->name().isEmpty()){
      firstProperty = false;
      edge.append("[");
      edge.append(QString(" label = \"%2\" ").arg(e->name()));
    }
    foreach(QByteArray property, e->dynamicPropertyNames()){
      if (property != "SubGraph"){
        if(firstProperty == true){
          firstProperty = false;
          edge.append("[");
        }else{
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

QString const DotParser::processNode(Datum* n) const
{
        QString node;
        if (n->property("NodeName").isValid())
          node = QString("%1").arg(n->property("NodeName").toString());
        else
          node = QString("%1").arg(n->name());

        bool firstProperty = true;
        foreach(QByteArray property, n->dynamicPropertyNames()){
          if (property != "NodeName" && property != "SubGraph"){
            if(firstProperty == true){
              firstProperty = false;
              node.append("[");
            }else{
              node.append(", ");
            }
            node.append(QString(" %1 = \"%2\" ").arg(QString(property)).arg(n->property(property).toString()));
          }
        }
        //Need save X and Y??
        if (!firstProperty){ //At least one property was inserted
          node.append("]");
        }else{ //No needs os nodes definition, it doesn't have any property.
          return QString();
        }
        node.append(";\n");
        return node;
}

const QString DotParser::lastError() {
    return _lastError;
}


void DotParser::setError ( QString arg1 ) {
    _lastError = arg1;
}

const QString DotParser::scriptToRun()
{
    return QString ("include ( arrangeNodes.js )\n"
                     "for (g = 0; g < graphs.length; ++g){\n"
                     "  nodes = graphs[g].list_nodes();\n"
                     "  for (var i = 0; i < nodes.length; i++){\n"
                     "    nodes[i].addDynamicProperty(\"NodeName\", nodes[i].name);\n"
                     "    if (nodes[i].label != undefined)\n"
                     "       nodes[i].name = nodes[i].label;\n"
                     "  }\n"
                     "}"
                   );
}


#include "DotParser.moc"
