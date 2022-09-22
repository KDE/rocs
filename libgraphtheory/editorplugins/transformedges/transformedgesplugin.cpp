/*
 *  SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "editorplugins/editorplugininterface.h"

#include "edge.h"
#include "graphdocument.h"
#include "logging_p.h"
#include "node.h"
#include "transformedgeswidget.h"
#include "typenames.h"
#include <KPluginFactory>
#include <QDialog>

using namespace GraphTheory;

class TransformEdgesPlugin : public EditorPluginInterface
{
    Q_OBJECT
public:
    TransformEdgesPlugin(QObject *parent, const KPluginMetaData &data, const QVariantList &)
        : EditorPluginInterface(parent, data)
    {
    }

    void showDialog(GraphDocumentPtr document) override
    {
        if (!document) {
            qCCritical(GRAPHTHEORY_GENERAL) << "No valid graph document given, aborting.";
        }
        QPointer<TransformEdgesWidget> dialog = new TransformEdgesWidget(document);
        dialog->exec();
        return;
    }
};

K_PLUGIN_CLASS_WITH_JSON(TransformEdgesPlugin, "transformedgesplugin.json")

#include "transformedgesplugin.moc"
