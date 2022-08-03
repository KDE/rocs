/*
 *  SPDX-FileCopyrightText: 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef GRAPHLAYOUTPLUGIN_H
#define GRAPHLAYOUTPLUGIN_H

#include "editorplugins/editorplugininterface.h"

namespace GraphTheory
{
class GraphLayoutPlugin : public EditorPluginInterface
{
    Q_OBJECT

public:
    GraphLayoutPlugin(QObject *parent, const KPluginMetaData &data, const QVariantList &);
    void showDialog(GraphDocumentPtr document) override;
};
}

#endif
