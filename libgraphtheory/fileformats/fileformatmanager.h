/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>

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

#include "fileformatinterface.h"
#include "typenames.h"
#include "libgraphtheoryexport.h"
#include <QObject>

class KPluginInfo;

namespace GraphTheory
{

class FileFormatManagerPrivate;

/** \class FileFormatManager
 * The FileFormatManager provides access to all graph file format plugins. The backend manager loads
 * plugins on creation.
 */
class GRAPHTHEORY_EXPORT FileFormatManager: public QObject
{
    Q_OBJECT
public:
    /**
     * Selector for graph file backend with specific capability.
     */
    enum PluginType {
        Import,
        Export
    };

    /**
     * Returns self reference to backend manager. First call of this method initializes
     * file backend manager and loads plugins.
     *
     * \return self reference
     */
    static FileFormatManager * self();

    /**
     * Returns list of loaded backends. Backends are loaded with first call to \see self().
     *
     * \return list of plugin interfaces of loaded backends
     */
    QList <FileFormatInterface*> backends() const;

    /**
     * Returns list of all loaded backends with specified capability (\see PluginType).
     * Backends are loaded with first call to \see self().
     *
     * \param type specifies capability of the plugin
     * \return list of plugin interfaces of loaded backends
     */
    QList <FileFormatInterface*> backends(PluginType type) const;

    /**
     * Returns an arbitrary loaded plugin that can handle extension \p ext. If no backend specifies
     * this extension, return value is 0.
     *
     * \param ext specifies the extension string
     * \return backend to handle files with specified extension or 0 otherwise
     */
    FileFormatInterface * backendByExtension(const QString &ext);

    /**
     * Returns the default backend used for serialization/loading of graph files. Use this if
     * the graph document shall be serialized in the default format.
     *
     * \return plugin interface for the graph file backend
     */
    FileFormatInterface * defaultBackend();

private:
    /**
     * \internal
     * Clears list of backends and reloads them from dynamic service trader.
     */
    void loadBackends();

    /**
     * \internal
     * Private constructor, \see self().
     */
    FileFormatManager();

    /**
     * Desctructor.
     */
    ~FileFormatManager();

    static FileFormatManager * instance;

    const QScopedPointer<FileFormatManagerPrivate> d;
};
}

#endif
