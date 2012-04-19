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
#include "Data.h"
#include "Pointer.h"

#include <boost/graph/graphviz.hpp>
#include <boost/graph/topology.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <KAboutData>
#include <KGenericFactory>

#include <QFile>
#include <QPair>

using namespace boost;

static const KAboutData aboutdata("rocs_dotFilePlugin", 0, ki18n("Open and Save Graphviz files") , "0.1");

K_PLUGIN_FACTORY(FilePLuginFactory, registerPlugin<DotFilePlugin>();)
K_EXPORT_PLUGIN(FilePLuginFactory(aboutdata))


DotFilePlugin::~DotFilePlugin()
{

}


DotFilePlugin::DotFilePlugin(QObject* parent, const QList< QVariant >&) :
    FilePluginInterface(FilePLuginFactory::componentData(), parent)
{
}


const QStringList DotFilePlugin::extensions() const
{
    return QStringList() << i18n("*.dot|Graphviz Files") + '\n';
}


Document* DotFilePlugin::readFile(const QString& fileName)
{
    // read file
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly)) {
        setError(i18n("Cannot open the file: %1. Error %2", fileName, f.errorString()));
        return 0;
    }
    QString content = f.readAll();

    // try to parse the file
    Document* graphDoc;
    try {
        graphDoc = parseGraphvizUndirected(content);
    } catch (boost::directed_graph_error) {
        qDebug() << "try reading as directed";
        try {
            graphDoc = parseGraphvizDirected(content);
        } catch (boost::bad_lexical_cast) {
            qDebug() << "Bad lexical cast when parsing graphviz file";
            delete graphDoc;
            return 0;
        } catch (boost::undirected_graph_error) {
            qDebug() << "this actually should not happen for a proper graph";
            delete graphDoc;
            return 0;
        }
        return graphDoc;
    } catch (boost::bad_graphviz_syntax) {
        setError(i18n("Could not parse Graphviz file due to syntax errors."));
        delete graphDoc;
        return 0;
    }

    return graphDoc;
}

Document* DotFilePlugin::parseGraphvizUndirected(const QString& graphvizContent)
{
    typedef boost::property < vertex_name_t, std::string,
            boost::property < vertex_color_t, qreal,
            boost::property< vertex_shape_t, std::string > > >
            VertexProperty;
    typedef boost::property < edge_weight_t, qreal,
            boost::property< edge_name_t, std::string > >
            EdgeProperty;
    typedef boost::property < graph_name_t, std::string >
    GraphProperty;
    typedef boost::adjacency_list < boost::listS, boost::vecS, boost::undirectedS,
            VertexProperty, EdgeProperty, GraphProperty >
            Graph;
    typedef boost::adjacency_list < boost::listS, boost::vecS, boost::undirectedS,
            VertexProperty, EdgeProperty, GraphProperty >
            Graph;

    typedef boost::rectangle_topology<> topology_type;
    typedef topology_type::point_type point_type;
    typedef QVector<point_type> PositionVec;

    // Construct an empty graph and prepare the dynamic_property_maps.
    Document* graphDoc = new Document("Untitled");
    Graph importGraph(0);

    // set up graph properties that shall be parsed
    dynamic_properties dp;

    property_map<Graph, vertex_name_t>::type name = get(vertex_name, importGraph);
    dp.property("node_id", name);

    property_map<Graph, vertex_color_t>::type color = get(vertex_color, importGraph);
    dp.property("color", color);

    property_map<Graph, edge_name_t>::type edgeName = get(edge_name, importGraph);
    dp.property("label", edgeName);

    property_map<Graph, edge_weight_t>::type weight = get(edge_weight, importGraph);
    dp.property("weight", weight);

    // Use ref_property_map to turn a graph property into a property map
    boost::ref_property_map<Graph*, std::string> gname(get_property(importGraph, graph_name));
    dp.property("name", gname);

    // Use ref_property_map to turn a graph property into a property map
    boost::property_map<Graph, vertex_shape_t>::type vertexShape = get(vertex_shape_t(), importGraph);
    dp.property("shape", vertexShape);

    // try to parse the file
    try {
        bool parse = boost::read_graphviz(QString(graphvizContent).toStdString(), importGraph, dp, "node_id");
        if (!parse) {
            setError(i18n("Graphviz parser returned unsuccessfully for file."));
            qDebug() << "Graphviz parser returned unsuccessfully for file";
            delete graphDoc;
            return 0;
        }
    } catch (boost::bad_graphviz_syntax) {
        setError(i18n("Could not parse Graphviz file due to syntax errors."));
        delete graphDoc;
        return 0;
    } catch (boost::bad_lexical_cast) {
        qDebug() << "throw bad lexical cast exception in DotFilePlugin";
        delete graphDoc;
        return 0;
    } catch (boost::property_not_found) {
        qDebug() << "stopped processing: not all DOT properties are known";
        delete graphDoc;
        return 0;
    }

    // Apply Layout
    // TODO move all general purpose layout functions to support library
    typedef boost::iterator_property_map < PositionVec::iterator,
            boost::property_map<Graph, boost::vertex_index_t>::type >
            PositionMap;
    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(1));
    // generate distribution topology and apply
    boost::rectangle_topology< boost::mt19937 > topology(gen, -200, -200, 200, 200);
    PositionVec position_vec(boost::num_vertices(importGraph));
    PositionMap positionMap(position_vec.begin(), get(boost::vertex_index, importGraph));
    boost::random_graph_layout(importGraph, positionMap, topology);
    // minimize cuts by Fruchtman-Reingold layout algorithm
    boost::fruchterman_reingold_force_directed_layout< boost::rectangle_topology< boost::mt19937 >, Graph, PositionMap >
    (importGraph,
     positionMap,
     topology,
     boost::cooling(boost::linear_cooling<double>(100))
    );

    // convert graphviz format to Document
    DataStructurePtr datastructure = graphDoc->addDataStructure(i18n("dotImport"));

    // put nodes at whiteboard as generated
    QMap<int, DataPtr > mapNodes;
    int index = 0;
    boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(importGraph); vi != vi_end; ++vi) {
        // TODO imported data values can be extended
        mapNodes[*vi] = datastructure->addData(
                            QString::fromStdString(get(name, *vi)),
                            QPointF(positionMap[*vi][0], positionMap[*vi][1])
                        );
        mapNodes[*vi]->setColor(get(color, *vi));
    }

    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(importGraph); ei != ei_end; ++ei) {
        PointerPtr newEdge = datastructure->addPointer(mapNodes[boost::source(*ei, importGraph)],
                             mapNodes[boost::target(*ei, importGraph)]);
        newEdge->setValue(QString::number(get(weight, *ei)));
    }
    return graphDoc;
}


Document* DotFilePlugin::parseGraphvizDirected(const QString& graphvizContent)
{
    typedef boost::property < vertex_name_t, std::string,
            boost::property < vertex_color_t, qreal,
            boost::property< vertex_shape_t, std::string > > >
            VertexProperty;
    typedef boost::property < edge_weight_t, qreal,
            boost::property< edge_name_t, std::string > >
            EdgeProperty;
    typedef boost::property < graph_name_t, std::string >
    GraphProperty;
    typedef boost::adjacency_list < boost::listS, boost::vecS, boost::directedS,
            VertexProperty, EdgeProperty, GraphProperty >
            Graph;
    typedef boost::adjacency_list < boost::listS, boost::vecS, boost::directedS,
            VertexProperty, EdgeProperty, GraphProperty >
            Graph;

    typedef boost::rectangle_topology<> topology_type;
    typedef topology_type::point_type point_type;
    typedef QVector<point_type> PositionVec;

    // Construct an empty graph and prepare the dynamic_property_maps.
    Document* graphDoc = new Document("Untitled");
    Graph importGraph(0);

    // set up graph properties that shall be parsed
    dynamic_properties dp;

    property_map<Graph, vertex_name_t>::type name = get(vertex_name, importGraph);
    dp.property("node_id", name);

    property_map<Graph, vertex_color_t>::type color = get(vertex_color, importGraph);
    dp.property("color", color);

    property_map<Graph, edge_name_t>::type edgeName = get(edge_name, importGraph);
    dp.property("label", edgeName);

    property_map<Graph, edge_weight_t>::type weight = get(edge_weight, importGraph);
    dp.property("weight", weight);

    // Use ref_property_map to turn a graph property into a property map
    boost::ref_property_map<Graph*, std::string> gname(get_property(importGraph, graph_name));
    dp.property("name", gname);

    // Use ref_property_map to turn a graph property into a property map
    boost::property_map<Graph, vertex_shape_t>::type vertexShape = get(vertex_shape_t(), importGraph);
    dp.property("shape", vertexShape);

    // try to parse the file
    try {
        bool parse = boost::read_graphviz(QString(graphvizContent).toStdString(), importGraph, dp, "node_id");
        if (!parse) {
            setError(i18n("Graphviz parser returned unsuccessfully for file."));
            qDebug() << "Graphviz parser returned unsuccessfully for file";
            delete graphDoc;
            return 0;
        }
    } catch (boost::bad_graphviz_syntax) {
        setError(i18n("Could not parse Graphviz file due to syntax errors."));
        qDebug() << "bad graphviz file syntax";
        delete graphDoc;
        return 0;
    } catch (boost::bad_lexical_cast) {
        qDebug() << "throw bad lexical cast exception in DotFilePlugin";
        delete graphDoc;
        return 0;
    } catch (boost::property_not_found) {
        qDebug() << "stopped processing: not all DOT properties are known";
        delete graphDoc;
        return 0;
    }

    // Apply Layout
    // TODO move all general purpose layout functions to support library
    typedef boost::iterator_property_map < PositionVec::iterator,
            boost::property_map<Graph, boost::vertex_index_t>::type >
            PositionMap;
    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(1));
    // generate distribution topology and apply
    boost::rectangle_topology< boost::mt19937 > topology(gen, -200, -200, 200, 200);
    PositionVec position_vec(boost::num_vertices(importGraph));
    PositionMap positionMap(position_vec.begin(), get(boost::vertex_index, importGraph));
    boost::random_graph_layout(importGraph, positionMap, topology);
    // minimize cuts by Fruchtman-Reingold layout algorithm
    boost::fruchterman_reingold_force_directed_layout< boost::rectangle_topology< boost::mt19937 >, Graph, PositionMap >
    (importGraph,
     positionMap,
     topology,
     boost::cooling(boost::linear_cooling<double>(100))
    );

    // convert graphviz format to Document
    DataStructurePtr datastructure = graphDoc->addDataStructure(i18n("dotImport"));

    // put nodes at whiteboard as generated
    QMap<int, DataPtr > mapNodes;
    int index = 0;
    boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(importGraph); vi != vi_end; ++vi) {
        // TODO imported data values can be extended
        mapNodes[*vi] = datastructure->addData(
                            QString::fromStdString(get(name, *vi)),
                            QPointF(positionMap[*vi][0], positionMap[*vi][1])
                        );
        mapNodes[*vi]->setColor(get(color, *vi));
    }

    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(importGraph); ei != ei_end; ++ei) {
        PointerPtr newEdge = datastructure->addPointer(mapNodes[boost::source(*ei, importGraph)],
                             mapNodes[boost::target(*ei, importGraph)]);
        newEdge->setValue(QString::number(get(weight, *ei)));
    }
    return graphDoc;
}


bool DotFilePlugin::writeFile(Document& graph, const QString& filename)
{
    typedef boost::property < vertex_name_t, std::string, boost::property < vertex_color_t, qreal > >
    VertexProperty;
    typedef boost::property < edge_weight_t, qreal >
    EdgeProperty;
    typedef boost::property < graph_name_t, std::string >
    GraphProperty;
    typedef boost::adjacency_list < boost::listS, boost::vecS, boost::directedS,
            VertexProperty, EdgeProperty, GraphProperty >
            Graph;
    typedef QPair<int, int> Edge;

    DataList dataList = graph.activeDataStructure()->dataList();

    // create IDs for all nodes
    QMap<Data*, int> node_mapping;
    QMap<QPair<int, int>, PointerPtr > edge_mapping; // to map all edges back afterwards
    int counter = 0;
    foreach(DataPtr data, dataList) {
        node_mapping[data.get()] = counter++;
    }

    DataStructurePtr ds = dataList.first()->dataStructure();
    QVector<Edge> edges(ds->pointers().count());

    counter = 0;
    foreach(PointerPtr p, ds->pointers()) {
        edges[counter++] = Edge(node_mapping[p->from().get()], node_mapping[p->to().get()]);
    }

    // setup the graph
    Graph serializeGraph(edges.begin(), edges.end(), dataList.count());

    // write to file
    std::ofstream fileStream;
    QFile file(filename);
    fileStream.open(file.fileName().toLocal8Bit());
    boost::write_graphviz(fileStream, serializeGraph);


    return true;
}


const QString DotFilePlugin::lastError()
{
    return _lastError;
}


void DotFilePlugin::setError(QString arg)
{
    _lastError = arg;
}


#include "DotFilePlugin.moc"
