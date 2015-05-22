/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#ifndef EDITORPLUGININTERFACE_H
#define EDITORPLUGININTERFACE_H

#include <QObject>
#include "typenames.h"
#include "graphtheory_export.h"

class QUrl;
class QDialog;

namespace GraphTheory
{

class EditorPluginInterfacePrivate;

/**
 * \class EditorPluginInterface
 */
class GRAPHTHEORY_EXPORT EditorPluginInterface : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructs an editor plugin.
     * @param componentData is description of the plugin
     * @param parent The parent object for the plugin.
     */
    explicit EditorPluginInterface(const QString &componentName, QObject *parent);

    virtual ~EditorPluginInterface();

    virtual void showDialog(GraphDocumentPtr document) = 0;

    /**
     * @return display name of plugin, if nothing is set returns componentName
     */
    virtual QString displayName();

    /**
     * set display name of plugin
     */
    virtual void setDisplayName(const QString &name);

private:
    EditorPluginInterfacePrivate * const d;
};
}

#endif
