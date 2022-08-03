/*
 *  SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "transformedgesplugin.h"
#include "transformedgeswidget.h"
#include "typenames.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include "logging_p.h"
#include <KPluginFactory>
#include <QDialog>

using namespace GraphTheory;

K_PLUGIN_CLASS_WITH_JSON(TransformEdgesPlugin, "transformedgesplugin.json")

TransformEdgesPlugin::TransformEdgesPlugin(QObject *parent, const KPluginMetaData &data, const QVariantList &)
    : EditorPluginInterface(parent, data)
{
}

void TransformEdgesPlugin::showDialog(GraphDocumentPtr document)
{
    if (!document) {
        qCCritical(GRAPHTHEORY_GENERAL) << "No valid graph document given, aborting.";
    }
    QPointer<TransformEdgesWidget> dialog = new TransformEdgesWidget(document);
    dialog->exec();
    return;
}

#include "transformedgesplugin.moc"
