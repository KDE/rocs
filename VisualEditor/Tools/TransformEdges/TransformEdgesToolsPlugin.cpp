/*
    This file is part of Rocs.
    Copyright (C) 2011 Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "TransformEdgesToolsPlugin.h"
#include "TransformEdgesWidget.h"
#include "../ToolsPluginInterface.h"
#include <CoreTypes.h>
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
#include "ui_transformedgeswidget.h"

static const KAboutData aboutdata("rocs_transformedgesplugin", 0, ki18nc("@title Displayed plugin name", "Transform Edges") , "0.1");

K_PLUGIN_FACTORY(ToolsPluginFactory, registerPlugin<TransformEdgesToolPlugin>();)
K_EXPORT_PLUGIN(ToolsPluginFactory(aboutdata))

TransformEdgesToolPlugin::TransformEdgesToolPlugin(QObject* parent,  const QList<QVariant> & /* args*/):
    ToolsPluginInterface(ToolsPluginFactory::componentData(), parent)
{

}

TransformEdgesToolPlugin::~TransformEdgesToolPlugin()
{

}

QString TransformEdgesToolPlugin::run(QObject* doc) const
{
    Document* graphDoc = qobject_cast<Document*> (doc);

    QPointer<TransformEdgesWidget> dialog = new TransformEdgesWidget(graphDoc, 0);

    QList< DataStructurePtr > dsList = graphDoc->dataStructures();
    QStringList dsNames;

    // be sure that only graph-datastructures are accessed by this plugin
    if (graphDoc->dataStructureInternalName() == "Graph") {
        foreach(DataStructurePtr ds, dsList) {
            dsNames << ds->name();
        }
    }

    dialog->addDataStructures(dsNames);
    dialog->exec();
    return "";
}

