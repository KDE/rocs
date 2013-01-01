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

#include "GenerateGraphToolsPlugin.h"
#include "GenerateGraphWidget.h"
#include "../ToolsPluginInterface.h"
#include <CoreTypes.h>
#include <Document.h>
#include <DocumentManager.h>

#include <KAboutData>
#include <KGenericFactory>
#include <KLocalizedString>

#include <map>

static const KAboutData aboutdata("rocs_generategraphplugin", 0, ki18nc("@Title Displayed plugin name", "Generate Graph") , "0.1");

K_PLUGIN_FACTORY(ToolsPluginFactory, registerPlugin<GenerateGraphToolPlugin>();)
K_EXPORT_PLUGIN(ToolsPluginFactory(aboutdata))

GenerateGraphToolPlugin::GenerateGraphToolPlugin(QObject* parent,  const QList<QVariant> & /* args*/)
:   ToolsPluginInterface(ToolsPluginFactory::componentData(), parent)
{

}

GenerateGraphToolPlugin::~GenerateGraphToolPlugin()
{

}

void GenerateGraphToolPlugin::run(Document *document) const
{
    if (document == 0) {
        document = DocumentManager::self().activeDocument();
    }
    QPointer<GenerateGraphWidget> dialog = new GenerateGraphWidget(document);
    dialog->exec();

    return;
}

#include "GenerateGraphToolsPlugin.moc"
