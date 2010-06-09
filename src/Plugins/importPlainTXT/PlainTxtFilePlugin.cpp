/*
    Rocs file plugin to handle KML Files.
    Copyright (C) 2010  Wagner Reck <wagner.reck@gmail.com>

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

#include "PlainTxtFilePlugin.h"
#include "../../Core/graphDocument.h"
#include <KAboutData>
#include <KGenericFactory>
#include <QFile>
#include <graph.h>

static const KAboutData aboutdata ( "rocs_plaintxtplugin", 0, ki18n ( "Open and Save Plain TXT files" ) , "0.1" );

K_PLUGIN_FACTORY ( FilePLuginFactory, registerPlugin< PlainTXTFilePlugin>(); )
K_EXPORT_PLUGIN ( FilePLuginFactory ( aboutdata ) )


PlainTXTFilePlugin::~PlainTXTFilePlugin() {

}

PlainTXTFilePlugin::PlainTXTFilePlugin ( QObject* parent, const QList< QVariant >& ) :
        FilePluginInterface ( FilePLuginFactory::componentData(), parent ) {

}

const QStringList PlainTXTFilePlugin::extensions() const {
    return QStringList()
           << i18n ( "*.txt|Plain TXT Files" ) + '\n';
}


GraphDocument * PlainTXTFilePlugin::readFile ( const QString &fileName ) {
    GraphDocument * graphDoc = new GraphDocument ( "Untitled" );
    Graph * graph = graphDoc->addGraph();
    QList < QPair<QString, QString> > edges;
    QFile f ( fileName );
    if ( !f.open ( QFile::ReadOnly ) ) {
        setError ( i18n ( "Cannot open the file %1. Error: %2" ).arg ( fileName ).arg ( f.errorString() ) );
        delete graphDoc;
        return 0;
    }
    while ( !f.atEnd() ) {
        QStringList list = QString ( f.readLine() ).trimmed().split ( ' ', QString::SkipEmptyParts );
        switch ( list.count() ) {
        case 1:
            graph->addNode ( list.at ( 0 ) );
            break;
        case 2:
            edges << QPair<QString, QString> ( list.at ( 0 ), list.at ( 1 ) ) ;
            break;
        case 3:
            graph->addNode ( list.at ( 0 ) );
            graph->node ( list.at ( 0 ) )->setX ( list.at ( 1 ).toDouble() );
            graph->node ( list.at ( 0 ) )->setY ( list.at ( 2 ).toDouble() );
            break;
        default:
            kDebug() << i18n ( "Ignoring line: %1" ).arg ( list.join ( " " ) );
            break;

        }
    }
    for ( int i = 0; i < edges.count(); ++i ) {
        graph->addEdge ( edges.at ( i ).first, edges.at ( i ).second );
        kDebug() << edges.at ( i );
    }
    return graphDoc;
}

bool PlainTXTFilePlugin::writeFile ( GraphDocument &graph , const QString &filename ) {
    QFile file ( filename );
    if ( file.open ( QFile::WriteOnly | QFile::Text) ) {
        QTextStream out (&file);
        Graph *g = graph.activeGraph();
        if (g){
            foreach ( Node *n, g->nodes() ) {
                out << n->name();
                out << " ";
                out << n->x();
                out << " ";
                out << n->y();
                out << '\n';
            }
            foreach ( Edge *e, g->edges() ) {
                out << e->from()->name() << " " << e->to()->name() << '\n';
            }
            return true;
        }
        setError(i18n("No active graph in this document."));
    }
    setError(i18n("Cannot open file %1 to write the document. Error: %2").arg(filename).arg(file.errorString()));
    return false;
}

const QString PlainTXTFilePlugin::lastError() {
    return _lastError;
}


void PlainTXTFilePlugin::setError ( QString arg1 ) {
    _lastError = arg1;
}
