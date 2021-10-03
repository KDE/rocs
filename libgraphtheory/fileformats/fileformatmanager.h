/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef FILEFORMATMANAGER_H
#define FILEFORMATMANAGER_H

#include "fileformatinterface.h"
#include "typenames.h"
#include "graphtheory_export.h"
#include <QObject>
#include <QList>

class KPluginInfo;

namespace GraphTheory
{

class FileFormatManagerPrivate;

/** \class FileFormatManager
 * The FileFormatManager provides access to all graph file format plugins. The backend manager loads
 * plugins on creation. For loading, the path "$QT_PLUGIN_PATH/rocs/fileformats" is searched for all plugins.
 */
class GRAPHTHEORY_EXPORT FileFormatManager : public QObject
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

    FileFormatManager();

    ~FileFormatManager() override;

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
     * this extension, return value is 0. Instead of the extact ending, also a complete filename
     * can be given.
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

    static FileFormatManager * instance;

    const QScopedPointer<FileFormatManagerPrivate> d;
};
}

#endif
