/*
    Rocs-Tools-Plugin: Automized Modification of Graph Edges
    Copyright (C) 2011  Andreas Cord-Landwehr

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

#include "transformedgestoolsplugin.h"
#include "transformedgeswidget.h"
#include "../ToolsPluginInterface.h"
#include <model_GraphDocument.h>
#include <Rocs_Typedefs.h>
#include <Document.h>
#include <DataStructure.h>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QFont>
#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>

#include <KAboutData>
#include <kgenericfactory.h>
#include <KLocalizedString>

#include <map>

static const KAboutData aboutdata("rocs_transformedgesplugin", 0, ki18n("Transform Edges") , "0.1" );

K_PLUGIN_FACTORY( ToolsPluginFactory, registerPlugin<TransformEdgesToolPlugin>(); )
K_EXPORT_PLUGIN( ToolsPluginFactory(aboutdata) )

TransformEdgesToolPlugin::TransformEdgesToolPlugin(QObject* parent,  const QList<QVariant> & /* args*/):
    ToolsPluginInterface(ToolsPluginFactory::componentData(), parent)
{

}

TransformEdgesToolPlugin::~TransformEdgesToolPlugin()
{

}

QString TransformEdgesToolPlugin::run(QObject* doc) const
{
    Document* graphDoc = qobject_cast<Document*> ( doc );

    TransformEdgesWidget* dialog = new TransformEdgesWidget(graphDoc, 0);
    dialog->show();

    return "";

}

// #include "tranformedgestoolsplugin.moc"
