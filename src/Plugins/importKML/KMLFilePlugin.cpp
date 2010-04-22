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

#include "KMLFilePlugin.h"
#include "../../Core/graphDocument.h"
#include <KAboutData>
#include <KGenericFactory>
#include <QFile>

static const KAboutData aboutdata("rocs_kmlfileplugin", 0, ki18n("Open and Save KML files") , "0.1" );

K_PLUGIN_FACTORY( FilePLuginFactory, registerPlugin< KMLFilePlugin>(); ) 
K_EXPORT_PLUGIN( FilePLuginFactory(aboutdata) )


KMLFilePlugin::~KMLFilePlugin()
{

}

KMLFilePlugin::KMLFilePlugin(QObject* parent, const QList< QVariant >& ):
    FilePluginInterface(FilePLuginFactory::componentData(), parent)
{

}

const QStringList KMLFilePlugin::extensions() const
{
    return QStringList()
	<< i18n("*.kml|KML Files") + '\n';
}


GraphDocument * KMLFilePlugin::readFile(const QString &/*fileName*/) const{
    GraphDocument * graph = new GraphDocument("Untitled");
    graph->addGraph();
    return graph;
}

bool KMLFilePlugin::writeFile(const GraphDocument &/*graph*/ , const QString &filename) const{
    QFile file (filename);
    if (file.open(QFile::WriteOnly))
    {
        return true;
    }

    return false;
}
