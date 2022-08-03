/*
 *  SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "editorplugins/editorplugininterface.h"

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

class AssignValuesPlugin : public EditorPluginInterface
{
    Q_OBJECT
public:
    AssignValuesPlugin(QObject *parent, const KPluginMetaData &data, const QVariantList &)
        : EditorPluginInterface(parent, data)
    {
    }

    void showDialog(GraphDocumentPtr document) override
    {
        if (!document) {
            qCCritical(GRAPHTHEORY_GENERAL) << "No valid graph document given, aborting.";
        }
        QPointer<AssignValuesWidget> dialog = new AssignValuesWidget(document);
        dialog->exec();
    }
};

K_PLUGIN_CLASS_WITH_JSON(AssignValuesPlugin, "assignvaluesplugin.json")

#include "assignvaluesplugin.moc"
