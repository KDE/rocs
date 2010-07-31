/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

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

#include "GraphPlugin.h"
#include "GraphStructure.h"
#include <KPluginFactory>
#include <KAboutData>

static const KAboutData aboutdata("rocs_GraphStructure", 0, ki18n("Graph Structure") , "0.1" );
using namespace Rocs;

K_PLUGIN_FACTORY( DSPluginFactory, registerPlugin< GraphPlugin>(); )
K_EXPORT_PLUGIN( DSPluginFactory(aboutdata) )


GraphPlugin::GraphPlugin ( QObject* parent, const QList< QVariant >& /*args*/ )
       : DSPluginInterface( DSPluginFactory::componentData(), parent)
{

}

GraphPlugin::~GraphPlugin()
{

}

Graph* GraphPlugin::changeToDS ( Graph* graph) {
    return graph;
}

Graph* GraphPlugin::createDS ( GraphDocument* parent )
{
  return new GraphStructure(parent);
//     return new Graph(parent);
}

