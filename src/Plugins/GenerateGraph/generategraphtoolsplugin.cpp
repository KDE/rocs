/*
    This file is part of Rocs.
    Copyright (C) 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "generategraphtoolsplugin.h"
#include "generategraphwidget.h"
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

static const KAboutData aboutdata("rocs_generategraphplugin", 0, ki18nc("@Title Displayed plugin name", "Generate Graph") , "0.1");

K_PLUGIN_FACTORY(ToolsPluginFactory, registerPlugin<GenerateGraphToolPlugin>();)
K_EXPORT_PLUGIN(ToolsPluginFactory(aboutdata))

GenerateGraphToolPlugin::GenerateGraphToolPlugin(QObject* parent,  const QList<QVariant> & /* args*/):
    ToolsPluginInterface(ToolsPluginFactory::componentData(), parent)
{

}

GenerateGraphToolPlugin::~GenerateGraphToolPlugin()
{

}

QString GenerateGraphToolPlugin::run(QObject* doc) const
{
    Document* graphDoc = qobject_cast<Document*> (doc);

    QPointer<GenerateGraphWidget> dialog = new GenerateGraphWidget(graphDoc);
    dialog->exec();

    return "";

}


// Q_EXPORT_PLUGIN2(makeCompleteToolsPlugin, MakeCompleteToolsPlugin)
#include "generategraphtoolsplugin.moc"
