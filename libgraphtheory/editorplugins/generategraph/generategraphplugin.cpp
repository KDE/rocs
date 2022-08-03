/*
 *  SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "generategraphplugin.h"
#include "generategraphwidget.h"
#include "typenames.h"
#include "graphdocument.h"
#include "modifiers/valueassign.h"
#include "logging_p.h"
#include <KPluginFactory>
#include <QDialog>

using namespace GraphTheory;

K_PLUGIN_CLASS_WITH_JSON(GenerateGraphPlugin, "generategraphplugin.json")

GenerateGraphPlugin::GenerateGraphPlugin(QObject *parent, const KPluginMetaData &data, const QVariantList &)
    : EditorPluginInterface(parent, data)
{

}

void GenerateGraphPlugin::showDialog(GraphDocumentPtr document)
{
    if (!document) {
        qCCritical(GRAPHTHEORY_GENERAL) << "No valid graph document given, aborting.";
    }
    QPointer<GenerateGraphWidget> dialog = new GenerateGraphWidget(document);
    dialog->exec();
}

#include "generategraphplugin.moc"
