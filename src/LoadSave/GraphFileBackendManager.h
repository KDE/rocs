/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GRAPHFILEBACKENDMANAGER_H
#define GRAPHFILEBACKENDMANAGER_H
#include <QList>

#include "GraphFilePluginInterface.h"
#include "rocslib_export.h"

class KPluginInfo;

/** \class GraphFileBackendManager
 * The GraphFileBackendMananger is a singleton class that provides backends for
 * graph file writing and reading. The backend manager loads dynamic and static backend plugins
 * on first access to \see GraphFileBackendMananger::self() and provides access to these backends
 * as \see GraphFilePluginInterface objects.
 */

class ROCSLIB_EXPORT GraphFileBackendManager: public QObject
{
    Q_OBJECT

public:
    /**
     * Returns self reference to backend manager. First call of this method initializes
     * file backend manager and loads plugins.
     * \return self reference
     */
    static GraphFileBackendManager * self();

    /**
     * Returns list of loaded backends. Backends are loaded with first call to \see self().
     * \return list of plugin interfaces of loaded backends
     */
    QList <GraphFilePluginInterface*> backends() const;

    /**
     * Returns an arbitrary loaded plugin that can handle extension \p ext. If no backend specifies
     * this extension, return value is 0.
     * \return backend to handle files with specified extension or 0 otherwise
    */
    GraphFilePluginInterface* backendByExtension(QString ext);

    /**
     * Returns the default backend used for serialization/loading of graph files. Use this if
     * the graph document shall be serialized in the default format.
     * \return plugin interface for the graph file backend
     */
    GraphFilePluginInterface* defaultBackend();

private:
    void loadBackends();

    GraphFileBackendManager();

    ~GraphFileBackendManager();

    static GraphFileBackendManager * instance;

    class GraphFileBackendManagerPrivate * d;
};

#endif // GRAPHFILEBACKENDMANAGER_H
