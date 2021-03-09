/*
 *  SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef GENERATEGRAPHPLUGIN_H
#define GENERATEGRAPHPLUGIN_H

#include "editorplugins/editorplugininterface.h"

class QObject;

namespace GraphTheory
{
class GenerateGraphPluginPrivate;

class GenerateGraphPlugin : public EditorPluginInterface
{
    Q_OBJECT

public:
    GenerateGraphPlugin(QObject* parent, const QList< QVariant >&);
    virtual ~GenerateGraphPlugin();
    void showDialog(GraphDocumentPtr document) Q_DECL_OVERRIDE;

private:
    const QScopedPointer<GenerateGraphPluginPrivate> d;
};
}

#endif
