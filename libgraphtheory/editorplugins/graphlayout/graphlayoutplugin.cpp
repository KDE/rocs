/*
 *  SPDX-FileCopyrightText: 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "editorplugins/editorplugininterface.h"

#include "graphlayoutwidget.h"
#include "typenames.h"
#include "graphdocument.h"
#include "logging_p.h"
#include <KPluginFactory>
#include <QDialog>

using namespace GraphTheory;

class GraphLayoutPlugin : public EditorPluginInterface
{
    Q_OBJECT
public:
    GraphLayoutPlugin(QObject *parent, const KPluginMetaData &data, const QVariantList &)
        : EditorPluginInterface(parent, data)
    {
    }

    void showDialog(GraphDocumentPtr document) override
    {
        if (!document) {
            qCCritical(GRAPHTHEORY_GENERAL) << "No valid graph document given, aborting.";
        }
        QPointer<GraphLayoutWidget> dialog = new GraphLayoutWidget(document);
        dialog->exec();
    }
};

K_PLUGIN_CLASS_WITH_JSON(GraphLayoutPlugin, "graphlayoutplugin.json")

#include "graphlayoutplugin.moc"
