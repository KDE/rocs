/*
 *  SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef TRANSFORMEDGESPLUGIN_H
#define TRANSFORMEDGESPLUGIN_H

#include "editorplugins/editorplugininterface.h"

class QObject;

namespace GraphTheory
{
class TransformEdgesPluginPrivate;

class TransformEdgesPlugin : public EditorPluginInterface
{
    Q_OBJECT

public:
    TransformEdgesPlugin(QObject *parent, const KPluginMetaData &data, const QVariantList &);
    ~TransformEdgesPlugin() override;
    void showDialog(GraphDocumentPtr document) override;

private:
    const QScopedPointer<TransformEdgesPluginPrivate> d;
};
}

#endif
