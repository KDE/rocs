/*
 *  SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "assignvaluesplugin.h"
#include "assignvalueswidget.h"
#include "typenames.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include "logging_p.h"
#include <KPluginFactory>
#include <QString>
#include <QStringList>
#include <QDialog>

using namespace GraphTheory;

K_PLUGIN_CLASS_WITH_JSON(AssignValuesPlugin, "assignvaluesplugin.json")

class GraphTheory::AssignValuesPluginPrivate
{
public:
    AssignValuesPluginPrivate()
        : m_dialog(nullptr)
    {
    }

    ~AssignValuesPluginPrivate()
    {
        m_dialog->deleteLater();
    }
    QDialog *m_dialog;
};

AssignValuesPlugin::AssignValuesPlugin(QObject *parent, const KPluginMetaData &data, const QVariantList &)
    : EditorPluginInterface(parent, data)
    , d(new AssignValuesPluginPrivate)
{
}

AssignValuesPlugin::~AssignValuesPlugin()
{

}

void AssignValuesPlugin::showDialog(GraphDocumentPtr document)
{
    if (!document) {
        qCCritical(GRAPHTHEORY_GENERAL) << "No valid graph document given, aborting.";
    }
    QPointer<AssignValuesWidget> dialog = new AssignValuesWidget(document);
    dialog->exec();
    return;
}

#include "assignvaluesplugin.moc"
