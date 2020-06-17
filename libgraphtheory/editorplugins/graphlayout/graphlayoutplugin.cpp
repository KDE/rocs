/*
 *  Copyright 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "graphlayoutplugin.h"
#include "graphlayoutwidget.h"
#include "typenames.h"
#include "graphdocument.h"
#include "logging_p.h"
#include <KPluginFactory>
#include <QDialog>

using namespace GraphTheory;

K_PLUGIN_FACTORY_WITH_JSON( EditorPluginFactory,
                            "graphlayoutplugin.json",
                            registerPlugin<GraphLayoutPlugin>();)

class GraphTheory::GraphLayoutPluginPrivate
{
public:
    GraphLayoutPluginPrivate()
        : m_dialog(0)
    {
    }

    ~GraphLayoutPluginPrivate()
    {
        m_dialog->deleteLater();
    }
    QDialog *m_dialog;
};

GraphLayoutPlugin::GraphLayoutPlugin(QObject* parent,  const QList<QVariant> & /* args*/)
    : EditorPluginInterface("rocs_graphlayoutplugin", parent)
    , d(new GraphLayoutPluginPrivate)
{

}

GraphLayoutPlugin::~GraphLayoutPlugin()
{

}

void GraphLayoutPlugin::showDialog(GraphDocumentPtr document)
{
    if (!document) {
        qCCritical(GRAPHTHEORY_GENERAL) << "No valid graph document given, aborting.";
    }
    QPointer<GraphLayoutWidget> dialog = new GraphLayoutWidget(document);
    dialog->exec();
    return;
}

#include "graphlayoutplugin.moc"
