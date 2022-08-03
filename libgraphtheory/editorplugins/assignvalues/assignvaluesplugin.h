/*
 *  SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef ASSIGNVALUESPLUGIN_H
#define ASSIGNVALUESPLUGIN_H

#include "editorplugins/editorplugininterface.h"

class QObject;

namespace GraphTheory
{
class AssignValuesPluginPrivate;

class AssignValuesPlugin : public EditorPluginInterface
{
    Q_OBJECT

public:
    AssignValuesPlugin(QObject *parent, const KPluginMetaData &data, const QVariantList &);
    ~AssignValuesPlugin() override;
    void showDialog(GraphDocumentPtr document) override;

private:
    const QScopedPointer<AssignValuesPluginPrivate> d;
};
}

#endif
