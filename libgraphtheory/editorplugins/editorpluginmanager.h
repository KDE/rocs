/*
 *  Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#ifndef EDITORPLUGINMANAGER_H
#define EDITORPLUGINMANAGER_H

#include "editorplugininterface.h"
#include "typenames.h"
#include "libgraphtheoryexport.h"
#include <QObject>
#include <QList>

class KPluginInfo;

namespace GraphTheory
{

class EditorPluginManagerPrivate;

/** \class EditorPluginManager
 * The EditorPluginManager provides access to all graph editor plugins. The backend manager loads
 * plugins on creation. For loading, the path "$QT_PLUGIN_PATH/rocs/fileformats" is searched for all
 * plugins of ServiceType "rocs/plugins".
 */
class GRAPHTHEORY_EXPORT EditorPluginManager : public QObject
{
    Q_OBJECT
public:
    EditorPluginManager();

    ~EditorPluginManager();

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
