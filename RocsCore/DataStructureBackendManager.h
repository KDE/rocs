/*
    This file is part of Rocs.
    Copyright 2010  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef DATASTRUCTUREBACKENDMANAGER_H
#define DATASTRUCTUREBACKENDMANAGER_H

#include <QObject>
#include <boost/scoped_ptr.hpp>
#include "RocsCoreExport.h"
#include "CoreTypes.h"

class QLayout;
class QGraphicsItem;
class KPluginInfo;
class DataStructure;
class Document;
class Data;
class Pointer;
class DataStructureBackendManagerPrivate;
class DataStructurePluginInterface;


/**
 * \class DataStructureBackendManager
 *
 * This singelton class provides access to all available data structure backends.
 */
class ROCSLIB_EXPORT DataStructureBackendManager
    : public QObject
{
    Q_OBJECT

public:
    /**
     * Returns self reference to backend manager. First call of this method initializes
     * data structure backend manager and loads plugins.
     *
     * \return self reference
     */
    static DataStructureBackendManager* self();

    /**
     * Returns list of loaded backends. Backends are loaded with first call to \see self().
     *
     * \return list of plugin interfaces of loaded backends
     */
    const QStringList backends() const;

    /**
     * Returns KPluginInfo object representing the plugin information for \p plugin.
     *
     * \param plugin the data structure plugin object
     * \return plugin information
     */
    KPluginInfo backendInfo(DataStructurePluginInterface* plugin) const;

    /**
     * Returns data structure backend identified by its identifier \p internalName.
     *
     * \param internalName the unique identifier for backend
     * \return the requested backend or '0' if backend is not found
     */
    DataStructurePluginInterface* backend(const QString& internalName) const;

    /**
     * TODO change this method: it is not clear how the parent document is registered; the creation
     *      functionality should only be located at the Document class.
     *
     * Create data structure with backend \p pluginName. If not backend is specified, the currently
     * active data structure backend is used, \see activeBackend().
     *
     * \param parent the Document that will hold the created data structure
     * \param pluginName the optional name of the data structure backend
     * \return the created data structure
     */
    DataStructurePtr createDataStructure(Document* parent, const QString& pluginName = QString());

    /**
     * TODO change this method: same comment as on createDataStructure()
     *
     * Changes the given data structure to use the currently active data structure plugin.
     *
     * \param dataStructure the data structure to be converted
     * \param parent
     */
    DataStructurePtr convertDataStructureToActiveBackend(DataStructurePtr dataStructure, Document* parent);

    /**
     * Returns the currently active data structure backend. The return value is only '0' if
     * \see DataStructureBackendManager::backends() returns an empty list.
     *
     * \return data structure backend or '0' if no backend exists
     */
    DataStructurePluginInterface* activeBackend() const;

    /**
     * Generates graphics item for represenation of \p data at graph scene based on data structure
     * backend of the parent document of \p data.
     *
     * \param data the data object
     * \return item associated with the data object
     */
    QGraphicsItem* dataItem(DataPtr data) const;

    /**
     * Generates graphics item for represenation of \p pointer at graph scene based on data structure
     * backend of the parent document of \p pointer.
     *
     * \param pointer the pointer object
     * \return item associated with the pointer object
     */
    QGraphicsItem* pointerItem(PointerPtr pointer) const;

    /**
     * Create widgets for backend specific data properties.
     * If no extra properties exist, return value is 0.
     */
    QLayout* dataExtraProperties(DataPtr data, QWidget* parent) const;

    /**
     * Create widgets for backend specific pointer properties.
     * If no extra properties exist, return value is 0.
     */
    QLayout* pointerExtraProperties(PointerPtr pointer, QWidget* parent) const;

    /**
     * Create widgets for backend specific data structure properties.
     * If no extra properties exist, return value is 0.
     */
    QLayout* dataStructureExtraProperties(DataStructurePtr dataStructure, QWidget* parent) const;

public Q_SLOTS:
    /**
     * Change the active backend. If backend is changed, the signal \see backendChanged() is emitted.
     * If \p pluginIdentifier does not denote a valid backend, the active backend is not changed.
     *
     * \param pluginIdentifier internal name of the data structure backend
     */
    void setBackend(const QString& pluginIdentifier);

Q_SIGNALS:
    /**
     * Emitted if the active backend is changed.
     */
    void backendChanged(const QString& pluginName);

private:
    /**
     * \internal
     * Private constructor, \see self().
     */
    DataStructureBackendManager();

    /**
     * Destructor.
     */
    ~DataStructureBackendManager();

    DataStructureBackendManager(const DataStructureBackendManager&);
    void operator=(const DataStructureBackendManager&);

    /**
     * \internal
     * Private loader for data structure backends, only to be called from static \see self() method.
     */
    void loadBackends();

    static DataStructureBackendManager instance;

    const boost::scoped_ptr<DataStructureBackendManagerPrivate> d;
};

#endif
