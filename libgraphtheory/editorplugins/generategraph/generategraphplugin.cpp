/*
 *  Copyright 2011-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
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

K_PLUGIN_FACTORY_WITH_JSON( EditorPluginFactory,
                            "generategraphplugin.json",
                            registerPlugin<GenerateGraphPlugin>();)

class GraphTheory::GenerateGraphPluginPrivate
{
public:
    GenerateGraphPluginPrivate()
        : m_dialog(0)
    {
    }

    ~GenerateGraphPluginPrivate()
    {
        m_dialog->deleteLater();
    }
    QDialog *m_dialog;
};

GenerateGraphPlugin::GenerateGraphPlugin(QObject* parent,  const QList<QVariant> & /* args*/)
    : EditorPluginInterface("rocs_generategraphplugin", parent)
    , d(new GenerateGraphPluginPrivate)
{

}

GenerateGraphPlugin::~GenerateGraphPlugin()
{

}

void GenerateGraphPlugin::showDialog(GraphDocumentPtr document)
{
    if (!document) {
        qCCritical(GRAPHTHEORY_GENERAL) << "No valid graph document given, aborting.";
    }
    QPointer<GenerateGraphWidget> dialog = new GenerateGraphWidget(document);
    dialog->exec();
    return;
}

#include "generategraphplugin.moc"
