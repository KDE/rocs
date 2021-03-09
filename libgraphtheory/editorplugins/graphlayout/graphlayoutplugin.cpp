/*
 *  SPDX-FileCopyrightText: 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
