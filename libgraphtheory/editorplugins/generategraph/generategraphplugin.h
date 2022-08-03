/*
 *  SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef GENERATEGRAPHPLUGIN_H
#define GENERATEGRAPHPLUGIN_H

#include "editorplugins/editorplugininterface.h"

namespace GraphTheory
{
class GenerateGraphPlugin : public EditorPluginInterface
{
    Q_OBJECT

public:
    GenerateGraphPlugin(QObject *parent, const KPluginMetaData &data, const QVariantList &);
    void showDialog(GraphDocumentPtr document) override;
};
}

#endif
