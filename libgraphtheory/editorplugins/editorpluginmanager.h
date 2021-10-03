/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef EDITORPLUGINMANAGER_H
#define EDITORPLUGINMANAGER_H

#include "editorplugininterface.h"
#include "typenames.h"
#include "graphtheory_export.h"
#include <QObject>
#include <QList>

class KPluginInfo;

namespace GraphTheory
{

class EditorPluginManagerPrivate;

/** \class EditorPluginManager
 * The EditorPluginManager provides access to all graph editor plugins. The backend manager loads
 * plugins on creation. For loading, the path "$QT_PLUGIN_PATH/rocs/fileformats" is searched for all plugins.
 */
class GRAPHTHEORY_EXPORT EditorPluginManager : public QObject
{
    Q_OBJECT
public:
    EditorPluginManager();

    ~EditorPluginManager() override;

    /**
     * Returns list of loaded plugins. Plugins are loaded with first call to \see self().
     *
     * \return list of plugin interfaces of loaded plugins
     */
    QList <EditorPluginInterface*> plugins() const;

private:
    /**
     * \internal
     * Clears list of plugins and reloads them.
     */
    void loadPlugins();

    static EditorPluginManager * instance;

    const QScopedPointer<EditorPluginManagerPrivate> d;
};
}

#endif
