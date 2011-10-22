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

#include <KAboutData>
#include <KGenericFactory>

#include <QFile>


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
    Document* graphDoc = new Document ( "Untitled" );
    // TODO
    return graphDoc;
}


bool DotFilePlugin::writeFile ( DataTypeDocument& graph, const QString& filename ) {
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