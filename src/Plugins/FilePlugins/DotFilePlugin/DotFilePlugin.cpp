/*
    This file is part of Rocs.
    Copyright 2011  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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


#include "DotFilePlugin.h"

#include "Document.h"
#include "DataStructure.h"

#include <boost/graph/graphviz.hpp>
#include <boost/graph/topology.hpp>

#include <KAboutData>
#include <KGenericFactory>

#include <QFile>

using namespace boost;

static const KAboutData aboutdata ( "rocs_dotFilePlugin", 0, ki18n ( "Open and Save Graphviz files" ) , "0.1" );

K_PLUGIN_FACTORY ( FilePLuginFactory, registerPlugin<DotFilePlugin>(); )
K_EXPORT_PLUGIN ( FilePLuginFactory ( aboutdata ) )


DotFilePlugin::~DotFilePlugin() {

}


DotFilePlugin::DotFilePlugin ( QObject* parent, const QList< QVariant >& ) :
    FilePluginInterface ( FilePLuginFactory::componentData(), parent ) 
{
}


const QStringList DotFilePlugin::extensions() const {
    return QStringList() << i18n ( "*.dot|Graphviz Files" ) + '\n';
}


Document* DotFilePlugin::readFile ( const QString& fileName ) {
    typedef boost::property < vertex_name_t, std::string, boost::property < vertex_color_t, qreal > > 
        VertexProperty;
    typedef boost::property < edge_weight_t, qreal > 
        EdgeProperty;
    typedef boost::property < graph_name_t, std::string > 
        GraphProperty;
    typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, 
            VertexProperty, EdgeProperty, GraphProperty > 
        Graph;
    typedef boost::rectangle_topology<> topology_type;
    typedef topology_type::point_type point_type;
    typedef QVector<point_type> PositionVec;

    // Construct an empty graph and prepare the dynamic_property_maps.
    Document* graphDoc = new Document ( "Untitled" );
    Graph importGraph(0);
    
    // set up graph properties that shall be parsed
    dynamic_properties dp;

    property_map<Graph, vertex_name_t>::type name = get(vertex_name, importGraph);
    dp.property("node_id", name);
    
    property_map<Graph, vertex_color_t>::type mass = get(vertex_color, importGraph);
    dp.property("mass",mass);

    property_map<Graph, edge_weight_t>::type weight = get(edge_weight, importGraph);
    dp.property("weight",weight);

    // read file
    QFile f ( fileName );
    if ( !f.open ( QFile::ReadOnly ) ) {
        setError ( i18n ( "Cannot open the file: %1. Error %2" ).arg ( fileName ).arg ( f.errorString() ) );
        delete graphDoc;
        return 0;
    }
    QString content = f.readAll();

    // Use ref_property_map to turn a graph property into a property map
    boost::ref_property_map<Graph*,std::string> gname(get_property(importGraph,graph_name));
    dp.property("name", gname);

    // try to parse the file
    try {
        bool parse = boost::read_graphviz(QString(content).toStdString(),importGraph,dp,"node_id");
        if (!parse) {
            setError(i18n("Graphviz parser returned unsuccessfully for file %1.").arg(fileName));
            delete graphDoc;
            return 0;
        }
    }
    catch (boost::bad_graphviz_syntax) {
        setError(i18n("Could not parse Graphviz file due to syntax errors."));
        delete graphDoc;
        return 0;
    }
    
    // TODO Apply Layout

    
    // convert graphviz format to Document
    DataStructurePtr datastructure = graphDoc->addDataStructure(i18n("dotImport"));

    // put nodes at whiteboard as generated
    QMap<int, DataPtr > mapNodes;
    int index=0;
    boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(importGraph); vi != vi_end; ++vi) {
        mapNodes[*vi] = datastructure->addData(
                QString("%1").arg( index++ ),
                QPointF(0,0)
            );
    }
    
    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(importGraph); ei !=ei_end; ++ei) {
        datastructure->addPointer ( mapNodes[boost::source(*ei, importGraph)],
                            mapNodes[boost::target(*ei, importGraph)]);
            
    }
    
    return graphDoc;
}


bool DotFilePlugin::writeFile ( Document& graph, const QString& filename ) {
    // TODO
    return false;
}


const QString DotFilePlugin::lastError() {
    return _lastError;
}


void DotFilePlugin::setError(QString arg) {
    _lastError = arg;
}


#include "DotFilePlugin.moc"