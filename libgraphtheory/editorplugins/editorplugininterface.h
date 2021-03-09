/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
    const QScopedPointer<EditorPluginInterfacePrivate> d;
};
}

#endif
