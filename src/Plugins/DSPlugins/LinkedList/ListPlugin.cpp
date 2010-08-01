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

#include "ListPlugin.h"
#include "ListStructure.h"
#include <KPluginFactory>
#include <KAboutData>

static const KAboutData aboutdata("rocs_ListStructure", 0, ki18n("Linked List Structure") , "0.1" );
using namespace Rocs;

K_PLUGIN_FACTORY( DSPluginFactory, registerPlugin< ListPlugin>(); )
K_EXPORT_PLUGIN( DSPluginFactory(aboutdata) )


ListPlugin::ListPlugin ( QObject* parent, const QList< QVariant >& /*args*/ )
       : DSPluginInterface( DSPluginFactory::componentData(), parent)
{

}

ListPlugin::~ListPlugin()
{

}

Graph* ListPlugin::changeToDS ( Graph* graph) {
    return new ListStructure(*graph);
}

Graph* ListPlugin::createDS ( GraphDocument* parent )
{
  return new ListStructure(parent);
//     return new Graph(parent);
}

