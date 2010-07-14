/* This file is part of KGraphViewer.
   Copyright (C) 2006-2007 Gael de Chalendar <kleag@free.fr>

   KGraphViewer is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation, version 2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA
*/


#include "DotGraphParsingHelper.h"
// #include "dotgraph.h"
#include "dotgrammar.h"
// #include "dotdefaults.h"
//#include "graphsubgraph.h"
// #include "graphnode.h"
// #include "graphedge.h"

#include <boost/throw_exception.hpp>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_distinct.hpp>
#include <boost/spirit/include/classic_loops.hpp>
#include <boost/spirit/include/classic_confix.hpp>


#include <iostream>

#include <kdebug.h>

#include <QFile>
#include<QUuid>
#include "Core/DynamicPropertiesList.h"

using namespace std;

extern KGraphViewer::DotGraphParsingHelper* phelper;

namespace KGraphViewer
{
#define KGV_MAX_ITEMS_TO_LOAD std::numeric_limits<int>::max()

DotGraphParsingHelper::DotGraphParsingHelper():
        attrid(),
        valid(),
        attributed(),
        subgraphid(),
        uniq(0),
        attributes(),
        graphAttributes(),
        nodesAttributes(),
        edgesAttributes(),
        graphAttributesStack(),
        nodesAttributesStack(),
        edgesAttributesStack(),
        edgebounds(),
        z(0),
        maxZ(0),
        graph(0),
//   gs(0),
        gn(0),
        ge(0)
{
}

void DotGraphParsingHelper::setgraphelementattributes(QObject* ge, const AttributesMap& attributes)
{
    AttributesMap::const_iterator it, it_end;
    it = attributes.begin();
    it_end = attributes.end();
    for (; it != it_end; it++)
    {
        kDebug() << "    " << QString::fromStdString((*it).first) << "\t=\t'" << QString::fromStdString((*it).second) <<"'";
        kDebug() << ge->metaObject()->className();
        if ((*it).first=="label" && strcmp(ge->metaObject()->className(), "Edge") == 0 )
        {

            QString label = QString::fromUtf8((*it).second.c_str());
            label.replace("\\n","\n");
            ge->setProperty("name", label);
        }
        else
        {
//       (*ge).attributes()[QString::fromStdString((*it).first)] =
            DynamicPropertiesList::New()->addProperty(ge, QString::fromStdString((*it).first));
            ge->setProperty((*it).first.c_str(), QString::fromStdString((*it).second));
        }
    }


}

void DotGraphParsingHelper::setgraphattributes()
{

    setgraphelementattributes(graph, graphAttributes);
}

void DotGraphParsingHelper::setsubgraphattributes()
{
//   kDebug() << "Attributes for subgraph are : ";
//   gs->setZ(z);
//   kDebug() << "z="<<gs->z();
//   setgraphelementattributes(gs, graphAttributes);
}

void DotGraphParsingHelper::setnodeattributes()
{

    if (gn == 0)
    {
//     kDebug() << "gn is null";
        return;
    }
//   kDebug() << "Attributes for node " << gn->id() << " are : ";
//   gn->setZ(z+1);
//   kDebug() << "z="<<gn->z();
    setgraphelementattributes(gn, nodesAttributes);
}

void DotGraphParsingHelper::setedgeattributes()
{
//   kDebug() << "setedgeattributeswith z = " << z;

//   kDebug() << "Attributes for edge " << ge->fromNode()->id() << "->" << ge->toNode()->id() << " are : ";
//   ge->setZ(z+1);
//   kDebug() << "z="<<ge->z();
    setgraphelementattributes(ge, edgesAttributes);

//   if (edgesAttributes.find("_tdraw_") != edgesAttributes.end())
//   {
//     parse_renderop(edgesAttributes["_tdraw_"], ge->renderOperations());
// //     kDebug() << "edge renderOperations size is now " << ge->renderOperations().size();
//     DotRenderOpVec::const_iterator it, it_end;
//     it = ge->renderOperations().constBegin(); it_end = ge->renderOperations().constEnd();
//     for (; it != it_end; it++)
//       ge->arrowheads().push_back(*it);
//   }
//   if (edgesAttributes.find("_hdraw_") != edgesAttributes.end())
//   {
//     parse_renderop(edgesAttributes["_hdraw_"], ge->renderOperations());
// //     kDebug() << "edge renderOperations size is now " << ge->renderOperations().size();
//     DotRenderOpVec::const_iterator it, it_end;
//     it = ge->renderOperations().constBegin(); it_end = ge->renderOperations().constEnd();
//     for (; it != it_end; it++)
//       ge->arrowheads().push_back(*it);
//   }
}

void DotGraphParsingHelper::setattributedlist()
{
// //   kDebug() << "Setting attributes list for " << QString::fromStdString(attributed);
    if (attributed == "graph")
    {
        if (attributes.find("bb") != attributes.end())
        {
            std::vector< int > v;
            parse_integers(attributes["bb"].c_str(), v);
            if (v.size()>=4)
            {
                kDebug() << "setting width and height to " << v[2] << v[3];
//         graph->width(v[2]);
//         graph->height(v[3]);
            }
        }
        AttributesMap::const_iterator it, it_end;
        it = attributes.begin();
        it_end = attributes.end();
        for (; it != it_end; it++)
        {
//       kDebug() << "    " << QString::fromStdString((*it).first) << " = " <<  QString::fromStdString((*it).second);
            graphAttributes[(*it).first] = (*it).second;
        }
    }
    else if (attributed == "node")
    {
        AttributesMap::const_iterator it, it_end;
        it = attributes.begin();
        it_end = attributes.end();
        for (; it != it_end; it++)
        {
//       kDebug() << "    " << QString::fromStdString((*it).first) << " = " <<  QString::fromStdString((*it).second);
            nodesAttributes[(*it).first] = (*it).second;
        }
    }
    else if (attributed == "edge")
    {
        AttributesMap::const_iterator it, it_end;
        it = attributes.begin();
        it_end = attributes.end();
        for (; it != it_end; it++)
        {
//       kDebug() << "    " << QString::fromStdString((*it).first) << " = " <<  QString::fromStdString((*it).second);
            edgesAttributes[(*it).first] = (*it).second;
        }
    }
    attributes.clear();
}

void DotGraphParsingHelper::createnode(const std::string& nodeid)
{
    QString id = QString::fromStdString(nodeid);
    kDebug() << id;
    gn = dynamic_cast<Node*>(graph->node(id));
    if (gn==0 )//&& graph->nodes().size() < KGV_MAX_ITEMS_TO_LOAD)
    {
        kDebug() << "Creating a new node" << subgraphid;
        gn = graph->addNode(id);

        if (!subgraphid.isEmpty()) {
            gn->addDynamicProperty("SubGraph", subgraphid.last());
//       kDebug () << gn->dynamicPropertyNames();
        }
//     gn->setId(id);
//     gn->label(QString::fromStdString(nodeid));
//     if (z>0 && gs != 0)
//     {
// //       kDebug() << "Adding node" << id << "in subgraph" << gs->id();
//       gs->content().push_back(gn);
//     }
//     else
//     {
// //       kDebug() << "Adding node" << id;
//       graph->nodes()[id] = gn;
//     }
    }
    edgebounds.clear();
}

void DotGraphParsingHelper::createsubgraph()
{
//   kDebug() ;
    if (phelper)
    {
        ;

//     std::string str = phelper->subgraphid;
//     if (str.empty())
//     {
//       std::ostringstream oss;
//       oss << "kgv_id_" << phelper->uniq++;
//       str = oss.str();
//     }
// //     kDebug() << QString::fromStdString(str);
//     if (graph->subgraphs().find(QString::fromStdString(str)) == graph->subgraphs().end())
//     {
// //       kDebug() << "Creating a new subgraph";
//       gs = new GraphSubgraph();
//       gs->setId(QString::fromStdString(str));
// //       gs->label(QString::fromStdString(str));
//       graph->subgraphs().insert(QString::fromStdString(str), gs);
// //       kDebug() << "there is now"<<graph->subgraphs().size()<<"subgraphs in" << graph;
//     }
//     else
//     {
// //       kDebug() << "Found existing subgraph";
//       gs = *(graph->subgraphs().find(QString::fromStdString(str)));
//     }
//     phelper->subgraphid = "";
    }
}

void DotGraphParsingHelper::createedges()
{
//   kDebug();
    std::string node1Name, node2Name;
    node1Name = edgebounds.front();
    edgebounds.pop_front();
    while (!edgebounds.empty())
    {
        node2Name = edgebounds.front();
        edgebounds.pop_front();

        Node* gn1 = graph->node(QString::fromStdString(node1Name));
        if (gn1 == 0)
        {
//       kDebug() << "new node 1";
            gn1 = graph->addNode(QString::fromStdString(node1Name));

            if (!subgraphid.isEmpty()) {
                gn1->addDynamicProperty("SubGraph", subgraphid.last());
            }
//             gn1 = graph->node(QString::fromStdString(node1Name));
        }
        Node* gn2 = graph->node(QString::fromStdString(node2Name));
        if (gn2 == 0)
        {
//       kDebug() << "new node 1";
            gn2 = graph->addNode(QString::fromStdString(node2Name));

            if (!subgraphid.isEmpty()) {
                gn2->addDynamicProperty("SubGraph", subgraphid.last());
            }
        }
//     if (graph->nodes().size() >= KGV_MAX_ITEMS_TO_LOAD || graph->edges().size() >= KGV_MAX_ITEMS_TO_LOAD)
//     {
//       return;
//     }
//     kDebug() << QString::fromStdString(node1Name) << ", " << QString::fromStdString(node2Name);
//     rocsGraph;

//     kDebug() << "Found gn1="<<gn1<<" and gn2=" << gn2;
        if (gn1 == 0 || gn2 == 0)
        {
            kError() << "Unable to find or create edge bound(s) gn1=" << gn1 << "; gn2=" << gn2;
        }

        ge = graph->addEdge(gn1, gn2);
        if (!subgraphid.isEmpty()) {
            ge->addDynamicProperty("SubGraph", subgraphid.last());
        }
//     ge->setFromNode(gn1);
//     ge->setToNode(gn2);
//     kDebug() << ge->fromNode()->id() << " -> " << ge->toNode()->id();
        setedgeattributes();
//     kDebug() << ge->id();
//     if (ge->name() == "")
//     {
//       ge->setName(QString::fromStdString(node1Name)+QString::fromStdString(node2Name)+QUuid::createUuid().toString().remove("{").remove("}").remove("-"));
//     }
//     kDebug() << ge->id();
//     kDebug() << "num before=" << graph->edges().size();
//     graph->edges().insert(ge->id(), ge);
//     kDebug() << "num after=" << graph->edges().size();


        node1Name = node2Name;
    }
    edgebounds.clear();
}

void DotGraphParsingHelper::finalactions()
{
//   GraphEdgeMap::iterator it, it_end;
//   it = graph->edges().begin(); it_end = graph->edges().end();
//   for (; it != it_end; it++)
//   {
//     (*it)->setZ(maxZ+1);
//   }
}

}
